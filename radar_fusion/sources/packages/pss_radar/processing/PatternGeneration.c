#include "PatternGeneration.h"


void PatternGeneration_rectangular(int16_t *data, uint8_t rxChannels, uint16_t samples, uint16_t chirps)
{
    for (uint16_t c = 0; c < chirps; c++)
    {
        uint16_t offset = c * samples * rxChannels;
        for (uint8_t a = 0; a < rxChannels; a++)
        {
            for (uint16_t s = 0; s < samples; s++)
            {
                if (a & 1)
                    data[a * samples + s + offset] = (s < (samples / 2)) ? 1024 : -1024;
                else
                    data[a * samples + s + offset] = (s < (samples / 2)) ? -2048 : 2047;
            }
        }
    }
}

void PatternGeneration_comb(int16_t *data, uint8_t rxChannels, uint16_t samples, uint16_t chirps)
{
    for (uint16_t c = 0; c < chirps; c++)
    {
        uint16_t offset = c * samples * rxChannels;
        for (uint8_t a = 0; a < rxChannels; a++)
        {
            for (uint16_t s = 0; s < samples; s++)
            {
                if (a & 1)
                    data[a * samples + s + offset] = (s & 1) ? 1024 : -1024;
                else
                    data[a * samples + s + offset] = (s & 1) ? -2048 : 2047;
            }
        }
    }
}

void PatternGeneration_checkerboard(int16_t *data, uint8_t rxChannels, uint16_t samples, uint16_t chirps)
{
    for (uint16_t c = 0; c < chirps; c++)
    {
        uint16_t offset = c * samples * rxChannels;
        for (uint8_t a = 0; a < rxChannels; a++)
        {
            for (uint16_t s = 0; s < samples; s++)
            {
                if (a & 1)
                    data[a * samples + s + offset] = ((c < chirps / 2) != (s < samples / 2)) ? 1024 : -1024;
                else
                    data[a * samples + s + offset] = ((c < chirps / 2) != (s < samples / 2)) ? -2048 : 2047;
            }
        }
    }
}
