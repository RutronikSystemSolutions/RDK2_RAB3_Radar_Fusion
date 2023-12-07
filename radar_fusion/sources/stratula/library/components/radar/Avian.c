#include "Avian.h"
#include <stddef.h>
#include <universal/components/implementations/radar.h>


#define SELF        ((Avian *)(uintptr_t)self)
#define REGISTERS   ((IRegisters8_32 *)&SELF->m_registers)
#define PINS_AVIAN  ((IPinsAvian *)&SELF->m_pins)
#define PROTOCOL    ((IProtocolAvian *)&SELF->m_protocol)
#define RADAR_AVIAN (&SELF->b_IRadarAvian)
#define RADAR       (&SELF->b_IRadarAvian.b_IRadar)


// Register definitions
#define MAIN               0x00
#define FIFO_RESET         (1u << 3)
#define FSM_RESET          (1u << 2)
#define SW_RESET           (1u << 1)
#define FRAME_START        (1u << 0)
#define ADC0               0x01
#define ADC0_reset_value   0x0A0240
#define ADC0_reset_value_E 0x0A0200
#define SFCTL              0x06


/* not implemented functions */
sr_t Avian_configure(IRadar *self, const IfxRfe_MmicConfig *c);
sr_t Avian_loadSequence(IRadar *self, IfxRfe_Sequence *s);
sr_t Avian_calibrate(IRadar *self);
sr_t Avian_startSequence(IRadar *self);
sr_t Avian_getRxChannels(IRadar *self, uint8_t *number);
sr_t Avian_enableConstantFrequencyMode(IRadar *self, uint16_t txMask, float txPower);
sr_t Avian_setConstantFrequency(IRadar *self, double frequency);


IPinsAvian *Avian_getIPinsAvian(IRadarAvian *self)
{
    return PINS_AVIAN;
}

IRegisters8_32 *Avian_getIRegisters(IRadarAvian *self)
{
    return REGISTERS;
}

IProtocolAvian *Avian_getIProtocolAvian(IRadarAvian *self)
{
    return PROTOCOL;
}

sr_t Avian_reset(IRadar *self, bool softReset)
{
    if (softReset)
    {
        RETURN_ON_ERROR(PROTOCOL->setBits(PROTOCOL, MAIN, SW_RESET));
    }
    else
    {
        RETURN_ON_ERROR(PINS_AVIAN->reset(PINS_AVIAN));
    }

    SELF->m_initialized = false;

    return E_SUCCESS;
}

sr_t Avian_getDataIndex(IRadar *self, uint8_t *index)
{
    *index = SELF->m_dataIndex;
    return E_SUCCESS;
}

sr_t Avian_initialize(IRadar *self)
{
    if (SELF->m_initialized)
    {
        return E_SUCCESS;
    }

    // TODO: refactor as ProtocolAvian_setHighspeedCompensation()
    //    if (SELF->m_properties->qspiWaitCycles > 15)
    //    {
    //        return E_INVALID_PARAMETER;
    //    }
    //
    //    // first of all, high speed compensation and QSPI wait cycles have to be configured
    //    const uint32_t spiConfig = AVIAN_WRITE(SFCTL,
    //                                           ((SELF->m_properties->qspiWaitCycles - 1) << 20) | (SELF->m_properties->highSpeedCompensation << 16));
    //    RETURN_ON_ERROR(PROTOCOL->execute(PROTOCOL, &spiConfig, 1, NULL));

    SELF->m_initialized = true;

    return E_SUCCESS;
}

sr_t Avian_configure(IRadar *self, const IfxRfe_MmicConfig *c)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Avian_loadSequence(IRadar *self, IfxRfe_Sequence *s)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Avian_calibrate(IRadar *self)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Avian_startSequence(IRadar *self)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Avian_startData(IRadar *self)
{
    RETURN_ON_ERROR(SELF->m_accessData->start(SELF->m_dataIndex));

    return PROTOCOL->setBits(PROTOCOL, MAIN, FRAME_START);
}

sr_t Avian_stopData(IRadar *self)
{
    RETURN_ON_ERROR(PROTOCOL->setBits(PROTOCOL, MAIN, FIFO_RESET | FSM_RESET));

    return SELF->m_accessData->stop(SELF->m_dataIndex);
}

