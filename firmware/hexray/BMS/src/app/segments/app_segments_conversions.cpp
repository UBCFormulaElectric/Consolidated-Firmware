#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"
#include "util_retry.hpp"

namespace app::segments
{
result<void> startPoll::cellOwcAdc(io::adbms::OpenWireSwitch owcSwitch) {
    RETURN_IF_ERR(io::adbms::command::owcCells(owcSwitch));
    io::time::delay(VOLT_CONV_TIME_MS);
    return util::retry(
        []() -> result<void>
        {
            io::time::delay(1);
            return io::adbms::command::pollCellsAdcConversion();
        },
        POLL_RETRIES);
}

result<void> startPoll::cellAdc() {
    RETURN_IF_ERR(io::adbms::command::startCellsAdcConversion());
    io::time::delay(VOLT_CONV_TIME_MS);
    return util::retry(
        []() -> result<void>
        {
            io::time::delay(1);
            return io::adbms::command::pollCellsAdcConversion();
        },
        POLL_RETRIES);
}

result<void> startPoll::auxAdc(ThermistorMux mux) {
    config::setThermistorConfig(static_cast<ThermistorMux>(mux));
    RETURN_IF_ERR(io::adbms::command::startAuxAdcConversion());
    io::time::delay(AUX_CONV_TIME_MS);
    return util::retry(
        []() -> result<void>
        {
            io::time::delay(1);
            return io::adbms::command::pollAuxAdcConversion();
        },
        POLL_RETRIES);
}

Cells<result<float>> conversion::cellVoltage() {
    const Cells<result<uint16_t>> cell_voltage = io::adbms::read::cellVoltage();

    Cells<result<float>> out_voltage;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++) {
            if (!cell_voltage[seg][cell]) {
                out_voltage[seg][cell] = std::unexpected(cell_voltage[seg][cell].error());
            } else {
                out_voltage[seg][cell] = convertRegToVoltage(cell_voltage[seg][cell].value());
            }
        }
    }
    return out_voltage;
}

result<pair<ThermGpios<result<float>>, ThermGpios<result<bool>>>> conversion::thermTempOwc(ThermistorMux mux) {
    RETURN_IF_ERR(startPoll::auxAdc(mux));
    const ThermGpios<result<uint16_t>> therm_voltage = io::adbms::read::thermVoltage();

    ThermGpios<result<float>> out_temp;
    ThermGpios<result<bool>> out_owc;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
        for (size_t thermGpio = 0; thermGpio < THERM_GPIOS_PER_SEGMENT; thermGpio++) {
            if (const auto &reg_result = aux_regs[mux][seg][gpio]; !reg_result) {
                out_temps[seg][therm] = std::unexpected(reg_result.error());
                out_owc[seg][therm]   = std::unexpected(reg_result.error());
            } else {
                out_temps[seg][therm] = convertRegToTemp(reg_result.value());
                out_owc[seg][therm]   = checkThermOwcOk(reg_result.value());
            }
        }
    }
    return pair{out_temp,out_owc};
}

Segments<result<float>> conversion::segVoltage() {
    const Segment<result<uint16_t>> seg_voltage = io::adbms::read::segVoltage();

    Segment<result<float>> out_voltage;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
        if (!seg_voltage[seg]) {
            out_voltage[seg] = std::unexpected(cell_voltage[seg].error());
        } else {
            out_voltage[seg] = convertRegToVoltage(cell_voltage[seg].value());
        }
    }
        
    return out_voltage;
}

result<Segment<result<StatusGroups>>> conversion::status() {
    RETURN_IF_ERR(io::adbms::clear::stat());
    return io::adbms::read::status();
}

result<Cell<result<bool>>> conversion::cellOwc() {
    RETURN_IF_ERR(startPoll::cellAdc());
    Cell<result<float>> baseline_voltage = io::adbms::read::cellVoltage();

    RETURN_IF_ERR(startPoll::cellOwcAdc(io::adbms::OpenWireSwitch::OddChannels));
    Cell<result<float>> odd_voltage = io::adbms::read::cellVoltage();

    RETURN_IF_ERR(startPoll::cellOwcAdc(io::adbms::OpenWireSwitch::EvenChannels));
    Cell<result<float>> even_voltage = io::adbms::read::cellVoltage();
    
    Cell<result<bool>> owc_cell;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++) {
            if (!baseline_voltage[seg][cell])
            {
                owc_cell[seg][cell] = unexpected(baseline_voltage[seg][cell].error());
                continue;
            }
            if (!odd_voltage[seg][cell])
            {
                owc_cell[seg][cell] = unexpected(odd_voltage[seg][cell].error());
                continue;
            }
            if (!even_voltage[seg][cell])
            {
                owc_cell[seg][cell] = unexpected(even_voltage[seg][cell].error());
                continue;
            }
            const float baseline_v = convertRegToVoltage(baseline_voltage[seg][cell].value());
            const float owc_v = (cell % 2 == 0) ? convertRegToVoltage(odd_voltage[seg][cell].value()) : convertRegToVoltage(even_voltage[seg][cell].value());
            owc_cell[seg][cell] = checkCellOwcOk(baseline_v, owc_v);
        }
    }
    return owc_cell;
}
} // namespace app::segments
