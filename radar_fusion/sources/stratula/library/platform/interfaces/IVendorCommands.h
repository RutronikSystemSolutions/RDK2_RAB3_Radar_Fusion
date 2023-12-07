#ifndef I_VENDOR_COMMANDS_H
#define I_VENDOR_COMMANDS_H 1

#include <stdint.h>

typedef struct _IVendorCommands IVendorCommands;

struct _IVendorCommands
{
    uint16_t (*getMaxTransfer)(void);

    uint8_t (*vendorWrite)(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLength, const uint8_t *payload);

    uint8_t (*vendorRead)(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t **payload);

    uint8_t (*vendorTransfer)(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLengthIn, const uint8_t *payloadIn, uint16_t *wLengthOut, uint8_t **payloadOut);
};

#endif /* I_VENDOR_COMMANDS_H */
