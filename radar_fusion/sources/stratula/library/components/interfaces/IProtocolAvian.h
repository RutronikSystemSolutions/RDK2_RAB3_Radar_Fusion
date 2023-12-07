/**
 * \addtogroup      IProtocolAvian
 * \brief
 * Radar component command interface
 * @{
 */

#ifndef I_PROTOCOL_AVIAN_H
#define I_PROTOCOL_AVIAN_H 1

#include <common/errors.h>
#include <stdint.h>


#define I_PROTOCOL_AVIAN_WRITE_BIT      (1u << 0)
#define I_PROTOCOL_AVIAN_ADDRESS_OFFSET 1u
#define I_PROTOCOL_AVIAN_ADDRESS_WIDTH  7u
#define I_PROTOCOL_AVIAN_VALUE_WIDTH    24u

#define AVIAN_ADDRESS_MASK (((1u << I_PROTOCOL_AVIAN_ADDRESS_WIDTH) - 1) << I_PROTOCOL_AVIAN_ADDRESS_OFFSET);
#define AVIAN_VALUE_MASK   ((1u << I_PROTOCOL_AVIAN_VALUE_WIDTH) - 1)

#define AVIAN_READ(address)                                   \
    {                                                         \
        (address) << I_PROTOCOL_AVIAN_ADDRESS_OFFSET, 0, 0, 0 \
    }
#define AVIAN_WRITE(address, value)                                                                                                              \
    {                                                                                                                                            \
        ((address) << I_PROTOCOL_AVIAN_ADDRESS_OFFSET) | I_PROTOCOL_AVIAN_WRITE_BIT, ((value) >> 16) & 0xFF, ((value) >> 8) & 0xFF, (value)&0xFF \
    }


typedef struct _IProtocolAvian IProtocolAvian;
struct _IProtocolAvian
{
    /**
    * Executes a list of commands. If results is not NULL, the return values are provided.
    */
    sr_t (*execute)(IProtocolAvian *self, const uint8_t commands[][4], uint32_t count, uint32_t results[]);

    /**
    * Sets a mask of bits at a given address
    */
    sr_t (*setBits)(IProtocolAvian *self, uint8_t address, uint32_t bitMask);
};

#endif  // I_PROTOCOL_AVIAN_H
