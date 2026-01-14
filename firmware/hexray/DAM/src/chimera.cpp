#include "chimera_v2.hpp"
#include "tasks.h"

class DAMChimeraConfig : public chimera_v2::config
{
  public:
    ~DAMChimeraConfig() override = default;
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override;
} dam_config;

void tasks_preInit() {}
void tasks_init()
{
    chimera_v2::task(dam_config);
}