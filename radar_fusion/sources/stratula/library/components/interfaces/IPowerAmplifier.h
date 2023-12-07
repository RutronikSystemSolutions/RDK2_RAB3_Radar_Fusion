/**
 * \addtogroup      IPowerAmplifier
 * \brief
 * PowerAmplifier component access interface
 * @{
 */
#ifndef I_POWERAMPLIFIER_H
#define I_POWERAMPLIFIER_H 1

#include <components/interfaces/IRegisters16.h>

typedef struct
{
    uint8_t devId;  ///< control interface device id
} BoardPowerAmplifierConfig_t;

typedef struct _IPowerAmplifier IPowerAmplifier;
struct _IPowerAmplifier
{
    IRegisters16 *(*getIRegisters)(IPowerAmplifier *self);

    /**
     * Special implementation of the IPowerAmplifier interface
     * Must be set by constructor of the derived "class"
     */
    uint8_t m_bImplementation;
};


#endif /* I_POWERAMPLIFIER_H */

/** @} */
