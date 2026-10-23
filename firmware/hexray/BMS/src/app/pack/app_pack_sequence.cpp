#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "app_fsm.hpp"
#include "app_timer.hpp"
#include "io_adbms.hpp"

namespace
{
constexpr uint32_t DIAG_PERIOD_MS  = 1000;
constexpr uint32_t DIAG_TIMEOUT_MS = 200; // Adjust timing

app::Timer diag_timer{ DIAG_PERIOD_MS };
app::Timer diag_timeout_timer{ DIAG_TIMEOUT_MS };
bool       sadc_done = false;
bool       xadc_done = false;

app::pack::VoltageBuffer             voltages{};
app::pack::TemperatureBuffer         temps{};
app::pack::SupplyBuffer              supply{};
app::pack::DiagnosticBuffer          diag{};
io::adbms::PackCells<result<float>>  owc_odd_voltages{};
io::adbms::PackCells<result<float>>  owc_even_voltages{};
io::adbms::PackTherms<result<float>> therm_voltages{};

result<void>                                                                    measureOnEntry();
void                                                                            measureOnTick();
result<void>                                                                    balanceOnEntry();
void                                                                            balanceOnTick();
result<void>                                                                    balanceOnExit();
template <io::adbms::OpenWireParity, io::adbms::ThermistorMux> result<void>     diagnosticOnEntry();
template <io::adbms::OpenWireParity, io::adbms::ThermistorMux> void             diagnosticOnTick();
result<void>                                                                    diagnosticOnExit();

const app::Fsm::State measure_state{
    "CONTINUOUS REDUNDANT PRIMARY VOLTAGE MEASUREMENTS; CONTINUOUS SECONDARY VOLTAGE MEASUREMENTS", measureOnEntry,
    measureOnTick, nullptr
};
const app::Fsm::State balance_state{ "CONTINUOUS PRIMARY VOLTAGE MEASUREMENTS; BALANCE", balanceOnEntry, balanceOnTick,
                                     balanceOnExit };
const app::Fsm::State diagnostic_a_state{ "CONTINUOUS PRIMARY VOLTAGE MEASUREMENTS; THERM_0_7 TEMP MEASUREMENTS; ODD CELL "
                                            "OPEN WIRE CHECK; THERM_8_13 THERM OPEN WIRE CHECK; DIAGNOSTICS",
                                            diagnosticOnEntry<io::adbms::OpenWireParity::ODD, io::adbms::ThermistorMux::THERM_0_7>,
                                            diagnosticOnTick<io::adbms::OpenWireParity::ODD, io::adbms::ThermistorMux::THERM_0_7>, nullptr };
const app::Fsm::State diagnostic_b_state{ "CONTINUOUS PRIMARY VOLTAGE MEASUREMENTS; THERM_8_13 TEMP MEASUREMENTS; EVEN "
                                             "CELL OPEN WIRE CHECK; THERM_8_13 THERM OPEN WIRE CHECK; DIAGNOSTICS",
                                             diagnosticOnEntry<io::adbms::OpenWireParity::EVEN, io::adbms::ThermistorMux::THERM_8_13>,
                                             diagnosticOnTick<io::adbms::OpenWireParity::EVEN, io::adbms::ThermistorMux::THERM_8_13>, diagnosticOnExit };

app::Fsm fsm;

result<void> measureOnEntry()
{
    RETURN_IF_ERR(io::adbms::command::startCadc(true)); // RD on
    RETURN_IF_ERR(io::adbms::command::startSadc(true, io::adbms::OpenWireParity::NONE)); // CONT on
    return {};
}

void measureOnTick()
{
    LOG_IF_ERR(io::adbms::command::snap());
    LOG_IF_ERR(io::adbms::read::cAdc(true, voltages)); // RD on
    app::pack::broadcast::cellVoltages(voltages);
    {
        const io::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::voltage_buffer = voltages;
    }
    LOG_IF_ERR(io::adbms::command::unsnap());

    if (diag_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED)
        fsm.setNextState(diagnostic_a_state);
    else if (app::pack::balancing::getRequest().start_balance)
        fsm.setNextState(balance_state);
}

result<void> balanceOnEntry()
{
    const io::adbms::PackCells<uint8_t> duty = app::pack::balancing::getRequest().duty; //needs to be fixed
    RETURN_IF_ERR(app::pack::config::setPWMConfig(duty));
    RETURN_IF_ERR(app::pack::config::checkPWMConfig());
    app::pack::broadcast::balancing(duty);
    RETURN_IF_ERR(io::adbms::command::startCadc(false)); // RD off
    RETURN_IF_ERR(io::adbms::command::startSadc(false, io::adbms::OpenWireParity::NONE)); // CONT off
    RETURN_IF_ERR(io::adbms::command::startBalance());
    return {};
}

void balanceOnTick()
{
    LOG_IF_ERR(io::adbms::command::snap());
    LOG_IF_ERR(io::adbms::read::cAdc(false, voltages)); // RD off
    app::pack::broadcast::cellVoltages(voltages);
    {
        const io::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::voltage_buffer = voltages;
    }
    LOG_IF_ERR(io::adbms::command::unsnap());

    if (diag_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED)
        fsm.setNextState(diagnostic_a_state);
    else if (!app::pack::balancing::getRequest().start_balance) //needs to be fixed
        fsm.setNextState(measure_state);
}

result<void> balanceOnExit()
{
    RETURN_IF_ERR(io::adbms::command::stopBalance());
    app::pack::broadcast::balancing({}); //needs to be fixed
    return {};
}

template <io::adbms::OpenWireParity owc_parity, io::adbms::ThermistorMux therm_mux> result<void> diagnosticOnEntry()
{
    RETURN_IF_ERR(app::pack::config::setSegmentConfig(therm_mux));
    RETURN_IF_ERR(app::pack::config::checkSegmentConfig());
    RETURN_IF_ERR(io::adbms::command::startCadc(false));         // RD off
    RETURN_IF_ERR(io::adbms::command::startSadc(false, owc_parity)); // CONT off
    RETURN_IF_ERR(io::adbms::command::startAuxadc());
    diag_timeout_timer.restart();
    sadc_done = false;
    xadc_done = false;
    return {};
}

template <io::adbms::OpenWireParity owc_parity, io::adbms::ThermistorMux therm_mux> void diagnosticOnTick()
{
    auto &owc_voltages = owc_parity == io::adbms::OpenWireParity::ODD ? owc_odd_voltages : owc_even_voltages;

    const bool sadc_ready = io::adbms::command::pollSadc().has_value();
    const bool xadc_ready = io::adbms::command::pollXadc().has_value();

    LOG_IF_ERR(io::adbms::command::snap());
    LOG_IF_ERR(io::adbms::read::cAdc(false, voltages)); // RD off
    app::pack::broadcast::cellVoltages(voltages);

    if (sadc_ready && !sadc_done)
    {
        LOG_IF_ERR(io::adbms::read::sAdc(owc_voltages));
        sadc_done = true;
    }

    if (xadc_ready && !xadc_done)
    {
        LOG_IF_ERR(io::adbms::read::xAdcTherm(therm_mux, temps, therm_voltages));
        LOG_IF_ERR(io::adbms::read::xAdcSupply(supply));
        app::pack::broadcast::cellTemps(temps);
        app::pack::broadcast::adbmsSupply(supply);
        xadc_done = true;
    }

    LOG_IF_ERR(io::adbms::read::flags(diag));
    app::pack::broadcast::adbmsFlags(diag);
    LOG_IF_ERR(io::adbms::write::clearFlags());
    LOG_IF_ERR(io::adbms::command::unsnap());

    if (diag_timeout_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED)
    {
        if (!sadc_done)
        {
            for (auto &seg : owc_voltages)
                seg.fill(std::unexpected(ErrorCode::TIMEOUT));
            sadc_done = true;
        }

        if (!xadc_done)
        {
            for (auto &seg : temps.temperatures)
                seg.fill(std::unexpected(ErrorCode::TIMEOUT));
            for (auto &seg : therm_voltages)
                seg.fill(std::unexpected(ErrorCode::TIMEOUT));
            supply.vmv.fill(std::unexpected(ErrorCode::TIMEOUT));
            supply.vpv.fill(std::unexpected(ErrorCode::TIMEOUT));
            supply.vref.fill(std::unexpected(ErrorCode::TIMEOUT));
            supply.itmp.fill(std::unexpected(ErrorCode::TIMEOUT));
            supply.va.fill(std::unexpected(ErrorCode::TIMEOUT));
            supply.vd.fill(std::unexpected(ErrorCode::TIMEOUT));
            supply.vres.fill(std::unexpected(ErrorCode::TIMEOUT));
            app::pack::broadcast::cellTemps(temps);
            app::pack::broadcast::adbmsSupply(supply);
            xadc_done = true;
        }
    }

    {
        const io::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::voltage_buffer = voltages;
        app::pack::temp_buffer    = temps;
        app::pack::diag_buffer    = diag;
        app::pack::supply_buffer  = supply;
    }

    if (sadc_done && xadc_done)
    {
        if constexpr (therm_mux == io::adbms::ThermistorMux::THERM_0_7)
            fsm.setNextState(diagnostic_b_state);
        else
            fsm.setNextState(measure_state);
    }
}

result<void> diagnosticOnExit()
{
    const app::pack::OpenWireCheckBuffer owc =
        app::pack::owc::check(owc_odd_voltages, owc_even_voltages, therm_voltages);
    app::pack::broadcast::cellOpenWire(owc);
    {
        const io::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::owc_buffer = owc;
    }
    diag_timer.restart();
    return {};
}
} // namespace

namespace app::pack::sequence
{
void init()
{
    voltages          = {};
    temps             = {};
    supply            = {};
    diag              = {};
    owc_odd_voltages  = {};
    owc_even_voltages = {};
    therm_voltages    = {};
    fsm.init(diagnostic_a_state);
}

void tick()
{
    fsm.tick();
}
} // namespace app::pack::sequence
