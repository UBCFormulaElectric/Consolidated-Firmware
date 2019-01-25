#include "SharedCan.h"
#include "CanMsgs.h"

void symbol1_callback(struct CanMsgs_fsm_errors_t msg) {
    // TODO: Do something with the message here
}

void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;
    SharedCan_ReceiveDataCan(hcan, rx_fifo, &rx_msg);

    SHAREDCAN_CAN_MSG_TO_CALLBACK_MAPPING(rx_msg.rx_header.StdId, rx_msg.data){
        SHAREDCAN_IF_STDID_IS(fsm_errors, symbol1_callback);
    };

}
