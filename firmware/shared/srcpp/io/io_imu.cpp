#include <array>

#include "io_imu.hpp"

namespace io::imu
{
// Register 25: SMPLRT_DIV
// Divides the internal sample rate (see CONFIG) to generate the sample
// rate that controls sensor data output rate (ODR), FIFO sample rate.
// ODR = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
// Note: does not do anything if DLPFs for Accel and Gyro are disabled (disabled via FCHOICE_B in GyroConfig and
// AccelConfig2) Set directly in init as it has a single bitfield

// Register 26: CONFIG
struct __attribute__((packed)) Config
{
    uint8_t G_DLPF_CFG : 3   = 0; // [2:0] Gyroscope Low Pass Filter setting
    uint8_t EXT_SYNC_SET : 3 = 0; // [5:3] Frame Sync bit location in Temp data (0=disabled)
    uint8_t FIFO_MODE : 1    = 0; // [6] 0=Overwrite old data, 1=Stop when full
    uint8_t RSVD : 1         = 0; // [7] Reserved
};

// Register 27: GYRO_CONFIG
struct __attribute__((packed)) GyroConfig
{
    uint8_t FCHOICE_B : 2 = 1; // [1:0] Used to bypass DLPF (0=Enable DLPF, 1-3=Bypass)
    uint8_t RSVD : 1      = 0; // [2] Reserved
    uint8_t FS_SEL : 2    = 0; // [4:3] Full Scale Range (0=250, 1=500, 2=1000, 3=2000 dps)
    uint8_t ZG_ST : 1     = 0; // [5] Z Gyro Self-Test
    uint8_t YG_ST : 1     = 0; // [6] Y Gyro Self-Test
    uint8_t XG_ST : 1     = 0; // [7] X Gyro Self-Test
};

// Register 28: ACCEL_CONFIG
struct __attribute__((packed)) AccelConfig
{
    uint8_t RSVD : 3   = 0; // [2:0] Reserved
    uint8_t FS_SEL : 2 = 1; // [4:3] Full Scale Range (0=2g, 1=4g, 2=8g, 3=16g)
    uint8_t ZA_ST : 1  = 0; // [5] Z Accel Self-Test
    uint8_t YA_ST : 1  = 0; // [6] Y Accel Self-Test
    uint8_t XA_ST : 1  = 0; // [7] X Accel Self-Test
};

// Register 29: ACCEL_CONFIG2
struct __attribute__((packed)) AccelConfig2
{
    uint8_t A_DLPF_CFG : 3 = 0; // [2:0] Accelerometer Low Pass Filter setting
    uint8_t FCHOICE_B : 1  = 1; // [3] 0=Enable DLPF, 1=Bypass DLPF
    uint8_t DEC2_CFG : 2   = 0; // [5:4] Averaging filter for Low Power
    uint8_t FIFO_SIZE : 2  = 0; // [7:6] Specifies FIFO Size (0=512 byte, 1=1 kbyte, 2=2 kbyte, 3=4 kbyte)
};

// Register 30: LP_MODE_CFG (Optional for Low Power)
struct __attribute__((packed)) LpModeConfig
{
    uint8_t A_WOM_ODR_CTRL : 4 = 11; // [3:0] Accel Wake on Motion config
    uint8_t G_AVG_CFG : 3      = 0;  // [6:4] Averaging filter config for gyro
    uint8_t GYRO_CYCLE : 1     = 0;  // [7] Enable Gyro Low Power
};

// Register 35: FIFO_EN
struct __attribute__((packed)) FifoEn
{
    uint8_t RSVD : 3          = 0; // [2:0]
    uint8_t ACCEL_FIFO_EN : 1 = 0; // [3] Write Accel data to FIFO
    uint8_t XG_FIFO_EN : 1    = 0; // [4] Write Gyro X data to FIFO
    uint8_t YG_FIFO_EN : 1    = 0; // [5] Write Gyro Y data to FIFO
    uint8_t ZG_FIFO_EN : 1    = 0; // [6] Write Gyro Z data to FIFO
    uint8_t TEMP_FIFO_EN : 1  = 0; // [7] Write Temp data to FIFO
};

// Register 55: INT_PIN_CFG
struct __attribute__((packed)) IntPinConfig
{
    uint8_t RSVD : 2         = 0; // [3:0]
    uint8_t FSYNC_EN : 1     = 0; // [4] 1=enable, 0=diable fsync
    uint8_t FSYNC_ACTL : 1   = 0; // [5] 1=Active Low, 0=Active High
    uint8_t INT_RD_CLEAR : 1 = 1; // [4] 1=Clear on any read, 0=Clear on Status Read
    uint8_t LATCH_INT_EN : 1 = 1; // [5] 1=Latch until clear, 0=50us pulse
    uint8_t OPEN : 1         = 0; // [6] 1=Open Drain, 0=Push-Pull
    uint8_t INT_ACTL : 1     = 0; // [7] 1=Active Low, 0=Active High
};

// Register 56: INT_ENABLE
struct __attribute__((packed)) IntEnable
{
    uint8_t DATA_RDY_INT_EN : 1   = 1; // [0] Data Ready Interrupt
    uint8_t RSVD : 1              = 0; // [1]
    uint8_t GDRIVE_INT_EN : 1     = 0; // [3] Gyroscope drive system ready interrupt
    uint8_t RSVD2 : 1             = 0; // [4]
    uint8_t FIFO_OFLOW_INT_EN : 1 = 0; // [5] FIFO Overflow Interrupt
    uint8_t WOM_INT_EN : 2        = 0; // [7:6]
};

// Register 106: USER_CTRL
struct __attribute__((packed)) UserCtrl
{
    uint8_t SIG_COND_RST : 1 = 0; // [0] Reset signal paths
    uint8_t RSVD : 1         = 0; // [1]
    uint8_t FIFO_RST : 1     = 0; // [2] Reset FIFO
    uint8_t RSVD2 : 1        = 0; // [3]
    uint8_t I2C_IF_DIS : 1   = 1; // [4] Disable I2C interface (Use for SPI)
    uint8_t RSVD3 : 1        = 0; // [5]
    uint8_t FIFO_EN : 1      = 0; // [6] Enable FIFO buffer operations
    uint8_t RSVD4 : 1        = 0; // [7]
};

// Register 107: PWR_MGMT_1
struct __attribute__((packed)) PwrMgmt1
{
    uint8_t CLKSEL : 3       = 1; // [2:0] Clock Source (1=Auto/Gyro best)
    uint8_t TEMP_DIS : 1     = 0; // [3] Disable Temperature Sensor
    uint8_t GYRO_STANDBY : 1 = 0; // [4] Gyro in standby (drive on, sense off)
    uint8_t CYCLE : 1        = 0; // [5] Cycle Mode (Wake-on-motion)
    uint8_t SLEEP : 1        = 0; // [6] Low Power Sleep Mode
    uint8_t DEVICE_RESET : 1 = 0; // [7] Reset internal registers
};

// Register 108: PWR_MGMT_2
struct __attribute__((packed)) PwrMgmt2
{
    uint8_t STBY_ZG : 1    = 0; // [0] Standby Z Gyro
    uint8_t STBY_YG : 1    = 0; // [1] Standby Y Gyro
    uint8_t STBY_XG : 1    = 0; // [2] Standby X Gyro
    uint8_t STBY_ZA : 1    = 0; // [3] Standby Z Accel
    uint8_t STBY_YA : 1    = 0; // [4] Standby Y Accel
    uint8_t STBY_XA : 1    = 0; // [5] Standby X Accel
    uint8_t RSVD : 1       = 0; // [6]
    uint8_t FIFO_LP_EN : 1 = 0; // [7] Enable FIFO in low-power mode
};

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
static constexpr uint8_t FIFO_COUNTH_MASK = 0x1F;
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

static constexpr uint8_t WRITE_IMU_REG(uint8_t reg_addr)
{
    return static_cast<uint8_t>(reg_addr & (~0x80));
}

static constexpr uint8_t READ_IMU_REG(uint8_t reg_addr)
{
    return static_cast<uint8_t>(reg_addr | 0x80U);
}

// Adjust scaling for all
static constexpr AccelScale accel_scale = AccelScale::ACCEL_G_4;
static constexpr GyroScale  gyro_scale  = GyroScale::GYRO_DPS_250;

static constexpr float accel_sensitivity = []()
{
    switch (accel_scale)
    {
        case AccelScale::ACCEL_G_2:
            return 16384.0f;
        case AccelScale::ACCEL_G_4:
            return 8192.0f;
        case AccelScale::ACCEL_G_8:
            return 4096.0f;
        case AccelScale::ACCEL_G_16:
            return 2048.0f;
        default:
            return 8192.0f;
    }
}();

static constexpr float gyro_sensitivity = []()
{
    switch (gyro_scale)
    {
        case GyroScale::GYRO_DPS_250:
            return 131.0f;
        case GyroScale::GYRO_DPS_500:
            return 65.5f;
        case GyroScale::GYRO_DPS_1000:
            return 32.8f;
        case GyroScale::GYRO_DPS_1250:
            return 16.4f;
        default:
            return 131.0f;
    }
}();

static constexpr float temp_scale = 326.8f;

static float translateAccelData(uint8_t data_h, uint8_t data_l)
{
    int16_t raw = static_cast<int16_t>((data_h << 8) | data_l);
    return static_cast<float>(raw / accel_sensitivity);
}

static float translateGyroData(uint8_t data_h, uint8_t data_l)
{
    int16_t raw = static_cast<int16_t>((data_h << 8) | data_l);
    return static_cast<float>(raw / gyro_sensitivity);
}

static float translateTempData(uint8_t data_h, uint8_t data_l)
{
    int16_t raw = static_cast<int16_t>((data_h << 8) | data_l);
    return static_cast<float>((raw / 326.8f) + 25.0f);
}

ExitCode Imu::init()
{
    // Check if we are able to communicate to the IMU
    std::array<const uint8_t, 1> tx_check = { { READ_IMU_REG(WHO_AM_I) } };
    std::array<uint8_t, 1>       rx;

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx_check, rx);

