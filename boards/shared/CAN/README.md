## Overview
This repository contains our CAN library for STM32 F3 and F0 microcontrollers. The goal is to abstract away low-level details, provide a set of easy-to-use CAN helper functions, and enforce consistency for CAN communication across every PCB on the vehicle.

The bxCAN controller has three transmit mailboxes, which means it can only hold three Tx messages at any given time. If the user attemps to transmit a message while all three transmit mailboxes are occupied, we store this message in a **software** FIFO queue. Messages in this FIFO queue will be automatically de-queued and transmitted when any of the transmit mailboxes becomes available. This FIFO queue has a fixed size of 20 levels deep, which is more-or-less arbitrary but it should be sufficient in most cases. If the FIFO queue were to overflow, a CAN message will be transmitted. If we ever see this CAN message in the data logger, we can increase the FIFO queue size accordingly.

## How to Use This Library
1. Add the board name to **Preprocessor Symbols** inside Keil. This will determine which CAN filters are activated.
<img src="https://user-images.githubusercontent.com/16970019/49477422-c5967e80-f7d1-11e8-95ee-74e2a36f4b73.png" width="50%" height="50%">

2. Add `SharedCan_StartCanInInterruptMode(&hcan)` to `MX_CAN_INIT()` 
```
// main.c
static void MX_CAN_INIT(void)
{
  /* Omitted some code */
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
  SharedCan_StartCanInInterruptMode(&hcan);
  /* USER CODE END CAN_Init 2 */
}
```
3. Include `SharedCan.h` in main.c`
```
// main.c
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SharedCan.h"
/* USER CODE END Includes */

```

4. Add `shared\STM32-CAN-Code\Inc` to Include Paths inside Keil (Note: Your relative path may look different than mine)
<img src="https://user-images.githubusercontent.com/16970019/49481076-14e2ac00-f7de-11e8-9efe-4947a8eb35c7.png" width="50%" height="50%">

5. Create `Can.c` for your Keil Project, in which you can write your own `Can_RxCommonCallback()` to handle incoming CAN messages. A skeleton code has been provided below for `Can.c`.

```
// Can.c
#include "SharedCan.h"

void Can_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t rx_fifo)
{
    struct CanRxMsg rx_msg;

    HAL_CAN_GetRxMessage(hcan, rx_fifo, &rx_msg.rx_header, &rx_msg.data[0]);

    switch(rx_msg.rx_header.StdId)
    {
        case DEMO_TOGGLE_GPIO1_STDID:
            break;
    }
}
```
5. Use `SHAREDCAN_PACK_CANTX_MSG()` to prepare a CAN message, and then use `SharedCan_TransmitDataCan()` to transmit the message.
```
SHAREDCAN_PACK_CANTXMSG(message,
                        PCB_STARTUP_STDID,
                        PCB_STARTUP_DLC,
                        PCB_STARTUP_DATA,
                        CanMsgs_fsm_startup_pack);
SharedCan_TransmitDataCan(message);
```

## CAN Filters
As previously mentioned, the CAN filters activated are dependent on the given board name in Preprocessor Symbol. Check `SharedCan.c ` and `SharedCan.h` for the accepted range of CAN IDs for each board name.

For example,
```
// SharedCan.c
#ifdef PDM
static CanMaskFilterConfig_Struct mask_filters[2] =
{
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_DCM, MASKMODE_16BIT_MASK_DCM),
    INIT_MASK_FILTER(MASKMODE_16BIT_ID_SHARED, MASKMODE_16BIT_MASK_SHARED)
};
```

This tells us for `PDM`, we have activated two CAN filters - the **DCM filter** and the **Shared filter**. We can then check `SharedCan.h` to see which CAN IDs each of these two filters will accept:

```
// SharedCan.h
/** DCM filter - CAN ID Range: 0x20 - 0x3F, RTR: Data Frame, IDE: Standard ID */
```

```
// SharedCan.h
/** Shared filter - CAN ID Range: 0x80 - 0x9F, RTR: Data Frame, IDE: Standard ID */
```

The comments tell us that the **DCM filter** accept CAN ID `0x20 - 0x3F` and the **Shared Filter** accept CAN ID `0x80 to 0x9F`. Or in other words, PDM is set up to accept any incoming CAN messages with CAN ID matching `0x20 - 0x3F` and `0x80 - 0x9F`.
