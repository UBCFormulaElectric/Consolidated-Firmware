#include "app_pack.hpp"
#include "app_pack_internal.hpp"

#include <array>
#include <span>

#include "app_canTx.hpp"
#include "io_canTx.hpp"

namespace {

    constexpr float INVALID_VOLTAGE = -0.1f;
    constexpr float INVALID_TEMP    = -40.0f;
    template <typename T, size_t N, void (*Send)()> class BroadcastBuffer {
        std::span<T, N> _buf;

      public:
        template <typename CanMsg> explicit BroadcastBuffer(CanMsg &can_msg) : _buf{ reinterpret_cast<T *>(&can_msg), N } {
            static_assert(sizeof(CanMsg) == N * sizeof(T));
        }

        T &operator[](const size_t i) const { return _buf[i]; }

        void send() const { Send(); }
    };

    template <typename T, size_t SEGS, void (*... Sends)()> class CellBroadcaster {
        static constexpr size_t NUM_MSGS = sizeof...(Sends);
        static_assert(SEGS * NUM_MSGS == NUM_SEGMENTS);

        std::array<T *, NUM_MSGS> _msgs;

      public:
        template <typename... CanMsgs>
        explicit CellBroadcaster(CanMsgs &...can_msgs) : _msgs{ { reinterpret_cast<T *>(&can_msgs)... } } {
            static_assert(sizeof...(CanMsgs) == NUM_MSGS);
            static_assert(((sizeof(CanMsgs) == SEGS * CELLS_PER_SEGMENT * sizeof(T)) && ...));
        }

        std::span<T, CELLS_PER_SEGMENT> operator[](const size_t seg) const {
            return std::span<T, CELLS_PER_SEGMENT>{ _msgs[seg / SEGS] + seg % SEGS * CELLS_PER_SEGMENT,
                                                    CELLS_PER_SEGMENT };
        }

        void send() const { (Sends(), ...); }
    };

    CellBroadcaster<
        float,
        2,
        io::can_tx::BMS_CellVoltages_Seg0_Seg1_sendAperiodic,
        io::can_tx::BMS_CellVoltages_Seg2_Seg3_sendAperiodic,
        io::can_tx::BMS_CellVoltages_Seg4_Seg5_sendAperiodic,
        io::can_tx::BMS_CellVoltages_Seg6_Seg7_sendAperiodic,
        io::can_tx::BMS_CellVoltages_Seg8_Seg9_sendAperiodic>
        cell_voltage_setters(
            app::can_tx::BMS_CellVoltages_Seg0_Seg1_getData(),
            app::can_tx::BMS_CellVoltages_Seg2_Seg3_getData(),
            app::can_tx::BMS_CellVoltages_Seg4_Seg5_getData(),
            app::can_tx::BMS_CellVoltages_Seg6_Seg7_getData(),
            app::can_tx::BMS_CellVoltages_Seg8_Seg9_getData());

    CellBroadcaster<
        float,
        2,
        io::can_tx::BMS_CellTemps_Seg0_Seg1_sendAperiodic,
        io::can_tx::BMS_CellTemps_Seg2_Seg3_sendAperiodic,
        io::can_tx::BMS_CellTemps_Seg4_Seg5_sendAperiodic,
        io::can_tx::BMS_CellTemps_Seg6_Seg7_sendAperiodic,
        io::can_tx::BMS_CellTemps_Seg8_Seg9_sendAperiodic>
        cell_temperature_setters(
            app::can_tx::BMS_CellTemps_Seg0_Seg1_getData(),
            app::can_tx::BMS_CellTemps_Seg2_Seg3_getData(),
            app::can_tx::BMS_CellTemps_Seg4_Seg5_getData(),
            app::can_tx::BMS_CellTemps_Seg6_Seg7_getData(),
            app::can_tx::BMS_CellTemps_Seg8_Seg9_getData());

    CellBroadcaster<
        uint8_t,
        5,
        io::can_tx::BMS_CellPwmDuty_Seg0_Seg4_sendAperiodic,
        io::can_tx::BMS_CellPwmDuty_Seg5_Seg9_sendAperiodic>
        cell_pwm_duty_setters(
            app::can_tx::BMS_CellPwmDuty_Seg0_Seg4_getData(),
            app::can_tx::BMS_CellPwmDuty_Seg5_Seg9_getData());

