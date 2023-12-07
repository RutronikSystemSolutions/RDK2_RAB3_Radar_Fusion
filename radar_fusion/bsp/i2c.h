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

#include <impl/PlatformI2cDefinition.h>


/* Speed is set to Standard Mode. MCU supports up to full speed (400KHz) */

PlatformI2cDefinition_t BoardI2cDefinition[] = {

    [0] = {
//        .twihs     = TWIHS2,
//        .id_twihs  = ID_TWIHS2,
//        .scl       = PIO_PD28_IDX,
//        .scl_flags = IOPORT_MODE_MUX_C,
//        .sda       = PIO_PD27_IDX,
//        .sda_flags = IOPORT_MODE_MUX_C,
        .speed     = 400000,
    },
    [1] = {
//        .twihs     = TWIHS0,
//        .id_twihs  = ID_TWIHS0,
//        .scl       = PIO_PA4_IDX,
//        .scl_flags = IOPORT_MODE_MUX_A,
//        .sda       = PIO_PA3_IDX,
//        .sda_flags = IOPORT_MODE_MUX_A,
        .speed     = 400000,
    },
};
