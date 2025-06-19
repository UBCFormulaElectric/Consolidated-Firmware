#include "io_sbgEllipseFakes.h"

namespace fakes::io_sbgEllipse
{
static bool initialized = false;
void        reset_init()
{
    initialized = false;
}
bool get_init()
{
    return initialized;
}
// void set_handleLogs(void) {

// }
uint32_t static timestamp;
uint32_t set_TimestampUs(const uint32_t timestamp)
{
    io_sbgEllipse::timestamp = timestamp;
}
static uint16_t general_status;
uint16_t        set_GeneralStatus(const uint16_t general_status)
{
    io_sbgEllipse::general_status = general_status;
}
static uint32_t com_status;
uint32_t        set_ComStatus(const uint32_t com_status)
{
    io_sbgEllipse::com_status = com_status;
}
static uint32_t overflow_count;
uint32_t        set_OverflowCount(const uint32_t overflow_count)
{
    io_sbgEllipse::overflow_count = overflow_count;
}
static uint32_t solution_mode;
uint32_t        set_EkfSolutionMode(const uint32_t solution_mode)
{
    io_sbgEllipse::solution_mode = solution_mode;
}

static Vector3 imu_accels;
Vector3       *set_ImuAccelerations(const Vector3 imu_accels)
{
    io_sbgEllipse::imu_accels = imu_accels;
}
static Attitude angular_vels;
Attitude       *set_ImuAngularVelocities(const Attitude angular_vels)
{
    io_sbgEllipse::angular_vels = angular_vels;
}
static Attitude ekf_euler_angles;
Attitude       *set_EkfEulerAngles(const Attitude ekf_euler_angles)
{
    io_sbgEllipse::ekf_euler_angles = ekf_euler_angles;
}
static VelocityData ekf_vel_data;
VelocityData       *set_EkfNavVelocityData(const VelocityData ekf_vel_data)
{
    io_sbgEllipse::ekf_vel_data = ekf_vel_data;
}
static PositionData ekf_pos_data;
PositionData       *set_EkfNavPositionData(const PositionData ekf_pos_data)
{
    io_sbgEllipse::ekf_pos_data = ekf_pos_data
}
} // namespace fakes::io_sbgEllipse
