#pragma once
#include <stdlib.h>
#include "pb.h"
#include "hw_adc.h"
#include "hw_gpio.h"
#include "hw_i2c.h"

extern bool io_chimera_v2_enabled;

extern const Gpio *id_to_gpio[] = {
    [GPIO_LEDB]                     = &led_b_pin,
    [GPIO_BSPD_OK]                  = &bspd_ok_current_status_pin,
    [GPIO_ACCEL_BRAKE_OK]           = &accel_brake_ok_pin,
    [GPIO_nBRAKE_PRESS_3V3]         = &n_brake_press_3v3_pin,
    [GPIO_nHIGH_CURRENT_BSPD]       = &n_high_current_bspd_pin,
    [GPIO_MSD_SHDN_SNS]             = &msd_shdn_sns_pin,
    [GPIO_HV_P_INTLCK_SNS]          = &hv_p_intlck_sns_pin,
    [GPIO_HV_N_INTLCK_SNS]          = &hv_n_intlck_sns_pin,
    [GPIO_IMD_LATCH]                = &imd_latch_pin,
    [GPIO_DIAG]                     = &diag_ts_pin,
    [GPIO_SD_CD]                    = &sd_cd_pin,
    [GPIO_LEDR]                     = &led_r_pin,
    [GPIO_SPI_CS_LS]                = &spi_cs_ls_pin,
    [GPIO_TSENSE_SEL0]              = &tsense_sel0_pin,
    [GPIO_TSENSE_SEL1]              = &tsense_sel1_pin,
    [GPIO_TSENSE_SEL2]              = &tsense_sel2_pin,
    [GPIO_BSPD_TEST_EN]             = &bspd_test_enable_pin,
    [GPIO_TS_ISENSE_OCSC_OK_3V3]    = &ts_isense_ocsc_ok_pin,
    [GPIO_SPI_CS_HS]                = &spi_cs_hs_pin,
    [GPIO_BMS_OK]                   = &bms_ok_current_status_pin,
    [GPIO_nEVSE_I_LIM_GPIO]         = &n_evse_i_lim_pin,
    [GPIO_IR_P_EN]                  = &ir_p_en_pin,
    [GPIO_SHDN_EN]                  = &shdn_en_pin,
    [GPIO_BMS_LATCH]                = &bms_latch_status_pin,
    [GPIO_IMD_OK]                   = &imd_ok_current_status_pin,
    [GPIO_BSPD_LATCH]               = &bspd_ok_latch_status_pin,
    [GPIO_PRE_CHARGE_EN]            = &precharge_en_pin,
    [GPIO_FB_STBY_SHDN]             = &fb_stby_shdn_pin,
    [GPIO_FAN_EN]                   = &fan_en_pin,
    [GPIO_FAN_PGOOD]                = &fan_pgood_pin
};

extern const AdcChannel *id_to_adc[] = {
    [ADC_FAN_ISNS]            = &fan_isns,
    [ADC_TS_VSENSE_P]         = &ts_vsense_p,
    [ADC_TS_VSENSE_N]         = &ts_vsense_n,
    [ADC_SHDN_ISNS]           = &shdn_isns,
    [ADC_EMETER_THERM_SNS]    = &emeter_tsns,
    [ADC_TS_ISNS_400A]        = &ts_isns_400a,
    [ADC_TS_ISNS_50A]         = &ts_isns_50a
};

extern const I2cDevice  *id_to_i2c[] = { [bms_I2cNetName_I2C_NET_NAME_UNSPECIFIED] = NULL };



/**
 * @brief The main Chimera loop.
 * @param gpio_tag The Protobuf-generated tag for the board's GPIO pins, defined in shared.pb.h.
 * @param id_to_gpio A table of Protobuf-generated net names to Gpio pins.
 * @param adc_tag The Protobuf-generated tag for the board's ADC channels, defined in shared.pb.h.
 * @param id_to_adc A table of Protobuf-generated net names to ADC channels.
 * @param i2c_tag The Protobuf-generated tag for the board's I2C devices, defined in shared.pb.h.
 * @param id_to_i2c A table of Protobuf-generated net names to I2C busses.
 */
void io_chimera_v2_main(
    pb_size_t         gpio_tag,
    const Gpio       *id_to_gpio[],
    pb_size_t         adc_tag,
    const AdcChannel *id_to_adc[],
    pb_size_t         i2c_tag,
    const I2cDevice  *id_to_i2c[]);