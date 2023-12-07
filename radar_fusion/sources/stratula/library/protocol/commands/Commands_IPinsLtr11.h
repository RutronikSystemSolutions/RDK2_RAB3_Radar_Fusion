#ifndef COMMANDS_IPINSLTR11_H_
#define COMMANDS_IPINSLTR11_H_ 1

#include <components/interfaces/IPinsLtr11.h>
#include <stdint.h>

/** Set Radar Reset Pin state
 *
 *  @param wLength  must be 1
 *  @param payload  state (0 = low, 1 = high)
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module, or state > 1
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IPinsLtr11_setResetPin(IPinsLtr11 *pinsLtr11, uint16_t wLength, const uint8_t *payload);

/** Cycle Radar reset pin
 *
 *  @param wLength  must be 0
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module, or state > 1
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IPinsLtr11_reset(IPinsLtr11 *pinsLtr11, uint16_t wLength, const uint8_t *payload);

/** Get Radar IRQ Pin state
 *
 *  @param wLength must be 1
 *  @param payload buffer pointer where the read-data will be stored:
 *                 payload[0] = Status Pin state: 1=high, 0=low
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IPinsLtr11_getIrqPin(IPinsLtr11 *pinsLtr11, uint16_t wLength, uint8_t **payload);

/** Get Radar Detector output from TargetDet and PhaseDet pins
 *
 *  @param wLength must be 1
 *  @param payload buffer pointer where the read-data will be stored:
 *                 payload[0] = Status Pin state: 1=high, 0=low
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IPinsLtr11_getDetectionPins(IPinsLtr11 *pinsLtr11, uint16_t wLength, uint8_t **payload);

uint8_t Commands_IPinsLtr11_read(IPinsLtr11 *pinsLtr11, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IPinsLtr11_write(IPinsLtr11 *pinsLtr11, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IPinsLtr11_transfer(IPinsLtr11 *pinsLtr11, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

#endif /* COMMANDS_IPINSLTR11_H_ */