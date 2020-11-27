#include <App_SharedExitCode.h>
#include "Io_LSM6DS33.h"

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

struct LSM6DS33
{
    I2C_HandleTypeDef *imu_i2c_handle;
    // Acceleration in m/s^2
    float accel_x;
    float accel_y;
    float accel_z;

    ExitCode exit_code;
};

static struct LSM6DS33 lsm_6ds33;

void Io_LSM6DS33_ExtiCallback()
{
    lsm_6ds33.accel_x = 0.0f;
    lsm_6ds33.accel_y = 0.0f;
    lsm_6ds33.accel_z = 0.0f;

    lsm_6ds33.exit_code = EXIT_CODE_OK;
}

ExitCode Io_LSM6DS33_GetExitCode()
{
    return lsm_6ds33.exit_code;
}

float Io_LSM6DS33_GetAccelerationX()
{
    return lsm_6ds33.accel_x;
}

float Io_LSM6DS33_GetAccelerationY()
{
    return lsm_6ds33.accel_y;
}

float Io_LSM6DS33_GetAccelerationZ()
{
    return lsm_6ds33.accel_z;
}
