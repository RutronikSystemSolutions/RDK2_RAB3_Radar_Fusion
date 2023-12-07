#ifndef DATA_PRE_PROCESSING_H_
#define DATA_PRE_PROCESSING_H_ 1

#include <stdint.h>


/* Performs DC mean removal on a time data measurement frame.
 * 
 * This function aims to filter the DC component, which corresponds
 * to removing the static objects from a radar measurement.
 * 
 * Note: data is expected to be de-interleaved!
 * 
 *       Data format: [rx] [chirps][tx][samples]
 */
void DataPreprocessing_meanRemoval(int16_t *data, uint8_t rxChannels, uint16_t samples, uint16_t chirps);


#endif /* DATA_PRE_PROCESSING_H_ */
