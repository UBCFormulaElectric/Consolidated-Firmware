#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "app_timer.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_stateMachine.hpp"
#include "states/app_states.hpp"

using namespace app;

namespace
{
constexpr float MAX_CELL_CHARGE_TEMP_DEGC = 45.0f;

// Makes sure segments do not fault during init
constexpr uint32_t STARTUP_BLANKING_MS = 300;
Timer              startup_blanking_timer(STARTUP_BLANKING_MS);

struct FaultThresholds
{
    float    min_voltage;
    float    max_voltage;
    float    max_temp;
    uint32_t under_voltage_debounce_ms;
    uint32_t over_voltage_debounce_ms;
    uint32_t over_temp_debounce_ms;
    uint32_t comm_err_debounce_ms;
    uint32_t cell_owc_debounce_ms;
    uint32_t therm_owc_debounce_ms;
};

constexpr FaultThresholds WARNING_THRESHOLDS = {
    .min_voltage               = 2.7f,
    .max_voltage               = 4.17f,
    .max_temp                  = 59.0f,
    .under_voltage_debounce_ms = 1000,
    .over_voltage_debounce_ms  = 1000,
    .over_temp_debounce_ms     = 1000,
    .comm_err_debounce_ms      = 1000,
    .cell_owc_debounce_ms      = 1000,
    .therm_owc_debounce_ms     = 1000,
};

constexpr FaultThresholds FAULT_THRESHOLDS = {
    .min_voltage               = 2.5f,
    .max_voltage               = 4.2f,
    .max_temp                  = 60.0f,
    .under_voltage_debounce_ms = 3000,
    .over_voltage_debounce_ms  = 3000,
    .over_temp_debounce_ms     = 3000,
    .comm_err_debounce_ms      = 3000,
    .cell_owc_debounce_ms      = 3000,
    .therm_owc_debounce_ms     = 0, // does not cause fault
};

struct WarningSetters
{
    static void set_undervoltage(bool value) { app::can_tx::BMS_Warning_CellUndervoltage_set(value); }
    static void set_overvoltage(bool value) { app::can_tx::BMS_Warning_CellOvervoltage_set(value); }
    static void set_overtemp(bool value) { app::can_tx::BMS_Warning_CellOvertemp_set(value); }
    static void set_comm_err(bool value) { app::can_tx::BMS_Warning_ModuleCommunicationError_set(value); }
    static void set_cell_owc(bool value) { app::can_tx::BMS_Warning_CellOpenWire_set(value); }
    static void set_therm_owc(bool value) { app::can_tx::BMS_Warning_ThermistorOpenWire_set(value); }
};

struct FaultSetters
{
    static void set_undervoltage(bool value) { app::can_tx::BMS_Fault_CellUndervoltage_set(value); }
    static void set_overvoltage(bool value) { app::can_tx::BMS_Fault_CellOvervoltage_set(value); }
    static void set_overtemp(bool value) { app::can_tx::BMS_Fault_CellOvertemp_set(value); }
    static void set_comm_err(bool value) { app::can_tx::BMS_Fault_ModuleCommunicationError_set(value); }
    static void set_cell_owc(bool value) { app::can_tx::BMS_Fault_CellOpenWire_set(value); }
    static void set_therm_owc(bool) {}
};

template <typename Setters> class FaultProfile
{
  public:
    explicit FaultProfile(const FaultThresholds &thresholds)
      : thresholds_(thresholds),
        under_voltage_timer_(thresholds.under_voltage_debounce_ms),
        over_voltage_timer_(thresholds.over_voltage_debounce_ms),
        over_temp_timer_(thresholds.over_temp_debounce_ms),
        comm_err_timer_(thresholds.comm_err_debounce_ms),
        cell_owc_timer_(thresholds.cell_owc_debounce_ms),
        therm_owc_timer_(thresholds.therm_owc_debounce_ms)
    {
    }

    bool check(bool blanking_expired)
    {
        const bool  is_charging       = app::StateMachine::get_current_state() == &app::states::charge_state;
        const float max_temp          = is_charging ? MAX_CELL_CHARGE_TEMP_DEGC : thresholds_.max_temp;
        const bool  therm_owc_enabled = thresholds_.therm_owc_debounce_ms > 0;

        bool comm_err      = false;
        bool any_cell_owc  = false;
        bool any_therm_owc = false;

        for (size_t seg = 0; seg < io::NUM_SEGMENTS; ++seg)
        {
            comm_err = comm_err || !app::segments::stat_success[seg];

            for (size_t cell = 0; cell < io::CELLS_PER_SEGMENT; ++cell)
            {
                comm_err = comm_err || !app::segments::cell_voltage_success[seg][cell] ||
                           !app::segments::filtered_cell_voltage_success[seg][cell];
                any_cell_owc = any_cell_owc || !app::segments::cell_owc_ok[seg][cell];
            }

            for (size_t therm = 0; therm < io::THERMISTORS_PER_SEGMENT && therm_owc_enabled; ++therm)
            {
                any_therm_owc = any_therm_owc || !app::segments::therm_owc_ok[seg][therm];
            }
        }

        const bool under_voltage_condition = app::segments::min_cell_voltage.voltage < thresholds_.min_voltage;
        const bool over_voltage_condition  = app::segments::max_cell_voltage.voltage > thresholds_.max_voltage;
        const bool over_temp_condition     = app::segments::max_cell_temp.temp > max_temp;

        const bool uv = under_voltage_timer_.runIfCondition(under_voltage_condition) == Timer::TimerState::EXPIRED &&
                        blanking_expired;
        const bool ov = over_voltage_timer_.runIfCondition(over_voltage_condition) == Timer::TimerState::EXPIRED &&
                        blanking_expired;
        const bool ot =
            over_temp_timer_.runIfCondition(over_temp_condition) == Timer::TimerState::EXPIRED && blanking_expired;
        const bool ce  = comm_err_timer_.runIfCondition(comm_err) == Timer::TimerState::EXPIRED && blanking_expired;
        const bool cow = cell_owc_timer_.runIfCondition(any_cell_owc) == Timer::TimerState::EXPIRED && blanking_expired;
        const bool tow = therm_owc_enabled &&
                         therm_owc_timer_.runIfCondition(any_therm_owc) == Timer::TimerState::EXPIRED &&
                         blanking_expired;

        Setters::set_undervoltage(uv);
        Setters::set_overvoltage(ov);
        Setters::set_overtemp(ot);
        Setters::set_comm_err(ce);
        Setters::set_cell_owc(cow);
        if (therm_owc_enabled)
        {
            Setters::set_therm_owc(tow);
        }

        return uv || ov || ot || ce || cow || tow;
    }

  private:
    const FaultThresholds &thresholds_;
    Timer                  under_voltage_timer_;
    Timer                  over_voltage_timer_;
    Timer                  over_temp_timer_;
    Timer                  comm_err_timer_;
    Timer                  cell_owc_timer_;
    Timer                  therm_owc_timer_;
};

FaultProfile<WarningSetters> warning_profile(WARNING_THRESHOLDS);
FaultProfile<FaultSetters>   fault_profile(FAULT_THRESHOLDS);
} // namespace

namespace app::segments
{
void initFaults()
{
    startup_blanking_timer.restart();

    for (auto &segment_cell_owc : cell_owc_ok)
    {
        segment_cell_owc.fill(true);
    }

    for (auto &segment_therm_owc : therm_owc_ok)
    {
        segment_therm_owc.fill(true);
    }
}

bool checkWarnings()
{
    const bool settled = startup_blanking_timer.updateAndGetState() == Timer::TimerState::EXPIRED;
    return warning_profile.check(settled);
}

bool checkFaults()
{
    const bool settled = startup_blanking_timer.updateAndGetState() == Timer::TimerState::EXPIRED;
    return fault_profile.check(settled);
}
} // namespace app::segments
