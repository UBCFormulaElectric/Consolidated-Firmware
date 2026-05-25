#include <cmath>

#include "app_segments_internal.hpp"
#include "app_segments.hpp"
#include "app_canTx.hpp"
#include "io_canTx.hpp"
#include "io_semaphore.hpp"

namespace
{
using app::segments::BroadcastBuffer;
using app::segments::CellBroadcaster;
namespace tx = io::can_tx;

constexpr size_t NUM_HEALTH_BITS = static_cast<size_t>(app::segments::health::ErrorBit::NUM_ERROR_BITS);
BroadcastBuffer<bool, MAX_NUM_SEGMENTS * NUM_HEALTH_BITS, tx::BMS_SegmentHealthErrors_sendAperiodic>
    segment_health_errors_buffer(app::can_tx::BMS_SegmentHealthErrors_getData());
// BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentCMDCNT_sendAperiodic> segment_cmdcnt_buffer(
//     app::can_tx::BMS_SegmentCMDCNT_getData());

// Debug Messages
CellBroadcaster<
    float,
    tx::BMS_CellVoltages_Seg0_Seg3_sendAperiodic,
    tx::BMS_CellVoltages_Seg4_Seg7_sendAperiodic,
    tx::BMS_CellVoltages_Seg8_Seg9_sendAperiodic>
    cell_voltage_setters(
        app::can_tx::BMS_CellVoltages_Seg0_Seg3_getData(),
        app::can_tx::BMS_CellVoltages_Seg4_Seg7_getData(),
        app::can_tx::BMS_CellVoltages_Seg8_Seg9_getData());

CellBroadcaster<
    float,
    tx::BMS_CellTemps_Seg0_Seg3_sendAperiodic,
    tx::BMS_CellTemps_Seg4_Seg7_sendAperiodic,
    tx::BMS_CellTemps_Seg8_Seg9_sendAperiodic>
    cell_temperature_setters(
        app::can_tx::BMS_CellTemps_Seg0_Seg3_getData(),
        app::can_tx::BMS_CellTemps_Seg4_Seg7_getData(),
        app::can_tx::BMS_CellTemps_Seg8_Seg9_getData());

BroadcastBuffer<float, MAX_NUM_SEGMENTS, tx::BMS_SegmentVoltages_sendAperiodic>
    segment_voltage_buffer(app::can_tx::BMS_SegmentVoltages_getData());
BroadcastBuffer<float, MAX_NUM_SEGMENTS, tx::BMS_SegmentVref2_sendAperiodic>
    segment_vref2_buffer(app::can_tx::BMS_SegmentVref2_getData());
BroadcastBuffer<float, MAX_NUM_SEGMENTS, tx::BMS_SegmentITMP_sendAperiodic>
    segment_itmp_buffer(app::can_tx::BMS_SegmentITMP_getData());
BroadcastBuffer<float, MAX_NUM_SEGMENTS, tx::BMS_SegmentVD_sendAperiodic>
    segment_vd_buffer(app::can_tx::BMS_SegmentVD_getData());
BroadcastBuffer<float, MAX_NUM_SEGMENTS, tx::BMS_SegmentVA_sendAperiodic>
    segment_va_buffer(app::can_tx::BMS_SegmentVA_getData());
BroadcastBuffer<float, MAX_NUM_SEGMENTS, tx::BMS_SegmentVRES_sendAperiodic>
    segment_vres_buffer(app::can_tx::BMS_SegmentVRES_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentVA_OV_sendAperiodic>
    segment_va_ov_buffer(app::can_tx::BMS_SegmentVA_OV_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentVA_UV_sendAperiodic>
    segment_va_uv_buffer(app::can_tx::BMS_SegmentVA_UV_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentVD_OV_sendAperiodic>
    segment_vd_ov_buffer(app::can_tx::BMS_SegmentVD_OV_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentVD_UV_sendAperiodic>
    segment_vd_uv_buffer(app::can_tx::BMS_SegmentVD_UV_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentCED_sendAperiodic>
    segment_ced_buffer(app::can_tx::BMS_SegmentCED_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentCMED_sendAperiodic>
    segment_cmed_buffer(app::can_tx::BMS_SegmentCMED_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentSED_sendAperiodic>
    segment_sed_buffer(app::can_tx::BMS_SegmentSED_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentSMED_sendAperiodic>
    segment_smed_buffer(app::can_tx::BMS_SegmentSMED_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentVDE_sendAperiodic>
    segment_vde_buffer(app::can_tx::BMS_SegmentVDE_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentVDEL_sendAperiodic>
    segment_vdel_buffer(app::can_tx::BMS_SegmentVDEL_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentTHSD_sendAperiodic>
    segment_thsd_buffer(app::can_tx::BMS_SegmentTHSD_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentTMODCHK_sendAperiodic>
    segment_tmodchk_buffer(app::can_tx::BMS_SegmentTMODCHK_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS, tx::BMS_SegmentOSCCHK_sendAperiodic>
    segment_oscchk_buffer(app::can_tx::BMS_SegmentOSCCHK_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT, tx::BMS_CellOverVoltage_sendAperiodic>
    cell_ov_buffer(app::can_tx::BMS_CellOverVoltage_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT, tx::BMS_CellUnderVoltage_sendAperiodic>
    cell_uv_buffer(app::can_tx::BMS_CellUnderVoltage_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT, tx::BMS_CellOpenWireCheck_sendAperiodic>
    cell_owc_ok_buffer(app::can_tx::BMS_CellOpenWireCheck_getData());
BroadcastBuffer<bool, MAX_NUM_SEGMENTS * CELLS_PER_SEGMENT, tx::BMS_ThermistorOpenWireCheck_sendAperiodic>
    therm_owc_ok_buffer(app::can_tx::BMS_ThermistorOpenWireCheck_getData());

// Error-code companion buffers. INVALID_ARGS (= 0) means no error.
using CanErr                  = app::can_utils::ErrorCode;
constexpr CanErr CAN_ERR_NONE = CanErr::ERROR_CODE_INVALID_ARGS;
constexpr CanErr toCanErr(const ErrorCode e)
{
    return static_cast<CanErr>(static_cast<uint8_t>(e));
}

CellBroadcaster<
    CanErr,
    tx::BMS_CellVoltageErrors_Seg0_Seg3_sendAperiodic,
    tx::BMS_CellVoltageErrors_Seg4_Seg7_sendAperiodic,
    tx::BMS_CellVoltageErrors_Seg8_Seg9_sendAperiodic>
    cell_voltage_error_setters(
        app::can_tx::BMS_CellVoltageErrors_Seg0_Seg3_getData(),
        app::can_tx::BMS_CellVoltageErrors_Seg4_Seg7_getData(),
        app::can_tx::BMS_CellVoltageErrors_Seg8_Seg9_getData());

CellBroadcaster<
    CanErr,
    tx::BMS_CellTempErrors_Seg0_Seg3_sendAperiodic,
    tx::BMS_CellTempErrors_Seg4_Seg7_sendAperiodic,
    tx::BMS_CellTempErrors_Seg8_Seg9_sendAperiodic>
    cell_temperature_error_setters(
        app::can_tx::BMS_CellTempErrors_Seg0_Seg3_getData(),
        app::can_tx::BMS_CellTempErrors_Seg4_Seg7_getData(),
        app::can_tx::BMS_CellTempErrors_Seg8_Seg9_getData());

CellBroadcaster<
    CanErr,
    tx::BMS_CellOpenWireCheckErrors_Seg0_Seg3_sendAperiodic,
    tx::BMS_CellOpenWireCheckErrors_Seg4_Seg7_sendAperiodic,
    tx::BMS_CellOpenWireCheckErrors_Seg8_Seg9_sendAperiodic>
    cell_owc_error_setters(
        app::can_tx::BMS_CellOpenWireCheckErrors_Seg0_Seg3_getData(),
        app::can_tx::BMS_CellOpenWireCheckErrors_Seg4_Seg7_getData(),
        app::can_tx::BMS_CellOpenWireCheckErrors_Seg8_Seg9_getData());

CellBroadcaster<
    CanErr,
    tx::BMS_ThermistorOpenWireCheckErrors_Seg0_Seg3_sendAperiodic,
    tx::BMS_ThermistorOpenWireCheckErrors_Seg4_Seg7_sendAperiodic,
    tx::BMS_ThermistorOpenWireCheckErrors_Seg8_Seg9_sendAperiodic>
    therm_owc_error_setters(
        app::can_tx::BMS_ThermistorOpenWireCheckErrors_Seg0_Seg3_getData(),
        app::can_tx::BMS_ThermistorOpenWireCheckErrors_Seg4_Seg7_getData(),
        app::can_tx::BMS_ThermistorOpenWireCheckErrors_Seg8_Seg9_getData());

BroadcastBuffer<CanErr, MAX_NUM_SEGMENTS, tx::BMS_SegmentVoltageErrors_sendAperiodic>
    segment_voltage_error_buffer(app::can_tx::BMS_SegmentVoltageErrors_getData());

BroadcastBuffer<CanErr, MAX_NUM_SEGMENTS, tx::BMS_SegmentStatAErrors_sendAperiodic>
    segment_stat_a_error_buffer(app::can_tx::BMS_SegmentStatAErrors_getData());
BroadcastBuffer<CanErr, MAX_NUM_SEGMENTS, tx::BMS_SegmentStatBErrors_sendAperiodic>
    segment_stat_b_error_buffer(app::can_tx::BMS_SegmentStatBErrors_getData());
BroadcastBuffer<CanErr, MAX_NUM_SEGMENTS, tx::BMS_SegmentStatCErrors_sendAperiodic>
    segment_stat_c_error_buffer(app::can_tx::BMS_SegmentStatCErrors_getData());
BroadcastBuffer<CanErr, MAX_NUM_SEGMENTS, tx::BMS_SegmentStatDErrors_sendAperiodic>
    segment_stat_d_error_buffer(app::can_tx::BMS_SegmentStatDErrors_getData());

} // namespace

namespace app::segments::broadcast
{
namespace debug
{
    void cellVoltages(const Cells<result<float>> &voltages, const result<void> &poll_ok)
    {
        if (!poll_ok)
        {
            cell_voltage_setters.fill(-0.1f);
            cell_voltage_error_setters.fill(toCanErr(poll_ok.error()));
            cell_voltage_setters.send();
            cell_voltage_error_setters.send();
            return;
        }
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            for (size_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
            {
                const auto &r                         = voltages[seg][cell];
                cell_voltage_setters[seg][cell]       = r.value_or(-0.1f);
                cell_voltage_error_setters[seg][cell] = r ? CAN_ERR_NONE : toCanErr(r.error());
            }
        }
        cell_voltage_setters.send();
        cell_voltage_error_setters.send();
    }

    void cellOwc(const Cells<result<bool>> &owc_results, const result<void> &poll_ok)
    {
        if (!poll_ok)
        {
            cell_owc_ok_buffer.fill(true);
            cell_owc_error_setters.fill(toCanErr(poll_ok.error()));
            cell_owc_ok_buffer.send();
            cell_owc_error_setters.send();
            return;
        }
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            for (size_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
            {
                const auto &r                                      = owc_results[seg][cell];
                cell_owc_ok_buffer[seg * CELLS_PER_SEGMENT + cell] = r.value_or(true);
                cell_owc_error_setters[seg][cell]                  = r ? CAN_ERR_NONE : toCanErr(r.error());
            }
        }
        cell_owc_ok_buffer.send();
        cell_owc_error_setters.send();
    }

    void thermTemps(const Therms<result<float>> &temps, const result<void> &poll_ok)
    {
        if (!poll_ok)
        {
            cell_temperature_setters.fill(-0.1f);
            cell_temperature_error_setters.fill(toCanErr(poll_ok.error()));
            cell_temperature_setters.send();
            cell_temperature_error_setters.send();
            return;
        }
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            for (size_t therm = 0U; therm < THERMISTORS_PER_SEGMENT; therm++)
            {
                const auto &r                              = temps[seg][therm];
                cell_temperature_setters[seg][therm]       = r.value_or(-0.1f);
                cell_temperature_error_setters[seg][therm] = r ? CAN_ERR_NONE : toCanErr(r.error());
            }
        }
        cell_temperature_setters.send();
        cell_temperature_error_setters.send();
    }

    void thermOwc(const Therms<result<bool>> &therm_owc, const result<void> &poll_ok)
    {
        if (!poll_ok)
        {
            therm_owc_ok_buffer.fill(true);
            therm_owc_error_setters.fill(toCanErr(poll_ok.error()));
            therm_owc_ok_buffer.send();
            therm_owc_error_setters.send();
            return;
        }
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            for (size_t therm = 0U; therm < THERMISTORS_PER_SEGMENT; therm++)
            {
                const auto &r                                        = therm_owc[seg][therm];
                therm_owc_ok_buffer[seg * CELLS_PER_SEGMENT + therm] = r.value_or(true);
                therm_owc_error_setters[seg][therm]                  = r ? CAN_ERR_NONE : toCanErr(r.error());
            }
        }
        therm_owc_ok_buffer.send();
        therm_owc_error_setters.send();
    }

    void segVoltages(const Segments<result<float>> &seg_voltages)
    {
        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            const auto &r                     = seg_voltages[seg];
            segment_voltage_buffer[seg]       = r.value_or(-0.1f);
            segment_voltage_error_buffer[seg] = r ? CAN_ERR_NONE : toCanErr(r.error());
        }
        segment_voltage_buffer.send();
        segment_voltage_error_buffer.send();
    }

    void status(const Segments<io::adbms::StatusGroupsRes> &status)
    {
        using io::adbms::STATC;
        using io::adbms::STATD;

        for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
        {
            const auto &stat_a = status[seg].stat_a;
            const auto &stat_b = status[seg].stat_b;
            const auto &stat_c = status[seg].stat_c;
            const auto &stat_d = status[seg].stat_d;
            const STATC c      = stat_c.value_or(STATC{});
            const STATD d      = stat_d.value_or(STATD{});

            segment_stat_a_error_buffer[seg] = stat_a ? CAN_ERR_NONE : toCanErr(stat_a.error());
            segment_stat_b_error_buffer[seg] = stat_b ? CAN_ERR_NONE : toCanErr(stat_b.error());
            segment_stat_c_error_buffer[seg] = stat_c ? CAN_ERR_NONE : toCanErr(stat_c.error());
            segment_stat_d_error_buffer[seg] = stat_d ? CAN_ERR_NONE : toCanErr(stat_d.error());

            // STATA
            segment_vref2_buffer[seg] = stat_a ? convertRegToVoltage(stat_a->vref2) : -0.1f;
            segment_itmp_buffer[seg]  = stat_a ? convertRegToVoltage(stat_a->itmp) : -0.1f;

            // STATB
            segment_vd_buffer[seg]   = stat_b ? convertRegToVoltage(stat_b->vd) : -0.1f;
            segment_va_buffer[seg]   = stat_b ? convertRegToVoltage(stat_b->va) : -0.1f;
            segment_vres_buffer[seg] = stat_b ? convertRegToVoltage(stat_b->vres) : -0.1f;

            // STATC (zero-init on missing = all faults reported as false)
            segment_va_ov_buffer[seg]   = c.va_ov;
            segment_va_uv_buffer[seg]   = c.va_uv;
            segment_vd_ov_buffer[seg]   = c.vd_ov;
            segment_vd_uv_buffer[seg]   = c.vd_uv;
            segment_ced_buffer[seg]     = c.ced;
            segment_cmed_buffer[seg]    = c.cmed;
            segment_sed_buffer[seg]     = c.sed;
            segment_smed_buffer[seg]    = c.smed;
            segment_vde_buffer[seg]     = c.vde;
            segment_vdel_buffer[seg]    = c.vdel;
            segment_thsd_buffer[seg]    = c.thsd;
            segment_tmodchk_buffer[seg] = c.tmodchk;
            segment_oscchk_buffer[seg]  = c.oscchk;

            // STATD — cell N UV at bit 2N, OV at bit 2N+1
            const auto covuv_bit = [&d](const size_t n) { return static_cast<bool>((d.covuv >> n) & 1U); };
            for (size_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
            {
                cell_uv_buffer[seg * CELLS_PER_SEGMENT + cell] = covuv_bit(2U * cell);
                cell_ov_buffer[seg * CELLS_PER_SEGMENT + cell] = covuv_bit(2U * cell + 1U);
            }
        }

        segment_vref2_buffer.send();
        segment_itmp_buffer.send();
        segment_vd_buffer.send();
        segment_va_buffer.send();
        segment_vres_buffer.send();
        segment_va_ov_buffer.send();
        segment_va_uv_buffer.send();
        segment_vd_ov_buffer.send();
        segment_vd_uv_buffer.send();
        segment_ced_buffer.send();
        segment_cmed_buffer.send();
        segment_sed_buffer.send();
        segment_smed_buffer.send();
        segment_vde_buffer.send();
        segment_vdel_buffer.send();
        segment_thsd_buffer.send();
        segment_tmodchk_buffer.send();
        segment_oscchk_buffer.send();
        cell_ov_buffer.send();
        cell_uv_buffer.send();
        segment_stat_a_error_buffer.send();
        segment_stat_b_error_buffer.send();
        segment_stat_c_error_buffer.send();
        segment_stat_d_error_buffer.send();
    }
} // namespace debug

// void cmdCountMismatch()
// {
//     const auto mismatches = io::adbms::getCmdCountMismatches();
//     for (size_t seg = 0U; seg < NUM_SEGMENTS; seg++)
//     {
//         segment_cmdcnt_buffer[seg] = mismatches[seg];
//     }
//     segment_cmdcnt_buffer.send();
// }

void segmentHealthError()
{
    using app::segments::health::ErrorBit;
    for (size_t seg = 0U; seg < MAX_NUM_SEGMENTS; seg++)
    {
        for (size_t bit = 0U; bit < NUM_HEALTH_BITS; bit++)
        {
            const size_t idx = seg * NUM_HEALTH_BITS + bit;
            segment_health_errors_buffer[idx] =
                (seg < NUM_SEGMENTS) && app::segments::health::getError(seg, static_cast<ErrorBit>(bit));
        }
    }
    segment_health_errors_buffer.send();
}

void voltageStats()
{
    const auto min = app::segments::shared::getMinCellVoltage();
    const auto max = app::segments::shared::getMaxCellVoltage();
    app::can_tx::BMS_MinCellVoltage_set(min.value);
    app::can_tx::BMS_MinCellVoltageSegment_set(min.segment);
    app::can_tx::BMS_MinCellVoltageCell_set(min.cell);
    app::can_tx::BMS_MaxCellVoltage_set(max.value);
    app::can_tx::BMS_MaxCellVoltageSegment_set(max.segment);
    app::can_tx::BMS_MaxCellVoltageCell_set(max.cell);
}

void temperatureStats()
{
    const auto min = app::segments::shared::getMinCellTemperature();
    const auto max = app::segments::shared::getMaxCellTemperature();
    app::can_tx::BMS_MinCellTemp_set(min.value);
    app::can_tx::BMS_MinCellTempSegment_set(min.segment);
    app::can_tx::BMS_MinCellTempCell_set(min.cell);
    app::can_tx::BMS_MaxCellTemp_set(max.value);
    app::can_tx::BMS_MaxCellTempSegment_set(max.segment);
    app::can_tx::BMS_MaxCellTempCell_set(max.cell);
}

void segmentVoltageStats()
{
    const auto min = app::segments::shared::getMinSegmentVoltage();
    const auto max = app::segments::shared::getMaxSegmentVoltage();
    app::can_tx::BMS_MinSegmentVoltage_set(min.value);
    app::can_tx::BMS_MinSegmentVoltageSegment_set(min.segment);
    app::can_tx::BMS_MaxSegmentVoltage_set(max.value);
    app::can_tx::BMS_MaxSegmentVoltageSegment_set(max.segment);
}

} // namespace app::segments::broadcast
