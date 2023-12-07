#ifndef LED_SEQUENCE_H_
#define LED_SEQUENCE_H_ 1

#include <stdint.h>


#define LED_ON  1
#define LED_OFF 0

typedef struct
{
    uint8_t r, g, b;
    uint8_t ticks; /* ticks in units of 100ms */
} LedRgb_t;


void LedSequence_Constructor(void);
void LedSequence_run(void);

void LedSequence_set(const LedRgb_t *sequence, uint8_t length);


#endif /* LED_SEQUENCE_H_ */
