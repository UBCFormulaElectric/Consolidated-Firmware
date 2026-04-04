#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using namespace std;
using namespace app::segments;

// check later
static constexpr uint8_t VOLT_CONV_TIME_MS      = 2U;
static constexpr uint8_t AUX_CONV_TIME_MS      = 4U;
static constexpr uint8_t OWC_CONVERSION_TIME_MS = 2U;

namespace app::segments
{
array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_voltage_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltage_success;
array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                     cell_voltages;

array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  filtered_cell_voltage_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> filtered_cell_voltage_success;
array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                     filtered_cell_voltages;

array<array<array<uint16_t, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>                  cell_temp_regs;
array<array<array<expected<void, ErrorCode>, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)> cell_temp_success;
array<array<float, io::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS>                                                                                             cell_temps;

array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_owc_odd_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_odd_success;
array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_owc_even_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_even_success;

array<array<uint16_t, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>                  therm_owc_odd_regs;
array<array<expected<void, ErrorCode>, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_odd_success;
array<array<uint16_t, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>                  therm_owc_even_regs;
array<array<expected<void, ErrorCode>, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_even_success;
array<array<bool, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>                      therm_owc_ok;

array<io::adbms::StatusGroups, io::NUM_SEGMENTS>                                 stat_regs;
array<expected<void, ErrorCode>, io::NUM_SEGMENTS>                               stat_success;

expected<void, ErrorCode> runVoltageConversion()
{
    RETURN_IF_ERR(writeConfig());
    RETURN_IF_ERR(configSync());
    
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);
    io::adbms::readCellVoltageReg(cell_voltage_regs, cell_voltage_success);
    return {};
}

expected<void, ErrorCode> runFilteredVoltageConversion()
{
    RETURN_IF_ERR(writeConfig());
    RETURN_IF_ERR(configSync());
    
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);
    io::adbms::readFilteredCellVoltageReg(filtered_cell_voltage_regs, filtered_cell_voltage_success);
    return {};
}


expected<void, ErrorCode> runAuxConversion()
{
    for (uint8_t mux_index = 0U; mux_index < static_cast<uint8_t>(ThermistorMux::THERMISTOR_MUX_COUNT); mux_index++)
    {
        setDefaultConfig();
        setThermistorConfig(static_cast<ThermistorMux>(mux_index));
        RETURN_IF_ERR(writeConfig());

        RETURN_IF_ERR(io::adbms::startTempAdcConversion());
        io::adbms::clearCellTempReg();
        io::time::delay(AUX_CONV_TIME_MS);
        io::adbms::readCellTempReg(cell_temp_regs[mux_index], cell_temp_success[mux_index]);
    }
    return {};
}

expected<void, ErrorCode> runStatusConversion()
{
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    RETURN_IF_ERR(io::adbms::startTempAdcConversion());
    io::time::delay(AUX_CONV_TIME_MS);
    io::adbms::readStatusReg(stat_regs, stat_success);
    return {};
}

expected<void, ErrorCode> runCellOpenWireCheck()
{
    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::OddChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellVoltageReg(cell_owc_odd_regs, cell_owc_odd_success);

    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::EvenChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellVoltageReg(cell_owc_even_regs, cell_owc_even_success);
    return {};
}

expected<void, ErrorCode> runThermOpenWireCheck()
{
    RETURN_IF_ERR(io::adbms::owcTherms(io::adbms::OpenWireSwitch::OddChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellTempReg(therm_owc_odd_regs, therm_owc_odd_success);

    RETURN_IF_ERR(io::adbms::owcTherms(io::adbms::OpenWireSwitch::EvenChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellTempReg(therm_owc_even_regs, therm_owc_even_success);
    return {};
}



} // namespace app::segments