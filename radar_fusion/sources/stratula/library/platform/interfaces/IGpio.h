/**
 * \addtogroup      IGpio
 * \brief
 * This is a generic interface for supporting access to GPIOs.
 * They can be identified either by a number, which is hardware-specific,
 * or by a predefined name that is independent of the hardware used,
 * as long as it is defined in the given bridge implementation.
 * @{
 */
#ifndef IGPIO_H
#define IGPIO_H 1

#include <common/errors.h>
#include <stdbool.h>
#include <stdint.h>
#include <universal/gpio_definitions.h>


typedef struct
{
    /**
     * Configure a GPIO for pull-up/pull-down, drive-high, drive-low and/or input
     * @param id either a generic number or a predefined name
     * @param flags bit-wise setting of features
     * @return Strata error code
     */
    sr_t (*configurePin)(uint16_t id, uint8_t flags);

    /**
     * Set a GPIO to logic high or low.
     * @param id either a generic number or a predefined name
     * @param state true for high and false for low
     * @return Strata error code
     */
    sr_t (*setPin)(uint16_t id, bool state);

    /**
     * Get the current logic level of a GPIO (which should have been configured for input)
     * @param id either a generic number or a predefined name
     * @param state output parameter for the state of the pin, only valid if the function returned no error
     * @return Strata error code
     */
    sr_t (*getPin)(uint16_t id, bool *state);
} IGpio;

#endif /* IGPIO_H */

/** @} */
