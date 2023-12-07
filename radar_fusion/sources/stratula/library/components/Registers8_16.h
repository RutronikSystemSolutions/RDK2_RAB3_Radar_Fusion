/*
 * Registers8_16.h
 * \internal
 * \addtogroup      Registers
 * \brief
 * Base implementation of device registers access
 * \endinternal
 * @{
 */


#ifndef REGISTERS_8_16_H_
#define REGISTERS_8_16_H_ 1

#include <components/interfaces/IRegisters8_16.h>


typedef struct _Registers8_16 Registers8_16;
struct _Registers8_16
{
    IRegisters8_16 b_IRegisters;

    uint8_t m_increment;
};

void Registers8_16_Constructor(Registers8_16 *self, RegType increment);


#endif /* REGISTERS_8_16_H_ */

/** @} */
