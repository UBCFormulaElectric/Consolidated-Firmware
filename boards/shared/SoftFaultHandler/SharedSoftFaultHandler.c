#include "SharedSoftFaultHandler.h"

#include "SharedCan.h"
#include "App_CanMsgsTx.h"

#define TEST

void SharedSoftFaultHandler_disableInterruptsAndSendOutErrorMsg(){
    // TODO: Send out CAN error message here

    __disable_irq();

    while(true){};
}
