#include "tasks.h"
#include "jobs.hpp"
#include "io_time.hpp"

void tasks_preInit() {}
void tasks_init()
{
    jobs_init();
}

void tasks_run1Hz(void *arg)
{
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1Hz_tick();
        io::time::delayUntil(start_time + 1000);
    }
}
void tasks_run100Hz(void *arg)
{
    forever
    {
        jobs_run100Hz_tick();
    }
}
void tasks_run1kHz(void *arg)
{
    forever
    {
        jobs_run1kHz_tick();
    }
}
void tasks_runCanTx(void *arg)
{
    forever
    {
        jobs_runCanTx_tick();
    }
}
void tasks_runCanRx(void *arg)
{
    forever
    {
        jobs_runCanTx_tick();
    }
}
void tasks_runChimera(void *arg)
{
    forever
    {
        jobs_runChimera_tick();
    }
}