#include "hw_gpios.h"
#include "main.h"

namespace hw::gpio
{
const Gpio led_b                  = { .port = LEDB_GPIO_Port, .pin = LEDB_Pin };
const Gpio bspd_ok_current_status = { .port = BSPD_OK_GPIO_Port, .pin = BSPD_OK_Pin };
const Gpio accel_brake_ok         = { .port = ACCEL_BRAKE_OK_GPIO_Port, .pin = ACCEL_BRAKE_OK_Pin };
const Gpio n_brake_press_3v3      = { .port = nBRAKE_PRESS_3V3_GPIO_Port, .pin = nBRAKE_PRESS_3V3_Pin };
const Gpio n_high_current_bspd    = { .port = nHIGH_CURRENT_BSPD_GPIO_Port, .pin = nHIGH_CURRENT_BSPD_Pin };
const Gpio msd_shdn_sns           = { .port = MSD_SHDN_SNS_GPIO_Port, .pin = MSD_SHDN_SNS_Pin };
const Gpio hv_p_intlck_sns        = { .port = HV_P_INTLCK_SNS_GPIO_Port, .pin = HV_P_INTLCK_SNS_Pin };
const Gpio hv_n_intlck_sns        = { .port = HV_N_INTLCK_SNS_GPIO_Port, .pin = HV_N_INTLCK_SNS_Pin };

// Update: IMD_LATCH changed to nIMD_LATCH in new design
const Gpio imd_latch = { .port = nIMD_LATCH_GPIO_Port, .pin = nIMD_LATCH_Pin };

const Gpio diag_ts = { .port = DIAG_GPIO_Port, .pin = DIAG_Pin };
const Gpio sd_cd   = { .port = SD_CD_GPIO_Port, .pin = SD_CD_Pin };

// Update: LED changed to LEDR (Red LED on PC13)
const Gpio led_r = { .port = LEDR_GPIO_Port, .pin = LEDR_Pin };

// Update: Renamed to SPI_C_LS (note that schematic says SPI_CS_LS, but matching code with main.h)
const Gpio spi_cs_ls = { .port = SPI_C_LS_GPIO_Port, .pin = SPI_C_LS_Pin };

const Gpio tsense_sel0           = { .port = TSENSE_SEL0_GPIO_Port, .pin = TSENSE_SEL0_Pin };
const Gpio tsense_sel1           = { .port = TSENSE_SEL1_GPIO_Port, .pin = TSENSE_SEL1_Pin };
const Gpio tsense_sel2           = { .port = TSENSE_SEL2_GPIO_Port, .pin = TSENSE_SEL2_Pin };
const Gpio bspd_test_enable      = { .port = BSPD_TEST_EN_GPIO_Port, .pin = BSPD_TEST_EN_Pin };
const Gpio ts_isense_ocsc_ok     = { .port = TS_ISENSE_OCSC_OK_3V3_GPIO_Port, .pin = TS_ISENSE_OCSC_OK_3V3_Pin };
const Gpio spi_cs_hs             = { .port = SPI_CS_HS_GPIO_Port, .pin = SPI_CS_HS_Pin };
const Gpio bms_ok_current_status = { .port = BMS_OK_GPIO_Port, .pin = BMS_OK_Pin };
const Gpio n_evse_i_lim          = { .port = nEVSE_I_LIM_PWM_GPIO_Port, .pin = nEVSE_I_LIM_PWM_Pin };
const Gpio ir_p_en               = { .port = IR_P_EN_GPIO_Port, .pin = IR_P_EN_Pin };
const Gpio shdn_en               = { .port = SHDN_EN_GPIO_Port, .pin = SHDN_EN_Pin };
// Update: BMS_LATCH changed to nBMS_LATCH (n means active low I think)
const Gpio bms_latch_status = { .port = nBMS_LATCH_GPIO_Port, .pin = nBMS_LATCH_Pin };

const Gpio imd_ok_current_status = { .port = IMD_OK_GPIO_Port, .pin = IMD_OK_Pin };
// Update: BSPD_LATCH changed to nBSPD_LATCH
const Gpio bspd_ok_latch_status = { .port = nBSPD_LATCH_GPIO_Port, .pin = nBSPD_LATCH_Pin };

const Gpio precharge_en = { .port = PRE_CHARGE_EN_GPIO_Port, .pin = PRE_CHARGE_EN_Pin };

// Update: Prev FB_STBY_SHDN mapped to SHDN_OL_ON (Shutdown Open Loop On) at PE13
const Gpio shdn_ol_on = { .port = SHDN_OL_ON_GPIO_Port, .pin = SHDN_OL_ON_Pin };

const Gpio fan_en    = { .port = FAN_EN_GPIO_Port, .pin = FAN_EN_Pin };
const Gpio fan_pgood = { .port = FAN_PGOOD_GPIO_Port, .pin = FAN_PGOOD_Pin };
} // namespace hw::gpio