#include "app_tireTemp.hpp"
#include "io_tireTemp.hpp"
#include "app_canTx.hpp"

constexpr float MIN_TIRE_TEMPERATURE_CELSIUS = -20.0f;
constexpr float MAX_TIRE_TEMPERATURE_CELSIUS = 200.0f;

namespace app::tireTemp
{
void broadcast()
{
    const float temperature = io::tireTemp::get();

    const bool outOfRange =
        (temperature >= MAX_TIRE_TEMPERATURE_CELSIUS) || (temperature <= MIN_TIRE_TEMPERATURE_CELSIUS);
    can_tx::RSM_Info_TireTemperatureOutOfRange_set(outOfRange);
    can_tx::RSM_TireTemperature_set(outOfRange ? 0 : temperature);
}
} // namespace app::tireTemp