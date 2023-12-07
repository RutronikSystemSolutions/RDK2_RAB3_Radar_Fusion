#include "RemotePinsRxs.h"
#include <stddef.h>
#include <universal/components/implementations/radar.h>
#include <universal/components/subinterfaces.h>
#include <universal/components/subinterfaces/ipins.h>
#include <universal/components/types.h>
#include <universal/protocol/protocol_definitions.h>


#define SELF ((RemotePinsRxs *)(uintptr_t)self)


static sr_t RemoteRadarPins_vendorRead(RemotePinsRxs *self, uint8_t bFunction, uint16_t size, uint8_t **buf)
{
    const uint16_t wValue = CMD_W_VALUE(COMPONENT_TYPE_RADAR, COMPONENT_IMPL_RADAR_RXS);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, COMPONENT_SUBIF_PINS, bFunction);
    return self->m_vendorCommands->vendorRead(CMD_COMPONENT, wValue, wIndex, size, buf);
}

static sr_t RemoteRadarPins_vendorWrite(RemotePinsRxs *self, uint8_t bFunction, uint16_t size, const uint8_t *buf)
{
    const uint16_t wValue = CMD_W_VALUE(COMPONENT_TYPE_RADAR, COMPONENT_IMPL_RADAR_RXS);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, COMPONENT_SUBIF_PINS, bFunction);
    return self->m_vendorCommands->vendorWrite(CMD_COMPONENT, wValue, wIndex, size, buf);
}

static sr_t RemoteRadarPins_vendorTransfer(RemotePinsRxs *self, uint8_t bFunction, uint16_t sizeIn, const uint8_t *bufIn, uint16_t *sizeOut, uint8_t **bufOut)
{
    const uint16_t wValue = CMD_W_VALUE(COMPONENT_TYPE_RADAR, COMPONENT_IMPL_RADAR_RXS);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, COMPONENT_SUBIF_PINS, bFunction);

    return self->m_vendorCommands->vendorTransfer(CMD_COMPONENT, wValue, wIndex, sizeIn, bufIn, sizeOut, bufOut);
}

static sr_t RemoteRadarPins_setResetPin(IPinsRxs *self, bool state)
{
    const uint8_t buf = state ? 1 : 0;
    return RemoteRadarPins_vendorWrite(SELF, FN_PINS_SET_RESET_PIN, sizeof(buf), &buf);
}

static sr_t RemoteRadarPins_getOkPin(IPinsRxs *self, bool *state)
{
    uint8_t buf[1];
    uint8_t *payload = buf;

    sr_t result = RemoteRadarPins_vendorRead(SELF, FN_PINS_GET_OK, 1, &payload);

    *state = *payload;
    return result;
}

static sr_t RemoteRadarPins_configureDmuxPin(IPinsRxs *self, uint8_t index, uint8_t flags)
{
    uint8_t buf[2];
    buf[0] = index;
    buf[1] = flags;
    return RemoteRadarPins_vendorWrite(SELF, FN_PINS_CONFIGURE_DMUX, 2, buf);
}

static sr_t RemoteRadarPins_getDmuxPin(IPinsRxs *self, uint8_t index, bool *state)
{
    uint8_t buf[1];
    uint8_t *payload = buf;
    uint16_t sizeOut = 1;

    sr_t result = RemoteRadarPins_vendorTransfer(SELF, FN_PINS_GET_DMUX, sizeof(index), &index, &sizeOut, &payload);

    *state = *payload;
    return result;
}

static sr_t RemoteRadarPins_setDmuxPin(IPinsRxs *self, uint8_t index, bool state)
{
    uint8_t buf[2];
    buf[0] = index;
    buf[1] = state ? 1 : 0;
    return RemoteRadarPins_vendorWrite(SELF, FN_PINS_SET_DMUX, 2, buf);
}

void RemotePinsRxs_Constructor(RemotePinsRxs *self, IVendorCommands *vendorCommands, uint8_t id)
{
    self->m_vendorCommands = vendorCommands;
    self->m_id             = id;

    self->b_IPinsRxs.setResetPin      = RemoteRadarPins_setResetPin;
    self->b_IPinsRxs.getOkPin         = RemoteRadarPins_getOkPin;
    self->b_IPinsRxs.configureDmuxPin = RemoteRadarPins_configureDmuxPin;
    self->b_IPinsRxs.getDmuxPin       = RemoteRadarPins_getDmuxPin;
    self->b_IPinsRxs.setDmuxPin       = RemoteRadarPins_setDmuxPin;
}
