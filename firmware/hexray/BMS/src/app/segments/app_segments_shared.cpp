// Holds the latest segment readings so other parts of BMS (alerts, balancing) can read them.
#include "app_segments_internal.hpp"
#include "io_semaphore.hpp"

namespace
{
using app::segments::CellParam;
using app::segments::Cells;

/*

*/

Cells<result<float>> latest_voltages{};
CellParam<float>     latest_min_cell_voltage{ .segment = 0, .cell = 0, .value = 0.0f };
CellParam<float>     latest_max_cell_voltage{ .segment = 0, .cell = 0, .value = 0.0f };
io::semaphore        voltage_lock{ true };

CellParam<float> latest_max_cell_temperature{ .segment = 0, .cell = 0, .value = 0.0f };
io::semaphore    temperature_lock{ true };

Cells<result<bool>> latest_cell_owc{};
io::semaphore       cell_owc_lock{ true };

// idk if this is needed
Cells<result<bool>> latest_therm_owc{};
io::semaphore       therm_owc_lock{ true };

} // namespace

namespace app::segments::shared
{

Cells<result<float>> getLatestVoltages()
{
    const io::unique_semaphore lock{ voltage_lock };
    return latest_voltages;
}

Cells<result<bool>> getLatestCellOwc()
{
    const io::unique_semaphore lock{ cell_owc_lock };
    return latest_cell_owc;
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

void setVoltageStats(
    const Cells<result<float>> latest,
    const CellParam<float>     min_voltage,
    const CellParam<float>     max_voltage)
{
    const io::unique_semaphore lock{ voltage_lock };
    latest_voltages         = latest;
    latest_min_cell_voltage = min_voltage;
    latest_max_cell_voltage = max_voltage;
}

void setTemperatureStats(const CellParam<float> max_temp)
{
    const io::unique_semaphore lock{ temperature_lock };
    latest_max_cell_temperature = max_temp;
}

void setThermOwc(const Cells<result<bool>> latest)
{
    const io::unique_semaphore lock{ therm_owc_lock };
    latest_therm_owc = latest;
}

void setCellOwc(const Cells<result<bool>> latest)
{
    const io::unique_semaphore lock{ cell_owc_lock };
    latest_cell_owc = latest;
}

} // namespace app::segments::shared
