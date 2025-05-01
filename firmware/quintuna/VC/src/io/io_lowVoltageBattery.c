#include "io_lowVoltageBattery.h"
#include "hw_utils.h"
#include "io_log.h"
#include <stdbool.h>
#include <stdint.h>

#define BYTE_MASK(x) ((x) & 0XFF)
#define FRACTION 4294967296.0

typedef union
{
    float    f;
    uint32_t u;
} ResponseValue;

typedef enum
{
    BQ76922_I2C_ADDR    = 0x10,
    REG_SUBCOMMAND      = 0x3E,
    REG_DATA_BUFFER     = 0x40,
    REG_CHECKSUM        = 0x60,
    REG_RESPONSE_LENGTH = 0x61,
    ALERT_PIN_CONFIG    = 0x56,
    ALARM_ENABLE_REG    = 0x66,
    ALARM_STATUS_REG    = 0x62
} register_address_t;

typedef struct
{
    uint8_t response_buffer[32];
    uint8_t length;
} Subcommand_Response;

typedef enum
{
    CONFIG_UPDATE_COMMAND      = 0x0090,
    CONFIG_UPDATE_EXIT_COMMAND = 0x0092,
    OTP_COMMAND                = 0x00F4,
    OTP_WR_CHECK               = 0x00A0,
    OTP_WR                     = 0x00A1
} config_cmd_t;

typedef enum
{
    ALARM_CLEAR_CMD            = 0x01,
    ALERT_ACTIVE_LOW_BIT       = 5,    // Bit position for active-low configuration
    ALERT_PIN_INTERRUPT_CONFIG = 0x02, // ALERT pin configured as an interrupt output
    ALARM_ENABLE_VALUE         = 0x82  // Enable ADC scan alerts
} alarm_config_t;

typedef enum
{
    I2C_TIMEOUT_MS       = 100, // Timeout for I2C operations
    CMD_SUBCOMMAND_SIZE  = 2,   // Two bytes for subcommand
    RESPONSE_LENGTH_SIZE = 4,   // One byte for response length and checksum
    SOC_RESPONSE_LENGTH  = 6,   // Expected response size for SOC command
} comm_settings_t;

typedef struct __attribute__((packed))
{
    uint8_t SEC0 : 1;
    uint8_t SEC1 : 1;
    uint8_t SS : 1;
    uint8_t FUSE : 1;
    uint8_t PF : 1;
    uint8_t SDM : 1;
    uint8_t RSVD : 1;
    uint8_t SLEEP : 1;

    uint8_t CFGUPDATE : 1;
    uint8_t PCHG_MODE : 1;
    uint8_t SLEEP_EN : 1;
    uint8_t POR : 1;
    uint8_t WD : 1;
    uint8_t COW_CHECK : 1;
    uint8_t OTPW : 1;
    uint8_t OTPB : 1;
} Battery_Status;

typedef struct __attribute__((packed))
{
    uint8_t RSVD2 : 8;

    uint8_t LD_ON : 1;
    uint8_t LD_TIMEOUT : 1;
    uint8_t DEEPSLEEP : 1;
    uint8_t RSVD1 : 5;
} Control_Status;

typedef struct __attribute__((packed))
{
    uint8_t RSVD1 : 1; // Reserved
    uint8_t RSVD2 : 1; // Reserved
    uint8_t CUV : 1;   // Cell undervoltage
    uint8_t COV : 1;   // Cell overvoltage
    uint8_t OCC : 1;   // Overcurrent in charge
    uint8_t OCD1 : 1;  // Overcurrent in discharge 1
    uint8_t OCD2 : 1;  // Overcurrent in discharge 2
    uint8_t SCD : 1;   // Short Circuit in Discharge
} SafteyAlertA;        // alerts associated with the battery chip

typedef struct __attribute__((packed))
{
    uint8_t RSVD1 : 1; // Reserved
    uint8_t RSVD2 : 1; // Reserved
    uint8_t CUV : 1;   // Cell undervoltage
    uint8_t COV : 1;   // Cell overvoltage
    uint8_t OCC : 1;   // Overcurrent in charge
    uint8_t OCD1 : 1;  // Overcurrent in discharge 1
    uint8_t OCD2 : 1;  // Overcurrent in discharge 2
    uint8_t SCD : 1;   // Short Circuit in Discharge
} SafteyFaultA;        // Faults associated with the battery chip


typedef struct __attribute__((packed))
{
    uint8_t INITCOMP : 1;
    uint8_t INITSTART : 1;
    uint8_t MSK_PFALERT :1;
    uint8_t MSK_SFALERT : 1;
    uint8_t PF :1;
    uint8_t SSA :1;
    uint8_t SSBC : 1;

    uint8_t WAKE : 1; 
    uint8_t ADSCAN : 1; 
    uint8_t CB : 1;   
    uint8_t FUSE : 1; 
    uint8_t SHUTV : 1;
    uint8_t XDSG : 1;  
    uint8_t XCHG : 1;  
    uint8_t FULLSCAN : 1;   
} AlertStatus;

