#pragma once
#include "util_errorCodes.hpp"
#include "io_imus.hpp"
#include "io_powerMonitoring.hpp"
#include "io_vcShdn.hpp"
#include "io_sbgEllipse.hpp"
#include "ecuTestBase.hpp"

namespace fakes::io
{

namespace loadswitches
{
    void setChannelEnabled(int channel, bool enabled);
    void setChannelCurrent(int channel, float current);
} // namespace loadswitches

namespace powerMonitoring
{
    void set_reading_voltage(Channel channel, float voltage);
    void set_reading_current(Channel channel, float current);
    void set_reading_power(Channel channel, float power);
} // namespace powerMonitoringpumpController

namespace sbgEllipse
{
    void setAttitude(float roll, float pitch, float yaw);
    void setVelocity(
        uint32_t status,
        float    north,

        float east,
        float down,
        float north_std_dev,
        float east_std_dev,
        float down_std_dev);
    void setAngularVelocity(float roll_rate, float pitch_rate, float yaw_rate);
    void setLinearAcceleration(float x_accel, float y_accel, float z_accel);
    void setGeneralStatus(uint16_t status);
    void setComStatus(uint32_t status);
    void setOverflowCount(uint8_t overflow);
    void setTimestampUs(uint32_t timestamp);
} // namespace sbgEllipse

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