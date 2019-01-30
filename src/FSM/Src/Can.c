#include "SharedCan.h"
#include "CanMsgs.h"

void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;
    SharedCan_ReceiveDataCan(hcan, rx_fifo, &rx_msg);

    SHAREDCAN_CAN_MSG_TO_CALLBACK_MAPPING(rx_msg.rx_header.StdId, rx_msg.data){
        // NOTE: This is where callbacks are specified, like:
        // SHAREDCAN_IF_STDID_IS(fsm_errors, fsm_errors_callback);
        // (TODO: Delete this comment when there is an actual example) 
    };

}
