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
static constexpr uint8_t LP_MODE_CONFIG = 0x1E;
static constexpr uint8_t ACCEL_WOM_THR  = 0x1F;

// FIFO and Interrupt Registers
static constexpr uint8_t FIFO_EN        = 0x23;
static constexpr uint8_t FSYNC_INT      = 0x36;
static constexpr uint8_t INT_PIN_CONFIG = 0x37;
static constexpr uint8_t INT_ENABLE     = 0x38;
static constexpr uint8_t INT_STATUS     = 0x3A;

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
static constexpr uint8_t WHO_AM_I     = 0x75;
static constexpr uint8_t WHO_AM_I_VAL = 0xF8;

// Accelerometer Offset Registers
static constexpr uint8_t XA_OFFSET_H = 0x77;
static constexpr uint8_t XA_OFFSET_L = 0x78;
static constexpr uint8_t YA_OFFSET_H = 0x7A;
static constexpr uint8_t YA_OFFSET_L = 0x7B;
static constexpr uint8_t ZA_OFFSET_H = 0x7D;
static constexpr uint8_t ZA_OFFSET_L = 0x7E;

inline constexpr uint8_t WRITE_IMU_REG(uint8_t reg_addr)
{
    return static_cast<uint8_t>(reg_addr & (~0x80));
}

inline constexpr uint8_t READ_IMU_REG(uint8_t reg_addr)
{
    return static_cast<uint8_t>(reg_addr | 0x80U);
}

inline constexpr float Imu::translateAccelData(uint8_t data_h, uint8_t data_l)
{
    int16_t raw = static_cast<int16_t>((data_h << 8) | data_l);
    return static_cast<uint8_t>(raw / accel_sensitivity);
}

inline constexpr float Imu::translateGyroData(uint8_t data_h, uint8_t data_l)
{
    int16_t raw = static_cast<int16_t>((data_h << 8) | data_l);
    return static_cast<uint8_t>(raw / gyro_sensitivity);
}

inline constexpr float Imu::translateTempData(uint8_t data_h, uint8_t data_l)
{
    int16_t raw = static_cast<int16_t>((data_h << 8) | data_l);
    return static_cast<uint8_t>((raw / 326.8f) + 25.0f);
}

ExitCode Imu::init()
{
    // Check if we are able to communicate to the IMU
    std::array<const uint8_t, 1> tx_check = { { READ_IMU_REG(WHO_AM_I) } };
    std::array<uint8_t, 1>       rx;

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx_check, rx);

    if (rx[0] != WHO_AM_I_VAL || (IS_EXIT_OK(exit) == false))
        return exit;

    // Send configs to IMU
    uint8_t      smplrt_div;
    Config       config;
    GyroConfig   gyro_config;
    AccelConfig  accel_config;
    AccelConfig2 accel_config2;
    LpModeConfig lp_mode_config;
    IntPinConfig int_pin_config;
    FifoEn       fifo_en;
    IntEnable    int_enable;
    UserCtrl     user_ctrl;
    PwrMgmt1     pwr_mgmt1;
    PwrMgmt2     pwr_mgmt2;

    accel_config.FS_SEL = static_cast<uint8_t>(accel_scale);
    gyro_config.FS_SEL  = static_cast<uint8_t>(gyro_scale);
    smplrt_div          = filter_config.accel_dlpf_cutoff;

    if (this->filter_config.enable_accel_dlpf == true)
    {
        accel_config2.FCHOICE_B  = 0;
        accel_config2.A_DLPF_CFG = static_cast<uint8_t>(filter_config.accel_dlpf_cutoff);
    }

    if (this->filter_config.enable_gyro_dlpf == true)
    {
        gyro_config.FCHOICE_B = 0;
        config.G_DLPF_CFG     = static_cast<uint8_t>(filter_config.gyro_dlpf_cutoff);
    }

    std::array<const uint8_t, 24> tx_config = {
        WRITE_IMU_REG(SMPLRT_DIV),     smplrt_div,     WRITE_IMU_REG(CONFIG),         config,
        WRITE_IMU_REG(GYRO_CONFIG),    gyro_config,    WRITE_IMU_REG(ACCEL_CONFIG),   accel_config,
        WRITE_IMU_REG(ACCEL_CONFIG_2), accel_config2,  WRITE_IMU_REG(LP_MODE_CONFIG), lp_mode_config,
        WRITE_IMU_REG(INT_PIN_CONFIG), int_pin_config, WRITE_IMU_REG(FIFO_EN),        fifo_en,
        WRITE_IMU_REG(INT_ENABLE),     int_enable,     WRITE_IMU_REG(USER_CTRL),      user_ctrl,
        WRITE_IMU_REG(PWR_MGMT_1),     pwr_mgmt1,      WRITE_IMU_REG(PWR_MGMT_2),     pwr_mgmt2
    };

    exit               = imu_spi_handle.transmit(tx_config);
    this->is_imu_ready = IS_EXIT_OK(exit);

    return exit;
}

ExitCode Imu::getAccelX(float &accel_x)
{
    if (this->is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(ACCEL_XOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    accel_x = translateAccelData(rx[0], rx[1]);
    ;

    return exit;
}

ExitCode Imu::getAccelY(float &accel_y)
{
    if (this->is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(ACCEL_YOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    accel_y = translateAccelData(rx[0], rx[1]);
    ;

    return exit;
}

ExitCode Imu::getAccelZ(float &accel_z)
{
    if (this->is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(ACCEL_ZOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    accel_z = translateAccelData(rx[0], rx[1]);
    ;

    return exit;
}

ExitCode Imu::getGyroX(float &gyro_x)
{
    if (this->is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_XOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    gyro_x = translateGyroData(rx[0], rx[1]);
    ;

    return exit;
}

ExitCode Imu::getGyroY(float &gyro_y)
{
    if (this->is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_YOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    gyro_y = translateGyroData(rx[0], rx[1]);
    ;

    return exit;
}

ExitCode Imu::getGyroZ(float &gyro_z)
{
    if (this->is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_ZOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    gyro_z = translateGyroData(rx[0], rx[1]);
    ;

    return exit;
}

ExitCode Imu::getTemp(float &temp)
{
    if (this->is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_ZOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    temp = translateTempData(rx[0], rx[1]);
    ;

    return exit;
}

ExitCode Imu::getAccelAll(AccelData &data)
{
    if (this->is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(ACCEL_XOUT_H) } };
    std::array<uint8_t, 6>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    data.x = translateAccelData(rx[0], rx[1]);
    data.y = translateAccelData(rx[2], rx[3]);
    data.z = translateAccelData(rx[4], rx[5]);

    return exit;
}

ExitCode Imu::getGyroAll(GyroData &data)
{
    if (this->is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_XOUT_H) } };
    std::array<uint8_t, 6>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    data.x = translateAccelData(rx[0], rx[1]);
    data.y = translateAccelData(rx[2], rx[3]);
    data.z = translateAccelData(rx[4], rx[5]);

    return exit;
}

} // namespace io::imu
