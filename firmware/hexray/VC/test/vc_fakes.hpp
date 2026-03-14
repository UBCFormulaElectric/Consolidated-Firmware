#pragma once
#include "util_errorCodes.hpp"
#include "io_imu.hpp"

namespace fakes::io
{
namespace contactor
{
    void setContactor(bool closed);
}

namespace loadswitches
{
    void setChannelEnabled(int channel, bool enabled);
    void setChannelCurrent(int channel, float current);
} // namespace loadswitches

namespace powerMonitoring
{
    void  set_reading_voltage(int channel, float voltage);
    float set_reading_current(int channel, float current);
    float set_reading_power(int channel, float power);
} // namespace powerMonitoring
namespace pumpController
{
    void pumps_ok(bool ok);
    void pumps_enabled(bool enabled);
} // namespace pumpController

namespace sbgEllipses
{
    void setAttitude(float roll, float pitch, float yaw);
    void setAngularVelocity(float roll_rate, float pitch_rate, float yaw_rate);
    void setLinearAcceleration(float x_accel, float y_accel, float z_accel);
} // namespace sbgEllipses

namespace shdnLoop
{
    void setNodeState(const char *node_name, bool state);
}

namespace startSwitch
{
}

namespace warningHandling
{
    void set_papps_pedal_percentage(float percentage);
    void set_brake_actuation_state(bool actuated);
} // namespace warningHandling
} // namespace fakes::io
