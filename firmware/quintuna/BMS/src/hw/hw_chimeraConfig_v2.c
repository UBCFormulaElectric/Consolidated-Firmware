#include "bms.pb.h"
#include "shared.pb.h"
#include "hw_chimeraConfig_v2.h"

#include "hw_adcs.h"
#include "hw_gpios.h"
#include "hw_spis.h"

const Gpio *id_to_gpio[] = { [bms_GpioNetName_GPIO_LEDB]                  = &led_b_pin,
                             [bms_GpioNetName_GPIO_BSPD_OK]               = &bspd_ok_current_status_pin,
                             [bms_GpioNetName_GPIO_ACCEL_BRAKE_OK]        = &accel_brake_ok_pin,
                             [bms_GpioNetName_GPIO_nBRAKE_PRESS_3V3]      = &n_brake_press_3v3_pin,
                             [bms_GpioNetName_GPIO_nHIGH_CURRENT_BSPD]    = &n_high_current_bspd_pin,
                             [bms_GpioNetName_GPIO_MSD_SHDN_SNS]          = &msd_shdn_sns_pin,
                             [bms_GpioNetName_GPIO_HV_P_INTLCK_SNS]       = &hv_p_intlck_sns_pin,
                             [bms_GpioNetName_GPIO_HV_N_INTLCK_SNS]       = &hv_n_intlck_sns_pin,
                             [bms_GpioNetName_GPIO_IMD_LATCH]             = &imd_latch_pin,
                             [bms_GpioNetName_GPIO_DIAG]                  = &diag_ts_pin,
                             [bms_GpioNetName_GPIO_SD_CD]                 = &sd_cd_pin,
                             [bms_GpioNetName_GPIO_LEDR]                  = &led_r_pin,
                             [bms_GpioNetName_GPIO_SPI_CS_LS]             = &spi_cs_ls_pin,
                             [bms_GpioNetName_GPIO_TSENSE_SEL0]           = &tsense_sel0_pin,
                             [bms_GpioNetName_GPIO_TSENSE_SEL1]           = &tsense_sel1_pin,
                             [bms_GpioNetName_GPIO_TSENSE_SEL2]           = &tsense_sel2_pin,
                             [bms_GpioNetName_GPIO_BSPD_TEST_EN]          = &bspd_test_enable_pin,
                             [bms_GpioNetName_GPIO_TS_ISENSE_OCSC_OK_3V3] = &ts_isense_ocsc_ok_pin,
                             [bms_GpioNetName_GPIO_SPI_CS_HS]             = &spi_cs_hs_pin,
                             [bms_GpioNetName_GPIO_BMS_OK]                = &bms_ok_current_status_pin,
                             [bms_GpioNetName_GPIO_nEVSE_I_LIM_GPIO]      = &n_evse_i_lim_pin,
                             [bms_GpioNetName_GPIO_IR_P_EN]               = &ir_p_en_pin,
                             [bms_GpioNetName_GPIO_SHDN_EN]               = &shdn_en_pin,
                             [bms_GpioNetName_GPIO_BMS_LATCH]             = &bms_latch_status_pin,
                             [bms_GpioNetName_GPIO_IMD_OK]                = &imd_ok_current_status_pin,
                             [bms_GpioNetName_GPIO_BSPD_LATCH]            = &bspd_ok_latch_status_pin,
                             [bms_GpioNetName_GPIO_PRE_CHARGE_EN]         = &precharge_en_pin,
                             [bms_GpioNetName_GPIO_FB_STBY_SHDN]          = &fb_stby_shdn_pin,
                             [bms_GpioNetName_GPIO_FAN_EN]                = &fan_en_pin,
                             [bms_GpioNetName_GPIO_FAN_PGOOD]             = &fan_pgood_pin };

const AdcChannel *id_to_adc[] = { [bms_AdcNetName_ADC_FAN_ISNS]         = &fan_isns,
                                  [bms_AdcNetName_ADC_TS_VSENSE_P]      = &ts_vsense_p,
                                  [bms_AdcNetName_ADC_TS_VSENSE_N]      = &ts_vsense_n,
                                  [bms_AdcNetName_ADC_SHDN_ISNS]        = &shdn_isns,
                                  [bms_AdcNetName_ADC_EMETER_THERM_SNS] = &emeter_tsns,
                                  [bms_AdcNetName_ADC_TS_ISNS_400A]     = &ts_isns_400a,
                                  [bms_AdcNetName_ADC_TS_ISNS_50A]      = &ts_isns_50a };

const SpiDevice *id_to_spi[] = {
    [bms_SpiNetName_SPI_LTC6813_HS] = &ltc6813_spi_hs,
    [bms_SpiNetName_SPI_LTC6813_LS] = &ltc6813_spi_ls,
};

hw_chimera_v2_Config chimera_v2_config = { .gpio_net_name_tag = GpioNetName_bms_net_name_tag,
                                           .id_to_gpio        = id_to_gpio,
                                           .adc_net_name_tag  = AdcNetName_bms_net_name_tag,
                                           .id_to_adc         = id_to_adc,
                                           .spi_net_name_tag  = SpiNetName_bms_net_name_tag,
                                           .id_to_spi         = id_to_spi };