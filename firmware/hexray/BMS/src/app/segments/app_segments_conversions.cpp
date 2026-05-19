#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using namespace std;

namespace app::segments
{
result<Cells<expected<float, ErrorCode>>> runVoltageConversion()
{
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);

    const auto regs_result = io::adbms::readCellVoltageReg();
    RETURN_IF_ERR(regs_result);
    const auto &regs = regs_result.value();

    Cells<result<float>> out;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            out[seg][cell] = regs[seg][cell].transform(convertRegToVoltage);
    return out;
}

result<pair<Therms<expected<float, ErrorCode>>, Therms<expected<bool, ErrorCode>>>> runTempConversion()
{
    array<Therms<result<uint16_t>>, static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)> regs{};

    for (size_t mux = 0U; mux < static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT); mux++)
    {
        RETURN_IF_ERR(config::setThermistorConfig(static_cast<ThermistorMux>(mux)));
        RETURN_IF_ERR(io::adbms::startTempAdcConversion());
        io::time::delay(AUX_CONV_TIME_MS);
        auto temp_result = io::adbms::readCellTempReg();
        RETURN_IF_ERR(temp_result);
        regs[mux] = temp_result.value();
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
                    out_temps[seg][therm] = regs[mux][seg][gpio].transform(convertRegToTemp);
                    out_owc[seg][therm]   = regs[mux][seg][gpio].transform(checkThermOwcOk);
                }
            }
        }
    }
    return pair{ out_temps, out_owc };
}

result<Segments<expected<float, ErrorCode>>> runSegVoltageConversion()
{
    RETURN_IF_ERR(io::adbms::startSegAdcConversion());
    io::time::delay(AUX_CONV_TIME_MS);

    const auto seg_regs_result = io::adbms::readSegVoltageReg();
    RETURN_IF_ERR(seg_regs_result);
    const auto &seg_regs = seg_regs_result.value();

    Segments<result<float>> out;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
        out[seg] = seg_regs[seg].transform(convertRegToVoltage);
    return out;
}

result<Segments<io::adbms::StatusGroups>> runStatusConversion()
{
    RETURN_IF_ERR(io::adbms::clear::StatReg());
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    RETURN_IF_ERR(io::adbms::startTempAdcConversion());
    io::time::delay(AUX_CONV_TIME_MS);
    return io::adbms::readStatusReg();
}

result<Cells<expected<bool, ErrorCode>>> runCellOpenWireCheck()
{
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    io::time::delay(OWC_CONVERSION_TIME_MS);
    const auto baseline_result = io::adbms::readCellVoltageReg();
    RETURN_IF_ERR(baseline_result);
    const auto &baseline_regs = baseline_result.value();

    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::OddChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    const auto owc_odd_result = io::adbms::readCellVoltageReg();
    RETURN_IF_ERR(owc_odd_result);
    const auto &owc_odd_regs = owc_odd_result.value();

    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::EvenChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    const auto owc_even_result = io::adbms::readCellVoltageReg();
    RETURN_IF_ERR(owc_even_result);
    const auto &owc_even_regs = owc_even_result.value();

    Cells<result<bool>> out;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!baseline_regs[seg][cell])
            {
                out[seg][cell] = unexpected(baseline_regs[seg][cell].error());
                continue;
            }
            if (!owc_odd_regs[seg][cell])
            {
                out[seg][cell] = unexpected(owc_odd_regs[seg][cell].error());
                continue;
            }
            if (!owc_even_regs[seg][cell])
            {
                out[seg][cell] = unexpected(owc_even_regs[seg][cell].error());
                continue;
            }

            const float baseline_v = convertRegToVoltage(baseline_regs[seg][cell].value());
            const float owc_v      = (cell % 2 == 0) ? convertRegToVoltage(owc_odd_regs[seg][cell].value())
                                                     : convertRegToVoltage(owc_even_regs[seg][cell].value());
            out[seg][cell]         = checkCellOwcOk(baseline_v, owc_v);
        }
    }
    return out;
}
} // namespace app::segments
