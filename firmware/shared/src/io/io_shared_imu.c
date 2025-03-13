#include "io_imu.h"
#include "hw_i2c.h"

// IMU ACC Configs
#define IMU_ACC_RANGE_4G
#define IMU_ACC_RELOAD_104Hz

// IMU Gyro Configs
#define IMU_GYRO_RESOLUTION_125DPS
#define IMU_GYRO_RELOAD_104Hz


#define XL_H_PERF_MODE_DIS_BIT 4 // disabled by default can be enabled in REG 0x15
#define G_H_PERF_MODE_DIS_BIT 7  // disabled by default can be enabled in REG 0x16
#define XL_LPF1_EN_BIT 1         // enable bit in CTRL REG 1 0x10 -- BANDWIDTH will be ODR/ 4  -> 104/4  = 26 
// Gyro LPF did not reduce the cutoff freq it only changed the phase delay which would not actuate the output but just delay the output 

// IMU Register Addresses
#define XL_OUTPUT_RATE_REG 0x10  // [3:0] select output rate
#define G_OUTPUT_RATE_REG 0x11   // [3:0] select output rate
// High Byte Reg = Low Byte Reg + 1
#define G_ROLL_LOW_BYTE_REG 0x22
#define G_PITCH_LOW_BYTE_REG 0x24
#define G_YAW_LOW_BYTE_REG 0x26
#define XL_X_LOW_BYTE_REG 0x28
#define XL_Y_LOW_BYTE_REG 0x2A
#define XL_Z_LOW_BYTE_REG 0x2C

bool io_shared_imu_init(const I2cDevice *imu_i2c_handle)
{
    if (!hw_i2c_isTargetReady(imu_i2c_handle))
    {
        return false;
    }

#ifdef IMU_ACC_RANGE_2G
#define ACC_CONFIG_RANGE (0)
#endif
#ifdef IMU_ACC_RANGE_4G
#define ACC_CONFIG_RANGE (2)
#endif
#ifdef IMU_ACC_RANGE_8G
#define ACC_CONFIG_RANGE (3)
#endif
#ifdef IMU_ACC_RANGE_16G
#define ACC_CONFIG_RANGE (1)
#endif

#ifdef IMU_ACC_RELOAD_12Hz5
#define ACC_CONFIG_RELOAD (0x10)
#endif
#ifdef IMU_ACC_RELOAD_26Hz
#define ACC_CONFIG_RELOAD (0x20)
#endif
#ifdef IMU_ACC_RELOAD_52Hz
#define ACC_CONFIG_RELOAD (0x30)
#endif
#ifdef IMU_ACC_RELOAD_104Hz
#define ACC_CONFIG_RELOAD (0x40)
#endif
#ifdef IMU_ACC_RELOAD_208Hz
#define ACC_CONFIG_RELOAD (0x50)
#endif
#ifdef IMU_ACC_RELOAD_416Hz
#define ACC_CONFIG_RELOAD (0x60)
#endif
#ifdef IMU_ACC_RELOAD_833Hz
#define ACC_CONFIG_RELOAD (0x70)
#endif
#ifdef IMU_ACC_RELOAD_1k66Hz
#define ACC_CONFIG_RELOAD (0x80)
#endif
#ifdef IMU_ACC_RELOAD_3k33Hz
#define ACC_CONFIG_RELOAD (0x90)
#endif
#ifdef IMU_ACC_RELOAD_6k66Hz
#define ACC_CONFIG_RELOAD (0xA0)
#endif
#ifdef IMU_ACC_RELOAD_1Hz6
#define ACC_CONFIG_RELOAD (0xB0)
#endif

#ifdef IMU_GYRO_RESOLUTION_125DPS
#define GYRO_CONFIG_RESOLUTION (1)
#endif
#ifdef IMU_GYRO_RESOLUTION_250DPS
#define GYRO_CONFIG_RESOLUTION (0)
#endif
#ifdef IMU_GYRO_RESOLUTION_500DPS
#define GYRO_CONFIG_RESOLUTION (2)
#endif
#ifdef IMU_GYRO_RESOLUTION_1000DPS
#define GYRO_CONFIG_RESOLUTION (4)
#endif
#ifdef IMU_GYRO_RESOLUTION_2000DPS
#define GYRO_CONFIG_RESOLUTION (6)
#endif

#ifdef IMU_GYRO_RELOAD_12Hz5
#define GYRO_CONFIG_RELOAD (0x10)
#endif
#ifdef IMU_GYRO_RELOAD_26Hz
#define GYRO_CONFIG_RELOAD (0x20)
#endif
#ifdef IMU_GYRO_RELOAD_52Hz
#define GYRO_CONFIG_RELOAD (0x30)
#endif
#ifdef IMU_GYRO_RELOAD_104Hz
#define GYRO_CONFIG_RELOAD (0x40)
#endif
#ifdef IMU_GYRO_RELOAD_208Hz
#define GYRO_CONFIG_RELOAD (0x50)
#endif
#ifdef IMU_GYRO_RELOAD_416Hz
#define GYRO_CONFIG_RELOAD (0x60)
#endif
#ifdef IMU_GYRO_RELOAD_833Hz
#define GYRO_CONFIG_RELOAD (0x70)
#endif
#ifdef IMU_GYRO_RELOAD_1k66Hz
#define GYRO_CONFIG_RELOAD (0x80)
#endif
#ifdef IMU_GYRO_RELOAD_3k33Hz
#define GYRO_CONFIG_RELOAD (0x90)
#endif
#ifdef IMU_GYRO_RELOAD_6k66Hz
#define GYRO_CONFIG_RELOAD (0xA0)
#endif

    const uint8_t acc_config  = ACC_CONFIG_RELOAD | ACC_CONFIG_RANGE, // | (1U << XL_LPF1_EN_BIT)  enable LPF
                  gyro_config = GYRO_CONFIG_RELOAD | GYRO_CONFIG_RESOLUTION;
    return hw_i2c_memoryWrite(imu_i2c_handle, XL_OUTPUT_RATE_REG, &acc_config, 1) &&
           hw_i2c_memoryWrite(imu_i2c_handle, G_OUTPUT_RATE_REG, &gyro_config, 1);
}

