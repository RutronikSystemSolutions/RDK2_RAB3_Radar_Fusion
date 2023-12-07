#ifndef COMMANDS_IPOWERAMPLIFIER_H_
#define COMMANDS_IPOWERAMPLIFIER_H_ 1

#include "ICommands.h"
#include <components/interfaces/IPowerAmplifier.h>


void Commands_IPowerAmplifier_Constructor(ICommands *self);

/** Register a power amplifier instance
 *  @param powerAmp The instance to register
 *  @return true if successful, false if all available slots are occupied
 */
bool Commands_IPowerAmplifier_register(IPowerAmplifier *powerAmp);

uint8_t Commands_IPowerAmplifier_read(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload);
uint8_t Commands_IPowerAmplifier_write(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload);
uint8_t Commands_IPowerAmplifier_transfer(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);


#endif /* COMMANDS_IPOWERAMPLIFIER_H_ */
