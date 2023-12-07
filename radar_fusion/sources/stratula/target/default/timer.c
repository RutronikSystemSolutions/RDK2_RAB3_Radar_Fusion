#include "timer.h"
#include <stddef.h>

#define MAX_TIMERS 4


typedef struct
{
    timer_ticks_t m_interval;
    timer_ticks_t m_timeout;

    Timer_Callback m_callback;
    void *m_callbackArg;
} Timer;


static Timer m_timers[MAX_TIMERS];
static TimerId_t m_count = 0;


TimerId_t Timer_add(void)
{
    if (m_count == MAX_TIMERS)
    {
        return (TimerId_t)-1;
    }

    m_timers[m_count].m_interval = 0;
    m_timers[m_count].m_callback = NULL;

    return m_count++;
}

sr_t Timer_setCallback(TimerId_t timerId, Timer_Callback callback, void *arg)
{
    if (timerId >= m_count)
    {
        return E_OUT_OF_BOUNDS;
    }

    m_timers[timerId].m_callback    = callback;
    m_timers[timerId].m_callbackArg = arg;

    return E_SUCCESS;
}

sr_t Timer_start(TimerId_t timerId, timer_ticks_t interval)
{
    if (timerId >= m_count)
    {
        return E_OUT_OF_BOUNDS;
    }

    m_timers[timerId].m_interval = interval;
    m_timers[timerId].m_timeout  = chrono_now();

    if (m_timers[timerId].m_interval == 0)
    {
        m_timers[timerId].m_interval = 1;
    }
    return E_SUCCESS;
}

sr_t Timer_setInterval(TimerId_t timerId, timer_ticks_t interval)
{
    if (timerId >= m_count)
    {
        return E_OUT_OF_BOUNDS;
    }

    m_timers[timerId].m_interval = interval;

    if (m_timers[timerId].m_interval == 0)
    {
        m_timers[timerId].m_interval = 1;
    }
    return E_SUCCESS;
}

sr_t Timer_stop(TimerId_t timerId)
{
    if (timerId >= m_count)
    {
        return E_OUT_OF_BOUNDS;
    }

    m_timers[timerId].m_interval = 0;

    return E_SUCCESS;
}

void Timer_run(void)
{
    for (unsigned int i = 0; i < m_count; i++)
    {
        if (m_timers[i].m_interval)
        {
            if (chrono_now() >= (m_timers[i].m_timeout))
            {
                if (m_timers[i].m_callback)
                {
                    m_timers[i].m_callback(m_timers[i].m_callbackArg);
                }

                m_timers[i].m_timeout += m_timers[i].m_interval;
            }
        }
    }
}

void Timer_Constructor(void)
{
}
