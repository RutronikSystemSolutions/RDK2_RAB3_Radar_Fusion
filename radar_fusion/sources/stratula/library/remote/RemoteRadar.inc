#include <common/serialization.h>
#include <common/type_serialization.h>
#include <common/type_serialization_size.h>
#include <stddef.h>
#include <universal/components/subinterfaces.h>
#include <universal/components/types.h>
#include <universal/components/types/iradar.h>
#include <universal/protocol/protocol_definitions.h>


static sr_t RemoteRadar_vendorRead(BaseType *self, uint8_t bFunction, uint16_t size, uint8_t **buf)
{
    const uint16_t wValue = CMD_W_VALUE(COMPONENT_TYPE_RADAR, COMPONENT_IMPL_DEFAULT);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, COMPONENT_SUBIF_DEFAULT, bFunction);
    return self->m_vendorCommands->vendorRead(CMD_COMPONENT, wValue, wIndex, size, buf);
}

static sr_t RemoteRadar_vendorWrite(BaseType *self, uint8_t bFunction, uint16_t size, const uint8_t *buf)
{
    const uint16_t wValue = CMD_W_VALUE(COMPONENT_TYPE_RADAR, COMPONENT_IMPL_DEFAULT);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, COMPONENT_SUBIF_DEFAULT, bFunction);
    return self->m_vendorCommands->vendorWrite(CMD_COMPONENT, wValue, wIndex, size, buf);
}

static sr_t RemoteRadar_initialize(IRadar *self)
{
    return RemoteRadar_vendorWrite(SELF, FN_RADAR_INITIALIZE, 0, NULL);
}

static sr_t RemoteRadar_reset(IRadar *self, bool softReset)
{
    const uint8_t buf = softReset ? 1 : 0;
    return RemoteRadar_vendorWrite(SELF, FN_RADAR_RESET, sizeof(buf), &buf);
}

static sr_t RemoteRadar_configure(IRadar *self, const IfxRfe_MmicConfig *c)
{
    const size_t size = sizeof_serialized_IfxRfe_MmicConfig();
    uint8_t buf[size];
    hostToSerial_IfxRfe_MmicConfig(buf, c);

    return RemoteRadar_vendorWrite(SELF, FN_RADAR_CONFIGURE, size, buf);
}

static sr_t RemoteRadar_loadSequence(IRadar *self, IfxRfe_Sequence *s)
{
    const size_t size = sizeof_serialized_IfxRfe_Sequence(s->rampCount);
    uint8_t buf[size];
    uint8_t *rampsBuf = hostToSerial_IfxRfe_Sequence(buf, s);
    hostToSerial_IfxRfe_Ramps(rampsBuf, s->ramps, s->rampCount);

    return RemoteRadar_vendorWrite(SELF, FN_RADAR_LOAD_SEQUENCE, size, buf);
}

static sr_t RemoteRadar_calibrate(IRadar *self)
{
    return RemoteRadar_vendorWrite(SELF, FN_RADAR_CALIBRATE, 0, NULL);
}

static sr_t RemoteRadar_startSequence(IRadar *self)
{
    return RemoteRadar_vendorWrite(SELF, FN_RADAR_START_SEQUENCE, 0, NULL);
}

static sr_t RemoteRadar_startData(IRadar *self)
{
    return RemoteRadar_vendorWrite(SELF, FN_RADAR_START_DATA, 0, NULL);
}

static sr_t RemoteRadar_stopData(IRadar *self)
{
    return RemoteRadar_vendorWrite(SELF, FN_RADAR_STOP_DATA, 0, NULL);
}

static sr_t RemoteRadar_getDataIndex(IRadar *self, uint8_t *index)
{
    uint8_t buf[1];
    uint8_t *payload = buf;

    const sr_t result = RemoteRadar_vendorRead(SELF, FN_RADAR_GET_DATA_INDEX, 1, &payload);

    *index = *payload;
    return result;
}

static sr_t RemoteRadar_getRxChannels(IRadar *self, uint8_t *number)
{
    uint8_t buf[1];
    uint8_t *payload = buf;

    const sr_t result = RemoteRadar_vendorRead(SELF, FN_RADAR_GET_RX_CHANNELS, 1, &payload);

    *number = *payload;
    return result;
}

static sr_t RemoteRadar_enableConstantFrequencyMode(IRadar *self, uint16_t txMask, float txPower)
{
    const size_t size = sizeof(txMask) + sizeof(txPower);
    uint8_t buf[size];
    hostToSerial16(buf, txMask);
    memcpy(buf + 2, &txPower, sizeof(txPower));

    return RemoteRadar_vendorWrite(SELF, FN_RADAR_ENABLE_CONSTANT_FREQ, size, buf);
}

static sr_t RemoteRadar_setConstantFrequency(IRadar *self, double frequency)
{
    return RemoteRadar_vendorWrite(SELF, FN_RADAR_SET_CONSTANT_FREQ, sizeof(frequency), (uint8_t *)&frequency);
}

static void RemoteRadar_Constructor(IRadar *b_IRadar)
{
    b_IRadar->m_bImplementation           = COMPONENT_IMPL_DEFAULT;
    b_IRadar->initialize                  = RemoteRadar_initialize;
    b_IRadar->reset                       = RemoteRadar_reset;
    b_IRadar->configure                   = RemoteRadar_configure;
    b_IRadar->loadSequence                = RemoteRadar_loadSequence;
    b_IRadar->calibrate                   = RemoteRadar_calibrate;
    b_IRadar->startSequence               = RemoteRadar_startSequence;
    b_IRadar->startData                   = RemoteRadar_startData;
    b_IRadar->stopData                    = RemoteRadar_stopData;
    b_IRadar->getDataIndex                = RemoteRadar_getDataIndex;
    b_IRadar->getRxChannels               = RemoteRadar_getRxChannels;
    b_IRadar->enableConstantFrequencyMode = RemoteRadar_enableConstantFrequencyMode;
    b_IRadar->setConstantFrequency        = RemoteRadar_setConstantFrequency;
}
