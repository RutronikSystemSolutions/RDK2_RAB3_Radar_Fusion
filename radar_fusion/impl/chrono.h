/* ===========================================================================
** Copyright (C) 2021 Infineon Technologies AG
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
** 1. Redistributions of source code must retain the above copyright notice,
**    this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
** ===========================================================================
*/

#ifndef CHRONO_H
#define CHRONO_H 1

#include <stdbool.h>
#include <stdint.h>

#include "system_timer.h"

typedef uint32_t chrono_ticks_t; /**< \brief Type for time information in clock ticks */

/**< \brief Converts clock ticks into micro seconds
 *
 * \param ticks amount of clock ticks
 * \return equivalent time in micro seconds
 */
static inline uint64_t chrono_ticks_to_microseconds(chrono_ticks_t ticks)
{
    return ticks * TICK_TIME_MICROSECS;
}

/**< \brief Converts clock ticks into milli seconds
 *
 * \param ticks amount of clock ticks
 * \return equivalent time in milli seconds
 */
static inline uint64_t chrono_ticks_to_milliseconds(chrono_ticks_t ticks)
{
    return ticks * TICK_TIME_MICROSECS / 1000;
}

/**< \brief Converts micro seconds into clock ticks
 *
 * \param duration time in micro seconds
 * \return equivalent time in clock ticks
 */
static inline chrono_ticks_t chrono_microseconds(uint64_t duration)
{
    return (duration + TICK_TIME_MICROSECS - 1) / TICK_TIME_MICROSECS;
}

/**< \brief Converts milli seconds into clock ticks
 *
 * \param duration time in milli seconds
 * \return equivalent time in clock ticks
 */
static inline chrono_ticks_t chrono_milliseconds(uint64_t duration)
{
    return duration * 1000 / TICK_TIME_MICROSECS;
}

/** \brief Returns system timer value.
 *
 * \return system timer value, in clock ticks.
 */
static inline chrono_ticks_t chrono_now(void)
{
    return SysTimer_getTime();
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
