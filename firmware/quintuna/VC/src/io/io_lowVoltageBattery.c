#include "io_lowVoltageBattery.h"
#include "app_utils.h"
#include "hw_i2cs.h"
#include "hw_utils.h"
#include "io_log.h"
#include <SEGGER.h>
#include <hw/hw_i2cs.h>
#include <pb.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BYTE_MASK(x) ((x) & 0XFF)
#define FRACTION 4294967296.0

/* Static function decleration--------------------------------------------------------------------------------*/
static inline uint8_t  check_sum_calc(uint16_t cmd, uint8_t *data, size_t length);
static inline ExitCode send_subcommand(uint16_t cmd);
static inline ExitCode send_subcommand_write(uint16_t cmd, uint8_t *write_buffer, uint16_t length);
static ExitCode        recieve_subcommand(uint16_t cmd, Subcommand_Response *response);

static inline uint8_t check_sum_calc(uint16_t cmd, uint8_t *data, size_t length)
{
    uint8_t calcChecksum = (uint8_t)(((uint8_t)(BYTE_MASK(cmd))) + ((uint8_t)((BYTE_MASK(cmd >> 8)))));

    for (uint8_t i = 0; i < length; i++)
    {
        calcChecksum += data[length];
    }

    return ~((uint8_t)BYTE_MASK(calcChecksum));
}

static inline ExitCode send_subcommand(uint16_t cmd)
{
    uint8_t lower_cmd[2] = { (uint8_t)(BYTE_MASK(cmd)), (uint8_t)BYTE_MASK(cmd >> 8) };

    RETURN_IF_ERR(hw_i2c_memoryWrite(&bat_mtr, REG_SUBCOMMAND, lower_cmd, 2));

    return EXIT_CODE_OK;
}

static ExitCode send_subcommand_write(uint16_t cmd, uint8_t *write_buffer, uint16_t length)
{
    // send the subcommand we are trying to communicate
    RETURN_IF_ERR(send_subcommand(cmd));

    RETURN_IF_ERR(hw_i2c_memoryWrite(&bat_mtr, REG_DATA_BUFFER, write_buffer, length));

    uint8_t calcChecksum      = check_sum_calc(cmd, write_buffer, length);
    uint8_t size_of_frame_mem = (uint8_t)length + 4;

    // 0x60 we need to write the crc and 0x61 we need to write the 0x61
    uint8_t crc_buffer[2] = { calcChecksum, size_of_frame_mem };
    RETURN_IF_ERR(hw_i2c_memoryWrite(&bat_mtr, REG_CHECKSUM, crc_buffer, 2));

    // make sure the data was written correctly
    RETURN_IF_ERR(send_subcommand(cmd));
    Subcommand_Response response;
    RETURN_IF_ERR(recieve_subcommand(cmd, &response));

    return EXIT_CODE_OK;
}

static ExitCode recieve_subcommand(uint16_t cmd, Subcommand_Response *response)
{
    bool    finished_subcmd = true;
    uint8_t buffer_read[2];

    // keeping waiting for the subcommand to finish operation and then proceed to read the message from the chip
    do
    {
        RETURN_IF_ERR(hw_i2c_memoryRead(&bat_mtr, REG_SUBCOMMAND, buffer_read, 2));
    } while (((buffer_read[0] | buffer_read[1] << 8) == cmd));

    uint8_t length;
    RETURN_IF_ERR(hw_i2c_memoryRead(&bat_mtr, REG_RESPONSE_LENGTH, &length, 1));
    // subtract by 4 to account for the 0x3E/0x3F and 0x61 and 0x60
    response->length = length - 4;

    // there may be two ways of doing this. If the frame is not limited to two bytes only then but then in the specific
    // function call you process the data
    RETURN_IF_ERR(hw_i2c_memoryRead(&bat_mtr, REG_DATA_BUFFER, response->response_buffer, response->length));

    uint8_t calccheck_inversion = check_sum_calc(cmd, response->response_buffer, response->length);
    uint8_t checksum;
    RETURN_IF_ERR(hw_i2c_memoryRead(&bat_mtr, REG_CHECKSUM, &checksum, 1));

    if (calccheck_inversion != checksum)
    {
        return EXIT_CODE_CHECKSUM_FAIL;
    }
    return EXIT_CODE_OK;
}