sr_t Avian_getRxChannels(IRadar *self, uint8_t *number)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Avian_enableConstantFrequencyMode(IRadar *self, uint16_t txMask, float txPower)
{
    return E_NOT_IMPLEMENTED;
}

sr_t Avian_setConstantFrequency(IRadar *self, double frequency)
{
    return E_NOT_IMPLEMENTED;
}

void Avian_Constructor(Avian *self, IData *accessData, IGpio *accessGpio, ISpi *accessSpi, const BoardRadarConfig_t *boardConfig, const IPinsAvianConfig_t *pinsAvianConfig)
{
    self->b_IRadarAvian.b_IRadar.m_bImplementation = COMPONENT_IMPL_RADAR_AVIAN;

    PinsAvian_Constructor(&self->m_pins, accessGpio, pinsAvianConfig);
    ProtocolAvian_Constructor(&self->m_protocol, accessSpi, boardConfig->devId);
    RegistersAvian_Constructor(&self->m_registers, PROTOCOL);

    // IRadar (high level interface)
    self->b_IRadarAvian.b_IRadar.reset                       = Avian_reset;
    self->b_IRadarAvian.b_IRadar.getDataIndex                = Avian_getDataIndex;
    self->b_IRadarAvian.b_IRadar.initialize                  = Avian_initialize;
    self->b_IRadarAvian.b_IRadar.configure                   = Avian_configure;
    self->b_IRadarAvian.b_IRadar.loadSequence                = Avian_loadSequence;
    self->b_IRadarAvian.b_IRadar.calibrate                   = Avian_calibrate;
    self->b_IRadarAvian.b_IRadar.startSequence               = Avian_startSequence;
    self->b_IRadarAvian.b_IRadar.startData                   = Avian_startData;
    self->b_IRadarAvian.b_IRadar.stopData                    = Avian_stopData;
    self->b_IRadarAvian.b_IRadar.getRxChannels               = Avian_getRxChannels;
    self->b_IRadarAvian.b_IRadar.enableConstantFrequencyMode = Avian_enableConstantFrequencyMode;
    self->b_IRadarAvian.b_IRadar.setConstantFrequency        = Avian_setConstantFrequency;

    // IRadarAvian (low level interface)
    self->b_IRadarAvian.getIRegisters     = Avian_getIRegisters;
    self->b_IRadarAvian.getIPinsAvian     = Avian_getIPinsAvian;
    self->b_IRadarAvian.getIProtocolAvian = Avian_getIProtocolAvian;


    self->m_accessData  = accessData;
    self->m_dataIndex   = boardConfig->dataIndex;
    self->m_initialized = false;

    // perform hard reset because device is at undefined state after power-up
    PinsAvian_reset(&self->m_pins.b_IPinsAvian);
}

sr_t Avian_Detect(ISpi *accessSpi, IGpio *accessGpio, const BoardRadarConfig_t *boardConfig, const IPinsAvianConfig_t *pinsAvianConfig)
{
	return E_SUCCESS;

//    PinsAvian pins;
//    PinsAvian_Constructor(&pins, accessGpio, pinsAvianConfig);
//    PinsAvian_reset(&pins.b_IPinsAvian);
//
//    ProtocolAvian protocol;
//    ProtocolAvian_Constructor(&protocol, accessSpi, boardConfig->devId);
//
//    // configure high speed compensation and QSPI wait cycles
//    const uint8_t writeCommand[4] = AVIAN_WRITE(SFCTL, 0x100000);
//    RETURN_ON_ERROR(ProtocolAvian_execute(&protocol.b_IProtocolAvian, &writeCommand, 1, NULL));
//
//    // check the presence of a supported device
//    const uint8_t readCommand[4] = AVIAN_READ(ADC0);
//    uint32_t adc0;
//    RETURN_ON_ERROR(ProtocolAvian_execute(&protocol.b_IProtocolAvian, &readCommand, 1, &adc0));
//    adc0 &= AVIAN_VALUE_MASK;
//    if ((adc0 == ADC0_reset_value) || (adc0 == ADC0_reset_value_E))
//    {
//        return E_SUCCESS;
//    }
//
//    return E_NOT_SUPPORTED;
}
