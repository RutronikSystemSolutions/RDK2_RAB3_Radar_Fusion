#include "PinsLtr11.h"

#include <impl/thread.h>

#define SELF ((PinsLtr11 *)(uintptr_t)self)

//TODO: get the specific reset pulse width from AE
#define RESET_PULSE_WIDTH_MS 1 /*!< in milliseconds (1ms according to data sheet)
                                    The RESET signal needs to be held low for this period.
                                */

sr_t PinsLtr11_setResetPin(IPinsLtr11 *self, bool state)
{
    return SELF->m_accessGpio->setPin(SELF->m_config->gpioReset, state);
}

sr_t PinsLtr11_reset(IPinsLtr11 *self)
{
    RETURN_ON_ERROR(PinsLtr11_setResetPin(self, false));
    this_thread_sleep_for(chrono_milliseconds(RESET_PULSE_WIDTH_MS));  // SPI_RESET_PIN
    RETURN_ON_ERROR(PinsLtr11_setResetPin(self, true));
    this_thread_sleep_for(chrono_milliseconds(RESET_PULSE_WIDTH_MS));  // SPI_RESET_PIN

    return E_SUCCESS;
}

sr_t PinsLtr11_getIrqPin(IPinsLtr11 *self, bool *state)
{
    return SELF->m_accessGpio->getPin(SELF->m_config->gpioIrq, state);
}

sr_t PinsLtr11_getDetectionPins(IPinsLtr11 *self, uint8_t *states)
{
    *states = 0;
    bool state;

    RETURN_ON_ERROR(SELF->m_accessGpio->getPin(SELF->m_config->targetDet, &state));
    if (state)
    {
        *states |= IPINSLTR11_TARGET_DET;
    }

    RETURN_ON_ERROR(SELF->m_accessGpio->getPin(SELF->m_config->phaseDet, &state));
    if (state)
    {
        *states |= IPINSLTR11_PHASE_DET;
    }

    return E_SUCCESS;
}

void PinsLtr11_Constructor(PinsLtr11 *self, IGpio *accessGpio, const IPinsLtr11Config_t *config)
{
    self->b_IPinsLtr11.setResetPin      = PinsLtr11_setResetPin;
    self->b_IPinsLtr11.reset            = PinsLtr11_reset;
    self->b_IPinsLtr11.getIrqPin        = PinsLtr11_getIrqPin;
    self->b_IPinsLtr11.getDetectionPins = PinsLtr11_getDetectionPins;

    {
        self->m_config     = config;
        self->m_accessGpio = accessGpio;
    }

    self->m_accessGpio->configurePin(self->m_config->gpioReset, GPIO_MODE_OUTPUT_PUSH_PULL | GPIO_FLAG_OUTPUT_INITIAL_HIGH);
    self->m_accessGpio->configurePin(self->m_config->targetDet, GPIO_MODE_INPUT);
    self->m_accessGpio->configurePin(self->m_config->phaseDet, GPIO_MODE_INPUT);
}
