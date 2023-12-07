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

#include "LedSequenceStatus.h"
#include <common/typeutils.h>


/* Define basic pattern, to be repeated for different board error scenarios.
   unit structure format is { State(RED), State(GREEN), State(BLUE), Duration}
   where Duration is in units of 100ms 
   */

/* short_off, short_green */
static const LedRgb_t operating[] = {
    {LED_OFF, LED_OFF, LED_OFF, 5},
    {LED_OFF, LED_ON, LED_OFF, 5},
};

/* red */
static const LedRgb_t measuring[] = {
    {LED_ON, LED_OFF, LED_OFF, 0xFF},
};

/* blue */
static const LedRgb_t transferring[] = {
    {LED_OFF, LED_OFF, LED_ON, 0xFF},
};


void LedSequence_setStatus(int code)
{
    switch (code)
    {
        case LED_STATUS_OPERATING:
            LedSequence_set(operating, ARRAY_SIZE(operating));
            break;
        case LED_STATUS_MEASURING:
            LedSequence_set(measuring, ARRAY_SIZE(measuring));
            break;
        case LED_STATUS_TRANSFERRING:
            LedSequence_set(transferring, ARRAY_SIZE(transferring));
            break;
        default:
            break;
    }
}