    BroadcastBuffer<bool, NUM_SEGMENTS * CELLS_PER_SEGMENT, io::can_tx::BMS_CellVoltagesValid_sendAperiodic>
        cell_voltage_valid_buffer(app::can_tx::BMS_CellVoltagesValid_getData());
    BroadcastBuffer<bool, NUM_SEGMENTS * THERMISTORS_PER_SEGMENT, io::can_tx::BMS_CellTempsValid_sendAperiodic>
        cell_temperature_valid_buffer(app::can_tx::BMS_CellTempsValid_getData());
    BroadcastBuffer<bool, NUM_SEGMENTS * CELLS_PER_SEGMENT, io::can_tx::BMS_CellOpenWireCheckOk_sendAperiodic>
        cell_owc_ok_buffer(app::can_tx::BMS_CellOpenWireCheckOk_getData());
    BroadcastBuffer<
        bool,
        NUM_SEGMENTS * THERMISTORS_PER_SEGMENT,
        io::can_tx::BMS_ThermistorOpenWireCheckOk_sendAperiodic>
        therm_owc_ok_buffer(app::can_tx::BMS_ThermistorOpenWireCheckOk_getData());
    BroadcastBuffer<bool, NUM_SEGMENTS * CELLS_PER_SEGMENT, io::can_tx::BMS_CellOverVoltageOk_sendAperiodic>
        cell_ov_ok_buffer(app::can_tx::BMS_CellOverVoltageOk_getData());
    BroadcastBuffer<bool, NUM_SEGMENTS * CELLS_PER_SEGMENT, io::can_tx::BMS_CellUnderVoltageOk_sendAperiodic>
        cell_uv_ok_buffer(app::can_tx::BMS_CellUnderVoltageOk_getData());
    BroadcastBuffer<bool, NUM_SEGMENTS, io::can_tx::BMS_SegmentThermShutdownOk_sendAperiodic>
        segment_therm_shdn_ok_buffer(app::can_tx::BMS_SegmentThermShutdownOk_getData());
    BroadcastBuffer<bool, NUM_SEGMENTS, io::can_tx::BMS_SegmentSelfTestOk_sendAperiodic>
        segment_self_test_ok_buffer(app::can_tx::BMS_SegmentSelfTestOk_getData());
    BroadcastBuffer<bool, NUM_SEGMENTS, io::can_tx::BMS_SegmentSupplyOk_sendAperiodic>
        segment_supply_ok_buffer(app::can_tx::BMS_SegmentSupplyOk_getData());
}

namespace app::pack::broadcast {
    void cellVoltages(const VoltStats &stats) {
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++) {
                const bool valid = stats.valid[seg][cell];

                cell_voltage_setters[seg][cell]                            = valid ? stats.voltages[seg][cell] : INVALID_VOLTAGE;
                cell_voltage_valid_buffer[seg * CELLS_PER_SEGMENT + cell] = valid;
            }
        }
        cell_voltage_setters.send();
        cell_voltage_valid_buffer.send();

        can_tx::BMS_MinCellVoltage_set((stats.updated_ms != 0U) ? stats.min.value : INVALID_VOLTAGE);
        can_tx::BMS_MinCellVoltageSegment_set(stats.min.segment);
        can_tx::BMS_MinCellVoltageCell_set(stats.min.index);
        can_tx::BMS_MaxCellVoltage_set((stats.updated_ms != 0U) ? stats.max.value : INVALID_VOLTAGE);
        can_tx::BMS_MaxCellVoltageSegment_set(stats.max.segment);
        can_tx::BMS_MaxCellVoltageCell_set(stats.max.index);
        io::can_tx::BMS_CellVoltageStats_sendAperiodic();
    }

    void cellTemps(const TempStats &stats) {
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++) {
                const bool valid = stats.valid[seg][therm];

                cell_temperature_setters[seg][therm]                                = valid ? stats.temperatures[seg][therm] : INVALID_TEMP;
                cell_temperature_valid_buffer[seg * THERMISTORS_PER_SEGMENT + therm] = valid;
            }
        }
        cell_temperature_setters.send();
        cell_temperature_valid_buffer.send();

        can_tx::BMS_MinCellTemp_set((stats.updated_ms != 0U) ? stats.min.value : INVALID_TEMP);
        can_tx::BMS_MinCellTempSegment_set(stats.min.segment);
        can_tx::BMS_MinCellTempCell_set(stats.min.index);
        can_tx::BMS_MaxCellTemp_set((stats.updated_ms != 0U) ? stats.max.value : INVALID_TEMP);
        can_tx::BMS_MaxCellTempSegment_set(stats.max.segment);
        can_tx::BMS_MaxCellTempCell_set(stats.max.index);
        io::can_tx::BMS_CellTempStats_sendAperiodic();
    }

    void cellOpenWire(const OwcStats &stats) {
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
                cell_owc_ok_buffer[seg * CELLS_PER_SEGMENT + cell] = stats.cells_ok[seg][cell];

            for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
                therm_owc_ok_buffer[seg * THERMISTORS_PER_SEGMENT + therm] = stats.therms_ok[seg][therm];
        }
        cell_owc_ok_buffer.send();
        therm_owc_ok_buffer.send();
    }

    void adbmsFlags(const ADBMS6830Diag &diag) {
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++) {
                cell_ov_ok_buffer[seg * CELLS_PER_SEGMENT + cell] = diag.ov_ok[seg][cell];
                cell_uv_ok_buffer[seg * CELLS_PER_SEGMENT + cell] = diag.uv_ok[seg][cell];
            }
            segment_therm_shdn_ok_buffer[seg] = diag.therm_shdn_ok[seg];
            segment_self_test_ok_buffer[seg]  = diag.self_test_ok[seg];
            segment_supply_ok_buffer[seg]     = diag.supply_ok[seg];
        }
        cell_ov_ok_buffer.send();
        cell_uv_ok_buffer.send();
        segment_therm_shdn_ok_buffer.send();
        segment_self_test_ok_buffer.send();
        segment_supply_ok_buffer.send();
    }

    void balancing(const io::adbms::Cells<uint8_t> &duty) {
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
                cell_pwm_duty_setters[seg][cell] = duty[seg][cell];
        }
        cell_pwm_duty_setters.send();
    }
}
