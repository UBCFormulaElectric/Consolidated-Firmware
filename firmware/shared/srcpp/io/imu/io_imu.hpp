#pragma once

#include <cstdint>

#include "util_errorCodes.hpp"
#ifdef TARGET_EMBEDDED
#include "hw_spi.hpp"
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

namespace io::imu
{
class Imu
{
  public:
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

        inline constexpr uint16_t calculateOdr(uint16_t base_rate)
        {
            return static_cast<uint16_t>(base_rate / (1U + sample_rate_div));
        }
        
        inline constexpr uint16_t getAccelOdrHz()
        {
            return (enable_accel_dlpf == true) ? calculateOdr(1000U) : 4000U;
        }

        inline constexpr uint16_t getGyroOdrHz()
        {
            if (enable_gyro_dlpf == false)
                return 32000U;

            return (gyro_dlpf_cutoff == GyroDlpfConfig::BW_250HZ_NOISE_306HZ) ? calculateOdr(8000U) : calculateOdr(1000U);
        }
    };

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
        bool enable_accel_fifo = false;
        bool enable_gyro_x_fifo = false;
        bool enable_gyro_y_fifo = false;
        bool enable_gyro_z_fifo = false;
        bool enable_temp_fifo = false;
        // fifo configuration
        bool fifo_mode    = false; // false=Overwrite old data, true=Stop when full
        FifoSize fifo_size = FifoSize::SIZE_512B; // Specifies FIFO Size
        bool     fifo_overflow_int_enable = false; 

        inline constexpr bool enableFifo()
        {
            return enable_accel_fifo || enable_gyro_x_fifo || enable_gyro_y_fifo || 
            enable_gyro_z_fifo || enable_temp_fifo;
        }

        inline constexpr size_t getFifoPacketSize() const
        {
            return (enable_accel_fifo  ? 6 : 0) +
                (enable_temp_fifo   ? 2 : 0) +
                (enable_gyro_x_fifo ? 2 : 0) +
                (enable_gyro_y_fifo ? 2 : 0) +
                (enable_gyro_z_fifo ? 2 : 0);
        }
    };

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
    hw::spi::SpiDevice &imu_spi_handle;
    #endif

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

    ImuFilterConfig filter_config;
    ImuFifoConfig   fifo_config;
    bool            is_imu_ready = false;

    inline constexpr float translateAccelData(uint8_t data_h, uint8_t data_l)
    {
        int16_t raw = static_cast<int16_t>((data_h << 8) | data_l);
        return static_cast<float>(raw / accel_sensitivity);
    }

    inline constexpr float translateGyroData(uint8_t data_h, uint8_t data_l)
    {
        int16_t raw = static_cast<int16_t>((data_h << 8) | data_l);
        return static_cast<float>(raw / gyro_sensitivity);
    }

    inline constexpr float translateTempData(uint8_t data_h, uint8_t data_l)
    {
        int16_t raw = static_cast<int16_t>((data_h << 8) | data_l);
        return static_cast<float>((raw / 326.8f) + 25.0f);
    }

    ExitCode getFifoCount(uint16_t &fifo_count);

  public:
    #ifdef TARGET_EMBEDDED
    constexpr explicit Imu(hw::spi::SpiDevice &in_imu_spi_handle, ImuFilterConfig &in_filter_config, ImuFifoConfig &in_fifo_config)
      : imu_spi_handle(in_imu_spi_handle), filter_config(in_filter_config), fifo_config(in_fifo_config)
    {
    }
    constexpr explicit Imu(hw::spi::SpiDevice &in_imu_spi_handle) : imu_spi_handle(in_imu_spi_handle) {}
    #endif

    constexpr explicit Imu() {}

    [[nodiscard]] ExitCode init();
    /**
     * @brief Get acceleration in the x axis
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

    #ifndef TARGET_EMBEDDED
    bool  initialized = false;
    float accel_x_fake = 0.0f, accel_y_fake = 0.0f, accel_z_fake = 0.0f, gyro_x_fake = 0.0f, gyro_y_fake = 0.0f,
          gyro_z_fake = 0.0f;

    void reset_init();
    bool get_init();
    void set_AccelX(float accel_x_fake);
    void set_AccelY(float accel_y_fake);
    void set_AccelZ(float accel_z_fake);
    void set_GyroRoll(float gyro_x_fake);
    void set_GyroPitch(float gyro_y_fake);
    void set_GyroYaw(float gyro_z_fake);
    #endif
};
} // namespace io::imu
