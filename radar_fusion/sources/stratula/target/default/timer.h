#ifndef TIMER_H
#define TIMER_H 1

#include <common/errors.h>
#include <impl/chrono.h>
#include <stddef.h>


typedef void (*Timer_Callback)(void *arg);
typedef unsigned int TimerId_t;

typedef chrono_ticks_t timer_ticks_t;
#define timer_milliseconds chrono_milliseconds
#define timer_microseconds chrono_microseconds


void Timer_Constructor(void);
void Timer_run(void);

TimerId_t Timer_add(void);
sr_t Timer_setCallback(TimerId_t timerId, Timer_Callback callback, void *arg);
sr_t Timer_start(TimerId_t timerId, timer_ticks_t interval);
sr_t Timer_setInterval(TimerId_t timerId, timer_ticks_t interval);
sr_t Timer_stop(TimerId_t timerId);


#endif /* TIMER_H */
