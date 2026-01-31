#include "util_errorCodes.hpp"
#include "io_imu.hpp"

using namespace io::imu;

void Imu::reset_init()
{
    initialized = false;
}

bool Imu::get_init()
{
    return initialized;
}

void Imu::set_AccelX(float accel_x)
{
    accel_x_fake = accel_x;
}

void Imu::set_AccelY(float accel_y)
{
    accel_y_fake = accel_y;
}

void Imu::set_AccelZ(float accel_z)
{
    accel_z_fake = accel_z;
}

void Imu::set_GyroRoll(float gyro_x)
{
    gyro_x_fake = gyro_x;
}
void Imu::set_GyroPitch(float gyro_y)
{
    gyro_y_fake = gyro_y;
}

void Imu::set_GyroYaw(float gyro_z)
{
    gyro_z_fake = gyro_z;
}

std::expected<void, ErrorCode> Imu::init()
{
    initialized = true;
    return {};
}
std::expected<void, ErrorCode> Imu::getAccelX(float &accel_x) const
{
    accel_x = accel_x_fake;
    return {};
}
std::expected<void, ErrorCode> Imu::getAccelY(float &accel_y) const
{
    accel_y = accel_y_fake;
    return {};
}
std::expected<void, ErrorCode> Imu::getAccelZ(float &accel_z) const
{
    accel_z = accel_z_fake;
    return {};
}
std::expected<void, ErrorCode> Imu::getGyroX(float &gyro_x) const
{
    gyro_x = gyro_x_fake;
    return {};
}
std::expected<void, ErrorCode> Imu::getGyroY(float &gyro_y) const
{
    gyro_y = gyro_y_fake;
    return {};
}
std::expected<void, ErrorCode> Imu::getGyroZ(float &gyro_z) const
{
    gyro_z = gyro_z_fake;
    return {};
}