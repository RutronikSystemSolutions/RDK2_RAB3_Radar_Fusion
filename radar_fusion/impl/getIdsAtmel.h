#include "ids/getUuidAtmel.h"
#include <platform/ids/impl/getMacNone.h>


// this definition is selected by including this header once in the project
// the linker will complain if multiple implementations are included
sr_t getUuid(uint8_t uuid[UUID_LENGTH])
{
    return getUuidAtmel(uuid);
}
