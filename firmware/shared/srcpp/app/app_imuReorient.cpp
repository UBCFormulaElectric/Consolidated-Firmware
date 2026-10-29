#include "app_imuReorient.hpp"
#include "util_errorCodes.hpp"
#include <cmath>
#include <array>

using namespace std; 
namespace app::imu_reorient 
{
void init(imu_reorient &reorient, const imu_reorient_config &conf)
{
    reorient.x_bias     = conf.x_bias;
    reorient.y_bias     = conf.y_bias;
    reorient.z_bias     = conf.z_bias;
    reorient.roll_bias  = conf.roll_bias;
    reorient.pitch_bias = conf.pitch_bias;
    reorient.yaw_bias   = conf.yaw_bias;

    float corrected_x = conf.mean_x_reading - conf.x_bias;
    float corrected_y = conf.mean_y_reading - conf.y_bias;
    float corrected_z = conf.mean_z_reading - conf.z_bias;

    float alpha = atan2(corrected_y, corrected_z);                      // angle to rotate by about x axis
    float beta  = -atan2(corrected_x, hypot(corrected_y, corrected_z)); // angle to rotate by about y axis
    float gamma = -atan(conf.xy_ratio);

    float cos_alpha = cos(alpha);
    float cos_beta  = cos(beta);
    float cos_gamma = cos(gamma);
    float sin_alpha = sin(alpha);
    float sin_beta  = sin(beta);
    float sin_gamma = sin(gamma);
    
    reorient . R = {
        { { { cos_beta * cos_gamma, sin_alpha *sin_beta *cos_gamma - cos_alpha *sin_gamma,
              cos_alpha *sin_beta *cos_gamma + sin_alpha *sin_gamma } },
          { { cos_beta * sin_gamma, sin_alpha *sin_beta *sin_gamma + cos_alpha *cos_gamma,
              cos_alpha *sin_beta *sin_gamma - sin_alpha *cos_gamma } },
          { { -sin_beta, sin_alpha *cos_beta, cos_alpha *cos_beta } } }
    }; // rotation matrix R = Rz(gamma) * Ry(beta) * Rx(alpha)
}

imu_readings apply(imu_reorient &reorient, imu_readings &imu) {
    imu_readings imu_results; 
    if (imu.accelX.has_value() && imu.accelY.has_value() && imu.accelZ.has_value())
    {
        const float x           = imu.accelX.value() - reorient.x_bias;
        const float y           = imu.accelY.value() - reorient.y_bias;
        const float z           = imu.accelZ.value() - reorient.z_bias;
        imu_results.accelX = reorient. R[0][0] * x + reorient.R[0][1] * y + reorient.R[0][2] * z;
        imu_results.accelY = reorient.R[1][0] * x + reorient.R[1][1] * y + reorient.R[1][2] * z;
        imu_results.accelZ = reorient.R[2][0] * x + reorient.R[2][1] * y + reorient.R[2][2] * z;
    }
    else
    {
        imu_results.accelX = std::unexpected(ErrorCode::ERROR);
        imu_results.accelY = std::unexpected(ErrorCode::ERROR);
        imu_results.accelZ = std::unexpected(ErrorCode::ERROR);
    }

    if (imu.gyroX.has_value() && imu.gyroY.has_value() && imu.gyroZ.has_value())
    {
        const float x          = imu.gyroX.value() - reorient.roll_bias;
        const float y          = imu.gyroY.value() - reorient.pitch_bias;
        const float z          = imu.gyroZ.value() - reorient.yaw_bias;
        imu_results.gyroX = reorient. R[0][0] * x + reorient.R[0][1] * y + reorient.R[0][2] * z;
        imu_results.gyroY = reorient.R[1][0] * x + reorient.R[1][1] * y + reorient.R[1][2] * z;
        imu_results.gyroZ = reorient.R[2][0] * x + reorient.R[2][1] * y + reorient.R[2][2] * z;
    }
    else
    {
        imu_results.gyroX = std::unexpected(ErrorCode::ERROR);
        imu_results.gyroY = std::unexpected(ErrorCode::ERROR);
        imu_results.gyroZ = std::unexpected(ErrorCode::ERROR);
    }

    return imu_results; 
}
}
