#include "Can.h"
#include "SharedCan.h"

void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    CanRxMsg_Struct rx_msg;

    HAL_CAN_GetRxMessage(hcan, rx_fifo, &rx_msg.rx_header, &rx_msg.data[0]);

    switch(rx_msg.rx_header.StdId)
    {
        case DEMO_1_DOUBLE_NUCLEO_TX_STDID:
            SharedCan_TransmitDataCan(DEMO_1_DOUBLE_NUCLEO_RX_STDID,
                DEMO_1_DOUBLE_NUCLEO_RX_STDID, &rx_msg.data[0]);
            break;
        case DEMO_2_FLOAT_NUCLEO_TX_STDID:
            SharedCan_TransmitDataCan(DEMO_2_FLOAT_NUCLEO_RX_STDID,
                DEMO_2_FLOAT_NUCLEO_RX_DLC, &rx_msg.data[0]);
            break;
        case DEMO_2_UINT32_NUCLEO_TX_STDID:
            SharedCan_TransmitDataCan(DEMO_2_UINT32_NUCLEO_RX_STDID,
                DEMO_2_UINT32_NUCLEO_RX_DLC, &rx_msg.data[0]);
            break;
        case DEMO_4_UINT16_NUCLEO_TX_STDID:
            SharedCan_TransmitDataCan(DEMO_4_UINT16_NUCLEO_RX_STDID,
                DEMO_4_UINT16_NUCLEO_RX_DLC, &rx_msg.data[0]);
            break;
        case DEMO_8_UINT8_NUCLEO_TX_STDID:
            SharedCan_TransmitDataCan(DEMO_8_UINT8_NUCLEO_RX_STDID,
                DEMO_8_UINT8_NUCLEO_RX_DLC, &rx_msg.data[0]);
            break;
        case DEMO_2_INT32_NUCLEO_TX_STDID:
            SharedCan_TransmitDataCan(DEMO_2_INT32_NUCLEO_RX_STDID,
                DEMO_2_INT32_NUCLEO_RX_DLC, &rx_msg.data[0]);
            break;
        default:
            break;
    }
}

// This interrupt is generated when any mailbox is empty, so
// we should use it to dequeue CAN tx queue 
void Can_TxCommonCallback(CAN_HandleTypeDef *hcan)
{
}
