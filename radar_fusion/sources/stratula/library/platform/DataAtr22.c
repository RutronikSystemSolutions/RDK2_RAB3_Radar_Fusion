#include "DataAtr22.h"

#include <fatal_error.h>
#include <impl/PlatformI2c.h>
#include <impl/PlatformInterrupt.h>
#include <impl/chrono.h>

#include <stdbool.h>
#include <stddef.h>
#include <string.h>


IData DataAtr22 = {
    .configure           = DataAtr22_configure,
    .calibrationRequired = DataAtr22_calibrationRequired,
    .calibrate           = DataAtr22_calibrate,
    .start               = DataAtr22_start,
    .stop                = DataAtr22_stop,
    .getStatusFlags      = DataAtr22_getStatusFlags,
    .registerCallback    = DataAtr22_registerCallback,
};

static IData_callback m_callback                  = NULL;
static void *m_arg                                = NULL;
static IData_acquisitionStatusCallback m_statusCb = NULL;

#define DATA_QUEUE_ELEMENT_TYPE uint16_t
#include <platform/data_queue.h>

#define DATA_ATR22_MAX_READOUTS (8)  // number of data readouts that can be performed for every IRQ
#define DATA_ATR22_MAX_COUNT    (4)  // number of data interfaces supported

typedef struct
{
    DataQueue_t queue;
    volatile uint64_t timestamp;  // timestamp identifying when the data generation occurred
    volatile uint16_t pending;    // flag indicating that acquisition is pending
    volatile sr_t readoutError;   // readout error value (When no readout error occurred equals to E_SUCCESS, otherwise holds the error code).
    volatile bool running;        // flag indicating if acquisition is running
    const PlatformInterruptDefinition_t *irq;
    IProtocolAtr22 *protocol;
    uint16_t readouts[DATA_ATR22_MAX_READOUTS][2];  // data acquisition readouts to be performed for every IRQ to obtain one slice
    uint16_t readoutEntries;                        //
    uint16_t alternateReadout;                      // Alternate readoutentry atr22 read command
    uint16_t firstReadout;                          // First readoutentry atr22 read command
    uint32_t sliceSize;                             // number of words per frame slice
    uint16_t aggregation;                           // frames may consist of 1 slice or be aggregated from multiple slices
} DataAtr22_t;
static DataAtr22_t m_dataAtr22Array[DATA_ATR22_MAX_COUNT] = {{{0}}};


static inline void swapReadout(DataAtr22_t *self)
{
    const uint16_t tmp     = self->alternateReadout;
    self->alternateReadout = self->readouts[0][0];
    self->readouts[0][0]   = tmp;
}

static inline void acquisitionStatus(bool state)
{
    if (m_statusCb)
    {
        m_statusCb(state);
    }
}

static inline void errorCallback(uint32_t code, uint8_t channel, uint64_t timestamp)
{
    m_callback(m_arg, NULL, code, channel, timestamp);
}

static inline void frameCallback(uint16_t *data, uint32_t size, uint8_t channel, uint64_t timestamp)
{
    uint8_t *payload      = (uint8_t *)(uintptr_t)data;
    const uint32_t length = size * sizeof(*data);
    m_callback(m_arg, payload, length, channel, timestamp);
}

static void DataAtr22_dataReadCallback(void *arg)
{
    DataAtr22_t *self  = arg;
    DataQueue_t *queue = &self->queue;

    /* update write index for the next data fetch */
    queue_updateWritePointer(queue, self->sliceSize);

    acquisitionStatus(false);
}

static void DataAtr22_readData(DataAtr22_t *self, uint16_t *data)
{
    /* Do nothing if data acquisition is not running.
     */
    if (!self->running)
    {
        return;
    }

    acquisitionStatus(true);

    /* Perform synchronous data readout. Depending on the settings,
     * this might consist of multiple readouts.
     */
    for (uint16_t i = 0; i < self->readoutEntries; i++)
    {
        const uint16_t command = self->readouts[i][0];
        const uint16_t count   = self->readouts[i][1];

        self->readoutError = ProtocolAtr22_executeRead(self->protocol, command, count, data);
        if (self->readoutError != E_SUCCESS)
        {
            // an error occurred, so we do not continue and let the run function handle it
            return;
        }

        data += count;
    }

    swapReadout(self);  // update alternate readout setting (has no effect if alternate is same as primary)
    DataAtr22_dataReadCallback(self);
}

