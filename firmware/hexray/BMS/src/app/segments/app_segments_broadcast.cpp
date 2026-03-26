#include <cfloat>
#include <cmath>

#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "app_canTx.hpp"


constexpr float convertRegToVoltage(uint16_t reg)
{
    return (static_cast<float>(reg) * 150e-6f) + 1.5f;
}

constexpr float convertRegToTemp(uint16_t reg) {
    // float voltage = convertRegToVoltage(reg);
    // float resistance = 10e3f * (voltage / (3.3f - voltage));
    // float inv_temp = (1.0f/298.15f) + (1.0f/3610.0f) * std::log(resistance/10e3f);
    // return (1.0f / inv_temp) - 273.15f;
    return convertRegToVoltage(reg);
}

namespace app::segments
{

float pack_voltage;
CellParam max_cell_voltage;
CellParam min_cell_voltage;
CellParam max_cell_temp;
CellParam min_cell_temp;

void broadcastCellVoltages()
{
    CellParam candidate_max_cell_voltage = { .segment = 0, .cell = 0, .voltage = __FLT_MIN__, .temp = 0.0f  };
    CellParam candidate_min_cell_voltage = { .segment = 0, .cell = 0, .voltage = __FLT_MAX__, .temp = 0.0f };

    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0U; cell < io::CELLS_PER_SEGMENT; cell++)
        {
            if (!cell_voltage_success[seg][cell])
            {
                cell_voltage_setters[seg][cell](-0.1f);
                continue;
            }

            const float voltage      = convertRegToVoltage(cell_voltage_regs[seg][cell]);
            cell_voltages[seg][cell] = voltage;
            cell_voltage_setters[seg][cell](voltage);

            if (voltage > candidate_max_cell_voltage.voltage) {
                candidate_max_cell_voltage.segment = static_cast<uint8_t> (seg);
                candidate_max_cell_voltage.cell = static_cast<uint8_t> (cell);
                candidate_max_cell_voltage.voltage = voltage;
                candidate_max_cell_voltage.temp = cell_temps[seg][cell];
            }
            if (voltage < candidate_min_cell_voltage.voltage) {
                candidate_min_cell_voltage.segment = static_cast<uint8_t> (seg);
                candidate_min_cell_voltage.cell = static_cast<uint8_t> (cell);
                candidate_min_cell_voltage.voltage = voltage;
                candidate_min_cell_voltage.temp = cell_temps[seg][cell];
            }
        }
    }

    max_cell_voltage = candidate_max_cell_voltage;
    min_cell_voltage = candidate_min_cell_voltage;
    
}

void broadcastCellTemps() {
    CellParam candidate_max_cell_temp = { .segment = 0, .cell = 0, .voltage = 0.0f, .temp = __FLT_MIN__ };
    CellParam candidate_min_cell_temp = { .segment = 0, .cell = 0, .voltage = 0.0f, .temp = __FLT_MAX__ };

    
    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++) {
        for (size_t mux = 0U; mux < static_cast<size_t> (ThermistorMux::THERMISTOR_MUX_COUNT); mux++) {
            for (size_t gpio = 0U; gpio < io::adbms::THERM_GPIOS_PER_SEGMENT; gpio++) {
                size_t cell = gpio + mux * 7U;
                if (cell >= io::CELLS_PER_SEGMENT) {
                    continue;
                }

                if (!cell_temp_success[mux][seg][gpio]) {
                    cell_temperature_setters[seg][cell](-0.1f);
                    continue;
                }
                const float temperature = convertRegToTemp(cell_temp_regs[mux][seg][gpio]);
                cell_temps[seg][cell] = temperature;
                cell_temperature_setters[seg][cell](temperature);
                if (temperature > candidate_max_cell_temp.temp) {
                    candidate_max_cell_temp.segment = static_cast<uint8_t> (seg);
                    candidate_max_cell_temp.cell = static_cast<uint8_t> (cell);
                    candidate_max_cell_temp.voltage = cell_voltages[seg][cell];
                    candidate_max_cell_temp.temp = temperature;
                }
                if (temperature < candidate_min_cell_temp.temp) {
                    candidate_min_cell_temp.segment = static_cast<uint8_t> (seg);
                    candidate_min_cell_temp.cell = static_cast<uint8_t> (cell);
                    candidate_min_cell_temp.voltage = cell_voltages[seg][cell];
                    candidate_min_cell_temp.temp = temperature;
                }
            
            }
        }
    }
}





} // namespace app::segments
