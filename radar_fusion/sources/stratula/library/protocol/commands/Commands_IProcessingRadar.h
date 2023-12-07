#ifndef _COMMANDS_IPROCESSING_RADAR_H_
#define _COMMANDS_IPROCESSING_RADAR_H_ 1

#include "ICommands.h"
#include <platform/interfaces/IProcessingRadar.h>


void Commands_IProcessingRadar_Constructor(ICommands *self);

/** Register a radar processing instance
 *  @param processing The instance to register
 *  @return true if successful, false if all available slots are occupied
 */
bool Commands_IProcessingRadar_registerIProcessingRadar(IProcessingRadar *processing);

/** Configure processing unit to process incoming data.
 *
 *  @param wLength must be sizeIProcessingRadar_Descriptor()
 *  @param payload buffer containing serialized IProcessingRadar_Descriptor.
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is incorrect value
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IProcessingRadar_configure(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IProcessingRadar_getRawData(IProcessingRadar *processingRadar, uint16_t wLength, uint8_t **payload);


/** Do the specified operation
 *
 *  @param wLength serialized buffer length
 *  @param payload Buffer containing parameters for operation
 *
 *  @return bStatus STATUS_SUCCESS if successful, otherwise it returns an error-code
 */
uint8_t Commands_IProcessingRadar_doFft(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IProcessingRadar_doNci(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IProcessingRadar_doPsd(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IProcessingRadar_doThresholding(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload);

/** Get result of the previous operation (e.g. FFT, NCI, ...)
 *
 *  @param wLength must be sizeof_serialized_IfxRsp_Signal
 *  @param payload buffer pointer where the read-data will be stored
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 1
 */
uint8_t Commands_IProcessingRadar_readOperationResult(IProcessingRadar *processingRadar, uint16_t wLength, uint8_t **payload);

/** Write processing unit's configuration directly into Ram-configuration area
 *
 *  @param wLength 2 + length in bytes of the "n" 32-bit values in payload buffer
 *  @param payload buffer containing "n" 32-bit values (LSB first)
 *                 offset in the configuration RAM (in 32-bit words), 16bit value
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not multiple of 4
 *                  error_platform if execution was not successful for some reason
 */
uint8_t Commands_IProcessingRadar_writeConfigRam(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload);

/** Over-write window-coefficients of an already configured slot
 *
 *  @param wLength 3 + length in bytes of the "n" 32-bit values in payload buffer
 *  @param payload buffer containing "n" 32-bit values as follows (LSB first)
 *                 offset (16bit)
 *                 Number of the already configured slot (8bit)
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not multiple of 4
 *                  error_platform if execution was not successful for some reason
 */
uint8_t Commands_IProcessingRadar_writeCustomWindowCoefficients(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload);

/** Re-initialize unit (reset configuration).
 *
 *  @param wLength must be 0
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IProcessingRadar_reinit(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload);

/** Start processing unit as configured.
 *
 *  @param wLength must be 0
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 0
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IProcessingRadar_start(IProcessingRadar *processingRadar, uint16_t wLength, const uint8_t *payload);

/** Get busy state, i.e. check if all configured data has been received and processed.
 *
 *  @param wLength must be 1 (i.e. 1 byte of payload will be provided)
 *  @param payload buffer pointer where the read-data will be stored:
 *                 payload[0] = Busy state: 1=true, 0=false
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not 1
 *                  E_FAILED if parameters were valid but execution failed
 */
uint8_t Commands_IProcessingRadar_isBusy(IProcessingRadar *processingRadar, uint16_t wLength, uint8_t **payload);

#endif /* _COMMANDS_IPROCESSING_RADAR_H_ */
