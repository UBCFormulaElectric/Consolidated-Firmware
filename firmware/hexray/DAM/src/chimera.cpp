#include "tasks.h"
#include "chimera_v2.hpp"
#include "hw_gpios.hpp"
#include "hw_rtosTaskHandler.hpp"
#include "shared.pb.h"
#include "io_log.hpp"
#include "hw_usb.hpp"

#include <cassert>
#include <dam.pb.h>

#include <optional>
#include <functional>

class DAMChimeraConfig : public chimera_v2::config
{
  public:
    ~DAMChimeraConfig() override = default;

    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override
    {
        if (gnn->which_name != gpio_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", gpio_net_name_tag, gnn->which_name);
            return std::nullopt;
        }

        switch (gnn->name.dam_net_name)
        {
            case dam_GpioNetName_GPIO_NTP:
                return std::cref(ntp_pin);
            case dam_GpioNetName_GPIO_BUZZER_PWR_EN:
                return std::cref(buzzer_pwr_en_pin);
            case dam_GpioNetName_GPIO_TSIM_RED_EN:
                return std::cref(tsim_red_en_pin);
            case dam_GpioNetName_GPIO_NTSIM_GRN_EN:
                return std::cref(ntsim_green_en_pin);
            case dam_GpioNetName_GPIO_R_SHDN_SENSE:
                return std::cref(r_shdn_sense_pin);
            case dam_GpioNetName_GPIO_L_SHDN_SENSE:
                return std::cref(l_shdn_sense_pin);
            case dam_GpioNetName_GPIO_LED:
                return std::cref(led_pin);
            case dam_GpioNetName_GPIO_BOOT:
                return std::cref(boot_pin);
            case dam_GpioNetName_GPIO_SD_PRESENT:
                return std::cref(sd_present);
            case dam_GpioNetName_GPIO_900M:
                return std::cref(_900m_pin);
            case dam_GpioNetName_GPIO_SD_FAIL:
                return std::cref(sd_fail_pin);
            case dam_GpioNetName_GPIO_D_P_PULLUP:
                return std::cref(d_p_pullup_pin);
            default:
            case dam_GpioNetName_GPIO_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unknown GPIO net name");
                return std::nullopt;
        }
    }

    consteval DAMChimeraConfig() { gpio_net_name_tag = GpioNetName_dam_net_name_tag; }

} dam_config;

static hw::rtos::StaticTask<8096>
    TaskChimera(osPriorityRealtime, "TaskChimera", [](void *) { chimera_v2::task(dam_config); });

void tasks_preInit(){}
char USBD_PRODUCT_STRING_FS[] = "dam";

[[noreturn]] void tasks_init()
{
    assert(hw::usb::init());
    osKernelInitialize();
    TaskChimera.start();
    osKernelStart();
    forever {}
}
