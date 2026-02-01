#include "chimera_v2.hpp"
#include "shared.pb.h"

#include "hw_gpios.hpp"
#include "hw_spis.hpp"
#include <crit.pb.h>

#include <optional>
#include <functional>

class VCChimeraConfig : public chimera_v2::config
{
  public:
    ~VCChimeraConfig() override = default;
    std::optional<std::reference_wrapper<const hw::Gpio>> id_to_gpio(const _GpioNetName *gnn) const override;
} vc_config;

void tasks_preInit() {}
void tasks_init()
{
    chimera_v2::task(vc_config);
}