#include "jobs.hpp"

#include "app_apps.hpp"
#include "app_brake.hpp"
#include "app_imu.hpp"
#include "app_steering.hpp"
#include "app_suspension.hpp"
// #include "app_shdnLoopNode.hpp"
// void io_canTx_enqueue100HzMsgs();

void jobs_init() {}
void jobs_run1Hz_tick()
{
    app::apps::broadcast();
    app::brake::broadcast();
    app::imu::broadcast();
    // app::shdnLoop::broadcast();
    app::steering::broadcast();
    app::suspension::broadcast();
}
void jobs_run100Hz_tick() {}
void jobs_run1kHz_tick() {}
void jobs_runCanTx_tick() {}
void jobs_runCanRx_tick() {}
void jobs_runChimera_tick() {}
