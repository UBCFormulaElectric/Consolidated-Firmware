#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using io::adbms::ThermGpios;
using std::pair;

namespace
{
template <typename Arr>
void fillWithError(Arr &arr, ErrorCode err)
{
    for (auto &seg : arr) seg.fill(std::unexpected(err));
}
} // namespace

namespace app::segments
{
result<void> startPoll::cellOwcAdc(io::adbms::OpenWireSwitch owcSwitch)
{
    const auto start = io::adbms::command::owcCells(owcSwitch);
    if (!start)
    {
        health::setAll(health::ErrorBit::OwcAdcPoll);
        return std::unexpected(start.error());
    }
    io::time::delay(VOLT_CONV_TIME_MS);
    const auto poll = io::adbms::command::pollCellsAdc();
    if (!poll)
    {
        health::setAll(health::ErrorBit::OwcAdcPoll);
        return std::unexpected(poll.error());
    }
    health::resetAll(health::ErrorBit::OwcAdcPoll);
    return {};
}

result<void> startPoll::cellAdc()
{
    const auto start = io::adbms::command::startCellsAdc();
    if (!start)
    {
        health::setAll(health::ErrorBit::CellAdcPoll);
        return std::unexpected(start.error());
    }
    io::time::delay(VOLT_CONV_TIME_MS);
    const auto poll = io::adbms::command::pollCellsAdc();
    if (!poll)
    {
        health::setAll(health::ErrorBit::CellAdcPoll);
        return std::unexpected(poll.error());
    }
    health::resetAll(health::ErrorBit::CellAdcPoll);
    return {};
}

result<void> startPoll::auxAdc(ThermistorMux mux)
{
    config::setThermistorConfig(static_cast<ThermistorMux>(mux));
    const auto start = io::adbms::command::startAuxAdc();
    if (!start)
    {
        health::setAll(health::ErrorBit::AuxAdcPoll);
        return std::unexpected(start.error());
    }
    io::time::delay(AUX_CONV_TIME_MS);
    const auto poll = io::adbms::command::pollAuxAdc();
    if (!poll)
    {
        health::setAll(health::ErrorBit::AuxAdcPoll);
        return std::unexpected(poll.error());
    }
    health::resetAll(health::ErrorBit::AuxAdcPoll);
    return {};
}

Cells<result<float>> conversion::cellVoltage()
{
    Cells<result<float>> out_voltage;
    if (!startPoll::cellAdc())
    {
        fillWithError(out_voltage, ErrorCode::POLL_INVALID);
        return out_voltage;
    }

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

        health::setOrReset(seg, health::ErrorBit::CellVoltage, seg_has_error);
    }
    return out_voltage;
}

