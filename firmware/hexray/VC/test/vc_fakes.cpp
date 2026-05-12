#include "vc_fakes.hpp"
#include "app_canTx.hpp"
#include "io_imus.hpp"
#include "io_pumpControl.hpp"
#include "io_sbgEllipse.hpp"
#include "io_powerMonitoring.hpp"
#include "io_shdnLoopNode.hpp"
#include "io_efuse_TI_TPS25.hpp"
#include "io_efuse_TI_TPS28.hpp"

io::imu                  IMU1;
io::imu                  IMU2;
io::imu                  IMU3;
const io::pump           rr_pump;
const io::pump           rl_pump;
const io::TI_TPS28_Efuse f_inv_efuse;
const io::TI_TPS28_Efuse r_inv_efuse;
const io::TI_TPS28_Efuse bms_efuse;
const io::TI_TPS28_Efuse rsm_efuse;
// app::Timer         timer{};

const io::TI_TPS28_Efuse dam_efuse;
const io::TI_TPS28_Efuse front_efuse;
const io::TI_TPS28_Efuse l_rad_fan_efuse;
const io::TI_TPS28_Efuse r_rad_fan_efuse;
const io::TI_TPS25_Efuse rl_pump_efuse;
const io::TI_TPS25_Efuse rr_pump_efuse;

// Shutdown loop nodes
const io::shdn::node tsms_node(false, app::can_tx::VC_TSMSOKStatus_set);
const io::shdn::node inertia_stop_node(false, app::can_tx::VC_InertiaSwitch_set);
const io::shdn::node rear_right_motor_interlock_node(false, app::can_tx::VC_RearRightMotorInterlock_set);
const io::shdn::node splitter_box_interlock_node(false, app::can_tx::VC_MSDOrEMeterOKStatus_set);

namespace fakes
{
namespace io
{
    namespace pcm
    {
        static bool enable_pin;
    }
    namespace powerMonitoring
    {

        enum : uint8_t
        {
            CH1,
            CH2,
            CH3,
            CH4
        };

        static float power_ch1   = 0.0f;
        static float current_ch1 = 0.0f;
        static float voltage_ch1 = 0.0f;

        static float power_ch2   = 0.0f;
        static float current_ch2 = 0.0f;
        static float voltage_ch2 = 0.0f;

        static float power_ch3   = 0.0f;
        static float current_ch3 = 0.0f;
        static float voltage_ch3 = 0.0f;

        static float power_ch4   = 0.0f;
        static float current_ch4 = 0.0f;
        static float voltage_ch4 = 0.0f;

        void setPower(float power, uint8_t ch)
        {
            switch (ch)
            {
                case CH1:
                    power_ch1 = power;
                    break;
                case CH2:
                    power_ch2 = power;
                    break;
                case CH3:
                    power_ch3 = power;
                    break;
                case CH4:
                    power_ch4 = power;
                    break;
                default:
                    break;
            }
        }

        void setCurrent(float current, uint8_t ch)
        {
            switch (ch)
            {
                case CH1:
                    current_ch1 = current;
                    break;
                case CH2:
                    current_ch2 = current;
                    break;
                case CH3:
                    current_ch3 = current;
                    break;
                case CH4:
                    current_ch4 = current;
                    break;
                default:
                    break;
            }
        }
        void setVoltage(float voltage, uint8_t ch)
        {
            switch (ch)
            {
                case CH1:
                    voltage_ch1 = voltage;
                    break;
                case CH2:
                    voltage_ch2 = voltage;
                    break;
                case CH3:
                    voltage_ch3 = voltage;
                    break;
                case CH4:
                    voltage_ch4 = voltage;
                    break;
                default:
                    break;
            }
        }

    } // namespace powerMonitoring

} // namespace io

} // namespace fakes

namespace io // Define the mocked functions here
{
namespace pcm
{
    void set(const bool enable)
    {
        fakes::io::pcm::enable_pin = true;
    }

    bool enabled()
    {
        return fakes::io::pcm::enable_pin;
    }
} // namespace pcm
namespace imus
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
    float read_power(uint8_t ch)
    {
        switch (ch)
        {
            case fakes::io::powerMonitoring::CH1:
                return fakes::io::powerMonitoring::power_ch1;
            case fakes::io::powerMonitoring::CH2:
                return fakes::io::powerMonitoring::power_ch2;
            case fakes::io::powerMonitoring::CH3:
                return fakes::io::powerMonitoring::power_ch3;
            case fakes::io::powerMonitoring::CH4:
                return fakes::io::powerMonitoring::power_ch4;
            default:
                break;
        }
        return 0.0f;
    }
    float read_current(uint8_t ch)
    {
        switch (ch)
        {
            case fakes::io::powerMonitoring::CH1:
                return fakes::io::powerMonitoring::current_ch1;
            case fakes::io::powerMonitoring::CH2:
                return fakes::io::powerMonitoring::current_ch2;
            case fakes::io::powerMonitoring::CH3:
                return fakes::io::powerMonitoring::current_ch3;
            case fakes::io::powerMonitoring::CH4:
                return fakes::io::powerMonitoring::current_ch4;
            default:
                break;
        }
        return 0.0f;
    }
    float read_voltage(uint8_t ch)
    {
        switch (ch)
        {
            case fakes::io::powerMonitoring::CH1:
                return fakes::io::powerMonitoring::voltage_ch1;
            case fakes::io::powerMonitoring::CH2:
                return fakes::io::powerMonitoring::voltage_ch2;
            case fakes::io::powerMonitoring::CH3:
                return fakes::io::powerMonitoring::voltage_ch3;
            case fakes::io::powerMonitoring::CH4:
                return fakes::io::powerMonitoring::voltage_ch4;
            default:
                break;
        }
        return 0.0f;
    }
    std::expected<void, ErrorCode> refresh()
    {
        return {};
    }
    std::expected<void, ErrorCode> init()
    {
        return {};
    }
    std::expected<void, ErrorCode> monitor_power_inputs()
    {
        return {};
    }
    std::expected<uint8_t, ErrorCode> read_alert_status()
    {
        return 0u;
    }
    std::expected<bool, ErrorCode> is_alert_asserted()
    {
        return false;
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
