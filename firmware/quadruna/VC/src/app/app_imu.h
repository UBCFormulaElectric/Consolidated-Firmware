typedef struct imuData
{
    float lat_accel;
    float long_accel; 
    float yaw_rate; 
    // float front_yaw; uncomment for quintuna 
    // float rear_yaw; uncomment for quintuna 
} imuData; 

void app_collectImuData(imuData *imu); 

