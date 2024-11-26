#pragma once
#include "hw_gpios.h"

extern "C"
{
#include "app_canTx.h"
}

namespace io::fans
{
class Fans
{
  private:
    const hw::Gpio &acc_fan;
    const hw::Gpio &rad_fan;

  public:
    explicit Fans(const hw::Gpio &acc_fan_in, const hw::Gpio &rad_fan_in) : acc_fan(acc_fan_in), rad_fan(rad_fan_in) {}

    void acc_fan_set(bool on)
    {
        acc_fan.writePin(on);
        app_canTx_RSM_AccumulatorFan_set(on);
    }

    void rad_fan_set(bool on)
    {
        rad_fan.writePin(on);
        app_canTx_RSM_RadiatorFan_set(on);
    }
}
} // namespace io::fans
