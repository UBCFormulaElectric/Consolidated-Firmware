#pragma once

extern "C"
{
    #include "io_sbgEllipse.h"
}

namespace fakes::set
{
    void reset_init();
    bool get_init();
    void set_handleLogs(void);
    void set_TimestampUs(const uint32_t timestamp);
    void set_GeneralStatus(const uint16_t general_status);
    void set_ComStatus(const uint32_t com_status);
    void set_OverflowCount(const uint32_t overflow_count);
    void set_EkfSolutionMode(const uint32_t solution_mode);
    void set_ImuAccelerations(const Vector3 imu_accels);
    void set_ImuAngularVelocities(const Attitude angular_vels);
    void set_EkfEulerAngles(const Attitude ekf_euler_angles);
    void set_EkfNavVelocityData(const VelocityData ekf_vel_data);
    void set_EkfNavPositionData(const PositionData ekf_pos_data);    
} // namespace fakes::io_imu
