#include <array>
#include <cstddef>
#include <bit>

#include "io_imu.hpp"
#include "io_time.hpp"
#include "util_utils.hpp"
#include "util_retry.hpp"
namespace io
{
// Register 25: SMPLRT_DIV
// Divides the internal sample rate (see CONFIG) to generate the sample
// rate that controls sensor data output rate (ODR), FIFO sample rate.
// ODR = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
// Note: For gyro, this only applies when FCHOICE_B=00 and DLPF_CFG is 1..6.
// Set directly in init as it has a single bitfield.

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
inline constexpr uint8_t SELF_TEST_X_GYRO  = 0x00;
inline constexpr uint8_t SELF_TEST_Y_GYRO  = 0x01;
inline constexpr uint8_t SELF_TEST_Z_GYRO  = 0x02;
inline constexpr uint8_t SELF_TEST_X_ACCEL = 0x0D;
inline constexpr uint8_t SELF_TEST_Y_ACCEL = 0x0E;
inline constexpr uint8_t SELF_TEST_Z_ACCEL = 0x0F;

// Gyro Offset Registers
inline constexpr uint8_t XG_OFFS_USRH = 0x13;
inline constexpr uint8_t XG_OFFS_USRL = 0x14;
inline constexpr uint8_t YG_OFFS_USRH = 0x15;
inline constexpr uint8_t YG_OFFS_USRL = 0x16;
inline constexpr uint8_t ZG_OFFS_USRH = 0x17;
inline constexpr uint8_t ZG_OFFS_USRL = 0x18;

// Configuration Registers and offsets
inline constexpr uint8_t SMPLRT_DIV     = 0x19;
inline constexpr uint8_t CONFIG         = 0x1A;
inline constexpr uint8_t GYRO_CONFIG    = 0x1B;
inline constexpr uint8_t ACCEL_CONFIG   = 0x1C;
inline constexpr uint8_t ACCEL_CONFIG_2 = 0x1D;
inline constexpr uint8_t LP_MODE_CONFIG = 0x1E;
inline constexpr uint8_t ACCEL_WOM_THR  = 0x1F;

// FIFO and Interrupt Registers
inline constexpr uint8_t FIFO_EN        = 0x23;
inline constexpr uint8_t FSYNC_INT      = 0x36;
inline constexpr uint8_t INT_PIN_CONFIG = 0x37;
inline constexpr uint8_t INT_ENABLE     = 0x38;
inline constexpr uint8_t INT_STATUS     = 0x3A;

// Accelerometer Output Registers
inline constexpr uint8_t ACCEL_XOUT_H = 0x3B;
inline constexpr uint8_t ACCEL_XOUT_L = 0x3C;
inline constexpr uint8_t ACCEL_YOUT_H = 0x3D;
inline constexpr uint8_t ACCEL_YOUT_L = 0x3E;
inline constexpr uint8_t ACCEL_ZOUT_H = 0x3F;
inline constexpr uint8_t ACCEL_ZOUT_L = 0x40;

// Temperature Output Registers
inline constexpr uint8_t TEMP_OUT_H = 0x41;
inline constexpr uint8_t TEMP_OUT_L = 0x42;

// Gyroscope Output Registers
inline constexpr uint8_t GYRO_XOUT_H = 0x43;
inline constexpr uint8_t GYRO_XOUT_L = 0x44;
inline constexpr uint8_t GYRO_YOUT_H = 0x45;
inline constexpr uint8_t GYRO_YOUT_L = 0x46;
inline constexpr uint8_t GYRO_ZOUT_H = 0x47;
inline constexpr uint8_t GYRO_ZOUT_L = 0x48;

// Signal Path and Control Registers
inline constexpr uint8_t SIGNAL_PATH_RESET = 0x68;
inline constexpr uint8_t ACCEL_INTEL_CTRL  = 0x69;
inline constexpr uint8_t USER_CTRL         = 0x6A;
inline constexpr uint8_t PWR_MGMT_1        = 0x6B;
inline constexpr uint8_t PWR_MGMT_2        = 0x6C;

// FIFO Registers
inline constexpr uint8_t FIFO_COUNTH      = 0x72;
inline constexpr uint8_t FIFO_COUNTH_MASK = 0x1F;
inline constexpr uint8_t FIFO_COUNTL      = 0x73;
inline constexpr uint8_t FIFO_R_W         = 0x74;

// Device Identification
inline constexpr uint8_t WHO_AM_I     = 0x75;
inline constexpr uint8_t WHO_AM_I_VAL = 0xF8;

// Accelerometer Offset Registers
inline constexpr uint8_t XA_OFFSET_H = 0x77;
inline constexpr uint8_t XA_OFFSET_L = 0x78;
inline constexpr uint8_t YA_OFFSET_H = 0x7A;
inline constexpr uint8_t YA_OFFSET_L = 0x7B;
inline constexpr uint8_t ZA_OFFSET_H = 0x7D;
inline constexpr uint8_t ZA_OFFSET_L = 0x7E;

static constexpr uint8_t WRITE_IMU_REG(uint8_t reg_addr)
{
    return static_cast<uint8_t>(reg_addr & (~0x80));
}

static constexpr uint8_t READ_IMU_REG(uint8_t reg_addr)
{
    return static_cast<uint8_t>(reg_addr | 0x80U);
}

// Adjust scaling for all
inline constexpr AccelScale accel_scale = AccelScale::ACCEL_G_4;
inline constexpr GyroScale  gyro_scale  = GyroScale::GYRO_DPS_250;

static constexpr float accel_sensitivity = []
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

static constexpr float gyro_sensitivity = []
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

inline constexpr float TEMP_SCALE = 326.8f;

// Datasheet startup timing limits for power-up, reset polling, and wake-from-sleep register access.
inline constexpr uint32_t POWER_UP_REGISTER_ACCESS_DELAY_MS = 100U;
inline constexpr uint32_t RESET_POLL_DELAY_MS               = 1U;
inline constexpr uint8_t  RESET_POLL_ATTEMPTS               = 100U;
inline constexpr uint32_t WAKE_REGISTER_ACCESS_DELAY_MS     = 5U;

static float translateAccelData(const uint8_t data_h, const uint8_t data_l)
{
    const auto raw = static_cast<int16_t>(data_h << 8 | data_l);
    return static_cast<float>(raw) / accel_sensitivity;
}

static float translateGyroData(const uint8_t data_h, const uint8_t data_l)
{
    const auto raw = static_cast<int16_t>(data_h << 8 | data_l);
    return static_cast<float>(raw) / gyro_sensitivity;
}

static float translateTempData(const uint8_t data_h, const uint8_t data_l)
{
    const auto raw = static_cast<int16_t>(data_h << 8 | data_l);
    return static_cast<float>(raw) / TEMP_SCALE + 25.0f;
}

result<void> imu::init() const
{
    const auto write_reg = [this](const uint8_t reg, const uint8_t value) -> std::expected<void, ErrorCode>
    {
        const std::array<const uint8_t, 2> tx = { { WRITE_IMU_REG(reg), value } };
        return imu_spi_handle.transmit(tx);
    };

    const auto read_reg = [this](const uint8_t reg) -> std::expected<uint8_t, ErrorCode>
    {
        const std::array<const uint8_t, 1> tx = { { READ_IMU_REG(reg) } };
        std::array<uint8_t, 1>             rx{};
        RETURN_IF_ERR_SILENT(imu_spi_handle.transmitThenReceive(tx, rx));
        return rx[0];
    };

    io::time::delay(POWER_UP_REGISTER_ACCESS_DELAY_MS);

    // Soft-reset the IMU; DEVICE_RESET self-clears when the reset is complete.
    RETURN_IF_ERR_SILENT(write_reg(PWR_MGMT_1, std::bit_cast<uint8_t>(PwrMgmt1{ .DEVICE_RESET = 1 })));

    const auto reset_done = util::retry(
        [&] -> std::expected<void, ErrorCode>
        {
            io::time::delay(RESET_POLL_DELAY_MS);
            const auto pwr_mgmt_1 = read_reg(PWR_MGMT_1);
            RETURN_IF_ERR_SILENT(pwr_mgmt_1);

            if (READ_BITS<uint8_t>(pwr_mgmt_1.value(), 7U, 1U) == 0U)
                return {};

            return std::unexpected(ErrorCode::TIMEOUT);
        },
        RESET_POLL_ATTEMPTS);

    RETURN_IF_ERR_SILENT(reset_done);

    // Wake the IMU from sleep and select the gyro clock source.
    RETURN_IF_ERR_SILENT(write_reg(PWR_MGMT_1, std::bit_cast<uint8_t>(PwrMgmt1{})));

    io::time::delay(WAKE_REGISTER_ACCESS_DELAY_MS);

    const auto pwr_mgmt_1 = read_reg(PWR_MGMT_1);
    if (not pwr_mgmt_1.has_value())
        return std::unexpected(pwr_mgmt_1.error());

    if (READ_BITS<uint8_t>(pwr_mgmt_1.value(), 6U, 1U) != 0U)
        return std::unexpected(ErrorCode::ERROR);

    // Send configs to IMU
    uint8_t      smplrt_div;
    Config       config{};
    GyroConfig   gyro_config{};
    AccelConfig  accel_config{};
    AccelConfig2 accel_config2{};
    LpModeConfig lp_mode_config{};
    IntPinConfig int_pin_config{};
    FifoEn       fifo_en{};
    IntEnable    int_enable{};
    PwrMgmt2     pwr_mgmt2{};

    // Disable the I2C interface while using SPI.
    RETURN_IF_ERR(write_reg(USER_CTRL, std::bit_cast<uint8_t>(UserCtrl{})));

    const auto user_ctrl_readback = read_reg(USER_CTRL);
    RETURN_IF_ERR(user_ctrl_readback);

    if (READ_BITS<uint8_t>(user_ctrl_readback.value(), 4U, 1U) != 1U)
        return std::unexpected(ErrorCode::ERROR);

    const auto who_am_i = read_reg(WHO_AM_I);
    RETURN_IF_ERR_SILENT(who_am_i);

    if (who_am_i.value() != WHO_AM_I_VAL)
        return std::unexpected(ErrorCode::ERROR);

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

    std::array<const uint8_t, 20> tx_config = { {
        PWR_MGMT_2,     std::bit_cast<uint8_t>(pwr_mgmt2),      INT_ENABLE,     std::bit_cast<uint8_t>(int_enable),
        SMPLRT_DIV,     std::bit_cast<uint8_t>(smplrt_div),     CONFIG,         std::bit_cast<uint8_t>(config),
        GYRO_CONFIG,    std::bit_cast<uint8_t>(gyro_config),    ACCEL_CONFIG,   std::bit_cast<uint8_t>(accel_config),
        ACCEL_CONFIG_2, std::bit_cast<uint8_t>(accel_config2),  LP_MODE_CONFIG, std::bit_cast<uint8_t>(lp_mode_config),
        INT_PIN_CONFIG, std::bit_cast<uint8_t>(int_pin_config), FIFO_EN,        std::bit_cast<uint8_t>(fifo_en),
    } };

    for (std::size_t i = 0; i < tx_config.size(); i += 2)
        RETURN_IF_ERR_SILENT(write_reg(tx_config[i], tx_config[i + 1]));

    const auto accel_config_readback = read_reg(ACCEL_CONFIG);
    RETURN_IF_ERR_SILENT(accel_config_readback);

    if (accel_config_readback.value() != std::bit_cast<uint8_t>(accel_config))
        return std::unexpected(ErrorCode::ERROR);

    const auto gyro_config_readback = read_reg(GYRO_CONFIG);
    RETURN_IF_ERR_SILENT(gyro_config_readback);

    if (gyro_config_readback.value() != std::bit_cast<uint8_t>(gyro_config))
        return std::unexpected(ErrorCode::ERROR);

    is_imu_ready = true;
    return {};
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

result<float> imu::getAccelX() const
{
    if (is_imu_ready == false)
        return std::unexpected(ErrorCode::ERROR);

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(ACCEL_XOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    const auto exit = imu_spi_handle.transmitThenReceive(tx, rx);
    RETURN_IF_ERR_SILENT(exit);
    return translateAccelData(rx[0], rx[1]);
}

result<float> imu::getAccelY() const
{
    if (is_imu_ready == false)
        return std::unexpected(ErrorCode::ERROR);

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(ACCEL_YOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    const auto exit = imu_spi_handle.transmitThenReceive(tx, rx);
    RETURN_IF_ERR_SILENT(exit);

    return translateAccelData(rx[0], rx[1]);
}

result<float> imu::getAccelZ() const
{
    if (is_imu_ready == false)
        return std::unexpected(ErrorCode::ERROR);

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(ACCEL_ZOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    const auto exit = imu_spi_handle.transmitThenReceive(tx, rx);
    RETURN_IF_ERR_SILENT(exit);

    return translateAccelData(rx[0], rx[1]);
}

result<float> imu::getGyroX() const
{
    if (is_imu_ready == false)
        return std::unexpected(ErrorCode::ERROR);

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_XOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    const auto exit = imu_spi_handle.transmitThenReceive(tx, rx);
    RETURN_IF_ERR_SILENT(exit);

    return translateGyroData(rx[0], rx[1]);
}

result<float> imu::getGyroY() const
{
    if (is_imu_ready == false)
        return std::unexpected(ErrorCode::ERROR);

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_YOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    const auto exit = imu_spi_handle.transmitThenReceive(tx, rx);
    RETURN_IF_ERR_SILENT(exit);

    return translateGyroData(rx[0], rx[1]);
}

result<float> imu::getGyroZ() const
{
    if (is_imu_ready == false)
        return std::unexpected(ErrorCode::ERROR);

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_ZOUT_H) } };
    std::array<uint8_t, 2>       rx{};

    const auto exit = imu_spi_handle.transmitThenReceive(tx, rx);
    RETURN_IF_ERR_SILENT(exit);

    return translateGyroData(rx[0], rx[1]);
}

result<float> imu::getTemp() const
{
    if (is_imu_ready == false)
        return std::unexpected(ErrorCode::ERROR);

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(TEMP_OUT_H) } };
    std::array<uint8_t, 2>       rx{};

