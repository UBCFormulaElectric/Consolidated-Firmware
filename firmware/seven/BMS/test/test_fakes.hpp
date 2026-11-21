#pragma once

#include "app_canUtils.hpp"

namespace fakes 
{
namespace charger 
{
    void setConnectionStatus(app::can_utils::ChargerConnectedType status);
    void setCPDutyCycle(float duty_cycle);
    void setCPFrequency(float frequency);
} // namespace charger 
} // namespace fakes