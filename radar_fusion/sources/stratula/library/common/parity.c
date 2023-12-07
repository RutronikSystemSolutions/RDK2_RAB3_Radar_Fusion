#include "parity.h"

bool getParity(unsigned int value)
{
    bool result = false;
    for (; value > 0; value >>= 1)
    {
        if (value & 1)
        {
            result = !result;
        }
    }
    return result;
}
