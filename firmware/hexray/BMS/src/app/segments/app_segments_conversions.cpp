#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using namespace std;
// using namespace app::segments;

namespace app::segments
{
expected<void, ErrorCode> runVoltageConversion(Cells<float> &out_voltages, CellSuccess &out_success)
{
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);
    const auto regs = io::adbms::readCellVoltageReg();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            out_voltages[seg][cell] = convertRegToVoltage(regs[seg][cell].value());
    return {};
}

expected<void, ErrorCode> runFilteredVoltageConversion(Cells<float> &out_voltages, CellSuccess &out_success)
{
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);
    const auto regs = io::adbms::readFilteredCellVoltageReg();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            out_voltages[seg][cell] = out_success[seg][cell] ? convertRegToVoltage(regs[seg][cell]) : -0.1f;
    return {};
}

expected<Therms<expected<float, ErrorCode>>, ErrorCode> runTempConversion()
{
    array<Therms<expected<uint16_t, ErrorCode>>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)> regs{};

    Therms<expected<float, ErrorCode>> out_temps;
    for (size_t mux = 0U; mux < static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT); mux++)
    {
        config::setThermistorConfig(static_cast<ThermistorMux>(mux));
        RETURN_IF_ERR_SILENT(config::configSync());
        RETURN_IF_ERR_SILENT(io::adbms::startTempAdcConversion());
        io::time::delay(AUX_CONV_TIME_MS);
        regs[mux] = io::adbms::readCellTempReg();
    }

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t mux = 0U; mux < static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT); mux++)
        {
            for (size_t gpio = 0U; gpio < THERM_GPIOS_PER_SEGMENT; gpio++)
            {
                // what the fuck
                if (const size_t therm = gpio + mux * THERM_GPIOS_PER_SEGMENT; therm < THERMISTORS_PER_SEGMENT)
                {
                    out_temps[seg][therm] = regs[mux][seg][gpio].transform(
                        [](const uint16_t reg) -> float { return convertRegToTemp(reg); });
                }
            }
        }
    }
    return { out_temps };
}

expected<Segments<expected<float, ErrorCode>>, ErrorCode> runSegVoltageConversion()
{
    RETURN_IF_ERR(io::adbms::startSegAdcConversion());
    io::time::delay(AUX_CONV_TIME_MS);
    const auto                           seg_regs = io::adbms::readSegVoltageReg();
    Segments<expected<float, ErrorCode>> out_seg_voltages;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        out_seg_voltages[seg] = seg_regs[seg].transform([](const uint16_t reg) { return convertRegToVoltage(reg); });
    }
    return out_seg_voltages;
}

expected<Segments<expected<io::adbms::StatusGroups, ErrorCode>>, ErrorCode> runStatusConversion()
{
    RETURN_IF_ERR(io::adbms::clear::StatReg());
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    RETURN_IF_ERR(io::adbms::startTempAdcConversion());
    io::time::delay(AUX_CONV_TIME_MS);
    return io::adbms::readStatusReg();
}

expected<void, ErrorCode> runCellOpenWireCheck(Owc &out_owc_ok, CellSuccess &out_success)
{
    RETURN_IF_ERR(io::adbms::startCellsBaseAdcConversion());
    io::time::delay(OWC_CONVERSION_TIME_MS);
    const auto baseline_regs = io::adbms::readCellVoltageReg();

    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::OddChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    const auto owc_odd_regs = io::adbms::readCellVoltageReg();

    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::EvenChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    const auto owc_even_regs = io::adbms::readCellVoltageReg();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!baseline_success[seg][cell])
            {
                out_success[seg][cell] = baseline_success[seg][cell];
                out_owc_ok[seg][cell]  = false;
                continue;
            }
            if (!owc_odd_success[seg][cell])
            {
                out_success[seg][cell] = owc_odd_success[seg][cell];
                out_owc_ok[seg][cell]  = false;
                continue;
            }
            if (!owc_even_success[seg][cell])
            {
                out_success[seg][cell] = owc_even_success[seg][cell];
                out_owc_ok[seg][cell]  = false;
                continue;
            }

            const float baseline_v = convertRegToVoltage(baseline_regs[seg][cell]);
            const float owc_v      = (cell % 2 == 0) ? convertRegToVoltage(owc_odd_regs[seg][cell])
                                                     : convertRegToVoltage(owc_even_regs[seg][cell]);
            out_owc_ok[seg][cell]  = checkCellOwcOk(baseline_v, owc_v);
            out_success[seg][cell] = {};
        }
    }
    return {};
}
} // namespace app::segments
