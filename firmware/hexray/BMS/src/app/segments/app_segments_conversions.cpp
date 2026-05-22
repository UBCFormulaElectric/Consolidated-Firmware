#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using io::adbms::ThermGpios;
using std::pair;

namespace app::segments
{
result<void> startPoll::cellOwcAdc(io::adbms::OpenWireSwitch owcSwitch) {
    RETURN_IF_ERR(io::adbms::command::owcCells(owcSwitch));
    io::time::delay(VOLT_CONV_TIME_MS);
    return io::adbms::command::pollCellsAdc();
}

result<void> startPoll::cellAdc() {
    RETURN_IF_ERR(io::adbms::command::startCellsAdc());
    io::time::delay(VOLT_CONV_TIME_MS);
    return io::adbms::command::pollCellsAdc();
}

result<void> startPoll::auxAdc(ThermistorMux mux) {
    config::setThermistorConfig(static_cast<ThermistorMux>(mux));
    RETURN_IF_ERR(io::adbms::command::startAuxAdc());

    io::time::delay(AUX_CONV_TIME_MS);
    return io::adbms::command::pollAuxAdc();
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

result<pair<Therms<result<float>>, Therms<result<bool>>>> conversion::thermTempOwc() {

    RETURN_IF_ERR(startPoll::auxAdc(ThermistorMux::THERMISTOR_MUX_0_7));
    ThermGpios<result<uint16_t>> therm_0_7_voltage = io::adbms::read::thermGpioVoltage();

    RETURN_IF_ERR(startPoll::auxAdc(ThermistorMux::THERMISTOR_MUX_8_13));
    ThermGpios<result<uint16_t>> therm_8_13_voltage = io::adbms::read::thermGpioVoltage();

    Therms<result<float>> out_temp;
    Therms<result<bool>> out_owc;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
        for (size_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++) {
            size_t therm_gpio = therm % THERM_GPIOS_PER_SEGMENT;

            if (therm < THERM_GPIOS_PER_SEGMENT) {
                if (!therm_0_7_voltage[seg][therm_gpio]) {
                out_temp[seg][therm] = std::unexpected(therm_0_7_voltage[seg][therm_gpio].error());
                out_owc[seg][therm] = std::unexpected(therm_0_7_voltage[seg][therm_gpio].error());
                } else {
                out_temp[seg][therm] = convertRegToTemp(therm_0_7_voltage[seg][therm_gpio].value());
                out_owc[seg][therm] = checkThermOwcOk(therm_0_7_voltage[seg][therm_gpio].value());
                }
            } else {
                if (!therm_8_13_voltage[seg][therm_gpio]) {
                out_temp[seg][therm] = std::unexpected(therm_8_13_voltage[seg][therm_gpio].error());
                out_owc[seg][therm] = std::unexpected(therm_8_13_voltage[seg][therm_gpio].error());
                } else {
                out_temp[seg][therm] = convertRegToTemp(therm_8_13_voltage[seg][therm_gpio].value());
                out_owc[seg][therm] = checkThermOwcOk(therm_8_13_voltage[seg][therm_gpio].value());
                }
            }
        }
    }

    return pair{out_temp,out_owc};
}

Segments<result<float>> conversion::segVoltage() {
    const Segments<result<uint16_t>> seg_voltage = io::adbms::read::segVoltage();

    Segments<result<float>> out_voltage;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
        if (!seg_voltage[seg]) {
            out_voltage[seg] = std::unexpected(seg_voltage[seg].error());
        } else {
            out_voltage[seg] = convertRegToVoltage(seg_voltage[seg].value());
        }
    }
        
    return out_voltage;
}

result<Segments<io::adbms::StatusGroups>> conversion::status() {
    RETURN_IF_ERR(io::adbms::clear::stat());
    return io::adbms::read::status();
}

result<Cells<result<bool>>> conversion::cellOwc() {
    RETURN_IF_ERR(startPoll::cellAdc());
    Cells<result<uint16_t>> baseline_voltage = io::adbms::read::cellVoltage();

    RETURN_IF_ERR(startPoll::cellOwcAdc(io::adbms::OpenWireSwitch::OddChannels));
    Cells<result<uint16_t>> odd_voltage = io::adbms::read::cellVoltage();

    RETURN_IF_ERR(startPoll::cellOwcAdc(io::adbms::OpenWireSwitch::EvenChannels));
    Cells<result<uint16_t>> even_voltage = io::adbms::read::cellVoltage();
    
    Cells<result<bool>> owc_cell;
    for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
        for (size_t cell = 0; cell < CELLS_PER_SEGMENT; cell++) {
            if (!baseline_voltage[seg][cell])
            {
                owc_cell[seg][cell] = std::unexpected(baseline_voltage[seg][cell].error());
                continue;
            }
            if (!odd_voltage[seg][cell])
            {
                owc_cell[seg][cell] = std::unexpected(odd_voltage[seg][cell].error());
                continue;
            }
            if (!even_voltage[seg][cell])
            {
                owc_cell[seg][cell] = std::unexpected(even_voltage[seg][cell].error());
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
