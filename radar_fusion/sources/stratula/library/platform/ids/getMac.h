#ifndef GET_MAC_H
#define GET_MAC_H 1

#include <common/errors.h>
#include <stdint.h>


#define MAC_LENGTH 6


sr_t getMac(uint8_t mac[MAC_LENGTH]);


#endif /* GET_MAC_H */
