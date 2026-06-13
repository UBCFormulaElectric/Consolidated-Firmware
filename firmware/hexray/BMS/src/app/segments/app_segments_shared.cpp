// Holds the latest segment readings so other parts of BMS (alerts, balancing) can read them.
#include <cassert>
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

CellParam<float> latest_max_therm_temperature{};
CellParam<float> latest_min_therm_temperature{};

Cells<result<bool>> latest_cell_owc_ok{};

Therms<result<bool>> latest_therm_owc_ok{};

SegmentParam<float> latest_max_segment_voltage{};
SegmentParam<float> latest_min_segment_voltage{};

result<float> pack_voltage;
} // namespace

io::semaphore shared_lock{ true };

namespace app::segments::shared
{

Cells<result<float>> getLatestVoltages()
{
    assert(shared_lock.is_held());
    return latest_voltages;
}

CellParam<float> getMinCellVoltage()
{
    assert(shared_lock.is_held());
    return latest_min_cell_voltage;
}

CellParam<float> getMaxCellVoltage()
{
    assert(shared_lock.is_held());
    return latest_max_cell_voltage;
}

CellParam<float> getMinCellTemperature()
{
    assert(shared_lock.is_held());
    return latest_min_therm_temperature;
}

CellParam<float> getMaxCellTemperature()
{
    assert(shared_lock.is_held());
    return latest_max_therm_temperature;
}

Cells<result<bool>> getLatestCellOwcOk()
{
    assert(shared_lock.is_held());
    return latest_cell_owc_ok;
}

Therms<result<bool>> getLatestThermOwcOk()
{
    assert(shared_lock.is_held());
    return latest_therm_owc_ok;
}

SegmentParam<float> getMinSegmentVoltage()
{
    assert(shared_lock.is_held());
    return latest_min_segment_voltage;
}

SegmentParam<float> getMaxSegmentVoltage()
{
    assert(shared_lock.is_held());
    return latest_max_segment_voltage;
}

result<float> getPackVoltage()
{
    assert(shared_lock.is_held());
    return pack_voltage;
}

void setCellOwcOk(const Cells<result<bool>> &latest)
{
    assert(shared_lock.is_held());
    latest_cell_owc_ok = latest;
}

void setVoltageStats(const Cells<result<float>> &latest)
{
    assert(shared_lock.is_held());
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

void setThermistorOwcOk(const Therms<result<bool>> &latest)
{
    assert(shared_lock.is_held());
    latest_therm_owc_ok = latest;
}

void setTemperatureStats(const Therms<result<float>> &latest)
{
    assert(shared_lock.is_held());

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
    assert(shared_lock.is_held());

    SegmentParam  min{ .segment = 0, .value = std::numeric_limits<float>::infinity() };
    SegmentParam  max{ .segment = 0, .value = -std::numeric_limits<float>::infinity() };
    result<float> pack = 0.0f;
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; ++seg)
    {
        const result<float> &v = latest[seg];
        if (!v)
        {
            // A failed segment read makes the whole pack voltage invalid (keep the first error).
            if (pack)
            {
                pack = std::unexpected(v.error());
            }
            continue;
        }
        if (pack)
        {
            pack.value() += v.value();
        }
        const SegmentParam sp{ .segment = seg, .value = v.value() };
        min = std::min(min, sp);
        max = std::max(max, sp);
    }
    latest_min_segment_voltage = min;
    latest_max_segment_voltage = max;
    pack_voltage               = pack;
}
} // namespace app::segments::shared
