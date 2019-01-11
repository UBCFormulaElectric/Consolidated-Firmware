#include "SharedCan.h"
#include "CanMsgs.h"

void symbol1_handler(CanMsgs_symbol_t msg){

}

void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;
    SharedCan_ReceiveDataCan(hcan, rx_fifo, &rx_msg);

    switch(rx_msg.rx_header.StdId) {
        case CANMSGS_symbol1_FRAME_ID:
            CanMsgs_symbol1_t msg;
            if (CanMsgs_symbol1_unpack(&msg, rx_msg.data, rx_msg.rx_header.dlc) != 0) {
                Error_Handler();
            }
            symbol1_handler(msg);
            break;

        case CANMSGS_SYMBOL2_FRAME_ID:
            break;
    }

    // =========================================

    START_CAN_MSG_CALLBACK_LIST();

    DEFINE_CAN_MESSAGE_CALLBACK(symbol1, symbol1_handler);

    END_CAN_MSG_CALLBACK_LIST();
}