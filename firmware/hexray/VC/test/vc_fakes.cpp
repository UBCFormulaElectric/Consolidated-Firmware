#include "vc_fakes.hpp"
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

namespace io // Define the mocked functions here
{
namespace bspdWarning
{
    float papps_pedal_percentage = 0.0f;
    bool  brake_actuation_state  = false;
    void  set_papps_pedal_percentage(float percentage)
    {
        papps_pedal_percentage = percentage;
    }
    void set_BrakeActuated_state(bool actuated)
    {
        brake_actuation_state = actuated;
    }
} // namespace bspdWarning

namespace pumpController
{
    bool    pumps_ok_state      = true;
    bool    pumps_enabled_state = true;
    uint8_t pump_percentage     = 0;

    void pumps_ok(bool ok)
    {
        pumps_ok_state = ok;
    }

    void pumps_enabled(bool enabled)
    {
        pumps_enabled_state = enabled;
    }
} // namespace pumpController

namespace powerMonitoring
{
    namespace
    {
        constexpr int NUM_CHANNELS = 5;
        float         voltages[NUM_CHANNELS]{};
        float         currents[NUM_CHANNELS]{};
        float         powers[NUM_CHANNELS]{};
    } // namespace

    float read_power(uint8_t channel)
    {
        return powers[static_cast<int>(channel)];
    }
    float read_current(uint8_t channel)
    {
        return currents[static_cast<int>(channel)];
    }
    float read_voltage(uint8_t channel)
    {
        return voltages[static_cast<int>(channel)];
    }
    void refresh() {}
    bool init()
    {
        return true;
    }
    void set_reading_voltage(uint8_t channel, float voltage)
    {
        voltages[channel] = voltage;
    }
    void set_reading_current(uint8_t channel, float current)
    {
        currents[channel] = current;
    }
    void set_reading_power(uint8_t channel, float power)
    {
        powers[channel] = power;
    }
} // namespace powerMonitoring

namespace sbgEllipse
{
    namespace
    {
        io::sbgEllipse::Attitude     attitude{ 0.0f, 0.0f, 0.0f };
        io::sbgEllipse::VelocityData velocity{ 0u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        uint32_t                     solution_mode  = 0;
        uint32_t                     timestamp_us   = 0;
        uint8_t                      overflow_count = 0;
        uint32_t                     com_status     = 0;
        uint16_t                     general_status = 0;
    } // namespace

    const io::sbgEllipse::Attitude getEkfEulerAngles()
    {
        return attitude;
    }
    uint32_t getEkfSolutionMode()
    {
        return solution_mode;
    }
    const io::sbgEllipse::VelocityData getEkfNavVelocityData()
    {
        return velocity;
    }
    uint32_t getTimestampUs()
    {
        return timestamp_us;
    }
    uint8_t getOverflowCount()
    {
        return overflow_count;
    }
    uint32_t getComStatus()
    {
        return com_status;
    }
    uint16_t getGeneralStatus()
    {
        return general_status;
    }
    void setAttitude(float roll, float pitch, float yaw)
    {
        attitude = io::sbgEllipse::Attitude{ roll, pitch, yaw };
    }
    void setVelocity(
        uint32_t status,
        float    north,
        float    east,
        float    down,
        float    north_std_dev,
        float    east_std_dev,
        float    down_std_dev)
    {
        velocity = io::sbgEllipse::VelocityData{ status, north, east, down, north_std_dev, east_std_dev, down_std_dev };
    }
    void setSolutionMode(uint32_t mode)
    {
        solution_mode = mode;
    }
    void setTimestampUs(uint32_t timestamp)
    {
        timestamp_us = timestamp;
    }
    void setOverflowCount(uint8_t overflow)
    {
        overflow_count = overflow;
    }
    void setComStatus(uint32_t status)
    {
        com_status = status;
    }
    void setGeneralStatus(uint16_t status)
    {
        general_status = status;
    }
} // namespace sbgEllipse

} // namespace io
