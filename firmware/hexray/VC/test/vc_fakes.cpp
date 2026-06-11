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

const io::shdn::node tsms_node(app::can_tx::VC_TSMSOKStatus_set);
const io::shdn::node inertia_stop_node(app::can_tx::VC_InertiaSwitch_set);
const io::shdn::node rear_right_motor_interlock_node(app::can_tx::VC_RearRightMotorInterlock_set);
const io::shdn::node splitter_box_interlock_node(app::can_tx::VC_MSDOrEMeterOKStatus_set);
namespace io // Define the mocked functions here
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
    result<void> initAll()
    {
        if (auto result = IMU1.init(); not result)
        {
            return result;
        }
        if (auto result = IMU2.init(); not result)
        {
            return result;
        }
        if (auto result = IMU3.init(); not result)
        {
            return result;
        }
        return {};
    }
} // namespace imus
namespace sbgEllipse
{
    result<void> init()
    {
        return {};
    }
    const Attitude getEkfEulerAngles()
    {
        return Attitude{ 0.0f, 0.0f, 0.0f };
    }
    uint32_t getEkfSolutionMode()
    {
        return 0;
    }
    const VelocityData getEkfNavVelocityData()
    {
        return VelocityData{ 0u, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    }
    uint32_t getTimestampUs()
    {
        return 0;
    }
    uint8_t getOverflowCount()
    {
        return 0;
    }
    uint32_t getComStatus()
    {
        return 0;
    }
    uint16_t getGeneralStatus()
    {
        return 0;
    }
    void broadcast() {}
} // namespace sbgEllipse

namespace powerMonitoring
{
    result<float> read_power(Channel)
    {
        return 0.0f;
    }
    result<float> read_current(Channel)
    {
        return 0.0f;
    }
    result<float> read_voltage(Channel)
    {
        return 0.0f;
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
namespace imus
{
} // namespace imus
} // namespace io