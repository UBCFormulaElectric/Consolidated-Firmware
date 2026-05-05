#include "util_errorCodes.hpp"
#include "io_imu.hpp"

using namespace io;

void imu::reset_init()
{
    initialized = false;
}
void Imu::imu_status_set(bool status)
{
    init_status = status ? std::expected<void, ErrorCode>{} : std::unexpected(ErrorCode::TIMEOUT);
}

bool imu::get_init() const
{
    return initialized;
}

void imu::set_AccelX(const float accel_x_fake) const
{
    _accel_x_fake = accel_x_fake;
}

void imu::set_AccelY(const float accel_y_fake) const
{
    _accel_y_fake = accel_y_fake;
}

void imu::set_AccelZ(const float accel_z_fake) const
{
    _accel_z_fake = accel_z_fake;
}

void imu::set_GyroRoll(const float gyro_x_fake) const
{
    _gyro_x_fake = gyro_x_fake;
}
void imu::set_GyroPitch(const float gyro_y_fake) const
{
    _gyro_y_fake = gyro_y_fake;
}

void imu::set_GyroYaw(const float gyro_z_fake) const
{
    _gyro_z_fake = gyro_z_fake;
}

std::expected<void, ErrorCode> imu::init() const
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
std::expected<float, ErrorCode> imu::getAccelX() const
{
    return _accel_x_fake;
}
std::expected<float, ErrorCode> imu::getAccelY() const
{
    return _accel_y_fake;
}
std::expected<float, ErrorCode> imu::getAccelZ() const
{
    return _accel_z_fake;
}
std::expected<float, ErrorCode> imu::getGyroX() const
{
    return _gyro_x_fake;
}
std::expected<float, ErrorCode> imu::getGyroY() const
{
    return _gyro_y_fake;
}
std::expected<float, ErrorCode> imu::getGyroZ() const
{
    return _gyro_z_fake;
}

std::expected<imu::AccelData, ErrorCode> imu::getAccelAll() const
{
    return AccelData{ _accel_x_fake, _accel_y_fake, _accel_z_fake };
}

std::expected<imu::GyroData, ErrorCode> imu::getGyroAll() const
{
    return GyroData{ _gyro_x_fake, _gyro_y_fake, _gyro_z_fake };
}
