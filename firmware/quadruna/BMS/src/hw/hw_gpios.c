#include "hw_gpios.h"
#include "main.h"

const Gpio accel_brake_ok_pin          = { .port = ACCEL_BRAKE_OK_3V3_GPIO_Port, .pin = ACCEL_BRAKE_OK_3V3_Pin };
const Gpio led_pin                     = { .port = LED_GPIO_Port, .pin = LED_Pin };
const Gpio n_chimera_pin               = { .port = nCHIMERA_GPIO_Port, .pin = nCHIMERA_Pin };
const Gpio n_program_pin               = { .port = nPROGRAM_3V3_GPIO_Port, .pin = nPROGRAM_3V3_Pin };
const Gpio ts_ilck_shdn_pin            = { .port = TS_ILCK_SHDN_OK_GPIO_Port, .pin = TS_ILCK_SHDN_OK_Pin };
const Gpio hvd_ok_shdn_pin             = { .port = HVD_SHDN_OK_GPIO_Port, .pin = HVD_SHDN_OK_Pin };
const Gpio ts_isense_ocsc_ok_pin       = { .port = TS_ISENSE_OCSC_OK_3V3_GPIO_Port, .pin = TS_ISENSE_OCSC_OK_3V3_Pin };
const Gpio sd_cd_pin                   = { .port = SD_CD_GPIO_Port, .pin = SD_CD_Pin };
const Gpio spi_cs_pin                  = { .port = SPI_CS_GPIO_Port, .pin = SPI_CS_Pin };
const Gpio air_p_gpio                  = { .port = AIR_P_EN_GPIO_Port, .pin = AIR_P_EN_Pin };
const Gpio air_n_gpio                  = { .port = HVD_SHDN_OK_GPIO_Port, .pin = HVD_SHDN_OK_Pin };
const Gpio precharge_gpio              = { .port = PRE_CHARGE_EN_GPIO_Port, .pin = PRE_CHARGE_EN_Pin };
const Gpio mux_0_gpio                  = { .port = AUX_TSENSE_MUX0_GPIO_Port, .pin = AUX_TSENSE_MUX0_Pin };
const Gpio mux_1_gpio                  = { .port = AUX_TSENSE_MUX1_GPIO_Port, .pin = AUX_TSENSE_MUX1_Pin };
const Gpio mux_2_gpio                  = { .port = AUX_TSENSE_MUX2_GPIO_Port, .pin = AUX_TSENSE_MUX2_Pin };
const Gpio mux_3_gpio                  = { .port = AUX_TSENSE_MUX3_GPIO_Port, .pin = AUX_TSENSE_MUX3_Pin };
const Gpio bms_ok_current_status_gpio  = { .port = BMS_OK_3V3_GPIO_Port, .pin = BMS_OK_3V3_Pin };
const Gpio bms_ok_latch_status_gpio    = { .port = BMS_LATCH_GPIO_Port, .pin = BMS_LATCH_Pin };
const Gpio imd_ok_current_status_gpio  = { .port = IMD_OK_3V3_GPIO_Port, .pin = IMD_OK_3V3_Pin };
const Gpio imd_ok_latch_status_gpio    = { .port = IMD_LATCH_GPIO_Port, .pin = IMD_LATCH_Pin };
const Gpio bspd_ok_current_status_gpio = { .port = BSPD_OK_3V3_GPIO_Port, .pin = BSPD_OK_3V3_Pin };
const Gpio bspd_ok_latch_status_gpio   = { .port = BSPD_LATCH_GPIO_Port, .pin = BSPD_LATCH_Pin };
const Gpio test_enable_gpio            = { .port = BSPD_TEST_EN_GPIO_Port, .pin = BSPD_TEST_EN_Pin };
const Gpio n_high_current_gpio = { .port = nHIGH_CURRENT_BSPD_3V3_GPIO_Port, .pin = nHIGH_CURRENT_BSPD_3V3_Pin };
const Gpio present_gpio        = { .port = SD_CD_GPIO_Port, .pin = SD_CD_Pin };