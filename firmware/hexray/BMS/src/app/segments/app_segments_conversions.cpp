// #include "app_segments.hpp"
// #include "app_segments_internal.hpp"
// #include "io_adbms.hpp"
// #include "io_time.hpp"
// #include "util_errorCodes.hpp"

// using namespace std;

// //check later
// static constexpr uint8_t VOLT_CONV_TIME_MS = 2U;
// static constexpr uint8_t TEMP_CONV_TIME_MS = 2U;

// array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltage_regs;
// array<array<std::expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltage_success;
// array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltages;

// array<array<uint16_t, io::adbms::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_temp_regs;
// array<array<std::expected<void, ErrorCode>, io::adbms::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_temp_success;
// array<array<float, io::adbms::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_temps;



// namespace app::segments {
//     expected<void, ErrorCode> runVoltageConversion() {
//         RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
//         io::time::delay(VOLT_CONV_TIME_MS);
//         io::adbms::readCellVoltageReg(cell_voltage_regs, cell_voltage_success);
//         return {};
//     }

//     expected<void, ErrorCode> runAuxConversion() {
//         for (ThermistorMux mux = 0U; mux < ThermistorMux::THERMISTOR_MUX_COUNT; mux++)
//         {
//             setThermistorMuxConfig(mux);
            

//             RETURN_IF_ERR(io::adbms::startTempAdcConversion());
//             io::time::delay(TEMP_CONV_TIME_MS);
//             io::adbms::readCellTempReg(cell_temp_regs, cell_temp_success);
//         }


//         RETURN_IF_ERR(io::adbms::startTempAdcConversion());
//         io::time::delay(TEMP_CONV_TIME_MS);
//         io::adbms::readCellTempReg(cell_temp_regs, cell_temp_success);
//         return {};
//     }
// }