#include <string.h>
#include "io_powerMonitoring.h"
#include "app_utils.h"
#include "app_canTx.h"
#include "hw_i2cs.h"

/*********************** DEFINES *************************/
// Register addresses
#define REG_REFRESH 0x00
#define REG_VBUS 0x07
#define REG_VSENSE 0x0B
#define REG_POWER 0x17
#define REG_ALERT_STATUS 0x26
#define REG_CTRL 0x01         // Control (SAMPLE_MODE, CHANNEL_N_OFF, ALERT pins)
#define REG_NEG_PWR_FSR 0x1D  // VSENSE/VBUS FSR configuration
#define REG_ACCUM_CONFIG 0x25 // Enable accumulation of VBUS & VSENSE
#define REG_ALERT_ENABLE 0x49 // Enable alrts
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
#define VSENSE_LSB (1.5259e-6f)
#define POWER_LSB (VBUS_LSB * VSENSE_LSB)

/*********************** FUNCTION DECLARITION *************************/
static bool read_register(uint8_t reg, uint8_t *data, uint16_t len);
static bool write_register(uint8_t reg, const uint8_t *data, uint16_t len);

/*********************** FUNCTION DEFINITION *************************/
static bool read_register(uint8_t reg, uint8_t *data, uint16_t len)
{
    if (hw_i2c_transmit(&pwr_mtr, &reg, 1) != EXIT_CODE_OK)
        return false;

    return (hw_i2c_receive(&pwr_mtr, data, len) == EXIT_CODE_OK);
}

static bool write_register(uint8_t reg, const uint8_t *data, uint16_t len)
{
    uint8_t buf[1 + len];
    buf[0] = reg;
    memcpy(&buf[1], data, len);
    return (hw_i2c_transmit(&pwr_mtr, buf, len + 1) == EXIT_CODE_OK);
}

bool io_powerMonitoring_init(void)
{
    // 1) Check if peripheral is ready
    if (hw_i2c_isTargetReady(&pwr_mtr) != EXIT_CODE_OK)
    {
        return false;
    }

    // 2) Config: CTRL: 1024 SPS continuous, CH1 and 2 enabled, slow ALERT1 enabled.
    const uint16_t ctrl_val      = 0x4730;
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

    // 5) Over current limits
    uint16_t oc1         = (uint16_t)(CH1_OC / VSENSE_LSB);
    uint16_t oc2         = (uint16_t)(CH2_OC / VSENSE_LSB);
    uint8_t  oc_bytes[2] = { (uint8_t)(oc1 >> 8), (uint8_t)(oc1 & 0xFF) };
    write_register(REG_OC1_LIMIT, oc_bytes, 2);
    oc_bytes[0] = (uint8_t)(oc2 >> 8);
    oc_bytes[1] = (uint8_t)(oc2 & 0xFF);
    write_register(REG_OC1_LIMIT + 2, oc_bytes, 2);

    // 6) Under current limits
    uint16_t uc1         = (uint16_t)(CH1_UC / VSENSE_LSB);
    uint16_t uc2         = (uint16_t)(CH2_UC / VSENSE_LSB);
    uint8_t  uc_bytes[2] = { (uint8_t)(uc1 >> 8), (uint8_t)(uc1 & 0xFF) };
    write_register(REG_UC1_LIMIT, uc_bytes, 2);
    uc_bytes[0] = (uint8_t)(uc2 >> 8);
    uc_bytes[1] = (uint8_t)(uc2 & 0xFF);
    write_register(REG_UC1_LIMIT + 2, uc_bytes, 2);

    // 7) Over voltage limits
    uint16_t ov1         = (uint16_t)(CH1_OV / VBUS_LSB);
    uint16_t ov2         = (uint16_t)(CH2_OV / VBUS_LSB);
    uint8_t  ov_bytes[2] = { (uint8_t)(ov1 >> 8), (uint8_t)(ov1 & 0xFF) };
    write_register(REG_OV1_LIMIT, ov_bytes, 2);
    ov_bytes[0] = (uint8_t)(ov2 >> 8);
    ov_bytes[1] = (uint8_t)(ov2 & 0xFF);
    write_register(REG_OV1_LIMIT + 2, ov_bytes, 2);

    // 8) Under voltage limits
    uint16_t uv1         = (uint16_t)(CH1_UV / VBUS_LSB);
    uint16_t uv2         = (uint16_t)(CH2_UV / VBUS_LSB);
    uint8_t  uv_bytes[2] = { (uint8_t)(uv1 >> 8), (uint8_t)(uv1 & 0xFF) };
    write_register(REG_UV1_LIMIT, uv_bytes, 2);
    uv_bytes[0] = (uint8_t)(uv2 >> 8);
    uv_bytes[1] = (uint8_t)(uv2 & 0xFF);
    write_register(REG_UV1_LIMIT + 2, uv_bytes, 2);

    // 9) Enable alerts for ch1/2
    uint8_t alert_en[3] = { 0 };
    alert_en[0] |= ((1 << 7) | (1 << 6) | (1 << 3) | (1 << 2)); // OC1 | OC2 | UC1 | UC2
    alert_en[1] |= ((1 << 7) | (1 << 6) | (1 << 3) | (1 << 2)); // OV1 | OV2 | UV1 | UV2
    alert_en[2] = 0;                                            // to disable ch3 & ch4 alerts.

    if (!write_register(REG_ALERT_ENABLE, alert_en, 3))
    {
        return false;
    }

    return true;
}

bool io_powerMonitoring_read_voltage(uint8_t ch, float *voltage)
{
    uint8_t buf[2];
    uint8_t reg = (uint8_t)(REG_VBUS + ((ch - 1) * 2));
    if (!read_register(reg, buf, 2))
    {
        return false;
    }

    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    *voltage     = raw * VSENSE_LSB;
    return true;
}

bool io_powerMonitoring_read_current(uint8_t ch, float *current)
{
    uint8_t buf[2];
    uint8_t reg = (uint8_t)(REG_VSENSE + ((ch - 1) * 2));
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
    uint8_t buf[2];
    uint8_t reg = (uint8_t)(REG_POWER + ((ch - 1) * 2));

    if (!read_register(reg, buf, 2))
    {
        return false;
    }

    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    *power       = raw * POWER_LSB;
    return true;
}

bool io_powerMonitoring_read_alerts(uint32_t *status)
{
    uint8_t buf[3];
    uint8_t reg = REG_ALERT_STATUS;
    if (!read_register(reg, buf, 3))
    {
        return false;
    }

    *status = ((uint32_t)buf[0] << 16) | ((uint32_t)buf[1] << 8) | buf[2];
    return true;
}
