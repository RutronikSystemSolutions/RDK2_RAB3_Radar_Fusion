#include "CommandHandler.h"
#include <universal/protocol/protocol_definitions.h>

#include <stddef.h>

static ICommands *findRegistration(CommandHandler *self, uint8_t bType)
{
    for (uint8_t idx = 0; idx < self->m_occupiedSlots; idx++)
    {
        if (self->m_commandRegistrations[idx].m_bType == bType)
        {
            return (self->m_commandRegistrations + idx);
        }
    }
    return NULL;
}

bool CommandHandler_registerImplementation(CommandHandler *self, const ICommands *commands)
{
    if (self->m_occupiedSlots >= MAX_COMMAND_REGISTRATIONS)
    {
        return false;
    }
    self->m_commandRegistrations[self->m_occupiedSlots++] = *commands;
    return true;
}

uint8_t CommandHandler_write(CommandHandler *self, uint8_t bType, uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    ICommands *commands = findRegistration(self, bType);
    if (commands == NULL)
    {
        return STATUS_COMMAND_TYPE_INVALID;
    }
    return commands->write(bImplementation, bId, bSubinterface, bFunction, wLength, payload);
}

uint8_t CommandHandler_read(CommandHandler *self, uint8_t bType, uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    ICommands *commands = findRegistration(self, bType);
    if (commands == NULL)
    {
        return STATUS_COMMAND_TYPE_INVALID;
    }
    return commands->read(bImplementation, bId, bSubinterface, bFunction, wLength, payload);
}

uint8_t CommandHandler_transfer(CommandHandler *self, uint8_t bType, uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    ICommands *commands = findRegistration(self, bType);
    if (commands == NULL)
    {
        return STATUS_COMMAND_TYPE_INVALID;
    }
    return commands->transfer(bImplementation, bId, bSubinterface, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
}

void CommandHandler_Constructor(CommandHandler *self)
{
    self->m_occupiedSlots = 0;
}
