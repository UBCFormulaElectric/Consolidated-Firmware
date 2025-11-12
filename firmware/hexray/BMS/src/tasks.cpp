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
        jobs_runCanTx();
    }
}
void tasks_runCanRx(void *arg)
{
    forever
    {
        jobs_runCaRx();
    }
}
void tasks_runChimera(void *arg)
{
    forever
    {
        jobs_runChimera_tick();
    }
}

void tasks_runLtcVoltages(void *arg)
{
    forever
    {
        jobs_initLTCVoltages();
    }
}

void tasks_runLtcTemps(void *arg)
{
    forever
    {
        jobs_initLTCTemps();
    }
}

void tasks_runLtcDiagnostics(void *arg)
{
    forever
    {
        jobs_initLTCDiagnostics();
    }
}
