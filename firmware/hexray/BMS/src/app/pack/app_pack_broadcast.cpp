#include "app_pack.hpp"
#include "app_pack_internal.hpp"

#include <algorithm>
#include <array>
#include <span>

#include "app_canTx.hpp"

using io::adbms::CELLS_PER_SEGMENT;
using io::adbms::NUM_SEGMENTS;
using io::adbms::THERMISTORS_PER_SEGMENT;

namespace
{

constexpr float INVALID_VOLTAGE = -0.1f;
constexpr float INVALID_TEMP    = -40.0f;

template <typename T, size_t SEGS, size_t PER_SEG = CELLS_PER_SEGMENT> class SegmentBroadcaster
{
    // Only the msgs holding populated segments, segments past NUM_SEGMENTS are never written
    static constexpr size_t NUM_MSGS = (NUM_SEGMENTS + SEGS - 1) / SEGS;

    std::array<T *, NUM_MSGS> _msgs;

  public:
    template <typename... CanMsgs> explicit SegmentBroadcaster(CanMsgs &...can_msgs)
    {
        static_assert(SEGS * sizeof...(CanMsgs) >= NUM_SEGMENTS);
        static_assert(((sizeof(CanMsgs) == SEGS * PER_SEG * sizeof(T)) && ...));

        const std::array<T *, sizeof...(CanMsgs)> msgs{ { reinterpret_cast<T *>(&can_msgs)... } };
        std::copy_n(msgs.begin(), NUM_MSGS, _msgs.begin());
    }

    std::span<T, PER_SEG> operator[](const size_t seg) const
    {
        return std::span<T, PER_SEG>{ _msgs[seg / SEGS] + seg % SEGS * PER_SEG, PER_SEG };
    }
};

SegmentBroadcaster<float, 2> cell_voltage_setters(
    app::can_tx::BMS_CellVoltages_Seg0_Seg1_getData(),
    app::can_tx::BMS_CellVoltages_Seg2_Seg3_getData(),
    app::can_tx::BMS_CellVoltages_Seg4_Seg5_getData(),
    app::can_tx::BMS_CellVoltages_Seg6_Seg7_getData(),
    app::can_tx::BMS_CellVoltages_Seg8_Seg9_getData());
SegmentBroadcaster<app::can_utils::ErrorCode, 5> cell_voltage_error_setters(
    app::can_tx::BMS_CellVoltageErrors_Seg0_Seg4_getData(),
    app::can_tx::BMS_CellVoltageErrors_Seg5_Seg9_getData());

SegmentBroadcaster<float, 2, THERMISTORS_PER_SEGMENT> cell_temperature_setters(
    app::can_tx::BMS_CellTemps_Seg0_Seg1_getData(),
    app::can_tx::BMS_CellTemps_Seg2_Seg3_getData(),
    app::can_tx::BMS_CellTemps_Seg4_Seg5_getData(),
    app::can_tx::BMS_CellTemps_Seg6_Seg7_getData(),
    app::can_tx::BMS_CellTemps_Seg8_Seg9_getData());
SegmentBroadcaster<app::can_utils::ErrorCode, 5, THERMISTORS_PER_SEGMENT> cell_temperature_error_setters(
    app::can_tx::BMS_CellTempErrors_Seg0_Seg4_getData(),
    app::can_tx::BMS_CellTempErrors_Seg5_Seg9_getData());

SegmentBroadcaster<float, 10, 1> segment_vmv_setters(app::can_tx::BMS_SegmentVmv_getData());
SegmentBroadcaster<float, 10, 1> segment_vpv_setters(app::can_tx::BMS_SegmentVpv_getData());
SegmentBroadcaster<float, 10, 1> segment_vref_setters(app::can_tx::BMS_SegmentVref_getData());
SegmentBroadcaster<float, 10, 1> segment_itmp_setters(app::can_tx::BMS_SegmentItmp_getData());
SegmentBroadcaster<float, 10, 1> segment_va_setters(app::can_tx::BMS_SegmentVa_getData());
SegmentBroadcaster<float, 10, 1> segment_vd_setters(app::can_tx::BMS_SegmentVd_getData());
SegmentBroadcaster<float, 10, 1> segment_vres_setters(app::can_tx::BMS_SegmentVres_getData());
SegmentBroadcaster<app::can_utils::ErrorCode, 10, 1>
    segment_vmv_error_setters(app::can_tx::BMS_SegmentVmvErrors_getData());
SegmentBroadcaster<app::can_utils::ErrorCode, 10, 1>
    segment_vpv_error_setters(app::can_tx::BMS_SegmentVpvErrors_getData());
SegmentBroadcaster<app::can_utils::ErrorCode, 10, 1>
    segment_vref_error_setters(app::can_tx::BMS_SegmentVrefErrors_getData());
SegmentBroadcaster<app::can_utils::ErrorCode, 10, 1>
    segment_itmp_error_setters(app::can_tx::BMS_SegmentItmpErrors_getData());
SegmentBroadcaster<app::can_utils::ErrorCode, 10, 1>
    segment_va_error_setters(app::can_tx::BMS_SegmentVaErrors_getData());
SegmentBroadcaster<app::can_utils::ErrorCode, 10, 1>
    segment_vd_error_setters(app::can_tx::BMS_SegmentVdErrors_getData());
SegmentBroadcaster<app::can_utils::ErrorCode, 10, 1>
    segment_vres_error_setters(app::can_tx::BMS_SegmentVresErrors_getData());

SegmentBroadcaster<uint8_t, 5> cell_pwm_duty_setters(
    app::can_tx::BMS_CellPwmDuty_Seg0_Seg4_getData(),
    app::can_tx::BMS_CellPwmDuty_Seg5_Seg9_getData());

SegmentBroadcaster<bool, 10> cell_owc_ok_setters(app::can_tx::BMS_CellOpenWireCheckOk_getData());
SegmentBroadcaster<bool, 10, THERMISTORS_PER_SEGMENT> therm_owc_ok_setters(
    app::can_tx::BMS_ThermistorOpenWireCheckOk_getData());
SegmentBroadcaster<bool, 10> cell_ov_ok_setters(app::can_tx::BMS_CellOverVoltageOk_getData());
SegmentBroadcaster<bool, 10> cell_uv_ok_setters(app::can_tx::BMS_CellUnderVoltageOk_getData());
SegmentBroadcaster<bool, 10, 1> segment_therm_shdn_ok_setters(
    app::can_tx::BMS_SegmentThermShutdownOk_getData());
SegmentBroadcaster<bool, 10, 1> segment_self_test_ok_setters(app::can_tx::BMS_SegmentSelfTestOk_getData());
SegmentBroadcaster<bool, 10, 1> segment_supply_ok_setters(app::can_tx::BMS_SegmentSupplyOk_getData());
} // namespace