pair<Therms<result<float>>, Therms<result<bool>>> conversion::thermTempOwc()
{
    Therms<result<float>> out_temp;
    Therms<result<bool>>  out_owc;

    if (!startPoll::auxAdc(ThermistorMux::THERMISTOR_MUX_0_7))
    {
        fillWithError(out_temp, ErrorCode::POLL_INVALID);
        fillWithError(out_owc, ErrorCode::POLL_INVALID);
        return pair{ out_temp, out_owc };
    }
    ThermGpios<result<uint16_t>> therm_0_7_voltage = io::adbms::read::thermGpioVoltage();

    if (!startPoll::auxAdc(ThermistorMux::THERMISTOR_MUX_8_13))
    {
        fillWithError(out_temp, ErrorCode::POLL_INVALID);
        fillWithError(out_owc, ErrorCode::POLL_INVALID);
        return pair{ out_temp, out_owc };
    }
    ThermGpios<result<uint16_t>> therm_8_13_voltage = io::adbms::read::thermGpioVoltage();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        bool seg_has_error = false;
        for (size_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
        {
            size_t therm_gpio = therm % THERM_GPIOS_PER_SEGMENT;

            if (therm < THERM_GPIOS_PER_SEGMENT)
            {
                if (!therm_0_7_voltage[seg][therm_gpio])
                {
                    out_temp[seg][therm] = std::unexpected(therm_0_7_voltage[seg][therm_gpio].error());
                    out_owc[seg][therm]  = std::unexpected(therm_0_7_voltage[seg][therm_gpio].error());
                    seg_has_error        = true;
                }
                else
                {
                    out_temp[seg][therm] = convertRegToTemp(therm_0_7_voltage[seg][therm_gpio].value());
                    out_owc[seg][therm]  = checkThermOwcOk(therm_0_7_voltage[seg][therm_gpio].value());
                }
            }
            else
            {
                if (!therm_8_13_voltage[seg][therm_gpio])
                {
                    out_temp[seg][therm] = std::unexpected(therm_8_13_voltage[seg][therm_gpio].error());
                    out_owc[seg][therm]  = std::unexpected(therm_8_13_voltage[seg][therm_gpio].error());
                    seg_has_error        = true;
                }
                else
                {
                    out_temp[seg][therm] = convertRegToTemp(therm_8_13_voltage[seg][therm_gpio].value());
                    out_owc[seg][therm]  = checkThermOwcOk(therm_8_13_voltage[seg][therm_gpio].value());
                }
            }
        }

        health::setOrReset(seg, health::ErrorBit::ThermTemp, seg_has_error);
        health::setOrReset(seg, health::ErrorBit::ThermOwc, seg_has_error);
    }

    return pair{ out_temp, out_owc };
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

        health::setOrReset(seg, health::ErrorBit::SegVoltage, seg_has_error);
    }
    return out_voltage;
}

Segments<io::adbms::StatusGroups> conversion::status()
{
    
    Segments<io::adbms::StatusGroups> status = io::adbms::read::status();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        const bool seg_has_error = !status[seg].stat_a || !status[seg].stat_b || !status[seg].stat_c ||
                                   !status[seg].stat_d || !status[seg].stat_e;

        health::setOrReset(seg, health::ErrorBit::Status, seg_has_error);
    }
    return status;
}

Cells<result<bool>> conversion::cellOwc()
{
    Cells<result<bool>> owc_cell;

    if (!startPoll::cellAdc())
    {
        fillWithError(owc_cell, ErrorCode::POLL_INVALID);
        return owc_cell;
    }
    Cells<result<uint16_t>> baseline_voltage = io::adbms::read::cellVoltage();

    if (!startPoll::cellOwcAdc(io::adbms::OpenWireSwitch::OddChannels))
    {
        fillWithError(owc_cell, ErrorCode::POLL_INVALID);
        return owc_cell;
    }
    Cells<result<uint16_t>> odd_voltage = io::adbms::read::cellVoltage();

    if (!startPoll::cellOwcAdc(io::adbms::OpenWireSwitch::EvenChannels))
    {
        fillWithError(owc_cell, ErrorCode::POLL_INVALID);
        return owc_cell;
    }
    Cells<result<uint16_t>> even_voltage = io::adbms::read::cellVoltage();

    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        bool seg_has_error = false;
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!baseline_voltage[seg][cell])
            {
                owc_cell[seg][cell] = std::unexpected(baseline_voltage[seg][cell].error());
                seg_has_error       = true;
                continue;
            }
            if (!odd_voltage[seg][cell])
            {
                owc_cell[seg][cell] = std::unexpected(odd_voltage[seg][cell].error());
                seg_has_error       = true;

                continue;
            }
            if (!even_voltage[seg][cell])
            {
                owc_cell[seg][cell] = std::unexpected(even_voltage[seg][cell].error());
                seg_has_error       = true;
                continue;
            }
            const float baseline_v = convertRegToVoltage(baseline_voltage[seg][cell].value());
            const float owc_v      = (cell % 2 == 0) ? convertRegToVoltage(odd_voltage[seg][cell].value())
                                                     : convertRegToVoltage(even_voltage[seg][cell].value());
            owc_cell[seg][cell]    = checkCellOwcOk(baseline_v, owc_v);
        }

        health::setOrReset(seg, health::ErrorBit::CellOwc, seg_has_error);
    }
    return owc_cell;
}
} // namespace app::segments
