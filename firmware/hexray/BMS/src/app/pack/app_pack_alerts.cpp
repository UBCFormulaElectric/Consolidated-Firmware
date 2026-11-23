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
constexpr uint32_t FAULT_V_DEBOUNCE_MS      = 1000U;
constexpr uint32_t FAULT_T_DEBOUNCE_MS      = 3000U;
constexpr uint32_t FAULT_OWC_DEBOUNCE_MS    = 5000U;
constexpr uint32_t FAULT_HEALTH_DEBOUNCE_MS = 3000U;
constexpr float    T_FAULT_OT               = 55.0f;
constexpr float    T_CHARGE_FAULT_OT        = 45.0f;
constexpr float    T_FAULT_ITMP_OT          = 105.0f;

// Warning thresholds
constexpr uint32_t WARN_DEBOUNCE_MS = 1000U;
constexpr float    V_WARN_UV        = 2.7f;
constexpr float    V_WARN_OV        = 4.18f;
constexpr float    T_WARN_OT        = 50.0f;
constexpr float    T_CHARGE_WARN_OT = 42.0f;
constexpr float    T_WARN_UT        = -10.0f;
constexpr float    T_CHARGE_WARN_UT = 5.0f;
constexpr float    T_WARN_ITMP_OT   = 95.0f;

// Info thresholds
constexpr uint32_t INFO_DEBOUNCE_MS           = 100U;
constexpr uint32_t INFO_OWC_DEBOUNCE_MS       = 5000U;
constexpr uint32_t INFO_IMBALANCE_DEBOUNCE_MS = 5000U;
constexpr uint32_t INFO_T_INVALID_DEBOUNCE_MS = 3000U;
constexpr float    V_INFO_UV                  = 2.8f;
constexpr float    V_INFO_OV                  = 4.15f;
constexpr float    V_INFO_IMBALANCE           = 0.1f;
constexpr float    T_INFO_OT                  = 45.0f;
constexpr float    T_CHARGE_INFO_OT           = 40.0f;

constexpr uint32_t V_STALE_MS    = 500;
constexpr uint32_t DIAG_STALE_MS = 3000;
constexpr uint32_t OWC_STALE_MS  = BALANCE_PERIOD_MS + SETTLE_MS + std::max(COWC_PERIOD_MS, SOWC_PERIOD_MS);

template <size_t BITS, size_t SEGMENTS> bool anyNotOk(const std::array<std::bitset<BITS>, SEGMENTS> &flags)
{
    return std::ranges::any_of(flags, [](const std::bitset<BITS> &segment) { return !segment.all(); });
}
template <size_t N> bool anyInvalid(const io::adbms::Segments<std::array<result<float>, N>> &readings)
{
    return std::ranges::any_of(
        readings, [](const std::array<result<float>, N> &segment)
        { return std::ranges::any_of(segment, [](const result<float> &reading) { return !reading; }); });
}

struct AlertData
{
    uint32_t                            now_ms   = 0U;    // Reference time to determine stale data
    bool                                charging = false; // Selects tighter charge temp thresholds
    io::adbms::VoltageBuffer            voltages{};       // Cell UV/OV, imbalance, invalid/stale voltage
    io::adbms::TemperatureBuffer        temps{};          // Cell OT/UT, invalid temp
    app::pack::OpenWireCheckBuffer      cadc_owc{};       // Cell open wire for CADC, if stale leads to stale fault
    app::pack::OpenWireCheckBuffer      sadc_owc{};       // Cell open wire for SADC, if stale leads to stale fault
    app::pack::ThermOpenWireCheckBuffer therm_owc{};      // Thermistor open wire, if stale leads to stale fault
    io::adbms::DiagnosticBuffer         diag{};   // Diag fault: OV/UV comparators, thermal shdn, self test, supply
    io::adbms::SupplyBuffer             supply{}; // ADBMS die OT

    // ADBMS stops updating data
    bool isStale(const uint32_t updated_ms, const uint32_t stale_ms) const
    {
        return updated_ms == 0U || now_ms - updated_ms > stale_ms;
    }

