#ifndef REQUESTS_IGPIO_H_
#define REQUESTS_IGPIO_H_ 1

#include <platform/interfaces/IGpio.h>
#include <stdint.h>


uint8_t Requests_IGpio_read(IGpio *gpio, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload);
uint8_t Requests_IGpio_write(IGpio *gpio, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload);

#endif /* REQUESTS_IGPIO_H_ */
