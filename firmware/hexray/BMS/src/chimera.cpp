#include "tasks.h"
#include "chimera_v2.hpp"

class BMS final : public chimera_v2::config
{
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override
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
    chimera_v2::task(bms_config);
}
