#include <cfloat>

#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "app_canTx.hpp"


constexpr float convertRegToVoltage(uint16_t reg)
{
    return (reg * 150e-6f) + 1.5f;
}

namespace app::segments
{

float     pack_voltage;
CellParam max_cell_voltage;
CellParam min_cell_voltage;
CellParam max_cell_temp;
CellParam min_cell_temp;
void      broadcastCellVoltages()
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

} // namespace app::segments
