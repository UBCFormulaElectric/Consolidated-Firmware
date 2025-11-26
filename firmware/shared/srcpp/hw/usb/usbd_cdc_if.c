#include "usbd_cdc_if.h"
#include "hw_usb.h"
#include "io_log.h"

uint8_t UserRxBufferHS[APP_RX_DATA_SIZE];
uint8_t UserTxBufferHS[APP_TX_DATA_SIZE];

extern USBD_HandleTypeDef hUsbDeviceHS;

static int8_t CDC_Init(void);
static int8_t CDC_DeInit(void);
static int8_t CDC_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t CDC_Receive(uint8_t *pbuf, uint32_t *Len);
static int8_t CDC_TransmitCplt(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);

USBD_CDC_ItfTypeDef USBD_Interface_fops_HS = { CDC_Init, CDC_DeInit, CDC_Control, CDC_Receive,
                                               CDC_TransmitCplt };

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Initializes the CDC media low layer over the USB HS IP
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_Init(void)
{
    /* USER CODE BEGIN 8 */
    /* Set Application Buffers */
    USBD_CDC_SetTxBuffer(&hUsbDeviceHS, UserTxBufferHS, 0);
    USBD_CDC_SetRxBuffer(&hUsbDeviceHS, UserRxBufferHS);
    return (USBD_OK);
    /* USER CODE END 8 */
}

/**
 * @brief  DeInitializes the CDC media low layer
 * @param  None
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_DeInit(void)
{
    /* USER CODE BEGIN 9 */
    return (USBD_OK);
    /* USER CODE END 9 */
}

/**
 * @brief  Manage the CDC class requests
 * @param  cmd: Command code
 * @param  pbuf: Buffer containing command data (request parameters)
 * @param  length: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length)
{
    switch (cmd)
    {
        case CDC_SEND_ENCAPSULATED_COMMAND:

            break;

        case CDC_GET_ENCAPSULATED_RESPONSE:

            break;

        case CDC_SET_COMM_FEATURE:

            break;

        case CDC_GET_COMM_FEATURE:

            break;

        case CDC_CLEAR_COMM_FEATURE:

            break;

        case CDC_SET_LINE_CODING:

            break;

        case CDC_GET_LINE_CODING:

            break;

        case CDC_SET_CONTROL_LINE_STATE:

            break;

        case CDC_SEND_BREAK:

            break;

        default:
            break;
    }

    return (USBD_OK);
}

/**
 * @brief Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 *
 *         @note
 *         This function will issue a NAK packet on any OUT packet received on
 *         USB endpoint until exiting this function. If you exit this function
 *         before transfer is complete on CDC interface (ie. using DMA controller)
 *         it will result in receiving more data while previous ones are still
 *         not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAILL
 */
static int8_t CDC_Receive(uint8_t *Buf, uint32_t *Len)
{
    /* USER CODE BEGIN 11 */
    USBD_CDC_SetRxBuffer(&hUsbDeviceHS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceHS);

    // Hook to hw_usb.
    if (!hw_usb_pushRxMsgToQueue(Buf, *Len))
        LOG_ERROR("Error encountered pushing USB RX Message to queue.");
    return (USBD_OK);
    /* USER CODE END 11 */
}

/**
 * @brief  Data to send over USB IN endpoint are sent over CDC interface
 *         through this function.
 * @param  Buf: Buffer of data to be sent
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
 */
uint8_t CDC_Transmit(uint8_t *Buf, uint16_t Len)
{
    uint8_t result = USBD_OK;
    /* USER CODE BEGIN 12 */
    USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef *)hUsbDeviceHS.pClassData;
    if (hcdc->TxState != 0)
    {
        return USBD_BUSY;
    }
    USBD_CDC_SetTxBuffer(&hUsbDeviceHS, Buf, Len);
    result = USBD_CDC_TransmitPacket(&hUsbDeviceHS);
    /* USER CODE END 12 */
    return result;
}

/**
 * @brief  CDC_TransmitCplt_HS
 *         Data transmitted callback
 *
 *         @note
 *         This function is IN transfer complete callback used to inform user that
 *         the submitted Data is successfully sent over USB.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_TransmitCplt(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
    uint8_t result = USBD_OK;
    /* USER CODE BEGIN 14 */
    UNUSED(Buf);
    UNUSED(Len);
    UNUSED(epnum);
    /* USER CODE END 14 */
    return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
 * @}
 */

/**
 * @}
 */
