#ifndef MODULE_H
#define MODULE_H 1

#include <modules/interfaces/IModule.h>
#include <platform/led/Led.h>


typedef struct _Module Module;

void Module_Constructor(Module *self, IModule *derived, const LedDefinition_t *ledMeasuring, const LedDefinition_t *ledTransferring);

void Module_run(Module *self);
void Module_dataCallback(void *arg, uint32_t data[][2], uint8_t count, uint8_t channel, uint64_t timestamp);
void Module_registerDataCallback(Module *self, IProcessing_DataCallback dataCallback, void *arg);

void Module_configure(Module *self, uint8_t measurementMode, uint8_t lastChannel);
void Module_reset(Module *self);
sr_t Module_startMeasurements(Module *self, double measurementCycle);
void Module_stopMeasurements(Module *self);
sr_t Module_doMeasurement(Module *self);


struct _Module
{
    IModule *m_derived;

    unsigned int m_currentState;
    unsigned int m_measurementMode;

    IProcessing_DataCallback m_dataCallback;
    void *m_dataCallbackArg;

    unsigned int m_timerId;
    bool m_doMeasurementAfterData;
    uint8_t m_lastChannel;

    const LedDefinition_t *m_ledMeasuring;
    const LedDefinition_t *m_ledTransferring;
};


#endif /* MODULE_H */
