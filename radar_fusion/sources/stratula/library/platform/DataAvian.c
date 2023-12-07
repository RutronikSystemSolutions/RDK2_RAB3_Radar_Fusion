#include "DataAvian.h"

#include <fatal_error.h>
#include <impl/PlatformInterrupt.h>
#include <impl/PlatformSpi.h>
#include <impl/chrono.h>

#include <stdbool.h>
#include <stddef.h>
#include <string.h>


IData DataAvian = {
    .configure           = DataAvian_configure,
    .calibrationRequired = DataAvian_calibrationRequired,
    .calibrate           = DataAvian_calibrate,
    .start               = DataAvian_start,
    .stop                = DataAvian_stop,
    .getStatusFlags      = DataAvian_getStatusFlags,
    .registerCallback    = DataAvian_registerCallback,
};

static IData_callback m_callback                  = NULL;
static void *m_arg                                = NULL;
static IData_acquisitionStatusCallback m_statusCb = NULL;

#define DATA_QUEUE_ELEMENT_TYPE uint8_t
#include <platform/data_queue.h>

#define DATA_AVIAN_MAX_READOUTS (1)  // number of data readouts that can be performed for every IRQ
#define DATA_AVIAN_MAX_COUNT    (1)  // number of data interfaces supported

typedef struct
{
    DataQueue_t queue;
    volatile uint64_t timestamp;  // timestamp identifying when the data generation occurred
    volatile uint16_t pending;    // flag indicating that acquisition is pending
    volatile sr_t readoutError;   // readout return value (When no readout error occurred equals to E_SUCCESS, otherwise holds the error code).
    volatile bool running;        // flag indicating if acquisition is running
    const PlatformInterruptDefinition_t *irq;
    uint8_t devId;
    uint8_t wordSize;                               // number of bits per readout word
    uint16_t readouts[DATA_AVIAN_MAX_READOUTS][2];  // data acquisition readouts to be performed for every IRQ to obtain one slice
    uint16_t readoutEntries;                        //
    uint32_t sliceSize;                             // number of bytes per frame slice
    uint16_t aggregation;                           // frames may consist of 1 slice or be aggregated from multiple slices
    bool useQspi;
} DataAvian_t;
static DataAvian_t m_dataAvianArray[DATA_AVIAN_MAX_COUNT] = {{{0}}};


/** @brief Return true if a BGT FIFO overflow occurred
 *
 * @param [in]  gsr0    BGT GSR0 register
 * @retval      true    if a FIFO overflow occurred
 * @retval      false   if no FIFO overflow occurred
 */