typedef struct
{
    float  r_sense;
    double q_full;
    float  seconds_per_hour;
    double percentage_factor;
    float  adc_calibration_factor;
} HardwareConfig_t;

/* Define the hardware configuration values */
static const HardwareConfig_t HardwareConfig = { .r_sense                = 3.0f,
                                                 .q_full                 = 11200.0,
                                                 .seconds_per_hour       = 3600.0f,
                                                 .percentage_factor      = 100.0,
                                                 .adc_calibration_factor = 7.4768f };

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
    uint8_t lower_cmd[2] = { (uint8_t)(BYTE_MASK(cmd)), (uint8_t)BYTE_MASK(cmd >> 8) };
    if (!hw_i2c_memoryWrite(&bat_mtr, REG_SUBCOMMAND, lower_cmd, 2))
    {
        return false;
    }
    return true;
}

static bool recieve_subcommand(uint16_t cmd, Subcommand_Response *response)
{
    bool    finished_subcmd = false;
    uint8_t buffer_read[2];

    // keeping waiting for the subcommand to finish operation and then proceed to read the message from the chip
    while (finished_subcmd)
    {
        if (!hw_i2c_memoryRead(&bat_mtr, REG_SUBCOMMAND, buffer_read, 2))
        {
            return false;
        }

        finished_subcmd = ((buffer_read[0] << 8 | buffer_read[1]) == cmd) ? true : false;
    }
    uint8_t length;
    if (!hw_i2c_memoryRead(&bat_mtr, REG_RESPONSE_LENGTH, &length, 1))
    {
        return false;
    }
    // subtract by 4 to account for the 0x3E/0x3F and 0x61 and 0x60
    response->length = length - 4;

    // there may be two ways of doing this. If the frame is not limited to two bytes only then but then in the specific
    // function call you process the data
    if (!hw_i2c_memoryRead(&bat_mtr, REG_DATA_BUFFER, response->response_buffer, response->length))
    {
        return false;
    }

    // optional CRC check if we want. Gonn leave it uncommented for now
    uint8_t calcChecksum =
        (uint8_t)(((uint8_t)(BYTE_MASK(cmd))) + ((uint8_t)((BYTE_MASK(cmd >> 8))) + ((uint8_t)response->length)));

    uint8_t calc_inversion = ~calcChecksum;
    uint8_t checksum;
    if (!hw_i2c_memoryRead(&bat_mtr, REG_CHECKSUM, &checksum, 1))
    {
        return false;
    }

    if (calc_inversion != checksum)
    {
        return false;
    }
    return true;
}

bool io_lowVoltageBattery_SafetyStatusCheck(){
    uint8_t buffer_safety[1] = { (uint8_t)0x64 };
    if (!hw_i2c_transmit(&bat_mtr, buffer_safety, 1))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }
    AlertStatus saftey;
    if (!hw_i2c_receive(&bat_mtr, (uint8_t *)&saftey, 2))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }

    uint8_t buffer_safetA[1] = { (uint8_t)0x03 };
    if (!hw_i2c_transmit(&bat_mtr, buffer_safetA, 1))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }
    SafteyFaultA safteyA;
    if (!hw_i2c_receive(&bat_mtr, (uint8_t *)&safteyA, 1))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }

    uint8_t buffer_safetB[1] = { (uint8_t)0x05 };
    if (!hw_i2c_transmit(&bat_mtr, buffer_safetB, 1))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }
    uint8_t safteyB[1];
    if (!hw_i2c_receive(&bat_mtr, safteyB, 1))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }
    uint8_t buffer_safetC[1] = { (uint8_t)0x07 };
    if (!hw_i2c_transmit(&bat_mtr, buffer_safetC, 1))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }
    uint8_t safteyC[1];
    if (!hw_i2c_receive(&bat_mtr, safteyC, 1))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }

    return true;
}

