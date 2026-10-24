#include "app_pack.hpp"
#include "app_pack_internal.hpp"

#include <array>
#include <span>

#include "app_canTx.hpp"

using io::adbms::CELLS_PER_SEGMENT;
using io::adbms::NUM_SEGMENTS;
using io::adbms::THERMISTORS_PER_SEGMENT;

namespace
{

constexpr float  INVALID_VOLTAGE             = -0.1f;
constexpr float  INVALID_TEMP                = -40.0f;
constexpr size_t SUPPLY_READINGS_PER_SEGMENT = 7;

template <typename T, size_t N> class BroadcastBuffer
{
    std::span<T, N> _buf;

  public:
    template <typename CanMsg> explicit BroadcastBuffer(CanMsg &can_msg) : _buf{ reinterpret_cast<T *>(&can_msg), N }
    {
        static_assert(sizeof(CanMsg) == N * sizeof(T));
    }

    T &operator[](const size_t i) const { return _buf[i]; }
};

template <typename T, size_t SEGS, size_t PER_SEG = CELLS_PER_SEGMENT> class CellBroadcaster
{
    static constexpr size_t NUM_MSGS = NUM_SEGMENTS / SEGS;
    static_assert(SEGS * NUM_MSGS == NUM_SEGMENTS);

    std::array<T *, NUM_MSGS> _msgs;

  public:
    template <typename... CanMsgs>
    explicit CellBroadcaster(CanMsgs &...can_msgs) : _msgs{ { reinterpret_cast<T *>(&can_msgs)... } }
    {
        static_assert(sizeof...(CanMsgs) == NUM_MSGS);
        static_assert(((sizeof(CanMsgs) == SEGS * PER_SEG * sizeof(T)) && ...));
    }

    std::span<T, PER_SEG> operator[](const size_t seg) const
    {
        return std::span<T, PER_SEG>{ _msgs[seg / SEGS] + seg % SEGS * PER_SEG, PER_SEG };
    }
};

CellBroadcaster<float, 2> cell_voltage_setters(
    app::can_tx::BMS_CellVoltages_Seg0_Seg1_getData(),
    app::can_tx::BMS_CellVoltages_Seg2_Seg3_getData(),
    app::can_tx::BMS_CellVoltages_Seg4_Seg5_getData(),
    app::can_tx::BMS_CellVoltages_Seg6_Seg7_getData(),
    app::can_tx::BMS_CellVoltages_Seg8_Seg9_getData());
CellBroadcaster<app::can_utils::ErrorCode, 5> cell_voltage_error_setters(
    app::can_tx::BMS_CellVoltageErrors_Seg0_Seg4_getData(),
    app::can_tx::BMS_CellVoltageErrors_Seg5_Seg9_getData());

CellBroadcaster<float, 2> cell_temperature_setters(
    app::can_tx::BMS_CellTemps_Seg0_Seg1_getData(),
    app::can_tx::BMS_CellTemps_Seg2_Seg3_getData(),
    app::can_tx::BMS_CellTemps_Seg4_Seg5_getData(),
    app::can_tx::BMS_CellTemps_Seg6_Seg7_getData(),
    app::can_tx::BMS_CellTemps_Seg8_Seg9_getData());
CellBroadcaster<app::can_utils::ErrorCode, 5> cell_temperature_error_setters(
    app::can_tx::BMS_CellTempErrors_Seg0_Seg4_getData(),
    app::can_tx::BMS_CellTempErrors_Seg5_Seg9_getData());

CellBroadcaster<float, 5, SUPPLY_READINGS_PER_SEGMENT> segment_supply_setters(
    app::can_tx::BMS_SegmentSupply_Seg0_Seg4_getData(),
    app::can_tx::BMS_SegmentSupply_Seg5_Seg9_getData());
CellBroadcaster<app::can_utils::ErrorCode, NUM_SEGMENTS, SUPPLY_READINGS_PER_SEGMENT> segment_supply_error_setters(
    app::can_tx::BMS_SegmentSupplyErrors_getData());

CellBroadcaster<uint8_t, 5> cell_pwm_duty_setters(
    app::can_tx::BMS_CellPwmDuty_Seg0_Seg4_getData(),
    app::can_tx::BMS_CellPwmDuty_Seg5_Seg9_getData());

BroadcastBuffer<bool, NUM_SEGMENTS * CELLS_PER_SEGMENT> cell_owc_ok_buffer(
    app::can_tx::BMS_CellOpenWireCheckOk_getData());
BroadcastBuffer<bool, NUM_SEGMENTS * THERMISTORS_PER_SEGMENT> therm_owc_ok_buffer(
    app::can_tx::BMS_ThermistorOpenWireCheckOk_getData());
BroadcastBuffer<bool, NUM_SEGMENTS * CELLS_PER_SEGMENT> cell_ov_ok_buffer(app::can_tx::BMS_CellOverVoltageOk_getData());
BroadcastBuffer<bool, NUM_SEGMENTS * CELLS_PER_SEGMENT> cell_uv_ok_buffer(app::can_tx::BMS_CellUnderVoltageOk_getData());
BroadcastBuffer<bool, NUM_SEGMENTS> segment_therm_shdn_ok_buffer(app::can_tx::BMS_SegmentThermShutdownOk_getData());
BroadcastBuffer<bool, NUM_SEGMENTS> segment_self_test_ok_buffer(app::can_tx::BMS_SegmentSelfTestOk_getData());
BroadcastBuffer<bool, NUM_SEGMENTS> segment_supply_ok_buffer(app::can_tx::BMS_SegmentSupplyOk_getData());
} // namespace

