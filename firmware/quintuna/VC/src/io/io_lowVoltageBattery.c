#include "io_lowVoltageBattery.h"
#include "app_utils.h"
#include "hw_i2cs.h"
#include "hw_utils.h"
#include "io_log.h"
#include <SEGGER.h>
#include <pb.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BYTE_MASK(x) ((x) & 0XFF)
#define FRACTION 4294967296.0

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

static inline uint8_t check_sum_calc(uint16_t cmd, uint8_t *data, size_t length)
{
    uint8_t calcChecksum = (uint8_t)(((uint8_t)(BYTE_MASK(cmd))) + ((uint8_t)((BYTE_MASK(cmd >> 8)))));

    for (uint8_t i = 0; i < length; i++)
    {
        calcChecksum += data[length];
    }

    return ~((uint8_t)BYTE_MASK(calcChecksum));
}

static ExitCode send_subcommand(uint16_t cmd)
{
    uint8_t lower_cmd[2] = { (uint8_t)(BYTE_MASK(cmd)), (uint8_t)BYTE_MASK(cmd >> 8) };

    RETURN_IF_ERR(hw_i2c_memoryWrite(&bat_mtr, REG_SUBCOMMAND, lower_cmd, 2));

    return EXIT_CODE_OK;
}

static ExitCode recieve_subcommand(uint16_t cmd, Subcommand_Response *response)
{
    bool    finished_subcmd = true;
    uint8_t buffer_read[2];

    // keeping waiting for the subcommand to finish operation and then proceed to read the message from the chip
    while (finished_subcmd)
    {
        RETURN_IF_ERR(hw_i2c_memoryRead(&bat_mtr, REG_SUBCOMMAND, buffer_read, 2));

        finished_subcmd = ((buffer_read[0] | buffer_read[1] << 8) == cmd) ? false : true;
    }
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
        return EXIT_CODE_ERROR;
    }
    return EXIT_CODE_OK;
}

static float translateVoltageData(int16_t voltage)
{
    return voltage * 0.001f;
}

static ExitCode command_to_setThresholds(uint16_t cmd_id, uint8_t *data, size_t size_of_data)
{
    uint8_t thresh_cmd_address[2] = { (uint8_t)BYTE_MASK(cmd_id), (uint8_t)BYTE_MASK(cmd_id >> 8) };
    // first we are writing to address 0x3E to set the subcommand
    RETURN_IF_ERR(hw_i2c_memoryWrite(&bat_mtr, REG_SUBCOMMAND, thresh_cmd_address, 2));

    RETURN_IF_ERR(hw_i2c_memoryWrite(&bat_mtr, REG_DATA_BUFFER, data, 2));
    // need to add the CRC config here and we need to transmit both checksum and length in one

    uint8_t calcChecksum      = check_sum_calc(cmd_id, data, size_of_data);
    uint8_t size_of_frame_mem = (uint8_t)size_of_data + 4;

    uint8_t crc_length_thresh[2] = { calcChecksum, size_of_frame_mem };

    RETURN_IF_ERR(hw_i2c_memoryWrite(&bat_mtr, REG_CHECKSUM, crc_length_thresh, 2));

    RETURN_IF_ERR(hw_i2c_memoryWrite(&bat_mtr, REG_SUBCOMMAND, thresh_cmd_address, 2));
    Subcommand_Response thresh_response;
    RETURN_IF_ERR(recieve_subcommand(cmd_id, &thresh_response));

    return EXIT_CODE_OK;
}

ExitCode io_lowVoltageBattery_SafetyStatusCheck(SafetyStatusA *safetyA, SafetyStatusB *safetyB, SafetyStatusC *safetyC)
{
    // check the raw battery alarm status to see what is going on
    uint8_t buffer_safety[1] = { (uint8_t)0x64 };

    RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, buffer_safety, 1));

    AlertStatus safety_status;
    RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)&safety_status, 2));

    // if there is bit set in status A check what is going on
    if (safety_status.SSA)
    {
        LOG_ERROR("Registered a status fault A");
        uint8_t buffer_safetA[1] = { (uint8_t)0x03 };
        RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, buffer_safetA, 1));

        RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)safetyA, 1));
    }
    // if there is bit a bit set in status B check what is going on
    else if (safety_status.SSBC)
    {
        LOG_ERROR("Registered a status fault B");
        uint8_t buffer_safetB = (uint8_t)0x05;
        RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, &buffer_safetB, 1));

        RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)safetyB, 1));

        uint8_t buffer_safetC = (uint8_t)0x07;
        RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, &buffer_safetC, 1));

        RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)safetyC, 1));
    }

    return EXIT_CODE_OK;
}

ExitCode io_lowvoltageBattery_batteryStatus(Battery_Status *bat_status)
{
    uint8_t buffer_bat[1] = { (uint8_t)BATTERY_STATUS };
    // ask for battery status to check if the device is sleep or not
    RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, buffer_bat, 1));
    RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)bat_status, 2));

    return EXIT_CODE_OK;
}

