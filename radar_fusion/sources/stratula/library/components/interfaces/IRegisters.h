/**
 * \addtogroup      IRegisters
 * \brief
 * Device registers access interface
 * @{
 */
//#ifndef I_REGISTERS_H
//#define I_REGISTERS_H 1

#include <common/errors.h>
#include <stdbool.h>
#include <stdint.h>


#ifdef IRegisters_Extended
#undef IRegisters_Extended
#endif

#ifdef AddrType
#define IRegisters_Extended
#else
#define AddrType RegType
#endif


typedef struct
{
    AddrType address;
    RegType value;

} BatchType;


#define _IRegistersType COMBINE(_, _, IRegistersType)

typedef struct _IRegistersType IRegistersType;
struct _IRegistersType
{
    /**
    * Reads device register value.
    *
    * @param regAddr register address
    * @param readData address to store the read data to
    * @return Strata error code
    * @return error code
    */
    sr_t (*read)(IRegistersType *self, AddrType regAddr, RegType *readData);

    /**
    * Writes value to device register address.
    *
    * @param regAddr register address
    * @param value register value
    * @return Strata error code
    */
    sr_t (*write)(IRegistersType *self, AddrType regAddr, RegType value);

    /**
    * Reads a sequence of consecutive device register values from an initial start address.
    *
    * @param regAddr initial register address
    * @param count number of register values to be read
    * @param values receives sequence of register values
    * @return Strata error code
    */
    sr_t (*readBurst)(IRegistersType *self, AddrType regAddr, AddrType count, RegType values[]);

    /**
    * Writes a sequence of consecutive device register values from an initial start address.
    *
    * @param regAddr initial register address
    * @param count number of register values to be written
    * @param values sequence of register values
    * @return Strata error code
    */
    sr_t (*writeBurst)(IRegistersType *self, AddrType regAddr, AddrType count, const RegType values[]);

    /**
    * Writes a sequence of different device register values to their respective addresses.
    *
    * @param BatchType sequence of batches of address and value pairs
    * @param count number of register batches to be written
    * @return Strata error code
    */
    sr_t (*writeBatch)(IRegistersType *self, const BatchType values[], AddrType count);

#ifndef IRegisters_Extended
    /**
    * Writes a sequence of different device register values to their respective addresses.
    *
    * @param regVals sequence of register address and value pairs
    * @param count number of register values to be written
    * @return Strata error code
    */
    sr_t (*writeBatchArray)(IRegistersType *self, const RegType regVals[][2], AddrType count);
#endif

    /**
    * Reads a sequence of different device register values from their respective addresses.
    *
    * @param AddrType sequence of register addresses
    * @param count number of register addresses to read from
    * @param values receives sequence of register values
    * @return Strata error code
    */
    sr_t (*readBatch)(IRegistersType *self, const AddrType regAddr[], AddrType count, RegType value[]);

    /**
    * Sets specific bits on a device register.
    *
    * @param regAddr register address
    * @param bitmask bitmask to be set
    * @return Strata error code
    */
    sr_t (*setBits)(IRegistersType *self, AddrType regAddr, RegType bitmask);

    /**
    * Clears specific bits on a device register.
    *
    * @param regAddr register address
    * @param bitmask bitmask to be cleared
    * @return Strata error code
    */
    sr_t (*clearBits)(IRegistersType *self, AddrType regAddr, RegType bitmask);

    /**
    * Modifies specific bits on a device register.
    * clearBitmask takes precedence over setBitmask!
    *
    * @param regAddr register address
    * @param clearBitmask bitmask to be cleared (done first)
    * @param setBitmask bitmask to be set (done second)
    * @return Strata error code
    */
    sr_t (*modifyBits)(IRegistersType *self, AddrType regAddr, RegType clearBitmask, RegType setBitmask);
};


//#endif /* I_REGISTERS_H */

/** @} */
