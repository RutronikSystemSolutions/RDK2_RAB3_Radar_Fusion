/*
 * \addtogroup      RegistersLtr11.h
 * \brief
 * Implementation of radar component registers access
 * @{
 */


#ifndef REGISTERS_LTR11_H_
#define REGISTERS_LTR11_H_ 1

#include <components/Registers8_16.h>
#include <components/interfaces/IProtocolLtr11.h>


typedef struct
{
    Registers8_16 b_Registers;

    IProtocolLtr11 *m_protocol;
} RegistersLtr11;


void RegistersLtr11_Constructor(RegistersLtr11 *self, IProtocolLtr11 *protocol);

#endif /* REGISTERS_LTR11_H_ */

/** @} */