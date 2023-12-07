
#ifndef SPILINKERCONFIG_H_
#define SPILINKERCONFIG_H_ 1

#include <common/typeutils.h>
#include <stdint.h>


/**
 * SpiLinker's remote-masters
 */
#define SPILINKER_REMOTE_MASTER_0 (0xA0)
#define SPILINKER_REMOTE_MASTER_1 (0xA1)
#define SPILINKER_REMOTE_MASTER_2 (0xA2)
#define SPILINKER_REMOTE_MASTER_3 (0xA3)

/**
 * SpiLinker's slave-select lines on a remote-master
 */
#define SPILINKER_SS1 (0xA4)
#define SPILINKER_SS2 (0xA5)

/**
 * SpiLinker's master
 */
typedef struct
{
    uint8_t devId;     ///< device id associated to the SpiLinker
    uint16_t gpioRo;   ///< RO: Read only
    uint16_t gpioBne;  ///< Bne: Buffer not empty
} SpiLinkerConfig_Master;

/**
 * Configuration of devices connected to the SpiLinker (on the remote side)
 */
typedef struct
{
    const SpiLinkerConfig_Master *localMaster;  ///< the SpiLinker's local-master
    uint8_t remoteMaster;                       ///< the the remote master id
    uint8_t slaveSelect;                        ///< the slave-select id
    uint16_t gpioSSel;                          ///< the slave-select pin
    uint8_t config;                             ///< configuration flags
} SpiLinkerConfig_Device;

#endif /* SPILINKERCONFIG_H_ */
