#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using namespace std;
using namespace app::segments;

// check later
static constexpr uint8_t VOLT_CONV_TIME_MS      = 2U;
static constexpr uint8_t TEMP_CONV_TIME_MS      = 2U;
static constexpr uint8_t OWC_CONVERSION_TIME_MS = 2U;

namespace app::segments
{
array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_voltage_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltage_success;
array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                     cell_voltages;

array<array<array<uint16_t, io::adbms::GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>, 2>                  cell_temp_regs;
array<array<array<expected<void, ErrorCode>, io::adbms::GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>, 2> cell_temp_success;
array<array<float, io::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS>                                cell_temps;

array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_owc_odd_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_odd_success;
array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  cell_owc_even_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_even_success;

array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  therm_owc_odd_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_odd_success;
array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  therm_owc_even_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_even_success;

expected<void, ErrorCode> runVoltageConversion()
{
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);
    io::adbms::readCellVoltageReg(cell_voltage_regs, cell_voltage_success);
    return {};
}

expected<void, ErrorCode> runAuxConversion()
{
    for (uint8_t mux_index = 0U; mux_index < static_cast<uint8_t>(ThermistorMux::THERMISTOR_MUX_COUNT); mux_index++)
    {
        const ThermistorMux mux = static_cast<ThermistorMux>(mux_index);
        setThermistorConfig(mux);
        writeConfig();
        configSync();

        RETURN_IF_ERR(io::adbms::startTempAdcConversion());
        io::time::delay(TEMP_CONV_TIME_MS);
        io::adbms::readCellTempReg(cell_temp_regs[mux_index], cell_temp_success[mux_index]);
    }
    return {};
}

expected<void, ErrorCode> runCellOwc()
{
    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::OddChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::OddChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellVoltageReg(therm_owc_odd_regs, therm_owc_odd_success);

    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::EvenChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::EvenChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellVoltageReg(therm_owc_even_regs, therm_owc_even_success);
    return {};
}

expected<void, ErrorCode> runThermOpenWireCheck()
{
    return {};
}

expected<void, ErrorCode> runStatusConversion()
{
    return {};
}

expected<void, ErrorCode> runAdcAccuracyTest()
{
    return {};
}

} // namespace app::segments