#include "tasks.h"
#include "chimera_v2.hpp"
#include "hw_gpios.hpp"
#include "hw_adcs.hpp"
#include "hw_spis.hpp"
#include "hw_usb.hpp"
#include "hw_i2cs.hpp"
#include <bms.pb.h>
#include "hw_rtosTaskHandler.hpp"

#include <cassert>
#include <optional>
#include <functional>

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
            case bms_GpioNetName_GPIO_LEDB:
                return std::cref(hw::gpio::led_b);
            case bms_GpioNetName_GPIO_BSPD_OK:
                return std::cref(hw::gpio::bspd_ok_current_status);
            case bms_GpioNetName_GPIO_ACCEL_BRAKE_OK:
                return std::cref(hw::gpio::accel_brake_ok);
            case bms_GpioNetName_GPIO_nBRAKE_PRESS_3V3:
                return std::cref(hw::gpio::n_brake_press_3v3);
            case bms_GpioNetName_GPIO_nHIGH_CURRENT_BSPD:
                return std::cref(hw::gpio::n_high_current_bspd);
            case bms_GpioNetName_GPIO_MSD_SHDN_SNS:
                return std::cref(hw::gpio::msd_shdn_sns);
            case bms_GpioNetName_GPIO_HV_P_INTLCK_SNS:
                return std::cref(hw::gpio::hv_p_intlck_sns);
            case bms_GpioNetName_GPIO_HV_N_INTLCK_SNS:
                return std::cref(hw::gpio::hv_n_intlck_sns);
            case bms_GpioNetName_GPIO_IMD_LATCH:
                return std::cref(hw::gpio::imd_latch);
            case bms_GpioNetName_GPIO_DIAG:
                return std::cref(hw::gpio::diag_ts);
            case bms_GpioNetName_GPIO_SD_CD:
                return std::cref(hw::gpio::sd_cd);
            case bms_GpioNetName_GPIO_LEDR:
                return std::cref(hw::gpio::led_r);
            case bms_GpioNetName_GPIO_SPI_CS_LS:
                return std::cref(hw::gpio::spi_cs_ls);
            case bms_GpioNetName_GPIO_TSENSE_SEL0:
                return std::cref(hw::gpio::tsense_sel0);
            case bms_GpioNetName_GPIO_TSENSE_SEL1:
                return std::cref(hw::gpio::tsense_sel1);
            case bms_GpioNetName_GPIO_TSENSE_SEL2:
                return std::cref(hw::gpio::tsense_sel2);
            case bms_GpioNetName_GPIO_BSPD_TEST_EN:
                return std::cref(hw::gpio::bspd_test_enable);
            case bms_GpioNetName_GPIO_TS_ISENSE_OCSC_OK_3V3:
                return std::cref(hw::gpio::ts_isense_ocsc_ok);
            case bms_GpioNetName_GPIO_SPI_CS_HS:
                return std::cref(hw::gpio::spi_cs_hs);
            case bms_GpioNetName_GPIO_BMS_OK:
                return std::cref(hw::gpio::bms_ok_current_status);
            case bms_GpioNetName_GPIO_nEVSE_I_LIM_GPIO:
                return std::cref(hw::gpio::n_evse_i_lim);
            case bms_GpioNetName_GPIO_IR_P_EN:
                return std::cref(hw::gpio::ir_p_en);
            case bms_GpioNetName_GPIO_SHDN_EN:
                return std::cref(hw::gpio::shdn_en);
            case bms_GpioNetName_GPIO_nBMS_LATCH:
                return std::cref(hw::gpio::n_bms_latch_status);
            case bms_GpioNetName_GPIO_IMD_OK:
                return std::cref(hw::gpio::imd_ok_current_status);
            case bms_GpioNetName_GPIO_nBSPD_LATCH:
                return std::cref(hw::gpio::n_bspd_latch_status);
            case bms_GpioNetName_GPIO_PRE_CHARGE_EN:
                return std::cref(hw::gpio::precharge_en);
            case bms_GpioNetName_GPIO_SHDN_OL_ON:
                return std::cref(hw::gpio::shdn_ol_on);
            case bms_GpioNetName_GPIO_FAN_EN:
                return std::cref(hw::gpio::fan_en);
            case bms_GpioNetName_GPIO_FAN_PGOOD:
                return std::cref(hw::gpio::fan_pgood);
            case bms_GpioNetName_GPIO_PGOOD_3V3:
                return std::cref(hw::gpio::pgood_3v3);
            case bms_GpioNetName_GPIO_PGOOD_5V:
                return std::cref(hw::gpio::pgood_5v);
            case bms_GpioNetName_GPIO_SHDN_DIAG_EN:
                return std::cref(hw::gpio::shdn_diag_en);
            case bms_GpioNetName_GPIO_SHDN_FAULT:
                return std::cref(hw::gpio::shdn_diag_en);
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
                return std::cref(hw::adc::fan_isns);
            case bms_AdcNetName_ADC_TS_VSENSE_P:
                return std::cref(hw::adc::ts_vsense_p);
            case bms_AdcNetName_ADC_TS_VSENSE_N:
                return std::cref(hw::adc::ts_vsense_n);
            case bms_AdcNetName_ADC_SHDN_SNS:
                return std::cref(hw::adc::shdn_sns);
            case bms_AdcNetName_ADC_EMETER_THERM_SNS:
                return std::cref(hw::adc::emeter_tsns);
            case bms_AdcNetName_ADC_TS_ISENSE_400A:
                return std::cref(hw::adc::ts_isense_400a);
            case bms_AdcNetName_ADC_TS_ISENSE_50A:
                return std::cref(hw::adc::ts_isense_50a);
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
            case bms_SpiNetName_SPI_LTC6813_LS:
                return std::cref(hw::spi::adbms_spi_ls);
            case bms_SpiNetName_SPI_LTC6813_HS:
                return std::cref(hw::spi::adbms_spi_hs);
            default:
            case bms_SpiNetName_SPI_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified SPI net name");
                return std::nullopt;
        }
    }

    std::optional<std::reference_wrapper<const hw::i2c::I2CDevice>> id_to_i2c(const _I2cNetName *inn) const override
    {
        return std::nullopt;
    }

  public:
    ~BMSChimeraConfig() override = default;
    BMSChimeraConfig()
    {
        gpio_net_name_tag = GpioNetName_bms_net_name_tag;
        adc_net_name_tag  = AdcNetName_bms_net_name_tag;
        spi_net_name_tag  = SpiNetName_bms_net_name_tag;
        // i2c_net_name_tag  = I2cNetName_bms_net_name_tag;
    }
} bms_config;

static hw::rtos::StaticTask<8096>
    TaskChimera(osPriorityRealtime, "TaskChimera", [](void *) { chimera_v2::task(bms_config); });


void tasks_preInit() {}
char USBD_PRODUCT_STRING_FS[] = "bms";

[[noreturn]] void tasks_init()
{
    assert(hw::usb::init());
    osKernelInitialize();
    TaskChimera.start();
    osKernelStart();
    forever {}
}
