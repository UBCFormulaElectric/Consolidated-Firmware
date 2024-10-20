#include "io_watchdog.h"
#include "hw_watchdogs.h"
#include "main.h" 

namespace io{
    
    namespace watchdog{

        void watchdogInit(){
            //renabling watchdog
          //  SEGGER_SYSVIEW_Conf(); //not sure if this one belongs here... but I think its part of teh init
            __HAL_DBGMCU_FREEZE_IWDG(); //not really supposed to be here for stuff with hw 
            //hw_hardFaultHandler_init(); //new thing to contain it thats not io
        }

        void registerWatchdogInstance (uint8_t period_ms, uint8_t RTOS_TASK){
            hw::watchdog::WatchdogInstance watchdog{period_ms, RTOS_TASK};
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