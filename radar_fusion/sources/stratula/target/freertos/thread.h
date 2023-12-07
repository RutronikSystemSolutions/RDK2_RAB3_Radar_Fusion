
#include "chrono.h"

/** \brief Wait for a minimum duration.
 *
 * \param rel_time Specifies the waiting time duration, in clock ticks.
 */
static inline void this_thread_sleep_for(chrono_ticks_t rel_time)
{
    vTaskDelay(rel_time);
}
