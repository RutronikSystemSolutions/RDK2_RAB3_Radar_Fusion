#ifndef COMMANDS_I_PINS_RXS_H_
#define COMMANDS_I_PINS_RXS_H_ 1

#include <components/interfaces/IPinsRxs.h>


/** Set Radar Reset Pin state
 *
 *  @param wLength  must be 1
 *  @param payload  state (0 = low, 1 = high)
 *
 *  @sa Commands_IPinsRxs_reset
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_INVALID_PARAMETER if state > 1
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IPinsRxs_setResetPin(IPinsRxs *pinsRadar, uint16_t wLength, const uint8_t *payload);

/** Perform a reset of the Rxs
 *  This sets the reset pin low and then high respecting the correct timing.
 *
 *  @param wLength  must be 0
 *  @param payload  none
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IPinsRxs_reset(IPinsRxs *pinsRadar, uint16_t wLength, const uint8_t *payload);

/** Configure Radar DMUX Pin
 *
 *  @param wLength  must be 2
 *  @param payload  index, flags
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_INVALID_PARAMETER if payload maps to invalid DMUX flags or index
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IPinsRxs_configureDmuxPin(IPinsRxs *pinsRadar, uint16_t wLength, const uint8_t *payload);

/** Set Radar DMUX Pin
 *
 *  @param wLength  must be 2
 *  @param payload  index, state (0 = low, 1 = high)
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_INVALID_PARAMETER if payload maps to invalid DMUX flags or index
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IPinsRxs_setDmuxPin(IPinsRxs *pinsRadar, uint16_t wLength, const uint8_t *payload);

/** Get Radar Ok Pin state
 *
 *  @param wLength must be 1
 *  @param payload buffer pointer where the read-data will be stored:
 *                 payload[0] = Status Pin state: 1=high, 0=low
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IPinsRxs_getOkPin(IPinsRxs *pinsRadar, uint16_t wLength, uint8_t **payload);

/** Get Radar DMUX Pin state
 *
 *  @param wLengthIn must be 1
 *  @param payloadIn index
 *  @param wLengthOut 1
 *  @param payload buffer pointer where the read-data will be stored:
 *                 payload[0] = DMUX Pin state: 1=high, 0=low
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_INVALID_PARAMETER if payload maps to invalid DMUX index
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IPinsRxs_getDmuxPin(IPinsRxs *pinsRadar, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

uint8_t Commands_IPinsRxs_read(IPinsRxs *pinsRadar, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IPinsRxs_write(IPinsRxs *pinsRadar, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IPinsRxs_transfer(IPinsRxs *pinsRadar, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

#endif /* COMMANDS_I_PINS_RXS_H_ */
