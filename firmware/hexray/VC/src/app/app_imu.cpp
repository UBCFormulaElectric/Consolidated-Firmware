#include <algorithm>
#include <cmath>
#include "app_imu.hpp"
#include "app_canAlerts.hpp"
#include "app_canTx.hpp"

namespace app::imus
{
using namespace app::can_alerts;
using namespace io::imus;
using namespace std;


static VCImuResults imu_results = { .accel_x_res = std::unexpected(ErrorCode::ERROR),
                                    .accel_y_res = std::unexpected(ErrorCode::ERROR),
                                    .accel_z_res = std::unexpected(ErrorCode::ERROR),
                                    .gyro_x_res  = std::unexpected(ErrorCode::ERROR),
                                    .gyro_y_res  = std::unexpected(ErrorCode::ERROR),
                                    .gyro_z_res  = std::unexpected(ErrorCode::ERROR) };

result<void> imu1_init_ok = std::unexpected(ErrorCode::ERROR_INDETERMINATE);

void init()
{
    imu1_init_ok = IMU1.init();
    infos::Imu1InitFailed_set(not imu1_init_ok.has_value());
}

// Hard coded values taken from manually sampling
// In future, sample during boot
constexpr float mean_x_reading = 0.2f; 
constexpr float mean_y_reading = 0.1f; 
constexpr float mean_z_reading = 0.97468f; 

constexpr float xy_ratio = 0.0f; // mean y/x when moving straight 


const float alpha = atan2(mean_y_reading, mean_z_reading); // angle to rotate by about x axis 
const float beta = -atan2(mean_x_reading, hypot(mean_y_reading, mean_z_reading)); // angle to rotate by about y axis 
const float gamma =     -atan(xy_ratio);

const float ca = cos(alpha); 
const float cb = cos(beta); 
const float cc = cos(gamma); 
const float sa = sin(alpha); 
const float sb = sin(beta); 
const float sc = sin(gamma); 

void reorient_IMU(result<float> accelX, result<float> accelY, result<float> accelZ, 
result<float> gyroX, result<float> gyroY, result<float> gyroZ) {
    float x = accelX.value_or(0.0f);
    float y = accelY.value_or(0.0f);
    float z = accelZ.value_or(1.0f);

    float new_accelx; 
    float new_accely; 
    float new_accelz; 


    new_accelx = (cb * cc) * x
           + (sa * sb * cc - ca * sc) * y
           + (ca * sb * cc + sa * sc) * z;

    new_accely = (cb * sc) * x
           + (sa * sb * sc + ca * cc) * y
           + (ca * sb * sc - sa * cc) * z;

    new_accelz = (-sb) * x
           + (sa * cb) * y
           + (ca * cb) * z;

    // after transformation, check value and see if it is not error, update vc imu results
    imu_results =  { 
        accelX.has_value() ? new_accelx : accelX, 
        accelY.has_value() ? new_accely : accelY, 
        accelZ.has_value() ? new_accelz : accelZ, 
        gyroX, gyroY, gyroZ
    };
}

void broadcast()
{

    reorient_IMU(IMU1.getAccelX(), IMU1.getAccelY(), IMU1.getAccelZ(),
                    IMU1.getGyroX(),  IMU1.getGyroY(),  IMU1.getGyroZ());

    can_tx::VC_Imu1AccelX_set(imu_results.accel_x_res.value_or(0.0f));
    can_tx::VC_Imu1AccelY_set(imu_results.accel_y_res.value_or(0.0f));
    can_tx::VC_Imu1AccelZ_set(imu_results.accel_z_res.value_or(0.0f));
    can_tx::VC_Imu1GyroX_set(imu_results.gyro_x_res.value_or(0.0f));
    can_tx::VC_Imu1GyroY_set(imu_results.gyro_y_res.value_or(0.0f));
    can_tx::VC_Imu1GyroZ_set(imu_results.gyro_z_res.value_or(0.0f));
}

result<float> getAccelX()
{
    return imu_results.accel_x_res;
}

result<float> getAccelY()
{
    return imu_results.accel_y_res;
}

result<float> getAccelZ()
{
    return imu_results.accel_z_res;
}

result<float> getGyroX()
{
    return imu_results.gyro_x_res;
}

result<float> getGyroY()
{
    return imu_results.gyro_y_res;
}

result<float> getGyroZ()
{
    return imu_results.gyro_z_res;
}

bool initSuccess()
{
    return imu1_init_ok.has_value();
}
} // namespace app::imus
