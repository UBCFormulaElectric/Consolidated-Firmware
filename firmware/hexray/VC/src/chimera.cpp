#include "tasks.h"
#include "chimera_v2.hpp"
#include "hw_gpios.hpp"
#include "hw_spis.hpp"
#include "hw_adcs.hpp"
#include "hw_i2cs.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "shared.pb.h"
#include "io_log.hpp"
#include "hw_usb.hpp"
#include "hw_hardFaultHandler.hpp"

#include <cassert>
#include <vc.pb.h>

#include <optional>
#include <functional>

class VCChimeraConfig : public chimera_v2::config
{
  public:
    ~VCChimeraConfig() override = default;

    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override
    {
        if (gnn->which_name != gpio_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", gpio_net_name_tag, gnn->which_name);
            return std::nullopt;
        }

        switch (gnn->name.vc_net_name)
        {
            case vc_GpioNetName_GPIO_F_INV_EN:
                return std::cref(f_inv_en);
            case vc_GpioNetName_GPIO_F_INV_PG:
                return std::cref(f_inv_pg);
            case vc_GpioNetName_GPIO_R_INV_EN:
                return std::cref(r_inv_en);
            case vc_GpioNetName_GPIO_R_INV_PG:
                return std::cref(r_inv_pg);
            case vc_GpioNetName_GPIO_DAM_EN:
                return std::cref(dam_en);
            case vc_GpioNetName_GPIO_DAM_PG:
                return std::cref(dam_pg);
            case vc_GpioNetName_GPIO_BMS_EN:
                return std::cref(bms_en);
            case vc_GpioNetName_GPIO_BMS_PG:
                return std::cref(bms_pg);
            case vc_GpioNetName_GPIO_BOOST_PG:
                return std::cref(boost_pg);
            case vc_GpioNetName_GPIO_RSM_EN:
                return std::cref(rsm_en);
            case vc_GpioNetName_GPIO_RSM_PG:
                return std::cref(rsm_pg);
            case vc_GpioNetName_GPIO_RR_PUMP_EN:
                return std::cref(rr_pump_en);
            case vc_GpioNetName_GPIO_RR_PUMP_PGOOD:
                return std::cref(rr_pump_pgood);
            case vc_GpioNetName_GPIO_RL_PUMP_EN:
                return std::cref(rl_pump_en);
            case vc_GpioNetName_GPIO_RL_PUMP_PGOOD:
                return std::cref(rl_pump_pgood);
            case vc_GpioNetName_GPIO_PCM_EN:
                return std::cref(pcm_en);
            case vc_GpioNetName_GPIO_L_RAD_FAN_EN:
                return std::cref(l_rad_fan_en);
            case vc_GpioNetName_GPIO_L_RAD_FAN_PG:
                return std::cref(l_rad_fan_pg);
            case vc_GpioNetName_GPIO_R_RAD_FAN_EN:
                return std::cref(r_rad_fan_en);
            case vc_GpioNetName_GPIO_R_RAD_FAN_PG:
                return std::cref(r_rad_fan_pg);
            case vc_GpioNetName_GPIO_FRONT_EN:
                return std::cref(front_en);
            case vc_GpioNetName_GPIO_FRONT_PG:
                return std::cref(front_pg);
            case vc_GpioNetName_GPIO_SB_SHDN_3V3_OUT:
                return std::cref(sb_shdn_3v3_out);
            case vc_GpioNetName_GPIO_BAT_CHRG_MODE:
                return std::cref(bat_chrg_mode);
            case vc_GpioNetName_GPIO_BAT_CHRG_nSHDN:
                return std::cref(bat_chrg_nSHDN);
            case vc_GpioNetName_GPIO_IMU_FSYNC:
                return std::cref(imu_fsync);
            case vc_GpioNetName_GPIO_PWR_MTR_nALERT:
                return std::cref(pwr_mtr_nalert);
            case vc_GpioNetName_GPIO_BOOT:
                return std::cref(boot);
            case vc_GpioNetName_GPIO_LED:
                return std::cref(led);
            case vc_GpioNetName_GPIO_RR_ILCK_3V3_OUT:
                return std::cref(rr_ilck_3v3_out);
            case vc_GpioNetName_GPIO_TSMS_3V3_OUT:
                return std::cref(tsms_3v3_out);
            case vc_GpioNetName_GPIO_IMU_CS1:
                return std::cref(imu_cs1);
            case vc_GpioNetName_GPIO_IMU_CS2:
                return std::cref(imu_cs2);
            case vc_GpioNetName_GPIO_IMU_CS3:
                return std::cref(imu_cs3);
            default:
            case vc_GpioNetName_GPIO_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unknown GPIO net name");
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

        switch (ann->name.vc_net_name)
        {
            case vc_AdcNetName_ADC_RR_PUMP:
                return std::cref(hw::adcs::adc_rr_pump);
            case vc_AdcNetName_ADC_DAM:
                return std::cref(hw::adcs::adc_dam);
            case vc_AdcNetName_ADC_L_RAD_FAN:
                return std::cref(hw::adcs::adc_l_rad_fan);
            case vc_AdcNetName_ADC_R_RAD_FAN:
                return std::cref(hw::adcs::adc_r_rad_fan);
            case vc_AdcNetName_ADC_RL_PUMP:
                return std::cref(hw::adcs::adc_rl_pump);
            case vc_AdcNetName_ADC_F_INV:
                return std::cref(hw::adcs::adc_f_inv);
            case vc_AdcNetName_ADC_R_INV:
                return std::cref(hw::adcs::adc_r_inv);
            case vc_AdcNetName_ADC_BMS:
                return std::cref(hw::adcs::adc_bms);
            case vc_AdcNetName_ADC_FRONT:
                return std::cref(hw::adcs::adc_front);
            case vc_AdcNetName_ADC_RSM:
                return std::cref(hw::adcs::adc_rsm);
            case vc_AdcNetName_ADC_MISC_FUSE:
                return std::cref(hw::adcs::adc_misc_fuse);
            default:
            case vc_AdcNetName_ADC_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified ADC net name");
                return std::nullopt;
        }
    }

    std::optional<std::reference_wrapper<const hw::i2c::I2CDevice>> id_to_i2c(const _I2cNetName *inn) const override
    {
        if (inn->which_name != i2c_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected I2C net name with tag %d, got %d", i2c_net_name_tag, inn->which_name);
            return std::nullopt;
        }

        switch (inn->name.vc_net_name)
        {
            case vc_I2cNetName_I2C_BAT_MON:
                return std::cref(hw::i2c::bat_mon);
            case vc_I2cNetName_I2C_PWR_PUMP:
                return std::cref(hw::i2c::pwr_pump);
            default:
            case vc_I2cNetName_I2C_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified I2C net name");
                return std::nullopt;
        }
    }

    std::optional<std::reference_wrapper<const hw::spi::SpiDevice>> id_to_spi(const _SpiNetName *snn) const override
    {
        if (snn->which_name != spi_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", spi_net_name_tag, snn->which_name);
            return std::nullopt;
        }

        switch (snn->name.crit_net_name)
        {
            case vc_SpiNetName_IMU1:
                return std::cref(hw::spi::imu1);
            case vc_SpiNetName_IMU2:
                return std::cref(hw::spi::imu2);
            case vc_SpiNetName_IMU3:
                return std::cref(hw::spi::imu3);
            // case vc_SpiNetName_RPI:
            // return std::cref(RPI);
            default:
            case vc_SpiNetName_SPI_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified SPI net name");
                return std::nullopt;
        }
        return std::nullopt;
    }

  public:
    VCChimeraConfig()
    {
        gpio_net_name_tag = GpioNetName_vc_net_name_tag;
        spi_net_name_tag  = SpiNetName_vc_net_name_tag;
        adc_net_name_tag  = AdcNetName_vc_net_name_tag;
        i2c_net_name_tag  = I2cNetName_vc_net_name_tag;
    }
} vc_config;

static hw::rtos::StaticTask<8096>
    TaskChimera(osPriorityRealtime, "TaskChimera", [](void *) { chimera_v2::task(vc_config); });

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}
char USBD_PRODUCT_STRING_FS[] = "vc";

[[noreturn]] void tasks_init()
{
    SEGGER_SYSVIEW_Conf();
    assert(hw::usb::init());
    osKernelInitialize();
    TaskChimera.start();
    osKernelStart();
    forever {}
}
