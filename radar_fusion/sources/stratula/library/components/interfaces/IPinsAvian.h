/**
 * \addtogroup      IPinsAvian
 * \brief
 * Radar component GPIO access interface
 * @{
 */
#ifndef I_PINS_AVIAN_H
#define I_PINS_AVIAN_H 1

#include <common/errors.h>
#include <platform/interfaces/IGpio.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct
{
    uint16_t gpioReset;
    uint16_t gpioIrq;
} IPinsAvianConfig_t;


typedef struct _IPinsAvian IPinsAvian;
struct _IPinsAvian
{
    /**
    * Sets state of radar reset pin.
    *
    * Because the reset input of the Avian device is multiplexed with the quad
    * SPI data line DIO3, the SPI CS signal must be HIGH all time during a reset
    * condition.
    *
    * The reset signal of the connected Avian device must be driven low
    * and kept low for at least 1000ns, before going HIGH again.
    *
    * @param state true for high and false for low
    * @return Strata error code
    */
    sr_t (*setResetPin)(IPinsAvian *self, bool state);

    /**
    * Obtains state of radar IRQ pin.
    *
    * @return State of the pin, true for high and false for low
    */
    sr_t (*getIrqPin)(IPinsAvian *self, bool *state);

    sr_t (*reset)(IPinsAvian *self);
};


#endif /* I_PINS_AVIAN_H */

/** @} */
