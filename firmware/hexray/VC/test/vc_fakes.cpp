#include "vc_fakes.hpp"
#include "app_canTx.hpp"
#include "io_imus.hpp"
#include "io_batteryCharging.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_pumps.hpp"
#include "io_sbgEllipse.hpp"
#include "io_powerMonitoring.hpp"
#include "io_shdnLoopNode.hpp"
#include "io_efuse_TI_TPS25.hpp"
#include "io_efuse_TI_TPS28.hpp"
#include "io_semaphores.hpp"

// imus
io::imu IMU1;

// pumps
const io::pump rr_pump;

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
namespace fakes::io
{
namespace pcm
{
    static bool enable_pin;
}
namespace powerMonitoring
{
    static float ch_pcm_voltage = 0.0f;
    static float ch_pcm_current = 0.0f;
    static float ch_pcm_power   = 0.0f;

    static float ch_ext_voltage = 0.0f;
    static float ch_ext_current = 0.0f;
    static float ch_ext_power   = 0.0f;

    static float ch_vbat_voltage = 0.0f;
    static float ch_vbat_current = 0.0f;
    static float ch_vbat_power   = 0.0f;

    static float ch_boost_voltage = 0.0f;
    static float ch_boost_current = 0.0f;
    static float ch_boost_power   = 0.0f;

    void set_reading_voltage(const Channel channel, const float voltage)
    {
        switch (channel)
        {
            case CH1:
                fakes::io::powerMonitoring::ch_pcm_voltage = voltage;
                break;
            case CH2:
                fakes::io::powerMonitoring::ch_ext_voltage = voltage;
                break;
            case CH3:
                fakes::io::powerMonitoring::ch_vbat_voltage = voltage;
                break;
            case CH4:
                fakes::io::powerMonitoring::ch_boost_voltage = voltage;
                break;
            default:
                break;
        }
    }
    void set_reading_current(const Channel channel, const float current)
    {
        switch (channel)
        {
            case CH1:
                fakes::io::powerMonitoring::ch_pcm_current = current;
                break;
            case CH2:
                fakes::io::powerMonitoring::ch_ext_current = current;
                break;
            case CH3:
                fakes::io::powerMonitoring::ch_vbat_current = current;
                break;
            case CH4:
                fakes::io::powerMonitoring::ch_boost_current = current;
                break;
            default:
                break;
        }
    }
    void set_reading_power(const Channel channel, const float power)
    {
        switch (channel)
        {
            case CH1:
                fakes::io::powerMonitoring::ch_pcm_power = power;
                break;
            case CH2:
                fakes::io::powerMonitoring::ch_ext_power = power;
                break;
            case CH3:
                fakes::io::powerMonitoring::ch_vbat_power = power;
                break;
            case CH4:
                fakes::io::powerMonitoring::ch_boost_power = power;
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
    void imu_status_set(const bool status)
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

    void setAttitude(const float roll, const float pitch, const float yaw)
    {
        attitude = ::io::sbgEllipse::Attitude{ roll, pitch, yaw };
    }
    void setVelocity(
        const uint32_t status,
        const float    north,
        const float    east,
        const float    down,
        const float    north_std_dev,
        const float    east_std_dev,
        const float    down_std_dev)
    {
        velocity =
            ::io::sbgEllipse::VelocityData{ status, north, east, down, north_std_dev, east_std_dev, down_std_dev };
    }
    void setSolutionMode(const uint32_t mode)
    {
        solution_mode = mode;
    }
    void setTimestampUs(const uint32_t timestamp)
    {
        timestamp_us = timestamp;
    }
    void setOverflowCount(const uint8_t overflow)
    {
        overflow_count = overflow;
    }
    void setComStatus(const uint32_t status)
    {
        com_status = status;
    }
    void setGeneralStatus(const uint16_t status)
    {
        general_status = status;
    }
} // namespace sbgEllipse

} // namespace fakes::io

namespace io
{
namespace imus
{
    imu IMU1;
} // namespace imus

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
    void  set_papps_pedal_percentage(const float percentage)
    {
        papps_pedal_percentage = percentage;
    }
    void set_BrakeActuated_state(const bool actuated)
    {
        brake_actuation_state = actuated;
    }
} // namespace imus

namespace powerMonitoring
{
    result<float> read_power(const Channel ch)
    {
        std::expected<float, ErrorCode> result{ std::unexpected(ErrorCode::INVALID_ARGS) };
        switch (ch)
        {
            case CH1:
                return fakes::io::powerMonitoring::ch_pcm_power;
            case CH2:
                return fakes::io::powerMonitoring::ch_ext_power;
            case CH3:
                return fakes::io::powerMonitoring::ch_vbat_power;
            case CH4:
                return fakes::io::powerMonitoring::ch_boost_power;
            default:
                break;
        }
        return result;
    }
    result<float> read_current(const Channel ch)
    {
        std::expected<float, ErrorCode> result{ std::unexpected(ErrorCode::INVALID_ARGS) };
        switch (ch)
        {
            case CH1:
                return fakes::io::powerMonitoring::ch_pcm_current;
            case CH2:
                return fakes::io::powerMonitoring::ch_ext_current;
            case CH3:
                return fakes::io::powerMonitoring::ch_vbat_current;
            case CH4:
                return fakes::io::powerMonitoring::ch_boost_current;
            default:
                break;
        }
        return result;
    }
    result<float> read_voltage(const Channel ch)
    {
        std::expected<float, ErrorCode> result{ std::unexpected(ErrorCode::INVALID_ARGS) };
        switch (ch)
        {
            case CH1:
                return fakes::io::powerMonitoring::ch_pcm_voltage;
            case CH2:
                return fakes::io::powerMonitoring::ch_ext_voltage;
            case CH3:
                return fakes::io::powerMonitoring::ch_vbat_voltage;
            case CH4:
                return fakes::io::powerMonitoring::ch_boost_voltage;
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

    Attitude getEkfEulerAngles()
    {
        return fakes::io::sbgEllipse::attitude;
    }
    uint32_t getEkfSolutionMode()
    {
        return fakes::io::sbgEllipse::solution_mode;
    }
    VelocityData getEkfNavVelocityData()
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

const io::semaphore pwr_pump_i2c_bus_lock{ false };
