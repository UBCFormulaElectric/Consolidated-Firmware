#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "app_canTx.hpp"

#include <algorithm>
#include <app_canUtils.hpp>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>

using io::adbms::CELLS_PER_SEGMENT;
using io::adbms::NUM_SEGMENTS;
using io::adbms::THERMISTORS_PER_SEGMENT;

namespace
{
namespace can_tx    = app::can_tx;
namespace can_utils = app::can_utils;

constexpr float INVALID_VOLTAGE = -0.1f;
constexpr float INVALID_TEMP    = -40.0f;

// The backing CAN objects must contain a contiguous sequence of actual T
// objects. sizeof alone cannot prove this; see note below.
template <typename T, size_t SEGS_PER_MSG, size_t VALUES_PER_SEG> class ArrayBroadcaster
{
    static constexpr size_t    MSG_COUNT = (NUM_SEGMENTS + SEGS_PER_MSG - 1) / SEGS_PER_MSG;
    std::array<T *, MSG_COUNT> messages_;

  public:
    template <typename... Messages>
    explicit ArrayBroadcaster(Messages &...messages) : messages_{ { reinterpret_cast<T *>(&messages)... } }
    {
        static_assert(sizeof...(Messages) == MSG_COUNT);
        static_assert(((sizeof(Messages) == SEGS_PER_MSG * VALUES_PER_SEG * sizeof(T)) && ...));
    }

    std::span<T, VALUES_PER_SEG> operator[](size_t segment) const
    {
        assert(segment < NUM_SEGMENTS);

        const size_t msg   = segment / SEGS_PER_MSG;
        const size_t first = (segment % SEGS_PER_MSG) * VALUES_PER_SEG;

        return std::span<T, VALUES_PER_SEG>{ messages_[msg] + first, VALUES_PER_SEG };
    }
};

template <typename T, size_t SEGS_PER_MSG> using CellBroadcaster = ArrayBroadcaster<T, SEGS_PER_MSG, CELLS_PER_SEGMENT>;

template <typename T, size_t SEGS_PER_MSG>
using ThermBroadcaster = ArrayBroadcaster<T, SEGS_PER_MSG, THERMISTORS_PER_SEGMENT>;

// One value per battery segment, so access is values[seg], not values[seg][0].
template <typename T> class SegmentValues
{
    T *values_;

  public:
    template <typename Message> explicit SegmentValues(Message &message) : values_(reinterpret_cast<T *>(&message))
    {
        static_assert(sizeof(Message) == NUM_SEGMENTS * sizeof(T));
    }

    T &operator[](size_t segment) const
    {
        assert(segment < NUM_SEGMENTS);
        return values_[segment];
    }
};

struct CellBroadcastData
{
    CellBroadcaster<float, 2> cell_voltage_setters{ app::can_tx::BMS_CellVoltages_Seg0_Seg1_getData(),
                                                    app::can_tx::BMS_CellVoltages_Seg2_Seg3_getData(),
                                                    app::can_tx::BMS_CellVoltages_Seg4_Seg5_getData(),
                                                    app::can_tx::BMS_CellVoltages_Seg6_Seg7_getData(),
                                                    app::can_tx::BMS_CellVoltages_Seg8_Seg9_getData() };

    CellBroadcaster<app::can_utils::ErrorCode, 5> cell_voltage_error_setters{
        app::can_tx::BMS_CellVoltageErrors_Seg0_Seg4_getData(), app::can_tx::BMS_CellVoltageErrors_Seg5_Seg9_getData()
    };
};

struct TemperatureBroadcastData
{
    ThermBroadcaster<float, 2> cell_temperature_setters{ app::can_tx::BMS_CellTemps_Seg0_Seg1_getData(),
                                                         app::can_tx::BMS_CellTemps_Seg2_Seg3_getData(),
                                                         app::can_tx::BMS_CellTemps_Seg4_Seg5_getData(),
                                                         app::can_tx::BMS_CellTemps_Seg6_Seg7_getData(),
                                                         app::can_tx::BMS_CellTemps_Seg8_Seg9_getData() };

    ThermBroadcaster<app::can_utils::ErrorCode, 5> cell_temperature_error_setters{
        app::can_tx::BMS_CellTempErrors_Seg0_Seg4_getData(), app::can_tx::BMS_CellTempErrors_Seg5_Seg9_getData()
    };

