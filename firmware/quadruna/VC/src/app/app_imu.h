
typedef struct imu_data
{
    float long_accel;
    float lat_accel;
    // float front_yaw_rate; uncomment when moved to shared 
    // float rear_yaw_rate;  uncomment when moved to shared
    
}imu_data;

void app_collect_imu_data(imu_data *imu);