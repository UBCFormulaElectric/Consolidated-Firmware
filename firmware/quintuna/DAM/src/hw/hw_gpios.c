#include "hw_gpios.h"
#include "main.h"

const Gpio telem_pwr_en_pin   = { TELEM_PWR_EN_GPIO_Port, TELEM_PWR_EN_Pin };
const Gpio buzzer_pwr_en_pin  = { BUZZER_PWR_EN_GPIO_Port, BUZZER_PWR_EN_Pin };
const Gpio tsim_red_en_pin    = { TSIM_RED_EN_GPIO_Port, TSIM_RED_EN_Pin };
const Gpio ntsim_green_en_pin = { nTSIM_GRN_EN_GPIO_Port, nTSIM_GRN_EN_Pin };
const Gpio r_shdn_sense_pin   = { SPLITTER_SHDN_SENSE_GPIO_Port, SPLITTER_SHDN_SENSE_Pin };
const Gpio l_shdn_sense_pin   = { L_SHDN_SENSE_GPIO_Port, L_SHDN_SENSE_Pin };
const Gpio led_pin            = { LED_GPIO_Port, LED_Pin };
const Gpio boot_pin           = { BOOT_LED_GPIO_Port, BOOT_LED_Pin };
const Gpio sd_present         = { SD_CD_GPIO_Port, SD_CD_Pin };
const Gpio n_rtc_int1_pin     = { nRTC_INT1_GPIO_Port, nRTC_INT1_Pin };
const Gpio _900m_pin          = { _900M_GPIO_GPIO_Port, _900M_GPIO_Pin };
const Gpio sd_fail_pin        = { SD_FAIL_GPIO_Port, SD_FAIL_Pin };