namespace app::pack::broadcast
{
void cellVoltages(const VoltageBuffer &stats)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const result<float> &voltage = stats.voltages[seg][cell];

            cell_voltage_setters[seg][cell] = std::clamp(voltage.value_or(INVALID_VOLTAGE), -0.1f, 5.0f);
            cell_voltage_error_setters[seg][cell] =
                voltage ? can_utils::ErrorCode::NO_ERROR : static_cast<can_utils::ErrorCode>(voltage.error());
        }
    }

    can_tx::BMS_MinCellVoltage_set(stats.min.value);
    can_tx::BMS_MinCellVoltageSegment_set(stats.min.segment);
    can_tx::BMS_MinCellVoltageCell_set(stats.min.index);
    can_tx::BMS_MaxCellVoltage_set(stats.max.value);
    can_tx::BMS_MaxCellVoltageSegment_set(stats.max.segment);
    can_tx::BMS_MaxCellVoltageCell_set(stats.max.index);
}

void cellTemps(const TemperatureBuffer &stats)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
        {
            const result<float> &temp = stats.temperatures[seg][therm];

            cell_temperature_setters[seg][therm] = std::clamp(temp.value_or(INVALID_TEMP), -40.0f, 125.0f);
            cell_temperature_error_setters[seg][therm] =
                temp ? can_utils::ErrorCode::NO_ERROR : static_cast<can_utils::ErrorCode>(temp.error());
        }
    }

    can_tx::BMS_MinCellTemp_set(stats.min.value);
    can_tx::BMS_MinCellTempSegment_set(stats.min.segment);
    can_tx::BMS_MinCellTempCell_set(stats.min.index);
    can_tx::BMS_MaxCellTemp_set(stats.max.value);
    can_tx::BMS_MaxCellTempSegment_set(stats.max.segment);
    can_tx::BMS_MaxCellTempCell_set(stats.max.index);
}

