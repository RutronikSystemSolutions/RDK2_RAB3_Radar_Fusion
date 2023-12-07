/**
 * \addtogroup      IProtocolLtr11.h
 * \brief
 * Radar component command interface
 * @{
 */

#ifndef IPROTOCOLLTR11_H_
#define IPROTOCOLLTR11_H_ 1

#include <common/errors.h>
#include <stdbool.h>
#include <stdint.h>

#define I_PROTOCOL_LTR11_VALUE_WIDTH    16u
#define I_PROTOCOL_LTR11_ADDRESS_WIDTH  7u
#define I_PROTOCOL_LTR11_ADDRESS_OFFSET (I_PROTOCOL_LTR11_VALUE_WIDTH + 1)
#define I_PROTOCOL_BURST_PREFIX_LTR11   0xFF
#define I_PROTOCOL_LTR11_WRITE_BIT      (1u << 0)

#define LTR11_READ(address) ((address) << 1)

#define LTR11_WRITE(address, value)                                               \
    {                                                                             \
        ((address) << 1) | I_PROTOCOL_LTR11_WRITE_BIT, (value) >> 8, (value)&0xFF \
    }

#define LTR11_READ_BURST(address)                     \
    {                                                 \
        I_PROTOCOL_BURST_PREFIX_LTR11, (address) << 1 \
    }

#define LTR11_WRITE_BURST(address)                                                   \
    {                                                                                \
        I_PROTOCOL_BURST_PREFIX_LTR11, ((address) << 1) | I_PROTOCOL_LTR11_WRITE_BIT \
    }

#define LTR11_WRITE_VALUE(value)   \
    {                              \
        (value) >> 8, (value)&0xFF \
    }

typedef void (*ReadoutCallback)(void *arg);

typedef struct _IProtocolLtr11 IProtocolLtr11;
struct _IProtocolLtr11
{
    /**
    * Executes a single write command.
    */
    sr_t (*executeWrite)(IProtocolLtr11 *self, const uint8_t command[3]);

    /**
    * Executes a single read command.
    */
    sr_t (*executeRead)(IProtocolLtr11 *self, uint8_t command, uint16_t *value);

    /**
    * Executes a batch write command.
    */
    sr_t (*executeWriteBatch)(IProtocolLtr11 *self, const uint8_t commands[][3], uint16_t count);

    /**
    * Executes a burst write command.
    */
    sr_t (*executeWriteBurst)(IProtocolLtr11 *self, const uint8_t command[2], uint16_t count, const uint8_t values[][2]);

    /**
    * Executes a burst read command.
    */
    sr_t (*executeReadBurst)(IProtocolLtr11 *self, const uint8_t command[2], uint16_t count, uint16_t values[]);

    /**
    * Sets a mask of bits at a given address.
    */
    sr_t (*setBits)(IProtocolLtr11 *self, uint8_t address, uint16_t bitMask);

    /**
    * Enables the Miso Arbitration if the given Pulse Repetition Time (PRT) is different than 0, and disables it if prt is equal to 0.
    */
    sr_t (*setMisoArbitration)(IProtocolLtr11 *self, uint16_t prt);

    /**
    * Registers Callback.
    */
    sr_t (*registerCallback)(IProtocolLtr11 *self, ReadoutCallback callback, void *arg);
};

#endif  // IPROTOCOLLTR11_H_
