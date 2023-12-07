#include "getUuidAtmel.h"
#include <string.h>

sr_t getUuidAtmel(uint8_t uuid[UUID_LENGTH])
{
	/*Return hard-coded data*/
	uint8_t uuid_def[] = {0x00, 0x31, 0x37, 0x53, 0x36, 0x41, 0x36, 0x36, 0x31, 0x36, 0x30, 0x35, 0x32, 0x30, 0x35, 0x35};
	memcpy(uuid, uuid_def, UUID_LENGTH);
    return E_SUCCESS;
}
