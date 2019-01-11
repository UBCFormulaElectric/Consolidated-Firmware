#include "SharedCan.h"
#include "CanMsgs.h"

void symbol1_callback(CanMsgs_symbol1_t msg) {
    // TODO: Do something with the message here
}

void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;
    SharedCan_ReceiveDataCan(hcan, rx_fifo, &rx_msg);

    SHAREDCAN_PASS_CAN_MSG_TO_CALLBACK(rx_msg.rx_header.StdId, rx_msg.data){
        SHAREDCAN_CAN_MSG_CALLBACK(symbol1, symbol1_callback);
    }
}
