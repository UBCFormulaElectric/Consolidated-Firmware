#include <cmath>

#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "app_canTx.hpp"
#include "io_canTx.hpp"
#include "io_semaphore.hpp"

namespace
{

template <typename T> class CellBroadcaster
{
    std::span<T, 4 * CELLS_PER_SEGMENT> _segments0_3;
    std::span<T, 4 * CELLS_PER_SEGMENT> _segments4_7;
    std::span<T, 2 * CELLS_PER_SEGMENT> _segments8_9;

  public:
    template <typename seg03, typename seg47, typename seg89>
    CellBroadcaster(seg03 &segments0_3_can, seg47 &segments4_7_can, seg89 &segments8_9_can)
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

template <typename S, typename CanMsg>
constexpr std::span<S, MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT> CellBroadcastBuffer(CanMsg &can_msg)
{
    static_assert(sizeof(can_msg) == MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT * sizeof(S));
    return std::span<S, MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT>{ reinterpret_cast<S *>(&can_msg),
                                                               MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT };
}

CellBroadcaster<float> cell_voltage_setters(
    app::can_tx::BMS_CellVoltages_Seg0_Seg3_getData(),
    app::can_tx::BMS_CellVoltages_Seg4_Seg7_getData(),
    app::can_tx::BMS_CellVoltages_Seg8_Seg9_getData());
CellBroadcaster<float> cell_temperature_setters(
    app::can_tx::BMS_CellTemps_Seg0_Seg3_getData(),
    app::can_tx::BMS_CellTemps_Seg4_Seg7_getData(),
    app::can_tx::BMS_CellTemps_Seg8_Seg9_getData());

const std::span<bool, MAX_NUM_SEGMENTS> comm_ok_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentCommOk_getData());
const std::span<float, MAX_NUM_SEGMENTS> segment_voltage_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentVoltages_getData());
const std::span<float, MAX_NUM_SEGMENTS> segment_vref2_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentVref2_getData());
const std::span<float, MAX_NUM_SEGMENTS> segment_itmp_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentITMP_getData());
const std::span<float, MAX_NUM_SEGMENTS> segment_vd_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentVD_getData());
const std::span<float, MAX_NUM_SEGMENTS> segment_va_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentVA_getData());
const std::span<float, MAX_NUM_SEGMENTS> segment_vres_buffer =
    SegmentBroadcastBuffer<float>(app::can_tx::BMS_SegmentVRES_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_va_ov_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentVA_OV_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_va_uv_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentVA_UV_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_vd_ov_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentVD_OV_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_vd_uv_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentVD_UV_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_ced_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentCED_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_cmed_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentCMED_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_sed_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentSED_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_smed_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentSMED_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_vde_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentVDE_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_vdel_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentVDEL_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_thsd_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentTHSD_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_tmodchk_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentTMODCHK_getData());
const std::span<bool, MAX_NUM_SEGMENTS> segment_oscchk_buffer =
    SegmentBroadcastBuffer<bool>(app::can_tx::BMS_SegmentOSCCHK_getData());
const std::span<bool, MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT> cell_ov_buffer =
    CellBroadcastBuffer<bool>(app::can_tx::BMS_CellOverVoltage_getData());
const std::span<bool, MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT> cell_uv_buffer =
    CellBroadcastBuffer<bool>(app::can_tx::BMS_CellUnderVoltage_getData());
const std::span<bool, MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT> cell_owc_ok_buffer =
    CellBroadcastBuffer<bool>(app::can_tx::BMS_CellOpenWireCheck_getData());
const std::span<bool, MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT> therm_owc_ok_buffer =
    CellBroadcastBuffer<bool>(app::can_tx::BMS_ThermistorOpenWireCheck_getData());

} // namespace

