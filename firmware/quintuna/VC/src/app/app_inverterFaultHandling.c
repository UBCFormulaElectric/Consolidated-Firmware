#include <stdlib.h>
#define ARRAY_LEN(a) (sizeof(a)/sizeof((a)[0]))

/*just a pointer that points to a function that takes in a uint32_t and a pointer so we can 
change the type passed in (only passing in the type that contains the error code) const is there to ensure the handler 
doesn't change the value of the error code
returns void -> I could maybe change it into boolean*/

typedef void (*fault_handler)(inv_type inverter);

/*enum of all the inverters*/
typedef enum {INVFR, INVFL, INVRR, INVRL} inv_type;

/*struct for creating a map and key style */
typedef struct{
const uint32_t key;
fault_handler handler;
}FunctionMap;

void hardware_reset (inv_type inverter){           
    switch (inverter) {
    case INVFR:
        /* cast then act on FR */
        app_canTx_VC_INVFRbErrorReset_set(true);
        app_warningHandling_inverterReset
        break;
    case INVFL:
        app_canTx_VC_INVFLbErrorReset_set(true);
        break;
    case INVRR:
        app_canTx_VC_INVRRbErrorReset_set(true);
        break;
    case INVRL:
        app_canTx_VC_INVRLbErrorReset_set(true);
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
fault_handler invfr_error_handling (){
    for (size_t i = 0; i < ARRAY_LEN(MAP); ++i){
        if MAP[i]->key == app_canRx_INVFR_ErrorInfo_get(){
        return MAP[i]->handler(inv_type.INV_FR);
        }
        else if MAP[i]->key == app_canRx_INVFL_ErrorInfo_get(){
        return MAP[i]->handler(inv_type.INV_FL);
        }
        else if MAP[i]->key == app_canRx_INVRR_ErrorInfo_get(){
        return MAP[i]->handler(inv_type.INV_RR);
        }
        else if MAP[i]->key == app_canRx_INVRL_ErrorInfo_get() {
        return MAP[i]->handler(inv_type.INV_RL);
        }
        else{
            return MAP[i].handler; 
        }
    }
}
