/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Profet2.h"
#include "ErrorHandling.h"
#include "SharedMacros.h"
#include "CanDefinitions.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/
#define INIT_PROFET2(                                                         \
    index, efuse0_pin, efuse0_port, efuse0_ampere_per_volt,                   \
    efuse0_max_retries, efuse1_pin, efuse1_port, efuse1_ampere_per_volt,      \
    efuse1_max_retries, dsel_pin, dsel_port, den_pin, den_port, can_stdid,    \
    can_dlc)                                                                  \
    [index].efuse[SENSE_0].input_channel.pin  = efuse0_pin,                   \
    [index].efuse[SENSE_0].input_channel.port = efuse0_port,                  \
    [index].efuse[SENSE_0].current            = RESET,                        \
    [index].efuse[SENSE_0].ampere_per_volt    = efuse0_ampere_per_volt,       \
    [index].efuse[SENSE_0].fault.current_num_of_retries = RESET,              \
    [index].efuse[SENSE_0].fault.max_num_of_retries     = efuse0_max_retries, \
    [index].efuse[SENSE_1].input_channel.pin            = efuse1_pin,         \
    [index].efuse[SENSE_1].input_channel.port           = efuse1_port,        \
    [index].efuse[SENSE_1].current                      = RESET,              \
    [index].efuse[SENSE_1].ampere_per_volt = efuse1_ampere_per_volt,          \
    [index].efuse[SENSE_1].fault.current_num_of_retries = RESET,              \
    [index].efuse[SENSE_1].fault.max_num_of_retries     = efuse1_max_retries, \
    [index].dsel_pin_mapping.pin                        = dsel_pin,           \
    [index].dsel_pin_mapping.port                       = dsel_port,          \
    [index].den_pin_mapping.pin                         = den_pin,            \
    [index].den_pin_mapping.port                        = den_port,           \

    // [index].can.stdid                                   = can_stdid,          \
    // [index].can.dlc                                     = can_dlc,            \

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
static Profet2_Struct profet2[NUM_PROFET2S] = {
    INIT_PROFET2(
        AUX1_AUX2,
        EFUSE_AUX_1_IN_Pin,
        EFUSE_AUX_1_IN_GPIO_Port,
        AMP_PER_VOLT_AUX,
        AUX1_MAX_NUM_OF_RETRIES,
        EFUSE_AUX_2_IN_Pin,
        EFUSE_AUX_2_IN_GPIO_Port,
        AMP_PER_VOLT_AUX,
        AUX2_MAX_NUM_OF_RETRIES,
        EFUSE_DSEL_1_Pin,
        EFUSE_DSEL_1_GPIO_Port,
        EFUSE_DEN_1_Pin,
        EFUSE_DEN_1_GPIO_Port,
        PDM_AUX1_AUX2_CURRENT_STDID,
        PDM_AUX1_AUX2_CURRENT_DLC),
    INIT_PROFET2(
        COOLING_PDMFAN,
        EFUSE_COOLING_IN_Pin,
        EFUSE_COOLING_IN_GPIO_Port,
        AMP_PER_VOLT,
        COOLING_MAX_NUM_OF_RETRIES,
        EFUSE_PDM_FAN_IN_Pin,
        EFUSE_PDM_FAN_IN_GPIO_Port,
        AMP_PER_VOLT,
        PDMFAN_MAX_NUM_OF_RETRIES,
        EFUSE_DSEL_2_Pin,
        EFUSE_DSEL_2_GPIO_Port,
        EFUSE_DEN_2_Pin,
        EFUSE_DEN_2_GPIO_Port,
        PDM_COOLING_PDMFAN_CURRENT_STDID,
        PDM_COOLING_PDMFAN_CURRENT_DLC),
    INIT_PROFET2(
        AIRSHDN_CANGLV,
        EFUSE_AIR_SHDN_IN_Pin,
        EFUSE_AIR_SHDN_IN_GPIO_Port,
        AMP_PER_VOLT,
        AIRSHDN_MAX_NUM_OF_RETRIES,
        EFUSE_CAN_IN_Pin,
        EFUSE_CAN_IN_GPIO_Port,
        AMP_PER_VOLT,
        CANGLV_MAX_NUM_OF_RETRIES,
        EFUSE_DSEL_3_Pin,
        EFUSE_DSEL_3_GPIO_Port,
        EFUSE_DEN_3_Pin,
        EFUSE_DEN_3_GPIO_Port,
        PDM_AIRSHDN_CANGLV_CURRENT_STDID,
        PDM_AIRSHDN_CANGLV_CURRENT_DLC),
    INIT_PROFET2(
        ACCSEGMENTFAN_ACCENCLOSUREFAN,
        EFUSE_ACC_SEG_FAN_IN_Pin,
        EFUSE_ACC_SEG_FAN_IN_GPIO_Port,
        AMP_PER_VOLT,
        ACCSEGFAN_MAX_NUM_OF_RETRIES,
        EFUSE_ACC_ENC_FAN_IN_Pin,
        EFUSE_ACC_ENC_FAN_IN_GPIO_Port,
        AMP_PER_VOLT,
        ACCENCFAN_MAX_NUM_OF_RETRIES,
        EFUSE_DSEL_4_Pin,
        EFUSE_DSEL_4_GPIO_Port,
        EFUSE_DEN_4_Pin,
        EFUSE_DEN_4_GPIO_Port,
        PDM_ACCUMULATOR_FAN_CURRENT_STDID,
        PDM_ACCUMULATOR_FAN_CURRENT_DLC),
    INIT_PROFET2(
        LEFTINVERTER_RIGHTINVERTER,
        EFUSE_LEFT_INVERTER_IN_Pin,
        EFUSE_LEFT_INVERTER_IN_GPIO_Port,
        AMP_PER_VOLT,
        LEFTINVERTER_MAX_NUM_OF_RETRIES,
        EFUSE_RIGHT_INVERTER_IN_Pin,
        EFUSE_RIGHT_INVERTER_IN_GPIO_Port,
        AMP_PER_VOLT,
        RIGHTINVERTER_MAX_NUM_OF_RETRIES,
        EFUSE_DSEL_5_Pin,
        EFUSE_DSEL_5_GPIO_Port,
        EFUSE_DEN_5_Pin,
        EFUSE_DEN_5_GPIO_Port,
        PDM_INVERTER_IO_CURRENT_STDID,
        PDM_INVERTER_IO_CURRENT_DLC),
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
void Profet2_ConfigureSingleEfuse(
    Efuse_Struct *           efuse,
    EfuseOnOff_GPIO_PinState state)
{
    SharedGpio_GPIO_WritePin(
        efuse->input_channel.port, efuse->input_channel.pin, state);
}

void Profet2_ConfigureSingleDen(
    Profet2_Struct *       profet2,
    DenOnOff_GPIO_PinState state)
{
    SharedGpio_GPIO_WritePin(
        profet2->den_pin_mapping.port, profet2->den_pin_mapping.pin, state);
}

void Profet2_ConfigureAllDens(DenOnOff_GPIO_PinState state)
{
    for (Profet2Index_Enum i = 0; i < ARRAY_SIZE(profet2); i++)
    {
        SharedGpio_GPIO_WritePin(
            profet2[i].den_pin_mapping.port, profet2[i].den_pin_mapping.pin,
            state);
    }
}

void Profet2_ConfigureAllDsels(DselOnOff_GPIO_PinState state)
{
    for (Profet2Index_Enum i = 0; i < ARRAY_SIZE(profet2); i++)
    {
        SharedGpio_GPIO_WritePin(
            profet2[i].dsel_pin_mapping.port, profet2[i].dsel_pin_mapping.pin,
            state);
    }
}

Profet2_Struct *const Profet2_GetProfet2s(void)
{
    return (Profet2_Struct *const)(&profet2[0]);
}
