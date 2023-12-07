/**
 * \addtogroup      Avian
 * \brief
 * Implementation of Avian radar component
 * @{
 */
#ifndef AVIAN_H
#define AVIAN_H 1

#include <stdbool.h>
#include <stdint.h>

#include "avian/PinsAvian.h"
#include "avian/ProtocolAvian.h"
#include "avian/RegistersAvian.h"
#include <components/interfaces/IRadarAvian.h>
#include <platform/interfaces/IData.h>


typedef struct _Avian Avian;

void Avian_Constructor(Avian *self, IData *accessData, IGpio *accessGpio, ISpi *accessSpi, const BoardRadarConfig_t *boardConfig, const IPinsAvianConfig_t *pinsAvianConfig);

sr_t Avian_Detect(ISpi *accessSpi, IGpio *accessGpio, const BoardRadarConfig_t *boardConfig, const IPinsAvianConfig_t *pinsAvianConfig);

//IRadar
sr_t Avian_reset(IRadar *self, bool softReset);
sr_t Avian_initialize(IRadar *self);
sr_t Avian_getDataIndex(IRadar *self, uint8_t *index);
sr_t Avian_startData(IRadar *self);
sr_t Avian_stopData(IRadar *self);

//IRadarAvian
IRegisters8_32 *Avian_getIRegisters(IRadarAvian *self);
IPinsAvian *Avian_getIPinsAvian(IRadarAvian *self);
IProtocolAvian *Avian_getIProtocolAvian(IRadarAvian *self);


struct _Avian
{
    IRadarAvian b_IRadarAvian;

    RegistersAvian m_registers;
    PinsAvian m_pins;
    ProtocolAvian m_protocol;

    IData *m_accessData;
    uint8_t m_dataIndex;

    bool m_initialized;

    /** \private @{ */
    /** @} */
};

/** @} */


#endif /* AVIAN_H */
