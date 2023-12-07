#ifndef DATA_QUEUE_H_
#define DATA_QUEUE_H_ 1
#else
#error "data_queue.h can only be included once per compilation unit"
#endif /* DATA_QUEUE_H_ */

#ifndef DATA_QUEUE_ELEMENT_TYPE
#error "data_queue.h needs DATA_QUEUE_ELEMENT_TYPE defined"
#endif


typedef struct
{
    DATA_QUEUE_ELEMENT_TYPE *buffer;
    uint32_t bufferSize;  // maximum number of bytes in the buffer

    // current state
    uint32_t size;               // current queue size
    volatile uint32_t idxRead;   // index of next element to be read, only updated by reader
    volatile uint32_t idxWrite;  // index of next element to be written, only updated by writer
    volatile bool full;          // flag to differentiate between empty and full
} DataQueue_t;

static inline void queue_initialize(DataQueue_t *self, DATA_QUEUE_ELEMENT_TYPE *buffer, uint32_t bufferSize)
{
    self->buffer = buffer;
    if (buffer == NULL)
    {
        self->bufferSize = 0;
    }
    else
    {
        self->bufferSize = bufferSize;
    }
}

/* @brief Configure the queue size to ensure that the frame storage is sequential/contiguous.
     * @param [in]  self    Queue
     * @param [in]
     * @retval      true    if at least one contiguousSize block fits in the buffer.
     * @retval      false   otherwise.
    */
static inline bool queue_configure(DataQueue_t *self, uint32_t contiguousSize)
{
    self->size = self->bufferSize - (self->bufferSize % contiguousSize);
    return (self->size > 0);
}

static inline void queue_reset(DataQueue_t *self)
{
    // Initially the queue is empty
    self->full     = false;
    self->idxRead  = 0;
    self->idxWrite = 0;
}

/* This function returns the number of bytes that can be read contiguously from the queue (returns 0 if queue is empty). */
static inline uint32_t queue_readAvailable(DataQueue_t *self)
{
    if ((self->idxWrite == self->idxRead) && (!self->full))
    {
        return 0;
    }

    if (self->idxWrite > self->idxRead)
    {
        return self->idxWrite - self->idxRead;
    }

    return self->size - self->idxRead;
}

/* This function returns the number of bytes that can be contiguously written to the queue (returns 0 if queue is full). */
static inline uint32_t queue_writeAvailable(DataQueue_t *self)
{
    if ((self->idxWrite == self->idxRead) && (self->full))
    {
        return 0;
    }

    if (self->idxWrite < self->idxRead)
    {
        return self->idxRead - self->idxWrite;
    }

    return self->size - self->idxWrite;
}

static inline DATA_QUEUE_ELEMENT_TYPE *queue_getReadPointer(DataQueue_t *self)
{
    return &self->buffer[self->idxRead];
}

static inline DATA_QUEUE_ELEMENT_TYPE *queue_getWritePointer(DataQueue_t *self)
{
    return &self->buffer[self->idxWrite];
}

static inline void queue_updateReadPointer(DataQueue_t *self, uint32_t offset)
{
    self->idxRead = (self->idxRead + offset) % self->size;
    self->full    = false;
}

static inline void queue_updateWritePointer(DataQueue_t *self, uint32_t offset)
{
    self->full     = true;
    self->idxWrite = (self->idxWrite + offset) % self->size;
}
