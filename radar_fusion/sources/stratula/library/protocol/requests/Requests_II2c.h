#ifndef REQUESTS_II2c_H_
#define REQUESTS_II2c_H_ 1

#include <platform/interfaces/II2c.h>
#include <stdint.h>


uint8_t Requests_II2c_write(II2c *i2c, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload);
uint8_t Requests_II2c_transaction16_write(II2c *i2c, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload);
uint8_t Requests_II2c_read(II2c *i2c, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload);
uint8_t Requests_II2c_transaction16_read(II2c *i2c, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload);

#endif /* REQUESTS_II2c_H_ */
