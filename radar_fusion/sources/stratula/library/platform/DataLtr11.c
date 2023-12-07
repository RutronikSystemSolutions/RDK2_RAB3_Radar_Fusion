#include "DataLtr11.h"

#include <components/radar/ltr11/PinsLtr11.h>
#include <components/radar/ltr11/ProtocolLtr11.h>
#include <fatal_error.h>
#include <impl/chrono.h>

#include <stdbool.h>
#include <stddef.h>
#include <string.h>


IData DataLtr11 = {
    .configure           = DataLtr11_configure,
    .calibrationRequired = DataLtr11_calibrationRequired,
    .calibrate           = DataLtr11_calibrate,
    .start               = DataLtr11_start,
    .stop                = DataLtr11_stop,
    .getStatusFlags      = DataLtr11_getStatusFlags,
    .registerCallback    = DataLtr11_registerCallback,
};

static IData_callback m_callback                  = NULL;
static void *m_arg                                = NULL;
static IData_acquisitionStatusCallback m_statusCb = NULL;

#define DATA_QUEUE_ELEMENT_TYPE uint16_t
#include <platform/data_queue.h>

#define DATA_LTR11_MAX_READOUTS (4)  // number of data readouts that can be performed for every IRQ
#define DATA_LTR11_MAX_COUNT    (1)  // number of data interfaces supported

typedef struct
{
    DataQueue_t queue;
    volatile uint64_t timestamp;  // timestamp identifying when the data generation occurred
    volatile sr_t readoutError;   // readout return value (When no readout error occurred equals to E_SUCCESS, otherwise holds the error code)
    volatile bool running;        // flag indicating if acquisition is running
    IProtocolLtr11 *protocol;
    IPinsLtr11 *pins;
    uint16_t readouts[DATA_LTR11_MAX_READOUTS][2];  // data acquisition readouts to be performed for every IRQ to obtain one slice
    uint16_t readoutEntries;                        //
    uint32_t sliceSize;                             // number of words per frame slice
    uint16_t aggregation;                           // frames may consist of 1 slice or be aggregated from multiple slices
} DataLtr11_t;
static DataLtr11_t m_dataLtr11Array[DATA_LTR11_MAX_COUNT] = {{{0}}};


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

static void DataLtr11_dataReadCallback(void *arg)
{
    DataLtr11_t *self  = arg;
    DataQueue_t *queue = &self->queue;

    /* update write index for the next data fetch */
    queue_updateWritePointer(queue, self->sliceSize);

    acquisitionStatus(false);
}

static void DataLtr11_readData(DataLtr11_t *self, uint16_t *data)
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
        const uint16_t readoutAddress = self->readouts[i][0];
        const uint16_t readoutCount   = self->readouts[i][1];

        if (readoutAddress == DATA_LTR11_DETECTOR_OUTPUT_VIRTUAL_ADDR)
        {
            uint8_t states;
            self->readoutError = PinsLtr11_getDetectionPins(self->pins, &states);
            *data              = states;
        }
        else
        {
            const uint8_t command[2] = LTR11_READ_BURST(readoutAddress);
            self->readoutError       = ProtocolLtr11_executeReadBurst(self->protocol, command, readoutCount, data);
        }
        if (self->readoutError != E_SUCCESS)
        {
            // an error occurred, so we do not continue and let the run function handle it
            return;
        }

        data += readoutCount;
    }

    /* Async DMA is not used, because SPI access is controlled by miso arbitration.
     * For this reason, ready callback is invoked here.
     */
    DataLtr11_dataReadCallback(self);
}

static void DataLtr11_newDataCallback(void *arg)
{
    DataLtr11_t *self = (DataLtr11_t *)arg;

    /* The timestamp is saved if it was not previously set. */
    if (!self->timestamp)
    {
        self->timestamp = chrono_ticks_to_microseconds(chrono_now());
    }

    DataQueue_t *queue            = &self->queue;
    const uint32_t writeAvailable = queue_writeAvailable(queue);
    const uint16_t readoutSize    = self->sliceSize;

    if (writeAvailable >= readoutSize)
    {
        uint16_t *data = queue_getWritePointer(queue);
        DataLtr11_readData(self, data);
    }
    else
    {
        /* Queue is full, so there is no memory left for data to be fetched.
         * This means, device data is lost, which is signaled by an error.
         */
        self->readoutError = E_OVERFLOW;
    }
}

