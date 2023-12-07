#ifndef CHRONO_H
#define CHRONO_H 1

#include <stdbool.h>
#include <stdint.h>

typedef unsigned int chrono_ticks_t; /**< \brief Type for time information in clock ticks */


static chrono_ticks_t chrono_now(void); /**< \brief This needs to be defined returning the current milliseconds */


/**< \brief Converts clock ticks into micro seconds
 *
 * \param ticks amount of clock ticks
 * \return equivalent time in micro seconds
 */
static inline unsigned int chrono_ticks_to_microseconds(chrono_ticks_t ticks)
{
    return ticks * 1000;
}

/**< \brief Converts clock ticks into milli seconds
 *
 * \param ticks amount of clock ticks
 * \return equivalent time in milli seconds
 */
static inline unsigned int chrono_ticks_to_milliseconds(chrono_ticks_t ticks)
{
    return ticks;
}

/**< \brief Converts micro seconds into clock ticks
 *
 * \param duration time in micro seconds
 * \return equivalent time in clock ticks
 */
static inline chrono_ticks_t chrono_microseconds(unsigned int duration)
{
    return duration / 1000;
}

/**< \brief Converts milli seconds into clock ticks
 *
 * \param duration time in milli seconds
 * \return equivalent time in clock ticks
 */
static inline chrono_ticks_t chrono_milliseconds(unsigned int duration)
{
    return duration;
}

/** \brief Returns the elapsed time between the current time and the timepoint given as parameter.
 */
static inline chrono_ticks_t chrono_elapsed_since(chrono_ticks_t timepoint)
{
    return chrono_now() - timepoint;
}

/** \brief Returns whether the timepoint given as parameter has passed.
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
