// Bridges the fakes::segments setters to the real app::segments::shared module.
//
// This lives in its own translation unit because test_fakes.hpp defines NUM_SEGMENTS /
// CELLS_PER_SEGMENT / etc. as placeholder macros, which collide with the constexpr definitions in
// io_adbms.hpp (pulled in transitively by app_segments.hpp). Keeping this file free of test_fakes.hpp
// lets us include the real headers and call shared::setVoltageStats / setTemperatureStats directly.

#include "app_segments.hpp"
#include "io_adbms.hpp"

using io::adbms::Cells, io::adbms::Therms;
// NUM_SEGMENTS / CELLS_PER_SEGMENT / THERMISTORS_PER_SEGMENT live in the global namespace in
// io_adbms.hpp, so they need no using-declaration here.

namespace fakes::segments::bridge
{
// Every cell is set to v, so shared::getMaxCellVoltage() (and getMinCellVoltage()) report v.
void pushMaxCellVoltageToShared(const float v)
{
    Cells<result<float>> voltages{};
    Cells<result<bool>>  owc{};
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; ++seg)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; ++cell)
        {
            voltages[seg][cell] = v;
            owc[seg][cell]      = false;
        }
    }
    app::segments::shared::setVoltageStats(voltages, owc);
}

// Every thermistor is set to t, so shared::getMaxCellTemperature() reports t.
void pushMaxCellTempToShared(const float t)
{
    Therms<result<float>> temps{};
    Therms<result<bool>>  owc{};
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; ++seg)
    {
        for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; ++therm)
        {
            temps[seg][therm] = t;
            owc[seg][therm]   = false;
        }
    }
    app::segments::shared::setTemperatureStats(temps, owc);
}
} // namespace fakes::segments::bridge
