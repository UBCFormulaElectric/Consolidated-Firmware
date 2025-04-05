#include "io_lowVoltageBattery.h"
#include "hw_i2c.h"
#include "hw_i2cs.h"
#include "hw_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "hw_utils.h"
#include "io_log.h"
#include "io_taskMonitor.h"
#include <stdbool.h>
#include <stdint.h>

#define BYTE_MASK(x) ((x) & 0XFF)
#define COMMAND_SIZE 1
#define SUB_COMMAND_SIZE 2
#define STATUS_LENGTH 2


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

typedef struct{
    uint8_t response[REG_RESPONSE_LENGTH];
    uint8_t length;
} Subcommand_Response;

typedef enum {
    CONFIG_UPDATE_COMMAND      = 0x0090,
    CONFIG_UPDATE_EXIT_COMMAND = 0x0092,
    OTP_COMMAND                = 0x00F4,
    OTP_WR_CHECK               = 0x00A0,
    OTP_WR                     = 0x00A1
} config_cmd_t;

typedef enum {
    ALARM_CLEAR_CMD            = 0x01,
    ALERT_ACTIVE_LOW_BIT       = 5,    // Bit position for active-low configuration
    ALERT_PIN_INTERRUPT_CONFIG = 0x02, // ALERT pin configured as an interrupt output
    ALARM_ENABLE_VALUE         = 0x82  // Enable ADC scan alerts
} alarm_config_t;

typedef enum {
    I2C_TIMEOUT_MS             = 100, // Timeout for I2C operations
    CMD_SUBCOMMAND_SIZE        = 2,   // Two bytes for subcommand
    RESPONSE_LENGTH_SIZE       = 4,   // One byte for response length and checksum
    SOC_RESPONSE_LENGTH        = 6,   // Expected response size for SOC command
} comm_settings_t;

typedef struct __attribute__((packed)){
    uint8_t CFGUPDATE :1;
    uint8_t PCHG_MODE :1;
    uint8_t SLEEP_EN :1;
    uint8_t POR: 1;
    uint8_t WD : 1;
    uint8_t COW_CHECK : 1;
    uint8_t OTPW : 1;
    uint8_t OTPB :1;
    uint8_t SEC0 :1;
    uint8_t SEC1 :1;
    uint8_t SS :1;
    uint8_t FUSE :1;
    uint8_t PF : 1;
    uint8_t SDM :1;
    uint8_t RSVD :1;
    uint8_t SLEEP :1;
}Battery_Status;


typedef struct __attribute__((packed)){
    uint8_t LD_ON :1;
    uint8_t LD_TIMEOUT :1;
    uint8_t DEEPSLEEP : 1;
    uint16_t RSVD : 13;
}Control_Status;

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

/**
 * @brief Sends a subcommand to the BQ76922 and waits until the device’s
 *        subcommand register clears (indicating that the response is ready).
 *
 * @param cmd The 16-bit subcommand to send.
 * @return true if the subcommand was sent and the response is ready; false otherwise.
 */
static bool send_subcommand(uint16_t cmd)
{
    if (!hw_i2c_isTargetReady(&bat_mtr))
    {
        return false;
    }

    uint8_t lower_cmd[1] = { (uint8_t)(BYTE_MASK(cmd))};
    if (!hw_i2c_memoryWrite(&bat_mtr, REG_SUBCOMMAND_LSB, lower_cmd,1))
    {
        return false;
    }
    uint8_t upper_cmd[1] = {(uint8_t) BYTE_MASK(cmd >> 8)};
    if (!hw_i2c_memoryWrite(&bat_mtr, REG_SUBCOMMAND_MSB, upper_cmd, 1)) {
        return false;
    }
    bool finished_subcmd = false;
    uint8_t lower_read;
    uint8_t upper_read;

    while (finished_subcmd) {
        if (!hw_i2c_memoryRead(&bat_mtr, REG_SUBCOMMAND_LSB, &lower_read,2))
        {
            return false;
        }
        if (!hw_i2c_memoryRead(&bat_mtr, REG_SUBCOMMAND_MSB, &upper_read, 2)) {
            return false;
        }

        finished_subcmd = ((upper_read<<8|lower_read) == 0xFF) ? false : true;
    }
    //reading the length of the buffer
    return true;
}

static bool recieve_subcommand(uint16_t cmd, Subcommand_Response* response){
    //reading the length of the buffer
    uint8_t response_length;
    if(!hw_i2c_memoryRead(&bat_mtr, REG_RESPONSE_LENGTH, &response_length, 1)){
        return false;
    }
    response->length = response_length; 
    for(uint8_t i = 0x40; i<response_length && i!= 0x5F;i++){
        uint8_t index = (i-(uint8_t)0x40);
        if (!hw_i2c_memoryRead(&bat_mtr, i, &response->response[index], 1)){
            return false;
        }
    }
    //ask for the checksum
    uint8_t calcChecksum = (uint8_t)(((uint8_t)(BYTE_MASK(cmd))) +
            ((uint8_t)((BYTE_MASK(cmd>>8))) +
            ((uint8_t)response_length)));

    uint8_t calc_inversion = ~calcChecksum;
    uint8_t checksum;
    if (!hw_i2c_memoryRead(&bat_mtr, REG_CHECKSUM, &checksum, 1)){
        return false;
    }

    if(calc_inversion != checksum){
        return false;
    }
    return true;
}

