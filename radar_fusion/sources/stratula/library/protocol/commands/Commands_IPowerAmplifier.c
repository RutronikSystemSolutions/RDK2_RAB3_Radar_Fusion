/**
 * \file 	Commands_IPowerAmplifier.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IPowerAmplifier               IPowerAmplifier Commands
 * \brief           Power amplifier Commands.
 *
 * @{
 */
#include "Commands_IPowerAmplifier.h"
#include "Commands_IRegisters16.h"
#include <stdbool.h>
#include <universal/components/subinterfaces.h>
#include <universal/components/types.h>
#include <universal/implementations.h>
#include <universal/protocol/protocol_definitions.h>

static IPowerAmplifier *m_powerAmplifier[MAX_INSTANCE_REGISTRATIONS];  //PowerAmplifier instance registrations
static uint8_t m_powerAmplifierCount;                                  //Number of currently registered components

static inline uint8_t Commands_IPowerAmplifier_getImplementation(IPowerAmplifier *powerAmp, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(powerAmp->m_bImplementation))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    *payload[0] = powerAmp->m_bImplementation;

    return E_SUCCESS;
}

static inline uint8_t checkCommandTarget(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface)
{
    if (bImplementation != COMPONENT_IMPL_DEFAULT)
    {
        //no special component implementations available
        return STATUS_COMMAND_IMPL_INVALID;
    }

    if (bId >= m_powerAmplifierCount)
    {
        return STATUS_COMMAND_ID_INVALID;
    }

    return STATUS_SUCCESS;
}

uint8_t Commands_IPowerAmplifier_read(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IPowerAmplifier *powerAmp = m_powerAmplifier[bId];

    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            if (bFunction == FN_GET_IMPL)
            {
                return Commands_IPowerAmplifier_getImplementation(powerAmp, wLength, payload);
            }
            else
            {
                return STATUS_COMMAND_FUNCTION_INVALID;
            }
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters16 *registers = powerAmp->getIRegisters(powerAmp);
            return Commands_IRegisters16_read(registers, bFunction, wLength, payload);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;
    }
}

uint8_t Commands_IPowerAmplifier_write(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IPowerAmplifier *powerAmp = m_powerAmplifier[bId];

    IRegisters16 *registers = powerAmp->getIRegisters(powerAmp);
    return Commands_IRegisters16_write(registers, bFunction, wLength, payload);
}

uint8_t Commands_IPowerAmplifier_transfer(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IPowerAmplifier *powerAmp = m_powerAmplifier[bId];

    IRegisters16 *registers = powerAmp->getIRegisters(powerAmp);
    return Commands_IRegisters16_transfer(registers, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
}

void Commands_IPowerAmplifier_Constructor(ICommands *self)
{
    m_powerAmplifierCount = 0;
    self->m_bType         = COMPONENT_TYPE_POWER_AMPLIFIER;
    self->read            = Commands_IPowerAmplifier_read;
    self->write           = Commands_IPowerAmplifier_write;
    self->transfer        = Commands_IPowerAmplifier_transfer;
}

bool Commands_IPowerAmplifier_register(IPowerAmplifier *powerAmp)
{
    if (m_powerAmplifierCount >= MAX_INSTANCE_REGISTRATIONS)
    {
        return false;
    }
    m_powerAmplifier[m_powerAmplifierCount] = powerAmp;
    m_powerAmplifierCount++;
    return true;
}

/*  @} */
