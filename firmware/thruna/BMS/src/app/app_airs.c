#include "app_airs.h"
#include "App_CanTx.h"
#include "io_airs.h"

void app_airs_broadcast()
{
    const bool negative_closed  = io_airs_isNegativeClosed();
    const bool positive_closed  = io_airs_isPositiveClosed();
    const bool precharge_closed = io_airs_isPrechargeClosed();

    App_CanTx_BMS_AirNegative_Set(negative_closed ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
    App_CanTx_BMS_AirPositive_Set(positive_closed ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
    App_CanTx_BMS_PrechargeRelay_Set(precharge_closed ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
}
