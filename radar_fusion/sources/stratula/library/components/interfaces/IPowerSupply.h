/**
 * \addtogroup      IPowerSupply
 * \brief
 * Power supply component access interface
 * @{
 */
#ifndef I_POWERSUPPLY_H
#define I_POWERSUPPLY_H 1

#include <components/interfaces/IRegisters8.h>

typedef struct
{
    uint8_t devId;  ///< control interface device id
} BoardPowerConfig_t;


typedef struct _IPowerSupply IPowerSupply;
struct _IPowerSupply
{
    IRegisters8 *(*getIRegisters)(IPowerSupply *self);

    /**
     * Special implementation of the IPowerSupply interface
     * Must be set by constructor of the derived "class"
     */
    uint8_t m_bImplementation;
};


#endif /* I_POWERSUPPLY_H */

/** @} */
