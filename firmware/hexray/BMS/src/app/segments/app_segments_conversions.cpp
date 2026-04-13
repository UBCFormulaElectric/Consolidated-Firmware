#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using namespace std;
using namespace app::segments;

// check later
static constexpr uint8_t VOLT_CONV_TIME_MS      = 2U;
static constexpr uint8_t AUX_CONV_TIME_MS       = 4U;
static constexpr uint8_t OWC_CONVERSION_TIME_MS = 8U;
static constexpr float   OW_CELL_RELATIVE_THRESHOLD  = 0.7f;
static constexpr float   OW_CELL_ABSOLUTE_THRESHOLD  = 0.5f;

static constexpr float convertRegToVoltage(uint16_t reg)
{
    return (static_cast<float>(static_cast<int16_t>(reg)) * 150e-6f) + 1.5f;
}

static constexpr bool checkCellOwcOk(float baselineVoltage, float owcVoltage)
{
    if (owcVoltage < baselineVoltage * OW_CELL_RELATIVE_THRESHOLD)
        return false;
    if (owcVoltage < OW_CELL_ABSOLUTE_THRESHOLD)
        return false;
    return true;
}

namespace app::segments
{
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltage_success;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> filtered_cell_voltage_success;

array<
    array<array<uint16_t, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>,
    static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
    cell_temp_regs;
array<
    array<array<expected<void, ErrorCode>, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>,
    static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
    cell_temp_success;

array<io::adbms::StatusGroups, io::NUM_SEGMENTS>   stat_regs;
array<expected<void, ErrorCode>, io::NUM_SEGMENTS> stat_success;

expected<void, ErrorCode> runVoltageConversion()
{
    array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> regs;
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);
    io::adbms::readCellVoltageReg(regs, cell_voltage_success);

    for (size_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
        for (size_t cell = 0; cell < io::CELLS_PER_SEGMENT; cell++)
            cell_voltages[seg][cell] = cell_voltage_success[seg][cell]
                ? convertRegToVoltage(regs[seg][cell])
                : -0.1f;
    return {};
}

expected<void, ErrorCode> runFilteredVoltageConversion()
{
    array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> regs;
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);
    io::adbms::readFilteredCellVoltageReg(regs, filtered_cell_voltage_success);

    for (size_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
        for (size_t cell = 0; cell < io::CELLS_PER_SEGMENT; cell++)
            filtered_cell_voltages[seg][cell] = filtered_cell_voltage_success[seg][cell]
                ? convertRegToVoltage(regs[seg][cell])
                : -0.1f;
    return {};
}

expected<void, ErrorCode> runAuxConversion()
{
    for (uint8_t mux_index = 0U; mux_index < static_cast<uint8_t>(ThermistorMux::THERMISTOR_MUX_COUNT); mux_index++)
    {
        setThermistorConfig(static_cast<ThermistorMux>(mux_index));
        RETURN_IF_ERR(configSync());
        RETURN_IF_ERR(io::adbms::startTempAdcConversion());
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
    // Stack locals — only needed during this function, results go into cell_owc_ok[][]
    array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS>                  baseline_regs, owc_odd_regs, owc_even_regs;
    array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> baseline_success, owc_odd_success, owc_even_success;

    RETURN_IF_ERR(io::adbms::baselineCells());
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellVoltageReg(baseline_regs, baseline_success);

    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::OddChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellVoltageReg(owc_odd_regs, owc_odd_success);

    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::EvenChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellVoltageReg(owc_even_regs, owc_even_success);

    // Evaluate OWC immediately
    for (size_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < io::CELLS_PER_SEGMENT; cell++)
        {
            if (!baseline_success[seg][cell] || !owc_odd_success[seg][cell] || !owc_even_success[seg][cell])
            {
                cell_owc_ok[seg][cell] = false;
                continue;
            }

            float baseline = convertRegToVoltage(baseline_regs[seg][cell]);
            float owc_v    = (cell % 2 == 0)
                ? convertRegToVoltage(owc_odd_regs[seg][cell])
                : convertRegToVoltage(owc_even_regs[seg][cell]);
            cell_owc_ok[seg][cell] = checkCellOwcOk(baseline, owc_v);
        }
    }
    return {};
}
} // namespace app::segments