    // Fault conditions
    bool isVoltInvalidStaleFault() const
    {
        return isStale(voltages.updated_ms, V_STALE_MS) || anyInvalid(voltages.voltages);
    }
    bool isUnderVoltageFault() const { return voltages.updated_ms != 0U && voltages.min.value < V_FAULT_UV; }
    bool isOverVoltageFault() const { return voltages.updated_ms != 0U && voltages.max.value > V_FAULT_OV; }
    bool isOverTempFault() const
    {
        return temps.updated_ms != 0U &&
               temps.max.value > (charging ? std::min(T_FAULT_OT, T_CHARGE_FAULT_OT) : T_FAULT_OT);
    }
    // Both C-ADC and S-ADC open wire checks must pass
    bool isCellOpenWireFault() const
    {
        return (cadc_owc.updated_ms != 0U && anyNotOk(cadc_owc.cells_owc_ok)) ||
               (sadc_owc.updated_ms != 0U && anyNotOk(sadc_owc.cells_owc_ok));
    }
    // OV/UV comparators, thermal shutdown, self test, supply
    bool isDiagFault() const
    {
        return anyNotOk(diag.ov_ok) || anyNotOk(diag.uv_ok) || !diag.therm_shdn_ok.all() || !diag.self_test_ok.all() ||
               !diag.supply_ok.all();
    }
    // Diagnostic or open wire results stopped updating
    bool isStaleFault() const
    {
        return isStale(diag.updated_ms, DIAG_STALE_MS) || isStale(cadc_owc.updated_ms, OWC_STALE_MS) ||
               isStale(sadc_owc.updated_ms, OWC_STALE_MS) || isStale(therm_owc.updated_ms, DIAG_STALE_MS);
    }
    bool isItmpOverTempFault() const
    {
        return supply.updated_ms != 0U &&
               std::ranges::any_of(
                   supply.itmp, [](const result<float> &itmp) { return itmp && *itmp > T_FAULT_ITMP_OT; });
    }

    // Warning conditions
    bool isUnderVoltageWarn() const { return voltages.updated_ms != 0U && voltages.min.value < V_WARN_UV; }
    bool isOverVoltageWarn() const { return voltages.updated_ms != 0U && voltages.max.value > V_WARN_OV; }
    bool isOverTempWarn() const
    {
        return temps.updated_ms != 0U &&
               temps.max.value > (charging ? std::min(T_WARN_OT, T_CHARGE_WARN_OT) : T_WARN_OT);
    }
    bool isUnderTempWarn() const
    {
        return temps.updated_ms != 0U &&
               temps.min.value < (charging ? std::max(T_WARN_UT, T_CHARGE_WARN_UT) : T_WARN_UT);
    }
    bool isItmpOverTempWarn() const
    {
        return supply.updated_ms != 0U &&
               std::ranges::any_of(
                   supply.itmp, [](const result<float> &itmp) { return itmp && *itmp > T_WARN_ITMP_OT; });
    }

    // Info conditions
    bool isUnderVoltageInfo() const { return voltages.updated_ms != 0U && voltages.min.value < V_INFO_UV; }
    bool isOverVoltageInfo() const { return voltages.updated_ms != 0U && voltages.max.value > V_INFO_OV; }
    bool isOverTempInfo() const
    {
        return temps.updated_ms != 0U &&
               temps.max.value > (charging ? std::min(T_INFO_OT, T_CHARGE_INFO_OT) : T_INFO_OT);
    }
    bool isThermOpenWireInfo() const { return therm_owc.updated_ms != 0U && anyNotOk(therm_owc.therms_owc_ok); }
    bool isTempInvalidInfo() const { return temps.updated_ms != 0U && anyInvalid(temps.temperatures); }
    bool isCellImbalanceInfo() const
    {
        return voltages.updated_ms != 0U && voltages.max.value - voltages.min.value > V_INFO_IMBALANCE;
    }
};

AlertData alert_data;

struct Alert
{
    bool (AlertData::*condition)() const;
    void (*setter)(bool);
    Timer timer;
};

