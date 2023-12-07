#include "RemoteRadarRxs.h"

#include <common/serialization.h>
#include <common/type_serialization.h>
#include <common/type_serialization_size.h>
#include <stddef.h>
#include <universal/components/implementations/radar.h>
#include <universal/components/implementations/radar/iradarrxs.h>
#include <universal/components/subinterfaces.h>
#include <universal/components/types.h>
#include <universal/protocol/protocol_definitions.h>


#define SELF ((RemoteRadarRxs *)(uintptr_t)self)


#define BaseType RemoteRadarRxs
#include "RemoteRadar.inc"


static sr_t RemoteRadarRxs_vendorRead(RemoteRadarRxs *self, uint8_t bFunction, uint16_t size, uint8_t **buf)
{
    const uint16_t wValue = CMD_W_VALUE(COMPONENT_TYPE_RADAR, COMPONENT_IMPL_RADAR_RXS);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, COMPONENT_SUBIF_DEFAULT, bFunction);
    return self->m_vendorCommands->vendorRead(CMD_COMPONENT, wValue, wIndex, size, buf);
}

static sr_t RemoteRadarRxs_vendorWrite(RemoteRadarRxs *self, uint8_t bFunction, uint16_t size, const uint8_t *buf)
{
    const uint16_t wValue = CMD_W_VALUE(COMPONENT_TYPE_RADAR, COMPONENT_IMPL_RADAR_RXS);
    const uint16_t wIndex = CMD_W_INDEX(self->m_id, COMPONENT_SUBIF_DEFAULT, bFunction);
    return self->m_vendorCommands->vendorWrite(CMD_COMPONENT, wValue, wIndex, size, buf);
}

static sr_t RemoteRadarRxs_getStatus(IRadarRxs *self, bool *status)
{
    uint8_t buf[1];
    uint8_t *payload = buf;

    const sr_t result = RemoteRadarRxs_vendorRead(SELF, FN_RADAR_RXS_GET_STATUS, 1, &payload);

    *status = (*payload != 0);
    return result;
}

static sr_t RemoteRadarRxs_configureDmuxMap(IRadarRxs *self, const uint8_t map[I_PINS_RXS_DMUX_COUNT])
{
    return RemoteRadarRxs_vendorWrite(SELF, FN_RADAR_RXS_CONFIGURE_DMUX_MAP, I_PINS_RXS_DMUX_COUNT, map);
}

static sr_t RemoteRadarRxs_enableDividerOutput(IRadarRxs *self, bool enable)
{
    const uint8_t buf = enable ? 1 : 0;
    return RemoteRadarRxs_vendorWrite(SELF, FN_RADAR_RXS_SET_DIV_OUTPUT, sizeof(buf), &buf);
}

static sr_t RemoteRadarRxs_enableDataCrc(IRadarRxs *self, bool enable)
{
    const uint8_t buf = enable ? 1 : 0;
    return RemoteRadarRxs_vendorWrite(SELF, FN_RADAR_RXS_SET_DATA_CRC, sizeof(buf), &buf);
}

static sr_t RemoteRadarRxs_enableTxRx(IRadarRxs *self)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_setTriggerSource(IRadarRxs *self, uint8_t src)
{
    return RemoteRadarRxs_vendorWrite(SELF, FN_RADAR_RXS_SET_TRIGGER_SRC, sizeof(src), &src);
}

// lower level interfaces
static sr_t RemoteRadarRxs_addConfigDescriptor(IRadarRxs *self, ConfigDescriptor *cd)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_addSegmentDescriptor(IRadarRxs *self, SegmentDescriptor *sd)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_startFirmwareFunction(IRadarRxs *self, uint16_t callcode, const uint16_t params[], uint16_t paramCount)
{
    const size_t size = sizeof(callcode) + (paramCount * 2);
    uint8_t buf[size];
    hostToSerial16(buf, callcode);
    memcpy(buf + 2, params, paramCount * 2);

    return RemoteRadarRxs_vendorWrite(SELF, FN_RADAR_RXS_START_FW_FUNCTION, size, buf);
}

