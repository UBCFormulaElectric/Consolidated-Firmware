#include "chimera_v2.hpp"
#include "tasks.h"
#include "shared.pb.h"
#include "io_log.hpp"
#include "hw_usb.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "hw_hardFaultHandler.hpp"
#include "hw_gpios.hpp"
#include "hw_spis.hpp"
#include "hw_adcs.hpp"

#include <fsm.pb.h>
#include <cassert>

#include <optional>
#include <functional>

class FSMChimeraConfig : public chimera_v2::config
{
  public:
    ~FSMChimeraConfig() override = default;
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override
    {
        if (gnn->which_name != gpio_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", gpio_net_name_tag, gnn->which_name);
            return std::nullopt;
        }
        switch (gnn->name.fsm_net_name)
        {
            case fsm_GpioNetName_GPIO_BOTS_3v3:
                return std::cref(hw::gpios::boot_led);
            case fsm_GpioNetName_GPIO_COCKPIT_SHDN_3v3:
                return std::cref(hw::gpios::cockpit_shdn_3v3);
            case fsm_GpioNetName_GPIO_IMU_NSS:
                return std::cref(hw::gpios::imu_nss);
            case fsm_GpioNetName_GPIO_IMU_FSYNC:
                return std::cref(hw::gpios::imu_fsync);
            case fsm_GpioNetName_GPIO_NSUSP_FL_OCSC:
                return std::cref(hw::gpios::nsusp_fl_ocsc);
            case fsm_GpioNetName_GPIO_NSUSP_FR_OCSC:
                return std::cref(hw::gpios::nsusp_fr_ocsc);
            case fsm_GpioNetName_GPIO_FR_INT_3v3:
                return std::cref(hw::gpios::fr_int_3v3);
            case fsm_GpioNetName_GPIO_FL_INT_3v3:
                return std::cref(hw::gpios::fl_int_3v3);
            case fsm_GpioNetName_GPIO_DEBUG_LED:
                return std::cref(hw::gpios::debug_led);
            case fsm_GpioNetName_GPIO_NSTR_ANGLE_OCSC:
                return std::cref(hw::gpios::nstr_angle_ocsc);
            case fsm_GpioNetName_GPIO_NBPS_F_OCSC:
                return std::cref(hw::gpios::nbps_f_ocsc);
            case fsm_GpioNetName_GPIO_BOOT_LED:
                return std::cref(hw::gpios::boot_led);
            default:
            case fsm_GpioNetName_GPIO_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified GPIO net name");
                LOG_INFO("Chimera: Unspecified GPIO net name");
                return std::nullopt;
        }
    }

    std::optional<std::reference_wrapper<const hw::Adc>> id_to_adc(const _AdcNetName *snn) const override
    {
        if (snn->which_name != adc_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected ADC net name with tag %d, got %d", adc_net_name_tag, snn->which_name);
            return std::nullopt;
        }

        switch (snn->name.fsm_net_name)
        {
            case fsm_AdcNetName_ADC_SUSP_FL:
                return std::cref(hw::adcs::susp_fl);
            case fsm_AdcNetName_ADC_SUSP_FR:
                return std::cref(hw::adcs::susp_fr);
            case fsm_AdcNetName_ADC_APPS2:
                return std::cref(hw::adcs::apps2);
            case fsm_AdcNetName_ADC_BPS_F:
                return std::cref(hw::adcs::bps_f);
            case fsm_AdcNetName_ADC_STR_ANGLE:
                return std::cref(hw::adcs::str_angle);
            case fsm_AdcNetName_ADC_APPS1:
                return std::cref(hw::adcs::apps1);
            default:
            case fsm_AdcNetName_ADC_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified ADC net name");
                return std::nullopt;
        }
        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const hw::spi::SpiDevice>> id_to_spi(const _SpiNetName *snn) const override
    {
        if (snn->which_name != spi_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected SPI net name with tag %d, got %d", spi_net_name_tag, snn->which_name);
            return std::nullopt;
        }

        switch (snn->name.fsm_net_name)
        {
            case fsm_SpiNetName_SPI_IMU:
                return std::cref(hw::spi::imu_spi);
            default:
            case fsm_SpiNetName_SPI_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified SPI net name");
                return std::nullopt;
        }
        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const hw::PwmOutput>> id_to_pwm(const _PwmNetName *pnn) const override
    {
        if (pnn->which_name != pwm_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected PWM net name with tag %d, got %d", pwm_net_name_tag, pnn->which_name);
            return std::nullopt;
        }
        return std::nullopt;
    }

  public:
    FSMChimeraConfig()
    {
        gpio_net_name_tag = GpioNetName_fsm_net_name_tag;
        adc_net_name_tag  = AdcNetName_fsm_net_name_tag;
        spi_net_name_tag  = SpiNetName_fsm_net_name_tag;
    }
} fsm_config;

static hw::rtos::StaticTask<8096>
    TaskChimera(osPriorityRealtime, "TaskChimera", [](void *) { chimera_v2::task(fsm_config); });

void tasks_preInit() {}

char USBD_PRODUCT_STRING_FS[] = "fsm";

[[noreturn]] void tasks_init()
{
    assert(hw::usb::init());
    osKernelInitialize();
    TaskChimera.start();
    osKernelStart();
    forever {}
}