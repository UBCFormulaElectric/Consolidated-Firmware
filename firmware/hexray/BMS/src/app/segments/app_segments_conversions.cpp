#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using namespace std;
using namespace app::segments;

namespace app::segments
{
expected<void, ErrorCode> runVoltageConversion(Cell &out_voltages, CellSuccess &out_success)
{
    array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> regs;
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);
    io::adbms::readCellVoltageReg(regs, out_success);

    for (size_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
        for (size_t cell = 0; cell < io::CELLS_PER_SEGMENT; cell++)
            out_voltages[seg][cell] = out_success[seg][cell] ? convertRegToVoltage(regs[seg][cell]) : -0.1f;
    return {};
}

expected<void, ErrorCode> runFilteredVoltageConversion(Cell &out_voltages, CellSuccess &out_success)
{
    array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> regs;
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);
    io::adbms::readFilteredCellVoltageReg(regs, out_success);

    for (size_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
        for (size_t cell = 0; cell < io::CELLS_PER_SEGMENT; cell++)
            out_voltages[seg][cell] = out_success[seg][cell] ? convertRegToVoltage(regs[seg][cell]) : -0.1f;
    return {};
}

expected<void, ErrorCode> runTempConversion(Therm &out_temps, ThermSuccess &out_success)
{
    array<
        array<array<uint16_t, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>,
        static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
        regs;
    array<
        array<array<expected<void, ErrorCode>, io::adbms::THERM_GPIOS_PER_SEGMENT>, io::NUM_SEGMENTS>,
        static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT)>
        regs_success;

    for (size_t mux = 0U; mux < static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT); mux++)
    {
        setThermistorConfig(static_cast<ThermistorMux>(mux));
        RETURN_IF_ERR(configSync());
        RETURN_IF_ERR(io::adbms::startTempAdcConversion());
        io::time::delay(AUX_CONV_TIME_MS);
        io::adbms::readCellTempReg(regs[mux], regs_success[mux]);
    }

    for (size_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
    {
        for (size_t mux = 0U; mux < static_cast<size_t>(ThermistorMux::THERMISTOR_MUX_COUNT); mux++)
        {
            for (size_t gpio = 0U; gpio < io::adbms::THERM_GPIOS_PER_SEGMENT; gpio++)
            {
                size_t therm = gpio + mux * io::adbms::THERM_GPIOS_PER_SEGMENT;
                if (therm < io::THERMISTORS_PER_SEGMENT)
                {
                    out_temps[seg][therm] =
                        regs_success[mux][seg][gpio] ? convertRegToVoltage(regs[mux][seg][gpio]) : -0.1f;
                    out_success[seg][therm] = regs_success[mux][seg][gpio];
                }
            }
        }
    }
    return {};
}

expected<void, ErrorCode> runSegVoltageConversion(Segment &out_seg_voltages, SegmentSuccess &out_seg_success)
{
    array<uint16_t, io::NUM_SEGMENTS> seg_regs;

    RETURN_IF_ERR(io::adbms::startSegAdcConversion());
    io::time::delay(AUX_CONV_TIME_MS);
    io::adbms::readSegVoltageReg(seg_regs, out_seg_success);

    for (size_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
    {
        out_seg_voltages[seg] = out_seg_success[seg] ? convertRegToVoltage(seg_regs[seg]) : -0.1f;
    }
    return {};
}

expected<void, ErrorCode> runStatusConversion(Status &out_status, SegmentSuccess &out_success)
{
    RETURN_IF_ERR(io::adbms::clearStatReg());
    RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
    RETURN_IF_ERR(io::adbms::startTempAdcConversion());
    io::time::delay(AUX_CONV_TIME_MS);
    io::adbms::readStatusReg(out_status, out_success);

    return {};
}

expected<void, ErrorCode> runCellOpenWireCheck(Owc &out_owc_ok, CellSuccess &out_success)
{
    array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> baseline_regs, owc_odd_regs, owc_even_regs;
    array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> baseline_success, owc_odd_success,
        owc_even_success;

    RETURN_IF_ERR(io::adbms::startCellsBaseAdcConversion());
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellVoltageReg(baseline_regs, baseline_success);

    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::OddChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellVoltageReg(owc_odd_regs, owc_odd_success);

    RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::EvenChannels));
    io::time::delay(OWC_CONVERSION_TIME_MS);
    io::adbms::readCellVoltageReg(owc_even_regs, owc_even_success);

    for (size_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0; cell < io::CELLS_PER_SEGMENT; cell++)
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

            float baseline_v       = convertRegToVoltage(baseline_regs[seg][cell]);
            float owc_v            = (cell % 2 == 0) ? convertRegToVoltage(owc_odd_regs[seg][cell])
                                                     : convertRegToVoltage(owc_even_regs[seg][cell]);
            out_owc_ok[seg][cell]  = checkCellOwcOk(baseline_v, owc_v);
            out_success[seg][cell] = {};
        }
    }
    return {};
}
} // namespace app::segments
