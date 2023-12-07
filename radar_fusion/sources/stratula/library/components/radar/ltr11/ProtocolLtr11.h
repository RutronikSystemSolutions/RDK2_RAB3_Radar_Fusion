/*
 * \addtogroup      ProtocolLtr11.h
 * \brief
 * Implementation of radar command interface
 * @{
 */


#ifndef PROTOCOLLTR11_H_
#define PROTOCOLLTR11_H_ 1

#include <common/lock.h>
#include <components/interfaces/IProtocolLtr11.h>
#include <impl/PlatformInterrupt.h>
#include <timer.h>
#include <platform/interfaces/IGpio.h>
#include <platform/interfaces/ISpi.h>

typedef struct _ProtocolLtr11 ProtocolLtr11;
struct _ProtocolLtr11
{
    IProtocolLtr11 b_IProtocolLtr11;

    const PlatformInterruptDefinition_t *m_irqConfig;
    ISpi *m_accessSpi;
    uint8_t m_devId;
    LockType m_spiLock;
    LockType m_misoLock;
    TimerId_t m_timerId;
    timer_ticks_t m_interval;
    volatile bool m_allowSpiAccess;
    volatile bool m_misoArbitration;
    ReadoutCallback m_callback;
    void *m_callbackArg;
};


void ProtocolLtr11_Constructor(ProtocolLtr11 *self, ISpi *accessSpi, uint8_t devId, const PlatformInterruptDefinition_t *irqConfig);


sr_t ProtocolLtr11_executeWrite(IProtocolLtr11 *self, const uint8_t command[3]);
sr_t ProtocolLtr11_executeRead(IProtocolLtr11 *self, uint8_t command, uint16_t *value);
sr_t ProtocolLtr11_setBits(IProtocolLtr11 *self, uint8_t address, uint16_t bitMask);

sr_t ProtocolLtr11_executeWriteBatch(IProtocolLtr11 *self, const uint8_t commands[][3], uint16_t count);
sr_t ProtocolLtr11_executeWriteBurst(IProtocolLtr11 *self, const uint8_t command[2], uint16_t count, const uint8_t values[][2]);
sr_t ProtocolLtr11_executeReadBurst(IProtocolLtr11 *self, const uint8_t command[2], uint16_t count, uint16_t values[]);

sr_t ProtocolLtr11_setMisoArbitration(IProtocolLtr11 *self, uint16_t prt);

sr_t ProtocolLtr11_registerCallback(IProtocolLtr11 *self, ReadoutCallback callback, void *arg);

#endif /* PROTOCOLLTR11_H_ */

/** @} */
