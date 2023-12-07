#ifndef PROTOCOL_HANDLER_H_
#define PROTOCOL_HANDLER_H_ 1

#include <common/errors.h>
#include <stdint.h>

/**
 * Establish connection to the communication ports (i.e. Serial and Ethernet)
 *
 */
void ProtocolHandler_Constructor(void);

/**
 * Process incoming requests (if any) and sends back a response.
 * i.e. if not request available, it returns immediately.
 *
 * See VendorProtocol.h, for more info about requests and responses.
 */
void ProtocolHandler_run(void);

void ProtocolHandler_setRequestInterfaceSerial(void);
void ProtocolHandler_setRequestInterfaceEthernet(void);
sr_t ProtocolHandler_sendDebugFrameImpl(char *message, uint8_t channel, uint64_t timestamp);


/**
 * Send data-frame.
 * See VendorProtocol.h, for more info about data-frames.
 *
 * @param payload the payload of the data-frame.
 * @param size the number of bytes of payload
 *
 * @note Requires ProtocolHandler_enableSendingDataFrames() to be called first.
 */
sr_t ProtocolHandler_sendDataFrame(const uint8_t *payload, uint32_t size, uint8_t channel, uint64_t timestamp);
#ifndef TEST_CEEDLING
sr_t ProtocolHandler_sendDataFrameArray(uint32_t data[][2], uint8_t count, uint8_t channel, uint64_t timestamp);
#endif

sr_t ProtocolHandler_sendErrorFrame(uint32_t code, uint8_t channel, uint64_t timestamp);

/**
 * Initialize sending data frames from last request interface and set maximum packet size
 *
 */
void ProtocolHandler_initializeSendingDataFrames(void);

/**
 * Reset the sending of data frames. Even the currently outgoing-frame is dropped immediately.
 *
 */
void ProtocolHandler_resetSendingDataFrames(void);

#endif /* PROTOCOL_HANDLER_H_ */
