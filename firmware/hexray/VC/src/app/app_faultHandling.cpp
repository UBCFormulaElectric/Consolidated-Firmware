#include "app_faultHandling.hpp"
// #include <app_canRx.hpp>
// #include <app_canUtils.h>
// #include <app_canAlerts.h>
// #include "app_stateMachine.h"
// #include "states/app_states.h"

#define CONTACTOR_STATE_CLOSED 1

namespace vc::app::fault_handling{
    bool app_faultHandling_air_minus_closed(void)
    {
        // return (CONTACTOR_STATE_CLOSED == app_canRx_BMS_IrNegative_get());
        //TODO: CanRx functions are not implemented build sys yet
        return (CONTACTOR_STATE_CLOSED == 1);
    }

    bool app_faultHandling_fsm_bms_HeartbeartChecks(void)
    {
        //TODO: CanAlerts not implemented in cpp yet
        //return app_canAlerts_BoardHasFault(VC_NODE);
        return 1;
    }

}  //namespace vc::app::fault_handling