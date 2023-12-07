#ifndef PROTOCOL_HANDLER_SERIAL_H_
#define PROTOCOL_HANDLER_SERIAL_H_ 1

#include "VendorProtocol.h"
#include <common/errors.h>
#include <universal/link_definitions.h>

#include <stdint.h>


#define PROTOCOL_HANDLER_SERIAL_CRC_SIZE 2
#define PROTOCOL_HANDLER_SERIAL_BAUDRATE 921600
#define PROTOCOL_HANDLER_SERIAL_TIMEOUT  200

/** Enable the Serial communication
 * i.e. Open a connection through a serial-port
 *
 */
void ProtocolHandlerSerial_Constructor(void);

/** Process incoming requests (if any) and sends back a response.
 *  i.e. if not request available, it returns immediately.
 *
 *  See VendorProtocol.h, for more info about requests and responses.
 */
void ProtocolHandlerSerial_run(void);

/** Send a data-packet
 *  See VendorProtocol.h, for more info about data-packets.
 *
 * @param type the type of packet
 * @param count the packet's count value (used for error checking on the host-side)
 * @param length number of bytes of the packet's payload
 * @param payload a pointer to the packet's payload
 *
 */
sr_t ProtocolHandlerSerial_sendDataPacket(VendorProtocol_DataPacketHeader *header, const uint8_t *payload, uint64_t timestamp);

void ProtocolHandlerSerial_reset(void);

#endif /* PROTOCOL_HANDLER_SERIAL_H_ */
