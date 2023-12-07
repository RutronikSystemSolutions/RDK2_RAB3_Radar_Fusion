
#ifndef SPILINKER_H_
#define SPILINKER_H_ 1

#include "SpiLinkerConfig.h"
#include <platform/interfaces/IGpio.h>
#include <platform/interfaces/ISpi.h>

extern ISpi SpiLinker;

void SpiLinker_Constructor(ISpi *accessSpi, IGpio *accessGpio,
                           const SpiLinkerConfig_Master *masters, uint8_t masterCount,
                           const SpiLinkerConfig_Device *devices, uint8_t deviceCount);


#endif /* SPILINKER_H_ */