    if ((IS_EXIT_OK(exit) == false) || rx[0] != WHO_AM_I_VAL)
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

    // Filter Config
    accel_config.FS_SEL = static_cast<uint8_t>(accel_scale);
    gyro_config.FS_SEL  = static_cast<uint8_t>(gyro_scale);
    smplrt_div          = filter_config.sample_rate_div;

    if (filter_config.enable_accel_dlpf == true)
    {
        accel_config2.FCHOICE_B  = 0;
        accel_config2.A_DLPF_CFG = static_cast<uint8_t>(filter_config.accel_dlpf_cutoff) & 0x07;
    }

    if (filter_config.enable_gyro_dlpf == true)
    {
        gyro_config.FCHOICE_B = 0;
        config.G_DLPF_CFG     = static_cast<uint8_t>(filter_config.gyro_dlpf_cutoff) & 0x07;
    }

    // Fifo Config
    // TODO: Add Fifo after research
    // if (fifo_config.enableFifo())
    // {
    //     assert(filter_config.getAccelOdrHz() == filter_config.getGyroOdrHz());
        
    //     // Enable fifos
    //     user_ctrl.FIFO_EN = static_cast<uint8_t>(fifo_config.enableFifo()) & 0x01;
    //     fifo_en.ACCEL_FIFO_EN = static_cast<uint8_t>(fifo_config.enable_accel_fifo) & 0x01;
    //     fifo_en.XG_FIFO_EN = static_cast<uint8_t>(fifo_config.enable_gyro_x_fifo) & 0x01;
    //     fifo_en.YG_FIFO_EN = static_cast<uint8_t>(fifo_config.enable_gyro_y_fifo) & 0x01;
    //     fifo_en.ZG_FIFO_EN = static_cast<uint8_t>(fifo_config.enable_gyro_z_fifo) & 0x01;

