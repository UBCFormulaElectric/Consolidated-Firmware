#include "app_suspension.hpp"
#include "io_suspension.hpp"
extern "C"
{
    #include "app_canTx.h"
    #include "app_canAlerts.h"
}

namespace app::suspension
{
    void broadcast()
    {
        const float left_travel = io::suspension::getRLTravel();
        const float right_travel = io::suspension::getRRTravel();
        app_canAlerts_RSM_Info_RearRightSuspension_OCSC_set(io::suspension::RR_OCSC());
        app_canAlerts_RSM_Info_RearLeftSuspension_OCSC_set(io::suspension::RL_OCSC());
        app_canTx_RSM_RearLeftSuspensionTravel_set(left_travel);
        app_canTx_RSM_RearRightSuspensionTravel_set(right_travel);
    }
}