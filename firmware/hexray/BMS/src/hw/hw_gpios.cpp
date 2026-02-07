#include "hw_gpios.hpp"
#include "main.h"

namespace hw::gpio
{
const Gpio led_b                  ( LEDB_GPIO_Port, LEDB_Pin );
const Gpio shdn_diag_en           ( SHDN_DIAG_EN_GPIO_Port, SHDN_DIAG_EN_Pin );
const Gpio bspd_ok_current_status ( BSPD_OK_GPIO_Port, BSPD_OK_Pin );
const Gpio accel_brake_ok         ( ACCEL_BRAKE_OK_GPIO_Port, ACCEL_BRAKE_OK_Pin );
const Gpio n_brake_press_3v3      ( nBRAKE_PRESS_3V3_GPIO_Port, nBRAKE_PRESS_3V3_Pin );
const Gpio n_high_current_bspd    ( nHIGH_CURRENT_BSPD_GPIO_Port, nHIGH_CURRENT_BSPD_Pin );
const Gpio msd_shdn_sns           ( MSD_SHDN_SNS_GPIO_Port, MSD_SHDN_SNS_Pin );
const Gpio hv_p_intlck_sns        ( HV_P_INTLCK_SNS_GPIO_Port, HV_P_INTLCK_SNS_Pin );
const Gpio hv_n_intlck_sns        ( HV_N_INTLCK_SNS_GPIO_Port, HV_N_INTLCK_SNS_Pin );

// Update: IMD_LATCH changed to nIMD_LATCH in new design
const Gpio imd_latch ( nIMD_LATCH_GPIO_Port, nIMD_LATCH_Pin );

const Gpio diag_ts ( DIAG_GPIO_Port, DIAG_Pin );
const Gpio sd_cd   ( SD_CD_GPIO_Port, SD_CD_Pin );

// Update: LED changed to LEDR (Red LED on PC13)
const Gpio led_r ( LEDR_GPIO_Port, LEDR_Pin );

// Update: Renamed to SPI_C_LS (note that schematic says SPI_CS_LS, but matching code with main.h)
const Gpio spi_cs_ls ( SPI_CS_LS_GPIO_Port, SPI_CS_LS_Pin );

const Gpio tsense_sel0           ( TSENSE_SEL0_GPIO_Port, TSENSE_SEL0_Pin );
const Gpio tsense_sel1           ( TSENSE_SEL1_GPIO_Port, TSENSE_SEL1_Pin );
const Gpio tsense_sel2           ( TSENSE_SEL2_GPIO_Port, TSENSE_SEL2_Pin );
const Gpio bspd_test_enable      ( BSPD_TEST_EN_GPIO_Port, BSPD_TEST_EN_Pin );
const Gpio ts_isense_ocsc_ok     ( TS_ISENSE_OCSC_OK_3V3_GPIO_Port, TS_ISENSE_OCSC_OK_3V3_Pin );
const Gpio spi_cs_hs             ( SPI_CS_HS_GPIO_Port, SPI_CS_HS_Pin );
const Gpio bms_ok_current_status ( BMS_OK_GPIO_Port, BMS_OK_Pin );
const Gpio n_evse_i_lim          ( nEVSE_I_LIM_PWM_GPIO_Port, nEVSE_I_LIM_PWM_Pin );
const Gpio ir_p_en               ( IR_P_EN_GPIO_Port, IR_P_EN_Pin );
const Gpio shdn_en               ( SHDN_EN_GPIO_Port, SHDN_EN_Pin );
const Gpio shdn_fault            ( SHDN_FAULT_GPIO_Port, SHDN_FAULT_Pin );

// Update: BMS_LATCH changed to nBMS_LATCH (n means active low I think)
const Gpio n_bms_latch_status ( nBMS_LATCH_GPIO_Port, nBMS_LATCH_Pin );
const Gpio imd_ok_current_status ( IMD_OK_GPIO_Port, IMD_OK_Pin );

// Update: BSPD_LATCH changed to nBSPD_LATCH
const Gpio n_bspd_latch_status ( nBSPD_LATCH_GPIO_Port, nBSPD_LATCH_Pin );
const Gpio precharge_en ( PRE_CHARGE_EN_GPIO_Port, PRE_CHARGE_EN_Pin );

// Update: Prev FB_STBY_SHDN mapped to SHDN_OL_ON (Shutdown Open Loop On) at PE13
const Gpio shdn_ol_on ( SHDN_OL_ON_GPIO_Port, SHDN_OL_ON_Pin );
const Gpio fan_en    ( FAN_EN_GPIO_Port, FAN_EN_Pin );
const Gpio fan_pgood ( FAN_PGOOD_GPIO_Port, FAN_PGOOD_Pin );
const Gpio pgood_3v3 ( PGOOD_3V3_GPIO_Port, PGOOD_3V3_Pin );
const Gpio pgood_5v  ( PGOOD_5V_GPIO_Port, PGOOD_5V_Pin );

} // namespace hw::gpio