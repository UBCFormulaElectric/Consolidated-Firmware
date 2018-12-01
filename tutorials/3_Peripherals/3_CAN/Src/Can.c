#include "Can.h"
#include "SharedCan.h"

void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;

    HAL_CAN_GetRxMessage(hcan, rx_fifo, &rx_msg.rx_header, &rx_msg.data[0]);
    
    switch(rx_msg.rx_header.StdId)
    {
        //case can_headers[DEMO_2_UINT32_NUCLEO_TX].StdId:
        case 0x0:
            SharedCan_TransmitDataCan(DEMO_2_UINT32_NUCLEO_RX_STDID, DEMO_2_UINT32_NUCLEO_RX_DLC, &rx_msg.data[0]);
            break;
        default:
            break;
    }

}
void Can_TxCommonCallback(CAN_HandleTypeDef *hcan)
{
}
