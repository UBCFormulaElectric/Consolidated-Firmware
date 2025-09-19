#include <stdlib.h>
#define ARRAY_LEN(a) (sizeof(a)/sizeof((a)[0]))

//just a pointer that points to a function that takes in a uint32_t and a pointer so we can 
//change the type passed in (only passing in the type that contains the error code) const is there to ensure the handler 
//doesn't change the value of the error code
//returns void -> I could maybe change it into boolean 
typedef void (*fault_handler)(inv_type inverter);

//enum of all the inverters
typedef enum {INVFR, INVFL, INVRR, INVRL} inv_type;

//struct for creating a map and key style 
typedef struct{
const uint32_t key;
fault_handler handler;
}FunctionMap;


void hardware_reset (inv_type inverter){           
    switch (inverter) {
    case INVFR:
        /* cast then act only on FR */
        app_canTx_VC_INVFRbErrorReset_set(true);
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


void inverter_retry (inv_type inverter){
    app_canAlerts_VC_Info_InverterRetry_set(true); 
}

//wip
// void soft_reset (inv_type inverter){}
// const hardware_reset_list [] = {259, 1342, 2311};
// const soft_reset_list [] = {}; 

static const FunctionMap MAP[] = {
    {259u, hardware_reset},
    {1342u, hardware_reset},
    {2311u, hardware_reset},
    {475u,soft_reset},
};

bool app_warningHandling_boardWarningCheck(void)
{
    return app_canAlerts_AnyBoardHasWarning();
}

const INVFR_FRInverterInfo2_Signals* const in_msg_fr;
const INVFL_FLInverterInfo2_Signals* const in_msg_fl;
const INVRR_RRInverterInfo2_Signals* const in_msg_rr;
const INVRL_RLInverterInfo2_Signals* const in_msg_rl;
uint32_t INVFR_ErrorInfo_val = in_msg_fr->INVFR_ErrorInfo_value;
uint32_t INVFL_ErrorInfo_val = in_msg_fl->INVFL_ErrorInfo_value;
uint32_t INVRR_ErrorInfo_val = in_msg_rr->INVRR_ErrorInfo_value;
uint32_t INVRL_ErrorInfo_val = in_msg_rl->INVFRL_ErrorInfo_value;


*fault_handler invfr_error_handling (uint32_t *INVFR_ErrorInfo_val, uint32_t *INVFL_ErrorInfo_val, uint32_t *INVRR_ErrorInfo_val, uint32_t *INVRL_ErrorInfo_val){
    for (size_t i = 0; i < ARRAY_LEN(MAP); ++i){
        if MAP[i]->key == &INVFR_ErrorInfo_val {
           return MAP[i]->handler(inv_type.INV_FR);
        }
        else if MAP[i]->key == &INVFL_ErrorInfo_val {
           return MAP[i]->handler(inv_type.INV_FL);
        }
        else if MAP[i]->key == &INVRR_ErrorInfo_val {
           return MAP[i]->handler(inv_type.INV_RR);
        }
        else if MAP[i]->key == &INVRL_ErrorInfo_val {
           return MAP[i]->handler(inv_type.INV_RL);
        }
        else{
            return MAP[i].handler; 
        }
    }

}
