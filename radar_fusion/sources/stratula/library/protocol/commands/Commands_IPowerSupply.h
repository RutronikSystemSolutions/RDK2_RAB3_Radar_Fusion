#ifndef COMMANDS_IPOWERSUPPLY_H_
#define COMMANDS_IPOWERSUPPLY_H_ 1

#include "ICommands.h"
#include <components/interfaces/IPowerSupply.h>


void Commands_IPowerSupply_Constructor(ICommands *self);

/** Register a power supply instance
 *  @param powerSupply The instance to register
 *  @return true if successful, false if all available slots are occupied
 */
bool Commands_IPowerSupply_register(IPowerSupply *powerSupply);

uint8_t Commands_IPowerSupply_read(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IPowerSupply_write(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IPowerSupply_transfer(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);


#endif /* COMMANDS_IPOWERSUPPLY_H_ */
