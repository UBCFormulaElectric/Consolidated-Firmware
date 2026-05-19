#include "app_segments_internal.hpp"
#include "io_semaphore.hpp"
#include "io_adbms.hpp"
#include <array>
#include <bitset>
#include <algorithm>

namespace
{
using io::adbms::Cells;

std::array<std::bitset<4>, MAX_NUM_SEGMENTS> segment_health{};
io::semaphore                                health_mutex{ true };

Cells<result<float>>     latest_voltages{};
app::segments::CellParam latest_min_cell_voltage{ .segment = 0, .cell = 0, .value = 0.0f };
app::segments::CellParam latest_max_cell_voltage{ .segment = 0, .cell = 0, .value = 0.0f };
io::semaphore            voltage_lock{ true };

app::segments::CellParam latest_max_cell_temperature{ .segment = 0, .cell = 0, .value = 0.0f };
bool                     latest_therm_owc = false;
io::semaphore            temperature_lock{ true };

bool          latest_cell_owc = false;
io::semaphore cell_owc_lock{ true };
} // namespace

namespace app::segments::state
{

void reset(const size_t seg, Bit bit)
{
    const io::unique_semaphore lock{ health_mutex };
    segment_health[seg].reset(static_cast<size_t>(bit));
}

void resetAll(Bit bit)
{
    const io::unique_semaphore lock{ health_mutex };
    for (auto &bs : segment_health)
        bs.reset(static_cast<size_t>(bit));
}

void set(const size_t seg, Bit bit)
{
    const io::unique_semaphore lock{ health_mutex };
    segment_health[seg].set(static_cast<size_t>(bit));
}

void setAll(Bit bit)
{
    const io::unique_semaphore lock{ health_mutex };
    for (auto &bs : segment_health)
        bs.set(static_cast<size_t>(bit));
}

bool isOk(const size_t seg)
{
    const io::unique_semaphore lock{ health_mutex };
    return segment_health[seg].all();
}

bool allOk()
{
    const io::unique_semaphore lock{ health_mutex };
    return std::all_of(segment_health.begin(), segment_health.end(), [](const std::bitset<4> &bs) { return bs.all(); });
}

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

CellParam<float> getMaxCellTemperature()
{
    const io::unique_semaphore lock{ temperature_lock };
    return latest_max_cell_temperature;
}

bool getCellOwc()
{
    const io::unique_semaphore lock{ cell_owc_lock };
    return latest_cell_owc;
}

bool getThermOwc()
{
    const io::unique_semaphore lock{ temperature_lock };
    return latest_therm_owc;
}

void setVoltageStats(const Cells<result<float>> &latest, const CellParam<float> min, const CellParam<float> max)
{
    const io::unique_semaphore lock{ voltage_lock };
    latest_voltages         = latest;
    latest_min_cell_voltage = min;
    latest_max_cell_voltage = max;
}

void setTempStats(const CellParam<float> max_temp, const bool any_therm_owc)
{
    const io::unique_semaphore lock{ temperature_lock };
    latest_max_cell_temperature = max_temp;
    latest_therm_owc            = any_therm_owc;
}

void setCellOwc(const bool any_cell_owc)
{
    const io::unique_semaphore lock{ cell_owc_lock };
    latest_cell_owc = any_cell_owc;
}

} // namespace app::segments::state
