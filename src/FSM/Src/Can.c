#include "SharedCan.h"

void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;
    SharedCan_ReceiveDataCan(hcan, rx_fifo, &rx_msg);

    // NOTE: This is just here to force this function to be compiled in
    // for debugging purposes, feel free to delete it when implementing
    // this function.
    volatile int a = 2;
}
