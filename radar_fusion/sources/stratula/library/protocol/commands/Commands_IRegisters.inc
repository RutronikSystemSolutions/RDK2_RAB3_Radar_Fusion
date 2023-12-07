/**
 * \file    Commands_IRegisters.c
 *
 * \addtogroup      Command_Interface   Command Interface
 *
 * \defgroup        Commands_IRegisters IRegisters Commands
 * \brief           Direct register interface commands.
 *
 * @{
 */
#include <common/serialization.h>
#include <universal/components/subinterfaces/iregisters.h>
#include <universal/protocol/protocol_definitions.h>


#define serialToHost_uint8_t  serialToHost8
#define serialToHost_uint16_t serialToHost16
#define serialToHost_uint32_t serialToHost32


/** Reads a burst of register values
 *
 *  @param wLengthIn must be 2 * sizeof(AddrType)
 *  @param payloadIn buffer containing
 *                 AddrType address: start address
 *                 AddrType count: number of registers to read
 *  @param wLengthOut length in bytes of count return values of type RegType
 *  @param payloadOut buffer (RegType aligned) where register values will be stored:
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not valid
 *                  E_INVALID_PARAMETER if id maps to a non existing component
 *                  E_FAILED if parameters were valid but execution failed
 */
static uint8_t _readBurst(IRegistersType *registers, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

/** Writes a burst of register values
 *
 *  @param wLength length in bytes of payload buffer + sizeof(AddrType)
 *  @param payload buffer containing count register values of type RegType (little endian)
 *                 start address of AddrType
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not valid
 *                  E_INVALID_PARAMETER if id maps to a non existing component
 *                  E_FAILED if parameters were valid but execution failed
 */
static uint8_t _writeBurst(IRegistersType *registers, uint16_t wLength, const uint8_t *payload);

/** Reads a batch of register values
 *
 *  @param wLength      length in bytes of payload buffer
 *  @param payloadIn    buffer containing count addresses of AddrType (little endian)
 *  @param wLengthOut   length in bytes of count return values of RegType
 *  @param payloadOut   buffer (RegType aligned) where register values will be stored
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not valid
 *                  E_INVALID_PARAMETER if id maps to a non existing component
 *                  E_FAILED if parameters were valid but execution failed
 */
static uint8_t _readBatch(IRegistersType *registers, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);

/** Writes a batch of register values
 *
 *  @param wLengthIn    length in bytes of payload buffer
 *  @param payload      buffer containing count address/value pairs of (AddrType, RegType) (little endian)
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength is not valid
 *                  E_INVALID_PARAMETER if id maps to a non existing component
 *                  E_FAILED if parameters were valid but execution failed
 */
static uint8_t _writeBatch(IRegistersType *registers, uint16_t wLength, const uint8_t *payload);

/** Set bits on a given register
 *
 *  @param wLength  must be sizeof(AddrType) + sizeof(RegType)
 *  @param payload  buffer containing:
 *                  register address (AddrType)
 *                  bitmask (RegType)
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_INVALID_PARAMETER if id maps to a non existing component
 *                  E_FAILED if parameters were valid but execution failed
 */
static uint8_t _setBits(IRegistersType *registers, uint16_t wLength, const uint8_t *payload);

/** Clear bits on a given register
 *
 *  @param wLength  must be sizeof(AddrType) + sizeof(RegType)
 *  @param payload  buffer containing:
 *                  register address (AddrType)
 *                  bitmask (RegType)
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_INVALID_PARAMETER if id maps to a non existing component
 *                  E_FAILED if parameters were valid but execution failed
 */
static uint8_t _clearBits(IRegistersType *registers, uint16_t wLength, const uint8_t *payload);

/** Modify bits on a given register
 *
 *  @param wLength  must be sizeof(AddrType) + 2 * sizeof(RegType)
 *  @param payload  buffer containing:
 *                  register address (AddrType)
 *                  setmask (RegType)
 *                  clearmask (RegType)
 *
 *  @return bStatus STATUS_SUCCESS if parameters were valid and execution successful
 *                  STATUS_COMMAND_WLENGTH_INVALID if wLength has incorrect value
 *                  E_INVALID_PARAMETER if id maps to a non existing component
 *                  E_FAILED if parameters were valid but execution failed
 */
static uint8_t _modifyBits(IRegistersType *registers, uint16_t wLength, const uint8_t *payload);


uint8_t _readBurst(IRegistersType *registers, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    if (wLengthIn != (2 * sizeof(AddrType)))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const AddrType regAddr = COMBINE(serialToHost, _, AddrType)(payloadIn);
    const AddrType count   = COMBINE(serialToHost, _, AddrType)(payloadIn + sizeof(regAddr));
    *wLengthOut            = count * sizeof(RegType);
    RegType *values        = (RegType *)(uintptr_t)(*payloadOut);

    return registers->readBurst(registers, regAddr, count, values);
}

uint8_t _writeBurst(IRegistersType *registers, uint16_t wLength, const uint8_t *payload)
{
    const uint16_t length = wLength - sizeof(AddrType);
    if (length % sizeof(RegType))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint16_t count   = length / sizeof(RegType);
    const RegType *values  = (RegType *)(uintptr_t)(payload);
    const AddrType regAddr = COMBINE(serialToHost, _, AddrType)(payload + length);
    return registers->writeBurst(registers, regAddr, count, values);
}

uint8_t _readBatch(IRegistersType *registers, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    const uint16_t elemSize = sizeof(AddrType);
    if (wLengthIn % elemSize)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }
    const uint8_t count = wLengthIn / elemSize;

    *wLengthOut     = count * sizeof(RegType);
    RegType *values = (RegType *)(uintptr_t)(*payloadOut);

    return registers->readBatch(registers, (const AddrType(*))(uintptr_t)(payloadIn), count, values);
}

