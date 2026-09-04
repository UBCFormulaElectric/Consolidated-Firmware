#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "app_timer.hpp"
#include "io_time.hpp"
namespace {

    inline constexpr uint32_t DIAG_PERIOD_MS = 250;

    app::pack::Snapshot local{};
    app::Timer          diag_timer{ DIAG_PERIOD_MS };
    const SequenceState *current = nullptr;
    const SequenceState *next    = nullptr;
    bool                entered = false;
    io::adbms::OpenWireParity sadc_ow_parity;
    io::adbms::ThermistorMux  xadc_therm_mux;
    bool sadc_done = false;
    bool xadc_done = false;

    const SequenceState measure_state{ measureOnEntry, measureOnTick};
    const SequenceState balance_state{ balanceOnEntry, balanceOnTick};
    const SequenceState diagnostic_state{ diagnosticOnEntry, diagnosticOnTick};

    void setNextState(const SequenceState *const s) {
        next = s;
    }

    void runStateMachine() {
        if (next != current) {
            current = next;
            entered = false;
        }
        if (!entered) {
            entered = current->run_on_entry().has_value();
            return;
        }
        current->run_on_tick();
    }

    result<void> measureOnEntry() {
        RETURN_IF_ERR(io::adbms::command::stopBalance());
        RETURN_IF_ERR(app::pack::config::setSegmentConfig(xadc_therm_mux));
        RETURN_IF_ERR(app::pack::config::checkSegmentConfig());
        RETURN_IF_ERR(io::adbms::command::startCadc(true));  // RD on
        RETURN_IF_ERR(io::adbms::command::startSadc(true, io::adbms::OpenWireParity::NONE)); // CONT on
        return {};
    }

    void measureOnTick() {
        LOG_IF_ERR(io::adbms::command::snap());
        LOG_IF_ERR(io::adbms::read::Cadc(true, local.voltage_stats)); // RD on
        app::pack::broadcast::cellVoltages(local.voltage_stats);
        LOG_IF_ERR(io::adbms::command::unsnap());
    }

    result<void> balanceOnEntry() {
        RETURN_IF_ERR(app::pack::config::setPWMConfig(app::pack::balancing::getRequest().duty));
        RETURN_IF_ERR(app::pack::config::checkPWMConfig());
        RETURN_IF_ERR(io::adbms::command::startCadc(false)); // RD off
        RETURN_IF_ERR(io::adbms::command::startSadc(false, io::adbms::OpenWireParity::NONE)); // CONT off
        RETURN_IF_ERR(io::adbms::command::startBalance());
        return {};
    }

    void balanceOnTick() {
        LOG_IF_ERR(io::adbms::command::snap());
        LOG_IF_ERR(io::adbms::read::Cadc(false, local.voltage_stats)); // RD off
        app::pack::broadcast::cellVoltages(local.voltage_stats);
        LOG_IF_ERR(io::adbms::command::unsnap());
    }

    result<void> diagnosticOnEntry() {
        RETURN_IF_ERR(io::adbms::command::stopBalance());
        RETURN_IF_ERR(app::pack::config::setSegmentConfig(xadc_therm_mux));
        RETURN_IF_ERR(app::pack::config::checkSegmentConfig());
        RETURN_IF_ERR(io::adbms::command::startCadc(false)); // RD off
        RETURN_IF_ERR(io::adbms::command::startSadc(false, sadc_ow_parity)); // CONT off
        RETURN_IF_ERR(io::adbms::command::startAuxadc());
        sadc_done = false;
        xadc_done = false;
        return {};
    }

    void diagnosticOnTick() {
        const bool sadc_ready = io::adbms::command::pollSadc().has_value();
        const bool xadc_ready = io::adbms::command::pollXadc().has_value();

        LOG_IF_ERR(io::adbms::command::snap());
        LOG_IF_ERR(io::adbms::read::Cadc(false)); // RD off
        app::pack::broadcast::cellVoltages(local.voltage_stats);

        if (sadc_ready && !sadc_done) {
            LOG_IF_ERR(io::adbms::read::Sadc(sadc_ow_parity, local.owc_stats));
            io::adbms::broadcast::cellOpenWire(local.owc_stats);
            sadc_done = true;
        }

        if (xadc_ready && !xadc_done) {
            LOG_IF_ERR(io::adbms::read::Xadc(xadc_therm_mux, local.temperature_stats));
            io::adbms::broadcast::cellTemps(local.temperature_stats);
            xadc_done = true;
        }

        LOG_IF_ERR(io::adbms::read::flags(local.adbms6830_diag));
        io::adbms::broadcast::adbmsFlags(local.adbms6830_diag);
        LOG_IF_ERR(io::adbms::command::unsnap());

        if (sadc_done && xadc_done) {
            setNextState(
                app::pack::balancing::getRequest().start_balance ? &balance_state : &measure_state);
        }
    }
}

namespace app::pack::sequence {
    void init() {
        local = Snapshot{};
        current = &measure_state;
        next    = &measure_state;
        entered = false;
        sadc_ow_parity = io::adbms::OpenWireParity::ODD;
        xadc_therm_mux = io::adbms::ThermistorMux::ODD;
        diag_timer.restart();
    }

    void tick() {
        const bool diag_due = diag_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED;

        if (diag_due && current != &diagnostic_state) {
            diag_timer.restart();
            sadc_ow_parity = (sadc_ow_parity == io::adbms::OpenWireParity::EVEN)
                               ? io::adbms::OpenWireParity::ODD
                               : io::adbms::OpenWireParity::EVEN;
            xadc_therm_mux = (xadc_therm_mux == io::adbms::ThermistorMux::EVEN)
                               ? io::adbms::ThermistorMux::ODD
                               : io::adbms::ThermistorMux::EVEN;
            setNextState(&diagnostic_state);
        } else if (current != &diagnostic_state) {
            setNextState(balancing::getRequest().start_balance ? &balance_state : &measure_state);
        }
        runStateMachine();
    }
}
