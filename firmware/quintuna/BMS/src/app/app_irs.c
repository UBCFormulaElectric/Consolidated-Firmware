#include "app_irs.h"
#include "app_canTx.h"
#include "io_irs.h"

void app_irs_broadcast()
{
    const IRsState negative_closed  = io_irs_negativeState() == IRS_CLOSED;
    const IRsState positive_closed  = io_irs_positiveState() == IRS_CLOSED;
    const IRsState precharge_closed = io_irs_prechargeState() == IRS_CLOSED;
    app_canTx_BMS_IrNegative_set((ContactorState)negative_closed);
    app_canTx_BMS_IrPositive_set((ContactorState)positive_closed);
    app_canTx_BMS_PrechargeRelay_set((ContactorState)precharge_closed);
}
