/**
 * \addtogroup      ProtocolAvian
 * \brief
 * Implementation of radar command interface
 * @{
 */
#ifndef PROTOCOL_AVIAN_H
#define PROTOCOL_AVIAN_H 1

#include <components/interfaces/IProtocolAvian.h>
#include <platform/interfaces/ISpi.h>


typedef struct _ProtocolAvian ProtocolAvian;
struct _ProtocolAvian
{
    IProtocolAvian b_IProtocolAvian;

    ISpi *m_accessSpi;
    uint8_t m_devId;
};


void ProtocolAvian_Constructor(ProtocolAvian *self, ISpi *accessSpi, uint8_t devId);


sr_t ProtocolAvian_execute(IProtocolAvian *self, const uint8_t commands[][4], uint32_t count, uint32_t results[]);
sr_t ProtocolAvian_setBits(IProtocolAvian *self, uint8_t address, uint32_t bitMask);


#endif /* PROTOCOL_AVIAN_H */

/** @} */