/**
 * Converts raw gyro data to angular velocity in degrees per second
 * @param gyro_data Raw gyro data, must be 2 bytes long
 * @return Translated gyro data
 */
static float translate_gyro_data(const uint8_t *gyro_data)
{
// Default gyroscope sensitivity for IMU is 8.75 mdeg/least significant digit
#ifdef IMU_GYRO_RESOLUTION_125DPS
#define GYRO_SENSITIVITY_MDEG (4.375f)
#endif
#ifdef IMU_GYRO_RESOLUTION_250DPS
#define GYRO_SENSITIVITY_MDEG (8.75f)
#endif
#ifdef IMU_GYRO_RESOLUTION_500DPS
#define GYRO_SENSITIVITY_MDEG (17.5f)
#endif
#ifdef IMU_GYRO_RESOLUTION_1000DPS
#define GYRO_SENSITIVITY_MDEG (35.0f)
#endif
#ifdef IMU_GYRO_RESOLUTION_2000DPS
#define GYRO_SENSITIVITY_MDEG (70.0f)
#endif

    // ReSharper disable once CppRedundantCastExpression
    const int16_t raw = (int16_t)(gyro_data[1] << 8 | gyro_data[0]);
    return (float)raw * GYRO_SENSITIVITY_MDEG / 1000.0f;
}

/**
 * Converts raw acceleration data to acceleration in m/s^2
 * @param acc_data Raw acceleration data, must be 2 bytes long
 * @return Translated acceleration data
 */
static float translate_acceleration_data(const uint8_t *acc_data)
{
#ifdef IMU_ACC_RANGE_2G
#define ACC_SENSITIVITY_MG (0.061f)
#endif
#ifdef IMU_ACC_RANGE_4G
#define ACC_SENSITIVITY_MG (0.122f)
#endif
#ifdef IMU_ACC_RANGE_8G
#define ACC_SENSITIVITY_MG (0.244f)
#endif
#ifdef IMU_ACC_RANGE_16G
#define ACC_SENSITIVITY_MG (0.488f)
#endif

    // ReSharper disable once CppRedundantCastExpression
    const int16_t raw = (int16_t)(acc_data[1] << 8 | acc_data[0]);
    return (float)raw * ACC_SENSITIVITY_MG * 9.81f / 1000.0f;
}

bool io_shared_imu_getLinearAccelerationX(const I2cDevice *imu_i2c_handle, float *x_acceleration)
{
    uint8_t x_data[2];
    if (!hw_i2c_memoryRead(imu_i2c_handle, XL_X_LOW_BYTE_REG, x_data, 2))
        return false;
    // Convert raw value to acceleration in m/s^2
    *x_acceleration = translate_acceleration_data(x_data);
    return true;
}

bool io_shared_imu_getLinearAccelerationY(const I2cDevice *imu_i2c_handle, float *y_acceleration)
{
    uint8_t y_data[2];
    if (!hw_i2c_memoryRead(imu_i2c_handle, XL_Y_LOW_BYTE_REG, y_data, 2))
        return false;
    // Convert raw value to acceleration in m/s^2
    *y_acceleration = translate_acceleration_data(y_data);
    return true;
}

bool io_shared_imu_getLinearAccelerationZ(const I2cDevice *imu_i2c_handle, float *z_acceleration)
{
    uint8_t z_data[2];
    if (!hw_i2c_memoryRead(imu_i2c_handle, XL_Z_LOW_BYTE_REG, z_data, 2))
        return false;
    // Convert raw value to acceleration in m/s^2
    *z_acceleration = translate_acceleration_data(z_data);
    return true;
}

bool io_shared_imu_getAngularVelocityRoll(const I2cDevice *imu_i2c_handle, float *roll_velocity)
{
    uint8_t roll_data[2];
    if (!hw_i2c_memoryRead(imu_i2c_handle, G_ROLL_LOW_BYTE_REG, roll_data, 2)) // reading the high and low register
        return false;
    // Convert raw value to angular velocity (degrees per second or radians per second as required)
    *roll_velocity = translate_gyro_data(roll_data);
    return true;
}

bool io_shared_imu_getAngularVelocityPitch(const I2cDevice *imu_i2c_handle, float *pitch_velocity)
{
    uint8_t pitch_data[2];
    if (!hw_i2c_memoryRead(imu_i2c_handle, G_PITCH_LOW_BYTE_REG, pitch_data, 2))
        return false;
    // Convert raw value to angular velocity (degrees per second or radians per second as required)
    *pitch_velocity = translate_gyro_data(pitch_data);
    return true;
}

bool io_shared_imu_getAngularVelocityYaw(const I2cDevice *imu_i2c_handle, float *yaw_velocity)
{
    uint8_t yaw_data[2];
    if (!hw_i2c_memoryRead(imu_i2c_handle, G_YAW_LOW_BYTE_REG, yaw_data, 2))
        return false;
    // Convert raw value to angular velocity (degrees per second or radians per second as required)
    *yaw_velocity = translate_gyro_data(yaw_data);
    return true;
}