namespace app::segments::broadcast
{
void cellVoltages(const Cells<result<float>> &voltages)
{
    CellParam candidate_max_cell_voltage = { .segment = 0, .cell = 0, .value = __FLT_MIN__ };
    CellParam candidate_min_cell_voltage = { .segment = 0, .cell = 0, .value = __FLT_MAX__ };

    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (!voltages[seg][cell])
            {
                cell_voltage_setters[seg][cell] = -0.1f;
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
        comm_ok_buffer[seg] = health::isOk(seg);
    }

    // health::setVoltageStats(voltages, candidate_min_cell_voltage, candidate_max_cell_voltage);

    using namespace io::can_tx;
    BMS_CellVoltages_Seg0_Seg3_sendAperiodic();
    BMS_CellVoltages_Seg4_Seg7_sendAperiodic();
    BMS_CellVoltages_Seg8_Seg9_sendAperiodic();
    BMS_SegmentCommOk_sendAperiodic();
}

void thermTemps(const Therms<result<float>> &temps)
{
    CellParam candidate_max_cell_temp = { .segment = 0, .cell = 0, .value = __FLT_MIN__ };
    CellParam candidate_min_cell_temp = { .segment = 0, .cell = 0, .value = __FLT_MAX__ };

    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t therm = 0U; therm < THERMISTORS_PER_SEGMENT; therm++)
        {
            const auto &r                        = temps[seg][therm];
            cell_temperature_setters[seg][therm] = r.value_or(-0.1f);
            if (r)
            {
                const CellParam current_cell_temp{
                    .segment = static_cast<uint8_t>(seg),
                    .cell    = static_cast<uint8_t>(therm),
                    .value   = r.value(),
                };
                candidate_max_cell_temp = std::max(candidate_max_cell_temp, current_cell_temp);
                candidate_min_cell_temp = std::min(candidate_min_cell_temp, current_cell_temp);
            }
        }
        comm_ok_buffer[seg] = health::isOk(seg);
    }

    // health::setMaxCellTemperature(candidate_max_cell_temp);

    using namespace io::can_tx;
    BMS_CellTemps_Seg0_Seg3_sendAperiodic();
    BMS_CellTemps_Seg4_Seg7_sendAperiodic();
    BMS_CellTemps_Seg8_Seg9_sendAperiodic();
    BMS_SegmentCommOk_sendAperiodic();
}

void thermOwc(const Therms<result<bool>> &therm_owc)
{
    // bool candidate_therm_owc = false;
    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t therm = 0U; therm < THERMISTORS_PER_SEGMENT; therm++)
        {
            const bool ok                                        = therm_owc[seg][therm].value_or(true);
            therm_owc_ok_buffer[seg * CELLS_PER_SEGMENT + therm] = ok;
            // if (!ok)
            // candidate_therm_owc = true;
        }
    }
    // health::setThermOwc(candidate_therm_owc);

    io::can_tx::BMS_ThermistorOpenWireCheck_sendAperiodic();
}

void segVoltages(const Segments<result<float>> &seg_voltages)
{
    float   max_voltage  = std::numeric_limits<float>::lowest();
    float   min_voltage  = std::numeric_limits<float>::max();
    uint8_t max_seg      = 0;
    uint8_t min_seg      = 0;
    float   pack_voltage = 0.0f;

    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        const auto &r               = seg_voltages[seg];
        comm_ok_buffer[seg]         = r.has_value();
        segment_voltage_buffer[seg] = r.value_or(-0.1f);

        if (!r)
            continue;

        pack_voltage += r.value();

        if (r.value() > max_voltage)
        {
            max_voltage = r.value();
            max_seg     = static_cast<uint8_t>(seg);
        }
        if (r.value() < min_voltage)
        {
            min_voltage = r.value();
            min_seg     = static_cast<uint8_t>(seg);
        }
    }

    app::can_tx::BMS_PackVoltage_set(pack_voltage);
    app::can_tx::BMS_MaxSegmentVoltage_Segment_set(max_seg);
    app::can_tx::BMS_MaxSegmentVoltage_Voltage_set(max_voltage);
    app::can_tx::BMS_MinSegmentVoltage_Segment_set(min_seg);
    app::can_tx::BMS_MinSegmentVoltage_Voltage_set(min_voltage);

    using namespace io::can_tx;
    BMS_SegmentVoltages_sendAperiodic();
    BMS_SegmentCommOk_sendAperiodic();
}

