#include "f4dev.pb.h"
#include "shared.pb.h"
#include "chimera_v2.hpp"

#include "hw_gpios.hpp"

#include <optional>
#include <functional>

class F4Chimera : public chimera_v2_config
{
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override
    {
        if (gnn->which_name != gpio_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", gpio_net_name_tag, gnn->which_name);
            return std::nullopt;
        }
        switch (gnn->name.f4dev_net_name)
        {
            case f4dev_GpioNetName_GPIO_6:
                return std::cref(gpio_6);
            case f4dev_GpioNetName_GPIO_5:
                return std::cref(gpio_5);
            default:
            case f4dev_GpioNetName_GPIO_NET_NAME_UNSPECIFIED:
                LOG_INFO("");
                return std::nullopt;
        }
    }

    std::optional<std::reference_wrapper<const hw::Adc>> id_to_adc(const _AdcNetName *ann) const override
    {
        if (ann->which_name != adc_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", adc_net_name_tag, ann->which_name);
            return std::nullopt;
        }
        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const hw::i2c::I2CDevice>> id_to_i2c(const _I2cNetName *inn) const override
    {
        if (inn->which_name != i2c_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", i2c_net_name_tag, inn->which_name);
            return std::nullopt;
        }
        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const hw::spi::SpiDevice>> id_to_spi(const _SpiNetName *snn) const override
    {
        if (snn->which_name != spi_net_name_tag)
        {
            LOG_ERROR("Chimera: Expected GPIO net name with tag %d, got %d", spi_net_name_tag, snn->which_name);
            return std::nullopt;
        }
        return std::nullopt;
    }

  public:
    F4Chimera()
    {
        gpio_net_name_tag = GpioNetName_f4dev_net_name_tag;
        adc_net_name_tag  = 0;
        i2c_net_name_tag  = 0;
        spi_net_name_tag  = 0;
    }
} f4_chimera{};