static void DataAtr22_newDataCallback(void *arg)
{
    /* Data will be fetched by the main loop function DataAtr22_checkReadData() */
    DataAtr22_t *self = (DataAtr22_t *)arg;
    self->pending++;

    /* The timestamp is saved if it was not previously set. */
    if (!self->timestamp)
    {
        self->timestamp = chrono_ticks_to_microseconds(chrono_now());
    }
}

static void DataAtr22_checkReadData(DataAtr22_t *self)
{
    if (!self->pending)
    {
        // No pending request to read data
        return;
    }

    if (self->pending > 1)
    {
        /* Multiple readouts are pending, since they were not serviced on time.
         * This means, device data is lost, which is signaled by an error.
         */
        self->readoutError = E_OVERFLOW;
        return;
    }

    DataQueue_t *queue            = &self->queue;
    const uint32_t writeAvailable = queue_writeAvailable(queue);
    const uint16_t readoutSize    = self->sliceSize;

    if (writeAvailable >= readoutSize)
    {
        uint16_t *data = queue_getWritePointer(queue);
        DataAtr22_readData(self, data);
    }
    else
    {
        /* Queue is full, so there is no memory left for data to be fetched.
         * This means, device data is lost, which is signaled by an error.
         */
        self->readoutError = E_OVERFLOW;
    }
}

void DataAtr22_run(void)
{
    if (!m_callback)
    {
        return;
    }

    for (unsigned int index = 0; index < DATA_ATR22_MAX_COUNT; index++)
    {
        DataAtr22_t *self = &m_dataAtr22Array[index];

        if (!self->running)
        {
            continue;
        }

        if (self->readoutError != E_SUCCESS)
        {
            DataAtr22_stop(index);
            errorCallback(self->readoutError, index, self->timestamp);
            continue;
        }

        /* New data might be ready to be fetched */
        DataAtr22_checkReadData(self);

        DataQueue_t *queue        = &self->queue;
        const uint32_t queueCount = queue_readAvailable(queue);
        const uint32_t frameSize  = self->sliceSize * self->aggregation;
        if (queueCount < frameSize)
        {
            // complete frame is not yet available
            continue;
        }
        else
        {
            // A frame reading was completed
            self->pending--;
        }

        uint16_t *data           = queue_getReadPointer(queue);
        const uint64_t timestamp = self->timestamp;
        /* Reset timestamp to indicate that timestamp for the next frame slice needs to be acquired.
         * This should be done as soon as possible, since an interrupt can occur during the callback.
         */
        self->timestamp = 0;
        frameCallback(data, frameSize, index, timestamp);

        /* Update the queue's read position, which also frees the memory of the consumed data */
        queue_updateReadPointer(queue, frameSize);

        /* If the queue was full, a data fetch request might be pending */
        DataAtr22_checkReadData(self);
    }
}

sr_t DataAtr22_calibrationRequired(uint8_t index, double dataRate, bool *isRequired)
{
    *isRequired = false;
    return E_SUCCESS;
}

sr_t DataAtr22_calibrate(uint8_t index)
{
    return E_NOT_AVAILABLE;
}

