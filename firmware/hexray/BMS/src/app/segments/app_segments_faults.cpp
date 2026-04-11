#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "app_timer.hpp"
#include "app_canTx.hpp"
#include "app_canRx.hpp"

using namespace app::segments;
using namespace app;

// find correct values
static constexpr float MAX_CELL_VOLTAGE_BALANCING = 4.25f;
static constexpr float MAX_CELL_CHARGE_TEMP_DEGC  = 45.0f;

static constexpr uint32_t           STARTUP_BLANKING_MS = 300;
static Timer                        startup_blanking_timer(STARTUP_BLANKING_MS);
static FaultProfile<WarningSetters> warning_profile(WARNING_THERSHOLDS);
static FaultProfile<FaultSetters>   fault_profile(FAULT_THERSHOLDS);

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
    uint32_t therm_err_debounce_ms;
};

static constexpr FaultThresholds WARNING_THERSHOLDS = {
    .min_voltage               = 2.7f,
    .max_voltage               = 4.17f,
    .max_temp                  = 59.0f,
    .under_voltage_debounce_ms = 1000,
    .over_temp_debounce_ms     = 1000,
    .comm_err_debounce_ms      = 1000,
    .cell_owc_debounce_ms      = 1000,
    .therm_err_debounce_ms     = 1000,
};

static constexpr FaultThresholds FAULT_THERSHOLDS = {
    .min_voltage               = 2.5f,
    .max_voltage               = 4.2f,
    .max_temp                  = 60.0f,
    .under_voltage_debounce_ms = 3000,
    .over_temp_debounce_ms     = 3000,
    .comm_err_debounce_ms      = 3000,
    .cell_owc_debounce_ms      = 3000,
    .therm_err_debounce_ms     = 0, // unused - therm owc is warning only
};

struct WarningSetters
{
    static void set_undervoltage(bool v) { app::can_tx::BMS_Warning_CellUndervoltage_set(v); }
    static void set_overvoltage(bool v) { app::can_tx::BMS_Warning_CellOvervoltage_set(v); }
    static void set_overtemp(bool v) { app::can_tx::BMS_Warning_CellOvertemp_set(v); }
    static void set_comm_err(bool v) { app::can_tx::BMS_Warning_ModuleCommunicationError_set(v); }
    static void set_cell_owc(bool v) { app::can_tx::BMS_Warning_CellOpenWire_set(v); }
    static void set_therm_owc(bool v) { app::can_tx::BMS_Warning_ThermOpenWire_Count_set(v); }
};

struct FaultSetters
{
    static void set_undervoltage(bool v) { app::can_tx::BMS_Fault_CellUndervoltage_set(v); }
    static void set_overvoltage(bool v) { app::can_tx::BMS_Fault_CellOvervoltage_set(v); }
    static void set_overtemp(bool v) { app::can_tx::BMS_Fault_CellOvertemp_set(v); }
    static void set_comm_err(bool v) { app::can_tx::BMS_Fault_ModuleCommunicationError_set(v); }
    static void set_cell_owc(bool v) { app::can_tx::BMS_Fault_CellOpenWire_set(v); }
    static void set_therm_owc(bool v) {} // therm owc doesn't cause a fault
};

template <typename Setters> class FaultProfile
{
    const FaultThresholds &thresholds;
    Timer                  under_voltage_timer;
    Timer                  over_voltage_timer;
    Timer                  over_temp_timer;
    Timer                  comm_err_timer;
    Timer                  cell_owc_timer;
    Timer                  therm_owc_timer;

  public:
    explicit FaultProfile(const FaultThresholds &t)
      : thresholds(t),
        under_voltage_timer(t.under_voltage_debounce_ms),
        over_voltage_timer(t.over_voltage_debounce_ms),
        over_temp_timer(t.over_temp_debounce_ms),
        comm_err_timer(t.comm_err_debounce_ms),
        cell_owc_timer(t.cell_owc_debounce_ms),
        therm_owc_timer(t.therm_owc_debounce_ms)
    {
    }

    bool check(bool blanking_expired)
    {
        const bool  is_charging = app::StateMachine::get_current_state() = &charge_state;
        const float max_temp = is_charging ? MAX_CELL_CHARGE_TEMP_DEGC : thresholds.max_temp;

        const balancing_enabled = app::can_rx::Debug_CellBalancingRequest_get() && !is_charging;
        const float max_voltage = balancing_enabled ? MAX_CELL_VOLTAGE_BALANCING;
        thresholds.max_voltage;

        const bool under_voltage_condition = min_cell_voltage.voltage < thresholds.min_voltage;
        const bool over_voltage_condition  = max_cell_voltage.voltage > max_voltage;
        const bool over_temp_condition     = max_cell_temp.temp > max_temp;

        for (uint8_t seg = 0; seg < io::NUM_SEGMENTS; seg++)
        {
            for ()
        }

        const bool uv = under_voltage_timer.runIfCondition(under_voltage_condition) == Timer::TimerState::EXPIRED &&
                        blanking_expired;
        const bool ov =
            over_voltage_timer.runIfCondition(over_voltage_condition) == Timer::TimerState::EXPIRED && blanking_expired;
        const bool ot =
            over_temp_timer.runIfCondition(over_temp_condition) == Timer::TimerState::EXPIRED && blanking_expired;
        const bool ce  = comm_err_timer.runIfCondition(!comm_ok) == Timer::TimerState::EXPIRED && blanking_expired;
        const bool cow = cell_owc_timer.runIfCondition(any_cell_owc) == Timer::TimerState::EXPIRED && blanking_expired;
        const bool tow =
            therm_owc_timer.runIfCondition(any_therm_owc) == Timer::TimerState::EXPIRED && blanking_expired;

        Setters::set_undervoltage(uv) Setters::set_overvoltage(ov);
        Setters::set_overtemp(ot);
        Setters::set_comm_err(ce);
        Setters::set_cell_owc(cow);
        Setters::set_therm_owc(tow);

        return uv || ov || ot || ce || cow || tow;
    }
};

namespace app::segments
{
void initFaults()
{
    startup_blanking_timer.restart();
}

void checkWarnings()
{
    const bool settled = startup_blanking_timer.updateAndGetState() == Timer::TimerState::EXPIRED;
    return warning_profile.check(settled);
}

void checkFaults()
{
    const bool settled = startup_blanking_timer.updateAndGetState() == Timer::TimerState::EXPIRED;
    return fault_profile.check(settled);
}
} // namespace app::segments