void status(const Segments<io::adbms::StatusGroupsRes> &status)
{
    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        const auto &stat_a = status[seg].stat_a;
        const auto &stat_b = status[seg].stat_b;
        const auto &stat_c = status[seg].stat_c;
        const auto &stat_d = status[seg].stat_d;

        // STATA
        segment_vref2_buffer[seg] = stat_a ? convertRegToVoltage(stat_a->vref2) : -0.1f;
        segment_itmp_buffer[seg]  = stat_a ? convertRegToVoltage(stat_a->itmp) : -0.1f;

        // STATB
        segment_vd_buffer[seg]   = stat_b ? convertRegToVoltage(stat_b->vd) : -0.1f;
        segment_va_buffer[seg]   = stat_b ? convertRegToVoltage(stat_b->va) : -0.1f;
        segment_vres_buffer[seg] = stat_b ? convertRegToVoltage(stat_b->vres) : -0.1f;

        // STATC
        segment_va_ov_buffer[seg]   = stat_c && stat_c->va_ov;
        segment_va_uv_buffer[seg]   = stat_c && stat_c->va_uv;
        segment_vd_ov_buffer[seg]   = stat_c && stat_c->vd_ov;
        segment_vd_uv_buffer[seg]   = stat_c && stat_c->vd_uv;
        segment_ced_buffer[seg]     = stat_c && stat_c->ced;
        segment_cmed_buffer[seg]    = stat_c && stat_c->cmed;
        segment_sed_buffer[seg]     = stat_c && stat_c->sed;
        segment_smed_buffer[seg]    = stat_c && stat_c->smed;
        segment_vde_buffer[seg]     = stat_c && stat_c->vde;
        segment_vdel_buffer[seg]    = stat_c && stat_c->vdel;
        segment_thsd_buffer[seg]    = stat_c && stat_c->thsd;
        segment_tmodchk_buffer[seg] = stat_c && stat_c->tmodchk;
        segment_oscchk_buffer[seg]  = stat_c && stat_c->oscchk;

        // STATD — cell N UV at bit 2*(N-1), OV at bit 2*(N-1)+1
        for (size_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            cell_uv_buffer[seg * CELLS_PER_SEGMENT + cell] =
                stat_d && static_cast<bool>((stat_d->covuv >> (2U * cell)) & 1U);
            cell_ov_buffer[seg * CELLS_PER_SEGMENT + cell] =
                stat_d && static_cast<bool>((stat_d->covuv >> (2U * cell + 1U)) & 1U);
        }
    }

    using namespace io::can_tx;
    // STATA / STATB analog readings
    BMS_SegmentVref2_sendAperiodic();
    BMS_SegmentITMP_sendAperiodic();
    BMS_SegmentVD_sendAperiodic();
    BMS_SegmentVA_sendAperiodic();
    BMS_SegmentVRES_sendAperiodic();
    // STATC fault bits
    BMS_SegmentVA_OV_sendAperiodic();
    BMS_SegmentVA_UV_sendAperiodic();
    BMS_SegmentVD_OV_sendAperiodic();
    BMS_SegmentVD_UV_sendAperiodic();
    BMS_SegmentCED_sendAperiodic();
    BMS_SegmentCMED_sendAperiodic();
    BMS_SegmentSED_sendAperiodic();
    BMS_SegmentSMED_sendAperiodic();
    BMS_SegmentVDE_sendAperiodic();
    BMS_SegmentVDEL_sendAperiodic();
    BMS_SegmentTHSD_sendAperiodic();
    BMS_SegmentTMODCHK_sendAperiodic();
    BMS_SegmentOSCCHK_sendAperiodic();
    // STATD per-cell OV/UV
    BMS_CellOverVoltage_sendAperiodic();
    BMS_CellUnderVoltage_sendAperiodic();
}

void cellOwc(const Cells<result<bool>> &owc_results)
{
    // bool candidate_cell_owc = false;

    for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
    {
        for (size_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            const bool ok                                      = owc_results[seg][cell].value_or(true);
            cell_owc_ok_buffer[seg * CELLS_PER_SEGMENT + cell] = ok;
            // if (!ok)
            //     candidate_cell_owc = true;
        }
    }

    // health::setCellOwc(candidate_cell_owc);

    io::can_tx::BMS_CellOpenWireCheck_sendAperiodic();
}

void cellVoltagesPollErr()
{
    // TODO: raise/transmit cell-voltage poll-failure signal.
}

void cellOwcPollErr()
{
    // TODO: raise/transmit cell open-wire-check poll-failure signal.
}

void thermTempsPollErr()
{
    // TODO: raise/transmit thermistor-temperature poll-failure signal.
}

void thermOwcPollErr()
{
    // TODO: raise/transmit thermistor open-wire-check poll-failure signal.
}
} // namespace app::segments::broadcast