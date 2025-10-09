#include "tasks.h"
#include "io_time.hpp"

void tasks_preInit() {}
void tasks_init() {}

void tasks_run1Hz()
{
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        io::time::delayUntil(start_time + 1000);
    }
}
void tasks_run100Hz()
{
    forever {}
}
void tasks_run1kHz()
{
    forever {}
}
void tasks_runCanTx()
{
    forever {}
}
void tasks_runCanRx()
{
    forever {}
}
void tasks_runChimera()
{
    forever {}
}