#include "hw_gpios.hpp"
#include "main.h"

using hw::gpio;

const gpio buzzer_pwr_en_pin(BUZZER_PWR_EN_GPIO_Port, BUZZER_PWR_EN_Pin);
const gpio tsim_red_en_pin(TSIM_RED_EN_GPIO_Port, TSIM_RED_EN_Pin);
const gpio ntsim_green_en_pin(nTSIM_GRN_EN_GPIO_Port, nTSIM_GRN_EN_Pin);
const gpio r_shdn_sense_pin(R_SHDN_SENSE_GPIO_Port, R_SHDN_SENSE_Pin);
const gpio l_shdn_sense_pin(L_SHDN_SENSE_GPIO_Port, L_SHDN_SENSE_Pin);
const gpio led_pin(LED_GPIO_Port, LED_Pin);
const gpio boot_pin(BOOT_GPIO_Port, BOOT_Pin);
const gpio ntp_pin(NTP_GPIO_Port, NTP_Pin);
const gpio sd_present(SD_CD_GPIO_Port, SD_CD_Pin);
const gpio _900m_pin(_900M_GPIO_GPIO_Port, _900M_GPIO_Pin);
const gpio sd_fail_pin(SD_FAIL_GPIO_Port, SD_FAIL_Pin);
const gpio d_p_pullup_pin(D_P_PULLUP_GPIO_Port, D_P_PULLUP_Pin);
