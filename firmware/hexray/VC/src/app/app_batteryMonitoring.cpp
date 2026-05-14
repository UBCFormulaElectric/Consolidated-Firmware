#include "app_batteryMonitoring.hpp"
#include "io_batteryMonitoring.hpp"
#include "io_batteryMonitoring_datatypes.hpp"
#include "app_canTx.hpp"

namespace app::batteryMonitoring
{
std::expected<void, ErrorCode> update()
{
    static bool init_done = false;
    
    if (!init_done)
    {
        for (int tries = 0; tries < 10 && !init_done; tries++)
            init_done = io::batteryMonitoring::init().has_value();
        if (!init_done)
            return std::unexpected(ErrorCode::ERROR);
        LOG_INFO("Battery monitor initialized");
    }

    io::batteryMonitoring::send_balancing_subcommand(CellBalance_BitMask::CELL2);
    auto val = io::batteryMonitoring::is_balancing_active();

    // Cells
    const auto cell1_voltage = io::batteryMonitoring::get_voltage_cell(CellReading::CELL1);
    const auto cell2_voltage = io::batteryMonitoring::get_voltage_cell(CellReading::CELL2);
    const auto cell3_voltage = io::batteryMonitoring::get_voltage_cell(CellReading::CELL3);
    const auto cell4_voltage = io::batteryMonitoring::get_voltage_cell(CellReading::CELL4);

    // System
    const auto pack_voltage = io::batteryMonitoring::get_voltage_system(SystemReading::PACK_V);
    const auto load_voltage = io::batteryMonitoring::get_voltage_system(SystemReading::LOAD_V);

    // Current through sense resistor
    const auto current = io::batteryMonitoring::get_current();

    // Integrated Charge
    // const auto integrated_charge = io::batteryMonitoring::get_integrated_charge();

    return {};
}
} // namespace app::batteryMonitoring