bool io_lowVoltageBattery_initial_setup(void)
{
    // checking if the target is even ready to communicate with
    // adding debuggers to make sure the code doesnt run wild and free when we run
    uint8_t buffer_bat[1] = { (uint8_t)BATTERY_STATUS };

    if (!hw_i2c_isTargetReady(&bat_mtr))
    {
        return false;
    }

    // ask for battery status to check if the device is sleep or not
    if (!hw_i2c_transmit(&bat_mtr, buffer_bat, 1))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }

    Battery_Status bat_status;
    if (!hw_i2c_receive(&bat_mtr, (uint8_t *)&bat_status, 2))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }

    if(!io_lowVoltageBattery_SafetyStatusCheck()){
        return false;
    }

    // ask for control status
    uint8_t buffer_control[1] = { (uint8_t)CONTROL_STATUS };
    if (!hw_i2c_transmit(&bat_mtr, buffer_control, 1))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }

    Control_Status ctrl_status;
    if (!hw_i2c_receive(&bat_mtr, (uint8_t *)&ctrl_status, 2))
    {
        BREAK_IF_DEBUGGER_CONNECTED();
        return false;
    }
    if (bat_status.SLEEP == 1)
    {
        LOG_INFO("Battery is currently in sleep mode");

        uint16_t disable_sleep = 0x009A;
        if (!send_subcommand(disable_sleep))
        {
            return false;
        }
    }
    else if (ctrl_status.DEEPSLEEP == 1)
    {
        LOG_INFO("Battery is currently in deep sleep mode");

        uint16_t exit_deep_sleep = 0x00E;
        if (!send_subcommand(exit_deep_sleep))
        {
            return false;
        }
    }

    if (!hw_i2c_transmit(&bat_mtr, buffer_bat, 1))
    {
        return false;
    }

    if (!hw_i2c_receive(&bat_mtr, (uint8_t *)&bat_status, 2))
    {
        return false;
    }
    // turn on all the fets
    uint16_t turning_on_fets_cmd = 0x0096;
    if (!send_subcommand(turning_on_fets_cmd))
    {
        return false;
    }

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

typedef enum
{
    ENABLED_PROTECTIONS_A  = 0x9256,
    ENABLED_PROTECTIONS_B  = 0x9262,
    OV_THRESHOLD           = 0x9278,
    UV_THRESHOLD           = 0x9275,
    OV_DELAY               = 0x9279,
    UV_DELAY               = 0x9276,
    OV_RECOVERY_HYSTERESIS = 0x927C,
    UV_RECOVERY_HYSTERESIS = 0x927B,
    SCD_THRESHOLD          = 0x92C0,
    OTC_THRESHOLD          = 0x929A,
    OCD1_DELAY             = 0x9283,
    OCD1_THRESHOLD         = 0x9282,
    OCD_RECOVERY_THRESHOLD = 0x928D
} protection_t;

/**
 * @brief Gets the battery state-of-charge (SOC) as a percentage.
 *
 * @return SOC percentage on success, or -1.0f on error.
 */
double io_lowVoltageBattery_get_SOC(void)
{
    if (!send_subcommand(ACCUMULATED_CHARGE_COMMAND))
    {
        return -1.0;
    }

    Subcommand_Response subcmd_response;
    if (!recieve_subcommand(ACCUMULATED_CHARGE_COMMAND, &subcmd_response))
    {
        return -1.0;
    }

    // Check what the hell is going on here

    /* Parse the 3-byte charge value (buffer[0]-buffer[2]) */
    // uint32_t charge = ((uint32_t)subcmd_response.response_buffer[0]) |
    //               ((uint32_t)subcmd_response.response_buffer[1] << 8) |
    //               ((uint32_t)subcmd_response.response_buffer[2] << (8 * 2));
    // float CC_GAIN = HardwareConfig.adc_calibration_factor / HardwareConfig.r_sense;
    // float charge_mAh = (((float)charge) * CC_GAIN) / ((float)HardwareConfig.seconds_per_hour);

    uint32_t charge_interger_portion =
        ((uint32_t)subcmd_response.response_buffer[0] << 16) | ((uint32_t)subcmd_response.response_buffer[1] << 8) |
        ((uint32_t)subcmd_response.response_buffer[2]) | ((uint32_t)subcmd_response.response_buffer[3]);

    uint32_t charge_fraction_portion =
        ((uint32_t)subcmd_response.response_buffer[3] << 16) | ((uint32_t)subcmd_response.response_buffer[4] << 8) |
        ((uint32_t)subcmd_response.response_buffer[5]) | ((uint32_t)subcmd_response.response_buffer[6]);

    double charge = (double)charge_interger_portion + ((double)charge_fraction_portion / FRACTION);
    /* Clear any pending alert */
    // uint8_t alarmClear = ALARM_CLEAR_CMD;
    // if (!hw_i2c_memoryWrite(&bat_mtr, ALARM_STATUS_REG, &alarmClear, RESPONSE_LENGTH_SIZE)){
    //     return -1.0;
    // }
    return (charge / HardwareConfig.q_full) * HardwareConfig.percentage_factor;
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
    uint8_t voltage_cmd = (uint8_t)voltage_cell;

    if (!hw_i2c_transmit(&bat_mtr, &voltage_cmd, 1))
    {
        return (uint16_t)-1;
    }
    uint8_t voltage_buffer[2];
    if (!hw_i2c_receive(&bat_mtr, voltage_buffer, 2))
    {
        return (uint16_t)-1;
    }
    uint16_t voltage = (uint16_t)(voltage_buffer[0] | (voltage_buffer[1] << 8));
    return voltage;
}

/**
 * @brief Handles releasing the semaphore after an interupt.
 *
 * @param GPIO_pin from the interupt.
 */
void io_lowVoltageBattery_completeAlert_callBack(uint16_t GPIO_pin) {}
