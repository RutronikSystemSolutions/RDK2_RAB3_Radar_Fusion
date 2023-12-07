#include "Module.h"
#include <stddef.h>

#include <timer.h>

static void Module_triggerMeasurement(Module *self)
{
    self->m_doMeasurementAfterData = false;

    IModule *derived = self->m_derived;
    derived->doMeasurement(derived);
}

static void Module_timerCallback(void *arg)
{
    Module *self = (Module *)arg;

    if (self->m_currentState == MODULE_MEASURING)
    {
        self->m_doMeasurementAfterData = true;
    }
    else
    {
        Module_triggerMeasurement(self);
    }
}

void Module_dataCallback(void *arg, uint32_t data[][2], uint8_t count, uint8_t channel, uint64_t timestamp)
{
    Module *self = (Module *)arg;

    Led_set(self->m_ledMeasuring, false);

    if (self->m_dataCallback != NULL)
    {
        Led_set(self->m_ledTransferring, true);
        self->m_dataCallback(self->m_dataCallbackArg, data, count, channel, timestamp);
        Led_set(self->m_ledTransferring, false);
    }

    //If it has not been stopped, check if this is the last channel of one measurement
    if (self->m_currentState != MODULE_STOPPED)
    {
        if ((self->m_lastChannel == 0) || (self->m_lastChannel == channel))
        {
            // check if it is time to directly trigger a new measurement,
            self->m_currentState = MODULE_READY;
            if (self->m_doMeasurementAfterData)
            {
                Module_triggerMeasurement(self);
            }
        }
    }
}

void Module_registerDataCallback(Module *self, IProcessing_DataCallback dataCallback, void *arg)
{
    self->m_dataCallback    = dataCallback;
    self->m_dataCallbackArg = arg;
}

void Module_configure(Module *self, uint8_t measurementMode, uint8_t lastChannel)
{
    self->m_currentState    = MODULE_STOPPED;
    self->m_measurementMode = measurementMode;
    self->m_lastChannel     = lastChannel;
}

void Module_reset(Module *self)
{
    Module_stopMeasurements(self);
    self->m_currentState = MODULE_UNCONFIGURED;
}

sr_t Module_startMeasurements(Module *self, double measurementCycle)
{
    if (self->m_currentState == MODULE_UNCONFIGURED)
    {
        return E_NOT_POSSIBLE;
    }
    else if (self->m_currentState == MODULE_STOPPED)
    {
        self->m_currentState = MODULE_READY;
    }

    switch (self->m_measurementMode)
    {
        case MEASUREMENT_MODE_TIMER:
            if (measurementCycle == 0)
            {
                Timer_stop(self->m_timerId);
            }
            else
            {
                Timer_start(self->m_timerId, timer_milliseconds(measurementCycle * 1000));
            }
            break;
        case MEASUREMENT_MODE_MANUAL:
        case MEASUREMENT_MODE_CONTINUOUS:
            break;
        default:
            return E_UNEXPECTED_VALUE;
    }
    return E_SUCCESS;
}

void Module_stopMeasurements(Module *self)
{
    if (self->m_currentState == MODULE_STOPPED)
    {
        return;
    }

    Led_set(self->m_ledMeasuring, false);
    if (self->m_measurementMode == MEASUREMENT_MODE_TIMER)
    {
        Timer_stop(self->m_timerId);
        self->m_doMeasurementAfterData = false;
    }
    self->m_currentState = MODULE_STOPPED;
}

sr_t Module_doMeasurement(Module *self)
{
    if (self->m_currentState != MODULE_READY)
    {
        return E_NOT_AVAILABLE;
    }
    self->m_currentState = MODULE_MEASURING;
    Led_set(self->m_ledMeasuring, true);
    return E_SUCCESS;
}

void Module_Constructor(Module *self, IModule *derived, const LedDefinition_t *ledMeasuring, const LedDefinition_t *ledTransferring)
{
    self->m_derived         = derived;
    self->m_currentState    = MODULE_UNCONFIGURED;
    self->m_measurementMode = MEASUREMENT_MODE_MANUAL;
    self->m_dataCallback    = NULL;

    self->m_doMeasurementAfterData = false;
    self->m_lastChannel            = 0;

    self->m_ledMeasuring    = ledMeasuring;
    self->m_ledTransferring = ledTransferring;
    Led_initialize(self->m_ledMeasuring);
    Led_initialize(self->m_ledTransferring);

    self->m_timerId = Timer_add();
    Timer_setCallback(self->m_timerId, Module_timerCallback, self);
}