    //     // Configure Fifo
    //     config.FIFO_MODE = static_cast<uint8_t>(fifo_config.fifo_mode) & 0x01;
    //     accel_config2.FIFO_SIZE = static_cast<uint8_t>(fifo_config.fifo_size) & 0x03;
    //     int_enable.FIFO_OFLOW_INT_EN = static_cast<uint8_t>(fifo_config.fifo_overflow_int_enable) & 0x01;
    // }

    std::array<const uint8_t, 24> tx_config = {
        { WRITE_IMU_REG(SMPLRT_DIV),     std::bit_cast<uint8_t>(smplrt_div),
          WRITE_IMU_REG(CONFIG),         std::bit_cast<uint8_t>(config),
          WRITE_IMU_REG(GYRO_CONFIG),    std::bit_cast<uint8_t>(gyro_config),
          WRITE_IMU_REG(ACCEL_CONFIG),   std::bit_cast<uint8_t>(accel_config),
          WRITE_IMU_REG(ACCEL_CONFIG_2), std::bit_cast<uint8_t>(accel_config2),
          WRITE_IMU_REG(LP_MODE_CONFIG), std::bit_cast<uint8_t>(lp_mode_config),
          WRITE_IMU_REG(INT_PIN_CONFIG), std::bit_cast<uint8_t>(int_pin_config),
          WRITE_IMU_REG(FIFO_EN),        std::bit_cast<uint8_t>(fifo_en),
          WRITE_IMU_REG(INT_ENABLE),     std::bit_cast<uint8_t>(int_enable),
          WRITE_IMU_REG(USER_CTRL),      std::bit_cast<uint8_t>(user_ctrl),
          WRITE_IMU_REG(PWR_MGMT_1),     std::bit_cast<uint8_t>(pwr_mgmt1),
          WRITE_IMU_REG(PWR_MGMT_2),     std::bit_cast<uint8_t>(pwr_mgmt2) }
    };

