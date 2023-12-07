/**
 * \internal
 * \addtogroup      Registers
 * \brief
 * Base implementation of device registers access
 * \endinternal
 * @{
 */
#ifndef REGISTERS_8_32_H
#define REGISTERS_8_32_H 1


#include <components/interfaces/IRegisters8_32.h>


typedef struct _Registers8_32 Registers8_32;
struct _Registers8_32
{
    IRegisters8_32 b_IRegisters;

    uint8_t m_increment;
};


void Registers8_32_Constructor(Registers8_32 *self, RegType increment);

#endif /* REGISTERS_8_32_H */

/** @} */
