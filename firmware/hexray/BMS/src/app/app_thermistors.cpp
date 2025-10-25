#include <array>
#include "app_thermistors.hpp"

namespace
{
    constexpr std::array<const float, 201> ltc_thermistor_lut_buffer = {};
}

using namespace app::therm;
namespace app::therm{
    constexpr ThermistorLUT adbms_thermistor_lut();
}
