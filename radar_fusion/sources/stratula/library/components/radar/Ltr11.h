/**
 * \addtogroup      Ltr11.h
 *
 * \brief
 * Implementation of Ltr11 radar component
 * @{
 */


#ifndef LTR11_H_
#define LTR11_H_ 1

#include <stdbool.h>
#include <stdint.h>

#include "ltr11/PinsLtr11.h"
#include "ltr11/ProtocolLtr11.h"
#include "ltr11/RegistersLtr11.h"
#include <components/interfaces/IRadarLtr11.h>
#include <platform/interfaces/IData.h>


typedef struct _Ltr11 Ltr11;

void Ltr11_Constructor(Ltr11 *self, IData *accessData, IGpio *accessGpio, ISpi *accessSpi, const BoardRadarConfig_t *boardConfig, const IPinsLtr11Config_t *pinsLtr11Config, const PlatformInterruptDefinition_t *irqConfig);

sr_t Ltr11_Detect(ISpi *accessSpi, const BoardRadarConfig_t *boardConfig);

//IRadar
sr_t Ltr11_reset(IRadar *self, bool softReset);
sr_t Ltr11_initialize(IRadar *self);
sr_t Ltr11_getDataIndex(IRadar *self, uint8_t *index);
sr_t Ltr11_startData(IRadar *self);
sr_t Ltr11_stopData(IRadar *self);

//IRadarLtr11
IRegisters8_16 *Ltr11_getIRegisters(IRadarLtr11 *self);
IPinsLtr11 *Ltr11_getIPinsLtr11(IRadarLtr11 *self);
IProtocolLtr11 *Ltr11_getIProtocolLtr11(IRadarLtr11 *self);


struct _Ltr11
{
    IRadarLtr11 b_IRadarLtr11;

    RegistersLtr11 m_registers;
    PinsLtr11 m_pins;
    ProtocolLtr11 m_protocol;

    IData *m_accessData;
    uint8_t m_dataIndex;

    bool m_initialized;

    /** \private @{ */
    /** @} */
};

/** @} */

#endif /* LTR11_H_ */
