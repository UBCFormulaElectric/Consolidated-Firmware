#include "chimera_v2.hpp"
#include "shared.pb.h"

#include "hw_gpios.hpp"
#include "hw_spis.hpp"
#include <crit.pb.h>

#include <optional>
#include <functional>

class CRITChimeraConfig : public chimera_v2::config
{
  public:
    ~CRITChimeraConfig() override = default;
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override
    {
        if (gnn->which_name != gpio_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", gpio_net_name_tag, gnn->which_name);
            return std::nullopt;
        }
        switch (gnn->name.crit_net_name)
        {
            case crit_GpioNetName_GPIO_BOOT:
                return std::cref(boot_led);
            case crit_GpioNetName_GPIO_LED:
                return std::cref(status_led);
            case crit_GpioNetName_GPIO_TELEM_SIG:
                return std::cref(telem_sig);
            case crit_GpioNetName_GPIO_PUSH_DRIVE_SIG:
                return std::cref(push_drive_sig);
            case crit_GpioNetName_GPIO_ROT_S:
                return std::cref(rot_s);
            case crit_GpioNetName_GPIO_ROT_B:
                return std::cref(rot_b);
            case crit_GpioNetName_GPIO_ROT_A:
                return std::cref(rot_a);
            case crit_GpioNetName_GPIO_LAUNCH_CONTROL_SIG:
                return std::cref(launch_control_sig);
            case crit_GpioNetName_GPIO_TORQUE_VECTORING_SIG:
                return std::cref(torque_vectoring_sig);
            case crit_GpioNetName_GPIO_REGEN_SIG:
                return std::cref(regen_sig);
            default:
            case crit_GpioNetName_GPIO_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified GPIO net name");
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
            case crit_SpiNetName_LED:
                return std::cref(leds_device);
            case crit_SpiNetName_SEVEN_SEG:
                return std::cref(seven_seg_device);
            case crit_SpiNetName_PWR_CHG:
                return std::cref(pwr_chg_device);
            default:
            case crit_SpiNetName_SPI_NET_NAME_UNSPECIFIED:
                LOG_INFO("Chimera: Unspecified SPI net name");
                return std::nullopt;
        }
        return std::nullopt;
    }

  public:
    CRITChimeraConfig()
    {
        gpio_net_name_tag = GpioNetName_crit_net_name_tag;
        spi_net_name_tag  = SpiNetName_crit_net_name_tag;
    }
} crit_config;

[[noreturn]] void tasks_preInit()
{
    chimera_v2::task(crit_config);
}
[[noreturn]] void tasks_run100Hz() {}