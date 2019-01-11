/******************************************************************************
 * Includes
 ******************************************************************************/
#include "VoltageSense.h"
#include "SharedFilters.h"
#include "Gpio.h"
#include "Adc.h"
#include "SharedAdc.h"
#include "CanDefinitions.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// clang-format off

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/
#define INIT_VOLTAGE_SENSE(index, conversion_factor, can_stdid, can_dlc) \
    [index].voltage = 0, [index].voltage_scale = conversion_factor, \
    [index].can.std_id = can_stdid, [index].can.dlc = can_dlc \

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/
// clang-format on
typedef struct
{
    float32_t      voltage;
    float32_t      voltage_scale;
    CanInfo_Struct can;
} VoltageSense_Struct;

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
static VoltageSense_Struct voltage_sense[NUM_VOLTAGE_SENSE_PINS] = {
    INIT_VOLTAGE_SENSE(
        _12V_SUPPLY,
        GLV_VOLTAGE,
        PDM_12V_VBAT_STDID,
        PDM_12V_VBAT_DLC),
    INIT_VOLTAGE_SENSE(
        VBAT_SUPPLY,
        VBAT_VOLTAGE,
        PDM_12V_VBAT_STDID,
        PDM_12V_VBAT_DLC),
    INIT_VOLTAGE_SENSE(
        FLYWIRE,
        ADC1_IN10_TO_12V_ACC_RATIO,
        PDM_FLYWIRE_STDID,
        PDM_FLYWIRE_DLC),
};

/******************************************************************************
 * Private Function Prototypes
 *******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void VoltageSense_ConvertVoltageAdcReadings(void)
{
    for (VoltageSenseIndex_Enum i = 0, j = ADC_READINGS_VOLTAGE_START_INDEX;
         i < NUM_VOLTAGE_SENSE_PINS; i++, j++)
    {
        // Convert ADC readings to voltage values
        float32_t temp_voltage = (float32_t)(SharedAdc_GetAdcVoltage(j)) *
                                 voltage_sense[i].voltage_scale /
                                 (float32_t)(SharedAdc_GetActualVdda());

        // Apply low pass filter to current values
        SharedFilters_LowPassFilter(
            &temp_voltage, &voltage_sense[i].voltage,
            VOLTAGE_IIR_LPF_SAMPLING_PERIOD, VOLTAGE_IIR_LPF_RC);
    }
}

void VoltageSense_TransmitVoltage(void)
{
    // Transmit CAN data
    uint8_t data[8];
    memcpy(&data[0], (uint8_t *)(&voltage_sense[_12V_SUPPLY].voltage), sizeof(float32_t));
    memcpy(&data[4], (uint8_t *)(&voltage_sense[VBAT_SUPPLY].voltage), sizeof(float32_t));
    SharedCan_TransmitDataCan(
        voltage_sense[_12V_SUPPLY].can.std_id,
        voltage_sense[_12V_SUPPLY].can.dlc,
        &data[0]);

    memset(&data[0], 0, sizeof(data));
    memcpy(&data[0], (uint8_t *)(&voltage_sense[FLYWIRE].voltage), sizeof(float32_t));
    SharedCan_TransmitDataCan(
        voltage_sense[FLYWIRE].can.std_id,
        voltage_sense[FLYWIRE].can.dlc,
        &data[0]);
}