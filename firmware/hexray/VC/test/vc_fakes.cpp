#include "vc_fakes.hpp"
#include "app_canTx.hpp"
#include "io_imus.hpp"
#include "io_batteryCharging.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_pumpControl.hpp"
#include "io_sbgEllipse.hpp"
#include "io_powerMonitoring.hpp"
#include "io_shdnLoopNode.hpp"
#include "io_efuse_TI_TPS25.hpp"
#include "io_efuse_TI_TPS28.hpp"
#include "io_vcShdn.hpp"

// imus
io::imu IMU1;
io::imu IMU2;
io::imu IMU3;

// pumps
const io::pump rr_pump;
const io::pump rl_pump;

// efuses
io::TI_TPS28_Efuse f_inv_efuse;
io::TI_TPS28_Efuse r_inv_efuse;
io::TI_TPS28_Efuse bms_efuse;
io::TI_TPS28_Efuse rsm_efuse;
// app::Timer         timer{};

io::TI_TPS28_Efuse dam_efuse;
io::TI_TPS28_Efuse front_efuse;
io::TI_TPS28_Efuse l_rad_fan_efuse;
io::TI_TPS28_Efuse r_rad_fan_efuse;
io::TI_TPS25_Efuse rl_pump_efuse;
io::TI_TPS25_Efuse rr_pump_efuse;

// Shutdown loop nodes
const io::shdn::node tsms_node(app::can_tx::VC_TSMSOKStatus_set);
const io::shdn::node inertia_stop_node(app::can_tx::VC_InertiaSwitch_set);
const io::shdn::node rear_right_motor_interlock_node(app::can_tx::VC_RearRightMotorInterlock_set);
const io::shdn::node splitter_box_interlock_node(app::can_tx::VC_MSDOrEMeterOKStatus_set);

