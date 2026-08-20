#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "io_time.hpp"


namespace {
    app::pack::Snapshot local{};

    CellStepState   cell_step_state   = CellStepState::IDLE;
    ThermStepState  therm_step_state  = ThermStepState::IDLE;
    OwcStepState    owc_step_state    = OwcStepState::IDLE;
    ConfigStepState config_step_state = ConfigStepState::IDLE;
    FlagsStepState  flags_step_state  = FlagsStepState::IDLE;

    void stepCells() {
        switch (cell_step_state) {
            case CellStepState::IDLE:
                break;
            case CellStepState::START_REDUNDANT:
                auto a = io::adbms::command::startCellsAdc(true);
                auto b = io::adbms::command::startSecondaryCells();
                if (a && b) cell_step_state = CellStepState::READ_VOLTAGES;
                break;
            case CellStepState::START_NO_REDUNDANT:
                auto a = io::adbms::command::startCellsAdc(false);
                auto b = io::adbms::command::startSecondaryCells();
                if (a && b) cell_step_state = CellStepState::READ_VOLTAGES;
                break;
            case CellStepState::READ_VOLTAGES:
                // need to add proper path for error handling/retry
                io::adbms::command::snap();
                auto time = io::time::getCurrentMs();
                io::adbms::read::cellVoltage();
                io::adbms::read::cellVoltageReduancy();
                io::adbms::command::unsnap();
                break;
            case CellStepState::RECOVER:
                break;
        }
    }

    void stepTherms() {
        switch (therm_step_state) {
            case ThermStepState::IDLE:
                break;
            case ThermStepState::MUX_0_7_START:
                break;
            case ThermStepState::MUX_0_7_READ:
                break;
            case ThermStepState::MUX_8_13_START:
                break;
            case ThermStepState::MUX_8_13_READ:
                break;
        }
    }

    void stepCellOpenWire() {
        switch (owc_step_state) {
            case OwcStepState::IDLE:
                break;
            case OwcStepState::START_SADC_OW_ODD:
                break;
            case OwcStepState::READ_SADC_OW_ODD:
                break;
            case OwcStepState::START_SADC_OW_EVEN:
                break;
            case OwcStepState::READ_SADC_OW_EVEN:
                break;
        }
    }

    void stepConfig() {
        switch (config_step_state) {
            case ConfigStepState::IDLE:
                break;
            case ConfigStepState::WRITE:
                auto a = app::pack::config::writePwmConfig();
                auto b = app::pack::config::writeSegmentConfig();
                if (a && b) config_step_state = ConfigStepState::CHECK;
                break;
            case ConfigStepState::CHECK:
                auto a = app::pack::config::checkPwmConfig();
                auto b = app::pack::config::checkSegmentConfig();
                if (!a || !b) config_step_state = ConfigStepState::WRITE;
                else config_step_state = ConfigStepState::IDLE;
                break;
        }

        //need to broadcast mute status
        //need to broadcast pwm value
    }

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
