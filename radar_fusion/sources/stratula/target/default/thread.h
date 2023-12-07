#ifndef THREAD_H
#define THREAD_H 1

#include <impl/chrono.h>


/** \brief Wait for a minimum duration.
 *
 * \param rel_time Specifies the waiting time duration, in clock ticks.
 */
static inline void this_thread_sleep_for(chrono_ticks_t rel_time)
{
    const chrono_ticks_t deadline = chrono_now() + rel_time;
    while (chrono_now() < deadline)
        ;
}

#endif /* THREAD_H */
