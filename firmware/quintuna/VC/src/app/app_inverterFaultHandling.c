typedef int make_iso_compilers_happy;

#if 0
#include <stdlib.h>
#include "app_warningHandling.c"
#define ARRAY_LEN(a) (sizeof(a)/sizeof((a)[0]))

/*just a pointer that points to a function that takes in a uint32_t and a pointer so we can 
change the type passed in (only passing in the type that contains the error code) const is there to ensure the handler 
doesn't change the value of the error code
returns void -> I could maybe change it into boolean*/

typedef void (*fault_handler)(InverterConfig inverter_reset_handle);

/*struct for creating a map and key style */
typedef struct{
const uint32_t key;
fault_handler handler;
}FunctionMap;

void hardware_reset (InverterConfig inverter_reset_handle){           
    switch (inverter) {
    case INVERTER_FR:
        /* cast then act on FR */
        app_canTx_VC_INVFRbErrorReset_set(true);
        app_canAlerts_VC_Warning_FrontRightInverterFault_set(true);
        inverter_reset_handle[INVERTER_FR].can_invOn(false);
        inverter_reset_handle[INVERTER_FR].can_dcOn(false);
        inverter_reset_handle[INVERTER_FR].can_enable_inv(false);
        inverter_reset_handle[INVERTER_FR].error_reset(true);
        break;

    case INVERTER_FL:
        app_canTx_VC_INVFLbErrorReset_set(true);
        app_canAlerts_VC_Warning_FrontLeftInverterFault_set(true);
        inverter_reset_handle[INVERTER_FL].can_invOn(false);
        inverter_reset_handle[INVERTER_FL].can_dcOn(false);
        inverter_reset_handle[INVERTER_FL].can_enable_inv(false);
        inverter_reset_handle[INVERTER_FL].error_reset(true);
        break;

    case INVERTER_RR:
        app_canTx_VC_INVRRbErrorReset_set(true);
        app_canAlerts_VC_Warning_RearRightInverterFault_set(true);
        inverter_reset_handle[INVERTER_RR].can_invOn(false);
        inverter_reset_handle[INVERTER_RR].can_dcOn(false);
        inverter_reset_handle[INVERTER_RR].can_enable_inv(false);
        inverter_reset_handle[INVERTER_RR].error_reset(true);
        break;

    case INVERTER_RL:
        app_canTx_VC_INVRLbErrorReset_set(true);
        app_canAlerts_VC_Warning_RearLeftInverterFault_set(true);
        inverter_reset_handle[INVERTER_RL].can_invOn(false);
        inverter_reset_handle[INVERTER_RL].can_dcOn(false);
        inverter_reset_handle[INVERTER_RL].can_enable_inv(false);
        inverter_reset_handle[INVERTER_RL].error_reset(true);
        break;
    }
}

static const FunctionMap MAP[] = {
    {259u, hardware_reset},
    {1342u, hardware_reset},
    {2311u, hardware_reset},
};

bool app_warningHandling_boardWarningCheck(void)
{
    return app_canAlerts_AnyBoardHasWarning();
}

/*    if (app_warningHandling_boardWarningCheck == true){} wrap the fault_handler with this func later*/
fault_handler inv_error_handling(){
    for (size_t i = 0; i < ARRAY_LEN(MAP); ++i){
        if MAP[i]->key == app_canRx_INVFR_ErrorInfo_get(){
        return MAP[i]->handler(InverterConfig.INVERTER_FR);
        }
        else if MAP[i]->key == app_canRx_INVFL_ErrorInfo_get(){
        return MAP[i]->handler(InverterConfig.INVERTER_FL);
        }
        else if MAP[i]->key == app_canRx_INVRR_ErrorInfo_get(){
        return MAP[i]->handler(InverterConfig.INVERTER_RR);
        }
        else if MAP[i]->key == app_canRx_INVRL_ErrorInfo_get() {
        return MAP[i]->handler(InverterConfig.INVERTER_RL);
        }
        else{
            return MAP[i].handler; 
        }
    }
}
#endif