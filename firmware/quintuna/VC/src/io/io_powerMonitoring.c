#include <cmsis_os2.h>
#include <stdint.h>
#include <string.h>
#include "io_powerMonitoring.h"
#include "app_utils.h"
#include "app_canTx.h"
#include "hw_i2c.h"
#include "hw_i2cs.h"
#include "io_log.h"

/*********************** DEFINES *************************/
// Register addresses
#define REG_REFRESH 0x00
#define REG_VBUS 0x07
#define REG_VSENSE 0x0B
#define REG_POWER 0x17
#define REG_CTRL 0x01         // Control (SAMPLE_MODE, CHANNEL_N_OFF, ALERT pins)
#define REG_NEG_PWR_FSR 0x1D  // VSENSE/VBUS FSR configuration
#define REG_ACCUM_CONFIG 0x25 // Enable accumulation of VBUS & VSENSE
#define REG_OC1_LIMIT 0x30    // over current ch1
#define REG_UC1_LIMIT 0x34    // under current ch1
#define REG_OV1_LIMIT 0x3C    // over voltage ch1
#define REG_UV1_LIMIT 0x40    // under voltage ch1

// over/under current/voltage thresholds
#define CH1_OC 0.0f
#define CH2_OC 0.0f
#define CH1_UC 0.0f
#define CH2_UC 0.0f
#define CH1_OV 0.0f
#define CH2_OV 0.0f
#define CH1_UV 0.0f
#define CH2_UV 0.0f

// LSB scaling factors
#define VBUS_LSB (4.8828e-4f)
#define VSENSE_LSB (1.5259e-6f / 0.003f)
#define POWER_LSB (VBUS_LSB * VSENSE_LSB)

/*********************** FUNCTION DECLARITION *************************/
static bool read_register(uint8_t reg, uint8_t *data, uint16_t len);
static bool write_register(uint8_t reg, const uint8_t *data, uint16_t len);

/*********************** FUNCTION DEFINITION *************************/
static bool read_register(uint8_t reg, uint8_t *data, uint16_t len)
{
    return (hw_i2c_memoryRead(&pwr_mtr, reg, data, len) == EXIT_CODE_OK);
}

static bool write_register(uint8_t reg, const uint8_t *data, uint16_t len)
{
    uint8_t buf[1 + len];
    buf[0] = reg;
    memcpy(&buf[1], data, len);
    return (hw_i2c_memoryWrite(&pwr_mtr, reg, buf, len + 1) == EXIT_CODE_OK);
}

void io_powerMonitoring_refresh(void)
{
    uint8_t cmd = 0x00;
    hw_i2c_transmit(&pwr_mtr, &cmd, 1);
    osDelay(1);
}

bool io_powerMonitoring_init(void)
{
    // 1) Check if peripheral is ready
    if (hw_i2c_isTargetReady(&pwr_mtr) != EXIT_CODE_OK)
    {
        return false;
    }

    // 2) Config: CTRL: 1024 SPS continuous, CH1 and 2 enabled, slow ALERT1 enabled.
    const uint16_t ctrl_val      = 0x0730;
    uint8_t        ctrl_bytes[2] = { (uint8_t)(ctrl_val >> 8), (uint8_t)(ctrl_val & 0xFF) };
    if (!write_register(REG_CTRL, ctrl_bytes, 2))
    {
        return false;
    }

    // 3) FSR defaults
    uint8_t fsr[2] = { 0, 0 };
    if (!write_register(REG_NEG_PWR_FSR, fsr, 2))
    {
        return false;
    }

    // 4) Enable VBUS & VSENSE
    uint8_t acc_cfg = 0x03;
    if (!write_register(REG_ACCUM_CONFIG, &acc_cfg, 1))
    {
        return false;
    }

    uint8_t cmd = REG_REFRESH;
    if (hw_i2c_transmit(&pwr_mtr, &cmd, 1) != EXIT_CODE_OK)
    {
        return false;
    }

    return true;
}

bool io_powerMonitoring_read_voltage(uint8_t ch, float *voltage)
{
    uint8_t buf[2];
    uint8_t reg = (uint8_t)(REG_VBUS + (ch - 1));
    if (!read_register(reg, buf, 2))
    {
        return false;
    }

    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    *voltage     = raw * VBUS_LSB;
    return true;
}

bool io_powerMonitoring_read_current(uint8_t ch, float *current)
{
    uint8_t buf[2];
    uint8_t reg = (uint8_t)(REG_VSENSE + (ch - 1));
    if (!read_register(reg, buf, 2))
    {
        return false;
    }

    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    *current     = raw * VSENSE_LSB;
    return true;
}

bool io_powerMonitoring_read_power(uint8_t ch, float *power)
{
    uint8_t buf[4];
    uint8_t reg = (uint8_t)(REG_POWER + (ch - 1));

    if (!read_register(reg, buf, 4))
        return false;

    uint32_t raw30 =
        (((uint32_t)buf[0]) << 24) | (((uint32_t)buf[1]) << 16) | (((uint32_t)buf[2]) << 8) | ((uint32_t)buf[3]);

    raw30 &= 0x3FFFFFFFU;

    *power = (float)raw30 * POWER_LSB;
    return true;
}
