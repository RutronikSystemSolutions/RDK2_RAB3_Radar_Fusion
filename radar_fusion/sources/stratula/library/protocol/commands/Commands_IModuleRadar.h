#ifndef COMMANDS_IMODULERADAR_H_
#define COMMANDS_IMODULERADAR_H_ 1

#include "ICommands.h"
#include <modules/interfaces/IModuleRadar.h>


void Commands_IModuleRadar_Constructor(ICommands *self);

/** Register a module instance
 *  @param moduleRadar The instance to register
 *  @return true if successful, false if all available slots are occupied
 */
bool Commands_IModuleRadar_registerModule(IModuleRadar *moduleRadar);

/** Start the module radar measurements.
 *
 *  @param wLength must be 4
 *  @param payload 1 double Update cycle
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  STATUS_COMMAND_PAYLOAD_INVALID if command in invalid
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_startMeasurement(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload);

/** Stop the module radar measurements.
 *
 *  @param wLength must be 0
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  STATUS_COMMAND_PAYLOAD_INVALID if command in invalid
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_stopMeasurement(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload);

/** Do the module radar measurements.
 *
 *  @param wLength must be 0
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  STATUS_COMMAND_PAYLOAD_INVALID if command in invalid
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_doMeasurement(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload);

/** Reset the module radar.
 *
 *  @param wLength must be 0
 *
 *  @return bStatus STATUS_SUCCESS if successful
 *                  E_FAILED in case of error
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 *                  STATUS_COMMAND_PAYLOAD_INVALID if command is invalid
 */
uint8_t Commands_IModuleRadar_reset(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload);

/** Configure the module radar.
 *
 *  @param wLength must be 0
 *
 *  @return bStatus STATUS_SUCCESS if successful
 *                  E_FAILED in case of error
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 *                  STATUS_COMMAND_PAYLOAD_INVALID if command is invalid
 */
uint8_t Commands_IModuleRadar_configure(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload);

/** Sets MMIC configuration to be used by the module radar.
 *
 *  @param wLength must be sizeIfxRfe_MmicConfig()
 *  @param payload buffer containing serialized IfxRfe_MmicConfig.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_setConfiguration(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload);

/** Sets ramp sequence to be used by the module radar.
 *
 *  @param wLength must be sizeIfxRfe_Sequence(rampCount)
 *  @param payload buffer containing serialized IfxRfe_Sequence.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  STATUS_COMMAND_PAYLOAD_INVALID if ramp count exceeds MAX_RAMPS_SUPPORTED
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_setSequence(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload);

/** Sets processing stages to be used by the module radar.
 *
 *  @param wLength must be sizeIfxRsp_Stages()
 *  @param payload buffer containing serialized IfxRsp_Stages.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_setProcessingStages(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload);

/** Sets antenna calibration to be used by the module radar.
 *
 *  @param wLength must be 2 * sizeIfxRsp_AntennaCalibration()
 *  @param payload buffer containing 2 * serialized IfxRsp_AntennaCalibration.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_setCalibration(IModuleRadar *moduleRadar, uint16_t wLength, const uint8_t *payload);

/** Reads MMIC configuration used by the module radar.
 *
 *  @param wLength must be sizeIfxRfe_MmicConfig()
 *  @param payload buffer pointer where serialized IfxRfe_MmicConfig will be stored.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_getConfiguration(IModuleRadar *moduleRadar, uint16_t wLength, uint8_t **payload);

/** Reads ramp sequence used by the module radar.
 *
 *  @param wLengthIn must be 0
 *  @param wLengthOut will be sizeIfxRfe_Sequence(rampCount), filled by called function
 *  @param payload buffer pointer where serialized IfxRfe_Sequence will be stored.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  STATUS_COMMAND_PAYLOAD_INVALID if mode is invalid
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_getSequence(IModuleRadar *moduleRadar, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

/** Gets the module radar data properties.
 *
 *  @param wLength must be sizeof_serialized_IDataProperties
 *  @param payload buffer pointer where serialized info will be stored.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  STATUS_COMMAND_PAYLOAD_INVALID if mode invalid
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_getDataProperties(IModuleRadar *moduleRadar, uint16_t wLength, uint8_t **payload);

/** Gets the module radar info.
 *
 *  @param wLength must be sizeof_serialized_IProcessingRadarInput
 *  @param payload buffer pointer where serialized info will be stored.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  STATUS_COMMAND_PAYLOAD_INVALID if mode invalid
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_getRadarInfo(IModuleRadar *moduleRadar, uint16_t wLength, uint8_t **payload);

/** Gets processing stages used by the module radar.
 *
 *  @param wLength must be sizeIfxRsp_Stages()
 *  @param payload buffer pointer where serialized IfxRsp_Stages will be stored.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_getProcessingStages(IModuleRadar *moduleRadar, uint16_t wLength, uint8_t **payload);

/** Gets antenna calibration used by the module radar.
 *
 *  @param wLength must be 2 * sizeIfxRsp_AntennaCalibration()
 *  @param payload buffer pointer where 2 * serialized IfxRsp_AntennaCalibration will be stored.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 */
uint8_t Commands_IModuleRadar_getCalibration(IModuleRadar *moduleRadar, uint16_t wLength, uint8_t **payload);

#endif /* COMMANDS_IMODULERADAR_H_ */