static inline sr_t get_gsr0_error(uint8_t gsr0)
{
    /* Shows if FIFO overflow or underflow condition occurred.
     * The error will be cleared after the following resets:
     * FIFO reset, SW reset or HW reset
     */
    const uint8_t FOU_ERR = (1u << 3);
    if (gsr0 & FOU_ERR)
    {
        return E_OVERFLOW;
    }

    return E_SUCCESS;
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

static inline void frameCallback(uint8_t *data, uint32_t size, uint8_t channel, uint64_t timestamp)
{
    m_callback(m_arg, data, size, channel, timestamp);
}

static void DataAvian_dataReadCallback(void *arg)
{
    DataAvian_t *self  = arg;
    DataQueue_t *queue = &self->queue;

    /* update write index for the next data fetch */
    queue_updateWritePointer(queue, self->sliceSize);

    acquisitionStatus(false);
}

static void DataAvian_readData(DataAvian_t *self, uint8_t *data)
{
    /* Do nothing if data acquisition is not running.
     */
    if (!self->running)
    {
        return;
    }

    /* It cannot happen here that the queue is full. If the queue is full
     * this function will not get invoked until the queue is no longer full.
     */

    /* Prepares an SPI read burst.
     * First of all, prefix configuration word must be written.
     *
     * The prefix is a 32 bit word constructed as follows :
     * bits 24-31 : Burst command code : always 0xFF
     * bits 16-23 : Prefix Address << 1 (corresponds to R / W bit = 0) : needed for standard SPI mode
     * bits  8-15 : Infinite burst length (controlled by CS line) : always 0
     * bits  0-7  : Prefix Address : needed for quad SPI mode
     */
    const uint8_t burstPrefixAddr = (uint8_t)self->readouts[0][0];
    if (self->useQspi)
    {
        // TODO: read FSTAT to check for fifo overflow
        self->readoutError = PlatformSpi_writeQspi8(self->devId, sizeof(burstPrefixAddr), &burstPrefixAddr, true);
    }
    else
    {
        const uint8_t burstPrefix[4] =
            {
                0xFF,
                burstPrefixAddr << 1,
                0,
                0,
            };
        uint8_t bufIn[4];
        self->readoutError = PlatformSpi_transfer8(self->devId, sizeof(burstPrefix), burstPrefix, bufIn, true);
        if (self->readoutError != E_SUCCESS)
        {
            return;
        }
        /* Check if FIFO overflow or underflow condition occurred */
        self->readoutError = get_gsr0_error(bufIn[0]);
    }

    if (self->readoutError != E_SUCCESS)
    {
        return;  // return on error
    }

    /* Starts an SPI read burst.
     * Once completed, the predefined data callback gets invoked.
     *
     * It is intended to read also in the case of a BGT FIFO overflow.
     *
     * The pointer payload is not necessarily 32bit aligned. This is currently
     * also not needed as the DMA reads data byte by byte.
     */

    sr_t ret;
    const uint16_t burstCount = self->readouts[0][1];
    if (self->wordSize == 8)
    {
        ret = PlatformSpi_readBurstAsync8(self->devId, data, burstCount, DataAvian_dataReadCallback, self);
    }
    else if (self->wordSize == 12)
    {
        ret = PlatformSpi_readBurstAsync12(self->devId, (uint16_t *)(uintptr_t)data, burstCount, DataAvian_dataReadCallback, self);
    }
    else
    {
        ret = E_NOT_IMPLEMENTED;
    }
    if (ret != E_SUCCESS)
    {
        fatal_error(FATAL_ERROR_UNSPECIFIED);
    }

    acquisitionStatus(true);
}

static void DataAvian_newDataCallback(void *arg)
{
    DataAvian_t *self = (DataAvian_t *)arg;

    /* The timestamp is saved if it was not previously set. */
    if (!self->timestamp)
    {
        self->timestamp = chrono_ticks_to_microseconds(chrono_now());
    }

    if (self->pending)
    {
        /* A previous fetch request is already pending or currently being serviced
         * in the main loop (function DataAvian_run). Therefore the pending counter
         * shall be simply incremented, to avoid a race condition.
         */
        self->pending++;
        return;
    }

    DataQueue_t *queue            = &self->queue;
    const uint32_t writeAvailable = queue_writeAvailable(queue);
    const uint16_t readoutSize    = self->sliceSize;

    if (writeAvailable >= readoutSize)
    {
        uint8_t *data = queue_getWritePointer(queue);
        DataAvian_readData(self, data);
    }
    else
    {
        /* Queue is full, no space left for another package, so do *not* fetch data yet.
         * This means the BGT FIFO won't be emptied until a package is read from the queue
         * in the main loop (function DataAvian_run). If this takes too long the BGT FIFO
         * will have an overflow.
         */
        self->pending++;
    }
}

void DataAvian_run(void)
{
    if (!m_callback)
    {
        return;
    }

    for (unsigned int index = 0; index < DATA_AVIAN_MAX_COUNT; index++)
    {
        DataAvian_t *self = &m_dataAvianArray[index];

        if (!self->running)
        {
            continue;
        }

        if (self->readoutError != E_SUCCESS)
        {
            DataAvian_stop(index);
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

        uint8_t *data            = queue_getReadPointer(queue);
        const uint64_t timestamp = self->timestamp;
        /* Reset timestamp to indicate that timestamp for the next frame slice needs to be acquired.
         * This should be done as soon as possible, since an interrupt can occur during the callback.
         */
        self->timestamp = 0;
        frameCallback(data, frameSize, index, timestamp);

        /* Update the queue's read position, which also frees the memory of the consumed data */
        queue_updateReadPointer(queue, frameSize);

        /* If the queue was full, a data fetch request might be pending,
         * hence we invoke it here now that queue space has been freed.
         */
        if (self->pending)
        {
            uint8_t *ptr = queue_getWritePointer(queue);
            DataAvian_readData(self, ptr);
            self->pending--;
        }
    }
}

sr_t DataAvian_calibrationRequired(uint8_t index, double dataRate, bool *isRequired)
{
    *isRequired = false;
    return E_SUCCESS;
}

sr_t DataAvian_calibrate(uint8_t index)
{
    return E_NOT_AVAILABLE;
}

sr_t DataAvian_configure(uint8_t index, const IDataProperties_t *dataProperties, const uint8_t *settings, uint16_t settingsSize)
{
    /* Configures the readout parameters for data to be read from the device.
     *
     * Internally, this method configures the IRQ handler triggered by the
     * front end device, which starts an SPI burst transfer to read data into
     * a local buffer.
     *
     * The burst transfer is always initiated with the provided prefix address
     * and always reads the specified number of 12-bit data samples.
     *
     * Data samples are unpacked into 16-bit words or packed pairwise into 3 bytes,
     * according to the format specified in data properties structure.
     */

    if (index >= DATA_AVIAN_MAX_COUNT)
    {
        return E_OUT_OF_BOUNDS;
    }

    DataAvian_stop(index);

    DataAvian_t *self    = &m_dataAvianArray[index];
    self->devId          = index;
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
    if (readoutEntries > DATA_AVIAN_MAX_READOUTS)
    {
        return E_INVALID_SIZE;
    }
    memcpy(self->readouts, settings, (readoutEntries * readoutEntrySize));

    // parse and validate readout entries
    const uint16_t i = 0;  // TODO: implement support for multiple entries
    //for (uint16_t i = 0; i < avian->readoutEntries; i++)
    const uint8_t readoutAddress = self->readouts[i][0];
    uint16_t readoutCount        = self->readouts[i][1];
    if (readoutAddress > UINT8_MAX)
    {
        return E_INVALID_PARAMETER;
    }
    if ((readoutCount == 0) || (readoutCount % 2))
    {
        return E_INVALID_PARAMETER;
    }
    switch (dataProperties->format)
    {
        case DataFormat_Auto:
        case DataFormat_Packed12:                           // default
            readoutCount         = (readoutCount * 3 / 2);  // two 12-bit samples are packed into three 8-bit words
            self->readouts[i][1] = readoutCount;            // update readout entry used by the ISR
            self->wordSize       = 8;                       // device data is read "as it is" and stored unmodified into three 8-bit words
            self->sliceSize      = readoutCount;            // set number of words per frame or frame slice in bytes
            break;
        case DataFormat_Raw16:
            // this configuration requires 16-bit aligned buffer
            {
                void *buffer = self->queue.buffer;
                if (buffer != (uint16_t *)buffer)
                {
                    return E_NOT_POSSIBLE;
                }
            }

            // one 12-bit sample is unpacked into one 16-bit word
            if (self->useQspi)
            {
                return E_NOT_IMPLEMENTED;
            }
            self->wordSize  = 12;                               // device data is read as 12-bit and stored as 16-bit
            self->sliceSize = readoutCount * sizeof(uint16_t);  // set number of words per frame or frame slice in bytes
            break;
        default:
            return E_INVALID_PARAMETER;
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

sr_t DataAvian_start(uint8_t index)
{
    if (index >= DATA_AVIAN_MAX_COUNT)
    {
        return E_OUT_OF_BOUNDS;
    }

    DataAvian_t *self = &m_dataAvianArray[index];
    if (self->readoutEntries == 0)
    {
        return E_NOT_CONFIGURED;
    }

    queue_reset(&self->queue);

    // Initialize the timestamp to 0, so it will be set when a data interrupt occurs
    self->timestamp = 0;

    self->pending      = 0;
    self->readoutError = E_SUCCESS;
    self->running      = true;
    PlatformInterrupt_enable(self->irq, true);  // enable IRQ

    return E_SUCCESS;
}

sr_t DataAvian_stop(uint8_t index)
{
    if (index >= DATA_AVIAN_MAX_COUNT)
    {
        return E_OUT_OF_BOUNDS;
    }

    DataAvian_t *self = &m_dataAvianArray[index];
    PlatformInterrupt_enable(self->irq, false);  // disable IRQ
    self->running = false;
    acquisitionStatus(false);

    return E_SUCCESS;
}

sr_t DataAvian_getStatusFlags(uint8_t index, uint32_t *flags)
{
    // TODO: implement if applicable.
    *flags = 0;
    return E_SUCCESS;
}

sr_t DataAvian_registerCallback(IData_callback callback, void *arg)
{
    m_callback = callback;
    m_arg      = arg;

    return E_SUCCESS;
}

void DataAvian_setBuffer(uint8_t index, uint8_t *buffer, uint32_t bufferSize)
{
    if (index >= DATA_AVIAN_MAX_COUNT)
    {
        fatal_error(FATAL_ERROR_DATA_CONFIG_FAILED);
    }

    DataAvian_t *self = &m_dataAvianArray[index];

    queue_initialize(&self->queue, buffer, bufferSize);

    // reset configuration due to changed buffer
    self->readoutEntries = 0;
}

void DataAvian_initialize(uint8_t index, const PlatformInterruptDefinition_t *irq, bool useQspi)
{
    if (index >= DATA_AVIAN_MAX_COUNT)
    {
        fatal_error(FATAL_ERROR_DATA_CONFIG_FAILED);
    }

    DataAvian_t *self = &m_dataAvianArray[index];
    self->irq         = irq;
    self->useQspi     = useQspi;

    const sr_t ret = PlatformInterrupt_registerCallback(irq, DataAvian_newDataCallback, self);
    if (ret != E_SUCCESS)
    {
        fatal_error(FATAL_ERROR_DATA_CONFIG_FAILED);
    }
}

void DataAvian_Constructor(IData_acquisitionStatusCallback statusCb)
{
    m_statusCb = statusCb;
}
