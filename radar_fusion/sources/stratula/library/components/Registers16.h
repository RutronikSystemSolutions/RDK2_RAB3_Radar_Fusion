/**
 * \internal
 * \addtogroup      Registers
 * \brief
 * Base implementation of device registers access
 * \endinternal
 * @{
 */
#ifndef REGISTERS_16_H
#define REGISTERS_16_H 1


#include <components/interfaces/IRegisters16.h>


typedef struct _Registers16 Registers16;
struct _Registers16
{
    IRegisters16 b_IRegisters;

    uint16_t m_increment;
};


void Registers16_Constructor(Registers16 *self, RegType increment);

#endif /* REGISTERS_16_H */

/** @} */
