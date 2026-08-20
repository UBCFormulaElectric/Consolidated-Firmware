#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "app_timer.hpp"
#include "io_time.hpp"


namespace {
    // Re-arm period for the therm sequence, measured cycle-start to cycle-start. The chain task
    // ticks at 50 ms and a full pass is 4 states, so a pass costs ~200 ms of this budget.
    inline constexpr uint32_t THERM_PERIOD_MS = 500;

    app::pack::Snapshot local{};

    CellStepState   cell_step_state;
    ThermStepState  therm_step_state;
    ConfigStepState config_step_stat;
    FlagsStepState  flags_step_state;

    app::Timer therm_timer{ THERM_PERIOD_MS };

    bool config_modified;
    ThermMux curr_mux;

    void stepCells() {
        switch (cell_step_state) {
            case CellStepState::IDLE:
                break;
            case CellStepState::START_BOTH_REDUNDANT:
                auto a = io::adbms::command::startCADC(true);
                auto b = io::adbms::command::startSADC();
                if (a && b) cell_step_state = CellStepState::READ_VOLTAGES;
                break;
            case CellStepState::START_BOTH_NO_REDUNDANT:
                auto a = io::adbms::command::startCADC(false);
                auto b = io::adbms::command::startSADC();
                if (a && b) cell_step_state = CellStepState::READ_VOLTAGES;
                break;
            case CellStepState::READ_BOTH_VOLTAGES:
                // need to add proper path for error handling/retry
                io::adbms::command::snap();
                auto time = io::time::getCurrentMs();
                io::adbms::read::cellVoltage();
                io::adbms::read::cellVoltageReduancy();
                io::adbms::command::unsnap();
                //figure out proper path 
                cell_step_state = CellStepState::IDLE;
                break;
            case CellStepState::START_SADC_OW_ODD:
                auto a = io::adbms::command::startSADC();
                if (!a) owc_step_state = OwcStepState::READ_SADC_OW_ODD;
                break;
            case CellStepState::READ_SADC_OW_ODD:
                if (io::adbms::command::pollSADC()) {
                    io::adbms::read::secondaryCellVoltage();
                    cell_step_state = CellStepState::IDLE;
                }
                break; 
            case CellStepState::START_SADC_OW_EVEN:
                auto a = io::adbms::command::startSADC();
                if (!a) owc_step_state = OwcStepState::READ_SADC_OW_EVEN;
                break;
            case CellStepState::READ_SADC_OW_EVEN:
                if (io::adbms::command::pollSADC()) {
                    io::adbms::read::secondaryCellVoltage();
                    cell_step_state = CellStepState::IDLE;
                }
                break;
        }
    }

    void stepTherms() {
        switch (therm_step_state) {
            case ThermStepState::IDLE:
                // Restarting on entry (not on completion) makes this a true period: a pass that
                // overruns THERM_PERIOD_MS just re-arms immediately instead of adding its own
                // runtime to the gap.
                if (therm_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED) {
                    therm_timer.restart();
                    therm_step_state = ThermStepState::MUX_0_7_START;
                }
                break;
            case ThermStepState::MUX_0_7_START:
                if (app::pack::config::setSegmentConfig(ThermMux::MUX_0_7)){
                    if (app::pack::config::checkSegmentConfig) 
                         if (io::adbms::command::startAuxAdc()) 
                            therm_step_state = ThermStepState::MUX_0_7_READ;
                break;
            case ThermStepState::MUX_0_7_READ:
                if (io::adbms::command::pollAuxAdc()) {
                    if (io::adbms::read::cellTemperature(ThermMux::MUX_0_7));
                        auto time = io::time::getCurrentMs(); //idk if u put here
                        therm_step_state = ThermStepState::MUX_8_13_START;
                }
                break;
            case ThermStepState::MUX_8_13_START:
                if (app::pack::config::setSegmentConfig(ThermMux::MUX_8_13)){
                    if (app::pack::config::checkSegmentConfig) 
                         if (io::adbms::command::startAuxAdc()) 
                            therm_step_state = ThermStepState::MUX_8_13_READ;
                break;
            case ThermStepState::MUX_8_13_READ:
                if (io::adbms::command::pollAuxAdc()) {
                    if (io::adbms::read::cellTemperature(ThermMux::MUX_8_13));
                        auto time = io::time::getCurrentMs(); //idk if u put here
                        therm_step_state = ThermStepState::IDLE;
                }
                break;
        }
    }

    // IS THERE A NEED TO CHECK CONFIG??
    // prob will get rid of 
    // void stepConfig() {
    //     switch (config_step_state) {
    //         case ConfigStepState::IDLE:
    //             break;
    //         case ConfigStepState::WRITE:
    //             auto a = app::pack::config::writePwmConfig();
    //             auto b = app::pack::config::writeSegmentConfig();
    //             if (a && b) config_step_state = ConfigStepState::CHECK;
    //             break;
    //         case ConfigStepState::CHECK:
    //             auto a = app::pack::config::checkPwmConfig();
    //             auto b = app::pack::config::checkSegmentConfig();
    //             if (!a || !b) {
    //                 config_step_state = ConfigStepState::WRITE;
    //             } else {
    //                 config_modified = false;
    //                 config_step_state = ConfigStepState::IDLE;
    //             }
    //             break;
    //     }

    //     //need to broadcast mute status
    //     //need to broadcast pwm value
    // }

    void stepADBMS6830Flags() {
        switch (flags_step_state) {
            case FlagsStepState::IDLE:
                break;
            case FlagsStepState::READ_STATUS:
                break;
            case FlagsStepState::CLEAR_STATUS:
                break;
        }
    }
}

namespace app::pack::sequence {
    void init() {
        local = Snapshot{};

        cell_step_state   = CellStepState::IDLE;
        therm_step_state  = ThermStepState::IDLE;
        owc_step_state    = OwcStepState::IDLE;
        config_step_state = ConfigStepState::IDLE;
        flags_step_state  = FlagsStepState::IDLE;

        // Required, not just tidy: on TARGET_TEST the Timer ctor leaves the state IDLE, and
        // updateAndGetState() only ever promotes RUNNING -> EXPIRED. Without this the therm
        // machine would sit in IDLE forever in unit tests.
        therm_timer.restart();
    }

    void tick() {
        // stepTractiveCurrent();
        // stepTractiveVoltage();
        stepCells();
        stepTherms();
        stepCellOpenWire();
        stepConfig();
        stepADBMS6830Flags();
    }
}
