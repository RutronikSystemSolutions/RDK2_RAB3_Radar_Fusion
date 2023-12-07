/**
 * \addtogroup      IPinsLtr11.h
 * \brief
 * Radar component GPIO access interface
 * @{
 */

#ifndef IPINSLTR11_H_
#define IPINSLTR11_H_ 1

#include <common/errors.h>
#include <platform/interfaces/IGpio.h>
#include <stdbool.h>
#include <stdint.h>

#define IPINSLTR11_TARGET_DET (1u << 0)
#define IPINSLTR11_PHASE_DET  (1u << 1)

typedef struct
{
    uint16_t gpioReset;
    uint16_t gpioIrq;
    uint16_t targetDet;
    uint16_t phaseDet;
} IPinsLtr11Config_t;


typedef struct _IPinsLtr11 IPinsLtr11;
struct _IPinsLtr11
{
    /**
    * Sets state of radar reset pin.
    *
    * The reset signal of the connected Ltr11 device must be driven low
    * and kept low for at least 1ms, before going HIGH again.
    *
    * @param state true for high and false for low
    * @return Strata error code
    */
    sr_t (*setResetPin)(IPinsLtr11 *self, bool state);

    /**
    * Gets state of IRQ pin
    *
    * @return Strata error code
    */
    sr_t (*getIrqPin)(IPinsLtr11 *self, bool *state);

    /**
    * Gets state of targetDet and phaseDet pins
    *
    * @return Strata error code
    */
    sr_t (*getDetectionPins)(IPinsLtr11 *self, uint8_t *states);

    sr_t (*reset)(IPinsLtr11 *self);
};


#endif /* IPINSLTR11_H_ */

/** @} */