#ifndef ICOMMANDS_H_
#define ICOMMANDS_H_ 1

#include <stddef.h>
#include <stdint.h>

#define MAX_INSTANCE_REGISTRATIONS 8 /* Maximum number of component / module implementations that can be registered to one instance of a commands implementation */

/**
 * This file defines the interface for module or component command implementations
 */

typedef struct _ICommands ICommands;
struct _ICommands
{
    uint8_t m_bType; /* Type of the component or module */

    /** Called when a write command was received for this module / component
     *
     *  @param bImplementation special implementation type if applicable
     *  @param bId ID of the module in case there are multiple instances, otherwise 0
     *  @param bSubinterface Interface of the module that shall be addressed
     *  @param bFunction The specific function to be executed on the interface or module
     *  @param wLength Length of the payload to be written
     *  @param payload buffer containing the payload
     *
     *  @return bStatus as follows:
     *  	- STATUS_REQUEST_INVALID if there is no module or interface or command fitting the parameters
     *  	- bStatus returned by the write function associated with the parameters
     */
    uint8_t (*write)(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLenght, const uint8_t *payload);

    /** Called when a read command was received for this module / component
     *
     *  @param bId ID of the module in case there are multiple instances, otherwise 0
     *  @param bSubinterface Interface of the module that shall be addressed
     *  @param bFunction The specific function to be executed on the interface or module
     *  @param wLength length of payload to be read (if zero, no payload-data will be sent back to the host)
     *  @param payload buffer pointer where the read-data will be stored
     *
     *  @return bStatus as follows:
     *  	- STATUS_REQUEST_INVALID if there is no module or interface or command fitting the parameters
     *  	- bStatus returned by the read function associated with the parameters
     */
    uint8_t (*read)(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload);

    /** Called when a transfer command was received for this module / component
     *
     *  @param bId ID of the module in case there are multiple instances, otherwise 0
     *  @param bSubinterface Interface of the module that shall be addressed
     *  @param bFunction The specific function to be executed on the interface or module
     *  @param wLengthIn Length of the payload to be written
     *  @param payloadIn buffer containing the payload
     *  @param wLengthOut length of returned payload (to be filled by the callee)
     *  @param payloadOut buffer pointer where the read-data will be stored
     *
     *  @return bStatus as follows:
     *  	- STATUS_REQUEST_INVALID if there is no module or interface or command fitting the parameters
     *  	- bStatus returned by the read function associated with the parameters
     */
    uint8_t (*transfer)(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLenghtIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);
};

#endif /* ICOMMANDS_H_ */
