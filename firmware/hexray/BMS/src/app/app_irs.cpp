#include "io_irs.hpp"
#include "app_irs.hpp"

extern "C" {
    #include "app_canTx.h"
    #include "app_canUtils.h"
}



namespace app::irs{
    
    void broadcast(){
    const ContactorState negative_closed  = io::irs::negativeState();
    const ContactorState positive_closed  = io::irs::positiveState();
    const ContactorState precharge_closed = io::irs::prechargeState();
    
    app_canTx_BMS_IrNegative_set(negative_closed);
    app_canTx_BMS_IrPositive_set(positive_closed);
    app_canTx_BMS_PrechargeRelay_set(precharge_closed);
    }
}