void DataLtr11_run(void)
{
    if (!m_callback)
    {
        return;
    }

    for (unsigned int index = 0; index < DATA_LTR11_MAX_COUNT; index++)
    {
        DataLtr11_t *self = &m_dataLtr11Array[index];

        if (!self->running)
        {
            continue;
        }

        if (self->readoutError != E_SUCCESS)
        {
            DataLtr11_stop(index);
            errorCallback(self->readoutError, index, self->timestamp);
            continue;
        }

        DataQueue_t *queue        = &self->queue;
        const uint32_t queueCount = queue_readAvailable(queue);
        const uint32_t frameSize  = self->sliceSize * self->aggregation;
        if (queueCount < frameSize)
        {
            // complete frame is not yet available
            continue;
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
    }
}

sr_t DataLtr11_calibrationRequired(uint8_t index, double dataRate, bool *isRequired)
{
    *isRequired = false;
    return E_SUCCESS;
}

sr_t DataLtr11_calibrate(uint8_t index)
{
    return E_NOT_AVAILABLE;
}

sr_t DataLtr11_configure(uint8_t index, const IDataProperties_t *dataProperties, const uint8_t *settings, uint16_t settingsSize)
{
    /* Configures the readout parameters for data to be read from the device.
     */

    if (index >= DATA_LTR11_MAX_COUNT)
    {
        return E_OUT_OF_BOUNDS;
    }

    DataLtr11_stop(index);

    DataLtr11_t *self    = &m_dataLtr11Array[index];
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

    // a local copy of the readout entries is needed by the ISR
    if (readoutEntries > DATA_LTR11_MAX_READOUTS)
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
        if ((readoutCount == 0) || ((readoutAddress == DATA_LTR11_DETECTOR_OUTPUT_VIRTUAL_ADDR) && (readoutCount != 1)))
        {
            return E_INVALID_PARAMETER;
        }
        self->sliceSize += readoutCount;  // add current entry to slice size
    }

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

sr_t DataLtr11_start(uint8_t index)
{
    if (index >= DATA_LTR11_MAX_COUNT)
    {
        return E_OUT_OF_BOUNDS;
    }

    DataLtr11_t *self = &m_dataLtr11Array[index];
    if (self->readoutEntries == 0)
    {
        return E_NOT_CONFIGURED;
    }

    queue_reset(&self->queue);

    // Initialize the timestamp to 0, so it will be set when a data interrupt occurs
    self->timestamp = 0;

    self->readoutError = E_SUCCESS;
    self->running      = true;

    return E_SUCCESS;
}

sr_t DataLtr11_stop(uint8_t index)
{
    if (index >= DATA_LTR11_MAX_COUNT)
    {
        return E_OUT_OF_BOUNDS;
    }

    DataLtr11_t *self = &m_dataLtr11Array[index];
    self->running     = false;
    acquisitionStatus(false);

    return E_SUCCESS;
}

sr_t DataLtr11_getStatusFlags(uint8_t index, uint32_t *flags)
{
    // TODO: implement if applicable.
    *flags = 0;
    return E_SUCCESS;
}

sr_t DataLtr11_registerCallback(IData_callback callback, void *arg)
{
    m_callback = callback;
    m_arg      = arg;

    return E_SUCCESS;
}

void DataLtr11_setBuffer(uint8_t index, uint16_t *buffer, uint32_t bufferSize)
{
    if (index >= DATA_LTR11_MAX_COUNT)
    {
        fatal_error(FATAL_ERROR_DATA_CONFIG_FAILED);
    }

    DataLtr11_t *self = &m_dataLtr11Array[index];

    queue_initialize(&self->queue, buffer, bufferSize);

    // reset configuration due to changed buffer
    self->readoutEntries = 0;
}

void DataLtr11_initialize(uint8_t index, IProtocolLtr11 *protocol, IPinsLtr11 *pins)
{
    if (index >= DATA_LTR11_MAX_COUNT)
    {
        fatal_error(FATAL_ERROR_DATA_CONFIG_FAILED);
    }

    DataLtr11_t *self = &m_dataLtr11Array[index];

    self->pins     = pins;      // interface used to get the detection pins state
    self->protocol = protocol;  // interface used to fetch data
    ProtocolLtr11_registerCallback(self->protocol, DataLtr11_newDataCallback, self);
}

void DataLtr11_Constructor(IData_acquisitionStatusCallback statusCb)
{
    m_statusCb = statusCb;
}
