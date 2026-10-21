#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "app_fsm.hpp"
#include "app_timer.hpp"

// only problem with this design is that when balancing, state machine goes between balance and diagnostic
// so there is never a time where cadc does a redundant check with sadc
// not sure how important this is

namespace
{
constexpr uint32_t DIAG_PERIOD_MS = 1000;

app::pack::Snapshot local{};
app::Timer          diag_timer{ DIAG_PERIOD_MS };
bool                sadc_done = false;
bool                xadc_done = false;

result<void> measureOnEntry();
void         measureOnTick();
result<void> balanceOnEntry();
void         balanceOnTick();
result<void> balanceOnExit();
result<void> diagnosticOddOnEntry();
void         diagnosticOddOnTick();
result<void> diagnosticEvenOnEntry();
void         diagnosticEvenOnTick();
result<void> diagnosticOnExit();

const app::Fsm::State measure_state{
    "CONTINUOUS REDUNDANT PRIMARY VOLTAGE MEASUREMENTS; CONTINUOUS SECONDARY VOLTAGE MEASUREMENTS", measureOnEntry,
    measureOnTick, nullptr
};
const app::Fsm::State balance_state{ "CONTINUOUS PRIMARY VOLTAGE MEASUREMENTS; BALANCE", balanceOnEntry, balanceOnTick,
                                     balanceOnExit };
const app::Fsm::State diagnostic_odd_state{ "CONTINUOUS PRIMARY VOLTAGE MEASUREMENTS; ODD TEMP MEASUREMENTS; ODD CELL "
                                            "OPEN WIRE CHECK; ODD THERM OPEN WIRE CHECK; DIAGNOSTICS",
                                            diagnosticOddOnEntry, diagnosticOddOnTick, nullptr };
const app::Fsm::State diagnostic_even_state{ "CONTINUOUS PRIMARY VOLTAGE MEASUREMENTS; EVEN TEMP MEASUREMENTS; EVEN "
                                             "CELL OPEN WIRE CHECK; EVEN THERM OPEN WIRE CHECK; DIAGNOSTICS",
                                             diagnosticEvenOnEntry, diagnosticEvenOnTick, diagnosticOnExit };

app::Fsm fsm;

result<void> measureOnEntry()
{
    RETURN_IF_ERR(io::adbms::command::startCadc(true));                                  // RD on
    RETURN_IF_ERR(io::adbms::command::startSadc(true, io::adbms::OpenWireParity::NONE)); // CONT on
    return {};
}

void measureOnTick()
{
    LOG_IF_ERR(io::adbms::command::snap());
    LOG_IF_ERR(io::adbms::read::cAdc(true, local.voltage_stats)); // RD on
    app::pack::broadcast::cellVoltages(local.voltage_stats);
    app::pack::voltage_channel.publish(local.voltage_stats);
    LOG_IF_ERR(io::adbms::command::unsnap());

    if (diag_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED)
        fsm.setNextState(diagnostic_odd_state);
    else if (app::pack::balancing::getRequest().start_balance)
        fsm.setNextState(balance_state);
}

result<void> balanceOnEntry()
{
    const io::adbms::PackCells<uint8_t> duty = app::pack::balancing::getRequest().duty;
    RETURN_IF_ERR(app::pack::config::setPWMConfig(duty));
    RETURN_IF_ERR(app::pack::config::checkPWMConfig());
    app::pack::broadcast::balancing(duty);
    RETURN_IF_ERR(io::adbms::command::startCadc(false));                                  // RD off
    RETURN_IF_ERR(io::adbms::command::startSadc(false, io::adbms::OpenWireParity::NONE)); // CONT off
    RETURN_IF_ERR(io::adbms::command::startBalance());
    return {};
}

void balanceOnTick()
{
    LOG_IF_ERR(io::adbms::command::snap());
    LOG_IF_ERR(io::adbms::read::cAdc(false, local.voltage_stats)); // RD off
    app::pack::broadcast::cellVoltages(local.voltage_stats);
    app::pack::voltage_channel.publish(local.voltage_stats);
    LOG_IF_ERR(io::adbms::command::unsnap());

    if (diag_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED)
        fsm.setNextState(diagnostic_odd_state);
    else if (!app::pack::balancing::getRequest().start_balance)
        fsm.setNextState(measure_state);
}

result<void> balanceOnExit()
{
    RETURN_IF_ERR(io::adbms::command::stopBalance());
    app::pack::broadcast::balancing({});
    return {};
}

result<void> diagnosticOddOnEntry()
{
    RETURN_IF_ERR(app::pack::config::setSegmentConfig(io::adbms::ThermistorMux::ODD));
    RETURN_IF_ERR(app::pack::config::checkSegmentConfig());
    RETURN_IF_ERR(io::adbms::command::startCadc(false));                                 // RD off
    RETURN_IF_ERR(io::adbms::command::startSadc(false, io::adbms::OpenWireParity::ODD)); // CONT off
    RETURN_IF_ERR(io::adbms::command::startAuxadc());
    sadc_done = false;
    xadc_done = false;
    return {};
}

void diagnosticOddOnTick()
{
    const bool sadc_ready = io::adbms::command::pollSadc().has_value();
    const bool xadc_ready = io::adbms::command::pollXadc().has_value();

    LOG_IF_ERR(io::adbms::command::snap());
    LOG_IF_ERR(io::adbms::read::cAdc(false, local.voltage_stats)); // RD off
    app::pack::broadcast::cellVoltages(local.voltage_stats);

    if (sadc_ready && !sadc_done)
    {
        LOG_IF_ERR(io::adbms::read::sAdc(io::adbms::OpenWireParity::ODD, local.owc_stats));
        app::pack::broadcast::cellOpenWire(local.owc_stats);
        app::pack::owc_channel.publish(local.owc_stats);
        sadc_done = true;
    }

    if (xadc_ready && !xadc_done)
    {
        LOG_IF_ERR(io::adbms::read::xAdc(io::adbms::ThermistorMux::ODD, local.temperature_stats));
        app::pack::broadcast::cellTemps(local.temperature_stats);
        app::pack::temperature_channel.publish(local.temperature_stats);
        xadc_done = true;
    }

    LOG_IF_ERR(io::adbms::read::flags(local.adbms6830_diag));
    app::pack::broadcast::adbmsFlags(local.adbms6830_diag);
    app::pack::diag_channel.publish(local.adbms6830_diag);
    LOG_IF_ERR(io::adbms::write::clearFlags());
    LOG_IF_ERR(io::adbms::command::unsnap());

    if (sadc_done && xadc_done)
        fsm.setNextState(diagnostic_even_state);
}

result<void> diagnosticEvenOnEntry()
{
    RETURN_IF_ERR(app::pack::config::setSegmentConfig(io::adbms::ThermistorMux::EVEN));
    RETURN_IF_ERR(app::pack::config::checkSegmentConfig());
    RETURN_IF_ERR(io::adbms::command::startCadc(false));                                  // RD off
    RETURN_IF_ERR(io::adbms::command::startSadc(false, io::adbms::OpenWireParity::EVEN)); // CONT off
    RETURN_IF_ERR(io::adbms::command::startAuxadc());
    sadc_done = false;
    xadc_done = false;
    return {};
}

void diagnosticEvenOnTick()
{
    const bool sadc_ready = io::adbms::command::pollSadc().has_value();
    const bool xadc_ready = io::adbms::command::pollXadc().has_value();

    LOG_IF_ERR(io::adbms::command::snap());
    LOG_IF_ERR(io::adbms::read::cAdc(false, local.voltage_stats)); // RD off
    app::pack::broadcast::cellVoltages(local.voltage_stats);

    if (sadc_ready && !sadc_done)
    {
        LOG_IF_ERR(io::adbms::read::sAdc(io::adbms::OpenWireParity::EVEN, local.owc_stats));
        app::pack::broadcast::cellOpenWire(local.owc_stats);
        app::pack::owc_channel.publish(local.owc_stats);
        sadc_done = true;
    }

    if (xadc_ready && !xadc_done)
    {
        LOG_IF_ERR(io::adbms::read::xAdc(io::adbms::ThermistorMux::EVEN, local.temperature_stats));
        app::pack::broadcast::cellTemps(local.temperature_stats);
        app::pack::temperature_channel.publish(local.temperature_stats);
        xadc_done = true;
    }

    LOG_IF_ERR(io::adbms::read::flags(local.adbms6830_diag));
    app::pack::broadcast::adbmsFlags(local.adbms6830_diag);
    app::pack::diag_channel.publish(local.adbms6830_diag);
    LOG_IF_ERR(io::adbms::write::clearFlags());
    LOG_IF_ERR(io::adbms::command::unsnap());

    if (sadc_done && xadc_done)
        fsm.setNextState(app::pack::balancing::getRequest().start_balance ? balance_state : measure_state);
}

result<void> diagnosticOnExit()
{
    // also add the cell owc and temp owc detection here
    diag_timer.restart();
    return {};
}
} // namespace

namespace app::pack::sequence
{
void init()
{
    local = Snapshot{};
    fsm.init(diagnostic_odd_state);
}

void tick()
{
    fsm.tick();
}
} // namespace app::pack::sequence
