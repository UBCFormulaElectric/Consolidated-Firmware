#include <cfloat>
#include <cmath>

#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "app_canTx.hpp"
namespace app::segments
{

float     pack_voltage;
CellParam max_cell_voltage;
CellParam min_cell_voltage;
CellParam max_cell_temp;
CellParam min_cell_temp;

Cell        cell_voltages;
CellSuccess cell_voltage_success;
Cell        filtered_cell_voltages;
CellSuccess filtered_cell_voltage_success;
Therm       cell_temps;
Owc         cell_owc_ok;
Owc         therm_owc_ok;
Segment     seg_comm_ok;

void broadcastCellVoltages(const Cell &voltages, const CellSuccess &voltages_success)
{
    cell_voltages        = voltages;
    cell_voltage_success = voltages_success;

    CellParam candidate_max_cell_voltage = { .segment = 0, .cell = 0, .voltage = __FLT_MIN__, .temp = 0.0f };
    CellParam candidate_min_cell_voltage = { .segment = 0, .cell = 0, .voltage = __FLT_MAX__, .temp = 0.0f };

    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++)
    {
        bool seg_ok = true;
        for (size_t cell = 0U; cell < io::CELLS_PER_SEGMENT; cell++)
        {
            const float voltage = cell_voltages[seg][cell];
            cell_voltage_setters[seg][cell](voltage);

            if (!voltages_success[seg][cell])
            {
                seg_ok = false;
                continue;
            }

            if (voltage > candidate_max_cell_voltage.voltage)
            {
                candidate_max_cell_voltage.segment = static_cast<uint8_t>(seg);
                candidate_max_cell_voltage.cell    = static_cast<uint8_t>(cell);
                candidate_max_cell_voltage.voltage = voltage;
                candidate_max_cell_voltage.temp    = cell_temps[seg][cell];
            }
            if (voltage < candidate_min_cell_voltage.voltage)
            {
                candidate_min_cell_voltage.segment = static_cast<uint8_t>(seg);
                candidate_min_cell_voltage.cell    = static_cast<uint8_t>(cell);
                candidate_min_cell_voltage.voltage = voltage;
                candidate_min_cell_voltage.temp    = cell_temps[seg][cell];
            }
        }
        seg_comm_ok[seg] = seg_ok;
        segment_comm_ok_setters[seg](seg_ok);
    }

    max_cell_voltage = candidate_max_cell_voltage;
    min_cell_voltage = candidate_min_cell_voltage;
}

void broadcastFilteredCellVoltages(const Cell &voltages, const CellSuccess &voltages_success)
{
    filtered_cell_voltages        = voltages;
    filtered_cell_voltage_success = voltages_success;
    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++)
    {
        bool seg_ok = true;

        for (size_t cell = 0U; cell < io::CELLS_PER_SEGMENT; cell++)
        {
            filtered_cell_voltage_setters[seg][cell](voltages[seg][cell]);

            if (!voltages_success[seg][cell])
            {
                seg_ok = false;
            }
        }
        seg_comm_ok[seg] = seg_ok;
        segment_comm_ok_setters[seg](seg_ok);
    }
}

