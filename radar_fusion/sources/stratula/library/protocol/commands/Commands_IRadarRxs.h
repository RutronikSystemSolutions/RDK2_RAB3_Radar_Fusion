#ifndef COMMANDS_IRADARRXS_H_
#define COMMANDS_IRADARRXS_H_ 1

#include <components/interfaces/IRadarRxs.h>
#include <stdint.h>

/** Executes RXS FW function.
 *
 *  @param wLength 2 + length in bytes of payload buffer containing "n" 16-bit FW function call parameters
 *  @param payload FW function call code and
 *                 buffer containing "n" 16-bit FW function call parameters distributed as:
 *                                        if n > 0:
 *                 payload[0 + (n-1)*2] = n-th 16-bit FW function call parameter, low byte
 *                 payload[1 + (n-1)*2] = n-th 16-bit FW function call parameter, high byte
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not multiple of 2
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IRadarRxs_startFirmwareFunction(IRadarRxs *radarRxs, uint16_t wLength, const uint8_t *payload);

/** Enables/Disables RXS divider output.
 *
 *  @param wLength length in bytes of payload (which is the same as the number of DMUXes to be configured)
 *  @param payload configuration values for every DMUX
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is invalid
 *                  STATUS_REQUEST_WVALUE_INVALID if id maps to a non existing Radar module
 *                  E_INVALID_PARAMETER if parameters in payload-content are not valid
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IRadarRxs_configureDmuxMap(IRadarRxs *radarRxs, uint16_t wLength, const uint8_t *payload);

/** Enables/Disables RXS divider output.
 *
 *  @param wLength must be 1
 *  @param payload Divider output action: 1=enable, 0=disable
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IRadarRxs_setDividerOutput(IRadarRxs *radarRxs, uint16_t wLength, const uint8_t *payload);

/** Reads status of previous RXS FW function execution.
 *
 *  @param wLength must be 4
 *  @param payload status (16bit) + result count (16bit)
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect
 */
uint8_t Commands_IRadarRxs_checkFirmwareFunctionStatus(IRadarRxs *radarRxs, uint16_t wLength, uint8_t **payload);

/** Reads results of previous RXS FW function execution.
 *
 *  @param wLength length of the actual result data
 *  @param payload buffer pointer where RXS FW function call return values will be stored:
 *                                        if wLength > 0, n=wLength/2:
 *                 payload[0 + (n-1)*2] = n-th 16-bit FW function call parameter, low byte
 *                 payload[1 + (n-1)*2] = n-th 16-bit FW function call parameter, high byte
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not multiple of 2
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IRadarRxs_getFirmwareFunctionResult(IRadarRxs *radarRxs, uint16_t wLength, uint8_t **payload);

/** Reads state of RXS Status Pin.
 *
 *  @param wLength must be 1
 *  @param payload buffer pointer where the read-data will be stored:
 *                 payload[0] = RXS Status Pin state: 1=high, 0=low
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 1
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IRadarRxs_getStatus(IRadarRxs *radarRxs, uint16_t wLength, uint8_t **payload);

/** Enables/Disables RXS data CRC transmission.
 *
 *  @param wLength must be 1
 *  @param payload Data CRC setting: 1=enable, 0=disable
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IRadarRxs_setDataCrc(IRadarRxs *radarRxs, uint16_t wLength, const uint8_t *payload);

/** Sets external trigger source for Ramp Sequence execution.
 *
 *  @param wLength must be 1
 *  @param payload DMUX pin index
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IRadarRxs_setTriggerSource(IRadarRxs *radarRxs, uint16_t wLength, const uint8_t *payload);

uint8_t Commands_IRadarRxs_read(IRadarRxs *radarRxs, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IRadarRxs_write(IRadarRxs *radarRxs, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IRadarRxs_transfer(IRadarRxs *radarRxs, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

#endif /* COMMANDS_IRADARRXS_H_ */
