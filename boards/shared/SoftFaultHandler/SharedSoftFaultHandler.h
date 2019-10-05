#ifndef SHARED_SOFT_FAULTHANDLER_H
#define SHARED_SOFT_FAULTHANDLER_H

#include "BoardSpecificDefines.h"

#include "CanMsgs.h"

// TODO: general doc comment here?

#ifndef CAN_ERROR_MSG_NAME
#error "Please define CAN_ERROR_MSG_NAME as the name of the CAN error msg for this board in order to use the `SharedSoftFaultHandler` library"
#endif

/**
 * Trigger a soft fault with the given error
 *
 * This will:
 * - set the given error
 * - transmit the error message for this board over CAN
 * - disable interrupts
 * - go into an infinite loop
 *
 * @param ERROR The name of the error to trigger
 */
#define SharedSoftFaultHandler_Error_Handler(ERROR) \
App_CanMsgsTx_GetCanTxPayloads()->CAN_ERROR_MSG_NAME.ERROR = true; \
SharedSoftFaultHandler_disableInterruptsAndSendOutErrorMsg()


/**
 * Disables interrupts and sends out the error message for this board, then goes
 * into an infinite loop
 */
void SharedSoftFaultHandler_disableInterruptsAndSendOutErrorMsg();


#endif //SHARED_SOFT_FAULTHANDLER_H
