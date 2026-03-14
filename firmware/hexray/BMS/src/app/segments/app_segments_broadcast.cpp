#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "app_canTx.hpp"


float pack_voltage;
app::segments::CellParam max_cell_voltage;
app::segments::CellParam min_cell_voltage;
app::segments::CellParam max_cell_temp;
app::segments::CellParam min_cell_temp;

constexpr float convertRegToVoltage(uint16_t reg) {
    return (reg * 150e-6f) + 1.5f;
}

namespace app::segments{
    void broadcastCellVoltages() {
        
        for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++) {
            for (size_t cell = 0U; cell < io::CELLS_PER_SEGMENT; cell++) {

                if (!cell_voltage_success[seg][cell]) {
                    //cell_voltage_setters[seg][cell](-0.1f);
                    continue;
                }

                const float voltage = convertRegToVoltage(cell_voltage_regs[seg][cell]);
                cell_voltages[seg][cell] = voltage;
                //cell_voltage_setters[seg][cell](voltage);
            }
        }
    }

    

    
}



