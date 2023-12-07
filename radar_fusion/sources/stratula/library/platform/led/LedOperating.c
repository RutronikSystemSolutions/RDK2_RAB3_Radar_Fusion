#include "LedOperating.h"
#include <impl/chrono.h>


static const LedDefinition_t *m_ledOperating;
static bool m_ledTurnedOn = true;
static chrono_ticks_t m_ledTimeout;


static void LedOperating_Callback(void)
{
    m_ledTurnedOn = !m_ledTurnedOn;
    Led_set(m_ledOperating, m_ledTurnedOn);
}

void LedOperating_Constructor(const LedDefinition_t *ledOperating)
{
    m_ledOperating = ledOperating;
    Led_initialize(m_ledOperating);
    Led_set(m_ledOperating, m_ledTurnedOn);
    m_ledTimeout = chrono_now() + chrono_milliseconds(500);
}

void LedOperating_run(void)
{
    if (chrono_has_passed(m_ledTimeout))
    {
        m_ledTimeout += chrono_milliseconds(500);
        LedOperating_Callback();
    }
}
