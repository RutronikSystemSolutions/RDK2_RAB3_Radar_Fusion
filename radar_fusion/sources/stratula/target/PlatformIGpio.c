#include <impl/PlatformGpio.h>
#include <platform/interfaces/IGpio.h>


IGpio PlatformGpio = {
    .configurePin = PlatformGpio_configurePin,
    .setPin       = PlatformGpio_setPin,
    .getPin       = PlatformGpio_getPin,
};
