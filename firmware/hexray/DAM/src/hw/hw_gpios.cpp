#include "hw_gpios.hpp"

extern "C"
{
#include "main.h"
}

using hw::Gpio;

const Gpio buzzer_pwr_en_pin(BUZZER_PWR_EN_GPIO_Port, BUZZER_PWR_EN_Pin);
const Gpio tsim_red_en_pin(TSIM_RED_EN_GPIO_Port, TSIM_RED_EN_Pin);
const Gpio ntsim_green_en_pin(nTSIM_GRN_EN_GPIO_Port, nTSIM_GRN_EN_Pin);
const Gpio r_shdn_sense_pin(R_SHDN_SENSE_GPIO_Port, R_SHDN_SENSE_Pin);
const Gpio l_shdn_sense_pin(L_SHDN_SENSE_GPIO_Port, L_SHDN_SENSE_Pin);
const Gpio led_pin(LED_GPIO_Port, LED_Pin);
const Gpio boot_pin(BOOT_GPIO_Port, BOOT_Pin);
const Gpio ntp_pin(NTP_GPIO_Port, NTP_Pin);
const Gpio sd_present(SD_CD_GPIO_Port, SD_CD_Pin);
const Gpio _900m_pin(_900M_GPIO_GPIO_Port, _900M_GPIO_Pin);
const Gpio sd_fail_pin(SD_FAIL_GPIO_Port, SD_FAIL_Pin);
const Gpio d_p_pullup_pin(D_P_PULLUP_GPIO_Port, D_P_PULLUP_Pin);