ExitCode io_lowVoltageBattery_controlStatus(Control_Status *ctrl_status)
{
    uint8_t buffer_control[1] = { (uint8_t)CONTROL_STATUS };
    RETURN_IF_ERR(hw_i2c_transmit(&bat_mtr, buffer_control, 1));
    RETURN_IF_ERR(hw_i2c_receive(&bat_mtr, (uint8_t *)&ctrl_status, 2));

    return EXIT_CODE_OK;
}

ExitCode io_lowVoltageBattery_init(void)
{
    SafetyStatusA safetyA;
    SafetyStatusB safetyB;
    SafetyStatusC safetyC;

    RETURN_IF_ERR(hw_i2c_isTargetReady(&bat_mtr));

    // ask for battery status to check if the device is sleep or not
    // For now I am gonna cast this as a void but later may change to do something else
    Battery_Status bat_status;
    RETURN_IF_ERR(io_lowvoltageBattery_batteryStatus(&bat_status));

    // Put the chip into config update mode
    RETURN_IF_ERR(send_subcommand((uint16_t)0x0090));

    RETURN_IF_ERR(io_lowVoltageBattery_SafetyStatusCheck(&safetyA, &safetyB, &safetyC));

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
    RETURN_IF_ERR(command_to_setThresholds((uint16_t)SCD_THRESHOLD, thresh_scd, 2));

    uint8_t CUV_thresh[2] = { 0x37, 0x00 };
    RETURN_IF_ERR(command_to_setThresholds((uint16_t)UV_THRESHOLD, CUV_thresh, 2));

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
    return (charge / HardwareConfig.q_full) * HardwareConfig.percentage_factor;
}

/**
 * @brief Gets the cell voltage or the entire stack voltage
 *
 * @param voltage_cmd The subcommand used to read the voltage.
 *
 * @return The battery voltage on success, or -1 on error.
 */
float io_lowVoltageBattery_get_voltage(voltage_cmd_t voltage_cell)
{
    uint8_t voltage_cmd = (uint8_t)voltage_cell;

    if (!hw_i2c_transmit(&bat_mtr, &voltage_cmd, 1))
    {
        return -1;
    }
    uint8_t voltage_buffer[2];
    if (!hw_i2c_receive(&bat_mtr, voltage_buffer, 2))
    {
        return -1;
    }
    int16_t voltage = (int16_t)(voltage_buffer[0] | (voltage_buffer[1] << 8));
    return translateVoltageData(voltage);
}

// 62, 77
ExitCode io_lowVoltageBattery_controlBalancing(bool cell0, bool cell1, bool cell2, bool cell3)
{
    return EXIT_CODE_UNIMPLEMENTED;

    /*
    uint8_t cells_to_balance = (uint8_t)(0x00 | (cell0 | (cell1 << 1) | (cell2 << 2) | (cell3 << 3)));

    uint8_t balance_cmd_buf[2] = { 0x00, cells_to_balance };
    RETURN_IF_ERR(hw_i2c_memoryWrite(&bat_mtr, CELL_BALANCING, balance_cmd_buf, 2));

    uint8_t balance_info[2];
    RETURN_IF_ERR(hw_i2c_memoryRead(&bat_mtr, CELL_BALANCING, balance_info, 2));

    uint8_t cells_balancing = balance_info[0];

    if (cells_to_balance ^ cells_balancing)
    {
        return EXIT_CODE_ERROR;
    }

    return EXIT_CODE_OK;
    */
}

ExitCode io_lowVoltageBattery_configureBalancingThreshold(uint16_t voltage)
{
    return EXIT_CODE_UNIMPLEMENTED;

    /*
    uint8_t thresh_cmd_address[2] = {(uint8_t) BYTE_MASK(voltage), (uint8_t) BYTE_MASK(voltage)};

    uint8_t balance_cmd_buf[2] = { 0x00, cells_to_balance };
    RETURN_IF_ERR(hw_i2c_memoryWrite(&bat_mtr, CELL_BALANCING, balance_cmd_buf, 2));

    uint8_t balance_info[2];
    RETURN_IF_ERR(hw_i2c_memoryRead(&bat_mtr, CELL_BALANCING, balance_info, 2));

    uint8_t cells_balancing = balance_info[0];

    if (cells_to_balance ^ cells_balancing)
    {
        return EXIT_CODE_ERROR;
    }

    return EXIT_CODE_OK;
    */
}

ExitCode io_lowVoltageBattery_showCellsBalancing(BalanceStatus *status)
{
    return EXIT_CODE_UNIMPLEMENTED;

    // RETURN_IF_ERR(hw_i2c_memoryRead(&bat_mtr, CELL_BALANCING, &status, 2));
    // return EXIT_CODE_OK;
}

ExitCode io_lowVoltageBattery_additionalMeasurements(void)
{
    return EXIT_CODE_UNIMPLEMENTED;
}

/**
 * @brief Handles releasing the semaphore after an interupt.
 *
 * @param GPIO_pin from the interupt.
 */
void io_lowVoltageBattery_completeAlert_callBack(uint16_t GPIO_pin) {}
