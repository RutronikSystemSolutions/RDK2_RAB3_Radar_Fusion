/**
 * \addtogroup        Board
 * @{
 */

#ifndef BOARD_INFO_H
#define BOARD_INFO_H 1

#include <BoardDefinition.h>
#include <stdint.h>


#define STRINGIZE_(s) #s
#define STRINGIZE(s)  STRINGIZE_(s)


#ifndef EXTENDED_VERSION
#define EXTENDED_VERSION ""
#endif


typedef struct
{
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
    uint16_t build;
    uint16_t protocolMajor;
    uint16_t protocolMinor;
    uint16_t reserved0;
    uint16_t reserved1;
} VersionInfo_t;

typedef struct
{
    uint16_t vid;
    uint16_t pid;
    char name[sizeof(BOARD_NAME)];
} BoardInfo_t;


extern VersionInfo_t versionInfo;
extern BoardInfo_t boardInfo;
extern char extendedVersion[sizeof(EXTENDED_VERSION)];


#endif /* BOARD_INFO_H */

/** @} */
