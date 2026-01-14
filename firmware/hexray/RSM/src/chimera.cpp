#include "chimera_v2.hpp"
#include "tasks.h"

class RSMChimeraConfig : public chimera_v2::config
{
  public:
    ~RSMChimeraConfig() override = default;
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override;
} rsm_config;

void tasks_preInit() {}
void tasks_init()
{
    chimera_v2::task(rsm_config);
}