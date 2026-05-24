#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using io::adbms::ThermGpios;
using std::pair;

namespace
{
template <typename Arr> void fillWithError(Arr &arr, ErrorCode err)
{
    for (auto &seg : arr)
        seg.fill(std::unexpected(err));
}
} // namespace

namespace app::segments
{
result<void> startPoll::secondaryCellAdc(const io::adbms::OpenWireSwitch owcSwitch)
{
    if (const auto start = io::adbms::command::owcCells(owcSwitch); !start)
    {
        health::setAll(health::ErrorBit::OWC_ADC_POLL);
        return std::unexpected(start.error());
    }
    io::time::delay(SECONDARY_CELL_CONV_TIME_MS);
    if (const auto poll = io::adbms::command::pollSecondaryCellsAdc(); !poll)
    {
        health::setAll(health::ErrorBit::OWC_ADC_POLL);
        return std::unexpected(poll.error());
    }
    health::resetAll(health::ErrorBit::OWC_ADC_POLL);
    return {};
}

result<void> startPoll::cellAdc()
{
    if (const auto start = io::adbms::command::startCellsAdc(); !start)
    {
        health::setAll(health::ErrorBit::CELL_ADC_POLL);
        return std::unexpected(start.error());
    }
    io::time::delay(CELL_CONV_TIME_MS);
    if (const auto poll = io::adbms::command::pollCellsAdc(); !poll)
    {
        health::setAll(health::ErrorBit::CELL_ADC_POLL);
        return std::unexpected(poll.error());
    }
    health::resetAll(health::ErrorBit::CELL_ADC_POLL);
    return {};
}

result<void> startPoll::auxAdc(const ThermistorMux mux)
{
    config::setThermistorConfig(mux);
    if (const auto start = io::adbms::command::startAuxAdc(); !start)
    {
        health::setAll(health::ErrorBit::AUX_ADC_POLL);
        return std::unexpected(start.error());
    }
    io::time::delay(AUX_CONV_TIME_MS);
    if (const auto poll = io::adbms::command::pollAuxAdc(); !poll)
    {
        health::setAll(health::ErrorBit::AUX_ADC_POLL);
        return std::unexpected(poll.error());
    }
    health::resetAll(health::ErrorBit::AUX_ADC_POLL);
    return {};
}

Cells<result<float>> conversion::cellVoltage()
{
    Cells<result<float>>          out_voltage;
    const Cells<result<uint16_t>> cell_voltage = io::adbms::read::cellVoltage();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        bool seg_has_error = false;
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!cell_voltage[seg][cell])
            {
                out_voltage[seg][cell] = std::unexpected(cell_voltage[seg][cell].error());
                seg_has_error          = true;
            }
            else
            {
                out_voltage[seg][cell] = convertRegToVoltage(cell_voltage[seg][cell].value());
            }
        }

        health::setOrReset(seg, health::ErrorBit::CELL_VOLTAGE, seg_has_error);
    }
    return out_voltage;
}

ThermGpios<result<float>> conversion::thermVoltage(const ThermistorMux mux)
{
    ThermGpios<result<float>>          out_therm;
    const ThermGpios<result<uint16_t>> therm_voltage = io::adbms::read::thermGpioVoltage();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        bool seg_has_error = false;
        for (size_t gpio = 0; gpio < THERM_GPIOS_PER_SEGMENT; gpio++)
        {
            if (!therm_voltage[seg][gpio])
            {
                out_therm[seg][gpio] = std::unexpected(therm_voltage[seg][gpio].error());
                seg_has_error        = true;
            }
            else
            {
                out_therm[seg][gpio] = convertRegToVoltage(therm_voltage[seg][gpio].value());
            }
        }
        health::setOrReset(seg, health::ErrorBit::THERM_VOLTAGE, seg_has_error);
    }
    return out_therm;
}

Segments<result<float>> conversion::segVoltage()
{
    const Segments<result<uint16_t>> seg_voltage = io::adbms::read::segVoltage();

    Segments<result<float>> out_voltage;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        bool seg_has_error = false;
        if (!seg_voltage[seg])
        {
            out_voltage[seg] = std::unexpected(seg_voltage[seg].error());
            seg_has_error    = true;
        }
        else
        {
            out_voltage[seg] = convertRegToVoltage(seg_voltage[seg].value());
        }

        health::setOrReset(seg, health::ErrorBit::SEG_VOLTAGE, seg_has_error);
    }
    return out_voltage;
}

Segments<io::adbms::StatusGroupsRes> conversion::status()
{
    Segments<io::adbms::StatusGroupsRes> status = io::adbms::read::status();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        const bool seg_has_error = !status[seg].stat_a || !status[seg].stat_b || !status[seg].stat_c ||
                                   !status[seg].stat_d || !status[seg].stat_e;

        health::setOrReset(seg, health::ErrorBit::STATUS, seg_has_error);
    }
    return status;
}

Cells<result<float>> conversion::cellOwcVoltages()
{
    Cells<result<float>>          out_cell_owc;
    const Cells<result<uint16_t>> cell_owc_voltage = io::adbms::read::secondaryCellVoltage();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        bool seg_has_error = false;
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!cell_owc_voltage[seg][cell])
            {
                out_cell_owc[seg][cell] = std::unexpected(cell_owc_voltage[seg][cell].error());
                seg_has_error           = true;
            }
            else
            {
                out_cell_owc[seg][cell] = convertRegToVoltage(cell_owc_voltage[seg][cell].value());
            }
        }
        health::setOrReset(seg, health::ErrorBit::CELL_OWC_VOLTAGE, seg_has_error);
    }
    return out_cell_owc;
}
} // namespace app::segments
