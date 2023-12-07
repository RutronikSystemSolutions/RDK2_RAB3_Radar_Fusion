#include "BoardInfo.h"
#include <universal/protocol/protocol_definitions.h>


VersionInfo_t versionInfo = {
    FIRMWARE_VERSION_MAJOR,
    FIRMWARE_VERSION_MINOR,
    FIRMWARE_VERSION_PATCH,
    FIRMWARE_VERSION_BUILD,
    PROTOCOL_VERSION_MAJOR,
    PROTOCOL_VERSION_MINOR,
    0,
    0,
};


BoardInfo_t boardInfo = {
    BOARD_VID,
    BOARD_PID,
    BOARD_NAME,
};

char extendedVersion[] = EXTENDED_VERSION;
