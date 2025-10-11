#include "jobs.hpp"
#include "io_brake.hpp"
#include "app_brake.hpp"


void jobs_init() {}
void jobs_run1Hz_tick() 
{
    app_brake_broadcast();

}
void jobs_run100Hz_tick() {}
void jobs_run1kHz_tick() {}
