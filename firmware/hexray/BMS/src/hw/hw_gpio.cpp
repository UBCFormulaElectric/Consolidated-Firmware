#include "hw_gpios.h"
#include "main.h"

const Gpio led_b_pin                  = { .port = LEDB_GPIO_Port,                 .pin = LEDB_Pin };
const Gpio bspd_ok_current_status_pin = { .port = BSPD_OK_GPIO_Port,              .pin = BSPD_OK_Pin };
const Gpio accel_brake_ok_pin         = { .port = ACCEL_BRAKE_OK_GPIO_Port,       .pin = ACCEL_BRAKE_OK_Pin };
const Gpio n_brake_press_3v3_pin      = { .port = nBRAKE_PRESS_3V3_GPIO_Port,     .pin = nBRAKE_PRESS_3V3_Pin };
const Gpio n_high_current_bspd_pin    = { .port = nHIGH_CURRENT_BSPD_GPIO_Port,   .pin = nHIGH_CURRENT_BSPD_Pin };
const Gpio msd_shdn_sns_pin           = { .port = MSD_SHDN_SNS_GPIO_Port,         .pin = MSD_SHDN_SNS_Pin };
const Gpio hv_p_intlck_sns_pin        = { .port = HV_P_INTLCK_SNS_GPIO_Port,      .pin = HV_P_INTLCK_SNS_Pin };
const Gpio hv_n_intlck_sns_pin        = { .port = HV_N_INTLCK_SNS_GPIO_Port,      .pin = HV_N_INTLCK_SNS_Pin };

// Update: IMD_LATCH changed to nIMD_LATCH in new design
const Gpio imd_latch_pin              = { .port = nIMD_LATCH_GPIO_Port,           .pin = nIMD_LATCH_Pin };

const Gpio diag_ts_pin                = { .port = DIAG_GPIO_Port,                 .pin = DIAG_Pin };
const Gpio sd_cd_pin                  = { .port = SD_CD_GPIO_Port,                .pin = SD_CD_Pin };

// Update: LED changed to LEDR (Red LED on PC13)
const Gpio led_r_pin                  = { .port = LEDR_GPIO_Port,                 .pin = LEDR_Pin };

// Update: Renamed to SPI_C_LS (note that schematic says SPI_CS_LS, but matching code with main.h)
const Gpio spi_cs_ls_pin              = { .port = SPI_C_LS_GPIO_Port,             .pin = SPI_C_LS_Pin };

const Gpio tsense_sel0_pin            = { .port = TSENSE_SEL0_GPIO_Port,          .pin = TSENSE_SEL0_Pin };
const Gpio tsense_sel1_pin            = { .port = TSENSE_SEL1_GPIO_Port,          .pin = TSENSE_SEL1_Pin };
const Gpio tsense_sel2_pin            = { .port = TSENSE_SEL2_GPIO_Port,          .pin = TSENSE_SEL2_Pin };
const Gpio bspd_test_enable_pin       = { .port = BSPD_TEST_EN_GPIO_Port,         .pin = BSPD_TEST_EN_Pin };
const Gpio ts_isense_ocsc_ok_pin      = { .port = TS_ISENSE_OCSC_OK_3V3_GPIO_Port, .pin = TS_ISENSE_OCSC_OK_3V3_Pin };
const Gpio spi_cs_hs_pin              = { .port = SPI_CS_HS_GPIO_Port,            .pin = SPI_CS_HS_Pin };
const Gpio bms_ok_current_status_pin  = { .port = BMS_OK_GPIO_Port,               .pin = BMS_OK_Pin };
const Gpio n_evse_i_lim_pin           = { .port = nEVSE_I_LIM_GPIO_GPIO_Port,     .pin = nEVSE_I_LIM_GPIO_Pin };
const Gpio ir_p_en_pin                = { .port = IR_P_EN_GPIO_Port,              .pin = IR_P_EN_Pin };
const Gpio shdn_en_pin                = { .port = SHDN_EN_GPIO_Port,              .pin = SHDN_EN_Pin };

// Update: BMS_LATCH changed to nBMS_LATCH (n means active low I think)
const Gpio bms_latch_status_pin       = { .port = nBMS_LATCH_GPIO_Port,           .pin = nBMS_LATCH_Pin };

const Gpio imd_ok_current_status_pin  = { .port = IMD_OK_GPIO_Port,               .pin = IMD_OK_Pin };

// Update: BSPD_LATCH changed to nBSPD_LATCH
const Gpio bspd_ok_latch_status_pin   = { .port = nBSPD_LATCH_GPIO_Port,          .pin = nBSPD_LATCH_Pin };

const Gpio precharge_en_pin           = { .port = PRE_CHARGE_EN_GPIO_Port,        .pin = PRE_CHARGE_EN_Pin };

// Update: Prev FB_STBY_SHDN mapped to SHDN_OL_ON (Shutdown Open Loop On) at PE13
const Gpio shdn_ol_on_pin             = { .port = SHDN_OL_ON_GPIO_Port,           .pin = SHDN_OL_ON_Pin };

const Gpio fan_en_pin                 = { .port = FAN_EN_GPIO_Port,               .pin = FAN_EN_Pin };
const Gpio fan_pgood_pin              = { .port = FAN_PGOOD_GPIO_Port,            .pin = FAN_PGOOD_Pin };