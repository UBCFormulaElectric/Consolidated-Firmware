#include "chimera_v2.hpp"
#include "tasks.h"

class FSMChimeraConfig : public chimera_v2::config
{
  public:
    ~FSMChimeraConfig() override = default;
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override;
} fsm_config;

void tasks_preInit() {}
void tasks_init()
{
    chimera_v2::task(fsm_config);
}