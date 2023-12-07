#include "PinsAvian.h"

#include <impl/thread.h>
#include "spi_interface.h"


#define SELF ((PinsAvian *)(uintptr_t)self)


#define RESET_PULSE_WIDTH_US 1 /*!< in microseconds (100 ns according to data sheet)
                                    The RESET signal to needs to be held low for this period,
                                    and before communicating with the chip the same amount of time
                                    has to be waited after setting it to high */


sr_t PinsAvian_setResetPin(IPinsAvian *self, bool state)
{
    uint8_t flags = GPIO_MODE_OUTPUT_PUSH_PULL;
    if (state)
    {
        flags |= GPIO_FLAG_OUTPUT_INITIAL_HIGH;
    }
    return SELF->m_accessGpio->configurePin(SELF->m_config->gpioReset, flags);
}

sr_t PinsAvian_getIrqPin(IPinsAvian *self, bool *state)
{
    return SELF->m_accessGpio->getPin(SELF->m_config->gpioIrq, state);
}

sr_t PinsAvian_reset(IPinsAvian *self)
{
    //RETURN_ON_ERROR(PinsAvian_setResetPin(self, false));
    //this_thread_sleep_for(chrono_microseconds(RESET_PULSE_WIDTH_US));  // T_reset
    //RETURN_ON_ERROR(PinsAvian_setResetPin(self, true));
    //this_thread_sleep_for(chrono_microseconds(RESET_PULSE_WIDTH_US));  // T_reset

	cyhal_gpio_write(RAD_RSTN, false);
	this_thread_sleep_for(chrono_microseconds(RESET_PULSE_WIDTH_US));  // T_reset
	cyhal_gpio_write(RAD_RSTN, true);
	this_thread_sleep_for(chrono_microseconds(RESET_PULSE_WIDTH_US));  // T_reset
    return E_SUCCESS;
}


void PinsAvian_Constructor(PinsAvian *self, IGpio *accessGpio, const IPinsAvianConfig_t *config)
{
    self->b_IPinsAvian.setResetPin = PinsAvian_setResetPin;
    self->b_IPinsAvian.getIrqPin   = PinsAvian_getIrqPin;
    self->b_IPinsAvian.reset       = PinsAvian_reset;

    {
        self->m_config     = config;
        self->m_accessGpio = accessGpio;
    }

    /* The reset pin also acts as QSPI DIO3 pin.
     * For this reason, it is (re)configured as GPIO/peripheral on demand.
     */

    // the IRQ should be triggered on the rising low-high edge, so the signal should be pulled into a defined low state,
    // so that spurious IRQs aren't generated whilst the pin is floating (i.e. during startup or whilst the BGT is in reset)
    self->m_accessGpio->configurePin(self->m_config->gpioIrq, GPIO_MODE_INPUT_PULL_DOWN);
}