ExitCode io_lowVoltageBattery_SafetyStatusCheck(SafteyStatusA *safteyA, SafteyStatusB *safteyB, SafteyStatusC *safteyC)
{
    // check the raw battery alarm status to see what is going on
    uint8_t alarm_raw_status = (uint8_t)ALARM_RAW_STATUS;
    RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, &alarm_raw_status, 1));

    AlertStatus saftey_status;
    RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)&saftey_status, 2));

    // if there is bit set in status A check what is going on
    if (saftey_status.SSA)
    {
        LOG_ERROR("Registered a status fault A");
        uint8_t buffer_safetA = (uint8_t)SAFTEY_ALERT_A;

        RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, &buffer_safetA, 1));
        RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)safteyA, 1));
    }
    // if there is bit a bit set in status B check what is going on
    if (saftey_status.SSBC)
    {
        LOG_ERROR("Registered a status fault B and C");
        uint8_t buffer_safetB = (uint8_t)SAFTEY_STATUS_B;
        RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, &buffer_safetB, 1));
        RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)safteyB, 1));

        uint8_t buffer_safetC = (uint8_t)SAFTEY_STATUS_C;
        RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, &buffer_safetC, 1));
        RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)safteyC, 1));
    }

    return EXIT_CODE_OK;
}

ExitCode io_lowvoltageBattery_batteryStatus(Battery_Status *bat_status)
{
    uint8_t buffer_bat = (uint8_t)BATTERY_STATUS;
    // ask for battery status to check if the device is sleep or not
    RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, &buffer_bat, 1));
    RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)bat_status, 2));

    return EXIT_CODE_OK;
}

ExitCode io_lowVoltageBattery_controlStatus(Control_Status *ctrl_status)
{
    uint8_t buffer_control = (uint8_t)CONTROL_STATUS;
    RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, &buffer_control, 1));
    RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)&ctrl_status, 2));

    return EXIT_CODE_OK;
}

ExitCode io_lowVoltageBattery_init(void)
{
    RETURN_IF_ERR(hw_i2c_isTargetReady(&bat_mtr));

    // ask for battery status to check if the device is sleep or not
    // For now I am gonna cast this as a void but later may change to do something else
    Battery_Status bat_status;
    RETURN_IF_ERR(io_lowvoltageBattery_batteryStatus(&bat_status));

    // Put the chip into config update mode
    RETURN_IF_ERR(send_subcommand((uint16_t)0x0090))

    //RETURN_IF_ERR(io_lowVoltageBattery_SafetyStatusCheck())

    // ask for control status
    // for now leave as void but I am going to change this later
    Control_Status ctrl_status;
    RETURN_IF_ERR(io_lowVoltageBattery_controlStatus(&ctrl_status));

    if (bat_status.SLEEP == 1)
    {
        LOG_INFO("Battery is currently in sleep mode");

        uint16_t disable_sleep = 0x009A;
        RETURN_IF_ERR(send_subcommand(disable_sleep));
    }
    else if (ctrl_status.DEEPSLEEP == 1)
    {
        LOG_INFO("Battery is currently in deep sleep mode");

        uint16_t exit_deep_sleep = 0x00E;
        RETURN_IF_ERR(send_subcommand(exit_deep_sleep));
    }

    // uint16_t manu_status = 0x0057;
    // RETURN_IF_ERR(send_subcommand(manu_status));
    // Subcommand_Response response;
    // RETURN_IF_ERR(recieve_subcommand(manu_status,&response));

    uint8_t thresh_scd[2] = { 0x09, 0x00 };
    RETURN_IF_ERR(send_subcommand_write((uint16_t)SCD_THRESHOLD, thresh_scd, 2));

    uint8_t CUV_thresh[2] = { 0x37, 0x00 };
    RETURN_IF_ERR(send_subcommand_write((uint16_t)UV_THRESHOLD, CUV_thresh, 2));

    RETURN_IF_ERR(send_subcommand((uint16_t)0x0092));

    uint16_t fet_enable = 0x0022;
    RETURN_IF_ERR(send_subcommand(fet_enable));

    uint16_t turning_on_fets_cmd = 0x0096;
    RETURN_IF_ERR(send_subcommand(turning_on_fets_cmd));
    return EXIT_CODE_OK;
}

/**
 * @brief Gets the battery state-of-charge (SOC) as a percentage.
 *
 * @return SOC percentage on success, or -1.0f on error.
 */
double io_lowVoltageBattery_get_SOC(void)
{
    if (IS_EXIT_ERR(send_subcommand(ACCUMULATED_CHARGE_COMMAND)))
    {
        return -1.0;
    }

    Subcommand_Response subcmd_response;
    if (IS_EXIT_ERR(recieve_subcommand(ACCUMULATED_CHARGE_COMMAND, &subcmd_response)))
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
    return 0.0;
    //return (charge / HardwareConfig.q_full) * HardwareConfig.percentage_factor;
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