    exit               = imu_spi_handle.transmit(tx_config);
    is_imu_ready = IS_EXIT_OK(exit);

    return exit;
}

// ExitCode Imu::getFifoCount(uint16_t &fifo_count)
// {    
//     if (is_imu_ready == false)
//         return ExitCode::EXIT_CODE_ERROR;
    
//     std::array<const uint8_t, 1> tx {{ READ_IMU_REG(FIFO_COUNTH)}};
//     std::array<uint8_t, 2> rx{};

//     ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

//     fifo_count = static_cast<uint16_t>(((rx[0] & FIFO_COUNTH_MASK) << 8) | rx[1]);

//     return exit;
// }

ExitCode Imu::getAccelX(float &accel_x)
{
    if (is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(ACCEL_XOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    accel_x = translateAccelData(rx[0], rx[1]);

    return exit;
}

ExitCode Imu::getAccelY(float &accel_y)
{
    if (is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(ACCEL_YOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    accel_y = translateAccelData(rx[0], rx[1]);

    return exit;
}

ExitCode Imu::getAccelZ(float &accel_z)
{
    if (is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(ACCEL_ZOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    accel_z = translateAccelData(rx[0], rx[1]);

    return exit;
}

ExitCode Imu::getGyroX(float &gyro_x)
{
    if (is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_XOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    gyro_x = translateGyroData(rx[0], rx[1]);

    return exit;
}

ExitCode Imu::getGyroY(float &gyro_y)
{
    if (is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_YOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    gyro_y = translateGyroData(rx[0], rx[1]);

    return exit;
}

ExitCode Imu::getGyroZ(float &gyro_z)
{
    if (is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_ZOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    gyro_z = translateGyroData(rx[0], rx[1]);

    return exit;
}

ExitCode Imu::getTemp(float &temp)
{
    if (is_imu_ready == false)
        return ExitCode::EXIT_CODE_ERROR;

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_ZOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    ExitCode exit = imu_spi_handle.transmitThenReceive(tx, rx);

    temp = translateTempData(rx[0], rx[1]);

    return exit;
}

ExitCode Imu::getAccelAll(AccelData &data)
{
    if (is_imu_ready == false)
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
    if (is_imu_ready == false)
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