sr_t DataAtr22_configure(uint8_t index, const IDataProperties_t *dataProperties, const uint8_t *settings, uint16_t settingsSize)
{
    /* Configures the readout parameters for data to be read from the device.
     */

    if (index >= DATA_ATR22_MAX_COUNT)
    {
        return E_OUT_OF_BOUNDS;
    }

    DataAtr22_stop(index);

    DataAtr22_t *self    = &m_dataAtr22Array[index];
    self->readoutEntries = 0;  // disable configuration

    const uint16_t readoutEntrySize = sizeof(*self->readouts);
    if ((settingsSize % readoutEntrySize) || (settings == NULL))
    {
        return E_INVALID_SIZE;
    }

    /* The last entry might contain an optional aggregation setting.
     * This is indicated by field [1] (readout count) equal to zero.
     * In such case, field [0] contains the setting.
     */
    uint16_t readoutEntries   = settingsSize / readoutEntrySize;
    const uint16_t *lastEntry = (const uint16_t *)(uintptr_t)&settings[settingsSize - readoutEntrySize];
    if (lastEntry[1] == 0)
    {
        self->aggregation = lastEntry[0] + 1;
        readoutEntries--;
    }
    else
    {
        self->aggregation = 1;  // no aggregation (default)
    }

    const uint16_t *firstEntry = (const uint16_t *)(uintptr_t)&settings[0];
    self->alternateReadout     = ATR22_READ(firstEntry[0]);
    /* if the first entry has a length of zero,
     * it is meant to be an alternate readout setting for every other frame.
     * In this case, we remove the first entry from the settings and save its content.
     * Otherwise, we just duplicate the first entry, effectively switching always between
     * the same entry, so effectively without alternation.
     * We also have to set the first readout below to know where to start anew.
     */
    if (firstEntry[1] == 0)
    {
        settings += readoutEntrySize;
        settingsSize -= readoutEntrySize;
    }

    // a local copy of the readout entries is needed by the ISR
    if (readoutEntries > DATA_ATR22_MAX_READOUTS)
    {
        return E_INVALID_SIZE;
    }
    memcpy(self->readouts, settings, (readoutEntries * readoutEntrySize));

    // parse and validate readout entries
    self->sliceSize = 0;
    for (uint16_t i = 0; i < readoutEntries; i++)
    {
        const uint16_t readoutAddress = self->readouts[i][0];
        const uint16_t readoutCount   = self->readouts[i][1];
        if (readoutCount == 0)
        {
            return E_INVALID_PARAMETER;
        }

        self->readouts[i][0] = ATR22_READ(readoutAddress);  // convert readout address into read command
        self->sliceSize += readoutCount;                    // add current entry to frame size
    }

    self->firstReadout = self->readouts[0][0];

    // Configure the queue size to ensure that the frame storage is sequential/contiguous.
    DataQueue_t *queue = &self->queue;
    if (!queue_configure(queue, self->sliceSize * self->aggregation))
    {
        // return overflow error if the queue can not store the configured contiguous size
        return E_OVERFLOW;
    }

    self->readoutEntries = readoutEntries;  // enable configuration
    return E_SUCCESS;
}

sr_t DataAtr22_start(uint8_t index)
{
    if (index >= DATA_ATR22_MAX_COUNT)
    {
        return E_OUT_OF_BOUNDS;
    }

    DataAtr22_t *self = &m_dataAtr22Array[index];
    if (self->readoutEntries == 0)
    {
        return E_NOT_CONFIGURED;
    }

    queue_reset(&self->queue);

    // if the primary readout address is not the first, restore it by swapping
    if (self->readouts[0][0] != self->firstReadout)
    {
        swapReadout(self);
    }

    self->pending      = 0;
    self->readoutError = E_SUCCESS;
    // Initialize the timestamp to 0, so it will be set when a data interrupt occurs
    self->timestamp = 0;
    self->running   = true;
    PlatformInterrupt_enable(self->irq, true);  // enable IRQ

    return E_SUCCESS;
}

sr_t DataAtr22_stop(uint8_t index)
{
    if (index >= DATA_ATR22_MAX_COUNT)
    {
        return E_OUT_OF_BOUNDS;
    }

    DataAtr22_t *self = &m_dataAtr22Array[index];
    PlatformInterrupt_enable(self->irq, false);  // disable IRQ
    self->running = false;
    acquisitionStatus(false);

    return E_SUCCESS;
}

sr_t DataAtr22_getStatusFlags(uint8_t index, uint32_t *flags)
{
    // TODO: implement if applicable.
    *flags = 0;
    return E_SUCCESS;
}

sr_t DataAtr22_registerCallback(IData_callback callback, void *arg)
{
    m_callback = callback;
    m_arg      = arg;

    return E_SUCCESS;
}

void DataAtr22_setBuffer(uint8_t index, uint16_t *buffer, uint32_t bufferSize)
{
    if (index >= DATA_ATR22_MAX_COUNT)
    {
        fatal_error(FATAL_ERROR_DATA_CONFIG_FAILED);
    }

    DataAtr22_t *self = &m_dataAtr22Array[index];

    queue_initialize(&self->queue, buffer, bufferSize);

    // reset configuration due to changed buffer
    self->readoutEntries = 0;
}

void DataAtr22_initialize(uint8_t index, IProtocolAtr22 *protocol, const PlatformInterruptDefinition_t *irq)
{
    if (index >= DATA_ATR22_MAX_COUNT)
    {
        fatal_error(FATAL_ERROR_DATA_CONFIG_FAILED);
    }

    DataAtr22_t *self = &m_dataAtr22Array[index];

    self->protocol = protocol;  // interface used to fetch data
    self->irq      = irq;

    sr_t ret = PlatformInterrupt_registerCallback(irq, DataAtr22_newDataCallback, self);
    if (ret != E_SUCCESS)
    {
        fatal_error(FATAL_ERROR_DATA_CONFIG_FAILED);
    }
}

void DataAtr22_Constructor(IData_acquisitionStatusCallback statusCb)
{
    m_statusCb = statusCb;
}