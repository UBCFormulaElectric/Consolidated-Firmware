#pragma once
#include "util_errorCodes.hpp"
#include "io_imus.hpp"
#include "io_pumpControl.hpp"
#include "io_efuses.hpp"
#include "io_vcShdn.hpp"
#include "io_sbgEllipse.hpp"
#include "ecuTestBase.hpp"

namespace io
{

namespace bspdWarning
{
    void set_papps_pedal_percentage(float percentage);
    void set_BrakeActuated_state(bool actuated);
} // namespace bspdWarning

namespace loadswitches
{
    void setChannelEnabled(int channel, bool enabled);
    void setChannelCurrent(int channel, float current);
} // namespace loadswitches

namespace powerMonitoring
{
    float read_power(uint8_t channel);
    float read_current(uint8_t channel);
    float read_voltage(uint8_t channel);
    void  refresh();
    bool  init();
    void  set_reading_voltage(uint8_t channel, float voltage);
    void  set_reading_current(uint8_t channel, float current);
    void  set_reading_power(uint8_t channel, float power);
} // namespace powerMonitoring

namespace sbgEllipse
{
    void setAttitude(float roll, float pitch, float yaw);
    void setVelocity(
        uint32_t status,
        float    north,
        float    east,
        float    down,
        float    north_std_dev,
        float    east_std_dev,
        float    down_std_dev);
    void setSolutionMode(uint32_t mode);
    void setTimestampUs(uint32_t timestamp_us);
    void setOverflowCount(uint8_t overflow_count);
    void setComStatus(uint32_t com_status);
    void setGeneralStatus(uint16_t general_status);
} // namespace sbgEllipse

namespace pumpController
{
    void pumps_ok(bool ok);
    void pumps_enabled(bool enabled);
} // namespace pumpController

namespace shdnLoop
{
    void setNodeState(const char *node_name, bool state);
}

namespace startSwitch
{
}

} // namespace io
