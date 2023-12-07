/**
 * \addtogroup      PlatformGpio
 * @{
 */
#ifndef PLATFORM_GPIO_DEFINITION_H
#define PLATFORM_GPIO_DEFINITION_H 1

#include <stdint.h>
#include <universal/gpio_definitions.h>


typedef struct
{
    uint16_t gpio;
    uint8_t flags;
} PlatformGpioDefinition_t;

#endif /* PLATFORM_GPIO_DEFINITION_H */

/** @} */
