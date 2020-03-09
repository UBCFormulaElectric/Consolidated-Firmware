#include "flowMeter_driver.h"

void _Initialize_FlowMeter(FlowMeter * _FlowMeter, FlowMeterSettings * _FlowMeterSettings, FlowMeterInputSettings * _FlowMeterInputSettings){

    //Check
    shared_assert(_FlowMeter != NULL);
    shared_assert(_FlowMeterSettings != NULL);
    shared_assert(_FlowMeterInputSettings != NULL);
}