    const auto exit = imu_spi_handle.transmitThenReceive(tx, rx);
    RETURN_IF_ERR_SILENT(exit);

    return translateTempData(rx[0], rx[1]);
}

result<imu::AccelData> imu::getAccelAll() const
{
    if (is_imu_ready == false)
        return std::unexpected(ErrorCode::ERROR);

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(ACCEL_XOUT_H) } };
    std::array<uint8_t, 6>       rx{};

    const auto exit = imu_spi_handle.transmitThenReceive(tx, rx);
    RETURN_IF_ERR_SILENT(exit);

    return AccelData{
        translateAccelData(rx[0], rx[1]),
        translateAccelData(rx[2], rx[3]),
        translateAccelData(rx[4], rx[5]),
    };
}

result<imu::GyroData> imu::getGyroAll() const
{
    if (is_imu_ready == false)
        return std::unexpected(ErrorCode::ERROR);

    std::array<const uint8_t, 1> tx = { { READ_IMU_REG(GYRO_XOUT_H) } };
    std::array<uint8_t, 6>       rx{};

    const auto exit = imu_spi_handle.transmitThenReceive(tx, rx);
    RETURN_IF_ERR_SILENT(exit);

    return GyroData{
        translateGyroData(rx[0], rx[1]),
        translateGyroData(rx[2], rx[3]),
        translateGyroData(rx[4], rx[5]),
    };
}

} // namespace io
