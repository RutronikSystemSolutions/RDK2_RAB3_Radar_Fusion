#include "Led.h"
#include <impl/PlatformGpio.h>
#include <stddef.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

sr_t Led_initialize(const LedDefinition_t *led)
{
	return E_SUCCESS;
}

sr_t Led_set(const LedDefinition_t *led, bool state)
{
    if (led == NULL)
    {
        return E_SUCCESS;
    }

    if(led->gpio == LED1)
    {
    	if(state)
    	{
    		cyhal_gpio_write(LED1, CYBSP_LED_STATE_ON);
    	}
    	else
    	{
    		cyhal_gpio_write(LED1, CYBSP_LED_STATE_OFF);
    	}
    }

    else if(led->gpio  == LED2)
    {
    	if(state)
    	{
    		cyhal_gpio_write(LED2, CYBSP_LED_STATE_ON);
    	}
    	else
    	{
    		cyhal_gpio_write(LED2, CYBSP_LED_STATE_OFF);
    	}
    }

    return E_SUCCESS;
}
