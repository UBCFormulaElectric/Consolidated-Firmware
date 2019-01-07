/******************************************************************************
 * Includes
 ******************************************************************************/
//TODO: UPDATE INCLUDE(S)
#include "Profet2.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/
/* https://www.infineon.com/dgdl/Infineon-BTS7008-2EPA-DS-v01_00-EN.pdf?fileId=5546d46258fc0bc101590212356876b1 */
#define INIT_PROFET2(                                                   \
    index, efuse0_pin, efuse0_port, efuse0_ampere_per_volt, efuse1_pin, \
    efuse1_port, efuse1_ampere_per_volt, dsel_pin, dsel_port, den_pin,  \
    den_port)                                                           \
    [index].efuse[SENSE_0].input_channel.pin  = efuse0_pin,             \
    [index].efuse[SENSE_0].input_channel.port = efuse0_port,            \
    [index].efuse[SENSE_0].current            = 0,                      \
    [index].efuse[SENSE_0].ampere_per_volt    = efuse0_ampere_per_volt, \
    [index].efuse[SENSE_1].input_channel.pin  = efuse1_pin,             \
    [index].efuse[SENSE_1].input_channel.port = efuse1_port,            \
    [index].efuse[SENSE_1].current            = 0,                      \
    [index].efuse[SENSE_1].ampere_per_volt    = efuse1_ampere_per_volt, \
    [index].dsel_pin_mapping.pin              = dsel_pin,               \
    [index].dsel_pin_mapping.port             = dsel_port,              \
    [index].den_pin_mapping.pin               = den_pin,                \
    [index].den_pin_mapping.port              = den_port

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
        EFUSE_AUX_2_IN_Pin,
        EFUSE_AUX_2_IN_GPIO_Port,
        AMP_PER_VOLT_AUX,
        EFUSE_DSEL_1_Pin,
        EFUSE_DSEL_1_GPIO_Port,
        EFUSE_DEN_1_Pin,
        EFUSE_DEN_1_GPIO_Port),
    INIT_PROFET2(
        COOLING_PDMFAN,
        EFUSE_COOLING_IN_Pin,
        EFUSE_COOLING_IN_GPIO_Port,
        AMP_PER_VOLT,
        EFUSE_PDM_FAN_IN_Pin,
        EFUSE_PDM_FAN_IN_GPIO_Port,
        AMP_PER_VOLT,
        EFUSE_DSEL_2_Pin,
        EFUSE_DSEL_2_GPIO_Port,
        EFUSE_DEN_2_Pin,
        EFUSE_DEN_2_GPIO_Port),
    INIT_PROFET2(
        AIRSHDN_CANGLV,
        EFUSE_AIR_SHDN_IN_Pin,
        EFUSE_AIR_SHDN_IN_GPIO_Port,
        AMP_PER_VOLT,
        EFUSE_CAN_IN_Pin,
        EFUSE_CAN_IN_GPIO_Port,
        AMP_PER_VOLT,
        EFUSE_DSEL_3_Pin,
        EFUSE_DSEL_3_GPIO_Port,
        EFUSE_DEN_3_Pin,
        EFUSE_DEN_3_GPIO_Port),
    INIT_PROFET2(
        ACCSEGMENTFAN_ACCENCLOSUREFAN,
        EFUSE_ACC_SEG_FAN_IN_Pin,
        EFUSE_ACC_SEG_FAN_IN_GPIO_Port,
        AMP_PER_VOLT,
        EFUSE_ACC_ENC_FAN_IN_Pin,
        EFUSE_ACC_ENC_FAN_IN_GPIO_Port,
        AMP_PER_VOLT,
        EFUSE_DSEL_4_Pin,
        EFUSE_DSEL_4_GPIO_Port,
        EFUSE_DEN_4_Pin,
        EFUSE_DEN_4_GPIO_Port),
    INIT_PROFET2(
        LEFTINVERTER_RIGHTINVERTER,
        EFUSE_LEFT_INVERTER_IN_Pin,
        EFUSE_LEFT_INVERTER_IN_GPIO_Port,
        AMP_PER_VOLT,
        EFUSE_RIGHT_INVERTER_IN_Pin,
        EFUSE_RIGHT_INVERTER_IN_GPIO_Port,
        AMP_PER_VOLT,
        EFUSE_DSEL_5_Pin,
        EFUSE_DSEL_5_GPIO_Port,
        EFUSE_DEN_5_Pin,
        EFUSE_DEN_5_GPIO_Port),
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
    efuse_struct *           efuse,
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

void Profet2_ConfigureAllDsels(DselOnOff_GPIO_PinState state)
{
    // TODO: Test that the sizeof works and this function works in general
    for (Profet2Index_Enum i = 0; i < sizeof(profet2) / sizeof(profet2[0]); i++)
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
