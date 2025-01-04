#include "shared.pb.h"
#include "hw_gpios.h"
#include "hw_adc.h"
#include "hw_uarts.h"

const Gpio *id_to_gpio[] = { [VC_GpioNetName_BUZZER_PWR_EN]    = &buzzer_pwr_en,
                             [VC_GpioNetName_BAT_I_SNS_NFLT]   = &bat_i_sns_nflt,
                             [VC_GpioNetName_LED]              = &led_gpio,
                             [VC_GpioNetName_TELEM_PWR_EN]     = &telem_pwr_en,
                             [VC_GpioNetName_NPCM_EN]          = &npcm_en,
                             [VC_GpioNetName_ACC_I_SENSE_NFLT] = &acc_i_sns_nflt,
                             [VC_GpioNetName_PGOOD]            = &pgood,
                             [VC_GpioNetName_LV_PWR_EN]        = &lv_pwr_en,
                             [VC_GpioNetName_AUX_PWR_EN]       = &aux_pwr_en,
                             [VC_GpioNetName_PUMP_PWR_EN]      = &pump_pwr_en,
                             [VC_GpioNetName__900K_GPIO]       = &_900k_gpio,
                             [VC_GpioNetName_NCHRG_FAULT]      = &nchrg_fault,
                             [VC_GpioNetName_NCHRG]            = &nchrg,
                             [VC_GpioNetName_INV_L_PWR_EN]     = &inv_l_pwr_en,
                             [VC_GpioNetName_INV_R_PWR_EN]     = &inv_r_pwr_en,
                             [VC_GpioNetName_SHDN_PWR_EN]      = &shdn_pwr_en,
                             [VC_GpioNetName_FR_STBY1]         = &fr_stby1,
                             [VC_GpioNetName_FR_STBY2]         = &fr_stby2,
                             [VC_GpioNetName_FR_STBY3]         = &fr_stby3,
                             [VC_GpioNetName_INV_L_PROGRAM]    = &inv_l_program,
                             [VC_GpioNetName_INV_R_PROGRAM]    = &inv_r_program,
                             [VC_GpioNetName_L_SHDN_SNS]       = &l_shdn_sns,
                             [VC_GpioNetName_R_SHDN_SNS]       = &r_shdn_sns,
                             [VC_GpioNetName_NCHIMERA]         = &n_chimera_pin,
                             [VC_GpioNetName_NPROGRAM_3V3]     = &nprogram_3v3,
                             [VC_GpioNetName_SB_ILCK_SHDN_SNS] = &sb_ilck_shdn_sns,
                             [VC_GpioNetName_TSMS_SHDN_SNS]    = &tsms_shdn_sns };

const AdcChannel id_to_adc[] = {
    [VC_AdcNetName_INV_R_PWR_I_SNS]  = ADC1_IN10_INV_R_PWR_I_SNS,
    [VC_AdcNetName_INV_L_PWR_I_SNS]  = ADC1_IN11_INV_L_PWR_I_SNS,
    [VC_AdcNetName_AUX_PWR_I_SNS]    = ADC3_IN0_AUX_PWR_I_SNS,
    [VC_AdcNetName_SHDN_PWR_I_SNS]   = ADC1_IN18_SHDN_PWR_I_SNS,
    [VC_AdcNetName_VBAT_SENSE]       = ADC1_IN19_VBAT_SENSE,
    [VC_AdcNetName__24V_ACC_SENSE]   = ADC1_IN3_24V_ACC_SENSE,
    [VC_AdcNetName__22V_BOOST_SENSE] = ADC1_IN7_22V_BOOST_SENSE,
    [VC_AdcNetName_LV_PWR_I_SNS]     = ADC1_IN4_LV_PWR_I_SNS,
    [VC_AdcNetName_ACC_I_SENSE]      = ADC1_IN5_ACC_I_SENSE,
    [VC_AdcNetName_PUMP_PWR_I_SNS]   = ADC3_IN1_PUMP_PWR_I_SNS,
};

const UART *chimera_uart   = &debug_uart;
const Gpio *n_chimera_gpio = &n_chimera_pin;