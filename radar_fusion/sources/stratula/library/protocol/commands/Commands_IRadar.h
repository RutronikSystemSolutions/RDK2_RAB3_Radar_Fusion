#ifndef COMMANDS_IRADAR_H_
#define COMMANDS_IRADAR_H_ 1

#include "ICommands.h"
#include <components/interfaces/IRadar.h>


void Commands_IRadar_Constructor(ICommands *self);

/** Register a radar instance
 *  @param radar The instance to register
 *  @return true if successful, false if all available slots are occupied
 */
bool Commands_IRadar_register(IRadar *radar);

/** Initialize radar, including FW update.
 *
 *  @param radar   radar-component to be used
 *  @param wLength must be 0
 *  @param payload unused
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  error_component if radar-component execution fails
 */
uint8_t Commands_IRadar_initialize(IRadar *radar, uint16_t wLength, const uint8_t *payload);

/** Resets Radar chip.
 *
 *  @param wLength must be 1
 *  @param payload 1 byte Reset type: 1=soft reset, 0=hard reset
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  E_INVALID_PARAMETER if id maps to a non existing Radar module
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IRadar_reset(IRadar *radar, uint16_t wLength, const uint8_t *payload);

/** Load radar configuration.
 *
 *  @param radar   radar-component to be used
 *  @param wLength must be sizeIfxRfe_MmicConfig()
 *  @param payload buffer containing the serialized IfxRfe_MmicConfig.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  error_component if radar-component execution fails
 */
uint8_t Commands_IRadar_configure(IRadar *radar, uint16_t wLength, const uint8_t *payload);

/** Load radar ramp-sequence.
 *
 *  @param radar   radar-component to be used
 *  @param wLength must be sizeIfxRfe_Sequence(rampCount)
 *  @param payload buffer containing the serialized IfxRfe_Sequence and its ramps (IfxRfe_Ramp).
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  error_component if radar-component execution fails
 */
uint8_t Commands_IRadar_loadSequence(IRadar *radar, uint16_t wLength, const uint8_t *payload);

/** Calibrate radar.
 *
 *  @param radar   radar-component to be used
 *  @param wIndex  unused
 *  @param wLength must be 0
 *  @param payload unused
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  error_component if radar-component execution fails
 */
uint8_t Commands_IRadar_calibrate(IRadar *radar, uint16_t wLength, const uint8_t *payload);

/** Start radar's execution of ramp sequence
 *
 *  @param radar   radar-component to be used
 *  @param wLength must be 0
 *  @param payload unused
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  error_component if radar-component execution fails
 */
uint8_t Commands_IRadar_startSequence(IRadar *radar, uint16_t wLength, const uint8_t *payload);

/** Start the radar's data interface.
 *
 *  @param radar   radar-component to be used
 *  @param wLength must be 0
 *  @param payload unused
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  error_component if radar-component execution fails
 */
uint8_t Commands_IRadar_startData(IRadar *radar, uint16_t wLength, const uint8_t *payload);

/** Stop the radar's data interface.
 *
 *  @param radar   radar-component to be used
 *  @param wLength must be 0
 *  @param payload unused
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  error_component if radar-component execution fails
 */
uint8_t Commands_IRadar_stopData(IRadar *radar, uint16_t wLength, const uint8_t *payload);

/** Get radar's index of data-interface.
 *
 *  @param radar   radar-component to be used
 *  @param wIndex  unused
 *  @param wLength must be 1
 *  @param payload buffer pointer where the data-interface's index will be stored.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 1
 *                  error_component if radar-component execution fails
 */
uint8_t Commands_IRadar_getDataIndex(IRadar *radar, uint16_t wLength, uint8_t **payload);

/** Enables a constant frequency on selected radar TX outputs.
 *
 *  @param radar   radar-component to be used
 *  @param wLength must be 6
 *  @param payload txMask (16bit)
 *                 serialized txPower value (float)
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  error_component if radar-component execution fails
 */
uint8_t Commands_IRadar_enableConstantFrequencyMode(IRadar *radar, uint16_t wLength, const uint8_t *payload);

/** Sets a static frequency on previously selected radar TX outputs.
 *
 *  @param radar   radar-component to be used
 *  @param wLength must be 8
 *  @param payload buffer containing the serialized frequency value (double)
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 4
 *                  error_component if radar-component execution fails
 */
uint8_t Commands_IRadar_setConstantFrequency(IRadar *radar, uint16_t wLength, const uint8_t *payload);

/** Get radar's number of configured RX channels.
 *
 *  @param radar   radar-component to be used
 *  @param wLength must be 1
 *  @param payload buffer pointer where the number of configured RX channels will be stored.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 1
 *                  error_component if radar-component execution fails
 */
uint8_t Commands_IRadar_getRxChannels(IRadar *radar, uint16_t wLength, uint8_t **payload);

#endif /* COMMANDS_IRADAR_H_ */
