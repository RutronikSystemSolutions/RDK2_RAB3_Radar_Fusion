/**
 * \addtogroup      RegistersAvian
 * \brief
 * Implementation of radar component registers access
 * @{
 */
#ifndef REGISTERS_AVIAN_H
#define REGISTERS_AVIAN_H 1

#include <components/Registers8_32.h>
#include <components/interfaces/IProtocolAvian.h>


typedef struct
{
    Registers8_32 b_Registers;

    IProtocolAvian *m_protocol;
} RegistersAvian;


void RegistersAvian_Constructor(RegistersAvian *self, IProtocolAvian *protocol);

#endif /* REGISTERS_AVIAN_H */

/** @} */
