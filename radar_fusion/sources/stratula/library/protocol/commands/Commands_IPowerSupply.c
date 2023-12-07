/**
 * \file 	Commands_IPowerSupply.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IPowerSupply               IPowerSupply Commands
 * \brief           Power supply Commands.
 *
 * @{
 */
#include "Commands_IPowerSupply.h"
#include "Commands_IRegisters8.h"
#include <stdbool.h>
#include <universal/components/subinterfaces.h>
#include <universal/components/types.h>
#include <universal/implementations.h>
#include <universal/protocol/protocol_definitions.h>

static IPowerSupply *m_powerSupply[MAX_INSTANCE_REGISTRATIONS];  //PowerSupply instance registrations
static uint8_t m_powerSupplyCount;                               //Number of currently registered components

static inline uint8_t Commands_IPowerSupply_getImplementation(IPowerSupply *powerSupply, uint16_t wLength, uint8_t **payload)
{
    if (wLength != sizeof(powerSupply->m_bImplementation))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    *payload[0] = powerSupply->m_bImplementation;

    return E_SUCCESS;
}

static inline uint8_t checkCommandTarget(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface)
{
    if (bId >= m_powerSupplyCount)
    {
        return STATUS_COMMAND_ID_INVALID;
    }
    if (m_powerSupply[bId] == NULL)
    {
        return STATUS_COMMAND_ID_INVALID;
    }

    if (bImplementation != COMPONENT_IMPL_DEFAULT)
    {
        //no special component implementations available
        return STATUS_COMMAND_IMPL_INVALID;
    }

    return STATUS_SUCCESS;
}

uint8_t Commands_IPowerSupply_read(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IPowerSupply *powerSupply = m_powerSupply[bId];

    switch (bSubinterface)
    {
        case COMPONENT_SUBIF_DEFAULT:
            if (bFunction == FN_GET_IMPL)
            {
                return Commands_IPowerSupply_getImplementation(powerSupply, wLength, payload);
            }
            else
            {
                return STATUS_COMMAND_FUNCTION_INVALID;
            }
        case COMPONENT_SUBIF_REGISTERS:
        {
            IRegisters8 *registers = powerSupply->getIRegisters(powerSupply);
            return Commands_IRegisters8_read(registers, bFunction, wLength, payload);
        }
        default:
            return STATUS_COMMAND_SUBIF_INVALID;
    }
}

uint8_t Commands_IPowerSupply_write(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IPowerSupply *powerSupply = m_powerSupply[bId];

    IRegisters8 *registers = powerSupply->getIRegisters(powerSupply);
    return Commands_IRegisters8_write(registers, bFunction, wLength, payload);
}

uint8_t Commands_IPowerSupply_transfer(uint8_t bImplementation, uint8_t bId, uint8_t bSubinterface, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    const uint8_t bStatus = checkCommandTarget(bImplementation, bId, bSubinterface);
    if (bStatus != STATUS_SUCCESS)
    {
        return bStatus;
    }

    IPowerSupply *powerSupply = m_powerSupply[bId];

    IRegisters8 *registers = powerSupply->getIRegisters(powerSupply);
    return Commands_IRegisters8_transfer(registers, bFunction, wLengthIn, payloadIn, wLengthOut, payloadOut);
}

void Commands_IPowerSupply_Constructor(ICommands *self)
{
    m_powerSupplyCount = 0;
    self->m_bType      = COMPONENT_TYPE_POWER_SUPPLY;
    self->read         = Commands_IPowerSupply_read;
    self->write        = Commands_IPowerSupply_write;
    self->transfer     = Commands_IPowerSupply_transfer;
}

bool Commands_IPowerSupply_register(IPowerSupply *powerSupply)
{
    if (m_powerSupplyCount >= MAX_INSTANCE_REGISTRATIONS)
    {
        return false;
    }
    m_powerSupply[m_powerSupplyCount] = powerSupply;
    m_powerSupplyCount++;
    return true;
}

/*  @} */
