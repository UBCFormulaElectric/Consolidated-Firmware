#include "app_pack.hpp"
#include "app_pack_internal.hpp"

#include <algorithm>
#include <array>
#include <cstddef>

#include "app_canAlerts.hpp"
#include "app_stateMachine.hpp"
#include "app_states.hpp"
#include "app_timer.hpp"
#include "io_semaphore.hpp"
#include "io_time.hpp"

namespace
{
using app::Timer;

// Fault thresholds
constexpr uint32_t FAULT_V_DEBOUNCE_MS      = 1000;
constexpr uint32_t FAULT_T_DEBOUNCE_MS      = 3000;
constexpr uint32_t FAULT_OWC_DEBOUNCE_MS    = 5000;
constexpr uint32_t FAULT_HEALTH_DEBOUNCE_MS = 3000;
constexpr float    T_FAULT_OT               = 55.0f;
constexpr float    T_CHARGE_FAULT_OT        = 45.0f;
constexpr float    T_FAULT_UT               = -20.0f;
constexpr float    T_CHARGE_FAULT_UT        = 0.0f;

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

// Voltages are read every ADBMS tick (50 ms), everything else once per diagnostic cycle (~1 s)
constexpr uint32_t V_STALE_MS    = 500;
constexpr uint32_t DIAG_STALE_MS = 3000;

// Everything the alerts need, copied out of the shared buffers once per tick
struct Snapshot
{
    uint32_t                       now_ms   = 0U;
    bool                           charging = false;
    app::pack::VoltageBuffer       voltages{};
    app::pack::TemperatureBuffer   temps{};
    app::pack::OpenWireCheckBuffer owc{};
    app::pack::DiagnosticBuffer    diag{};
};

// Helper functions
template <size_t BITS, size_t SEGMENTS> bool anyClear(const std::array<std::bitset<BITS>, SEGMENTS> &flags)
{
    return std::ranges::any_of(flags, [](const std::bitset<BITS> &segment) { return !segment.all(); });
}
template <size_t N> bool anyInvalid(const io::adbms::Segments<std::array<result<float>, N>> &readings)
{
    return std::ranges::any_of(
        readings,
        [](const std::array<result<float>, N> &segment)
        { return std::ranges::any_of(segment, [](const result<float> &reading) { return !reading; }); });
}
// Never updated, or the ADBMS stopped updating it (comms loss, sequencer stuck retrying a state)
bool isStale(const uint32_t updated_ms, const uint32_t stale_ms, const uint32_t now_ms)
{
    return updated_ms == 0U || now_ms - updated_ms > stale_ms;
}

// Fault conditions
bool isVoltInvalidFault(const Snapshot &s)
{
    return isStale(s.voltages.updated_ms, V_STALE_MS, s.now_ms) || anyInvalid(s.voltages.voltages);
}
bool isUnderVoltageFault(const Snapshot &s)
{
    return s.voltages.updated_ms != 0U && s.voltages.min.value < V_FAULT_UV;
}
bool isOverVoltageFault(const Snapshot &s)
{
    return s.voltages.updated_ms != 0U && s.voltages.max.value > V_FAULT_OV;
}
bool isTempInvalidFault(const Snapshot &s)
{
    return isStale(s.temps.updated_ms, DIAG_STALE_MS, s.now_ms) || anyInvalid(s.temps.temperatures);
}
bool isOverTempFault(const Snapshot &s)
{
    return s.temps.updated_ms != 0U &&
           s.temps.max.value > (s.charging ? std::min(T_FAULT_OT, T_CHARGE_FAULT_OT) : T_FAULT_OT);
}
bool isUnderTempFault(const Snapshot &s)
{
    return s.temps.updated_ms != 0U &&
           s.temps.min.value < (s.charging ? std::max(T_FAULT_UT, T_CHARGE_FAULT_UT) : T_FAULT_UT);
}
bool isCellOpenWireFault(const Snapshot &s)
{
    return s.owc.updated_ms != 0U && anyClear(s.owc.cells_owc_ok);
}
// ADBMS hardware OV/UV comparators, thermal shutdown, self test and supply, plus stale diagnostics/open wire results
bool isHealthFault(const Snapshot &s)
{
    return isStale(s.diag.updated_ms, DIAG_STALE_MS, s.now_ms) || isStale(s.owc.updated_ms, DIAG_STALE_MS, s.now_ms) ||
           anyClear(s.diag.ov_ok) || anyClear(s.diag.uv_ok) || !s.diag.therm_shdn_ok.all() ||
           !s.diag.self_test_ok.all() || !s.diag.supply_ok.all();
}

// Warning conditions
bool isUnderVoltageWarn(const Snapshot &s)
{
    return s.voltages.updated_ms != 0U && s.voltages.min.value < V_WARN_UV;
}
bool isOverVoltageWarn(const Snapshot &s)
{
    return s.voltages.updated_ms != 0U && s.voltages.max.value > V_WARN_OV;
}
bool isOverTempWarn(const Snapshot &s)
{
    return s.temps.updated_ms != 0U &&
           s.temps.max.value > (s.charging ? std::min(T_WARN_OT, T_CHARGE_WARN_OT) : T_WARN_OT);
}
bool isUnderTempWarn(const Snapshot &s)
{
    return s.temps.updated_ms != 0U &&
           s.temps.min.value < (s.charging ? std::max(T_WARN_UT, T_CHARGE_WARN_UT) : T_WARN_UT);
}

// Info conditions
bool isUnderVoltageInfo(const Snapshot &s)
{
    return s.voltages.updated_ms != 0U && s.voltages.min.value < V_INFO_UV;
}
bool isOverVoltageInfo(const Snapshot &s)
{
    return s.voltages.updated_ms != 0U && s.voltages.max.value > V_INFO_OV;
}
bool isOverTempInfo(const Snapshot &s)
{
    return s.temps.updated_ms != 0U &&
           s.temps.max.value > (s.charging ? std::min(T_INFO_OT, T_CHARGE_INFO_OT) : T_INFO_OT);
}
bool isThermOpenWireInfo(const Snapshot &s)
{
    return s.owc.updated_ms != 0U && anyClear(s.owc.therms_owc_ok);
}

struct Entry
{
    bool (*condition)(const Snapshot &);
    void (*setter)(bool);
    Timer timer;
};

std::array<Entry, 8> fault_entries = { {
    { &isVoltInvalidFault, &app::can_alerts::faults::CellVoltageInvalid_set, Timer{ FAULT_V_DEBOUNCE_MS } },
    { &isUnderVoltageFault, &app::can_alerts::faults::CellUndervoltage_set, Timer{ FAULT_V_DEBOUNCE_MS } },
    { &isOverVoltageFault, &app::can_alerts::faults::CellOvervoltage_set, Timer{ FAULT_V_DEBOUNCE_MS } },
    { &isTempInvalidFault, &app::can_alerts::faults::CellTemperatureInvalid_set, Timer{ FAULT_T_DEBOUNCE_MS } },
    { &isOverTempFault, &app::can_alerts::faults::CellOvertemp_set, Timer{ FAULT_T_DEBOUNCE_MS } },
    { &isUnderTempFault, &app::can_alerts::faults::CellUndertemp_set, Timer{ FAULT_T_DEBOUNCE_MS } },
    { &isCellOpenWireFault, &app::can_alerts::faults::CellOpenWire_set, Timer{ FAULT_OWC_DEBOUNCE_MS } },
    { &isHealthFault, &app::can_alerts::faults::HealthError_set, Timer{ FAULT_HEALTH_DEBOUNCE_MS } },
} };

std::array<Entry, 4> warning_entries = { {
    { &isUnderVoltageWarn, &app::can_alerts::warnings::CellUnderVoltage_set, Timer{ WARN_DEBOUNCE_MS } },
    { &isOverVoltageWarn, &app::can_alerts::warnings::CellOverVoltage_set, Timer{ WARN_DEBOUNCE_MS } },
    { &isOverTempWarn, &app::can_alerts::warnings::CellOverTemp_set, Timer{ WARN_DEBOUNCE_MS } },
    { &isUnderTempWarn, &app::can_alerts::warnings::CellUnderTemp_set, Timer{ WARN_DEBOUNCE_MS } },
} };

std::array<Entry, 4> info_entries = { {
    { &isUnderVoltageInfo, &app::can_alerts::infos::CellUnderVoltage_set, Timer{ INFO_DEBOUNCE_MS } },
    { &isOverVoltageInfo, &app::can_alerts::infos::CellOverVoltage_set, Timer{ INFO_DEBOUNCE_MS } },
    { &isOverTempInfo, &app::can_alerts::infos::CellOverTemp_set, Timer{ INFO_DEBOUNCE_MS } },
    { &isThermOpenWireInfo, &app::can_alerts::infos::ThermOpenWire_set, Timer{ INFO_OWC_DEBOUNCE_MS } },
} };

template <size_t N> bool runEntries(std::array<Entry, N> &entries, const Snapshot &s)
{
    bool any_fired = false;
    for (auto &[condition, setter, timer] : entries)
    {
        const bool fired = timer.runIfCondition(condition(s)) == Timer::TimerState::EXPIRED;
        setter(fired);
        any_fired = any_fired || fired;
    }
    return any_fired;
}

template <size_t N> void stopEntries(std::array<Entry, N> &entries)
{
    for (auto &[_c, _s, timer] : entries)
        timer.stop();
}
} // namespace

namespace app::pack::alerts
{
void init()
{
    stopEntries(fault_entries);
    stopEntries(warning_entries);
    stopEntries(info_entries);
}

bool tick()
{
    Snapshot s{};
    {
        const io::semaphore::unique_semaphore lock{ buffer_lock };
        s.voltages = voltage_buffer;
        s.temps    = temp_buffer;
        s.owc      = owc_buffer;
        s.diag     = diag_buffer;
        // Read after the copy so it's never older than an updated_ms, which would underflow isStale
        s.now_ms = io::time::getCurrentMs();
    }
    s.charging = app::StateMachine::get_current_state() == &app::states::charge_state;

    const bool fault_active = runEntries(fault_entries, s);
    runEntries(warning_entries, s);
    runEntries(info_entries, s);
    return fault_active;
}
} // namespace app::pack::alerts
