#include "app_irs.h"
#include "app_canTx.h"
#include "io_irs.h"

void app_irs_broadcast()
{
    const ContactorsState negative_closed  = io_irs_negativeState();
    const ContactorsState positive_closed  = io_irs_positiveState();
    const ContactorsState precharge_closed = io_irs_prechargeState();
    app_canTx_BMS_IrNegative_set((ContactorState)negative_closed);
    app_canTx_BMS_IrPositive_set((ContactorState)positive_closed);
    app_canTx_BMS_PrechargeRelay_set((ContactorState)precharge_closed);
}
