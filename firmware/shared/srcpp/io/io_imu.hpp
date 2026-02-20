#pragma once

#include "util_errorCodes.hpp"
#ifdef TARGET_EMBEDDED
#include "hw_spi.hpp"
#else
#include <cstdint>
#endif
/**
 * IMU Datasheet: https://invensense.tdk.com/wp-content/uploads/2021/11/DS-000409-IAM-20680HP-v1.2-Typ.pdf
 */

/**
 * Default Behaviour:
 *
 * Accel and Gyro Filtering Disabled
 * Accel ODR = 4kHz
 * Gyro ODR = 32kHz
 *
 * FIFO Disabled
 * FSYNC Disabled
 * Low Power Mode Disabled
 * WOM Disabled
 *
 * Interrupts Push Pull and Latching until Read
 * Data Ready Interrupt Enabled
 * Accel Scale +-4G
 * Gyro Scale +-250dps
 */

enum class GyroScale : uint8_t
{
    GYRO_DPS_250 = 0x0U,
    GYRO_DPS_500,
    GYRO_DPS_1000,
    GYRO_DPS_1250,
};

enum class AccelScale : uint8_t
{
    ACCEL_G_2 = 0x0U,
    ACCEL_G_4,
    ACCEL_G_8,
    ACCEL_G_16,
};

/**
 * Go to the IMU datasheet for more precise frequencies
 * Page 35 for GYRO, Page 36 for ACCEL
 *
 * 1st frequency represents Cuttoff Frequency
 * 2nd Frequency represents Noise frequencies passed through (realistic)
 */
enum class GyroDlpfConfig : uint8_t
{
    BW_250HZ_NOISE_306HZ = 0x0U, // Internal Base Rate = 8kHz
    BW_176HZ_NOISE_177HZ,        // Internal Base Rate = 1kHz
    BW_92HZ_NOISE_108HZ,         // Internal Base Rate = 1kHz
    BW_41HZ_NOISE_59HZ,          // Internal Base Rate = 1kHz
    BW_20HZ_NOISE_31HZ,          // Internal Base Rate = 1kHz
    BW_10HZ_NOISE_16HZ,          // Internal Base Rate = 1kHz
    BW_5HZ_NOISE_8HZ,            // Internal Base Rate = 1kHz
    BW_3KHZ_NOISE_3KHZ,          // Internal Base Rate = 8kHz
};

enum class AccelDlpfConfig : uint8_t
{
    BW_218HZ_NOISE_235HZ = 0x1U, // Internal Base Rate = 1kHz
    BW_99HZ_NOISE_121HZ,         // Internal Base Rate = 1kHz
    BW_44HZ_NOISE_62HZ,          // Internal Base Rate = 1kHz
    BW_21HZ_NOISE_31HZ,          // Internal Base Rate = 1kHz
    BW_10HZ_NOISE_16HZ,          // Internal Base Rate = 1kHz
    BW_5HZ_NOISE_8HZ,            // Internal Base Rate = 1kHz
    BW_420HZ_NOISE_442HZ,        // Internal Base Rate = 1kHz
};

/**
 * IMU Filter Configuration
 *
 * Enable/Disable accelerometer and gyroscope low pass filters
 * Adjust cutoff frequency of their respectie low pass filters
 *
 * Adjust ODR via Sample Rate Divider (Output Data Rate of IMU), where:
 *
 * ODR = Internal Base Rate / (1 + Sample Rate Divider)
 * Look at Internal Base Rates for the Accelerometer and Gryoscope depending
 * on cutoff frequency above
 *
 * Note: If Fifo is enabled, Accel and Gyro ODR should be the same
 * to prevent data duplication in the Fifo
 *
 * Default Behaviour:
 * Accel and Gyro DLPF disabled
 * Cutoff frequencies specified below if enabled
 * if enabled: Accel and Gyro ODR = 100Hz
 * if disabled: Accel ODR = 4kHz | Gyro ODR = 32kHz
 */
struct ImuFilterConfig
{
    bool            enable_accel_dlpf = false;
    bool            enable_gyro_dlpf  = false;
    AccelDlpfConfig accel_dlpf_cutoff = AccelDlpfConfig::BW_44HZ_NOISE_62HZ;
    GyroDlpfConfig  gyro_dlpf_cutoff  = GyroDlpfConfig::BW_41HZ_NOISE_59HZ;
    uint8_t         sample_rate_div   = 9;

