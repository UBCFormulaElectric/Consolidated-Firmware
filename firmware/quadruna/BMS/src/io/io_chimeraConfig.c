#include "hw_gpios.h"
#include "hw_adcs.h"
#include "hw_uarts.h"
#include "shared.pb.h"

const Gpio *id_to_gpio[] = { [BMS_GpioNetName_ACCEL_BRAKE_OK_3V3]     = &accel_brake_ok_pin,
                             [BMS_GpioNetName_AIR_P_EN]               = &air_p_gpio,
                             [BMS_GpioNetName_AUX_TSENSE_MUX0]        = &mux_0_gpio,
                             [BMS_GpioNetName_AUX_TSENSE_MUX1]        = &mux_1_gpio,
                             [BMS_GpioNetName_AUX_TSENSE_MUX2]        = &mux_2_gpio,
                             [BMS_GpioNetName_AUX_TSENSE_MUX3]        = &mux_3_gpio,
                             [BMS_GpioNetName_BMS_LATCH]              = &bms_ok_latch_status_gpio,
                             [BMS_GpioNetName_BMS_OK_3V3]             = &bms_ok_current_status_gpio,
                             [BMS_GpioNetName_BSPD_LATCH]             = &bspd_ok_latch_status_gpio,
                             [BMS_GpioNetName_BSPD_OK_3V3]            = &bspd_ok_current_status_gpio,
                             [BMS_GpioNetName_BSPD_TEST_EN]           = &test_enable_gpio,
                             [BMS_GpioNetName_HVD_SHDN_OK]            = &air_n_gpio,
                             [BMS_GpioNetName_IMD_LATCH]              = &imd_ok_latch_status_gpio,
                             [BMS_GpioNetName_IMD_OK_3V3]             = &imd_ok_current_status_gpio,
                             [BMS_GpioNetName_LED]                    = &led_pin,
                             [BMS_GpioNetName_NCHIMERA]               = &n_chimera_pin,
                             [BMS_GpioNetName_NHIGH_CURRENT_BSPD_3V3] = &n_high_current_gpio,
                             [BMS_GpioNetName_NPROGRAM_3V3]           = &n_program_pin,
                             [BMS_GpioNetName_PRE_CHARGE_EN]          = &precharge_gpio,
                             [BMS_GpioNetName_TS_ILCK_SHDN_OK]        = &ts_ilck_shdn_pin,
                             [BMS_GpioNetName_TS_ISENSE_OCSC_OK_3V3]  = &ts_isense_ocsc_ok_pin,
                             [BMS_GpioNetName_SD_CD]                  = &sd_cd_pin,
                             [BMS_GpioNetName_SPI_CS]                 = &spi_cs_pin };

const AdcChannel *id_to_adc[] = {
    [BMS_AdcNetName_AUX_TSENSE] = &aux_tsns,       [BMS_AdcNetName_TS_ISENSE_400A] = &ts_isns_400a,
    [BMS_AdcNetName_TS_ISENSE_75A] = &ts_isns_75a, [BMS_AdcNetName_TS_VSENSE_P] = &ts_vsense_p,
    [BMS_AdcNetName_TS_VSENSE_N] = &ts_vsense_n,
};

const Gpio *n_chimera_gpio = &n_chimera_pin;
