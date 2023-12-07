#include "DataPreProcessing.h"


void DataPreprocessing_meanRemoval(int16_t *data, uint8_t rxChannels, uint16_t samples, uint16_t chirps)
{
    for (uint16_t c = 0; c < chirps; c++)
    {
        uint16_t offset = c * samples * rxChannels;
        for (uint8_t a = 0; a < rxChannels; a++)
        {
            int32_t chirpSum = 0;
            for (uint16_t s = 0; s < samples; s++)
            {
                chirpSum += data[a * samples + s + offset];
            }

            int16_t chirpMean = (int16_t)(chirpSum / samples);

            for (uint16_t s = 0; s < samples; s++)
            {
                data[a * samples + s + offset] -= chirpMean;
            }
        }
    }
}
