#include "Atr22.h"
#include <impl/thread.h>
#include <stddef.h>
#include <universal/components/implementations/radar.h>


#define SELF        ((Atr22 *)(uintptr_t)self)
#define REGISTERS   ((IRegisters16 *)&SELF->m_registers)
#define PROTOCOL    ((IProtocolAtr22 *)&SELF->m_protocol)
#define RADAR_ATR22 (&SELF->b_IRadarAvian)
#define RADAR       (&SELF->b_IRadarAtr22.b_IRadar)


// register definitions
static const uint16_t SEQ_MAIN_CONF  = 0x0300;
static const uint16_t swResetBitMask = 0x0002;
static const uint16_t I2C_CONF       = 0x010F;


IRegisters16 *Atr22_getIRegisters(IRadarAtr22 *self)
{
    return REGISTERS;
}

IProtocolAtr22 *Atr22_getIProtocolAtr22(IRadarAtr22 *self)
{
    return PROTOCOL;
}

/****************************************************************************
 * Public methods
 ****************************************************************************/
sr_t Atr22_reset(IRadar *self, bool softReset)
{
    if (softReset)
    {
        REGISTERS->setBits(REGISTERS, SEQ_MAIN_CONF, swResetBitMask);  // SEQ_MAIN_CONF.SW_RST = 1;
    }
    else
    {
        // Atr22 does not have a dedicated HW reset line
        return E_NOT_SUPPORTED;
    }

    SELF->m_initialized = false;

    return E_SUCCESS;
}

sr_t Atr22_getDataIndex(IRadar *self, uint8_t *index)
{
    // each Atr22 device uses 4 data indices
    return E_NOT_POSSIBLE;
}

sr_t Atr22_initialize(IRadar *self)
{
    if (SELF->m_initialized)
    {
        return E_SUCCESS;
    }

    SELF->m_initialized = true;

    return E_SUCCESS;
}

sr_t Atr22_configure(IRadar *self, const IfxRfe_MmicConfig *c)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Atr22_loadSequence(IRadar *self, IfxRfe_Sequence *s)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Atr22_calibrate(IRadar *self)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Atr22_startSequence(IRadar *self)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Atr22_startData(IRadar *self)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Atr22_stopData(IRadar *self)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Atr22_getRxChannels(IRadar *self, uint8_t *number)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Atr22_enableConstantFrequencyMode(IRadar *self, uint16_t txMask, float txPower)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Atr22_setConstantFrequency(IRadar *self, double frequency)
{
    return E_NOT_IMPLEMENTED;
}


sr_t Atr22_WorkaroundEs(IGpio *gpio, const RadarAtr22PinsConfig_t *pinsConfig)
{
    /* Explanation: Date 15_11_2021
	Problem: There is a HW issue that demands the 1.8 V to be supplied a bit later then 3.3 V.
	IC Database: "KSD008-404" : "Levelshifters undefined state during startup"
	HW solution: currently there is a transistor that needs to be enabled after enabling the 3.3 V (LDO) to delay the 1.8 V supply.
	The transistor is currently connected to RST line of the Sensor Connector. We do not have a dedicated RESET Pin on ATR22.
	SW solution: 
	-	First we have to disable the transistor through setting the RST line to high initially
	-	enable LDO
	-	enable the transistor through setting RST line to low
	*/

    /* Needs to be configured to initial high before enabling LDO (for the delay of 1.8V) */
    RETURN_ON_ERROR(gpio->configurePin(pinsConfig->gpioReset, GPIO_MODE_OUTPUT_PUSH_PULL | GPIO_FLAG_OUTPUT_INITIAL_HIGH));
    /* ATR22 does not have a dedicated HW reset line, LDO is used to hard reset */
    RETURN_ON_ERROR(gpio->setPin(pinsConfig->gpioEnableLdo, false));  // disable LDO
    this_thread_sleep_for(chrono_milliseconds(100));                  // time needed for LDO capacitor to discharge
    RETURN_ON_ERROR(gpio->setPin(pinsConfig->gpioEnableLdo, true));   // enable LDO
    RETURN_ON_ERROR(gpio->setPin(pinsConfig->gpioReset, false));      // set reset pin low to activate transistor for the 1.8 V delay
    this_thread_sleep_for(chrono_milliseconds(100));                  // time needed for LDO capacitor to recharge

    return E_SUCCESS;
}

void Atr22_Constructor(Atr22 *self, IData *accessData, II2c *accessI2c,
                       const RadarAtr22Config_t *radarConfig)
{
    self->b_IRadarAtr22.b_IRadar.m_bImplementation = COMPONENT_IMPL_RADAR_ATR22;

    ProtocolAtr22_Constructor(&self->m_protocol, accessI2c, radarConfig->i2cDevAddr);
    RegistersAtr22_Constructor(&self->m_registers, PROTOCOL);

    // IRadar (high level interface)
    self->b_IRadarAtr22.b_IRadar.reset                       = Atr22_reset;
    self->b_IRadarAtr22.b_IRadar.getDataIndex                = Atr22_getDataIndex;
    self->b_IRadarAtr22.b_IRadar.initialize                  = Atr22_initialize;
    self->b_IRadarAtr22.b_IRadar.configure                   = Atr22_configure;
    self->b_IRadarAtr22.b_IRadar.loadSequence                = Atr22_loadSequence;
    self->b_IRadarAtr22.b_IRadar.calibrate                   = Atr22_calibrate;
    self->b_IRadarAtr22.b_IRadar.startSequence               = Atr22_startSequence;
    self->b_IRadarAtr22.b_IRadar.startData                   = Atr22_startData;
    self->b_IRadarAtr22.b_IRadar.stopData                    = Atr22_stopData;
    self->b_IRadarAtr22.b_IRadar.getRxChannels               = Atr22_getRxChannels;
    self->b_IRadarAtr22.b_IRadar.enableConstantFrequencyMode = Atr22_enableConstantFrequencyMode;
    self->b_IRadarAtr22.b_IRadar.setConstantFrequency        = Atr22_setConstantFrequency;

    // IRadarAtr2 (low level interface)
    self->b_IRadarAtr22.getIRegisters     = Atr22_getIRegisters;
    self->b_IRadarAtr22.getIProtocolAtr22 = Atr22_getIProtocolAtr22;


    self->m_accessData  = accessData;
    self->m_initialized = false;
}

sr_t Atr22_Detect(II2c *accessI2c, const RadarAtr22Config_t *radarConfig)
{
    ProtocolAtr22 protocol;
    ProtocolAtr22_Constructor(&protocol, accessI2c, radarConfig->i2cDevAddr);

    // check the presence of a supported device
    uint16_t value;
    RETURN_ON_ERROR(ProtocolAtr22_executeRead(&protocol.b_IProtocolAtr22, ATR22_READ(I2C_CONF), 1, &value));
    const uint16_t I2C_CONF_SLAVE_ADDR_MASK = 0x007F;
    switch (value & I2C_CONF_SLAVE_ADDR_MASK)
    {
        case 0x0033:  // ATR22 read-only slave address value
            break;
        default:
            return E_NOT_SUPPORTED;
            break;
    }

    return E_SUCCESS;
}
