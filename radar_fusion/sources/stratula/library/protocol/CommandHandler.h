#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_ 1

#include "commands/ICommands.h"
#include <stdbool.h>
#include <stdint.h>

#define MAX_COMMAND_REGISTRATIONS 8 /* Maximum number of commands implementations that can be installed to one instance of this handler */

typedef struct
{
    ICommands m_commandRegistrations[MAX_COMMAND_REGISTRATIONS];
    uint8_t m_occupiedSlots;  //number of currently occupied registration slots
} CommandHandler;

/** Register a component command implementation
 *  @param commands The command implementation instance
 *  @return true if successful, false if all available slots are occupied
 */
bool CommandHandler_registerImplementation(CommandHandler *self, const ICommands *commands);

/** Forward a write-command-request to the appropriate component, which must be registered before
 *
 *  @param bType Type of component to be addressed
 *  @param bImplementation Special implementation of the component type
 *  @param bId ID of the component in case there are multiple instances, otherwise 0
 *  @param bSubinterface Interface of the component that shall be addressed
 *  @param bFunction The specific function to be executed on the interface or component
 *  @param wLength Length of the payload to be written
 *  @param payload buffer containing the payload
 *
 *  @return bStatus as follows:
 *  	- STATUS_REQUEST_INVALID if there is no component or interface or command fitting the parameters
 *  	- bStatus returned by the write function associated with the parameters
 */
uint8_t CommandHandler_write(CommandHandler *self, uint8_t bType, uint8_t bImplementation, uint8_t bId,
                             uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);

/** Forward a read-command-request to the appropriate component, which must be registered before
 *
 *  @param bType Type of component to be addressed
 *  @param bImplementation Special implementation of the component type
 *  @param bId ID of the component in case there are multiple instances, otherwise 0
 *  @param bSubinterface Interface of the component that shall be addressed
 *  @param bFunction The specific function to be executed on the interface or component
 *  @param wLength length of payload to be read (if zero, no payload-data will be sent back to the host)
 *  @param payload buffer pointer where the read-data will be stored
 *
 *  @return bStatus as follows:
 *  	- STATUS_REQUEST_INVALID if there is no component or interface or command fitting the parameters
 *  	- bStatus returned by the read function associated with the parameters
 */
uint8_t CommandHandler_read(CommandHandler *self, uint8_t bType, uint8_t bImplementation, uint8_t bId,
                            uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload);

/** Forward a transfer-command-request to the appropriate component, which must be registered before
 *
 *  @param bType Type of component to be addressed
 *  @param bImplementation Special implementation of the component type
 *  @param bId ID of the component in case there are multiple instances, otherwise 0
 *  @param bSubinterface Interface of the component that shall be addressed
 *  @param bFunction The specific function to be executed on the interface or component
 *  @param wLengthIn length of the payloadIn which comes with this call
 *  @param payloadIn buffer pointer to the incoming data
 *  @param wLengthOut length of payload to be sent back (pointer to be filled by the callee)
 *  @param payloadOut buffer pointer where the read-data will be stored
 *
 *  @return bStatus as follows:
 *  	- STATUS_REQUEST_INVALID if there is no component or interface or command fitting the parameters
 *  	- bStatus returned by the read function associated with the parameters
 */
uint8_t CommandHandler_transfer(CommandHandler *self, uint8_t bType, uint8_t bImplementation, uint8_t bId,
                                uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

void CommandHandler_Constructor(CommandHandler *self);

#endif /* COMMANDHANDLER_H_ */
