#pragma once
#include <cstring>
#include <concepts>

#include "io_batteryMonitoring_datatypes.hpp"
#include "util_errorCodes.hpp"

namespace io::batteryMonitoring
{
    template<typename A> result<float> getVoltage(A request);
    result<float> getCurrent();
    result<float> getTemperature();

} // namespace io::batteryMonitoring    