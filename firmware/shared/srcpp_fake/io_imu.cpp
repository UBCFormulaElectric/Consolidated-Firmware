#include "util_errorCodes.hpp"
#include "io_imu.hpp"

using namespace io;

void imu::reset_init()
{
    initialized = false;
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

result<void> imu::init() const
{
    initialized = true;
    return {};
}
result<float> imu::getAccelX() const
{
    return _accel_x_fake;
}
result<float> imu::getAccelY() const
{
    return _accel_y_fake;
}
result<float> imu::getAccelZ() const
{
    return _accel_z_fake;
}
result<float> imu::getGyroX() const
{
    return _gyro_x_fake;
}
result<float> imu::getGyroY() const
{
    return _gyro_y_fake;
}
result<float> imu::getGyroZ() const
{
    return _gyro_z_fake;
}

result<imu::AccelData> imu::getAccelAll() const
{
    return AccelData{ _accel_x_fake, _accel_y_fake, _accel_z_fake };
}

result<imu::GyroData> imu::getGyroAll() const
{
    return GyroData{ _gyro_x_fake, _gyro_y_fake, _gyro_z_fake };
}
