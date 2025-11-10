#include "app_suspension.hpp"
extern "C"
{
#include "app_canTx.h"
#include "app_canAlerts.h"
}

#include "io_suspension.hpp"

namespace app::suspension{
    void broadcast(void)
    {
        const float left_travel  = io::suspension::getLeftTravel();
        const float right_travel = io::suspension::getRightTravel();
        const bool  left_ocsc    = io::suspension::leftSensorOCSC();
        const bool  right_ocsc   = io::suspension::rightSensorOCSC();
        app_canTx_FSM_LeftSuspensionTravel_set(left_travel);
        app_canTx_FSM_RightSuspensionTravel_set(right_travel);
        app_canAlerts_FSM_Info_LeftSuspensionOCSC_set(left_ocsc);
        app_canAlerts_FSM_Info_RightSuspensionOCSC_set(right_ocsc);
    }
}
