#pragma once
#include <stdbool.h>

extern "C"
{
#include "app_utils.h"
}

#include "hw_gpio.h"
#include "hw_adc.h"

namespace io::currentSensing
{
class CurrentSensing
{
#ifdef TARGET_EMBEDDED
  private:
    const Gpio *bat_fault_gpio;
    const Gpio *acc_fault_gpio;
    const Adc  *bat_current_adc;
    const Adc  *acc_current_adc;

  public:
    explicit CurrentSensing(
        const Gpio *bat_fault_in,
        const Gpio *acc_fault_in,
        const Adc  *bat_current_in,
        const Adc  *acc_current_in)
      : bat_fault_gpio(bat_fault_in),
        acc_fault_gpio(acc_fault_in),
        bat_current_adc(bat_current_in),
        acc_current_adc(acc_current_in) {};
#endif

    bool  hasAccumulatorFault();
    bool  hasBatteryFault();
    float getAccumulatorCurrent();
    float getBatteryCurrent();
};
} // namespace io::currentSensing
