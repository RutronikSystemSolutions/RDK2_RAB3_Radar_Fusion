/**
 * \addtogroup      IRadarAtr22
 * \brief
 * Radar component access interface
 * @{
 */

#ifndef IRADARATR22_H_
#define IRADARATR22_H_

#include <components/interfaces/IProtocolAtr22.h>
#include <components/interfaces/IRadar.h>
#include <components/interfaces/IRegisters16.h>

#define ATR22_DEFAULT_I2C_DEV_ADDR (0x33)

typedef struct
{
    uint16_t i2cDevAddr;
} RadarAtr22Config_t;

typedef struct
{
    uint16_t gpioEnableLdo;
    uint16_t gpioReset;
} RadarAtr22PinsConfig_t;

/**
 * \brief Access interface to a radar front end device of the Atr22 family
 */
typedef struct _IRadarAtr22 IRadarAtr22;
struct _IRadarAtr22
{
    IRadar b_IRadar;

    IRegisters16 *(*getIRegisters)(IRadarAtr22 *self);
    IProtocolAtr22 *(*getIProtocolAtr22)(IRadarAtr22 *self);
};


#endif /* IRADARATR22_H_ */