bool io_lowVoltageBattery_initial_setup(){
    BREAK_IF_DEBUGGER_CONNECTED();

    //checking if the target is even ready to communicate with 
    //adding debuggers to make sure the code doesnt run wild and free when we run
    if (!hw_i2c_isTargetReady(&bat_mtr)){
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }
    uint8_t buffer_bat[1] = {(uint8_t)BATTERY_STATUS};
    //ask for battery status to check if the device is sleep or not
    if (!hw_i2c_transmit(&bat_mtr, buffer_bat, 1)){
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }

    Battery_Status bat_status;
    if(!hw_i2c_receive(&bat_mtr, (uint8_t*)&bat_status , 2)){
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }
    //ask for control status
    uint8_t buffer_control[1] = {(uint8_t) CONTROL_STATUS};
    if (!hw_i2c_transmit(&bat_mtr, buffer_control, 1)){
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }

    Control_Status ctrl_status;
    if(!hw_i2c_receive(&bat_mtr, (uint8_t*)&ctrl_status, 2)){
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }
    if (bat_status.SLEEP == 1){
        BREAK_IF_DEBUGGER_CONNECTED();
        LOG_INFO("Battery is currently in sleep mode");

        uint16_t disable_sleep = 0x009A;
        if(send_subcommand(disable_sleep)){
            return false;
        }

    }else if (ctrl_status.DEEPSLEEP ==1) {
        BREAK_IF_DEBUGGER_CONNECTED();
        LOG_INFO("Battery is currently in deep sleep mode");

        uint16_t exit_deep_sleep = 0x00E;
        if(send_subcommand(exit_deep_sleep)){
            return false;
        }
    }

    //turn on all the fets
    uint16_t turning_on_fets_cmd = 0x0096;
    if(send_subcommand(turning_on_fets_cmd)){
        return false;
    }
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
    if (!hw_i2c_memoryRead(&bat_mtr, REG_RESPONSE_LENGTH, &respLen, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    if (respLen != expectedLen)
    {
        return false;
    }

    if (!hw_i2c_memoryRead(&bat_mtr, REG_DATA_BUFFER, buffer, expectedLen))
    {
        return -1;
    }

    uint8_t checksum;
    if (!hw_i2c_memoryRead(&bat_mtr, REG_CHECKSUM, &checksum, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    uint8_t calcChecksum = (uint8_t)(((uint8_t)(cmd & BYTE_MASK)) +
                                 ((uint8_t)((cmd >> BYTE_SHIFT) & BYTE_MASK)) +
                                 ((uint8_t)respLen));
    for (uint8_t i = 0; i < expectedLen; i++)
    {
        calcChecksum += buffer[i];
    }
        
    calcChecksum = ~calcChecksum; // Invert bits

    return (calcChecksum == checksum);
}

typedef enum {
    ENABLED_PROTECTIONS_A      = 0x9256,
    ENABLED_PROTECTIONS_B      = 0x9262,
    OV_THRESHOLD               = 0x9278,
    UV_THRESHOLD               = 0x9275,
    OV_DELAY                   = 0x9279,
    UV_DELAY                   = 0x9276,
    OV_RECOVERY_HYSTERESIS     = 0x927C,
    UV_RECOVERY_HYSTERESIS     = 0x927B,
    SCD_THRESHOLD              = 0x92C0,
    OTC_THRESHOLD              = 0x929A,
    OCD1_DELAY                 = 0x9283,
    OCD1_THRESHOLD             = 0x9282,
    OCD_RECOVERY_THRESHOLD     = 0x928D
} protection_t;

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
    if (!hw_i2c_memoryRead(&bat_mtr, ALERT_PIN_CONFIG, &alert_config, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    /* Set the active-low bit while preserving other bits */
    alert_config |= (1 << ALERT_ACTIVE_LOW_BIT);
    if (!hw_i2c_memoryWrite(&bat_mtr, ALERT_PIN_CONFIG, &alert_config, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    /* Configure ALERT pin as an interrupt output */
    if (!hw_i2c_memoryWrite(&bat_mtr, ALERT_PIN_CONFIG, (uint8_t[]){ ALERT_PIN_INTERRUPT_CONFIG }, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    /* Enable ADC scan alerts */
    if (!hw_i2c_memoryWrite(&bat_mtr, ALARM_ENABLE_REG, (uint8_t[]){ ALARM_ENABLE_VALUE }, RESPONSE_LENGTH_SIZE))
    {
        return false;
    }

    /* Enable OV (Bit 3), UV (Bit 2), OCD1 (Bit 5) and SCD (Bit 7) */
    uint8_t protections_a = 0xAC;
    if (!hw_i2c_memoryWrite(&bat_mtr, ENABLED_PROTECTIONS_A, &protections_a, 1)) 
    {
        return false;
    }

    /* Enable OTD (Bit 5) */
    uint8_t protections_b = 0x20;
    if (!hw_i2c_memoryWrite(&bat_mtr, ENABLED_PROTECTIONS_B, &protections_b, 1))
    {
        return false;
    }

    /* Set the OV threshold to ~4200mV */
    uint8_t ov_threshold = 0x53;
    if (hw_i2c_memoryWrite(&bat_mtr, OV_THRESHOLD, &ov_threshold, 1))
    {
        return false;
    }

    /* Set the UV threshold to ~2500mV */
    uint8_t uv_threshold = 0x31;
    if (!hw_i2c_memoryWrite(&bat_mtr, UV_THRESHOLD, &uv_threshold, 1))
    {
        return false;
    }

    /* Set OCD1 threshold to 30V (the default delay is acceptable) */
    uint8_t ocd1_threshold = 0x5A;
    if (!hw_i2c_memoryWrite(&bat_mtr, OCD1_THRESHOLD, &ocd1_threshold, 1)) 
    {
        return false;
    }

    /* Set the OV, UV, and OCD1 delay to 100ms */
    uint8_t ov_u_ocd1_delay = 0x1E; // 100ms in register format
    if (!hw_i2c_memoryWrite(&bat_mtr, OV_DELAY, &ov_u_ocd1_delay, 1)) 
    {
        return false;
    }
    if (!hw_i2c_memoryWrite(&bat_mtr, UV_DELAY, &ov_u_ocd1_delay, 1)) 
    {
        return false;
    }
    if (!hw_i2c_memoryWrite(&bat_mtr, OCD1_DELAY, &ov_u_ocd1_delay, 1)) 
    {
        return false;
    }
    
    /* Default hysteresis for COV and CUV (0.1012V). */
    /* Default Recovery time for SCD (5s). */
    /* Default hysteresis for OCD (0.2A). */

    /* To-do: Validate config (not needed?)*/

    /**
     * config end 
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

    return true;
}

/**
 * @brief Initializes the low-voltage battery monitoring system.
 *
 * @return true if initialization was successful; false otherwise.
 */
bool io_lowVoltageBattery_init(void)
{
}

/**
 * @brief Gets the battery state-of-charge (SOC) as a percentage.
 *
 * @return SOC percentage on success, or -1.0f on error.
 */
float io_lowVoltageBattery_get_SOC(void)
{
    if (!io_lowVoltageBattery_send_subcommand(ACCUMULATED_CHARGE_COMMAND))
    {
        return -1.0f;
    }

    uint8_t buffer[SOC_RESPONSE_LENGTH];
    if (!io_lowVoltageBattery_read_response(ACCUMULATED_CHARGE_COMMAND, SOC_RESPONSE_LENGTH, buffer))
    {
        return -1.0f;
    }

    /* Parse the 3-byte charge value (buffer[0]-buffer[2]) */
    uint32_t charge = ((uint32_t)buffer[0]) | 
                  ((uint32_t)buffer[1] << BYTE_SHIFT) | 
                  ((uint32_t)buffer[2] << (BYTE_SHIFT * 2));
    float CC_GAIN = HardwareConfig.adc_calibration_factor / HardwareConfig.r_sense;
    float charge_mAh = (((float)charge) * CC_GAIN) / ((float)HardwareConfig.seconds_per_hour);
                  
    /* Clear any pending alert */
    uint8_t alarmClear = ALARM_CLEAR_CMD;
    hw_i2c_memoryWrite(&bat_mtr, ALARM_STATUS_REG, &alarmClear, RESPONSE_LENGTH_SIZE);

    return (charge_mAh / HardwareConfig.q_full) * HardwareConfig.percentage_factor;
}

/**
 * @brief Gets the cell voltage or the entire stack voltage
 *
 * @param voltage_cmd The subcommand used to read the voltage.
 *
 * @return The battery voltage on success, or -1 on error.
 */
uint16_t io_lowVoltageBattery_get_voltage(voltage_cmd_t voltage_cell)
{
    uint8_t voltage_cmd = (uint8_t) voltage_cell;

    if (!hw_i2c_transmit(&bat_mtr, &voltage_cmd, VOLTAGE_TRANSMIT_LENGTH))
    {
        return (uint16_t) -1;
    }
    uint8_t voltage_buffer[VOLTAGE_RESPONSE_LENGTH];
    if (!hw_i2c_receive(&bat_mtr, voltage_buffer, VOLTAGE_RESPONSE_LENGTH))
    {
        return (uint16_t) -1;
    }
    uint16_t voltage = (uint16_t)(buffer[0] | (buffer[1] << 8));
    return voltage;
}

/**
 * @brief Handles releasing the semaphore after an interupt.
 * 
 * @param GPIO_pin from the interupt. 
 */
void io_lowVoltageBattery_completeAlert(uint16_t GPIO_pin){}
