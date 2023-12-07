/*
 * \addtogroup      PinsLtr11.h
 * \brief
 * Implementation of radar component GPIO access
 * @{
 */

#ifndef PINSLTR11_H_
#define PINSLTR11_H_ 1

#include <components/interfaces/IPinsLtr11.h>
#include <platform/interfaces/IGpio.h>


typedef struct _PinsLtr11 PinsLtr11;
struct _PinsLtr11
{
    IPinsLtr11 b_IPinsLtr11;

    IGpio *m_accessGpio;
    const IPinsLtr11Config_t *m_config;
};


void PinsLtr11_Constructor(PinsLtr11 *self, IGpio *accessGpio, const IPinsLtr11Config_t *config);


sr_t PinsLtr11_reset(IPinsLtr11 *self);

sr_t PinsLtr11_setResetPin(IPinsLtr11 *self, bool state);

sr_t PinsLtr11_getIrqPin(IPinsLtr11 *self, bool *state);
sr_t PinsLtr11_getDetectionPins(IPinsLtr11 *self, uint8_t *states);

#endif /* PINSLTR11_H_ */

/** @} */