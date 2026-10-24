#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "io_time.hpp"

namespace
{
constexpr float OWC_V_LOW  = 1.0f;
constexpr float OWC_V_HIGH = 4.6f;

constexpr float THERM_OWC_V_HIGH = 2.8f;
} // namespace

namespace app::pack::owc
{
OpenWireCheckBuffer check(
    const io::adbms::PackCells<result<float>>  &odd_voltages,
    const io::adbms::PackCells<result<float>>  &even_voltages,
    const io::adbms::PackTherms<result<float>> &therm_voltages)
{
    OpenWireCheckBuffer owc{};

    for (size_t seg = 0; seg < io::adbms::NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < io::adbms::CELLS_PER_SEGMENT; cell++)
        {
            if (!odd_voltages[seg][cell] || !even_voltages[seg][cell])
                continue;

            const float odd             = odd_voltages[seg][cell].value();
            const float even            = even_voltages[seg][cell].value();
            const bool  odd_ok          = odd >= OWC_V_LOW && odd <= OWC_V_HIGH;
            const bool  even_ok         = even >= OWC_V_LOW && even <= OWC_V_HIGH;
            owc.cells_owc_ok[seg][cell] = odd_ok && even_ok;
        }

        for (size_t therm = 0; therm < io::adbms::THERMISTORS_PER_SEGMENT; therm++)
        {
            if (!therm_voltages[seg][therm])
                continue;

            owc.therms_owc_ok[seg][therm] = therm_voltages[seg][therm].value() < THERM_OWC_V_HIGH;
        }
    }

    owc.updated_ms = io::time::getCurrentMs();
    return owc;
}
} // namespace app::pack::owc