// Shutdown loop nodes
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

        void set_reading_voltage(Channel channel, float voltage)
        {
            switch (channel)
            {
                case CH1:
                    fakes::io::powerMonitoring::voltage_ch1 = voltage;
                    break;
                case CH2:
                    fakes::io::powerMonitoring::voltage_ch2 = voltage;
                    break;
                case CH3:
                    fakes::io::powerMonitoring::voltage_ch3 = voltage;
                    break;
                case CH4:
                    fakes::io::powerMonitoring::voltage_ch4 = voltage;
                    break;
                default:
                    break;
            }
        }
        void set_reading_current(Channel channel, float current)
        {
            switch (channel)
            {
                case CH1:
                    fakes::io::powerMonitoring::current_ch1 = current;
                    break;
                case CH2:
                    fakes::io::powerMonitoring::current_ch2 = current;
                    break;
                case CH3:
                    fakes::io::powerMonitoring::current_ch3 = current;
                    break;
                case CH4:
                    fakes::io::powerMonitoring::current_ch4 = current;
                    break;
                default:
                    break;
            }
        }
        void set_reading_power(Channel channel, float power)
        {
            switch (channel)
            {
                case CH1:
                    fakes::io::powerMonitoring::power_ch1 = power;
                    break;
                case CH2:
                    fakes::io::powerMonitoring::power_ch2 = power;
                    break;
                case CH3:
                    fakes::io::powerMonitoring::power_ch3 = power;
                    break;
                case CH4:
                    fakes::io::powerMonitoring::power_ch4 = power;
                    break;
                default:
                    break;
            }
        }
    } // namespace powerMonitoring

    namespace sbgEllipse
    {
        ::io::sbgEllipse::Attitude     attitude{ 0.0f, 0.0f, 0.0f };
        ::io::sbgEllipse::VelocityData velocity{ 0u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        uint32_t                       solution_mode  = 0;
        uint32_t                       timestamp_us   = 0;
        uint8_t                        overflow_count = 0;
        uint32_t                       com_status     = 0;
        uint16_t                       general_status = 0;
        bool                           initialized    = false;
        result<void>                   init_status    = {};

        void reset_init()
        {
            initialized = false;
        }
        void imu_status_set(bool status)
        {
            init_status = status ? std::expected<void, ErrorCode>{} : std::unexpected(ErrorCode::TIMEOUT);
        }

        bool get_init()
        {
            return initialized;
        }

        result<void> init()
        {
            if (init_status.has_value())
            {
                initialized = true;
                return {};
            }
            else
            {
                return std::unexpected(init_status.error());
            }
        }

        void setAttitude(float roll, float pitch, float yaw)
        {
            attitude = ::io::sbgEllipse::Attitude{ roll, pitch, yaw };
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
            velocity =
                ::io::sbgEllipse::VelocityData{ status, north, east, down, north_std_dev, east_std_dev, down_std_dev };
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
} // namespace fakes

namespace io
{
namespace batteryMonitoring
{
    result<void> init()
    {
        return {};
    }
    result<float> get_voltage_cell(CellReading)
    {
        return 0.0f;
    }
    result<float> get_voltage_system(SystemReading)
    {
        return 0.0f;
    }
    result<float> get_current()
    {
        return 0.0f;
    }
    result<uint64_t> get_integrated_charge()
    {
        return 0ULL;
    }
    result<float> get_temperatureIC()
    {
        return 0.0f;
    }
    result<SafetyStatusA> get_safety_alert_a()
    {
        return SafetyStatusA{ .raw_status = 0 };
    }
    result<SafetyStatusB> get_safety_alert_b()
    {
        return SafetyStatusB{ .raw_status = 0 };
    }
    result<void> send_balancing_subcommand(CellBalance_BitMask)
    {
        return {};
    }
    result<void> stop_balancing_subcommand()
    {
        return {};
    }
    result<uint16_t> read_balancing_subcommand()
    {
        return 0U;
    }
    result<uint16_t> balacing_time_elapsed()
    {
        return 0U;
    }
    result<std::array<uint16_t, 5>> get_voltage_OV(uint16_t)
    {
        return std::array<uint16_t, 5>{};
    }
    result<std::array<uint16_t, 5>> get_voltage_UV(uint16_t)
    {
        return std::array<uint16_t, 5>{};
    }
    void alert_handler() {}
    bool consume_alert_pending()
    {
        return false;
    }
    result<AlertStatus> read_alarm_status()
    {
        return AlertStatus{ .raw_value = 0 };
    }
    result<int16_t> read_currentcc1()
    {
        return 0;
    }
} // namespace batteryMonitoring
namespace batteryCharging
{
    void charger_enable(void) {}
    void charger_disable(void) {}
    bool check_status(void)
    {
        return false;
    }
} // namespace batteryCharging
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
} // namespace imus

namespace powerMonitoring
{
    result<float> read_power(Channel ch)
    {
        std::expected<float, ErrorCode> result{ std::unexpected(ErrorCode::INVALID_ARGS) };
        switch (ch)
        {
            case CH1:
                return fakes::io::powerMonitoring::power_ch1;
            case CH2:
                return fakes::io::powerMonitoring::power_ch2;
            case CH3:
                return fakes::io::powerMonitoring::power_ch3;
            case CH4:
                return fakes::io::powerMonitoring::power_ch4;
            default:
                break;
        }
        return result;
    }
    result<float> read_current(Channel ch)
    {
        std::expected<float, ErrorCode> result{ std::unexpected(ErrorCode::INVALID_ARGS) };
        switch (ch)
        {
            case CH1:
                return fakes::io::powerMonitoring::current_ch1;
            case CH2:
                return fakes::io::powerMonitoring::current_ch2;
            case CH3:
                return fakes::io::powerMonitoring::current_ch3;
            case CH4:
                return fakes::io::powerMonitoring::current_ch4;
            default:
                break;
        }
        return result;
    }
    result<float> read_voltage(Channel ch)
    {
        std::expected<float, ErrorCode> result{ std::unexpected(ErrorCode::INVALID_ARGS) };
        switch (ch)
        {
            case CH1:
                return fakes::io::powerMonitoring::voltage_ch1;
            case CH2:
                return fakes::io::powerMonitoring::voltage_ch2;
            case CH3:
                return fakes::io::powerMonitoring::voltage_ch3;
            case CH4:
                return fakes::io::powerMonitoring::voltage_ch4;
            default:
                break;
        }
        return result;
    }
    result<void> refresh()
    {
        return {};
    }
    result<void> init()
    {
        return {};
    }
    result<void> monitor_power_inputs()
    {
        return {};
    }
    result<uint8_t> read_alert_status()
    {
        return 0u;
    }
    result<bool> is_alert_asserted()
    {
        return false;
    }
} // namespace powerMonitoring

namespace sbgEllipse
{
    result<void> init()
    {
        return fakes::io::sbgEllipse::init();
    }

    const ::io::sbgEllipse::Attitude getEkfEulerAngles()
    {
        return fakes::io::sbgEllipse::attitude;
    }
    uint32_t getEkfSolutionMode()
    {
        return fakes::io::sbgEllipse::solution_mode;
    }
    const ::io::sbgEllipse::VelocityData getEkfNavVelocityData()
    {
        return fakes::io::sbgEllipse::velocity;
    }
    uint32_t getTimestampUs()
    {
        return fakes::io::sbgEllipse::timestamp_us;
    }
    uint8_t getOverflowCount()
    {
        return fakes::io::sbgEllipse::overflow_count;
    }
    uint32_t getComStatus()
    {
        return fakes::io::sbgEllipse::com_status;
    }
    uint16_t getGeneralStatus()
    {
        return fakes::io::sbgEllipse::general_status;
    }

} // namespace sbgEllipse
namespace pcm
{
    void set(const bool enable)
    {
        fakes::io::pcm::enable_pin = enable;
    }

    bool enabled()
    {
        return fakes::io::pcm::enable_pin;
    }
} // namespace pcm

} // namespace io
