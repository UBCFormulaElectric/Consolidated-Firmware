#include "tasks.hpp"
#include "jobs.hpp"
#include "io_time.hpp"

void tasks_preInit() {}
void tasks_init()
{
    jobs_init();
}

void tasks_run1Hz()
{
    forever
    {
        const uint32_t start_time = io::time::getCurrentMs();
        jobs_run1Hz_tick();
        io::time::delayUntil(start_time + 1000);
    }
}
void tasks_run100Hz()
{
    forever
    {
        jobs_run100Hz_tick();
    }
}
void tasks_run1kHz()
{
    forever
    {
        jobs_run1kHz_tick();
    }
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
void tasks_runCanRxCallback(const io::CanMsg *msg)
{
    // ??
}