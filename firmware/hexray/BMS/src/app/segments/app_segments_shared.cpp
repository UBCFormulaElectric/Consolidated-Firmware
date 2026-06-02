// Holds the latest segment readings so other parts of BMS (alerts, balancing) can read them.
#include <limits>
#include "app_segments_internal.hpp"
#include "io_semaphore.hpp"

namespace
{
using app::segments::CellParam, app::segments::SegmentParam;
using io::adbms::Cells, io::adbms::Segments, io::adbms::Therms;

Cells<result<float>> latest_voltages{};
CellParam<float>     latest_min_cell_voltage{};
CellParam<float>     latest_max_cell_voltage{};
io::semaphore        voltage_lock{ true };

Therms<result<float>> latest_temperatures{};
CellParam<float>      latest_max_therm_temperature{};
CellParam<float>      latest_min_therm_temperature{};
io::semaphore         temperature_lock{ true };

Cells<result<bool>> latest_cell_owc{};
io::semaphore       cell_owc_lock{ true };

Therms<result<bool>> latest_therm_owc{};
io::semaphore        therm_owc_lock{ true };

Segments<result<float>> latest_segment_voltages{};
SegmentParam<float>     latest_max_segment_voltage{};
SegmentParam<float>     latest_min_segment_voltage{};
io::semaphore           segment_voltage_lock{ true };
} // namespace

namespace app::segments::shared
{

Cells<result<float>> getLatestVoltages()
{
    const io::unique_semaphore lock{ voltage_lock };
    return latest_voltages;
}

CellParam<float> getMinCellVoltage()
{
    const io::unique_semaphore lock{ voltage_lock };
    return latest_min_cell_voltage;
}

CellParam<float> getMaxCellVoltage()
{
    const io::unique_semaphore lock{ voltage_lock };
    return latest_max_cell_voltage;
}

Therms<result<float>> getLatestTemperatures()
{
    const io::unique_semaphore lock{ temperature_lock };
    return latest_temperatures;
}

CellParam<float> getMinCellTemperature()
{
    const io::unique_semaphore lock{ temperature_lock };
    return latest_min_therm_temperature;
}

CellParam<float> getMaxCellTemperature()
{
    const io::unique_semaphore lock{ temperature_lock };
    return latest_max_therm_temperature;
}

Cells<result<bool>> getLatestCellOwc()
{
    const io::unique_semaphore lock{ cell_owc_lock };
    return latest_cell_owc;
}

Therms<result<bool>> getLatestThermOwc()
{
    const io::unique_semaphore lock{ therm_owc_lock };
    return latest_therm_owc;
}

Segments<result<float>> getLatestSegmentVoltages()
{
    const io::unique_semaphore lock{ segment_voltage_lock };
    return latest_segment_voltages;
}

SegmentParam<float> getMinSegmentVoltage()
{
    const io::unique_semaphore lock{ segment_voltage_lock };
    return latest_min_segment_voltage;
}

SegmentParam<float> getMaxSegmentVoltage()
{
    const io::unique_semaphore lock{ segment_voltage_lock };
    return latest_max_segment_voltage;
}

void setVoltageStats(const Cells<result<float>> &latest, const Cells<result<bool>> &owc)
{
    {
        const io::unique_semaphore lock{ cell_owc_lock };
        latest_cell_owc = owc;
    }

    const io::unique_semaphore lock{ voltage_lock };
    latest_voltages = latest;

    CellParam min{ .segment = 0, .cell = 0, .value = std::numeric_limits<float>::infinity() };
    CellParam max{ .segment = 0, .cell = 0, .value = -std::numeric_limits<float>::infinity() };
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; ++seg)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; ++cell)
        {
            const result<float> &v = latest[seg][cell];
            if (!v)
            {
                continue;
            }
            const CellParam cp{ .segment = seg, .cell = cell, .value = v.value() };
            min = std::min(min, cp);
            max = std::max(max, cp);
        }
    }
    latest_min_cell_voltage = min;
    latest_max_cell_voltage = max;
}

void setTemperatureStats(const Therms<result<float>> &latest, const Therms<result<bool>> &owc)
{
    {
        const io::unique_semaphore lock{ therm_owc_lock };
        latest_therm_owc = owc;
    }

    const io::unique_semaphore lock{ temperature_lock };
    latest_temperatures = latest;

    CellParam min{ .segment = 0, .cell = 0, .value = std::numeric_limits<float>::infinity() };
    CellParam max{ .segment = 0, .cell = 0, .value = -std::numeric_limits<float>::infinity() };
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; ++seg)
    {
        for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; ++therm)
        {
            const result<float> &t = latest[seg][therm];
            if (!t)
            {
                continue;
            }
            const CellParam cp{ .segment = seg, .cell = therm, .value = t.value() };
            min = std::min(min, cp);
            max = std::max(max, cp);
        }
    }
    latest_min_therm_temperature = min;
    latest_max_therm_temperature = max;
}

void setSegmentVoltageStats(const Segments<result<float>> &latest)
{
    const io::unique_semaphore lock{ segment_voltage_lock };
    latest_segment_voltages = latest;

    SegmentParam min{ .segment = 0, .value = std::numeric_limits<float>::infinity() };
    SegmentParam max{ .segment = 0, .value = -std::numeric_limits<float>::infinity() };
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; ++seg)
    {
        const result<float> &v = latest[seg];
        if (!v)
        {
            continue;
        }
        const SegmentParam sp{ .segment = seg, .value = v.value() };
        min = std::min(min, sp);
        max = std::max(max, sp);
    }
    latest_min_segment_voltage = min;
    latest_max_segment_voltage = max;
}
} // namespace app::segments::shared