void cellOpenWire(const OpenWireCheckBuffer &owc)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            cell_owc_ok_setters[seg][cell] = owc.cells_owc_ok[seg][cell];

        for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
            therm_owc_ok_setters[seg][therm] = owc.therms_owc_ok[seg][therm];
    }
}

void adbmsFlags(const DiagnosticBuffer &diag)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            cell_ov_ok_setters[seg][cell] = diag.ov_ok[seg][cell];
            cell_uv_ok_setters[seg][cell] = diag.uv_ok[seg][cell];
        }
        segment_therm_shdn_ok_setters[seg][0] = diag.therm_shdn_ok[seg];
        segment_self_test_ok_setters[seg][0]  = diag.self_test_ok[seg];
        segment_supply_ok_setters[seg][0]     = diag.supply_ok[seg];
    }
}

void adbmsSupply(const SupplyBuffer &supply)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        segment_vmv_setters[seg][0]  = std::clamp(supply.vmv[seg].value_or(INVALID_VOLTAGE), -1.0f, 5.0f);
        segment_vpv_setters[seg][0]  = std::clamp(supply.vpv[seg].value_or(INVALID_VOLTAGE), -0.1f, 65.0f);
        segment_vref_setters[seg][0] = std::clamp(supply.vref[seg].value_or(INVALID_VOLTAGE), -0.1f, 5.0f);
        segment_itmp_setters[seg][0] = std::clamp(supply.itmp[seg].value_or(INVALID_TEMP), -40.0f, 150.0f);
        segment_va_setters[seg][0]   = std::clamp(supply.va[seg].value_or(INVALID_VOLTAGE), -0.1f, 7.0f);
        segment_vd_setters[seg][0]   = std::clamp(supply.vd[seg].value_or(INVALID_VOLTAGE), -0.1f, 5.0f);
        segment_vres_setters[seg][0] = std::clamp(supply.vres[seg].value_or(INVALID_VOLTAGE), -0.1f, 5.0f);

        segment_vmv_error_setters[seg][0]  = supply.vmv[seg]
                                                 ? can_utils::ErrorCode::NO_ERROR
                                                 : static_cast<can_utils::ErrorCode>(supply.vmv[seg].error());
        segment_vpv_error_setters[seg][0]  = supply.vpv[seg]
                                                 ? can_utils::ErrorCode::NO_ERROR
                                                 : static_cast<can_utils::ErrorCode>(supply.vpv[seg].error());
        segment_vref_error_setters[seg][0] = supply.vref[seg]
                                                 ? can_utils::ErrorCode::NO_ERROR
                                                 : static_cast<can_utils::ErrorCode>(supply.vref[seg].error());
        segment_itmp_error_setters[seg][0] = supply.itmp[seg]
                                                 ? can_utils::ErrorCode::NO_ERROR
                                                 : static_cast<can_utils::ErrorCode>(supply.itmp[seg].error());
        segment_va_error_setters[seg][0] =
            supply.va[seg] ? can_utils::ErrorCode::NO_ERROR : static_cast<can_utils::ErrorCode>(supply.va[seg].error());
        segment_vd_error_setters[seg][0] =
            supply.vd[seg] ? can_utils::ErrorCode::NO_ERROR : static_cast<can_utils::ErrorCode>(supply.vd[seg].error());
        segment_vres_error_setters[seg][0] = supply.vres[seg]
                                                 ? can_utils::ErrorCode::NO_ERROR
                                                 : static_cast<can_utils::ErrorCode>(supply.vres[seg].error());
    }
}

void balancing(const io::adbms::PackCells<uint8_t> &duty)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            cell_pwm_duty_setters[seg][cell] = duty[seg][cell];
    }
}
} // namespace app::pack::broadcast
