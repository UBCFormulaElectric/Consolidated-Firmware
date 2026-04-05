#include "vc_fakes.hpp"

namespace fakes::io // Define the mocked functions here
{
namespace bspdWarning
{
    float papps_pedal_percentage = 0.0f;
    bool brake_actuation_state = false;
    void set_papps_pedal_percentage(float percentage){
        papps_pedal_percentage = percentage;
    }
    void set_BrakeActuated_state(bool actuated){
        brake_actuation_state = actuated;
    }
} // namespace bspdWarning

// namespace imus
// {
//     std::expected<void, ErrorCode> initAll_status(bool imu1_status, bool imu2_status, bool imu3_status)
//     {
//         if (auto result = IMU1.init(); not result)
//         {
//             return result;
//         }
//         if (auto result = IMU2.init(); not result)
//         {
//             return result;
//         }
//         if (auto result = IMU3.init(); not result)
//         {
//             return result;
//         }
//         return {};
//     }
    
// } // namespace imus

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
    float read_power(unsigned char)
    {
        return 0.0f;
    }
    float read_current(unsigned char)
    {
        return 0.0f;
    }
    float read_voltage(unsigned char)
    {
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
} // namespace io::fakes