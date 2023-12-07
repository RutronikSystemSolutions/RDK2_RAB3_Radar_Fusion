#include <impl/PlatformI2c.h>
#include <platform/interfaces/II2c.h>


II2c PlatformI2c = {
    .getMaxTransfer       = PlatformI2c_getMaxTransfer,
    .configureBusSpeed    = PlatformI2c_configureBusSpeed,
    .clearBus             = PlatformI2c_clearBus,
    .pollForAck           = PlatformI2c_pollForAck,
    .writeWithoutPrefix   = PlatformI2c_writeWithoutPrefix,
    .writeWith8BitPrefix  = PlatformI2c_writeWith8BitPrefix,
    .writeWith16BitPrefix = PlatformI2c_writeWith16BitPrefix,
    .readWithoutPrefix    = PlatformI2c_readWithoutPrefix,
    .readWith8BitPrefix   = PlatformI2c_readWith8BitPrefix,
    .readWith16BitPrefix  = PlatformI2c_readWith16BitPrefix,
};
