#include "app_irs.h"
#include "app_canTx.h"
#include "io_irs.h"

void app_irs_broadcast()
{
    const bool negative_closed  = io_irs_isNegativeClosed();
    const bool positive_closed  = io_irs_isPositiveClosed();
    const bool precharge_closed = io_irs_isPrechargeClosed();

    app_canTx_BMS_IrNegative_set(negative_closed ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
    app_canTx_BMS_IrPositive_set(positive_closed ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
    app_canTx_BMS_PrechargeRelay_set(precharge_closed ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
}
