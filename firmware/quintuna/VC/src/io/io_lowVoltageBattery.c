#include "io_lowVoltageBattery.h"
#include "hw_i2c.h"
#include "hw_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

typedef enum {
    BQ76922_I2C_ADDR           = 0x10,
    REG_SUBCOMMAND_LSB         = 0x3E,
    REG_SUBCOMMAND_MSB         = 0x3F,
    REG_DATA_BUFFER            = 0x40,
    REG_CHECKSUM               = 0x60,
    REG_RESPONSE_LENGTH        = 0x61,
    ALERT_PIN_CONFIG           = 0x56,
    ALARM_ENABLE_REG           = 0x66,
    ALARM_STATUS_REG           = 0x62
} register_address_t;

typedef enum {
    CONFIG_UPDATE_COMMAND      = 0x0090,
    CONFIG_UPDATE_EXIT_COMMAND = 0x0092,
    OTP_COMMAND                = 0x00F4,
    OTP_WR_CHECK               = 0x00A0,
    OTP_WR                     = 0x00A1
} config_cmd_t;

typedef enum {
    ENABLED_PROTECTIONS_A      = 0x9256,
    ENABLED_PROTECTIONS_B      = 0x9262,
    OV_THRESHOLD               = 0x9278,
    UV_THRESHOLD               = 0x9275,
    OV_DELAY                   = 0x92AC,
    UV_DELAY                   = 0x92AE,
    SCD_THRESHOLD              = 0x92C0,
    OTC_THRESHOLD              = 0x929A,
    OCD1_DELAY                 = 0x9283,
    OCD1_THRESHOLD             = 0x9282
} protection_t;

typedef enum {
    ALARM_CLEAR_CMD            = 0x01,
    ALERT_ACTIVE_LOW_BIT       = 5,    // Bit position for active-low configuration
    ALERT_PIN_INTERRUPT_CONFIG = 0x02, // ALERT pin configured as an interrupt output
    ALARM_ENABLE_VALUE         = 0x82  // Enable ADC scan alerts
} alarm_config_t;

typedef enum {
    I2C_TIMEOUT_MS             = 100, // Timeout for I2C operations
    CMD_SUBCOMMAND_SIZE        = 2,   // Two bytes for subcommand
    RESPONSE_LENGTH_SIZE       = 1,   // One byte for response length and checksum
    SOC_RESPONSE_LENGTH        = 6,   // Expected response size for SOC command
    VOLTAGE_RESPONSE_LENGTH    = 2    // Expected response size for voltage command
} comm_settings_t;

typedef enum {
    BYTE_MASK  = 0xFF,  // 8-bit mask for lower byte extraction
    BYTE_SHIFT = 8      // Shift amount for upper byte extraction
} bit_mod_t;

typedef enum {
    SUBCOMMAND_PROCESS_DELAY_MS = 1, // Delay after issuing a subcommand
    POLL_DELAY_MS               = 1  // Delay between polls
} dalay_t;

typedef struct {
    float r_sense;
    float q_full;
    float seconds_per_hour;
    float percentage_factor;
    float adc_calibration_factor;
} HardwareConfig_t;

/* Define the hardware configuration values */
static const HardwareConfig_t HardwareConfig = {
    .r_sense = 3.0f,
    .q_full = 11200.0f,
    .seconds_per_hour = 3600.0f,
    .percentage_factor = 100.0f,
    .adc_calibration_factor = 7.4768f
};

extern I2C_HandleTypeDef hi2c1;

static I2cInterface lvBatMon = { &hi2c1, BQ76922_I2C_ADDR, I2C_TIMEOUT_MS };

osSemaphoreId_t bat_mtr_sem;

/**
 * @brief Sends a subcommand to the BQ76922 and waits until the device’s
 *        subcommand register clears (indicating that the response is ready).
 *
 * @param cmd The 16-bit subcommand to send.
 * @return true if the subcommand was sent and the response is ready; false otherwise.
 */
static bool io_lowVoltageBattery_send_subcommand(uint16_t cmd)
{
    if (!hw_i2c_isTargetReady(&lvBatMon))
    {
        return false;
    }

    uint8_t data[CMD_SUBCOMMAND_SIZE] = { (uint8_t)(cmd & BYTE_MASK), (uint8_t)((cmd >> BYTE_SHIFT) & BYTE_MASK) };

    if (!hw_i2c_memWrite(&lvBatMon, REG_SUBCOMMAND_LSB, data, CMD_SUBCOMMAND_SIZE))
    {
        return false;
    }

    /* Wait for the subcommand to be processed */
    osDelay(SUBCOMMAND_PROCESS_DELAY_MS);

    uint8_t low = 0, high = 0;
    do 
    {
        osDelay(POLL_DELAY_MS);
        low  = hw_i2c_memRead(&lvBatMon, REG_SUBCOMMAND_LSB);
        high = hw_i2c_memRead(&lvBatMon, REG_SUBCOMMAND_MSB);
    } while ((low | (high << BYTE_SHIFT)) == cmd);

    return true;
}

