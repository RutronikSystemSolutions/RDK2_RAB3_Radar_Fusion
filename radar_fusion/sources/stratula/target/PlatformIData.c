#include <impl/PlatformData.h>
#include <platform/interfaces/IData.h>


IData PlatformData = {
    .configure           = PlatformData_configure,
    .calibrationRequired = PlatformData_calibrationRequired,
    .calibrate           = PlatformData_calibrate,
    .start               = PlatformData_start,
    .stop                = PlatformData_stop,
    .getStatusFlags      = PlatformData_getStatusFlags,
    .registerCallback    = PlatformData_registerCallback,
};
