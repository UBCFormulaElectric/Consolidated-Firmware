#include <cfloat>
#include <cmath>

#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "app_canTx.hpp"

static constexpr float OW_RELATIVE_THRESHOLD = 0.70f;
static constexpr float OW_ABSOLUTE_THRESHOLD = 0.5f;

constexpr float convertRegToVoltage(uint16_t reg)
{
    return (static_cast<float>(static_cast<int16_t>(reg)) * 150e-6f) + 1.5f;
}

constexpr float convertRegToTemp(uint16_t reg) {
    float voltage = convertRegToVoltage(reg);
    float resistance = 10e3f * (voltage / (3.0f - voltage));
    float inv_temp = (1.0f/298.15f) + (1.0f/3610.0f) * std::log(resistance/10e3f);
    return (1.0f / inv_temp) - 273.15f;
}

constexpr bool checkOwcOk(float baselineVoltage, float owcVoltage) {
    if (owcVoltage < baselineVoltage * OW_RELATIVE_THRESHOLD) {
        return false;
    }
    if (owcVoltage < OW_ABSOLUTE_THRESHOLD) {
        return false;
    }
    return true;
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
        bool segment_comm_ok = false;
        for (size_t cell = 0U; cell < io::CELLS_PER_SEGMENT; cell++)
        {
            if (!cell_voltage_success[seg][cell])
            {

                cell_voltages[seg][cell] = -0.1f;
                cell_voltage_setters[seg][cell](-0.1f);
                continue;
            }

            segment_comm_ok = true;

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
        segment_comm_ok_setters[seg](segment_comm_ok);
    }

    max_cell_voltage = candidate_max_cell_voltage;
    min_cell_voltage = candidate_min_cell_voltage;
}

void broadcastFilteredCellVoltages()
{
    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++)
    {
        bool segment_comm_ok = false;
        for (size_t cell = 0U; cell < io::CELLS_PER_SEGMENT; cell++)
        {
            if (!filtered_cell_voltage_success[seg][cell])
            {
                filtered_cell_voltages[seg][cell] = -0.1f;
                filtered_cell_voltage_setters[seg][cell](-0.1f);
                continue;
            }
            segment_comm_ok = true;

            const float voltage      = convertRegToVoltage(cell_voltage_regs[seg][cell]);
            filtered_cell_voltages[seg][cell] = voltage;
            filtered_cell_voltage_setters[seg][cell](voltage);
        }
        segment_comm_ok_setters[seg](segment_comm_ok);
    }
    
}

void broadcastCellTemps() {
    CellParam candidate_max_cell_temp = { .segment = 0, .cell = 0, .voltage = 0.0f, .temp = __FLT_MIN__ };
    CellParam candidate_min_cell_temp = { .segment = 0, .cell = 0, .voltage = 0.0f, .temp = __FLT_MAX__ };

    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++) {
        bool segment_comm_ok = false;
        for (size_t mux = 0U; mux < static_cast<size_t> (ThermistorMux::THERMISTOR_MUX_COUNT); mux++) {
            for (size_t gpio = 0U; gpio < io::adbms::THERM_GPIOS_PER_SEGMENT; gpio++) {
                size_t cell = gpio + mux * io::adbms::THERM_GPIOS_PER_SEGMENT;
                if (cell >= io::CELLS_PER_SEGMENT) {
                    continue;
                }

                if (!cell_temp_success[mux][seg][gpio]) {
                    cell_temps[seg][cell] = -0.1f;
                    cell_temperature_setters[seg][cell](-0.1f);
                    continue;
                }
                segment_comm_ok = true;
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
        segment_comm_ok_setters[seg](segment_comm_ok);
    }
    max_cell_temp = candidate_max_cell_temp;
    min_cell_temp = candidate_min_cell_temp;
}

void broadcastStatus() {
    
}

void broadcastCellOpenWireCheck() {
    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++) {
        for (size_t cell = 0U; cell < io::CELLS_PER_SEGMENT; cell++) {
            
            
            if (!cell_baseline_success[seg][cell] || !cell_owc_even_success[seg][cell] || !cell_owc_odd_success[seg][cell])
            {
                cell_owc_ok[seg][cell] = false;
                cell_owc_setters[seg][cell](false);
                continue;
            }
            
            float baseline_voltage = convertRegToVoltage(cell_baseline_regs[seg][cell]);
            float owc_voltage = (cell % 2 == 0) ? convertRegToVoltage(cell_owc_odd_regs[seg][cell]) : convertRegToVoltage(cell_owc_even_regs[seg][cell]);
            cell_owc_ok[seg][cell] = checkOwcOk(baseline_voltage,owc_voltage);

            //need to disbale balancing 
            //maybe add sm number of consecutuive faults
    
        }
    }
}

void broadcastThermOpenWireCheck() {
    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++) {
        for (size_t gpio = 0U; gpio < io::adbms::THERM_GPIOS_PER_SEGMENT; gpio++) {
            if (!therm_owc_odd_success[seg][gpio] || !therm_owc_even_success[seg][gpio])
            {
                therm_owc_ok[seg][gpio] = false;
                therm_owc_setters[seg][gpio](false);
                continue;
            }  
        }
    }
}








} // namespace app::segments