std::array<Alert, 8> fault_alerts = { {
    { &AlertData::isVoltInvalidStaleFault, &app::can_alerts::faults::CellVoltageInvalidStale_set,
      Timer{ FAULT_V_DEBOUNCE_MS } },
    { &AlertData::isUnderVoltageFault, &app::can_alerts::faults::CellUndervoltage_set, Timer{ FAULT_V_DEBOUNCE_MS } },
    { &AlertData::isOverVoltageFault, &app::can_alerts::faults::CellOvervoltage_set, Timer{ FAULT_V_DEBOUNCE_MS } },
    { &AlertData::isOverTempFault, &app::can_alerts::faults::CellOvertemp_set, Timer{ FAULT_T_DEBOUNCE_MS } },
    { &AlertData::isCellOpenWireFault, &app::can_alerts::faults::CellOpenWire_set, Timer{ FAULT_OWC_DEBOUNCE_MS } },
    { &AlertData::isDiagFault, &app::can_alerts::faults::DiagnosticError_set, Timer{ FAULT_HEALTH_DEBOUNCE_MS } },
    { &AlertData::isStaleFault, &app::can_alerts::faults::StaleData_set, Timer{ FAULT_HEALTH_DEBOUNCE_MS } },
    { &AlertData::isItmpOverTempFault, &app::can_alerts::faults::ItmpOvertemp_set, Timer{ FAULT_T_DEBOUNCE_MS } },
} };

std::array<Alert, 5> warning_alerts = { {
    { &AlertData::isUnderVoltageWarn, &app::can_alerts::warnings::CellUnderVoltage_set, Timer{ WARN_DEBOUNCE_MS } },
    { &AlertData::isOverVoltageWarn, &app::can_alerts::warnings::CellOverVoltage_set, Timer{ WARN_DEBOUNCE_MS } },
    { &AlertData::isOverTempWarn, &app::can_alerts::warnings::CellOverTemp_set, Timer{ WARN_DEBOUNCE_MS } },
    { &AlertData::isUnderTempWarn, &app::can_alerts::warnings::CellUnderTemp_set, Timer{ WARN_DEBOUNCE_MS } },
    { &AlertData::isItmpOverTempWarn, &app::can_alerts::warnings::ItmpOverTemp_set, Timer{ WARN_DEBOUNCE_MS } },
} };

std::array<Alert, 6> info_alerts = { {
    { &AlertData::isUnderVoltageInfo, &app::can_alerts::infos::CellUnderVoltage_set, Timer{ INFO_DEBOUNCE_MS } },
    { &AlertData::isOverVoltageInfo, &app::can_alerts::infos::CellOverVoltage_set, Timer{ INFO_DEBOUNCE_MS } },
    { &AlertData::isOverTempInfo, &app::can_alerts::infos::CellOverTemp_set, Timer{ INFO_DEBOUNCE_MS } },
    { &AlertData::isThermOpenWireInfo, &app::can_alerts::infos::ThermOpenWire_set, Timer{ INFO_OWC_DEBOUNCE_MS } },
    { &AlertData::isTempInvalidInfo, &app::can_alerts::infos::CellTempInvalid_set,
      Timer{ INFO_T_INVALID_DEBOUNCE_MS } },
    { &AlertData::isCellImbalanceInfo, &app::can_alerts::infos::CellImbalance_set,
      Timer{ INFO_IMBALANCE_DEBOUNCE_MS } },
} };

template <size_t N> bool checkAlerts(std::array<Alert, N> &alerts)
{
    bool any_fired = false;
    for (auto &[condition, setter, timer] : alerts)
    {
        const bool fired = timer.runIfCondition((alert_data.*condition)()) == Timer::TimerState::EXPIRED;
        setter(fired);
        any_fired = any_fired || fired;
    }
    return any_fired;
}

template <size_t N> void stopAlerts(std::array<Alert, N> &alerts)
{
    for (auto &[_c, _s, timer] : alerts)
        timer.stop();
}
} // namespace

namespace app::pack::alerts
{
void init()
{
    stopAlerts(fault_alerts);
    stopAlerts(warning_alerts);
    stopAlerts(info_alerts);
}

bool tick()
{
    {
        const io::semaphore::unique_semaphore lock{ buffer_lock };
        alert_data.voltages  = voltage_buffer;
        alert_data.temps     = temp_buffer;
        alert_data.cadc_owc  = cadc_owc_buffer;
        alert_data.sadc_owc  = sadc_owc_buffer;
        alert_data.therm_owc = therm_owc_buffer;
        alert_data.diag      = diag_buffer;
        alert_data.supply    = supply_buffer;
    }
    alert_data.now_ms   = io::time::getCurrentMs();
    alert_data.charging = app::StateMachine::get_current_state() == &app::states::charge_state;

    const bool fault_active = checkAlerts(fault_alerts);
    checkAlerts(warning_alerts);
    checkAlerts(info_alerts);
    return fault_active;
}
} // namespace app::pack::alerts
