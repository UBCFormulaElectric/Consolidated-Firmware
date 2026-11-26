#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "app_timer.hpp"
#include "io_adbms.hpp"
#include <util_errorCodes.hpp>

namespace
{
// Each state runs on_entry until it succeeds, then on_tick every tick. on_exit runs before switching states.
class AdbmsStateMachine
{
  public:
    struct State
    {
        const char *name;
        result<void> (*on_entry)();
        void (*on_tick)();
        result<void> (*on_exit)();
    };

    void init(const State &initial_state)
    {
        state      = &initial_state;
        next_state = &initial_state;
        entered    = false;
    }

    void setNextState(const State &s) { next_state = &s; }

    [[nodiscard]] const State *getCurrentState() const { return state; }

    void tick()
    {
        if (next_state != state)
        {
            entered = false;
            if (state->on_exit != nullptr && !state->on_exit().has_value())
                return;
            state = next_state;
        }

        if (!entered)
        {
            entered = state->on_entry == nullptr || state->on_entry().has_value();
            return;
        }

        if (state->on_tick != nullptr)
            state->on_tick();
    }

  private:
    const State *state      = nullptr;
    const State *next_state = nullptr;
    bool         entered    = false;
};

app::Timer temp_timer{ TEMP_PERIOD_MS };
app::Timer cowc_timer{ COWC_PERIOD_MS };
app::Timer sowc_timer{ SOWC_PERIOD_MS };
app::Timer balance_timer{ BALANCE_PERIOD_MS };
app::Timer settle_timer{ SETTLE_MS };

io::adbms::VoltageBuffer             voltages{};
io::adbms::TemperatureBuffer         temps{};
io::adbms::SupplyBuffer              supply{};
io::adbms::DiagnosticBuffer          diag{};
io::adbms::VoltageBuffer             owc_odd_voltages{};
io::adbms::VoltageBuffer             owc_even_voltages{};
io::adbms::PackTherms<result<float>> therm_voltages{};

result<void> measureOnEntry();
void         measureOnTick();
result<void> balanceOnEntry();
void         balanceOnTick();
result<void> tempOnEntry();
void         tempOnTick();
result<void> tempOnExit();
result<void> cowcOnEntry();
void         cowcOnTick();
result<void> cowcOnExit();
result<void> sowcOnEntry();
void         sowcOnTick();
result<void> sowcOnExit();

const AdbmsStateMachine::State measure_state{
    "CONTINUOUS REDUNDANT PRIMARY VOLTAGE MEASUREMENTS; CONTINUOUS SECONDARY VOLTAGE MEASUREMENTS", measureOnEntry,
    measureOnTick, nullptr
};
const AdbmsStateMachine::State balance_a_state{
    "CONTINUOUS PRIMARY VOLTAGE MEASUREMENTS; THERM_0_7 TEMP MEASUREMENTS; BALANCE", balanceOnEntry, balanceOnTick,
    nullptr
};
const AdbmsStateMachine::State balance_b_state{
    "CONTINUOUS PRIMARY VOLTAGE MEASUREMENTS; THERM_8_13 TEMP MEASUREMENTS; "
    "THERM OPEN WIRE CHECK; BALANCE",
    balanceOnEntry, balanceOnTick, tempOnExit
};
const AdbmsStateMachine::State temp_a_state{
    "CONTINUOUS REDUNDANT PRIMARY VOLTAGE MEASUREMENTS; THERM_0_7 TEMP MEASUREMENTS; "
    "DIAGNOSTICS",
    tempOnEntry, tempOnTick, nullptr
};
const AdbmsStateMachine::State temp_b_state{
    "CONTINUOUS REDUNDANT PRIMARY VOLTAGE MEASUREMENTS; THERM_8_13 TEMP MEASUREMENTS; "
    "THERM OPEN WIRE CHECK; DIAGNOSTICS",
    tempOnEntry, tempOnTick, tempOnExit
};
const AdbmsStateMachine::State cowc_a_state{
    "CONTINUOUS SECONDARY VOLTAGE MEASUREMENTS; ODD CELL PRIMARY OPEN WIRE CHECK", cowcOnEntry, cowcOnTick, nullptr
};
const AdbmsStateMachine::State cowc_b_state{
    "CONTINUOUS SECONDARY VOLTAGE MEASUREMENTS; EVEN CELL PRIMARY OPEN WIRE CHECK", cowcOnEntry, cowcOnTick, cowcOnExit
};
const AdbmsStateMachine::State sowc_a_state{
    "CONTINUOUS PRIMARY VOLTAGE MEASUREMENTS; ODD CELL SECONDARY OPEN WIRE CHECK", sowcOnEntry, sowcOnTick, nullptr
};
const AdbmsStateMachine::State sowc_b_state{
    "CONTINUOUS PRIMARY VOLTAGE MEASUREMENTS; EVEN CELL SECONDARY OPEN WIRE CHECK", sowcOnEntry, sowcOnTick, sowcOnExit
};

AdbmsStateMachine fsm;

/*
                                    ADBMS STATE MACHINE

                                        init
                                          │
                                          ▼
  ┌─────────────────────────────────────────────────────────────────────────────────────┐
  │                                       MEASURE                                       │
  │                     C-ADC cont + RD    S-ADC cont    read C-ADC                     │
  └─────┬─────────▲──────────┬─────────▲──────────┬─────────▲─────────┬─────────────▲───┘
        │         │     cowc_timer     │     sowc_timer     │         │ balancing   │ !balancing ||
   temp_timer     │        (5 s)       │        (5 s)       │         │ && settled  │ balance_timer
      (1 s)       │     && settled     │     && settled     │         │ (10 s)      │ (120 s)
        ▼         │          ▼         │          ▼         │         ▼             │ from A or B
  ┌───────────┐   │    ┌───────────┐   │    ┌───────────┐   │   ┌───────────────────┴─────┐
  │   TEMP_A  │   │    │   COWC_A  │   │    │   SOWC_A  │   │   │        BALANCE_A        │
  │ therms 0-7│   │    │ C-ADC odd │   │    │ S-ADC odd │   │   │        therms 0-7       │
  └─────┬─────┘   │    └─────┬─────┘   │    └─────┬─────┘   │   └─────┬─────────────▲─────┘
        │ done    │          │ done    │          │ done    │         │ done        │ done
        ▼         │          ▼         │          ▼         │         ▼             │
  ┌───────────┐   │    ┌───────────┐   │    ┌───────────┐   │   ┌───────────────────┴─────┐
  │   TEMP_B  ├───┘    │   COWC_B  ├───┘    │   SOWC_B  ├───┘   │        BALANCE_B        │
  │therms 8-13│ done   │ C-ADC even│ done   │ S-ADC even│ done  │       therms 8-13       │
  └───────────┘        └───────────┘        └───────────┘       └─────────────────────────┘

  TIMERS    temp_timer     1 s  from TEMP_B / BALANCE_B exit to the next TEMP_A
            cowc_timer     5 s  from COWC_B exit to the next COWC_A
            sowc_timer     5 s  from SOWC_B exit to the next SOWC_A
            balance_timer 120 s from MEASURE -> BALANCE_A until balancing hands back to MEASURE
            settle_timer  10 s  from leaving BALANCE_A/B until voltages feed balance_buffer and BALANCE_A,
                                COWC and SOWC can be entered

  MEASURE   entry:  stop balance, start C-ADC (cont, RD on), S-ADC (cont)
            tick:   read C-ADC, copy into balance_buffer once settled
                    priority: temp_timer > cowc_timer > sowc_timer > balance (all but temps wait for settled)
                    restart balance_timer when going to BALANCE_A
  BALANCE_A/B
            entry:  set therm mux (0-7 / 8-13), load pwm, C-ADC (cont, RD off),
                    S-ADC single-shot with DCP (ends cont S-ADC, PWM keeps running), start AUX ADC, start balance
            tick:   read C-ADC; once AUX ready read therms, supply; read flags; then hop to the other half
                    leaving for MEASURE restarts settle_timer
            exit:   none on A; B does the therm OWC check -> therm_owc_buffer, restart temp_timer
  TEMP_A/B  entry:  set therm mux (0-7 / 8-13), C-ADC (cont, RD on), S-ADC (cont), start AUX ADC
            tick:   read C-ADC; once AUX ready read therms, supply; read flags
            B exit: therm OWC check -> therm_owc_buffer, restart temp_timer
  COWC_A/B  entry:  S-ADC (cont, no OW), C-ADC (single-shot, RD off, OW odd/even)
            tick:   read S-ADC for voltages; once C-ADC ready read the C-ADC OWC result
            B exit: C-ADC cell OWC check -> cadc_owc_buffer, restart cowc_timer
  SOWC_A/B  entry:  C-ADC (cont, RD off), S-ADC (single-shot, OW odd/even)
            tick:   read C-ADC for voltages; once S-ADC ready read the S-ADC OWC result
            B exit: S-ADC cell OWC check -> sadc_owc_buffer, restart sowc_timer

*/

result<void> measureOnEntry()
{
    RETURN_IF_ERR(io::adbms::command::resetCmdCount());
    RETURN_IF_ERR(io::adbms::command::stopBalance()); // After BALANCE, and on boot after an MCU reset
    RETURN_IF_ERR(app::pack::config::setSegmentConfig(io::adbms::ThermistorMux::THERM_0_7));
    RETURN_IF_ERR(app::pack::config::checkSegmentConfig());
    RETURN_IF_ERR(io::adbms::command::startCadc(true, true, io::adbms::OpenWireParity::NONE)); // CONT on, RD on
    RETURN_IF_ERR(io::adbms::command::startSadc(true, io::adbms::OpenWireParity::NONE));       // CONT on
    return {};
}

void measureOnTick()
{
    LOG_IF_ERR(io::adbms::command::resetCmdCount());
    LOG_IF_ERR(io::adbms::command::snap());
    const result<void> cadc = io::adbms::read::cAdc(true, voltages); // RD on
    LOG_IF_ERR(cadc);
    LOG_IF_ERR(io::adbms::write::clearCsFlags());
    app::pack::broadcast::cellVoltages(voltages);

    // Cells need to recover if they were previously balancing
    const bool settled = settle_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED;
    bool       balancing_enabled;
    {
        const io::semaphore::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::voltage_buffer = voltages;

        // Only settled data is used for balancing
        if (settled)
        {
            app::pack::balance_buffer.updated_ms = voltages.updated_ms;
            app::pack::balance_buffer.voltages   = voltages.voltages;
        }
        balancing_enabled = app::pack::balance_buffer.balancing_enabled;
    }
    LOG_IF_ERR(io::adbms::command::unsnap());

    if (temp_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED)
        fsm.setNextState(temp_a_state);
    // Open wire checks needs settled cells
    else if (settled && cowc_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED)
        fsm.setNextState(cowc_a_state);
    else if (settled && sowc_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED)
        fsm.setNextState(sowc_a_state);
    else if (balancing_enabled && settled)
    {
        balance_timer.restart();
        fsm.setNextState(balance_a_state);
    }
}

result<void> balanceOnEntry()
{
    const auto therm_mux = fsm.getCurrentState() == &balance_a_state ? io::adbms::ThermistorMux::THERM_0_7
                                                                     : io::adbms::ThermistorMux::THERM_8_13;
    RETURN_IF_ERR(io::adbms::command::resetCmdCount());
    io::adbms::PackCells<uint8_t> duty;
    {
        const io::semaphore::unique_semaphore s{ app::pack::buffer_lock };
        duty = app::pack::balance_buffer.pwm;
    }
    RETURN_IF_ERR(app::pack::config::setSegmentConfig(therm_mux));
    RETURN_IF_ERR(app::pack::config::checkSegmentConfig());
    RETURN_IF_ERR(app::pack::config::setPWMConfig(duty));
    RETURN_IF_ERR(app::pack::config::checkPWMConfig());
    app::pack::broadcast::balancing(duty);
    RETURN_IF_ERR(io::adbms::command::startCadc(true, false, io::adbms::OpenWireParity::NONE)); // CONT on, RD off
    RETURN_IF_ERR(io::adbms::command::startSadc(false, io::adbms::OpenWireParity::NONE));       // CONT off
    RETURN_IF_ERR(io::adbms::command::startAuxadc());
    RETURN_IF_ERR(io::adbms::command::startBalance());
    return {};
}

void balanceOnTick()
{
    const bool half_a    = fsm.getCurrentState() == &balance_a_state;
    const auto therm_mux = half_a ? io::adbms::ThermistorMux::THERM_0_7 : io::adbms::ThermistorMux::THERM_8_13;

    LOG_IF_ERR(io::adbms::command::resetCmdCount());

    const bool xadc_ready = io::adbms::command::pollXadc().has_value();

    LOG_IF_ERR(io::adbms::command::snap());
    LOG_IF_ERR(io::adbms::read::cAdc(false, voltages)); // RD off
    app::pack::broadcast::cellVoltages(voltages);

    if (xadc_ready)
    {
        LOG_IF_ERR(io::adbms::read::xAdcTherm(therm_mux, temps, therm_voltages));
        LOG_IF_ERR(io::adbms::read::xAdcSupply(supply));
        app::pack::broadcast::cellTemps(temps);
        app::pack::broadcast::thermVoltages(therm_voltages);
        app::pack::broadcast::adbmsSupply(supply);
    }

    LOG_IF_ERR(io::adbms::read::flags(diag));
    app::pack::broadcast::adbmsFlags(diag);
    LOG_IF_ERR(io::adbms::write::clearAllFlags());
    LOG_IF_ERR(io::adbms::command::unsnap());

    bool balancing_enabled;
    {
        const io::semaphore::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::voltage_buffer = voltages;
        app::pack::temp_buffer    = temps;
        app::pack::diag_buffer    = diag;
        app::pack::supply_buffer  = supply;
        balancing_enabled         = app::pack::balance_buffer.balancing_enabled;
    }

    if (!balancing_enabled || balance_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED)
    {
        settle_timer.restart();
        fsm.setNextState(measure_state);
    }
    else if (xadc_ready)
        fsm.setNextState(half_a ? balance_b_state : balance_a_state);
}

result<void> tempOnEntry()
{
    const auto therm_mux = fsm.getCurrentState() == &temp_a_state ? io::adbms::ThermistorMux::THERM_0_7
                                                                  : io::adbms::ThermistorMux::THERM_8_13;
    RETURN_IF_ERR(io::adbms::command::resetCmdCount());
    RETURN_IF_ERR(app::pack::config::setSegmentConfig(therm_mux));
    RETURN_IF_ERR(app::pack::config::checkSegmentConfig());
    RETURN_IF_ERR(io::adbms::command::startCadc(true, true, io::adbms::OpenWireParity::NONE)); // CONT on, RD on
    RETURN_IF_ERR(io::adbms::command::startSadc(true, io::adbms::OpenWireParity::NONE));       // CONT on
    RETURN_IF_ERR(io::adbms::command::startAuxadc());
    return {};
}

void tempOnTick()
{
    const bool half_a    = fsm.getCurrentState() == &temp_a_state;
    const auto therm_mux = half_a ? io::adbms::ThermistorMux::THERM_0_7 : io::adbms::ThermistorMux::THERM_8_13;

    LOG_IF_ERR(io::adbms::command::resetCmdCount());

    const bool xadc_ready = io::adbms::command::pollXadc().has_value();

    LOG_IF_ERR(io::adbms::command::snap());
    LOG_IF_ERR(io::adbms::read::cAdc(true, voltages)); // RD on
    app::pack::broadcast::cellVoltages(voltages);

    if (xadc_ready)
    {
        LOG_IF_ERR(io::adbms::read::xAdcTherm(therm_mux, temps, therm_voltages));
        LOG_IF_ERR(io::adbms::read::xAdcSupply(supply));
        app::pack::broadcast::cellTemps(temps);
        app::pack::broadcast::thermVoltages(therm_voltages);
        app::pack::broadcast::adbmsSupply(supply);
    }

    LOG_IF_ERR(io::adbms::read::flags(diag));
    app::pack::broadcast::adbmsFlags(diag);
    LOG_IF_ERR(io::adbms::write::clearAllFlags());
    LOG_IF_ERR(io::adbms::command::unsnap());

    {
        const io::semaphore::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::voltage_buffer = voltages;
        app::pack::temp_buffer    = temps;
        app::pack::diag_buffer    = diag;
        app::pack::supply_buffer  = supply;
    }

    if (xadc_ready)
        fsm.setNextState(half_a ? temp_b_state : measure_state);
}

result<void> tempOnExit()
{
    temp_timer.restart();
    const app::pack::ThermOpenWireCheckBuffer owc = app::pack::owc::checkTherms(therm_voltages);
    app::pack::broadcast::thermOpenWire(owc);
    {
        const io::semaphore::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::therm_owc_buffer = owc;
    }
    return {};
}

result<void> cowcOnEntry()
{
    const auto owc_parity =
        fsm.getCurrentState() == &cowc_a_state ? io::adbms::OpenWireParity::ODD : io::adbms::OpenWireParity::EVEN;
    RETURN_IF_ERR(io::adbms::command::resetCmdCount());
    RETURN_IF_ERR(app::pack::config::setSegmentConfig(io::adbms::ThermistorMux::THERM_0_7));
    RETURN_IF_ERR(app::pack::config::checkSegmentConfig());
    RETURN_IF_ERR(io::adbms::command::startSadc(true, io::adbms::OpenWireParity::NONE)); // CONT on
    RETURN_IF_ERR(io::adbms::command::startCadc(false, false, owc_parity));              // CONT off, RD off
    return {};
}

void cowcOnTick()
{
    const bool half_a = fsm.getCurrentState() == &cowc_a_state;

    LOG_IF_ERR(io::adbms::command::resetCmdCount());

    auto &owc_voltages = half_a ? owc_odd_voltages : owc_even_voltages;

    const bool cadc_ready = io::adbms::command::pollCadc().has_value();

    LOG_IF_ERR(io::adbms::command::snap());
    LOG_IF_ERR(io::adbms::read::sAdc(voltages));
    app::pack::broadcast::cellVoltages(voltages);

    if (cadc_ready)
    {
        LOG_IF_ERR(io::adbms::read::cAdc(false, owc_voltages)); // RD off
    }

    LOG_IF_ERR(io::adbms::command::unsnap());

    {
        const io::semaphore::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::voltage_buffer = voltages;
    }

    if (cadc_ready)
        fsm.setNextState(half_a ? cowc_b_state : measure_state);
}

result<void> cowcOnExit()
{
    cowc_timer.restart();
    const app::pack::OpenWireCheckBuffer owc =
        app::pack::owc::check(owc_odd_voltages.voltages, owc_even_voltages.voltages);
    app::pack::broadcast::cadcOpenWire(owc);
    {
        const io::semaphore::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::cadc_owc_buffer = owc;
    }
    return {};
}

result<void> sowcOnEntry()
{
    const auto owc_parity =
        fsm.getCurrentState() == &sowc_a_state ? io::adbms::OpenWireParity::ODD : io::adbms::OpenWireParity::EVEN;
    RETURN_IF_ERR(io::adbms::command::resetCmdCount());
    RETURN_IF_ERR(app::pack::config::setSegmentConfig(io::adbms::ThermistorMux::THERM_0_7));
    RETURN_IF_ERR(app::pack::config::checkSegmentConfig());
    RETURN_IF_ERR(io::adbms::command::startCadc(true, false, io::adbms::OpenWireParity::NONE)); // CONT on, RD off
    RETURN_IF_ERR(io::adbms::command::startSadc(false, owc_parity));                            // CONT off
    return {};
}

void sowcOnTick()
{
    const bool half_a = fsm.getCurrentState() == &sowc_a_state;

    LOG_IF_ERR(io::adbms::command::resetCmdCount());

    auto &owc_voltages = half_a ? owc_odd_voltages : owc_even_voltages;

    const bool sadc_ready = io::adbms::command::pollSadc().has_value();

    LOG_IF_ERR(io::adbms::command::snap());
    LOG_IF_ERR(io::adbms::read::cAdc(false, voltages)); // RD off
    app::pack::broadcast::cellVoltages(voltages);

    if (sadc_ready)
    {
        LOG_IF_ERR(io::adbms::read::sAdc(owc_voltages));
    }

    LOG_IF_ERR(io::adbms::command::unsnap());

    {
        const io::semaphore::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::voltage_buffer = voltages;
    }

    if (sadc_ready)
        fsm.setNextState(half_a ? sowc_b_state : measure_state);
}

result<void> sowcOnExit()
{
    sowc_timer.restart();
    const app::pack::OpenWireCheckBuffer owc =
        app::pack::owc::check(owc_odd_voltages.voltages, owc_even_voltages.voltages);
    app::pack::broadcast::sadcOpenWire(owc);
    {
        const io::semaphore::unique_semaphore s{ app::pack::buffer_lock };
        app::pack::sadc_owc_buffer = owc;
    }
    return {};
}
} // namespace

namespace app::pack::sequence
{
void init()
{
    fsm.init(measure_state);
}

void tick()
{
    fsm.tick();
}
} // namespace app::pack::sequence
