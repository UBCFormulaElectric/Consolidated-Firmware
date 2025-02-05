#include "hw_gpios.h"

#include "main.h"

const Gpio led_gpio         = { .port = LED_GPIO_Port, .pin = LED_Pin };
const Gpio sd_present       = { .pin = SD_CD_Pin, .port = SD_CD_GPIO_Port };
const Gpio buzzer_pwr_en    = { .port = BUZZER_PWR_EN_GPIO_Port, .pin = BUZZER_PWR_EN_Pin };
const Gpio bat_i_sns_nflt   = { .port = BAT_I_SNS_nFLT_GPIO_Port, .pin = BAT_I_SNS_nFLT_Pin };
const Gpio telem_pwr_en     = { .port = TELEM_PWR_EN_GPIO_Port, .pin = TELEM_PWR_EN_Pin };
const Gpio npcm_en          = { .port = nPCM_EN_GPIO_Port, .pin = nPCM_EN_Pin };
const Gpio acc_i_sns_nflt   = { .port = ACC_I_SENSE_nFLT_GPIO_Port, .pin = ACC_I_SENSE_nFLT_Pin };
const Gpio pgood            = { .port = PGOOD_GPIO_Port, .pin = PGOOD_Pin };
const Gpio lv_pwr_en        = { .port = LV_PWR_EN_GPIO_Port, .pin = LV_PWR_EN_Pin };
const Gpio aux_pwr_en       = { .port = AUX_PWR_EN_GPIO_Port, .pin = AUX_PWR_EN_Pin };
const Gpio pump_pwr_en      = { .port = PUMP_PWR_EN_GPIO_Port, .pin = PUMP_PWR_EN_Pin };
const Gpio _900k_gpio       = { .port = _900K_GPIO_GPIO_Port, .pin = _900K_GPIO_Pin };
const Gpio nchrg_fault      = { .port = nCHRG_FAULT_GPIO_Port, .pin = nCHRG_FAULT_Pin };
const Gpio nchrg            = { .port = nCHRG_GPIO_Port, .pin = nCHRG_Pin };
const Gpio inv_l_pwr_en     = { .port = INV_L_PWR_EN_GPIO_Port, .pin = INV_L_PWR_EN_Pin };
const Gpio inv_r_pwr_en     = { .port = INV_R_PWR_EN_GPIO_Port, .pin = INV_R_PWR_EN_Pin };
const Gpio shdn_pwr_en      = { .port = SHDN_PWR_EN_GPIO_Port, .pin = SHDN_PWR_EN_Pin };
const Gpio fr_stby1         = { .port = FR_STBY1_GPIO_Port, .pin = FR_STBY1_Pin };
const Gpio fr_stby2         = { .port = FR_STBY2_GPIO_Port, .pin = FR_STBY2_Pin };
const Gpio fr_stby3         = { .port = FR_STBY3_GPIO_Port, .pin = FR_STBY3_Pin };
const Gpio inv_l_program    = { .port = INV_L_PROGRAM_GPIO_Port, .pin = INV_L_PROGRAM_Pin };
const Gpio inv_r_program    = { .port = INV_R_PROGRAM_GPIO_Port, .pin = INV_R_PROGRAM_Pin };
const Gpio l_shdn_sns       = { .port = L_SHDN_SNS_GPIO_Port, .pin = L_SHDN_SNS_Pin };
const Gpio r_shdn_sns       = { .port = R_SHDN_SNS_GPIO_Port, .pin = R_SHDN_SNS_Pin };
const Gpio n_chimera_pin    = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
const Gpio nprogram_3v3     = { .port = NPROGRAM_3V3_GPIO_Port, .pin = NPROGRAM_3V3_Pin };
const Gpio sb_ilck_shdn_sns = { .port = SB_ILCK_SHDN_SNS_GPIO_Port, .pin = SB_ILCK_SHDN_SNS_Pin };
const Gpio tsms_shdn_sns    = { .port = TSMS_SHDN_SNS_GPIO_Port, .pin = TSMS_SHDN_SNS_Pin };
