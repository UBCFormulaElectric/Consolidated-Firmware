//need to also add something for balancing (PWM bits)
    //still have to modify this state machine
    void stepCells() {
        switch (cell_state) {
            case CellState::IDLE: {
                const bool volts_due = cell_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED;
                const bool owc_due   = owc_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED;

                if (volts_due) {
                    cell_timer.restart();
                    const bool balancing = app::pack::requests::get().kind == app::pack::RequestKind::UNMUTE_BALANCING;
                    if (balancing) cell_state = red_on ? CellState::START_BOTH_NO_REDUNDANT : CellState::READ_BOTH_VOLTAGES;
                    else cell_state = red_on ? CellState::READ_BOTH_VOLTAGES : CellState::START_BOTH_REDUNDANT;
                }

                if (owc_due)
                    cell_state = red_on ? CellState::START_BOTH_NO_REDUNDANT : CellState::READ_BOTH_VOLTAGES;
                break;
            }
            case CellState::START_BOTH_REDUNDANT:
                if (io::adbms::command::startCADC(true) && io::adbms::command::startSADC(OwcParity::NONE)) {
                    owc_par = OwcParity::NONE;
                    red_on = true;
                    cell_state = CellState::READ_VOLTAGES;
                }
                break;
            case CellState::START_BOTH_NO_REDUNDANT:
            try_par = NONE

            if (ow_par == NONE) {
                try_par = EVEN
            } if (ow_par == EVEN)
                if (io::adbms::command::startCADC(false) && io::adbms::command::startSADC(ow_par)) {
                    red_on = false;
                    ow_par = try_par;
                    cell_state = CellState::READ_VOLTAGES;
                }
                break;
            case CellState::READ_VOLTAGES:
                // need to add proper path for error handling/retry
                io::adbms::command::snap();
                auto time = io::time::getCurrentMs();
                io::adbms::read::CADC(red_on);
                io::adbms::read::SADC(ow_par);
                io::adbms::command::unsnap();
                //figure out proper path 
                cell_state = CellState::IDLE;
                break;
            
        }
    }

    void stepTherms() {
        switch (therm_state) {
            case ThermState::IDLE:
                if (therm_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED) {
                    therm_timer.restart();
                    therm_state = ThermState::MUX_0_7_START;
                }
                break;
            case ThermState::MUX_0_7_START:
                if (app::pack::config::setSegmentConfig(ThermMux::MUX_0_7)){
                    if (app::pack::config::checkSegmentConfig) 
                         if (io::adbms::command::startAuxAdc()) 
                            therm_state = ThermState::MUX_0_7_READ;
                break;
            case ThermState::MUX_0_7_READ:
                if (io::adbms::command::pollAuxAdc()) {
                    if (io::adbms::read::cellTemperature(ThermMux::MUX_0_7));
                        auto time = io::time::getCurrentMs(); //idk if u put here
                        therm_state = ThermState::MUX_8_13_START;
                }
                io::adbms::read::VM_VP();
                break;
            case ThermState::MUX_8_13_START:
                if (app::pack::config::setSegmentConfig(ThermMux::MUX_8_13)){
                    if (app::pack::config::checkSegmentConfig) 
                         if (io::adbms::command::startAuxAdc()) 
                            therm_state = ThermState::MUX_8_13_READ;
                break;
            case ThermState::MUX_8_13_READ:
                if (io::adbms::command::pollAuxAdc()) {
                    if (io::adbms::read::cellTemperature(ThermMux::MUX_8_13));
                        auto time = io::time::getCurrentMs(); //idk if u put here
                        therm_state = ThermState::IDLE;
                }
                io::adbms::read::VM_VP();
                break;
        }
    }

    void stepADBMS6830Diag() {
        switch (diag_state) {
            case DiagState::IDLE:
                if (diag_timer.updateAndGetState() == app::Timer::TimerState::EXPIRED) {
                    diag_timer.restart();
                    diag_state = DiagState::READ_STATUS;
                }
                break;
            case DiagState::READ_STATUS:
                
                io::adbms::read::serialNumber();
                io::adbms::read::statusRegs();
                //read VREF2 and ITMP (RDSTATA)
                //read VD and VA and VRES (RDSTATB)
                //read CSFLT, VA_OV/UV, VD_OV/UV, CED, CMED, SED, SMED , VDE, VDEL, SLEEP, THSD, TMODCHK, OSCCHK (RDSTATC)
                //read UV and OV and OC_CNTR (RDSTATD)
        
                //for CMED|| SMED || SLEEP || THSD || OSCCHK || TMODCHK  => soft reset => write config reg
                //

                break;
            case DiagState::RESET:
                io::adbms::command::softReset();
                app::pack::config::writeSegmentConfig();
                app::pack::config::writePwmConfig();
                if (app::pack::config::checkSegmentConfig && app::pack::config::checkPwmConfig) diag_state = DiagState::IDLE;
                break;
        }
    }