    ThermBroadcaster<float, 2> them_voltage_setters{ app::can_tx::BMS_ThermVoltages_Seg0_Seg1_getData(),
                                                     app::can_tx::BMS_ThermVoltages_Seg2_Seg3_getData(),
                                                     app::can_tx::BMS_ThermVoltages_Seg4_Seg5_getData(),
                                                     app::can_tx::BMS_ThermVoltages_Seg6_Seg7_getData(),
                                                     app::can_tx::BMS_ThermVoltages_Seg8_Seg9_getData() };
};

struct SupplyBroadcastData
{
    SegmentValues<float>                     segment_vmv_setters{ app::can_tx::BMS_SegmentVmv_getData() };
    SegmentValues<float>                     segment_vpv_setters{ app::can_tx::BMS_SegmentVpv_getData() };
    SegmentValues<float>                     segment_vref_setters{ app::can_tx::BMS_SegmentVref_getData() };
    SegmentValues<float>                     segment_itmp_setters{ app::can_tx::BMS_SegmentItmp_getData() };
    SegmentValues<float>                     segment_va_setters{ app::can_tx::BMS_SegmentVa_getData() };
    SegmentValues<float>                     segment_vd_setters{ app::can_tx::BMS_SegmentVd_getData() };
    SegmentValues<float>                     segment_vres_setters{ app::can_tx::BMS_SegmentVres_getData() };
    SegmentValues<app::can_utils::ErrorCode> segment_vmv_error_setters{ app::can_tx::BMS_SegmentVmvErrors_getData() };
    SegmentValues<app::can_utils::ErrorCode> segment_vpv_error_setters{ app::can_tx::BMS_SegmentVpvErrors_getData() };
    SegmentValues<app::can_utils::ErrorCode> segment_vref_error_setters{ app::can_tx::BMS_SegmentVrefErrors_getData() };
    SegmentValues<app::can_utils::ErrorCode> segment_itmp_error_setters{ app::can_tx::BMS_SegmentItmpErrors_getData() };
    SegmentValues<app::can_utils::ErrorCode> segment_va_error_setters{ app::can_tx::BMS_SegmentVaErrors_getData() };
    SegmentValues<app::can_utils::ErrorCode> segment_vd_error_setters{ app::can_tx::BMS_SegmentVdErrors_getData() };
    SegmentValues<app::can_utils::ErrorCode> segment_vres_error_setters{ app::can_tx::BMS_SegmentVresErrors_getData() };
};

struct DiagnosticBroadcastData
{
    CellBroadcaster<bool, 10>  cadc_owc_ok_setters{ app::can_tx::BMS_CadcCellOpenWireCheckOk_getData() };
    CellBroadcaster<bool, 10>  sadc_owc_ok_setters{ app::can_tx::BMS_SadcCellOpenWireCheckOk_getData() };
    CellBroadcaster<bool, 10>  cell_ov_ok_setters{ app::can_tx::BMS_CellOverVoltageOk_getData() };
    CellBroadcaster<bool, 10>  cell_uv_ok_setters{ app::can_tx::BMS_CellUnderVoltageOk_getData() };
    ThermBroadcaster<bool, 10> therm_owc_ok_setters{ app::can_tx::BMS_ThermistorOpenWireCheckOk_getData() };
    SegmentValues<bool>        segment_therm_shdn_ok_setters{ app::can_tx::BMS_SegmentThermShutdownOk_getData() };
    SegmentValues<bool>        segment_self_test_ok_setters{ app::can_tx::BMS_SegmentSelfTestOk_getData() };
    SegmentValues<bool>        segment_supply_ok_setters{ app::can_tx::BMS_SegmentSupplyOk_getData() };
};

struct BalancingBroadcastData
{
    CellBroadcaster<uint8_t, 5> cell_pwm_duty_setters{ app::can_tx::BMS_CellPwmDuty_Seg0_Seg4_getData(),
                                                       app::can_tx::BMS_CellPwmDuty_Seg5_Seg9_getData() };
};

BalancingBroadcastData   balancing_broadcast;
DiagnosticBroadcastData  diagnostic_broadcast;
CellBroadcastData        cell_broadcast;
SupplyBroadcastData      supply_broadcast;
TemperatureBroadcastData temp_broadcast;

} // namespace

namespace app::pack::broadcast
{
void cellVoltages(const io::adbms::VoltageBuffer &stats)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const result<float> &voltage = stats.voltages[seg][cell];

            cell_broadcast.cell_voltage_setters[seg][cell] = std::clamp(voltage.value_or(INVALID_VOLTAGE), -0.1f, 5.0f);
            cell_broadcast.cell_voltage_error_setters[seg][cell] =
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

void cellTemps(const io::adbms::TemperatureBuffer &stats)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
        {
            const result<float> &temp = stats.temperatures[seg][therm];

            temp_broadcast.cell_temperature_setters[seg][therm] =
                std::clamp(temp.value_or(INVALID_TEMP), -40.0f, 125.0f);
            temp_broadcast.cell_temperature_error_setters[seg][therm] =
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

void thermVoltages(const io::adbms::PackTherms<result<float>> &voltages)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
            temp_broadcast.them_voltage_setters[seg][therm] =
                std::clamp(voltages[seg][therm].value_or(INVALID_VOLTAGE), -0.1f, 5.0f);
    }
}

