#include "jobs.hpp"
#include "app_brake.hpp"
#include "app_coolant.hpp"
#include "app_imu.hpp"
#include "app_suspension.hpp"
#include "app_tireTemp.hpp"
#include "io_imus.hpp"

void jobs_init()
{
#ifdef TARGET_TEST
    LOG_IF_ERR(imu_config.init());
#endif // TARGET_TEST
}
void jobs_run1Hz_tick()
{
    app::brake::broadcast();
    app::coolant::broadcast();
    app::imu::broadcast();
    app::suspension::broadcast();
    app::tireTemp::broadcast();
}
void jobs_run100Hz_tick() {}
void jobs_run1kHz_tick() {}
void jobs_runCanTx_tick() {}
void jobs_runCanRx_tick() {}
void jobs_runChimera_tick() {}
