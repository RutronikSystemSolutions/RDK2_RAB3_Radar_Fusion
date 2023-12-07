#ifndef GET_UUID_H
#define GET_UUID_H 1

#include <common/errors.h>
#include <stdint.h>


#define UUID_LENGTH 16


sr_t getUuid(uint8_t uuid[UUID_LENGTH]);


#endif /* GET_UUID_H */
