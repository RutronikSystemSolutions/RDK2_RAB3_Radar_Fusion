/**
 * \addtogroup      ProtocolAtr22
 * \brief
 * Implementation of radar command interface
 * @{
 */
#ifndef PROTOCOL_ATR22_H
#define PROTOCOL_ATR22_H 1

#include <components/interfaces/IProtocolAtr22.h>
#include <platform/interfaces/II2c.h>


typedef struct _ProtocolAtr22 ProtocolAtr22;
struct _ProtocolAtr22
{
    IProtocolAtr22 b_IProtocolAtr22;

    II2c *m_accessI2c;
    uint16_t m_devAddr;
};


void ProtocolAtr22_Constructor(ProtocolAtr22 *self, II2c *accessI2c, uint16_t devAddr);


sr_t ProtocolAtr22_executeWrite(IProtocolAtr22 *self, const uint8_t commands[][4], uint16_t count);
sr_t ProtocolAtr22_executeRead(IProtocolAtr22 *self, uint16_t command, uint16_t count, uint16_t values[]);
sr_t ProtocolAtr22_setBits(IProtocolAtr22 *self, uint16_t address, uint16_t bitMask);


#endif /* PROTOCOL_ATR22_H */

/** @} */
