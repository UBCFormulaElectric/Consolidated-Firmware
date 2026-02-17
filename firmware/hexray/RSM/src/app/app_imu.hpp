#pragma once
namespace app::imu
{
    typedef struct
    {
        std::expected<void, ErrorCode> accel_x_fault;
        std::expected<void, ErrorCode> accel_y_fault;
        std::expected<void, ErrorCode> accel_z_fault;
        std::expected<void, ErrorCode> gyro_x_fault;
        std::expected<void, ErrorCode> gyro_y_fault;
        std::expected<void, ErrorCode> gyro_z_fault;

        inline constexpr bool hasFault()
        {
            return not (accel_x_fault.has_value() && accel_y_fault.has_value() && accel_z_fault.has_value() &&
                        gyro_x_fault.has_value() && gyro_y_fault.has_value() && gyro_z_fault.has_value());
        }
    } RSMImuFaults;

    void init();
    void broadcast();
}