    constexpr uint16_t calculateOdr(const uint16_t base_rate) const
    {
        return static_cast<uint16_t>(base_rate / (1U + sample_rate_div));
    }

    constexpr uint16_t getAccelOdrHz() const { return (enable_accel_dlpf == true) ? calculateOdr(1000U) : 4000U; }

    constexpr uint16_t getGyroOdrHz() const
    {
        if (enable_gyro_dlpf == false)
            return 32000U;

        return (gyro_dlpf_cutoff == GyroDlpfConfig::BW_250HZ_NOISE_306HZ) ? calculateOdr(8000U) : calculateOdr(1000U);
    }
};

/**
 * TODO: ADD Fifo after research
 * Considering polling 3 IMUs secquentially:
 *
 * So far it seems like 2000Hz ODR produces optimal phase delay (~1.47ms)
 * Number of bytes to poll in total = 720 bytes
 * RAM Buffer size 240 Bytes < MAX SPI BUFFER SIZE
 */
enum class FifoSize : uint8_t
{
    SIZE_512B = 0x0U,
    SIZE_1KB,
    SIZE_2KB,
    SIZE_4KB,
};

struct ImuFifoConfig
{
    // enable fifos
    bool enable_accel_fifo  = false;
    bool enable_gyro_x_fifo = false;
    bool enable_gyro_y_fifo = false;
    bool enable_gyro_z_fifo = false;
    bool enable_temp_fifo   = false;
    // fifo configuration
    bool     fifo_mode                = false;               // false=Overwrite old data, true=Stop when full
    FifoSize fifo_size                = FifoSize::SIZE_512B; // Specifies FIFO Size
    bool     fifo_overflow_int_enable = false;

    constexpr bool enableFifo() const
    {
        return enable_accel_fifo || enable_gyro_x_fifo || enable_gyro_y_fifo || enable_gyro_z_fifo || enable_temp_fifo;
    }

    constexpr size_t getFifoPacketSize() const
    {
        return (enable_accel_fifo ? 6 : 0) + (enable_temp_fifo ? 2 : 0) + (enable_gyro_x_fifo ? 2 : 0) +
               (enable_gyro_y_fifo ? 2 : 0) + (enable_gyro_z_fifo ? 2 : 0);
    }
};

namespace io::imu
{
class Imu
{
  public:
    struct AccelData
    {
        float x, y, z;
    };
    struct GyroData
    {
        float x, y, z;
    };

  private:
#ifdef TARGET_EMBEDDED
    const hw::spi::SpiDevice &imu_spi_handle;
#endif

    ImuFilterConfig filter_config;
    // ImuFifoConfig   fifo_config;
    mutable bool is_imu_ready = false;

    // ExitCode getFifoCount(uint16_t &fifo_count);

  public:
#ifdef TARGET_EMBEDDED
    constexpr explicit Imu(
        const hw::spi::SpiDevice &in_imu_spi_handle,
        const ImuFilterConfig    &in_filter_config = ImuFilterConfig{})
      : imu_spi_handle(in_imu_spi_handle), filter_config(in_filter_config)
    {
    }
#elif TARGET_TEST
    constexpr explicit Imu() {}
#endif

    [[nodiscard]] std::expected<void, ErrorCode> init() const;

    std::expected<float, ErrorCode> getAccelX() const;
    std::expected<float, ErrorCode> getAccelY() const;
    std::expected<float, ErrorCode> getAccelZ() const;

    std::expected<float, ErrorCode> getGyroX() const;
    std::expected<float, ErrorCode> getGyroY() const;
    std::expected<float, ErrorCode> getGyroZ() const;

    std::expected<float, ErrorCode> getTemp() const;

    std::expected<AccelData, ErrorCode> getAccelAll() const;
    std::expected<GyroData, ErrorCode>  getGyroAll() const;

#ifdef TARGET_TEST
    mutable bool  initialized   = false;
    mutable float _accel_x_fake = 0.0f, _accel_y_fake = 0.0f, _accel_z_fake = 0.0f, _gyro_x_fake = 0.0f,
                  _gyro_y_fake = 0.0f, _gyro_z_fake = 0.0f;

    void reset_init();
    bool get_init() const;
    void set_AccelX(float accel_x_fake) const;
    void set_AccelY(float accel_y_fake) const;
    void set_AccelZ(float accel_z_fake) const;
    void set_GyroRoll(float gyro_x_fake) const;
    void set_GyroPitch(float gyro_y_fake) const;
    void set_GyroYaw(float gyro_z_fake) const;
#endif
};
} // namespace io::imu
