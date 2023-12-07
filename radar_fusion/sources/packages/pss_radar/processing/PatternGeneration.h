#ifndef PATTERN_GENERATION_H_
#define PATTERN_GENERATION_H_ 1

/* /brief Functions to generate data patterns to validate an FFT processing module.
 * 
 * Note: output data is de-interleaved!
 * 
 *       Data format: [rx] [chirps][tx][samples]
  */

#include <stdint.h>


/* 
 * Data pattern consists of a rectangular wave containing a high amount of harmonics frequencies,
 * which will be noticeable applying an FFT to it.
 */
void PatternGeneration_rectangular(int16_t *data, uint8_t rxChannels, uint16_t samples, uint16_t chirps);

/* 
 * After applying an FFT to this signal, an "E" shaped pattern shall be visible on the resulting FFT plot.
 */
void PatternGeneration_comb(int16_t *data, uint8_t rxChannels, uint16_t samples, uint16_t chirps);

/*
 * After applying an FFT to this signal, a "chessboard" shaped pattern shall be visible on the resulting FFT plot.
 */
void PatternGeneration_checkerboard(int16_t *data, uint8_t rxChannels, uint16_t samples, uint16_t chirps);


#endif /* PATTERN_GENERATION_H_ */
