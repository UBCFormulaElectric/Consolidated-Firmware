#include "app_pack.hpp"

#include <algorithm>
#include <array>
#include <cstddef>

#include "app_canAlerts.hpp"
#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_timer.hpp"

namespace
{
using app::Timer;
using app::pack::ADBMS6830Diag;
using app::pack::OwcStats;
using app::pack::TempStats;
using app::pack::VoltStats;

// Fault thresholds
constexpr uint32_t FAULT_V_DEBOUNCE_MS   = 1000;
constexpr uint32_t FAULT_T_DEBOUNCE_MS   = 3000;
constexpr uint32_t FAULT_OWC_DEBOUNCE_MS = 5000;
constexpr float    V_FAULT_UV            = 2.5f;   
constexpr float    V_FAULT_OV            = 4.25f;  
constexpr float    T_FAULT_OT            = 55.0f;  
constexpr float    T_CHARGE_FAULT_OT     = 45.0f;
constexpr float    T_FAULT_UT            = -20.0f; 
constexpr float    T_CHARGE_FAULT_UT     = 0.0f; 

// Warning thresholds
constexpr uint32_t WARN_DEBOUNCE_MS = 1000;
constexpr float    V_WARN_UV        = 2.7f;
constexpr float    V_WARN_OV        = 4.22f;
constexpr float    T_WARN_OT        = 50.0f;
constexpr float    T_CHARGE_WARN_OT = 42.0f;
constexpr float    T_WARN_UT        = -10.0f;
constexpr float    T_CHARGE_WARN_UT = 5.0f;

// Info thresholds
constexpr uint32_t INFO_DEBOUNCE_MS     = 100;
constexpr uint32_t INFO_OWC_DEBOUNCE_MS = 5000;
constexpr float    V_INFO_UV            = 2.8f;
constexpr float    V_INFO_OV            = 4.2f; 
constexpr float    T_INFO_OT            = 45.0f;
constexpr float    T_CHARGE_INFO_OT     = 40.0f;

// Helper functions
template <size_t BITS, size_t SEGMENTS> bool anyClear(const std::array<std::bitset<BITS>, SEGMENTS> &flags)
{
    return std::ranges::any_of(flags, [](const std::bitset<BITS> &segment) { return !segment.all(); });
}

// Fault conditions
bool isVoltInvalidFault(const VoltStats &stats)
{
    return !stats.stamped || anyClear(stats.valid);
}
bool isUnderVoltageFault(const VoltStats &stats)
{
    return stats.stamped && stats.min.value < V_FAULT_UV;
}
bool isOverVoltageFault(const VoltStats &stats)
{
    return stats.stamped && stats.max.value > V_FAULT_OV;
}
bool isTempInvalidFault(const TempStats &stats)
{
    return !stats.stamped || anyClear(stats.valid);
}
bool isOverTempFault(const TempStats &stats)
{
    const bool charging = app::StateMachine::get_current_state() == &app::states::charge_state;
    return stats.stamped && stats.max.value > (charging ? std::min(T_FAULT_OT, T_CHARGE_FAULT_OT) : T_FAULT_OT);
}
bool isUnderTempFault(const TempStats &stats)
{
    const bool charging = app::StateMachine::get_current_state() == &app::states::charge_state;
    return stats.stamped && stats.min.value < (charging ? std::max(T_FAULT_UT, T_CHARGE_FAULT_UT) : T_FAULT_UT);
}
bool isCellOpenWireFault(const OwcStats &stats)
{
    return stats.stamped && anyClear(stats.cells_ok);
}

// Warning conditions
bool isUnderVoltageWarn(const VoltStats &stats)
{
    return stats.stamped && stats.min.value < V_WARN_UV;
}
bool isOverVoltageWarn(const VoltStats &stats)
{
    return stats.stamped && stats.max.value > V_WARN_OV;
}
bool isOverTempWarn(const TempStats &stats)
{
    const bool charging = app::StateMachine::get_current_state() == &app::states::charge_state;
    return stats.stamped && stats.max.value > (charging ? std::min(T_WARN_OT, T_CHARGE_WARN_OT) : T_WARN_OT);
}
bool isUnderTempWarn(const TempStats &stats)
{
    const bool charging = app::StateMachine::get_current_state() == &app::states::charge_state;
    return stats.stamped && stats.min.value < (charging ? std::max(T_WARN_UT, T_CHARGE_WARN_UT) : T_WARN_UT);
}

// Info conditions
bool isUnderVoltageInfo(const VoltStats &stats)
{
    return stats.stamped && stats.min.value < V_INFO_UV;
}
bool isOverVoltageInfo(const VoltStats &stats)
{
    return stats.stamped && stats.max.value > V_INFO_OV;
}
bool isOverTempInfo(const TempStats &stats)
{
    const bool charging = app::StateMachine::get_current_state() == &app::states::charge_state;
    return stats.stamped && stats.max.value > (charging ? std::min(T_INFO_OT, T_CHARGE_INFO_OT) : T_INFO_OT);
}
bool isThermOpenWireInfo(const OwcStats &stats)
{
    return stats.stamped && anyClear(stats.therms_ok);
}

template <typename T> struct Entry
{
    bool (*condition)(const T &);
    void (*setter)(bool);
    Timer timer;
};

std::array<Entry<VoltStats>, 3> volt_fault_entries = { {
    { &isVoltInvalidFault, &app::can_alerts::faults::CellVoltageInvalid_set, Timer{ FAULT_V_DEBOUNCE_MS } },
    { &isUnderVoltageFault, &app::can_alerts::faults::CellUndervoltage_set, Timer{ FAULT_V_DEBOUNCE_MS } },
    { &isOverVoltageFault, &app::can_alerts::faults::CellOvervoltage_set, Timer{ FAULT_V_DEBOUNCE_MS } },
} };

std::array<Entry<VoltStats>, 2> volt_warning_entries = { {
    { &isUnderVoltageWarn, &app::can_alerts::warnings::CellUnderVoltage_set, Timer{ WARN_DEBOUNCE_MS } },
    { &isOverVoltageWarn, &app::can_alerts::warnings::CellOverVoltage_set, Timer{ WARN_DEBOUNCE_MS } },
} };

std::array<Entry<VoltStats>, 2> volt_info_entries = { {
    { &isUnderVoltageInfo, &app::can_alerts::infos::CellUnderVoltage_set, Timer{ INFO_DEBOUNCE_MS } },
    { &isOverVoltageInfo, &app::can_alerts::infos::CellOverVoltage_set, Timer{ INFO_DEBOUNCE_MS } },
} };

std::array<Entry<TempStats>, 3> temp_fault_entries = { {
    { &isTempInvalidFault, &app::can_alerts::faults::CellTemperatureInvalid_set, Timer{ FAULT_T_DEBOUNCE_MS } },
    { &isOverTempFault, &app::can_alerts::faults::CellOvertemp_set, Timer{ FAULT_T_DEBOUNCE_MS } },
    { &isUnderTempFault, &app::can_alerts::faults::CellUndertemp_set, Timer{ FAULT_T_DEBOUNCE_MS } },
} };

std::array<Entry<TempStats>, 2> temp_warning_entries = { {
    { &isOverTempWarn, &app::can_alerts::warnings::CellOverTemp_set, Timer{ WARN_DEBOUNCE_MS } },
    { &isUnderTempWarn, &app::can_alerts::warnings::CellUnderTemp_set, Timer{ WARN_DEBOUNCE_MS } },
} };

std::array<Entry<TempStats>, 1> temp_info_entries = { {
    { &isOverTempInfo, &app::can_alerts::infos::CellOverTemp_set, Timer{ INFO_DEBOUNCE_MS } },
} };

std::array<Entry<OwcStats>, 1> owc_fault_entries = { {
    { &isCellOpenWireFault, &app::can_alerts::faults::CellOpenWire_set, Timer{ FAULT_OWC_DEBOUNCE_MS } },
} };

std::array<Entry<OwcStats>, 1> owc_info_entries = { {
    { &isThermOpenWireInfo, &app::can_alerts::infos::ThermOpenWire_set, Timer{ INFO_OWC_DEBOUNCE_MS } },
} };

template <typename T, size_t N> bool runEntries(std::array<Entry<T>, N> &entries, const T &stats)
{
    bool any_fired = false;
    for (auto &[condition, setter, timer] : entries)
    {
        const bool fired = timer.runIfCondition(condition(stats)) == Timer::TimerState::EXPIRED;
        setter(fired);
        any_fired = any_fired || fired;
    }
    return any_fired;
}

template <typename T, size_t N> void stopEntries(std::array<Entry<T>, N> &entries)
{
    for (auto &[_c, _s, timer] : entries)
        timer.stop();
}
} // namespace

namespace app::pack::alerts
{
void init()
{
    stopEntries(volt_fault_entries);
    stopEntries(volt_warning_entries);
    stopEntries(volt_info_entries);
    stopEntries(temp_fault_entries);
    stopEntries(temp_warning_entries);
    stopEntries(temp_info_entries);
    stopEntries(owc_fault_entries);
    stopEntries(owc_info_entries);
}

bool voltFault(const VoltStats &stats)
{
    const bool fault_active = runEntries(volt_fault_entries, stats);
    runEntries(volt_warning_entries, stats);
    runEntries(volt_info_entries, stats);
    return fault_active;
}

bool tempFault(const TempStats &stats)
{
    const bool fault_active = runEntries(temp_fault_entries, stats);
    runEntries(temp_warning_entries, stats);
    runEntries(temp_info_entries, stats);
    return fault_active;
}

bool owcFault(const OwcStats &stats)
{
    const bool fault_active = runEntries(owc_fault_entries, stats);
    runEntries(owc_info_entries, stats);
    return fault_active;
}
} // namespace app::pack::alerts
