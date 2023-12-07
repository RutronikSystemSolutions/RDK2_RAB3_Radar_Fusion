#ifndef CHRONO_H
#define CHRONO_H 1

#include <FreeRTOS.h>
#include <task.h>

#include <stdbool.h>
#include <stdint.h>


typedef TickType_t chrono_ticks_t; /**< \brief Type for time information in clock ticks */


/**< \brief Converts clock ticks into micro seconds
 *
 * \param ticks amount of clock ticks
 * \return equivalent time in micro seconds
 */
static inline unsigned int chrono_ticks_to_microseconds(chrono_ticks_t ticks)
{
    return ticks / pdMS_TO_TICKS(1000);
}

/**< \brief Converts clock ticks into milli seconds
 *
 * \param ticks amount of clock ticks
 * \return equivalent time in milli seconds
 */
static inline unsigned int chrono_ticks_to_milliseconds(chrono_ticks_t ticks)
{
    return ticks / pdMS_TO_TICKS(1);
}

/**< \brief Converts micro seconds into clock ticks
 *
 * \param duration time in micro seconds
 * \return equivalent time in clock ticks
 */
static inline chrono_ticks_t chrono_microseconds(unsigned int duration)
{
    return (pdMS_TO_TICKS(duration) + 999) / 1000;
}

/**< \brief Converts milli seconds into clock ticks
 *
 * \param duration time in milli seconds
 * \return equivalent time in clock ticks
 */
static inline chrono_ticks_t chrono_milliseconds(unsigned int duration)
{
    return pdMS_TO_TICKS(duration);
}

/** \brief Returns system timer value
 */
static inline chrono_ticks_t chrono_now(void)
{
    // we do not call the ISR version, since even in an ISR we usually want to get the time from when the ISR is called.
    // the time elapsed during an ISR should not matter.
    return xTaskGetTickCount();
}

/** \brief Returns the elapsed time between the current time and the timepoint given as parameter.
 */
static inline chrono_ticks_t chrono_elapsed_since(chrono_ticks_t timepoint)
{
    return chrono_now() - timepoint;
}

/** \brief Returns whether the timepoint given as parameter has passed
 */
static inline bool chrono_has_passed(chrono_ticks_t timepoint)
{
    const chrono_ticks_t diff = 1u << (sizeof(chrono_ticks_t) * 8 - 1);  // handle unsigned integer overflow
    return chrono_elapsed_since(timepoint) < diff;
}

/** \brief Returns a timepoint when duration given as parameter has passed.
 */
static inline chrono_ticks_t chrono_get_timepoint(chrono_ticks_t duration)
{
    return chrono_now() + duration;
}


#endif /* CHRONO_H */
