#include "tasks.h"
#include "chimera_v2.hpp"

class BMS final : public hw::chimera_v2::config
{
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override
    {
        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const hw::i2c::I2CDevice>> id_to_i2c(const _I2cNetName *inn) const override
    {
        return std::nullopt;
    }

  public:
    ~BMS() override = default;
    BMS()           = default;
} bms_config;

void tasks_preInit() {}

void tasks_init()
{
    hw::chimera_v2::task(bms_config);
}
