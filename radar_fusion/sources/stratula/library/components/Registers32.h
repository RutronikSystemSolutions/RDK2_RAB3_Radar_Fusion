/**
 * \internal
 * \addtogroup      Registers
 * \brief
 * Base implementation of device registers access
 * \endinternal
 * @{
 */
#ifndef REGISTERS_32_H
#define REGISTERS_32_H 1


#include <components/interfaces/IRegisters32.h>


typedef struct _Registers32 Registers32;
struct _Registers32
{
    IRegisters32 b_IRegisters;

    uint32_t m_increment;
};


void Registers32_Constructor(Registers32 *self, RegType increment);

#endif /* REGISTERS_32_H */

/** @} */
