#ifndef CRC16_H
#define CRC16_H 1

#include <stdint.h>

#define CRC16_CCITT_FALSE_SEED (0xFFFF)


#ifdef TARGET_PLATFORM_AURIX

#define TARGET_PLATFORM_CRC_ENGINE

#include <impl/AurixCrc.h>

#define crc16_ccitt_false AurixCrc16_ccitt_false

#else

/**
     * Calculate 16-bit CRC using the CRC-CCITT polynomial
     *
     * - polynomial: 0x1021 (x^16 + x^12 + x^5 + 1)
     * - seed: 0xFFFF (CRC16_CCITT_FALSE_SEED)
     * - check value: 0x0000
     *
     * @param buf      @param buf       data from which the checksum is calculated
     * @param length    Number of @p data bytes
     * @param crc       initial value for calculation
     * @return 16-bit checksum of @p data
     */
uint16_t crc16_ccitt_false(const uint8_t buf[], uint16_t len, uint16_t crc);
uint16_t crc16_ccitt_false_reg(uint32_t val);

#endif

#endif /* CRC16_H */
