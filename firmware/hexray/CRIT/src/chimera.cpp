#include "tasks.h"
#include "chimera_v2.hpp"

class CRITChimeraConfig : public chimera_v2::config
{
  public:
    ~CRITChimeraConfig() override = default;
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override
    {
        return std::nullopt;
    }
} crit_config;

void tasks_preInit() {}
void tasks_init()
{
    chimera_v2::task(crit_config);
}