#include <array>
#include "app_thermistors.hpp"

namespace
{
constexpr std::array<const float, 201> adbms_thermistor_lut_buffer = {};
}

using namespace io::therm;
namespace app::therm
{
constexpr ThermistorLUT adbms_thermistor_lut();
}
