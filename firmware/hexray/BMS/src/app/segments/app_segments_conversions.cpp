#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using namespace std;

namespace app::segments
{
result<Cells<result<float>>> runVoltageConversion()
{
    RETURN_IF_ERR(io::adbms::command::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);

    const auto regs_result = io::adbms::read::cellVoltage();
    RETURN_IF_ERR(regs_result);
    const auto &regs = regs_result.value();

    Cells<result<float>> out;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            out[seg][cell] = regs[seg][cell].transform(convertRegToVoltage);
    return out;
}

result<pair<Therms<result<float>>, Therms<result<bool>>>> runAuxConversion()
{
    array<Therms<result<uint16_t>>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)> aux_regs{};
    for (size_t mux = 0U; mux < static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT); mux++)
    {
        config::setThermistorConfig(static_cast<ThermistorMux>(mux));
        RETURN_IF_ERR(io::adbms::startAuxAdcConversion());
        io::time::delay(AUX_CONV_TIME_MS);
        auto aux = io::adbms::readCellTempRegs();
        RETURN_IF_ERR_SILENT(aux);
        aux_regs[mux] = aux.value();
    }

    Therms<result<float>> out_temps;
    Therms<result<bool>>  out_owc;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t mux = 0U; mux < static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT); mux++)
        {
            for (size_t gpio = 0U; gpio < THERM_GPIOS_PER_SEGMENT; gpio++)
            {
                if (const size_t therm = gpio + mux * THERM_GPIOS_PER_SEGMENT; therm < THERMISTORS_PER_SEGMENT)
                {
                    out_temps[seg][therm] = aux_regs[mux][seg][gpio].transform(convertRegToTemp);
                    out_owc[seg][therm]   = aux_regs[mux][seg][gpio].transform(checkThermOwcOk);
                }
            }
        }
    }

    // const auto seg_regs_result = io::adbms::readSegVoltageRegs();
    // RETURN_IF_ERR(seg_regs_result);
    // const Segments<result<uint16_t>> &seg_regs = seg_regs_result.value();
    // Segments<result<float>>           out_seg_voltages;
    // for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    // {
    //     out_seg_voltages[seg] = seg_regs[seg].transform(convertRegToVoltage);
    // }

    return pair{ out_temps, out_owc };
}

result<Segments<result<float>>> runSegVoltageConversion()
{
    RETURN_IF_ERR(io::adbms::command::startSegAdcConversion());
    io::time::delay(AUX_CONV_TIME_MS);

    const auto seg_regs_result = io::adbms::read::segVoltage();
    RETURN_IF_ERR(seg_regs_result);
    const auto &seg_regs = seg_regs_result.value();

    Segments<result<float>> out_seg_voltages;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
        out_seg_voltages[seg] = seg_regs[seg].transform(convertRegToVoltage);

    return out_seg_voltages;
}

result<Segments<io::adbms::StatusGroups>> runStatusConversion()
{
    RETURN_IF_ERR(io::adbms::clear::stat());
    RETURN_IF_ERR(io::adbms::command::startCellsAdcConversion());
    RETURN_IF_ERR(io::adbms::command::startAuxAdcConversion());
    io::time::delay(AUX_CONV_TIME_MS);
    return io::adbms::read::status();
}

result<Cells<result<bool>>> runCellOpenWireCheck()
{
    RETURN_IF_ERR(io::adbms::command::startCellsAdcConversion());
    io::time::delay(OWC_CONVERSION_TIME_MS);
    const result<Cells<result<uint16_t>>> baseline_result = io::adbms::read::cellVoltage();
    RETURN_IF_ERR(baseline_result);
    const Cells<result<uint16_t>> &baseline_regs = baseline_result.value();

    RETURN_IF_ERR(io::adbms::command::owcCells(io::adbms::OpenWireSwitch::OddChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    const result<Cells<result<uint16_t>>> owc_odd_result = io::adbms::read::cellVoltage();
    RETURN_IF_ERR(owc_odd_result);
    const Cells<result<uint16_t>> &owc_odd_regs = owc_odd_result.value();

    RETURN_IF_ERR(io::adbms::command::owcCells(io::adbms::OpenWireSwitch::EvenChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    const result<Cells<result<uint16_t>>> owc_even_result = io::adbms::read::cellVoltage();
    RETURN_IF_ERR(owc_even_result);
    const Cells<result<uint16_t>> &owc_even_regs = owc_even_result.value();

    Cells<result<bool>> open_wire_cells;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!baseline_regs[seg][cell])
            {
                open_wire_cells[seg][cell] = unexpected(baseline_regs[seg][cell].error());
                continue;
            }
            if (!owc_odd_regs[seg][cell])
            {
                open_wire_cells[seg][cell] = unexpected(owc_odd_regs[seg][cell].error());
                continue;
            }
            if (!owc_even_regs[seg][cell])
            {
                open_wire_cells[seg][cell] = unexpected(owc_even_regs[seg][cell].error());
                continue;
            }

            const float baseline_v     = convertRegToVoltage(baseline_regs[seg][cell].value());
            const float owc_v          = (cell % 2 == 0) ? convertRegToVoltage(owc_odd_regs[seg][cell].value())
                                                         : convertRegToVoltage(owc_even_regs[seg][cell].value());
            open_wire_cells[seg][cell] = checkCellOwcOk(baseline_v, owc_v);
        }
    }
    return open_wire_cells;
}
} // namespace app::segments