// All of these messages are periodic, so these only update the TX table and the 1kHz task sends them out
namespace app::pack::broadcast
{
void cellVoltages(const VoltageBuffer &stats)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const result<float> &voltage = stats.voltages[seg][cell];

            cell_voltage_setters[seg][cell] = voltage.value_or(INVALID_VOLTAGE);
            cell_voltage_error_setters[seg][cell] =
                voltage ? can_utils::ErrorCode::NO_ERROR : static_cast<can_utils::ErrorCode>(voltage.error());
        }
    }

    can_tx::BMS_MinCellVoltage_set((stats.updated_ms != 0U) ? stats.min.value : INVALID_VOLTAGE);
    can_tx::BMS_MinCellVoltageSegment_set(stats.min.segment);
    can_tx::BMS_MinCellVoltageCell_set(stats.min.index);
    can_tx::BMS_MaxCellVoltage_set((stats.updated_ms != 0U) ? stats.max.value : INVALID_VOLTAGE);
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

            cell_temperature_setters[seg][therm] = temp.value_or(INVALID_TEMP);
            cell_temperature_error_setters[seg][therm] =
                temp ? can_utils::ErrorCode::NO_ERROR : static_cast<can_utils::ErrorCode>(temp.error());
        }
    }

    can_tx::BMS_MinCellTemp_set((stats.updated_ms != 0U) ? stats.min.value : INVALID_TEMP);
    can_tx::BMS_MinCellTempSegment_set(stats.min.segment);
    can_tx::BMS_MinCellTempCell_set(stats.min.index);
    can_tx::BMS_MaxCellTemp_set((stats.updated_ms != 0U) ? stats.max.value : INVALID_TEMP);
    can_tx::BMS_MaxCellTempSegment_set(stats.max.segment);
    can_tx::BMS_MaxCellTempCell_set(stats.max.index);
}

void cellOpenWire(const OpenWireCheckBuffer &owc)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            cell_owc_ok_buffer[seg * CELLS_PER_SEGMENT + cell] = owc.cells_owc_ok[seg][cell];

        for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
            therm_owc_ok_buffer[seg * THERMISTORS_PER_SEGMENT + therm] = owc.therms_owc_ok[seg][therm];
    }
}

void adbmsFlags(const DiagnosticBuffer &diag)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            cell_ov_ok_buffer[seg * CELLS_PER_SEGMENT + cell] = diag.ov_ok[seg][cell];
            cell_uv_ok_buffer[seg * CELLS_PER_SEGMENT + cell] = diag.uv_ok[seg][cell];
        }
        segment_therm_shdn_ok_buffer[seg] = diag.therm_shdn_ok[seg];
        segment_self_test_ok_buffer[seg]  = diag.self_test_ok[seg];
        segment_supply_ok_buffer[seg]     = diag.supply_ok[seg];
    }
}

void adbmsSupply(const SupplyBuffer &supply)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        // Same order as the SegmentSupply signals in BMS_tx.json
        const std::array<result<float>, SUPPLY_READINGS_PER_SEGMENT> readings{ { supply.vmv[seg], supply.vpv[seg],
                                                                                supply.vref[seg], supply.itmp[seg],
                                                                                supply.va[seg], supply.vd[seg],
                                                                                supply.vres[seg] } };

        for (uint8_t i = 0; i < SUPPLY_READINGS_PER_SEGMENT; i++)
        {
            segment_supply_setters[seg][i] = readings[i].value_or(INVALID_VOLTAGE);
            segment_supply_error_setters[seg][i] =
                readings[i] ? can_utils::ErrorCode::NO_ERROR : static_cast<can_utils::ErrorCode>(readings[i].error());
        }
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
