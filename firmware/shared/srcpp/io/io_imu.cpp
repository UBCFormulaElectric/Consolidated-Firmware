#include "io_imu.hpp"

namespace io::imu
{
// Self-Test Registers
static constexpr uint8_t SELF_TEST_X_GYRO  = 0x00;
static constexpr uint8_t SELF_TEST_Y_GYRO  = 0x01;
static constexpr uint8_t SELF_TEST_Z_GYRO  = 0x02;
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

// Configuration Registers and offsets
static constexpr uint8_t SMPLRT_DIV     = 0x19;
static constexpr uint8_t CONFIG         = 0x1A;
static constexpr uint8_t GYRO_CONFIG    = 0x1B;
static constexpr uint8_t ACCEL_CONFIG   = 0x1C;
static constexpr uint8_t ACCEL_CONFIG_2 = 0x1D;
static constexpr uint8_t LP_MODE_CFG    = 0x1E;
static constexpr uint8_t ACCEL_WOM_THR  = 0x1F;

// FIFO and Interrupt Registers
static constexpr uint8_t FIFO_EN     = 0x23;
static constexpr uint8_t FSYNC_INT   = 0x36;
static constexpr uint8_t INT_PIN_CFG = 0x37;
static constexpr uint8_t INT_ENABLE  = 0x38;
static constexpr uint8_t INT_STATUS  = 0x3A;

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
static constexpr uint8_t ACCEL_INTEL_CTRL  = 0x69;
static constexpr uint8_t USER_CTRL         = 0x6A;
static constexpr uint8_t PWR_MGMT_1        = 0x6B;
static constexpr uint8_t PWR_MGMT_2        = 0x6C;

// FIFO Registers
static constexpr uint8_t FIFO_COUNTH = 0x72;
static constexpr uint8_t FIFO_COUNTL = 0x73;
static constexpr uint8_t FIFO_R_W    = 0x74;

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

static constexpr Config       config;
static constexpr GyroConfig   gyro_config;
static constexpr AccelConfig  accel_config;
static constexpr AccelConfig2 accel_config2;
static constexpr LpModeCfg    lp_mode_cfg;
static constexpr FifoEn       fifo_en;
static constexpr IntPinCfg    int_pin_cfg;
static constexpr IntEnable    int_enable;
static constexpr UserCtrl     user_ctrl;
static constexpr PwrMgmt1     pwr_mgmt1;
static constexpr PwrMgmt2     pwr_mgmt2;

inline constexpr uint8_t WRITE_REG(uint8_t reg_addr)
{
    return static_cast<uint8_t>(reg_addr & (~0x80));
}

inline constexpr uint8_t READ_REG(uint8_t reg_addr)
{
    return static_cast<uint8_t>(reg_addr | 0x80U);
}

Imu::Imu(const SpiDevice &in_imu_spi_handle)
  : imu_spi_handle(in_imu_spi_handle){ config.G_ }

    ExitCode Imu::getAccelX(float &accel_x)
{
    std::array<const uint8_t, 1> tx = { READ_REG(ACCEL_XOUT_H) };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    int16_t raw_data = static_cast<int16_t>((rx[0] << 8U) | rx[1]);
    accel_x          = translateAccelData(raw_data);

    return exit;
}

ExitCode Imu::getAccelY(float &accel_y)
{
    std::array<const uint8_t, 1> tx = { READ_REG(ACCEL_YOUT_H) };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    int16_t raw_data = static_cast<int16_t>((rx[0] << 8U) | rx[1]);
    accel_y          = translateAccelData(raw_data);

    return exit;
}

ExitCode Imu::getAccelZ(float &accel_z)
{
    std::array<const uint8_t, 1> tx = { READ_REG(ACCEL_ZOUT_H) };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    int16_t raw_data = static_cast<int16_t>((rx[0] << 8U) | rx[1]);
    accel_z          = translateAccelData(raw_data);

    return exit;
}

ExitCode Imu::getGyroX(float &gyro_x)
{
    std::array<const uint8_t, 1> tx = { READ_REG(GYRO_XOUT_H) };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    int16_t raw_data = static_cast<int16_t>((rx[0] << 8U) | rx[1]);
    gyro_x           = translateAccelData(raw_data);

    return exit;
}

ExitCode Imu::getGyroY(float &gyro_y)
{
    std::array<const uint8_t, 1> tx = { READ_REG(GYRO_YOUT_H) };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    int16_t raw_data = static_cast<int16_t>((rx[0] << 8U) | rx[1]);
    gyro_y           = translateAccelData(raw_data);

    return exit;
}

ExitCode Imu::getGyroZ(float &gyro_z)
{
    std::array<const uint8_t, 1> tx = { READ_REG(GYRO_ZOUT_H) };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    int16_t raw_data = static_cast<int16_t>((rx[0] << 8U) | rx[1]);
    gyro_z           = translateAccelData(raw_data);

    return exit;
}

ExitCode Imu::getTemp(float &temp) {}

ExitCode Imu::getAccelAll(AccelData &data)
{
    std::array<const uint8_t, 1> tx = { READ_REG(ACCEL_XOUT_H) };
    std::array<uint8_t, 6>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    data.x = translateAccelData(static_cast<int16_t>((rx[0] << 8U) | rx[1]));
    data.y = translateAccelData(static_cast<int16_t>((rx[2] << 8U) | rx[3]));
    data.z = translateAccelData(static_cast<int16_t>((rx[4] << 8U) | rx[5]));

    return exit;
}

ExitCode Imu::getGyroAll(GyroData &data)
{
    std::array<const uint8_t, 1> tx = { READ_REG(GYRO_XOUT_H) };
    std::array<uint8_t, 6>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    data.x = translateAccelData(static_cast<int16_t>((rx[0] << 8U) | rx[1]));
    data.y = translateAccelData(static_cast<int16_t>((rx[2] << 8U) | rx[3]));
    data.z = translateAccelData(static_cast<int16_t>((rx[4] << 8U) | rx[5]));

    return exit;
}

} // namespace io::imu