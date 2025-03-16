#include "hw_i2cs.h"
#include "hw_i2c.h"
#include "io_imu.h"
typedef struct imuData
{
    float long_accel; 
    float lat_accel; 
    float z_accel;
    float yaw_rate;
    float pitch_rate;
    float roll_rate; 
} imuData; 

extern const imuConfig imu_config;