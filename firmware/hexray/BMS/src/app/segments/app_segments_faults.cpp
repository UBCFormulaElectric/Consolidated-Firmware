#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "app_timer.hpp"
#include "app_canTx.hpp"
#include "app_stateMachine.hpp"
#include "app_states.hpp"

using namespace app;

namespace
{
constexpr float MAX_CELL_CHARGE_TEMP_DEGC = 45.0f;

constexpr uint32_t STARTUP_BLANKING_MS = 1000;
Timer              startup_blanking_timer(STARTUP_BLANKING_MS);

struct Signal
{
    void (*set)(bool);
    bool     reportable;
    uint32_t debounce_ms;
};

struct Profile
{
    float  min_voltage;
    float  max_voltage;
    float  max_temp;
    Signal undervoltage;
    Signal overvoltage;
    Signal overtemp;
    Signal comm_err;
    Signal cell_owc;
    Signal therm_owc;
};

constexpr Profile WARNING = {
    .min_voltage  = 2.7f,
    .max_voltage  = 4.17f,
    .max_temp     = 55.0f,
    .undervoltage = { &app::can_tx::BMS_Warning_CellUndervoltage_set, true, 1000 },
    .overvoltage  = { &app::can_tx::BMS_Warning_CellOvervoltage_set, true, 1000 },
    .overtemp     = { &app::can_tx::BMS_Warning_CellOvertemp_set, true, 1000 },
    .comm_err     = { nullptr, false, 0 },
    .cell_owc     = { nullptr, false, 0 },
    .therm_owc    = { &app::can_tx::BMS_Warning_ThermOpenWire_set, true, 1000 },
};

constexpr Profile FAULT = {
    .min_voltage  = app::segments::convertUVOVToFloat(VUV),
    .max_voltage  = app::segments::convertUVOVToFloat(VOV),
    .max_temp     = 60.0f,
    .undervoltage = { &app::can_tx::BMS_Fault_CellUndervoltage_set, true, 5000 },
    .overvoltage  = { &app::can_tx::BMS_Fault_CellOvervoltage_set, true, 5000 },
    .overtemp     = { &app::can_tx::BMS_Fault_CellOvertemp_set, true, 5000 },
    .comm_err     = { &app::can_tx::BMS_Fault_ModuleCommunicationError_set, true, 5000 },
    .cell_owc     = { &app::can_tx::BMS_Fault_CellOpenWire_set, true, 5000 },
    .therm_owc    = { nullptr, false, 0 },
};

class Checker
{
  public:
    explicit Checker(const Profile &profile)
      : profile_(profile),
        uv_timer_(profile.undervoltage.debounce_ms),
        ov_timer_(profile.overvoltage.debounce_ms),
        ot_timer_(profile.overtemp.debounce_ms),
        comm_timer_(profile.comm_err.debounce_ms),
        cell_owc_timer_(profile.cell_owc.debounce_ms),
        therm_owc_timer_(profile.therm_owc.debounce_ms)
    {
    }

    bool check(bool blanking_expired)
    {
        if (!blanking_expired)
        {
            clear(profile_.undervoltage);
            clear(profile_.overvoltage);
            clear(profile_.overtemp);
            clear(profile_.comm_err);
            clear(profile_.cell_owc);
            clear(profile_.therm_owc);
            return false;
        }

        const bool  is_charging = app::StateMachine::get_current_state() == &app::states::charge_state;
        const float max_temp = is_charging ? std::min(profile_.max_temp, MAX_CELL_CHARGE_TEMP_DEGC) : profile_.max_temp;

        bool any = false;
        any |= evaluate(
            profile_.undervoltage, uv_timer_, app::segments::state::getMinCellVoltage().value < profile_.min_voltage);
        any |= evaluate(
            profile_.overvoltage, ov_timer_, app::segments::state::getMaxCellVoltage().value > profile_.max_voltage);
        any |= evaluate(profile_.overtemp, ot_timer_, app::segments::state::getMaxCellTemperature().value > max_temp);
        any |= evaluate(profile_.comm_err, comm_timer_, !app::segments::state::allOk());
        any |= evaluate(profile_.cell_owc, cell_owc_timer_, app::segments::state::getCellOwc());
        any |= evaluate(profile_.therm_owc, therm_owc_timer_, app::segments::state::getThermOwc());
        return any;
    }

  private:
    static void clear(const Signal &s)
    {
        if (s.set)
            s.set(false);
    }

    static bool evaluate(const Signal &s, Timer &timer, bool cond)
    {
        const bool fired = timer.runIfCondition(cond) == Timer::TimerState::EXPIRED;
        if (s.set)
            s.set(fired);
        return s.reportable && fired;
    }

    const Profile &profile_;
    Timer          uv_timer_;
    Timer          ov_timer_;
    Timer          ot_timer_;
    Timer          comm_timer_;
    Timer          cell_owc_timer_;
    Timer          therm_owc_timer_;
};

Checker warning_checker(WARNING);
Checker fault_checker(FAULT);
} // namespace

namespace app::segments::faults
{

bool checkWarnings()
{
    const bool settled = startup_blanking_timer.updateAndGetState() == Timer::TimerState::EXPIRED;
    return warning_checker.check(settled);
}

bool checkFaults()
{
    const bool settled = startup_blanking_timer.updateAndGetState() == Timer::TimerState::EXPIRED;
    return fault_checker.check(settled);
}
} // namespace app::segments::faults
