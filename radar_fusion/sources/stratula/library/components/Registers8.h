/**
 * \internal
 * \addtogroup      Registers
 * \brief
 * Base implementation of device registers access
 * \endinternal
 * @{
 */
#ifndef REGISTERS_8_H
#define REGISTERS_8_H 1


#include <components/interfaces/IRegisters8.h>


typedef struct _Registers8 Registers8;
struct _Registers8
{
    IRegisters8 b_IRegisters;

    uint8_t m_increment;
};


void Registers8_Constructor(Registers8 *self, RegType increment);

#endif /* REGISTERS_8_H */

/** @} */
