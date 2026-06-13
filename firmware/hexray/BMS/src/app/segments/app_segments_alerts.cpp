#include <algorithm>
#include <cassert>
#include <iterator>

#include "app_segments.hpp"
#include "app_segments_internal.hpp"
#include "app_timer.hpp"
#include "app_canAlerts.hpp"
#include "app_stateMachine.hpp"
#include "app_states.hpp"

namespace
{
using app::Timer;
using app::segments::health::ErrorBit;

constexpr float V_WARN_LOW                = 2.7f;
constexpr float V_WARN_HIGH               = 4.17f;
constexpr float T_WARN_HIGH               = 55.0f;
constexpr float V_FAULT_LOW               = app::segments::convertUVOVToFloat(VUV); // 2.5 V
constexpr float V_FAULT_HIGH              = app::segments::convertUVOVToFloat(VOV); // 4.2 V
constexpr float T_FAULT_HIGH              = 60.0f;
constexpr float MAX_CELL_CHARGE_TEMP_DEGC = 45.0f;

constexpr uint32_t STARTUP_BLANKING_MS      = 1000;
constexpr uint32_t WARN_DEBOUNCE_MS         = 1000;
constexpr uint32_t FAULT_V_DEBOUNCE_MS      = 1000;
constexpr uint32_t FAULT_T_DEBOUNCE_MS      = 2000;
constexpr uint32_t FAULT_OWC_DEBOUNCE_MS    = 5000;
constexpr uint32_t FAULT_HEALTH_DEBOUNCE_MS = 2000;
constexpr uint32_t INFO_OWC_DEBOUNCE_MS     = 5000;

float maxTempLimit(const float base_limit)
{
    const bool charging = app::StateMachine::get_current_state() == &app::states::charge_state;
    return charging ? std::min(base_limit, MAX_CELL_CHARGE_TEMP_DEGC) : base_limit;
}

bool minCellUnder(const float threshold)
{
    assert(shared_lock.is_held());
    return app::segments::shared::getMinCellVoltage().value < threshold;
}
bool maxCellOver(const float threshold)
{
    assert(shared_lock.is_held());
    return app::segments::shared::getMaxCellVoltage().value > threshold;
}
bool maxTempOver(const float threshold)
{
    assert(shared_lock.is_held());
    return app::segments::shared::getMaxCellTemperature().value > maxTempLimit(threshold);
}

bool isCellUnderVoltageWarn()
{
    return minCellUnder(V_WARN_LOW);
}
bool isCellOverVoltageWarn()
{
    return maxCellOver(V_WARN_HIGH);
}
bool isCellOverTempWarn()
{
    return maxTempOver(T_WARN_HIGH);
}

bool isCellUndervoltageFault()
{
    return minCellUnder(V_FAULT_LOW);
}
bool isCellOvervoltageFault()
{
    return maxCellOver(V_FAULT_HIGH);
}
bool isCellOvertempFault()
{
    return maxTempOver(T_FAULT_HIGH);
}

bool anyCellOpenWire()
{
    assert(shared_lock.is_held());
    for (const io::adbms::Cells<result<bool>>         cells = app::segments::shared::getLatestCellOwcOk();
         const io::adbms::SegmentCells<result<bool>> &segment : cells)
        for (const auto &cell : segment)
            if (cell.has_value() && !cell.value()) // value()==false => open wire detected
                return true;
    return false;
}

bool anyThermOpenWire()
{
    assert(shared_lock.is_held());
    for (const io::adbms::Therms<result<bool>>         therms = app::segments::shared::getLatestThermOwcOk();
         const io::adbms::SegmentTherms<result<bool>> &segment : therms)
        for (const auto &therm : segment)
            if (therm.has_value() && !therm.value()) // value()==false => open wire detected
                return true;
    return false;
}

bool anyHealthError()
{
    assert(health_lock.is_held());
    for (size_t seg = 0; seg < NUM_SEGMENTS; ++seg)
        if (app::segments::health::getAnyError(seg))
            return true;
    return false;
}

struct Entry
{
    bool (*condition)();
    void (*setter)(bool);
    Timer timer;
};

std::array<Entry, 3> warning_entries = { {
    { &isCellUnderVoltageWarn, &app::can_alerts::warnings::CellUnderVoltage_set, Timer{ WARN_DEBOUNCE_MS } },
    { &isCellOverVoltageWarn, &app::can_alerts::warnings::CellOverVoltage_set, Timer{ WARN_DEBOUNCE_MS } },
    { &isCellOverTempWarn, &app::can_alerts::warnings::CellOverTemp_set, Timer{ WARN_DEBOUNCE_MS } },
} };

std::array<Entry, 5> fault_entries = { {
    { &isCellUndervoltageFault, &app::can_alerts::faults::CellUndervoltage_set, Timer{ FAULT_V_DEBOUNCE_MS } },
    { &isCellOvervoltageFault, &app::can_alerts::faults::CellOvervoltage_set, Timer{ FAULT_V_DEBOUNCE_MS } },
    { &isCellOvertempFault, &app::can_alerts::faults::CellOvertemp_set, Timer{ FAULT_T_DEBOUNCE_MS } },
    { &anyCellOpenWire, &app::can_alerts::faults::CellOpenWire_set, Timer{ FAULT_OWC_DEBOUNCE_MS } },
    { &anyHealthError, &app::can_alerts::faults::HealthError_set, Timer{ FAULT_HEALTH_DEBOUNCE_MS } },
} };

std::array<Entry, 1> info_entries = { {
    { &anyThermOpenWire, &app::can_alerts::infos::ThermOpenWire_set, Timer{ INFO_OWC_DEBOUNCE_MS } },
} };

Timer startup_blanking{ STARTUP_BLANKING_MS };

bool runEntries(const std::span<Entry> entries)
{
    bool any_fired = false;
    for (auto &[cond, setter, timer] : entries)
    {
        const bool fired = timer.runIfCondition(cond()) == Timer::TimerState::EXPIRED;
        setter(fired);
        any_fired = any_fired || fired;
    }
    return any_fired;
}
} // namespace

namespace app::segments::alerts
{
void init()
{
    for (auto &[_c, _s, t] : warning_entries)
        t.stop();
    for (auto &[_c, _s, t] : fault_entries)
        t.stop();
    for (auto &[_c, _s, t] : info_entries)
        t.stop();
    startup_blanking.restart();
}

bool tick()
{
    if (startup_blanking.updateAndGetState() != Timer::TimerState::EXPIRED)
        return false;

    const bool fault_active = runEntries(fault_entries);
    runEntries(warning_entries);
    runEntries(info_entries);

    return fault_active;
}
} // namespace app::segments::alerts
