#include "app_irs.h"
#include "app_canTx.h"
#include "io_irs.h"
#include "app_canUtils.h"

void app_irs_broadcast()
{
    const ContactorState negative_closed  = io_irs_negativeState();
    const ContactorState positive_closed  = io_irs_positiveState();
    const ContactorState precharge_closed = io_irs_prechargeState();
    app_canTx_BMS_IrNegative_set(negative_closed);
    app_canTx_BMS_IrPositive_set(positive_closed);
    app_canTx_BMS_PrechargeRelay_set(precharge_closed);
}