void broadcastTemps(const Therm &temps, const ThermSuccess &temps_success)
{
    CellParam candidate_max_cell_temp = { .segment = 0, .cell = 0, .voltage = 0.0f, .temp = __FLT_MIN__ };
    CellParam candidate_min_cell_temp = { .segment = 0, .cell = 0, .voltage = 0.0f, .temp = __FLT_MAX__ };

    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++)
    {
        bool seg_ok = true;
        for (size_t therm = 0U; therm < io::THERMISTORS_PER_SEGMENT; therm++)
        {
            const float temperature = temps[seg][therm];
            cell_temperature_setters[seg][therm](temperature);

            if (!temps_success[seg][therm])
            {
                seg_ok                   = false;
                therm_owc_ok[seg][therm] = false;
                therm_owc_setters[seg][therm](false);
                continue;
            }

            if (temperature < OW_THERM_THRESHOLD)
            {
                therm_owc_ok[seg][therm] = false;
                therm_owc_setters[seg][therm](false);
            }
            else
            {
                therm_owc_ok[seg][therm] = true;
                therm_owc_setters[seg][therm](true);
            }

            if (temperature > candidate_max_cell_temp.temp)
            {
                candidate_max_cell_temp.segment = static_cast<uint8_t>(seg);
                candidate_max_cell_temp.cell    = static_cast<uint8_t>(therm);
                candidate_max_cell_temp.voltage = cell_voltages[seg][therm];
                candidate_max_cell_temp.temp    = temperature;
            }
            if (temperature < candidate_min_cell_temp.temp)
            {
                candidate_min_cell_temp.segment = static_cast<uint8_t>(seg);
                candidate_min_cell_temp.cell    = static_cast<uint8_t>(therm);
                candidate_min_cell_temp.voltage = cell_voltages[seg][therm];
                candidate_min_cell_temp.temp    = temperature;
            }
        }
        seg_comm_ok[seg] = seg_ok;
        segment_comm_ok_setters[seg](seg_ok);
    }
    max_cell_temp = candidate_max_cell_temp;
    min_cell_temp = candidate_min_cell_temp;
}

void broadcastSegVoltages(const Segment &seg_voltages, const SegmentSuccess &seg_voltages_success)
{
    float   max_voltage = __FLT_MIN__;
    float   min_voltage = __FLT_MAX__;
    uint8_t max_seg     = 0;
    uint8_t min_seg     = 0;
    pack_voltage        = 0.0f;

    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++)
    {
        bool seg_ok      = static_cast<bool>(seg_voltages_success[seg]);
        seg_comm_ok[seg] = seg_ok;
        segment_comm_ok_setters[seg](seg_ok);

        segment_voltages_setters[seg](seg_voltages[seg]);

        if (!seg_ok)
            continue;

        pack_voltage += seg_voltages[seg];

        if (seg_voltages[seg] > max_voltage)
        {
            max_voltage = seg_voltages[seg];
            max_seg     = static_cast<uint8_t>(seg);
        }
        if (seg_voltages[seg] < min_voltage)
        {
            min_voltage = seg_voltages[seg];
            min_seg     = static_cast<uint8_t>(seg);
        }
    }

    app::can_tx::BMS_PackVoltage_set(pack_voltage);
    app::can_tx::BMS_MaxSegmentVoltage_Segment_set(max_seg);
    app::can_tx::BMS_MaxSegmentVoltage_Voltage_set(max_voltage);
    app::can_tx::BMS_MinSegmentVoltage_Segment_set(min_seg);
    app::can_tx::BMS_MinSegmentVoltage_Voltage_set(min_voltage);
}

