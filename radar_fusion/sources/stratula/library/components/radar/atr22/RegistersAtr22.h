/**
 * \addtogroup      RegistersAtr22
 * \brief
 * Implementation of radar component registers access
 * @{
 */
#ifndef REGISTERS_ATR22_H
#define REGISTERS_ATR22_H 1

#include "ProtocolAtr22.h"
#include <components/Registers16.h>


typedef struct
{
    Registers16 b_Registers;

    IProtocolAtr22 *m_protocol;
} RegistersAtr22;


void RegistersAtr22_Constructor(RegistersAtr22 *self, IProtocolAtr22 *commands);


#endif /* REGISTERS_ATR22_H */

/** @} */
