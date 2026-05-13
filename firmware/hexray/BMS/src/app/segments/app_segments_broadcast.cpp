#include <cmath>

#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "app_canTx.hpp"

namespace
{
// float                    pack_voltage;
// app::segments::CellParam max_cell_voltage;
// app::segments::CellParam min_cell_voltage;
// app::segments::CellParam max_cell_temp;
// app::segments::CellParam min_cell_temp;
// Cells<float>  cell_voltages;
// CellSuccess   cell_voltage_success;
// Cells<float>  filtered_cell_voltages;
// CellSuccess   filtered_cell_voltage_success;
// Therms<float> cell_temps;
// Owc           cell_owc_ok;
// Owc           therm_owc_ok;
// Segments      seg_comm_ok;

template <typename T> class CellBroadcaster
{
    std::span<T, 4 * CELLS_PER_SEGMENT> _segments0_3;
    std::span<T, 4 * CELLS_PER_SEGMENT> _segments4_7;
    std::span<T, 2 * CELLS_PER_SEGMENT> _segments8_9;

  public:
    template <typename seg03, typename seg47, typename seg89>
    CellBroadcaster(seg03 segments0_3_can, seg47 segments4_7_can, seg89 segments8_9_can)
      : _segments0_3(
            std::span<T, 4 * CELLS_PER_SEGMENT>{ reinterpret_cast<T *>(&segments0_3_can), 4 * CELLS_PER_SEGMENT }),
        _segments4_7(
            std::span<T, 4 * CELLS_PER_SEGMENT>{ reinterpret_cast<T *>(&segments4_7_can), 4 * CELLS_PER_SEGMENT }),
        _segments8_9(
            std::span<T, 2 * CELLS_PER_SEGMENT>{ reinterpret_cast<T *>(&segments8_9_can), 2 * CELLS_PER_SEGMENT })
    {
        static_assert(sizeof(seg03) / sizeof(T) == 4 * CELLS_PER_SEGMENT);
        static_assert(sizeof(seg47) / sizeof(T) == 4 * CELLS_PER_SEGMENT);
        static_assert(sizeof(seg89) / sizeof(T) == 2 * CELLS_PER_SEGMENT);
    }

    std::span<T, CELLS_PER_SEGMENT> operator[](const size_t seg) const
    {
        if (seg < 4)
        {
            return std::span<T, CELLS_PER_SEGMENT>{ _segments0_3.data() + seg * CELLS_PER_SEGMENT, CELLS_PER_SEGMENT };
        }
        if (seg < 8)
        {
            return std::span<T, CELLS_PER_SEGMENT>{ _segments4_7.data() + (seg - 4) * CELLS_PER_SEGMENT,
                                                    CELLS_PER_SEGMENT };
        }
        return std::span<T, CELLS_PER_SEGMENT>{ _segments8_9.data() + (seg - 8) * CELLS_PER_SEGMENT,
                                                CELLS_PER_SEGMENT };
    }
};

template <typename S, typename CanMsg> constexpr std::span<S, MAX_NUM_SEGMENTS> SegmentBroadcastBuffer(CanMsg &can_msg)
{
    static_assert(sizeof(can_msg) == MAX_NUM_SEGMENTS * sizeof(S));
    return std::span<S, MAX_NUM_SEGMENTS>{ reinterpret_cast<S *>(&can_msg), MAX_NUM_SEGMENTS };
}

CellBroadcaster<float> cell_voltage_setters(
    app::can_tx::BMS_CellVoltages_Seg0_Seg3_getData(),
    app::can_tx::BMS_CellVoltages_Seg4_Seg7_getData(),
    app::can_tx::BMS_CellVoltages_Seg8_Seg9_getData());
CellBroadcaster<float> cell_fvoltage_setters(
    app::can_tx::BMS_FilteredCellVoltages_Seg0_Seg3_getData(),
    app::can_tx::BMS_FilteredCellVoltages_Seg4_Seg7_getData(),
    app::can_tx::BMS_FilteredCellVoltages_Seg8_Seg9_getData());
CellBroadcaster<float> cell_temperature_setters(
    app::can_tx::BMS_CellTemps_Seg0_Seg3_getData(),
    app::can_tx::BMS_CellTemps_Seg4_Seg7_getData(),
    app::can_tx::BMS_CellTemps_Seg8_Seg9_getData());

const std::span<bool, MAX_NUM_SEGMENTS> seg_ok_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentCommOk_getData());
const std::span<float, MAX_NUM_SEGMENTS> seg_voltage_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentVoltages_getData());
const std::span<float, MAX_NUM_SEGMENTS> seg_vref2_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentVref2_getData());
const std::span<float, MAX_NUM_SEGMENTS> segment_itmp_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentITMP_getData());
const std::span<float, MAX_NUM_SEGMENTS> segment_vd_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentVD_getData());
const std::span<float, MAX_NUM_SEGMENTS> segment_va_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentVA_getData());
const std::span<float, MAX_NUM_SEGMENTS> segment_vres_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentVRES_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_va_ov_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentVAStatus_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_va_uv_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentVAStatus_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_vd_ov_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentVDStatus_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_vd_uv_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentVDStatus_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_ced_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentDiagFaults_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_cmed_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentDiagFaults_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_sed_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentDiagFaults_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_smed_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentDiagFaults_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_vde_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentDiagFaults_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_vdel_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentDiagFaults_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_thsd_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentMiscFaults_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_tmodchk_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentMiscFaults_getData());
// const std::span<bool, MAX_NUM_SEGMENTS> segment_oscchk_buffer =
//     SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentMiscFaults_getData());
} // namespace

