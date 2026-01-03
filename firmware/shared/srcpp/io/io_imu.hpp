#pragma once

#include "hw_spi.hpp"
#include "hw_spis.hpp"

#include "util_errorCodes.hpp"

/**
 * Default Behaviour:
 * 
 * Digital Filtering Disabled
 * FIFO Disabled
 * FSYNC Disabled
 * Low Power Mode Disabled
 * WOM Disabled
 * 
 * Interrupts Push Pull and Latching until Read
 * Data Ready Interrupt Enabled
 * Accel Scale +-4G
 * Gyro Scale +-500dps
 */

namespace io::imu
{
enum : uint8_t
{
    GYRO_DPS_250 = 0x0U,
    GYRO_DPS_500,
    GYRO_DPS_1000,
    GYRO_DPS_1250,
} GyroScale;

enum : uint8_t
{
    ACCEL_G_2 = 0x0U,
    ACCEL_G_4,
    ACCEL_G_8,
    ACCEL_G_16,
} AccelScale;

// Register 25: SMPLRT_DIV
// Divides the internal sample rate (see CONFIG) to generate the sample
// rate that controls sensor data output rate, FIFO sample rate.
// SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
typedef struct
{
    uint8_t SMPLRT_DIV;
} SampleRateDiv;

// Register 26: CONFIG
typedef struct __attribute__((packed))
{
    uint8_t G_DLPF_CFG : 3   = 0; // [2:0] Gyroscope Low Pass Filter setting
    uint8_t EXT_SYNC_SET : 3 = 0; // [5:3] Frame Sync bit location in Temp data (0=disabled)
    uint8_t FIFO_MODE : 1    = 1; // [6] 0=Overwrite old data, 1=Stop when full
    uint8_t RSVD : 1         = 0; // [7] Reserved
} Config;

// Register 27: GYRO_CONFIG
typedef struct __attribute__((packed))
{
    uint8_t   FCHOICE_B : 2 = 1;            // [1:0] Used to bypass DLPF (0=Enable DLPF, 1-3=Bypass)
    uint8_t   RSVD : 1      = 0;            // [2] Reserved
    GyroScale FS_SEL : 2    = GYRO_DPS_500; // [4:3] Full Scale Range (0=250, 1=500, 2=1000, 3=2000 dps)
    uint8_t   ZG_ST : 1     = 0;            // [5] Z Gyro Self-Test
    uint8_t   YG_ST : 1     = 0;            // [6] Y Gyro Self-Test
    uint8_t   XG_ST : 1     = 0;            // [7] X Gyro Self-Test
} GyroConfig;

// Register 28: ACCEL_CONFIG
typedef struct __attribute__((packed))
{
    uint8_t    RSVD : 3   = 0;         // [2:0] Reserved
    AccelScale FS_SEL : 2 = ACCEL_G_4; // [4:3] Full Scale Range (0=2g, 1=4g, 2=8g, 3=16g)
    uint8_t    ZA_ST : 1  = 0;         // [5] Z Accel Self-Test
    uint8_t    YA_ST : 1  = 0;         // [6] Y Accel Self-Test
    uint8_t    XA_ST : 1  = 0;         // [7] X Accel Self-Test
} AccelConfig;

// Register 29: ACCEL_CONFIG2
typedef struct __attribute__((packed))
{
    uint8_t A_DLPF_CFG : 3 = 0; // [2:0] Accelerometer Low Pass Filter setting
    uint8_t FCHOICE_B : 1  = 1; // [3] 0=Enable DLPF, 1=Bypass DLPF
    uint8_t DEC2_CFG : 2   = 0; // [5:4] Averaging filter for Low Power
    uint8_t FIFO_SIZE : 2  = 0; // [7:6] Specifies FIFO Size (0=512 byte, 1=1 kbyte, 2=2 kbyte, 3=4 kbyte)
} AccelConfig2;

// Register 30: LP_MODE_CFG (Optional for Low Power)
typedef struct __attribute__((packed))
{
    uint8_t A_WOM_ODR_CTRL : 4 = 11; // [3:0] Accel Wake on Motion config
    uint8_t G_AVG_CFG : 3      = 0;  // [6:4] Averaging filter config for gyro
    uint8_t GYRO_CYCLE : 3     = 0;  // [7] Enable Gyro Low Power
} LpModeCfg;

// Register 35: FIFO_EN
typedef struct __attribute__((packed))
{
    uint8_t RSVD : 3          = 0; // [2:0]
    uint8_t GYRO_FIFO_EN : 1  = 0; // [3] Write Gyro data to FIFO
    uint8_t XG_FIFO_EN : 1    = 0; // [3]
    uint8_t YG_FIFO_EN : 1    = 0; // [4]
    uint8_t ZG_FIFO_EN : 1    = 0; // [5]
    uint8_t ACCEL_FIFO_EN : 1 = 0; // [6] Write Accel data to FIFO
} FifoEn;

// Register 55: INT_PIN_CFG
typedef struct __attribute__((packed))
{
    uint8_t RSVD : 2         = 0; // [3:0]
    uint8_t FSYNC_EN : 1     = 0; // [4] 1=enable, 0=diable fsync
    uint8_t FSYNC_ACTL : 1   = 0; // [5] 1=Active Low, 0=Active High
    uint8_t INT_RD_CLEAR : 1 = 1; // [4] 1=Clear on any read, 0=Clear on Status Read
    uint8_t LATCH_INT_EN : 1 = 1; // [5] 1=Latch until clear, 0=50us pulse
    uint8_t OPEN : 1         = 0; // [6] 1=Open Drain, 0=Push-Pull
    uint8_t INT_ACTL : 1     = 0; // [7] 1=Active Low, 0=Active High
} IntPinCfg;

// Register 56: INT_ENABLE
typedef struct __attribute__((packed))
{
    uint8_t DATA_RDY_INT_EN : 1   = 1; // [0] Data Ready Interrupt
    uint8_t RSVD : 1              = 0; // [1]
    uint8_t GDRIVE_INT_EN : 1     = 0; // [3] Gyroscope drive system ready interrupt
    uint8_t RSVD : 1              = 0; // [4]
    uint8_t FIFO_OFLOW_INT_EN : 1 = 0; // [5] FIFO Overflow Interrupt
    uint8_t WOM_INT_EN : 2        = 0; // [7:6]
} IntEnable;

// Register 106: USER_CTRL
typedef struct __attribute__((packed))
{
    uint8_t SIG_COND_RST : 1 = 0; // [0] Reset signal paths
    uint8_t RSVD : 1         = 0; // [1]
    uint8_t FIFO_RST : 1     = 0; // [2] Reset FIFO
    uint8_t RSVD2 : 1        = 0; // [3]
    uint8_t I2C_IF_DIS : 1   = 1; // [4] Disable I2C interface (Use for SPI)
    uint8_t RSVD3 : 1        = 0; // [5]
    uint8_t FIFO_EN : 1      = 0; // [6] Enable FIFO buffer operations
    uint8_t RSVD4 : 1        = 0; // [7]
} UserCtrl;

// Register 107: PWR_MGMT_1
typedef struct __attribute__((packed))
{
    uint8_t CLKSEL : 3       = 1; // [2:0] Clock Source (1=Auto/Gyro best)
    uint8_t TEMP_DIS : 1     = 0; // [3] Disable Temperature Sensor
    uint8_t GYRO_STANDBY : 1 = 0; // [4] Gyro in standby (drive on, sense off)
    uint8_t CYCLE : 1        = 0; // [5] Cycle Mode (Wake-on-motion)
    uint8_t SLEEP : 1        = 0; // [6] Low Power Sleep Mode
    uint8_t DEVICE_RESET : 1 = 0; // [7] Reset internal registers
} PwrMgmt1;

// Register 108: PWR_MGMT_2
typedef struct __attribute__((packed))
{
    uint8_t STBY_ZG : 1    = 0; // [0] Standby Z Gyro
    uint8_t STBY_YG : 1    = 0; // [1] Standby Y Gyro
    uint8_t STBY_XG : 1    = 0; // [2] Standby X Gyro
    uint8_t STBY_ZA : 1    = 0; // [3] Standby Z Accel
    uint8_t STBY_YA : 1    = 0; // [4] Standby Y Accel
    uint8_t STBY_XA : 1    = 0; // [5] Standby X Accel
    uint8_t RSVD : 1       = 0; // [6]
    uint8_t FIFO_LP_EN : 1 = 0; // [7] Enable FIFO in low-power mode
} PwrMgmt2;

class Imu
{
  private:
    typedef struct
    {
        float x, y, z;
    } ImuData;

    const hw::spi::SpiDevice &imu_spi_handle;

    inline constexpr float accel_scale;
    inline constexpr float gyro_scale;
    inline constexpr float temp_scale;

    inline constexpr float translateAccelData(int16_t data);
    inline constexpr float translateGyroData(int16_t data);
    inline constexpr float translateTempData(int16_t &TempRawData);

  public:
    using AccelData = ImuData;
    using GyroData  = ImuData;

    constexpr explicit Imu(const hw::spi::SpiDevice &in_imu_spi_handle) : imu_spi_handle(in_imu_spi_handle) {}

    /**
     * @brief Get acceleration in the x acis
     * @param accel_x
     *
     * @return ExitCode OK for success, otherwise fail
     */
    ExitCode getAccelX(float &accel_x);
    ExitCode getAccelY(float &accel_y);
    ExitCode getAccelZ(float &accel_z);

    ExitCode getGyroX(float &gyro_x);
    ExitCode getGyroY(float &gyro_y);
    ExitCode getGyroZ(float &gyro_z);

    ExitCode getTemp(float &temp);

    ExitCode getAccelAll(AccelData &data);
    ExitCode getGyroAll(GyroData &data);
};
} // namespace io::imu