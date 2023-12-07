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

/**
 * \file RbbLed.c
 *
 * \brief Implements/consolidates the functionality related to the Radar Baseboard
 *        LED PWM Error Signaling
 *
 */

#include "LedSequenceRbb.h"

#include <common/typeutils.h>


/* Define basic pattern, to be repeated for different board error scenarios.
   unit structure format is { State(RED), State(GREEN), State(BLUE), Duration}
   where Duration is in units of 100ms 
   */

/* short_green, short_red */
static const LedRgb_t noRfShield[] = {
    {LED_ON, LED_OFF, LED_OFF, 5},
    {LED_OFF, LED_ON, LED_OFF, 5},
};

/* short_red, short_OFF */
static const LedRgb_t wrongOrientation[] = {
    {LED_ON, LED_OFF, LED_OFF, 5},
    {LED_OFF, LED_OFF, LED_OFF, 5},
};

/* short_yellow, short_red */
static const LedRgb_t unknownShield[] = {
    {LED_ON, LED_ON, LED_OFF, 5},
    {LED_ON, LED_OFF, LED_OFF, 5},
};

/* short_yellow, short_red, long_yellow, long_red */
static const LedRgb_t initializationFailed[] = {
    {LED_ON, LED_ON, LED_OFF, 5},
    {LED_ON, LED_OFF, LED_OFF, 5},
    {LED_ON, LED_ON, LED_OFF, 10},
    {LED_ON, LED_OFF, LED_OFF, 10},
};

/* short_red, short_OFF, long_yellow, long_OFF */
static const LedRgb_t i2cInitializationFailed[] = {
    {LED_ON, LED_OFF, LED_OFF, 5},
    {LED_OFF, LED_OFF, LED_OFF, 5},
    {LED_ON, LED_ON, LED_OFF, 10},
    {LED_OFF, LED_OFF, LED_OFF, 10},
};

/* short_yellow, short_OFF, long_red, long_OFF */
static const LedRgb_t internalError[] = {
    {LED_ON, LED_ON, LED_OFF, 5},
    {LED_OFF, LED_OFF, LED_OFF, 5},
    {LED_ON, LED_OFF, LED_OFF, 10},
    {LED_OFF, LED_OFF, LED_OFF, 10},
};


void LedSequence_setRbbStatus(int code)
{
    switch (code)
    {
        case RBB_ERROR_HARDWARE_NOT_DETECTED:
            LedSequence_set(noRfShield, ARRAY_SIZE(noRfShield));
            break;
        case RBB_ERROR_HARDWARE_CONNECTED_WRONG:
            LedSequence_set(wrongOrientation, ARRAY_SIZE(wrongOrientation));
            break;
        case RBB_ERROR_HARDWARE_NOT_SUPPORTED:
            LedSequence_set(unknownShield, ARRAY_SIZE(unknownShield));
            break;
        case RBB_ERROR_HARDWARE_INITIALIZATION_FAILED:
        case FATAL_ERROR_SPI_CONFIG_FAILED:
            LedSequence_set(initializationFailed, ARRAY_SIZE(initializationFailed));
            break;
        case FATAL_ERROR_I2C_CONFIG_FAILED:
            LedSequence_set(i2cInitializationFailed, ARRAY_SIZE(i2cInitializationFailed));
            break;
        case RBB_ERROR_HARDWARE_INTERNAL_ERROR:
        default:
            LedSequence_set(internalError, ARRAY_SIZE(internalError));
            break;
    }
}
