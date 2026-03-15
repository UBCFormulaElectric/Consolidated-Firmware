#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "io_adbms.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

using namespace std;

//check later
static constexpr uint8_t VOLT_CONV_TIME_MS      = 2U;
static constexpr uint8_t TEMP_CONV_TIME_MS      = 2U;
static constexpr uint8_t OWC_CONVERSION_TIME_MS = 2U;

array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltage_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltage_success;
array<array<float, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_voltages;

array<array<uint16_t, io::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_temp_regs;
array<array<expected<void, ErrorCode>, io::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_temp_success;
array<array<float, io::THERMISTORS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_temps;

array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_odd_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_odd_success;
array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_even_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> cell_owc_even_success;

array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_odd_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_odd_success;
array<array<uint16_t, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_even_regs;
array<array<expected<void, ErrorCode>, io::CELLS_PER_SEGMENT>, io::NUM_SEGMENTS> therm_owc_even_success;



namespace app::segments {
    expected<void, ErrorCode> runVoltageConversion() {
        RETURN_IF_ERR(io::adbms::startCellsAdcConversion());
        io::time::delay(VOLT_CONV_TIME_MS);
        io::adbms::readCellVoltageReg(cell_voltage_regs, cell_voltage_success);
        return {};
    }

    expected<void, ErrorCode> runAuxConversion() {
        for (uint8_t i = 0U; i < static_cast<uint8_t>(ThermistorMux::THERMISTOR_MUX_COUNT); i++) {
            const ThermistorMux mux = static_cast<ThermistorMux>(i);
            setThermistorConfig(mux);
            writeConfig();

            RETURN_IF_ERR(io::adbms::startTempAdcConversion());
            io::time::delay(TEMP_CONV_TIME_MS);
            io::adbms::readCellTempReg(cell_temp_regs, cell_temp_success);
        }
        return {};
    }

    expected<void, ErrorCode> runCellOwc() {
        RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::OddChannels));
        io::time::delay(OWC_CONVERSION_TIME_MS);
        RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::OddChannels));
        io::time::delay(OWC_CONVERSION_TIME_MS);
        io::adbms::readCellVoltageReg(therm_owc_odd_regs, therm_owc_odd_success);

        RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::EvenChannels));
        io::time::delay(OWC_CONVERSION_TIME_MS);
        RETURN_IF_ERR(io::adbms::owcCells(io::adbms::OpenWireSwitch::EvenChannels));
        io::time::delay(OWC_CONVERSION_TIME_MS);
        io::adbms::readCellVoltageReg(therm_owc_even_regs, therm_owc_even_success);
        return {};
    }

    expected<void, ErrorCode> runThermOpenWireCheck() {
        return {};
    }

    expected<void, ErrorCode> runStatusConversion() {
        return {};
    }

    expected<void, ErrorCode> runAdcAccuracyTest() {
        return {};
    }

    
 
}