uint8_t _writeBatch(IRegistersType *registers, uint16_t wLength, const uint8_t *payload)
{
    const uint16_t elemSize = sizeof(AddrType) + sizeof(RegType);
    if (wLength % elemSize)
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const uint8_t count = wLength / elemSize;

#ifndef IRegisters_Extended
    return registers->writeBatchArray(registers, (const RegType(*)[2])(uintptr_t)(payload), count);
#else
    BatchType values[count];

    for (unsigned int i = 0; i < count; i++)
    {
        values[i].address = COMBINE(serialToHost, _, AddrType)(payload);
        values[i].value   = COMBINE(serialToHost, _, RegType)(payload + sizeof(values[i].address));
        payload += elemSize;
    }

    return registers->writeBatch(registers, values, count);
#endif
}


uint8_t _setBits(IRegistersType *registers, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != (sizeof(AddrType) + sizeof(RegType)))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const AddrType regAddr = COMBINE(serialToHost, _, AddrType)(payload);
    const RegType bitmask  = COMBINE(serialToHost, _, RegType)(payload + sizeof(regAddr));
    return registers->setBits(registers, regAddr, bitmask);
}

uint8_t _clearBits(IRegistersType *registers, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != (sizeof(AddrType) + sizeof(RegType)))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const AddrType regAddr = COMBINE(serialToHost, _, AddrType)(payload);
    const RegType bitmask  = COMBINE(serialToHost, _, RegType)(payload + sizeof(regAddr));
    return registers->clearBits(registers, regAddr, bitmask);
}

uint8_t _modifyBits(IRegistersType *registers, uint16_t wLength, const uint8_t *payload)
{
    if (wLength != (sizeof(AddrType) + 2 * sizeof(RegType)))
    {
        return STATUS_COMMAND_WLENGTH_INVALID;
    }

    const AddrType regAddr     = COMBINE(serialToHost, _, AddrType)(payload);
    const RegType clearBitmask = COMBINE(serialToHost, _, RegType)(payload + sizeof(regAddr));
    const RegType setBitmask   = COMBINE(serialToHost, _, RegType)(payload + sizeof(regAddr) + sizeof(clearBitmask));
    return registers->modifyBits(registers, regAddr, clearBitmask, setBitmask);
}


uint8_t COMBINE(Commands_, IRegistersType, _read)(IRegistersType *registers, uint8_t bFunction, uint16_t wLength, uint8_t **payload)
{
    switch (bFunction)
    {
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t COMBINE(Commands_, IRegistersType, _write)(IRegistersType *registers, uint8_t bFunction, uint16_t wLength, const uint8_t *payload)
{
    switch (bFunction)
    {
        case FN_REGISTERS_WRITE_BURST:
            return _writeBurst(registers, wLength, payload);
            break;
        case FN_REGISTERS_SET_BITS:
            return _setBits(registers, wLength, payload);
            break;
        case FN_REGISTERS_CLEAR_BITS:
            return _clearBits(registers, wLength, payload);
            break;
        case FN_REGISTERS_MODIFY_BITS:
            return _modifyBits(registers, wLength, payload);
            break;
        case FN_REGISTERS_BATCH:
            return _writeBatch(registers, wLength, payload);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

uint8_t COMBINE(Commands_, IRegistersType, _transfer)(IRegistersType *registers, uint8_t bFunction, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut)
{
    switch (bFunction)
    {
        case FN_REGISTERS_READ_BURST:
            return _readBurst(registers, wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        case FN_REGISTERS_BATCH:
            return _readBatch(registers, wLengthIn, payloadIn, wLengthOut, payloadOut);
            break;
        default:
            return STATUS_COMMAND_FUNCTION_INVALID;
            break;
    }
}

/*  @} */
