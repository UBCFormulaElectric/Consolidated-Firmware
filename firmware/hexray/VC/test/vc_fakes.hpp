#pragma once
#include "util_errorCodes.hpp"
#include "io_imu.hpp"
#include "io_shdnLoopNode.hpp"
#include "io_imus.hpp"
#include "io_pumpControl.hpp"
#include "io_pump.hpp"
#include "io_efuses.hpp"
#include "io_sbgEllipse.hpp"
#include "io_powerMonitoring.hpp"
#include "io_vcShdn.hpp"
#include "io_efuse_TI_TPS25.hpp"
#include "io_efuse_TI_TPS28.hpp"
#include "app_canTx.hpp"

// IMUs
io::Imu IMU1;
io::Imu IMU2;
io::Imu IMU3;

// Pumps
io::Pump rr_pump;
io::Pump rl_pump;

// Efuses
io::TI_TPS28_Efuse f_inv_efuse;
io::TI_TPS28_Efuse r_inv_efuse;
io::TI_TPS28_Efuse bms_efuse;
io::TI_TPS28_Efuse rsm_efuse;
io::TI_TPS28_Efuse dam_efuse;
io::TI_TPS28_Efuse front_efuse;
io::TI_TPS28_Efuse l_rad_fan_efuse;
io::TI_TPS28_Efuse r_rad_fan_efuse;
io::TI_TPS25_Efuse rl_pump_efuse;
io::TI_TPS25_Efuse rr_pump_efuse;

// Shutdown loop nodes
const io::shdn::node tsms_node(false, app::can_tx::VC_TSMSOKStatus_set);
const io::shdn::node inertia_stop_node(false, app::can_tx::VC_InertiaSwitch_set);
const io::shdn::node rear_right_motor_interlock_node(false, app::can_tx::VC_RearRightMotorInterlock_set);
const io::shdn::node splitter_box_interlock_node(false, app::can_tx::VC_MSDOrEMeterOKStatus_set);

namespace fakes::io
{
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

namespace sbgEllipse
{
    struct Attitude
    {
        float roll;
        float pitch;
        float yaw;
    };
    struct VelocityData
    {
        uint32_t status;
        float    north;
        float    east;
        float    down;
        float    north_std_dev;
        float    east_std_dev;
        float    down_std_dev;
    };

    void setAttitude(float roll, float pitch, float yaw);
    void setAngularVelocity(float roll_rate, float pitch_rate, float yaw_rate);
    void setLinearAcceleration(float x_accel, float y_accel, float z_accel);
} // namespace sbgEllipse

namespace shdnLoop
{
    void setNodeState(const char *node_name, bool state);
}

namespace startSwitch
{
}

namespace bspdWarning
{
    void set_papps_pedal_percentage(float percentage);
    void set_BrakeActuated_state(bool actuated);
} // namespace bspdWarning
} // namespace fakes::io
