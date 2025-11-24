#include "io_imu.hpp"

namespace io::imu
{
    // Self-Test Registers
    static constexpr uint8_t SELF_TEST_X_GYRO = 0x00;
    static constexpr uint8_t SELF_TEST_Y_GYRO = 0x01;
    static constexpr uint8_t SELF_TEST_Z_GYRO = 0x02;
    static constexpr uint8_t SELF_TEST_X_ACCEL = 0x0D;
    static constexpr uint8_t SELF_TEST_Y_ACCEL = 0x0E;
    static constexpr uint8_t SELF_TEST_Z_ACCEL = 0x0F;

    // Gyro Offset Registers
    static constexpr uint8_t XG_OFFS_USRH = 0x13;
    static constexpr uint8_t XG_OFFS_USRL = 0x14;
    static constexpr uint8_t YG_OFFS_USRH = 0x15;
    static constexpr uint8_t YG_OFFS_USRL = 0x16;
    static constexpr uint8_t ZG_OFFS_USRH = 0x17;
    static constexpr uint8_t ZG_OFFS_USRL = 0x18;

    // Configuration Registers
    static constexpr uint8_t SMPLRT_DIV = 0x19;
    static constexpr uint8_t CONFIG = 0x1A;
    static constexpr uint8_t GYRO_CONFIG = 0x1B;
    static constexpr uint8_t ACCEL_CONFIG = 0x1C;
    static constexpr uint8_t ACCEL_CONFIG_2 = 0x1D;
    static constexpr uint8_t LP_MODE_CFG = 0x1E;
    static constexpr uint8_t ACCEL_WOM_THR = 0x1F;

    // FIFO and Interrupt Registers
    static constexpr uint8_t FIFO_EN = 0x23;
    static constexpr uint8_t FSYNC_INT = 0x36;
    static constexpr uint8_t INT_PIN_CFG = 0x37;
    static constexpr uint8_t INT_ENABLE = 0x38;
    static constexpr uint8_t INT_STATUS = 0x3A;

    // Accelerometer Output Registers
    static constexpr uint8_t ACCEL_XOUT_H = 0x3B;
    static constexpr uint8_t ACCEL_XOUT_L = 0x3C;
    static constexpr uint8_t ACCEL_YOUT_H = 0x3D;
    static constexpr uint8_t ACCEL_YOUT_L = 0x3E;
    static constexpr uint8_t ACCEL_ZOUT_H = 0x3F;
    static constexpr uint8_t ACCEL_ZOUT_L = 0x40;

    // Temperature Output Registers
    static constexpr uint8_t TEMP_OUT_H = 0x41;
    static constexpr uint8_t TEMP_OUT_L = 0x42;

    // Gyroscope Output Registers
    static constexpr uint8_t GYRO_XOUT_H = 0x43;
    static constexpr uint8_t GYRO_XOUT_L = 0x44;
    static constexpr uint8_t GYRO_YOUT_H = 0x45;
    static constexpr uint8_t GYRO_YOUT_L = 0x46;
    static constexpr uint8_t GYRO_ZOUT_H = 0x47;
    static constexpr uint8_t GYRO_ZOUT_L = 0x48;

    // Signal Path and Control Registers
    static constexpr uint8_t SIGNAL_PATH_RESET = 0x68;
    static constexpr uint8_t ACCEL_INTEL_CTRL = 0x69;
    static constexpr uint8_t USER_CTRL = 0x6A;
    static constexpr uint8_t PWR_MGMT_1 = 0x6B;
    static constexpr uint8_t PWR_MGMT_2 = 0x6C;

    // FIFO Registers
    static constexpr uint8_t FIFO_COUNTH = 0x72;
    static constexpr uint8_t FIFO_COUNTL = 0x73;
    static constexpr uint8_t FIFO_R_W = 0x74;

    // Device Identification
    static constexpr uint8_t WHO_AM_I = 0x75;

    // Accelerometer Offset Registers
    static constexpr uint8_t XA_OFFSET_H = 0x77;
    static constexpr uint8_t XA_OFFSET_L = 0x78;
    static constexpr uint8_t YA_OFFSET_H = 0x7A;
    static constexpr uint8_t YA_OFFSET_L = 0x7B;
    static constexpr uint8_t ZA_OFFSET_H = 0x7D;
    static constexpr uint8_t ZA_OFFSET_L = 0x7E;

    // Expected WHO_AM_I Value
    static constexpr uint8_t WHO_AM_I_VALUE = 0xF8;
        
    imu(const SpiDevice &in_imu_spi_handle) : imu_spi_handle(in_imu_spi_handle) 
    {

    }
    
    ExitCode getAccelX(float &accel_x)
    {
        
    }

    ExitCode getAccelY(float &accel_y)
    {

    }

    ExitCode getAccelZ(float &accel_z)
    {

    }

    ExitCode getGyroX(float &gyro_x)
    {

    }

    ExitCode getGyroY(float &gyro_y)
    {

    }

    ExitCode getGyroZ(float &gyro_z)
    {

    }

    ExitCode getTemp(float &temp)
    {

    }

    ExitCode getAccelAll(AccelData &data)
    {

    }

    ExitCode getGyroAll(GyroData &data)
    {

    }

}