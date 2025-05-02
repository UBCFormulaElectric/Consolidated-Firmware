#include "io_lowVoltageBattery.h"
#include "hw_i2c.h"
#include "hw_i2cs.h"
#include "hw_utils.h"
#include "io_log.h"
#include "io_lowVoltageBatReg.h"
#include <stdbool.h>
#include <stdint.h>
#include <stm32h7xx_hal_def.h>
#include <stm32h7xx_hal_dma.h>
#include <stm32h7xx_hal_mdma.h>

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
//0x9286 address of scd threshod

static bool recieve_subcommand(uint16_t cmd, Subcommand_Response *response)
{
    bool    finished_subcmd = true;
    uint8_t buffer_read[2];

    // keeping waiting for the subcommand to finish operation and then proceed to read the message from the chip
    while (finished_subcmd)
    {
        if (!hw_i2c_memoryRead(&bat_mtr, REG_SUBCOMMAND, buffer_read, 2))
        {
            return false;
        }

        finished_subcmd = ((buffer_read[0] | buffer_read[1] << 8) == cmd) ? false : true;
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
        (uint8_t)(((uint8_t)(BYTE_MASK(cmd))) + ((uint8_t)((BYTE_MASK(cmd >> 8)))));

    for (uint8_t i = 0; i<response->length; i++) {
        calcChecksum+=response->length;
    }

    uint8_t calc_inversion = ~((uint8_t)BYTE_MASK(calcChecksum));
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

static bool send_writeSubcommandToLowerSCDThresh(){
    
    uint8_t thresh_cmd_address[2] = {(uint8_t) BYTE_MASK(0x9286), (uint8_t) BYTE_MASK(0x9286 >> 8)};
    //first we are writing to address 0x3E to set the subcommand
    if(!hw_i2c_memoryWrite(&bat_mtr, REG_SUBCOMMAND, thresh_cmd_address, 2)){
        return false;
    }
    //what we are trying to do with info is send a number with max 15 which can be done in a byte
    uint8_t thresh = 0x09; //13 in hex cause we are setting it 400mV max threshold
    if (!hw_i2c_memoryWrite(&bat_mtr, REG_DATA_BUFFER, &thresh, 1)){
        return false;
    }
    //need to add the CRC config here and we need to transmit both checksum and length in one

    uint8_t calcChecksum =
    (uint8_t)(((uint8_t)(BYTE_MASK(0x9286))) + ((uint8_t)((BYTE_MASK(0x9286 >> 8)))) + thresh);

    uint8_t crc_length_thresh[2] = [~calcChecksum, 5];

    if (!hw_i2c_memoryWrite(&bat_mtr, REG_CHECKSUM, crc_length_thresh, 2)) {
        return false;
    }

    Subcommand_Response thresh_response;
    if(!recieve_subcommand(0x9286,&thresh_response)){
        return false;
    }

    return true;
}

static bool send_writeSubcommandToLowerSCDDelay(){
    uint8_t delay_cmd_address[2] = {(uint8_t) BYTE_MASK(0x9287), (uint8_t)BYTE_MASK(0x9287<<8)};
    if (!hw_i2c_memoryWrite(&bat_mtr, REG_SUBCOMMAND, delay_cmd_address, 2)) {
        return false;
    }

    uint8_t delay = 0x01;
    if (!hw_i2c_memoryWrite(&bat_mtr, REG_DATA_BUFFER, &delay, 1)) {
        return false;
    }
    uint8_t calcChecksum1 =
    (uint8_t)(((uint8_t)(BYTE_MASK(0x9287))) + ((uint8_t)((BYTE_MASK(0x9287 >> 8)))) + delay);

    uint8_t crc_length_delay[2] = [~calcChecksum1, 5];

    if (!hw_i2c_memoryWrite(&bat_mtr, REG_CHECKSUM, crc_length_thresh, 2)) {
        return false;
    }

    Subcommand_Response delay_response;
    if(!recieve_subcommand(0x9287,&delay_response)){
        return false;
    }
}

bool io_lowVoltageBattery_SafetyStatusCheck(){

    //check the raw battery alarm status to see what is going on
    uint8_t buffer_safety[1] = { (uint8_t)0x64 };
    if (!hw_i2c_transmit(&bat_mtr, buffer_safety, 1))
    {
        return false;
    }
    AlertStatus saftey_status;
    if (!hw_i2c_receive(&bat_mtr, (uint8_t *)&saftey_status, 2))
    {
        return false;
    }

    //if there is bit set in status A check what is going on
    //TODO: Make sure we transmit a CAN message saying there is something wrong and specifically what is wrong
    if (saftey_status.SSA){
        LOG_ERROR("Registered a status fault A");
        uint8_t buffer_safetA[1] = { (uint8_t)0x03 };
        if (!hw_i2c_transmit(&bat_mtr, buffer_safetA, 1))
        {
            return false;
        }
        SafteyStatusA safetyFaultA;
        if (!hw_i2c_receive(&bat_mtr, (uint8_t *)&safetyFaultA, 1))
        {
            return false;
        }
        LOG_INFO("The issue is the following binary %d", safetyFaultA);
    }
    //if there is bit a bit set in status B check what is going on
    //TODO: Make sure we transmit a CAN message saying there is something wrong and specifically 
    else if (saftey_status.SSBC) {
        LOG_ERROR("Registered a status fault B");
        uint8_t buffer_safetB[1] = { (uint8_t)0x05 };
        if (!hw_i2c_transmit(&bat_mtr, buffer_safetB, 1))
        {
            return false;
        }
        uint8_t safteyB[1];
        if (!hw_i2c_receive(&bat_mtr, safteyB, 1))
        {
            return false;
        }
        uint8_t buffer_safetC[1] = { (uint8_t)0x07 };
        if (!hw_i2c_transmit(&bat_mtr, buffer_safetC, 1))
        {
            return false;
        }
        uint8_t safteyC[1];
        if (!hw_i2c_receive(&bat_mtr, safteyC, 1))
        {
            return false;
        }
    }

    return true;
}

bool io_lowVoltageBattery_initial_setup(void)
{
    uint8_t buffer_bat[1] = { (uint8_t)BATTERY_STATUS };

    if (!hw_i2c_isTargetReady(&bat_mtr))
    {
        return false;
    }

    // ask for battery status to check if the device is sleep or not
    if (!hw_i2c_transmit(&bat_mtr, buffer_bat, 1))
    {
        return false;
    }

    Battery_Status bat_status;
    if (!hw_i2c_receive(&bat_mtr, (uint8_t *)&bat_status, 2))
    {
        return false;
    }
    //Put the chip into config update mode
    if (!send_subcommand((uint16_t) 0x0090)) {
        return false;
    }

    if(!io_lowVoltageBattery_SafetyStatusCheck()){
        return false;
    }

    // ask for control status
    uint8_t buffer_control[1] = { (uint8_t)CONTROL_STATUS };
    if (!hw_i2c_transmit(&bat_mtr, buffer_control, 1))
    {
        return false;
    }

    Control_Status ctrl_status;
    if (!hw_i2c_receive(&bat_mtr, (uint8_t *)&ctrl_status, 2))
    {
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

    if(!send_writeSubcommandToLowerSCDThresh()){
        return false;
    }
    if (!send_writeSubcommandToLowerSCDDelay()) {
        return false;
    }

    if (!hw_i2c_transmit(&bat_mtr, buffer_bat, 1))
    {
        return false;
    }

    if (!hw_i2c_receive(&bat_mtr, (uint8_t *)&bat_status, 2))
    {
        return false;
    }

    uint16_t manu_status1 = 0x0057;
    send_subcommand(manu_status1);
    Subcommand_Response response;
    if(!recieve_subcommand(manu_status1, &response)){
        return false;
    }

    uint16_t fet_enable = 0x0022;
    if (!send_subcommand(fet_enable))
    {
        return false;
    }

    uint16_t turning_on_fets_cmd = 0x0096;
    if (!send_subcommand(turning_on_fets_cmd))
    {
        return false;
    }

    while(1){
        uint16_t manu_status = 0x0057;
        send_subcommand(manu_status);
        Subcommand_Response response1;
        if(!recieve_subcommand(manu_status, &response1)){
            return false;
        }

            // turn on all the fets
        if (!io_lowVoltageBattery_SafetyStatusCheck()) {
            return false;
        }
    }
    //tell the chip to exit config udpate mode
    if(!send_subcommand((uint16_t) 0x0092)){
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
