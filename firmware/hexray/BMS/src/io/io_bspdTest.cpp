#include "io_bspdTest.hpp"
#include "hw_gpios.hpp"

using namespace hw::gpio;
namespace io
{
namespace bspdtest
{
    void enable(const bool enable)
    {
        hw::Gpio::writePin(&bspd_test_enable_pin, enable);
    }

    bool isCurrentThresholdExceeded(void)
    {
        return !hw::Gpio::readPin(&n_high_current_bspd_pin);
    }

    bool isBrakePressureThresholdExceeded(void)
    {
        return !hw::Gpio::readPin(&n_brake_press_3v3_pin);
    }

    bool isAccelBrakeOk(void)
    {
        return hw::Gpio::readPin(&accel_brake_ok_pin);
    }
} // namespace bspdtest
} // namespace io
