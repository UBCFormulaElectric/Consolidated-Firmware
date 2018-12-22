// Can.c
#include "SharedCan.h"
#include "CanTest.h"
#include "main.h"

/**
 * @brief override the CAN callback method so we can choose how to 
 * handle new CAN signals / what to do with them
 */
void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{

    uint8_t test_can_data[BMS_ERROR_DLC] = {0x9, 0x1, 0x2, 0x3};
    SharedCan_TransmitDataCan(BMS_ERROR_STDID, BMS_ERROR_DLC, &test_can_data[0]);

    //HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    //HAL_Delay(1000);
    //HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

    CanRxMsg_Struct rx_msg;

    HAL_CAN_GetRxMessage(hcan, rx_fifo, &rx_msg.rx_header, &rx_msg.data[0]);

    CanTest_handleCanMsg(rx_msg);
}
