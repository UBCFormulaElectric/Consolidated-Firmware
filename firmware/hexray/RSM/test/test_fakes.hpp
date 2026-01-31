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

namespace rPump
{
    void setStatus(std::expected<void, ErrorCode> value);
}

namespace tireTemp
{
    void set(float temp);
}
} // namespace fakes::io
