/**
 * \addtogroup      PinsAvian
 * \brief
 * Implementation of radar component GPIO access
 * @{
 */
#ifndef PINS_AVIAN_H
#define PINS_AVIAN_H 1

#include <components/interfaces/IPinsAvian.h>
#include <platform/interfaces/IGpio.h>


typedef struct _PinsAvian PinsAvian;
struct _PinsAvian
{
    IPinsAvian b_IPinsAvian;

    IGpio *m_accessGpio;
    const IPinsAvianConfig_t *m_config;
};


void PinsAvian_Constructor(PinsAvian *self, IGpio *accessGpio, const IPinsAvianConfig_t *config);


sr_t PinsAvian_reset(IPinsAvian *self);

sr_t PinsAvian_setResetPin(IPinsAvian *self, bool state);
sr_t PinsAvian_getIrqPin(IPinsAvian *self, bool *state);

#endif /* PINS_AVIAN_H */

/** @} */
