#include "tasks.h"
#include "chimera_v2.hpp"

class BMSChimeraConfig final : public chimera_v2::config
{
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override
    {
        if (gnn->which_name != gpio_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", gpio_net_name_tag, gnn->which_name);
            return std::nullopt;
        }
        switch (gnn->name.bms_net_name)
        {
            case bms_GpioNetName_GPIO_LED_B:
                return std::cref(hw::gpio::led_b);
            case bms_GpioNetName_GPIO_BSPD_OK_CURRENT_STATUS:
                return std::cref(hw::gpio::bspd_ok_current_status);
            case bms_GpioNetName_GPIO_ACCEL_BRAKE_OK:
                return std::cref(hw::gpio::accel_brake_ok);
            case bms_GpioNetName_GPIO_N_BRAKE_PRESS_3V3:
                return std::cref(hw::gpio::n_brake_press_3v3);
            case bms_GpioNetName_GPIO_N_HIGH_CURRENT_BSPD:
                return std::cref(hw::gpio::n_high_current_bspd);
            case bms_GpioNetName_GPIO_MSD_SHDN_SNS:
                return std::cref(hw::gpio::msd_shdn_sns);
            case bms_GpioNetName_GPIO_HV_P_INTLCK_SNS:
                return std::cref(hw::gpio::hv_p_intlck_sns);
            case bms_GpioNetName_GPIO_HV_N_INTLCK_SNS:
                return std::cref(hw::gpio::hv_n_intlck_sns);
            case bms_GpioNetName_GPIO_IMD_LATCH:
                return std::cref(hw::gpio::imd_latch);
            case bms_GpioNetName_GPIO_DIAG_TS:
                return std::cref(hw::gpio::diag_ts);
            case bms_GpioNetName_GPIO_SD_CD:
                return std::cref(hw::gpio::sd_cd);
            case bms_GpioNetName_GPIO_LED_R:
                return std::cref(hw::gpio::led_r);
            case bms_GpioNetName_GPIO_SPI_CS_LS:
                return std::cref(hw::gpio::spi_cs_ls);
            case bms_GpioNetName_GPIO_N_EVSE_I_LIM:
                return std::cref(hw::gpio::n_evse_i_lim);
            case bms_GpioNetName_GPIO_IR_P_EN:
                return std::cref(hw::gpio::ir_p_en);
            case bms_GpioNetName_GPIO_SHDN_EN:
                return std::cref(hw::gpio::shdn_en);
            case bms_GpioNetName_GPIO_BMS_LATCH:
                return std::cref(hw::gpio::bms_latch_status);
            case bms_GpioNetName_GPIO_IMD_OK_CURRENT_STATUS:
                return std::cref(hw::gpio::imd_ok_current_status);
            case bms_GpioNetName_GPIO_BSPD_OK_LATCH_STATUS:
                return std::cref(hw::gpio::bspd_ok_latch_status);
            case bms_GpioNetName_GPIO_PRECHARGE_EN:
                return std::cref(hw::gpio::precharge_en);
            case bms_GpioNetName_GPIO_SHDN_OL_ON:
                return std::cref(hw::gpio::shdn_ol_on);
            case bms_GpioNetName_GPIO_FAN_EN:
                return std::cref(hw::gpio::fan_en);
            case bms_GpioNetName_GPIO_FAN_PGOOD:
                return std::cref(hw::gpio::fan_pgood);
            default:
            case bms_GpioNetName_GPIO_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified GPIO net name");
                return std::nullopt;
        }
    }
    std::optional<std::reference_wrapper<const hw::Adc>> id_to_adc(const _AdcNetName *ann) const override
    {
        if (ann->which_name != adc_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected ADC net name with tag %d, got %d", adc_net_name_tag, ann->which_name);
            return std::nullopt;
        }
        switch (ann->name.bms_net_name)
        {
            case bms_AdcNetName_ADC_FAN_ISNS:
                return std::cref(hw::adcs::fan_isns);
            case bms_AdcNetName_ADC_TS_VSENSE_P:
                return std::cref(hw::adcs::ts_vsense_p);
            case bms_AdcNetName_ADC_TS_VSENSE_N:
                return std::cref(hw::adcs::ts_vsense_n);
            case bms_AdcNetName_ADC_SHDN_ISNS:
                return std::cref(hw::adcs::shdn_isns);
            case bms_AdcNetName_ADC_EMETER_THERM_SNS:
                return std::cref(hw::adcs::emeter_tsns);
            case bms_AdcNetName_ADC_TS_ISNS_400A:
                return std::cref(hw::adcs::ts_isns_400a);
            case bms_AdcNetName_ADC_TS_ISNS_50A:
                return std::cref(hw::adcs::ts_isns_50a);
            default:
            case bms_AdcNetName_ADC_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified ADC net name");
                return std::nullopt;
        }
    }

    std::optional<std::reference_wrapper<const hw::spi::SpiDevice>> id_to_spi(const _SpiNetName *snn) const override
    {
        if (snn->which_name != spi_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected SPI net name with tag %d, got %d", spi_net_name_tag, snn->which_name);
            return std::nullopt;
        }
        switch (snn->name.bms_net_name)
        {
            case bms_SpiNetName_SPI_ADBMS_SPI_LS:
                return std::cref(hw::spi::adbms_spi_ls);
            case bms_SpiNetName_SPI_ADBMS_SPI_HS:
                return std::cref(hw::spi::adbms_spi_hs);
            default:
            case bms_SpiNetName_SPI_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified SPI net name");
                return std::nullopt;
        }
    }

  public:
    ~BMSChimeraConfig() override = default;
    BMSChimeraConfig()
    {
        gpio_net_name_tag = GpioNetName_bms_net_name_tag;
        adc_net_name_tag  = AdcNetName_bms_net_name_tag;
        spi_net_name_tag  = SpiNetName_bms_net_name_tag;
    }
} bms_config;

void tasks_preInit() {}

void tasks_init()
{
    chimera_v2::task(bms_config);
}