/**
 * @brief Reads the response from the BQ76922 and validates the checksum.
 *
 * @param cmd         The subcommand that was sent.
 * @param expectedLen The expected number of data bytes.
 * @param buffer      Pointer to a buffer (of at least expectedLen bytes) where the data will be stored.
 *
 * @return true if the response was successfully read and the checksum is valid; false otherwise.
 */
static bool io_lowVoltageBattery_read_response(uint16_t cmd, uint8_t expectedLen, uint8_t *buffer)
{
    uint8_t respLen;
    if (!hw_i2c_memRead(&lvBatMon, REG_RESPONSE_LENGTH, &respLen, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    if (respLen != expectedLen)
    {
        return false;
    }

    if (!hw_i2c_memRead(&lvBatMon, REG_DATA_BUFFER, buffer, expectedLen))
    {
        return -1;
    }

    uint8_t checksum;
    if (!hw_i2c_memRead(&lvBatMon, REG_CHECKSUM, &checksum, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    uint8_t calcChecksum = (uint8_t)(cmd & BYTE_MASK) + (uint8_t)((cmd >> BYTE_SHIFT) & BYTE_MASK) + respLen;
    for (uint8_t i = 0; i < expectedLen; i++)
    {
        calcChecksum += buffer[i];
    }
        
    calcChecksum = ~calcChecksum; // Invert bits

    return (calcChecksum == checksum);
}

bool io_lowVoltageBattery_OTP_write(void)
{
    if (!io_lowVoltageBattery_send_subcommand(CONFIG_UPDATE_COMMAND))
    {
        return false;
    }

    if (!io_lowVoltageBattery_send_subcommand(OTP_WR_CHECK))
    {
        return false;
    }

    uint8_t response[2] = { 0 };
    if (!io_lowVoltageBattery_read_response(OTP_WR_CHECK, 2, response)) 
    {
        return false;
    }

    /*Check if OTP can be written to (bit 7 of byte 0 should be set).*/
    if (!(response[0] & 0x80)) 
    {
        return false;
    }

    /**
     * config start
     */

    /* Get the current alert config */
    uint8_t alert_config;
    if (!hw_i2c_memRead(&lvBatMon, ALERT_PIN_CONFIG, &alert_config, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    /* Set the active-low bit while preserving other bits */
    alert_config |= (1 << ALERT_ACTIVE_LOW_BIT);
    if (!hw_i2c_memWrite(&lvBatMon, ALERT_PIN_CONFIG, &alert_config, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    /* Configure ALERT pin as an interrupt output */
    if (!hw_i2c_memWrite(&lvBatMon, ALERT_PIN_CONFIG, (uint8_t[]){ ALERT_PIN_INTERRUPT_CONFIG }, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    /* Enable ADC scan alerts */
    if (!hw_i2c_memWrite(&lvBatMon, ALARM_ENABLE_REG, (uint8_t[]){ ALARM_ENABLE_VALUE }, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    /* Enable OV (Bit 3), UV (Bit 2), OCD1 (Bit 5) and SCD (Bit 7) */
    uint8_t protections_a = 0xAC;
    if (!hw_i2c_memWrite(&lvBatMon, ENABLED_PROTECTIONS_A, &protections_a, 1)) 
    {
        return false;
    }

    /* Enable OTD (Bit 5) */
    uint8_t protections_b = 0x20;
    if (!hw_i2c_memWrite(&lvBatMon, ENABLED_PROTECTIONS_B, &protections_b, 1))
    {
        return false;
    }

    /* Set the OV threshold to ~4200mV */
    uint8_t ov_threshold = 0x53;
    if (hw_i2c_memWrite(&lvBatMon, OV_THRESHOLD, &ov_threshold, 1))
    {
        return false;
    }

    /* Set the UV threshold to ~2500mV */
    uint8_t uv_threshold = 0x31;
    if (!hw_i2c_memWrite(&lvBatMon, UV_THRESHOLD, &uv_threshold, 1))
    {
        return false;
    }

    /* Set OCD1 threshold to 30V (the default delay is acceptable) */
    uint8_t ocd1_threshold = 0x5A;
    if (!hw_i2c_memWrite(&lvBatMon, OCD1_THRESHOLD, &ocd1_threshold, 1)) 
    {
        return false;
    }

    /* Set the OTC threshold to 60 degrees celcius (delay is not applicable for temperature) */
    uint8_t otc_threshold = 0x3C;
    if (!hw_i2c_memWrite(&lvBatMon, OTC_THRESHOLD, &otc_threshold, 1))
    {
        return false;
    }

    /* Set the OV, UV, and OCD1 delay to 100ms */
    uint8_t ov_u_ocd1_delay = 0x1E; // 100ms in register format
    if (!hw_i2c_memWrite(&lvBatMon, OV_DELAY, &ov_u_ocd1_delay, 1)) 
    {
        return false;
    }
    if (!hw_i2c_memWrite(&lvBatMon, UV_DELAY, &ov_u_ocd1_delay, 1)) 
    {
        return false;
    }
    if (!hw_i2c_memWrite(&lvBatMon, OCD1_DELAY, &ov_u_ocd1_delay, 1)) 
    {
        return false;
    }

    /* Set the OV, UV, and OCD1 delay to 100ms */
    uint8_t ov_u_ocd1_delay = 0x1E; // 100ms in register format
    if (!hw_i2c_memWrite(&lvBatMon, OV_DELAY, &ov_u_ocd1_delay, 1)) 
    {
        return false;
    }
    if (!hw_i2c_memWrite(&lvBatMon, UV_DELAY, &ov_u_ocd1_delay, 1)) 
    {
        return false;
    }
    if (!hw_i2c_memWrite(&lvBatMon, OCD1_DELAY, &ov_u_ocd1_delay, 1)) 
    {
        return -1;
    }

    /* To-do: write hysteresis config */

    /* To-do: Validate config */

    /**
     * config end (note that SCD does not need specs)
     */

    if (!io_lowVoltageBattery_send_subcommand(OTP_WR))
    {
        return false;
    } 

    if (!io_lowVoltageBattery_read_response(OTP_WR, 2, response)) {
        return false;
    }

    /*Verify if the write was sucessful (bit 7 of byte 0 should be set).*/
    if (!(response[0] & 0x80)) {
        return false;
    }

    if (!io_lowVoltageBattery_send_subcommand(CONFIG_UPDATE_EXIT_COMMAND))
    {
        return false;
    }
}

/**
 * @brief Initializes the low-voltage battery monitoring system.
 *
 * @return true if initialization was successful; false otherwise.
 */
bool io_lowVoltageBattery_init(void)
{
    bat_mtr_sem = osSemaphoreNew(1, 0, NULL);
    if (bat_mtr_sem == NULL)
    {
        return false;
    }

    return true;
}

typedef enum {
    ACCUMULATED_CHARGE_COMMAND = 0x0076
} charge_cmd_t

/**
 * @brief Gets the battery state-of-charge (SOC) as a percentage.
 *
 * @return SOC percentage on success, or -1.0f on error.
 */
float io_lowVoltageBattery_get_SOC(void)
{
    osSemaphoreAcquire(bat_mtr_sem, osWaitForever);

    if (!send_subcommand(ACCUMULATED_CHARGE_COMMAND))
    {
        return -1.0f;
    }

    uint8_t buffer[SOC_RESPONSE_LENGTH];
    if (!read_response(ACCUMULATED_CHARGE_COMMAND, SOC_RESPONSE_LENGTH, buffer))
    {
        return -1.0f;
    }

    /* Parse the 3-byte charge value (buffer[0]-buffer[2]) */
    uint32_t charge = buffer[0] | (buffer[1] << BYTE_SHIFT) | (buffer[2] << (BYTE_SHIFT * 2));
    float CC_GAIN    = ADC_CALIBRATION_FACTOR / R_SENSE;
    float charge_mAh = (charge * CC_GAIN) / SECONDS_PER_HOUR;

    /* Clear any pending alert */
    uint8_t alarmClear = ALARM_CLEAR_CMD;
    hw_i2c_memWrite(&lvBatMon, ALARM_STATUS_REG, &alarmClear, RESPONSE_LENGTH_SIZE);

    return (charge_mAh / Q_FULL) * PERCENTAGE_FACTOR;
}

typedef enum {
    CELL0_VOLTAGE_COMMAND = 0x1514,
    CELL1_VOLTAGE_COMMAND = 0x1716,
    CELL2_VOLTAGE_COMMAND = 0x1B1A,
    CELL4_VOLTAGE_COMMAND = 0x1D1C,
    STACK_VOLTAGE_COMMAND = 0x3534
} voltage_cmd_t;

/**
 * @brief Gets the battery voltage.
 *
 * @param voltage_cmd The subcommand used to read the voltage.
 *
 * @return The battery voltage on success, or -1 on error.
 */
uint16_t io_lowVoltageBattery_get_voltage(uint16_t voltage_cmd)
{
    osSemaphoreAcquire(bat_mtr_sem, osWaitForever);

    if (!send_subcommand(voltage_cmd))
    {
        return (uint16_t)-1;
    }

    uint8_t buffer[VOLTAGE_RESPONSE_LENGTH];
    if (!read_response(voltage_cmd, VOLTAGE_RESPONSE_LENGTH, buffer))
    {
        return (uint16_t)-1;
    }

    uint16_t voltage = buffer[0] | (buffer[1] << BYTE_SHIFT);
    return voltage;
}

/**
 * @brief Handles releasing the semaphore after an interupt.
 * 
 * @param GPIO_pin from the interupt. 
 */
void io_lowVoltageBattery_completeAlert(uint16_t GPIO_pin)
{
    if(GPIO_pin == BAT_MTR_ALERT_Pin) 
    {
        osSemaphoreRelease(bat_mtr_sem);
    }
}
