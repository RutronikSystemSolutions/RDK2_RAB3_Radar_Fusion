/****************************************************************************\
* Copyright (C) 2017 Infineon Technologies
*
* THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
\****************************************************************************/
#ifndef TYPE_SERIALIZATION_H
#define TYPE_SERIALIZATION_H 1

#include "type_serialization_size.h"


uint8_t *hostToSerial_IfxRfe_MmicConfig(uint8_t buf[], const IfxRfe_MmicConfig *c);
const uint8_t *serialToHost_IfxRfe_MmicConfig(const uint8_t buf[], IfxRfe_MmicConfig *c);

uint8_t *hostToSerial_IfxRfe_Ramps(uint8_t buf[], const IfxRfe_Ramp r[], const uint8_t rampCount);
uint8_t *hostToSerial_IfxRfe_Sequence(uint8_t buf[], const IfxRfe_Sequence *s);
const uint8_t *serialToHost_IfxRfe_Ramps(const uint8_t buf[], IfxRfe_Ramp r[], const uint8_t rampCount);
const uint8_t *serialToHost_IfxRfe_Sequence(const uint8_t buf[], IfxRfe_Sequence *s);

uint8_t *hostToSerial_IDataProperties(uint8_t buf[], const IDataProperties_t *p);
const uint8_t *serialToHost_IDataProperties(const uint8_t buf[], IDataProperties_t *p);

uint8_t *hostToSerial_IProcessingRadarInput(uint8_t buf[], const IProcessingRadarInput_t *c);
const uint8_t *serialToHost_IProcessingRadarInput(const uint8_t buf[], IProcessingRadarInput_t *c);

uint8_t *hostToSerial_IfxRsp_FftSetting(uint8_t buf[], const IfxRsp_FftSetting *c);
const uint8_t *serialToHost_IfxRsp_FftSetting(const uint8_t buf[], IfxRsp_FftSetting *c);

uint8_t *hostToSerial_IfxRsp_LocalMaxSetting(uint8_t buf[], const IfxRsp_LocalMaxSetting *c);
const uint8_t *serialToHost_IfxRsp_LocalMaxSetting(const uint8_t buf[], IfxRsp_LocalMaxSetting *c);

uint8_t *hostToSerial_IfxRsp_CfarCaSetting(uint8_t buf[], const IfxRsp_CfarCaSetting *c);
const uint8_t *serialToHost_IfxRsp_CfarCaSetting(const uint8_t buf[], IfxRsp_CfarCaSetting *c);

uint8_t *hostToSerial_IfxRsp_CfarGosSetting(uint8_t buf[], const IfxRsp_CfarGosSetting *c);
const uint8_t *serialToHost_IfxRsp_CfarGosSetting(const uint8_t buf[], IfxRsp_CfarGosSetting *c);

uint8_t *hostToSerial_IfxRsp_ThresholdingSetting(uint8_t buf[], const IfxRsp_ThresholdingSetting *c);
const uint8_t *serialToHost_IfxRsp_ThresholdingSetting(const uint8_t buf[], IfxRsp_ThresholdingSetting *c);

uint8_t *hostToSerial_IfxRsp_DetectionSettings(uint8_t buf[], const IfxRsp_DetectionSettings *c);
const uint8_t *serialToHost_IfxRsp_DetectionSettings(const uint8_t buf[], IfxRsp_DetectionSettings *c);

uint8_t *hostToSerial_IfxRsp_DbfSetting(uint8_t buf[], const IfxRsp_DbfSetting *c);
const uint8_t *serialToHost_IfxRsp_DbfSetting(const uint8_t buf[], IfxRsp_DbfSetting *c);

uint8_t *hostToSerial_IfxRsp_AntennaCalibration(uint8_t buf[], const IfxRsp_AntennaCalibration *c);
const uint8_t *serialToHost_IfxRsp_AntennaCalibration(const uint8_t buf[], IfxRsp_AntennaCalibration *c);

uint8_t *hostToSerial_IfxRsp_Stages(uint8_t buf[], const IfxRsp_Stages *c);
const uint8_t *serialToHost_IfxRsp_Stages(const uint8_t buf[], IfxRsp_Stages *c);

uint8_t *hostToSerial_IfxRsp_Signal(uint8_t buf[], const IfxRsp_Signal *s);
const uint8_t *serialToHost_IfxRsp_Signal(const uint8_t buf[], IfxRsp_Signal *s);

#endif /* TYPE_SERIALIZATION_H */
