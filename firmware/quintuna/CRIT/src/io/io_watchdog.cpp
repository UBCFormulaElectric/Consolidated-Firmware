#pragma once
#include "io_watchdog.h"
#include "stm32f4xx_hal.h"

namespace io{
    
    namespace watchdog{

        void watchdogInit(){
            //renabling watchdog
            SEGGER_SYSVIEW_Conf(); //not sure if this one belongs here... but I think its part of teh init
            __HAL_DBGMCU_FREEZE_IWDG();
            hw_hardFaultHandler_init();
        }

        void registerWatchdogInstance (uint16_t period_ms, int RTOS_TASK){
            hw::watchdog::WatchdogInstance watchdog{ RTOS_TASK, period_ms };
            hw::watchdog::monitor::registerWatchdogInstance(&watchdog);
        }

        void checkIn(hw::watchdog::WatchdogInstance watchdog){
            watchdog.checkIn();
        }        

        void checkForTimeouts(){
            hw::watchdog::monitor::checkForTimeouts();
        }

    }

} //io::watchdog namespace