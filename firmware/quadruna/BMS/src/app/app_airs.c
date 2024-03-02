#include "app_airs.h"
#include "app_canTx.h"
#include "io_airs.h"

void app_airs_broadcast()
{
    const bool negative_closed  = io_airs_isNegativeClosed();
    const bool positive_closed  = io_airs_isPositiveClosed();
    const bool precharge_closed = io_airs_isPrechargeClosed();

    app_canTx_BMS_AirNegative_set(negative_closed ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
    app_canTx_BMS_AirPositive_set(positive_closed ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
    app_canTx_BMS_PrechargeRelay_set(precharge_closed ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
}
