#include "util_errorCodes.hpp"
#include "io_imu.hpp"

using namespace io::imu;

void Imu::reset_init()
{
    initialized = false;
}

bool Imu::get_init() const
{
    return initialized;
}

void Imu::set_AccelX(const float accel_x_fake) const
{
    _accel_x_fake = accel_x_fake;
}

void Imu::set_AccelY(const float accel_y_fake) const
{
    _accel_y_fake = accel_y_fake;
}

void Imu::set_AccelZ(const float accel_z_fake) const
{
    _accel_z_fake = accel_z_fake;
}

void Imu::set_GyroRoll(const float gyro_x_fake) const
{
    _gyro_x_fake = gyro_x_fake;
}
void Imu::set_GyroPitch(const float gyro_y_fake) const
{
    _gyro_y_fake = gyro_y_fake;
}

void Imu::set_GyroYaw(const float gyro_z_fake) const
{
    _gyro_z_fake = gyro_z_fake;
}

std::expected<void, ErrorCode> Imu::init() const
{
    initialized = true;
    return {};
}
std::expected<float, ErrorCode> Imu::getAccelX() const
{
    return _accel_x_fake;
}
std::expected<float, ErrorCode> Imu::getAccelY() const
{
    return _accel_y_fake;
}
std::expected<float, ErrorCode> Imu::getAccelZ() const
{
    return _accel_z_fake;
}
std::expected<float, ErrorCode> Imu::getGyroX() const
{
    return _gyro_x_fake;
}
std::expected<float, ErrorCode> Imu::getGyroY() const
{
    return _gyro_y_fake;
}
std::expected<float, ErrorCode> Imu::getGyroZ() const
{
    return _gyro_z_fake;
}