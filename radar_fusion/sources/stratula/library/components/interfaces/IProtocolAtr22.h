/**
 * \addtogroup      IProtocolAtr22
 * \brief
 * Radar component command interface
 * @{
 */

#ifndef I_PROTOCOL_ATR22_H
#define I_PROTOCOL_ATR22_H 1

#include <common/errors.h>
#include <stdint.h>


#define I_PROTOCOL_ATR22_COMMAND_BIT (1u << 0)
#define I_PROTOCOL_ATR22_READ_BIT    (1u << 1)

#define ATR22_WRITE(address, value)                                                                          \
    {                                                                                                        \
        (address) >> 6, (((address) << 2) & 0xFF) | I_PROTOCOL_ATR22_COMMAND_BIT, (value) >> 8, (value)&0xFF \
    }
#define ATR22_READ(address) \
    ((address) << 2) | I_PROTOCOL_ATR22_READ_BIT | I_PROTOCOL_ATR22_COMMAND_BIT


typedef struct _IProtocolAtr22 IProtocolAtr22;
struct _IProtocolAtr22
{
    /**
    * Executes a list of write commands.
    */
    sr_t (*executeWrite)(IProtocolAtr22 *self, const uint8_t commands[][4], uint16_t count);

    /**
    * Executes a list of read commands.
    */
    sr_t (*executeRead)(IProtocolAtr22 *self, uint16_t command, uint16_t count, uint16_t values[]);

    /**
    * Sets a mask of bits at a given address
    */
    sr_t (*setBits)(IProtocolAtr22 *self, uint16_t address, uint16_t bitMask);
};

#endif  // I_PROTOCOL_ATR22_H
