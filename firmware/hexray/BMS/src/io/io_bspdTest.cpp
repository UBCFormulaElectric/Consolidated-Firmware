#include "io_bspdTest.hpp"
#include "hw_gpios.hpp"

namespace io::bspdtest
{
void enable(const bool enable)
{
    bspd_test_enable.writePin(enable);
}

bool isCurrentThresholdExceeded(void)
{
    return !n_high_current_bspd.readPin();
}

bool isBrakePressureThresholdExceeded(void)
{
    return !n_brake_press_3v3.readPin();
}

bool isAccelBrakeOk(void)
{
    return accel_brake_ok.readPin();
}
} // namespace io::bspdtest
