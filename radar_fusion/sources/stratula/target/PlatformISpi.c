#include <impl/PlatformSpi.h>
#include <platform/interfaces/ISpi.h>


ISpi PlatformSpi = {
    .getMaxTransfer = PlatformSpi_getMaxTransfer,
    .configure      = PlatformSpi_configure,
    .write8         = PlatformSpi_write8,
    .write16        = PlatformSpi_write16,
    .write32        = PlatformSpi_write32,
    .read8          = PlatformSpi_read8,
    .read16         = PlatformSpi_read16,
    .read32         = PlatformSpi_read32,
    .transfer8      = PlatformSpi_transfer8,
    .transfer16     = PlatformSpi_transfer16,
    .transfer32     = PlatformSpi_transfer32,
};
