#pragma once
#include "io_watchdog.h"

namespace io{
    
    namespace watchdog{

        void watchdogInit(){
            //renabling watchdog
            __HAL_DBGMCU_FREEZE_IWDG();
            hw_hardFaultHandler_init();
        }

        void registerWatchdogInstance (static const TickType_t period_ms, int RTOS_TASK){
            hw::watchdog::WatchdogInstance watchdog{ RTOS_TASK, period_ms };
            hw::watchdog::monitor::registerWatchdogInstance(&watchdog);
        }

        void checkIn(hw::watchdog::WatchdogInstance *watchdog){
            watchdog.checkIn();
        }        

        void checkForTimeouts(){
            hw::watchdog::monitor::checkForTimeouts();
        }

    }

} //io::watchdog namespace