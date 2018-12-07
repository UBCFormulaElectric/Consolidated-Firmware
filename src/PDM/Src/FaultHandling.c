/******************************************************************************
* Includes
*******************************************************************************/
#include "FaultHandling.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
volatile uint8_t num_faults[ADC_CHANNEL_COUNT * NUM_CHANNELS] = {0};

/******************************************************************************
* Private Function Prototypes
*******************************************************************************/
/**
 * @brief  Toggle E-Fuse output
 * @param  index Index of E-Fuse to toggle
 * @param  state Toggle on or off
 */
static void FaultHandling_CurrentFaultHandling(uint8_t index, GPIO_PinState state);

/******************************************************************************
* Private Function Definitions
*******************************************************************************/
static void FaultHandling_CurrentFaultHandling(uint8_t index, GPIO_PinState state)
{
    if (index < ADC_CHANNEL_COUNT) {
        HAL_GPIO_WritePin(OUTPUT_0_PINOUT.port[index],
                          OUTPUT_0_PINOUT.pin[index],
                          state);
    } else {
        index = index - ADC_CHANNEL_COUNT; // adjust index for pinout array
        HAL_GPIO_WritePin(OUTPUT_1_PINOUT.port[index],
                          OUTPUT_1_PINOUT.pin[index],
                          state);
    }
}

/******************************************************************************
* Function Definitions
*******************************************************************************/
void FaultHandling_Handler(volatile uint8_t* fault_states, volatile float* converted_readings)
{
    uint64_t CAN_error_msg;

    for (uint8_t ADC_channel = 0; ADC_channel < ADC_TOTAL_READINGS_SIZE; ADC_channel++) {
        if (ADC_channel == _12V_SUPPLY_INDEX || ADC_channel == VBAT_SUPPLY_INDEX ||
            ADC_channel == VICOR_SUPPLY_INDEX)
            continue;

        // If the efuse is not in RETRY or ERROR mode and the current reading is
        // over the limit, disable efuse
        if (fault_states[ADC_channel] == STATIC_EFUSE && converted_readings[ADC_channel] >= CURRENT_LIMIT) {
            num_faults[ADC_channel]++;
            FaultHandling_CurrentFaultHandling(ADC_channel, GPIO_PIN_RESET);

            // Has faulted fewer than max number of times
            if (num_faults[ADC_channel] <= MAX_FAULTS[ADC_channel]) {
                fault_states[ADC_channel] = RENABLE_EFUSE;
            } else {
                // Special handling for specific outputs
                if (ADC_channel == R_INV_INDEX || ADC_channel == L_INV_INDEX || ADC_channel == CAN_INDEX || ADC_channel == AIR_SHDN_INDEX) {
                    fault_states[R_INV_INDEX] = ERROR_EFUSE;
                    fault_states[L_INV_INDEX] = ERROR_EFUSE;
                    FaultHandling_CurrentFaultHandling(L_INV_INDEX, GPIO_PIN_RESET); // Disable L Inv
                    FaultHandling_CurrentFaultHandling(R_INV_INDEX, GPIO_PIN_RESET); // Disable R Inv

                    TransmitCANError(MOTOR_SHUTDOWN_ERROR, Power_Distribution_Module, 0, 0);
                } else if (ADC_channel == COOLING_INDEX) {
                    TransmitCANError(
                    MOTOR_SHUTDOWN_ERROR, Power_Distribution_Module, 0, 0);
                }
                fault_states[ADC_channel] = ERROR_EFUSE;

                // TODO  (Issue #191): CAN message implementation
                CAN_error_msg =
                (ADC_channel << 16) +
                (uint16_t)(converted_readings[ADC_channel] * ADC_12_BIT_POINTS /
                           (VOLTAGE_TO_CURRENT[ADC_channel] * VDDA_VOLTAGE));
                TransmitCANError(PDM_ERROR,
                                 Power_Distribution_Module,
                                 EFUSE_FAULT,
                                 CAN_error_msg);
            }
        }
    }

    if (converted_readings[_12V_SUPPLY_INDEX] < UNDERVOLTAGE_GLV_THRES) {
        TransmitCANError(
        PDM_ERROR, Power_Distribution_Module, _12V_FAULT_UV, CAN_error_msg);
        num_faults[_12V_SUPPLY_INDEX]++;
    } else if (converted_readings[_12V_SUPPLY_INDEX] > OVERVOLTAGE_GLV_THRES) {
        TransmitCANError(
        PDM_ERROR, Power_Distribution_Module, _12V_FAULT_OV, CAN_error_msg);
        num_faults[_12V_SUPPLY_INDEX]++;
    }

    if (converted_readings[VBAT_SUPPLY_INDEX] > VBAT_OVERVOLTAGE) {
        TransmitCANError(
        PDM_ERROR, Power_Distribution_Module, VBAT_FAULT, CAN_error_msg);
        num_faults[VBAT_SUPPLY_INDEX]++;
    }

    if (converted_readings[VICOR_SUPPLY_INDEX] > UNDERVOLTAGE_VICOR_THRES) {
            GPIO_ConfigurePreChargeComplete(fault_states);
    } else {
            GPIO_ConfigurePowerUp(fault_states);
    }
}

void FaultHandling_RetryEFuse(volatile uint8_t* fault_states)
{
    for (uint8_t ADC_channel = 0; ADC_channel < ADC_TOTAL_READINGS_SIZE; ADC_channel++) {
        if (ADC_channel == _12V_SUPPLY_INDEX || ADC_channel == VBAT_SUPPLY_INDEX ||
            ADC_channel == VICOR_SUPPLY_INDEX)
            continue;

        if (fault_states[ADC_channel] == RENABLE_EFUSE) {
            FaultHandling_CurrentFaultHandling(ADC_channel, GPIO_PIN_SET);
            fault_states[ADC_channel] = STATIC_EFUSE;
        } else if (fault_states[ADC_channel] == ERROR_EFUSE)
            FaultHandling_CurrentFaultHandling(ADC_channel, GPIO_PIN_RESET);
    }
}
