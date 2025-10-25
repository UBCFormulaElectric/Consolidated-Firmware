#include "app_coolant.hpp"
extern "C"
{
    #include "app_rangeCheck.h"
    #include "app_canTx.h"
    #include "app_canAlerts.h"
    #include "app_signal.h"
}

#include "io_coolant.hpp"

#define MIN_FLOW_RATE_L_PER_MIN 1.0f
#define MAX_FLOW_RATE_L_PER_MIN 30.0f

#define FLOW_METER_TIME_TO_FAULT 1000U
#define FLOW_METER_TIME_TO_CLEAR 1000U

static const RangeCheck flow_rate_in_range_check = {
    .min_value = MIN_FLOW_RATE_L_PER_MIN, 
    .max_value = MAX_FLOW_RATE_L_PER_MIN, 
};
static Signal flow_in_range_signal; 


namespace app::coolant
{
    void init()
    {
        app_signal_init(&flow_in_range_signal, FLOW_METER_TIME_TO_FAULT, FLOW_METER_TIME_TO_CLEAR);
    }
    
    void broadcast()
    {
        const float flow_val = io::coolant::getFlowRate();
        const RangeCheckStatusMetaData coolant_status = app_rangeCheck_getValue(&flow_rate_in_range_check, flow_val);
        app_canTx_RSM_CoolantFlowRate_set(flow_val); 
        app_canAlerts_RSM_Info_FlowRateOutOfRange_set(coolant_status.status != VALUE_IN_RANGE);
    }
}
