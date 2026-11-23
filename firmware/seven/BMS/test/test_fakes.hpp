#pragma once

#include "app_canUtils.hpp"

namespace fakes 
{
namespace ts
{
    void setVoltage(float voltage);
    void setCurrentLowResolution(float amps);
    void setCurrentHighResolution(float amps);
    void setVoltageSnsDiagState(bool ok);
    void setCurrentSnsDiagState(bool ok);
    void reset();
} // namespace fakes::ts

namespace charger 
{
    void setConnectionStatus(app::can_utils::ChargerConnectedType status);
    void setCPDutyCycle(float duty_cycle);
    void setCPFrequency(float frequency);
} // namespace fakes::charger

} // namespace fakes