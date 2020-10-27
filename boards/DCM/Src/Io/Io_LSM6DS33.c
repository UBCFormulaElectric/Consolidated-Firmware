#include "Io_LSM6DS33.h"
#include "App_Imu.h"

// The I2c slave device addresses to use when reading/writing to/from the IMU
// See table 11 in section 6.1.1 of the LSM6DS33 data sheet for details
// Register Address Constants
#define FUNC_CFG_ACCESS 0x01
#define ORIENT_CFG_G 0x0B
#define INT1_CTRL 0x0D
#define INT2_CTRL 0x0E
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL3_C 0x12
#define CTRL4_C 0x13
#define CTRL6_C 0x15
#define CTRL7_C 0x16
#define CTRL8_XL 0x17
#define CTRL9_XL 0x18
#define CTRL10_C 0x19
#define STATUS_REG 0x1E
#define DATA_OUT_STRT 0x22
#define IMU_READ_ADDR 0xd7
#define IMU_WRITE_ADDR 0xd6

struct ImuData
{
    // Acceleration in m/s^2
    float accel_x;
    float accel_y;
    float accel_z;

    // The time (in ms) that this data was received
    uint32_t received_time_ms;
};

struct LSM6DS33
{
    I2C_HandleTypeDef *imu_i2c_handle;
};

static struct LSM6DS33 lsm_6ds33;

static struct ImuData most_recently_received_data = { 0 };
static bool           most_recently_received_data_valid;
static bool           initialized = false;

/**
 * Send the given data to the IMU
 * @param register_address The register address to write data to
 * @param data The data to send
 * @param data_size The size of the "data" array
 * @return The status indicated by the HAL library for the I2c required
 * operations
 */
HAL_StatusTypeDef Io_LSM6DS33_WriteToImu(
    uint16_t register_address,
    uint8_t *data,
    uint16_t data_size)
{
    return HAL_I2C_Mem_Write(
        lsm_6ds33.imu_i2c_handle, IMU_WRITE_ADDR, register_address,
        I2C_MEMADD_SIZE_8BIT, data, data_size, 1000);
}

/**
 * Converts acceleration values from the IMU into m/s^2
 * @param accel_from_imu
 * @return
 */
static float Io_LSM6DS33_ConvertIMUAccelerationToMetersPerSecondSquared(
    int16_t accel_from_imu)
{
    const float max_abs_g_force           = 2.0f;
    const float max_abs_raw_accel_reading = 1 << 15;

    return accel_from_imu * max_abs_g_force / max_abs_raw_accel_reading * 9.8f;
}

/**
 * Read data from the IMU registers
 * @param read_start_address The address on the IMU to start reading from
 * @param data The array to save the read data into
 * @param data_size The size of the array (in # of bytes)
 * @return The status indicated by the HAL library for the I2c required
 * operations
 */
HAL_StatusTypeDef Io_LSM6DS33_ReadFromImu(
    uint16_t read_start_address,
    uint8_t *data,
    uint16_t data_size)
{
    return HAL_I2C_Mem_Read(
        lsm_6ds33.imu_i2c_handle, IMU_READ_ADDR, read_start_address,
        I2C_MEMADD_SIZE_8BIT, data, data_size, 100);
}

HAL_StatusTypeDef Io_LSM6DS33_ConfigureImu(I2C_HandleTypeDef *i2c_handle)
{
    lsm_6ds33.imu_i2c_handle = i2c_handle;
    HAL_StatusTypeDef status = HAL_OK;

    //===== Initialize the IMU chip =======

    // Wait until the device is ready to communicate
    status =
        HAL_I2C_IsDeviceReady(lsm_6ds33.imu_i2c_handle, IMU_READ_ADDR, 3, 1000);

    // Enable Accelerometer X,Y,Z axis
    if (status == HAL_OK)
    {
        uint8_t data = 0x38; // 0b00_111_000
        status       = Io_LSM6DS33_WriteToImu(CTRL9_XL, &data, 1);
    }

    // Set Accelerometer to read at 416hz, max acceleration to +/-2g,
    // anti-aliasing filter bandwidth at 400hz
    if (status == HAL_OK)
    {
        uint8_t data = 0x60; // 0b0110_00_00
        status       = Io_LSM6DS33_WriteToImu(CTRL1_XL, &data, 1);
    }

    // Enable Gyroscope X,Y,Z axis
    if (status == HAL_OK)
    {
        uint8_t data = 0x38; // 0b00_111_0_0_0
        status       = Io_LSM6DS33_WriteToImu(CTRL10_C, &data, 1);
    }

    // Set Gyroscope to read at 416hz, with max expected rotation of 250dps
    // (degrees per second)
    if (status == HAL_OK)
    {
        uint8_t data = 0x60; // 0b0110_00_0_0
        status       = Io_LSM6DS33_WriteToImu(CTRL2_G, &data, 1);
    }

    // Set IMU to trigger an interrupt on INT2 whenever there is new
    // gyroscope or accelerometer data available
    if (status == HAL_OK)
    {
        uint8_t data = 0x3; // 0b00000011
        status       = Io_LSM6DS33_WriteToImu(INT2_CTRL, &data, 1);
    }

    initialized = (status == HAL_OK);

    return status;
}

bool Io_LSM6DS33_UpdateImuData()
{
    uint8_t           data[12];
    HAL_StatusTypeDef status = HAL_ERROR;

    if (initialized)
    {
        status = Io_LSM6DS33_ReadFromImu(DATA_OUT_STRT, data, 12);
    }

    uint8_t status_reg_val = 0;
    Io_LSM6DS33_ReadFromImu(STATUS_REG, &status_reg_val, 1);

    uint32_t current_time_in_ms = HAL_GetTick();

    if (status == HAL_OK && status_reg_val != 0)
    {
        most_recently_received_data.received_time_ms = current_time_in_ms;

        // This is how you can read data from the gyroscope. Note that these
        // are raw values, please refer to the spec sheet (section 9.12) for
        // information on how to convert it to meaningful information
        // int16_t gyro_x = (data[1] << 8) + data[0];
        // int16_t gyro_y = (data[3] << 8) + data[2];
        // int16_t gyro_z = (data[5] << 8) + data[4];

        int16_t raw_accel_x = (int16_t)((data[7] << 8) + data[6]);
        int16_t raw_accel_y = (int16_t)((data[9] << 8) + data[8]);
        int16_t raw_accel_z = (int16_t)((data[11] << 8) + data[10]);

        most_recently_received_data.accel_x =
            Io_LSM6DS33_ConvertIMUAccelerationToMetersPerSecondSquared(
                raw_accel_x);
        most_recently_received_data.accel_y =
            Io_LSM6DS33_ConvertIMUAccelerationToMetersPerSecondSquared(
                raw_accel_y);
        most_recently_received_data.accel_z =
            Io_LSM6DS33_ConvertIMUAccelerationToMetersPerSecondSquared(
                raw_accel_z);

        most_recently_received_data_valid = true;
    }
    else
    {
        most_recently_received_data_valid = false;
    }

    return ((status == HAL_OK) && most_recently_received_data_valid);
}

float Io_LSM6DS33_GetAccelerationX()
{
    return most_recently_received_data.accel_x;
}

float Io_LSM6DS33_GetAccelerationY()
{
    return most_recently_received_data.accel_y;
}

float Io_LSM6DS33_GetAccelerationZ()
{
    return most_recently_received_data.accel_z;
}
