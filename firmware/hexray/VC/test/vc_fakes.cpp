#include "vc_fakes.hpp"
#include "app_canTx.hpp"
#include "io_imus.hpp"
#include "io_pumpControl.hpp"
#include "io_efuses.hpp"
#include "io_sbgEllipse.hpp"
#include "io_powerMonitoring.hpp"
#include "io_vcShdn.hpp"
#include "io_shdnLoopNode.hpp"
#include "io_efuse_TI_TPS25.hpp"
#include "io_efuse_TI_TPS28.hpp"

io::Imu            IMU1;
io::Imu            IMU2;
io::Imu            IMU3;
io::Pump           rr_pump;
io::Pump           rl_pump;

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

std::array<std::reference_wrapper<io::Efuse>, NUM_EFUSE_CHANNELS> efuses = {
    { f_inv_efuse, r_inv_efuse, bms_efuse, rsm_efuse, dam_efuse, front_efuse, l_rad_fan_efuse, r_rad_fan_efuse,
      rl_pump_efuse, rr_pump_efuse }
};



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
namespace powerMonitoring{

    enum : uint8_t
    {
        CH1, 
        CH2,
        CH3, 
        CH4
    };

    static float power_ch1 = 0.0f;
    static float current_ch1 = 0.0f;
    static float voltage_ch1 = 0.0f;

    static float power_ch2 = 0.0f;
    static float current_ch2 = 0.0f;
    static float voltage_ch2 = 0.0f;

    static float power_ch3 = 0.0f;
    static float current_ch3 = 0.0f;
    static float voltage_ch3 = 0.0f;

    static float power_ch4 = 0.0f;
    static float current_ch4 = 0.0f;
    static float voltage_ch4 = 0.0f;

    void setPower(float power, uint8_t ch)
    {
        switch (ch){
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
        switch (ch){
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
        switch (ch){
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
}
namespace imus
{
    std::expected<void, ErrorCode> initAll()
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
    float read_power(uint8_t ch)
    {
        switch (ch){
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
        switch (ch){
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
        switch (ch){
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
    void refresh() {}
    bool init()
    {
        return true;
    }
} // namespace powerMonitoring
namespace imus
{
} // namespace imus
} // namespace io