static sr_t RemoteRadarRxs_checkFirmwareFunctionStatus(IRadarRxs *self, uint16_t *status, uint16_t *count)
{
    const uint8_t size = sizeof(uint16_t) + sizeof(uint16_t);
    uint8_t buf[size];
    uint8_t *payload = buf;

    const sr_t result = RemoteRadarRxs_vendorRead(SELF, FN_RADAR_RXS_CHECK_FW_FUNCTION_STATUS, size, &payload);

    *(status) = serialToHost16(payload);
    *(count)  = serialToHost16(payload + 2);
    return result;
}

static sr_t RemoteRadarRxs_getFirmwareFunctionResult(IRadarRxs *self, uint16_t retVals[], uint16_t retCount)
{
    const uint8_t size = retCount * 2;

    uint8_t *payload = (uint8_t *)retVals;

    return RemoteRadarRxs_vendorRead(SELF, FN_RADAR_RXS_GET_FW_FUNCTION_RESULT, size, &payload);

    //Assumption: payload pointer is not overwritten by vendorTransfer
}

static sr_t RemoteRadarRxs_executeFirmwareFunction(IRadarRxs *self, uint16_t callcode, const uint16_t params[], uint16_t paramCount, uint16_t retVals[], uint16_t maxRetCount, uint16_t *retCount, uint16_t *executeState)
{
    executeState = 0;
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Get_firmware_version(IRadarRxs *self, uint8_t *chipVersion, uint32_t *fwRevision)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Measure_temperature(IRadarRxs *self, float *temperature)
{
    *temperature = 0.0;
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Set_LVDS_frame_delay(IRadarRxs *self, double *rise, double *fall)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Set_TX_power(IRadarRxs *self, uint8_t TX1_DAC_value, uint8_t TX2_DAC_value, uint8_t TX3_DAC_value)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Enable_RX(IRadarRxs *self, bool RX1_enable, bool RX2_enable, bool RX3_enable, bool RX4_enable, uint8_t LP_gain, uint8_t HP_gain, uint8_t DCOC_shift, uint8_t DCOC_enable, uint8_t RX_settings)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Enable_TX(IRadarRxs *self, bool TX1_enable, bool TX2_enable, bool TX3_enable, uint8_t operationMode)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_LVDS_calib_start(IRadarRxs *self)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_LVDS_calib_stop(IRadarRxs *self)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_LVDS_calib_fake(IRadarRxs *self)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Calibration(IRadarRxs *self, int32_t nf_min, int32_t nf_calib, uint8_t tx_phase_cal_iqm_mask1, uint8_t tx_phase_cal_iqm_mask2, uint8_t ref_sel, bool cal_ref, uint8_t tx_iqm_verif, uint16_t run_flags, uint16_t *calib_run_status)
{
    calib_run_status = 0;
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Static_frequency(IRadarRxs *self, int32_t nf)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Start_rmp_scenario(IRadarRxs *self, uint16_t startAddress, uint8_t triggerSource, uint8_t monitoring)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Goto_low_power(IRadarRxs *self)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Set_clk_out(IRadarRxs *self, bool Enable_25_Clk_Out, bool Enable_50_Clk_Out)
{
    return E_NOT_IMPLEMENTED;
}

static sr_t RemoteRadarRxs_Set_phase(IRadarRxs *self, uint8_t configuration_index, uint16_t phase_angle)
{
    return E_NOT_IMPLEMENTED;
}

static IPinsRxs *RemoteRadarRxs_getIPinsRxs(IRadarRxs *self)
{
    return (IPinsRxs *)(&SELF->m_radarPins);
}

static IRegisters16 *RemoteRadarRxs_getIRegisters(IRadarRxs *self)
{
    return (IRegisters16 *)(&SELF->m_radarRegisters);
}

void RemoteRadarRxs_Constructor(RemoteRadarRxs *self, IVendorCommands *vendorCommands, uint8_t id)
{
    self->m_vendorCommands = vendorCommands;
    self->m_id             = id;

    RemoteRadar_Constructor(&self->b_IRadarRxs.b_IRadar);

    self->b_IRadarRxs.b_IRadar.m_bImplementation = COMPONENT_IMPL_RADAR_RXS;

    RemotePinsRxs_Constructor(&self->m_radarPins, vendorCommands, id);
    RemoteRegisters16_Constructor(&self->m_radarRegisters, vendorCommands, id);

    self->b_IRadarRxs.getIPinsRxs   = RemoteRadarRxs_getIPinsRxs;
    self->b_IRadarRxs.getIRegisters = RemoteRadarRxs_getIRegisters;

    self->b_IRadarRxs.getStatus           = RemoteRadarRxs_getStatus;
    self->b_IRadarRxs.configureDmuxMap    = RemoteRadarRxs_configureDmuxMap;
    self->b_IRadarRxs.enableDividerOutput = RemoteRadarRxs_enableDividerOutput;
    self->b_IRadarRxs.enableDataCrc       = RemoteRadarRxs_enableDataCrc;
    self->b_IRadarRxs.setTriggerSource    = RemoteRadarRxs_setTriggerSource;
    self->b_IRadarRxs.enableTxRx          = RemoteRadarRxs_enableTxRx;

    // lower level interfaces
    self->b_IRadarRxs.addConfigDescriptor         = RemoteRadarRxs_addConfigDescriptor;
    self->b_IRadarRxs.addSegmentDescriptor        = RemoteRadarRxs_addSegmentDescriptor;
    self->b_IRadarRxs.startFirmwareFunction       = RemoteRadarRxs_startFirmwareFunction;
    self->b_IRadarRxs.checkFirmwareFunctionStatus = RemoteRadarRxs_checkFirmwareFunctionStatus;
    self->b_IRadarRxs.getFirmwareFunctionResult   = RemoteRadarRxs_getFirmwareFunctionResult;
    self->b_IRadarRxs.executeFirmwareFunction     = RemoteRadarRxs_executeFirmwareFunction;
    self->b_IRadarRxs.Get_firmware_version        = RemoteRadarRxs_Get_firmware_version;
    self->b_IRadarRxs.Measure_temperature         = RemoteRadarRxs_Measure_temperature;
    self->b_IRadarRxs.Set_LVDS_frame_delay        = RemoteRadarRxs_Set_LVDS_frame_delay;
    self->b_IRadarRxs.Set_TX_power                = RemoteRadarRxs_Set_TX_power;
    self->b_IRadarRxs.Enable_RX                   = RemoteRadarRxs_Enable_RX;
    self->b_IRadarRxs.Enable_TX                   = RemoteRadarRxs_Enable_TX;
    self->b_IRadarRxs.LVDS_calib_start            = RemoteRadarRxs_LVDS_calib_start;
    self->b_IRadarRxs.LVDS_calib_stop             = RemoteRadarRxs_LVDS_calib_stop;
    self->b_IRadarRxs.LVDS_calib_fake             = RemoteRadarRxs_LVDS_calib_fake;
    self->b_IRadarRxs.Calibration                 = RemoteRadarRxs_Calibration;
    self->b_IRadarRxs.Static_frequency            = RemoteRadarRxs_Static_frequency;
    self->b_IRadarRxs.Start_rmp_scenario          = RemoteRadarRxs_Start_rmp_scenario;
    self->b_IRadarRxs.Goto_low_power              = RemoteRadarRxs_Goto_low_power;
    self->b_IRadarRxs.Set_clk_out                 = RemoteRadarRxs_Set_clk_out;
    self->b_IRadarRxs.Set_phase                   = RemoteRadarRxs_Set_phase;
}