void broadcastStatus(const Status &status, const SegmentSuccess &status_success)
{
    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++)
    {
        if (!status_success[seg])
        {
            // STATA
            segment_vref2_setters[seg](-0.1f);
            segment_itmp_setters[seg](-0.1f);
            // STATB
            segment_vd_setters[seg](-0.1f);
            segment_va_setters[seg](-0.1f);
            segment_vres_setters[seg](-0.1f);
            // STATC
            segment_va_ov_setters[seg](false);
            segment_va_uv_setters[seg](false);
            segment_vd_ov_setters[seg](false);
            segment_vd_uv_setters[seg](false);
            segment_ced_setters[seg](false);
            segment_cmed_setters[seg](false);
            segment_sed_setters[seg](false);
            segment_smed_setters[seg](false);
            segment_vde_setters[seg](false);
            segment_vdel_setters[seg](false);
            segment_thsd_setters[seg](false);
            segment_tmodchk_setters[seg](false);
            segment_oscchk_setters[seg](false);
            // STATD
            for (size_t cell = 0U; cell < io::CELLS_PER_SEGMENT; cell++)
            {
                cell_ov_setters[seg][cell](false);
                cell_uv_setters[seg][cell](false);
            }
            continue;
        }

        const auto &stat_a = status[seg].stat_a;
        const auto &stat_b = status[seg].stat_b;
        const auto &stat_c = status[seg].stat_c;
        const auto &stat_d = status[seg].stat_d;

        // STATA
        segment_vref2_setters[seg](convertRegToVoltage(stat_a.vref2));
        segment_itmp_setters[seg](convertRegToVoltage(stat_a.itmp));

        // STATB
        segment_vd_setters[seg](convertRegToVoltage(stat_b.vd));
        segment_va_setters[seg](convertRegToVoltage(stat_b.va));
        segment_vres_setters[seg](convertRegToVoltage(stat_b.vres));

        // STATC
        segment_va_ov_setters[seg](stat_c.va_ov);
        segment_va_uv_setters[seg](stat_c.va_uv);
        segment_vd_ov_setters[seg](stat_c.vd_ov);
        segment_vd_uv_setters[seg](stat_c.vd_uv);
        segment_ced_setters[seg](stat_c.ced);
        segment_cmed_setters[seg](stat_c.cmed);
        segment_sed_setters[seg](stat_c.sed);
        segment_smed_setters[seg](stat_c.smed);
        segment_vde_setters[seg](stat_c.vde);
        segment_vdel_setters[seg](stat_c.vdel);
        segment_thsd_setters[seg](stat_c.thsd);
        segment_tmodchk_setters[seg](stat_c.tmodchk);
        segment_oscchk_setters[seg](stat_c.oscchk);

        // STATD
        for (size_t cell = 0U; cell < io::CELLS_PER_SEGMENT; cell++)
        {
            cell_ov_setters[seg][cell](static_cast<bool>((stat_d.covuv >> (16U + cell)) & 1U));
            cell_uv_setters[seg][cell](static_cast<bool>((stat_d.covuv >> cell) & 1U));
        }
    }
}

void broadcastCellOpenWireCheck(const Owc &owc_ok, const CellSuccess owc_ok_success)
{
    cell_owc_ok = owc_ok;

    for (size_t seg = 0U; seg < io::NUM_SEGMENTS; seg++)
    {
        bool seg_ok = true;
        for (size_t cell = 0U; cell < io::CELLS_PER_SEGMENT; cell++)
        {
            cell_owc_setters[seg][cell](cell_owc_ok[seg][cell]);

            if (!owc_ok_success[seg][cell])
            {
                continue;
            }
            seg_ok = false;
        }
        seg_comm_ok[seg] = seg_ok;
        segment_comm_ok_setters[seg](seg_ok);
    }
}

void broadcastInfo()
{
    app::can_tx::BMS_MinCellVoltage_Voltage_set(min_cell_voltage.voltage);
    app::can_tx::BMS_MinCellVoltage_Temperature_set(min_cell_voltage.temp);
    app::can_tx::BMS_MinCellVoltage_Segment_set(min_cell_voltage.segment);
    app::can_tx::BMS_MinCellVoltage_Cell_set(min_cell_voltage.cell);

    app::can_tx::BMS_MaxCellVoltage_Voltage_set(max_cell_voltage.voltage);
    app::can_tx::BMS_MaxCellVoltage_Temperature_set(max_cell_voltage.temp);
    app::can_tx::BMS_MaxCellVoltage_Segment_set(max_cell_voltage.segment);
    app::can_tx::BMS_MaxCellVoltage_Cell_set(max_cell_voltage.cell);

    app::can_tx::BMS_MinCellTemperature_Voltage_set(min_cell_temp.voltage);
    app::can_tx::BMS_MinCellTemperature_Temperature_set(min_cell_temp.temp);
    app::can_tx::BMS_MinCellTemperature_Segment_set(min_cell_temp.segment);
    app::can_tx::BMS_MinCellTemperature_Cell_set(min_cell_temp.cell);

    app::can_tx::BMS_MaxCellTemperature_Voltage_set(max_cell_temp.voltage);
    app::can_tx::BMS_MaxCellTemperature_Temperature_set(max_cell_temp.temp);
    app::can_tx::BMS_MaxCellTemperature_Segment_set(max_cell_temp.segment);
    app::can_tx::BMS_MaxCellTemperature_Cell_set(max_cell_temp.cell);
}
} // namespace app::segments