void cadcOpenWire(const OpenWireCheckBuffer &owc)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            diagnostic_broadcast.cadc_owc_ok_setters[seg][cell] = owc.cells_owc_ok[seg][cell];
    }
}

void sadcOpenWire(const OpenWireCheckBuffer &owc)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            diagnostic_broadcast.sadc_owc_ok_setters[seg][cell] = owc.cells_owc_ok[seg][cell];
    }
}

void thermOpenWire(const ThermOpenWireCheckBuffer &owc)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
            diagnostic_broadcast.therm_owc_ok_setters[seg][therm] = owc.therms_owc_ok[seg][therm];
    }
}

void adbmsFlags(const io::adbms::DiagnosticBuffer &diag)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            diagnostic_broadcast.cell_ov_ok_setters[seg][cell] = diag.ov_ok[seg][cell];
            diagnostic_broadcast.cell_uv_ok_setters[seg][cell] = diag.uv_ok[seg][cell];
        }
        diagnostic_broadcast.segment_therm_shdn_ok_setters[seg] = diag.therm_shdn_ok[seg];
        diagnostic_broadcast.segment_self_test_ok_setters[seg]  = diag.self_test_ok[seg];
        diagnostic_broadcast.segment_supply_ok_setters[seg]     = diag.supply_ok[seg];
    }
}

void adbmsSupply(const io::adbms::SupplyBuffer &supply)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        supply_broadcast.segment_vmv_setters[seg] = std::clamp(supply.vmv[seg].value_or(INVALID_VOLTAGE), -1.0f, 5.0f);
        supply_broadcast.segment_vpv_setters[seg] = std::clamp(supply.vpv[seg].value_or(INVALID_VOLTAGE), -0.1f, 65.0f);
        supply_broadcast.segment_vref_setters[seg] =
            std::clamp(supply.vref[seg].value_or(INVALID_VOLTAGE), -0.1f, 5.0f);
        supply_broadcast.segment_itmp_setters[seg] =
            std::clamp(supply.itmp[seg].value_or(INVALID_TEMP), -40.0f, 150.0f);
        supply_broadcast.segment_va_setters[seg] = std::clamp(supply.va[seg].value_or(INVALID_VOLTAGE), -0.1f, 7.0f);
        supply_broadcast.segment_vd_setters[seg] = std::clamp(supply.vd[seg].value_or(INVALID_VOLTAGE), -0.1f, 5.0f);
        supply_broadcast.segment_vres_setters[seg] =
            std::clamp(supply.vres[seg].value_or(INVALID_VOLTAGE), -0.1f, 5.0f);

        supply_broadcast.segment_vmv_error_setters[seg] =
            supply.vmv[seg] ? can_utils::ErrorCode::NO_ERROR
                            : static_cast<can_utils::ErrorCode>(supply.vmv[seg].error());
        supply_broadcast.segment_vpv_error_setters[seg] =
            supply.vpv[seg] ? can_utils::ErrorCode::NO_ERROR
                            : static_cast<can_utils::ErrorCode>(supply.vpv[seg].error());
        supply_broadcast.segment_vref_error_setters[seg] =
            supply.vref[seg] ? can_utils::ErrorCode::NO_ERROR
                             : static_cast<can_utils::ErrorCode>(supply.vref[seg].error());
        supply_broadcast.segment_itmp_error_setters[seg] =
            supply.itmp[seg] ? can_utils::ErrorCode::NO_ERROR
                             : static_cast<can_utils::ErrorCode>(supply.itmp[seg].error());
        supply_broadcast.segment_va_error_setters[seg] =
            supply.va[seg] ? can_utils::ErrorCode::NO_ERROR : static_cast<can_utils::ErrorCode>(supply.va[seg].error());
        supply_broadcast.segment_vd_error_setters[seg] =
            supply.vd[seg] ? can_utils::ErrorCode::NO_ERROR : static_cast<can_utils::ErrorCode>(supply.vd[seg].error());
        supply_broadcast.segment_vres_error_setters[seg] =
            supply.vres[seg] ? can_utils::ErrorCode::NO_ERROR
                             : static_cast<can_utils::ErrorCode>(supply.vres[seg].error());
    }
}

void balancing(const io::adbms::PackCells<uint8_t> &duty)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            balancing_broadcast.cell_pwm_duty_setters[seg][cell] = duty[seg][cell];
    }
}
} // namespace app::pack::broadcast