namespace app::segments::broadcast
{
// void cellVoltages(const Cells<float> &voltages, const CellSuccess &voltages_success)
void cellVoltages(const Cells<std::expected<float, ErrorCode>> &voltages)
{
    CellParam candidate_max_cell_voltage = { .segment = 0, .cell = 0, .value = __FLT_MIN__ };
    CellParam candidate_min_cell_voltage = { .segment = 0, .cell = 0, .value = __FLT_MAX__ };

    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        bool seg_ok = true;
        for (size_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!voltages[seg][cell])
            {
                seg_ok                          = false;
                cell_voltage_setters[seg][cell] = 0.0f; // Set to 0 or some sentinel value to indicate failure
                continue;
            }
            const float voltage             = voltages[seg][cell].value();
            cell_voltage_setters[seg][cell] = voltage;
            const CellParam current_cell_voltage{
                .segment = static_cast<uint8_t>(seg),
                .cell    = static_cast<uint8_t>(cell),
                .value   = voltage,
            };
            candidate_max_cell_voltage = std::max(candidate_max_cell_voltage, current_cell_voltage);
            candidate_min_cell_voltage = std::min(candidate_min_cell_voltage, current_cell_voltage);
        }
        seg_ok_buffer[seg] = seg_ok;
    }
    // TODO work this one out
    // max_cell_voltage = candidate_max_cell_voltage;
    // min_cell_voltage = candidate_min_cell_voltage;
}

void filteredCellVoltages(const Cells<std::expected<float, ErrorCode>> &filtered_voltages)
{
    // filtered_cell_voltages        = voltages;
    // filtered_cell_voltage_success = voltages_success;
    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        bool seg_ok = true;

        for (size_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!filtered_voltages[seg][cell])
            {
                seg_ok                           = false;
                cell_fvoltage_setters[seg][cell] = 0.0f; // Set to 0 or some sentinel value to indicate failure
                continue;
            }
            cell_fvoltage_setters[seg][cell] = filtered_voltages[seg][cell].value();
        }
        seg_ok_buffer[seg] = seg_ok;
    }
}

void temps(const Therms<float> &temps, const ThermSuccess &temps_success)
{
    CellParam candidate_max_cell_temp = { .segment = 0, .cell = 0, .value = __FLT_MIN__ };
    CellParam candidate_min_cell_temp = { .segment = 0, .cell = 0, .value = __FLT_MAX__ };

    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        bool seg_ok = true;
        for (size_t therm = 0U; therm < THERMISTORS_PER_SEGMENT; therm++)
        {
            const float temperature              = temps[seg][therm];
            cell_temperature_setters[seg][therm] = temperature;

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
            const CellParam current_cell_temp{
                .segment = static_cast<uint8_t>(seg),
                .cell    = static_cast<uint8_t>(therm),
                .value   = temperature,
            };
            candidate_max_cell_temp = std::max(candidate_max_cell_temp, current_cell_temp);
            candidate_min_cell_temp = std::min(candidate_min_cell_temp, current_cell_temp);
        }
        seg_ok_buffer[seg] = seg_ok;
    }
}

void segVoltages(const Segments<float> &seg_voltages, const SegmentSuccess &seg_voltages_success)
{
    float   max_voltage  = std::numeric_limits<float>::lowest();
    float   min_voltage  = std::numeric_limits<float>::max();
    uint8_t max_seg      = 0;
    uint8_t min_seg      = 0;
    float   pack_voltage = 0.0f;

    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        const bool seg_ok       = static_cast<bool>(seg_voltages_success[seg]);
        seg_ok_buffer[seg]      = seg_ok;
        seg_voltage_buffer[seg] = seg_voltages[seg];

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

void status(const Status &status, const SegmentSuccess &status_success)
{
    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        if (!status_success[seg])
        {
            // STATA
            seg_vref2_buffer[seg]    = -0.1f;
            segment_itmp_buffer[seg] = -0.1f;
            // STATB
            segment_vd_buffer[seg]   = -0.1f;
            segment_va_buffer[seg]   = -0.1f;
            segment_vres_buffer[seg] = -0.1f;
            // STATC
            segment_va_ov_buffer[seg]   = false;
            segment_va_uv_buffer[seg]   = false;
            segment_vd_ov_buffer[seg]   = false;
            segment_vd_uv_buffer[seg]   = false;
            segment_ced_buffer[seg]     = false;
            segment_cmed_buffer[seg]    = false;
            segment_sed_buffer[seg]     = false;
            segment_smed_buffer[seg]    = false;
            segment_vde_buffer[seg]     = false;
            segment_vdel_buffer[seg]    = false;
            segment_thsd_buffer[seg]    = false;
            segment_tmodchk_buffer[seg] = false;
            segment_oscchk_buffer[seg]  = false;
            // STATD
            for (size_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
            {
                cell_ov_buffer[seg][cell] = false;
                cell_uv_buffer[seg][cell] = false;
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
        for (size_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            cell_ov_setters[seg][cell](static_cast<bool>((stat_d.covuv >> (16U + cell)) & 1U));
            cell_uv_setters[seg][cell](static_cast<bool>((stat_d.covuv >> cell) & 1U));
        }
    }
}

void owc(const Owc &owc_ok, const CellSuccess &owc_ok_success)
{
    cell_owc_ok = owc_ok;

    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        bool seg_ok = true;
        for (size_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
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

void info()
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
} // namespace app::segments::broadcast