#ifndef LED_H
#define LED_H 1

#include <platform/interfaces/IGpio.h>


typedef struct
{
    uint16_t gpio;
    bool activeLow;
} LedDefinition_t;


/**
* Initialize LED
*
* @param led definition of LED
* @return Strata error code
*/
sr_t Led_initialize(const LedDefinition_t *led);

/**
* Turn on LED
*
* @param led definition of LED
* @param state whether LED should be off ( = false) or on ( = true)
* @return Strata error code
*/
sr_t Led_set(const LedDefinition_t *led, bool state);

#endif /* LED_H */
