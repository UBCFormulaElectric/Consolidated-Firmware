#include "SharedSoftFaultHandler.h"

#include "SharedCan.h"
#include "App_CanMsgsTx.h"

void SharedSoftFaultHandler_disableInterruptsAndSendOutErrorMsg(){
    // Send out the CAN error message. We force-send it here because we
    // want to make sure it gets out, even if the CAN TX queue is full
    // TODO: Actually force-send out the CAN error msg

    // Disable interrupts. If this board hs encountered a fault, we don't want it to
    // do anything else, and this is a quick way to enforce that.
    __disable_irq();

    // We don't want any more code running at this point, so just block forever
    while(true){};
}
