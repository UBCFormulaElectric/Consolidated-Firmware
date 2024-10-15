#include "hw_gpios.h"
#include "main.h"
#include "app_utils.h"

namespace hw::gpio
{
// Power control GPIOs
const Gpio buzzer_pwr_en{ BUZZER_PWR_EN_GPIO_Port, BUZZER_PWR_EN_Pin };
const Gpio telem_pwr_en{ TELEM_PWR_EN_GPIO_Port, TELEM_PWR_EN_Pin };
const Gpio npcm_en{ nPCM_EN_GPIO_Port, nPCM_EN_Pin };
const Gpio lv_pwr_en{ LV_PWR_EN_GPIO_Port, LV_PWR_EN_Pin };
const Gpio aux_pwr_en{ AUX_PWR_EN_GPIO_Port, AUX_PWR_EN_Pin };
const Gpio pump_pwr_en{ PUMP_PWR_EN_GPIO_Port, PUMP_PWR_EN_Pin };
const Gpio inv_l_pwr_en{ INV_L_PWR_EN_GPIO_Port, INV_L_PWR_EN_Pin };
const Gpio inv_r_pwr_en{ INV_R_PWR_EN_GPIO_Port, INV_R_PWR_EN_Pin };
const Gpio shdn_pwr_en{ SHDN_PWR_EN_GPIO_Port, SHDN_PWR_EN_Pin };
const Gpio fr_stby1{ FR_STBY1_GPIO_Port, FR_STBY1_Pin };
const Gpio fr_stby2{ FR_STBY2_GPIO_Port, FR_STBY2_Pin };
const Gpio fr_stby3{ FR_STBY3_GPIO_Port, FR_STBY3_Pin };

// Sensor GPIOs
const Gpio bat_i_sns_nflt{ BAT_I_SNS_nFLT_GPIO_Port, BAT_I_SNS_nFLT_Pin };
const Gpio acc_i_sns_nflt{ ACC_I_SENSE_nFLT_GPIO_Port, ACC_I_SENSE_nFLT_Pin };
const Gpio pgood{ PGOOD_GPIO_Port, PGOOD_Pin };
const Gpio l_shdn_sns{ L_SHDN_SNS_GPIO_Port, L_SHDN_SNS_Pin };
const Gpio r_shdn_sns{ R_SHDN_SNS_GPIO_Port, R_SHDN_SNS_Pin };
const Gpio tsms_shdn_sns{ TSMS_SHDN_SNS_GPIO_Port, TSMS_SHDN_SNS_Pin };

// Special GPIOs
Gpio       sd_present;
const Gpio _900k_gpio{ _900K_GPIO_GPIO_Port, _900K_GPIO_Pin };
const Gpio nchrg_fault{ nCHRG_FAULT_GPIO_Port, nCHRG_FAULT_Pin };
const Gpio nchrg{ nCHRG_GPIO_Port, nCHRG_Pin };
const Gpio n_chimera_pin{ NCHIMERA_GPIO_Port, NCHIMERA_Pin };
const Gpio nprogram_3v3{ NPROGRAM_3V3_GPIO_Port, NPROGRAM_3V3_Pin };
const Gpio sb_ilck_shdn_s{ SB_ILCK_SHDN_SNS_GPIO_Port, SB_ILCK_SHDN_SNS_Pin };
} // namespace hw::gpio
