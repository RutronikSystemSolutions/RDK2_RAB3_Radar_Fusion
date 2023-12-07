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
 * \file LedSequence.c
 *
 * \brief contains functionality related to LED sequence configuration and signaling
 *
 */

#include "LedSequence.h"
#include "Led.h"

#include <impl/thread.h>
#include <stddef.h>


extern const LedDefinition_t ledSequenceDefinition[3];


static unsigned int m_index       = 0;
static const LedRgb_t *m_sequence = NULL;
static uint8_t m_length;
static chrono_ticks_t m_timeout;


static inline void setColor(const LedRgb_t *color)
{
    Led_set(&ledSequenceDefinition[0], color->r);
    Led_set(&ledSequenceDefinition[1], color->g);
    Led_set(&ledSequenceDefinition[2], color->b);
}

static void LedSequence_Callback(void)
{
    m_index = (m_index + 1) % m_length;
    setColor(&m_sequence[m_index]);
}

void LedSequence_set(const LedRgb_t *sequence, uint8_t length)
{
    m_sequence = sequence;
    m_length   = length;
    m_index    = 0;

    setColor(&m_sequence[m_index]);
    m_timeout = chrono_get_timepoint(m_sequence[m_index].ticks * 100UL);
}

void LedSequence_Constructor(void)
{
    Led_initialize(&ledSequenceDefinition[0]);
    Led_initialize(&ledSequenceDefinition[1]);
    Led_initialize(&ledSequenceDefinition[2]);
}

void LedSequence_run(void)
{
    if (m_sequence == NULL)
    {
        return;
    }

    if (chrono_has_passed(m_timeout))
    {
        LedSequence_Callback();
        m_timeout += chrono_milliseconds(m_sequence[m_index].ticks * 100UL);
    }
}
