#pragma once
#include "util_errorCodes.hpp"
#include <array>
using namespace std; 

namespace app::imu_reorient {
    using Rotation = array<array<float, 3>, 3>;
    
    typedef struct  {
        float x_bias; 
        float y_bias; 
        float z_bias; 
        float roll_bias; 
        float pitch_bias; 
        float yaw_bias; 
        Rotation R;
    } imu_reorient; 

    typedef struct {
        result<float> accelX;
        result<float> accelY;
        result<float> accelZ;
        result<float> gyroX;
        result<float> gyroY;
        result<float> gyroZ;
    } imu_readings; 

    typedef struct { 
        // Mean stationary IMU readings 
        float mean_x_reading; 
        float mean_y_reading; 
        float mean_z_reading;

        // IMU manufacturer biases 
        float x_bias;
        float y_bias;
        float z_bias;
        float roll_bias;
        float pitch_bias;
        float yaw_bias;

        // Mean accel y / Mean accel x during straight acceleration 
        float xy_ratio; 
    } imu_reorient_config;

    void init(imu_reorient &reorient, const imu_reorient_config &conf);
imu_readings apply(imu_reorient &reorient, imu_readings &imu);
}

    
