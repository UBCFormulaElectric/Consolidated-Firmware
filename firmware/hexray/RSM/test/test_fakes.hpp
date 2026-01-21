#pragma once
#include "util_errorCodes.hpp"
#include "io_imu.hpp"

namespace fakes::io
{
namespace brake
{
    void setRearPressurePsi(float rear_PressurePsi);
}

namespace coolant
{
    void setFlowRate(float flow_rate);
}

namespace suspension
{
    void setRLTravel(float value);
    void setRRTravel(float value);
    void setRR_OCSC(bool value);
    void setRL_OCSC(bool value);
} // namespace suspension

namespace imu
{
    void setAccelX(float value, ExitCode status);
    void setAccelY(float value, ExitCode status);
    void setAccelZ(float value, ExitCode status);
    void setGyroX(float value, ExitCode status);
    void setGyroY(float value, ExitCode status);
    void setGyroZ(float value, ExitCode status);
} // namespace imu

namespace rPump
{
    void setStatus(ExitCode value);
}

namespace tireTemp
{
    void set(float temp);
}

} // namespace fakes::io

extern io::imu::Imu imu_config;