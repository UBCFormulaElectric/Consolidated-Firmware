#include "hw_usb.hpp"
#include "usbd_cdc.h"
#include "usbd_def.h"

#include "main.h"
#include <FreeRTOS.h>
#include <task.h>
#include "io_log.hpp"

#include <cassert>
#include <array>

// Select between FS or HS methods.
// F4s (CM4s) only support FS (Full Speed), while H7s (CM7s) support HS (High Speed).
// This macro exposes the USB_DEVICE_HANDLER macro (hUsbDeviceFS or hUsbDeviceHS),
// and the TRANSMIT macro (CDC_Transmit_FS or CDC_Transmit_HS).
// Everything is derived from usbd_cdc_if.h.
#if defined(STM32H562xx)
#define TRANSMIT(buf, len) (CDC_Transmit_FS(buf, static_cast<uint16_t>(len)))
extern USBD_HandleTypeDef hUsbDeviceFS;
#define USB_DEVICE_HANDLER (hUsbDeviceFS)
#define DEVICE_FS 0
extern USBD_DescriptorsTypeDef CDC_Desc;
#elif defined(STM32H733xx)
#define TRANSMIT(buf, len) (CDC_Transmit_HS(buf, len))
extern USBD_HandleTypeDef      hUsbDeviceHS;
#define USB_DEVICE_HANDLER (hUsbDeviceHS)
extern USBD_DescriptorsTypeDef HS_Desc;
extern USBD_DescriptorsTypeDef CDC_Desc;
#define DEVICE_HS 1
#else
#error Either STM32H733xx or STM32F412Rx must be defined, \
        so that hw_usb knows which handlers to use.
#endif

static constexpr size_t                      APP_RX_DATA_SIZE = 2048;
static constexpr size_t                      APP_TX_DATA_SIZE = 2048;
static std::array<uint8_t, APP_RX_DATA_SIZE> UserRxBufferFS;
static std::array<uint8_t, APP_TX_DATA_SIZE> UserTxBufferFS;
/**
 * @brief  Initializes the CDC media low layer over the USB FS IP
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_Init_FS()
{
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS.data(), 0);
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS.data());
    return USBD_OK;
}
/**
 * @brief  Manage the CDC class requests
 * @param  cmd: Command code
 * @param  pbuf: Buffer containing command data (request parameters)
 * @param  length: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_Control_FS(const uint8_t cmd, uint8_t *const pbuf, const uint16_t length)
{
    UNUSED(pbuf);
    UNUSED(length);
    switch (cmd)
    {
        case CDC_SEND_ENCAPSULATED_COMMAND:
        case CDC_GET_ENCAPSULATED_RESPONSE:
        case CDC_SET_COMM_FEATURE:
        case CDC_GET_COMM_FEATURE:
        case CDC_CLEAR_COMM_FEATURE:
        /*******************************************************************************/
        /* Line Coding Structure                                                       */
        /*-----------------------------------------------------------------------------*/
        /* Offset | Field       | Size | Value  | Description                          */
        /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
        /* 4      | bCharFormat |   1  | Number | Stop bits                            */
        /*                                        0 - 1 Stop bit                       */
        /*                                        1 - 1.5 Stop bits                    */
        /*                                        2 - 2 Stop bits                      */
        /* 5      | bParityType |  1   | Number | Parity                               */
        /*                                        0 - None                             */
        /*                                        1 - Odd                              */
        /*                                        2 - Even                             */
        /*                                        3 - Mark                             */
        /*                                        4 - Space                            */
        /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
        /*******************************************************************************/
        case CDC_SET_LINE_CODING:
        case CDC_GET_LINE_CODING:
        case CDC_SET_CONTROL_LINE_STATE:
        case CDC_SEND_BREAK:
        default:
            break;
    }
    return USBD_OK;
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
static int8_t CDC_Receive_FS(uint8_t *Buf, uint32_t *Len)
{
    hw::usb::receive({ Buf, *Len });
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    return USBD_OK;
}
/**
 * @brief  Data to send over USB IN endpoint are sent over CDC interface
 *         through this function.
 * @param  Buf: Buffer of data to be sent
 * @param  len: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
 */
static uint8_t CDC_Transmit_FS(uint8_t *Buf, const uint16_t len)
{
    if (static_cast<const USBD_CDC_HandleTypeDef *>(hUsbDeviceFS.pClassData)->TxState != 0)
        return USBD_BUSY;
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, len);
    return USBD_CDC_TransmitPacket(&hUsbDeviceFS);
}
/**
 * @brief  CDC_TransmitCplt_FS, Data transmitted callback
 *         @note
 *         This function is IN transfer complete callback used to inform user that
 *         the submitted Data is successfully sent over USB.
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @param epnum
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_TransmitCplt_FS(uint8_t *const Buf, uint32_t *const Len, const uint8_t epnum)
{
    constexpr int8_t result = USBD_OK;
    const std::span  buffer(Buf, *Len);
    UNUSED(buffer);
    UNUSED(epnum);
    return result;
}
static USBD_CDC_ItfTypeDef USBD_Interface_fops_FS = {
    CDC_Init_FS, [] {return static_cast<int8_t>(USBD_OK); }, CDC_Control_FS, CDC_Receive_FS, CDC_TransmitCplt_FS,
};

extern char *USBD_PRODUCT_STRING_FS;

static constexpr uint8_t USB_SIZ_STRING_SERIAL          = 0x1AU;
static constexpr int     USBD_VID                       = 1155;
static constexpr int     USBD_LANGID_STRING             = 1033;
static constexpr int     USBD_PID_FS                    = 22336;
static char              USBD_MANUFACTURER_STRING[]     = "ubc_formula_electric";
static char              USBD_CONFIGURATION_STRING_FS[] = "CDC Config";
static char              USBD_INTERFACE_STRING_FS[]     = "CDC Interface";

static __ALIGN_BEGIN uint8_t USBD_FS_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END = {
    0x12,                 /*bLength */
    USB_DESC_TYPE_DEVICE, /*bDescriptorType*/
#if ((USBD_LPM_ENABLED == 1) || (USBD_CLASS_BOS_ENABLED == 1))
    0x01,
/*bcdUSB */ /* changed to USB version 2.01
               in order to support LPM L1 suspend
               resume test of USBCV3.0*/
#else
    0x00, /*bcdUSB */
#endif /* (USBD_LPM_ENABLED == 1) */
    0x02,
    0x02,                /*bDeviceClass*/
    0x02,                /*bDeviceSubClass*/
    0x00,                /*bDeviceProtocol*/
    USB_MAX_EP0_SIZE,    /*bMaxPacketSize*/
    LOBYTE(USBD_VID),    /*idVendor*/
    HIBYTE(USBD_VID),    /*idVendor*/
    LOBYTE(USBD_PID_FS), /*idProduct*/
    HIBYTE(USBD_PID_FS), /*idProduct*/
    0x00,                /*bcdDevice rel. 2.00*/
    0x02,
    USBD_IDX_MFC_STR,          /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,      /*Index of product string*/
    USBD_IDX_SERIAL_STR,       /*Index of serial number string*/
    USBD_MAX_NUM_CONFIGURATION /*bNumConfigurations*/
};
/** USB lang identifier descriptor. */
static __ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END = {
    USB_LEN_LANGID_STR_DESC,
    USB_DESC_TYPE_STRING,
    LOBYTE(USBD_LANGID_STRING),
    HIBYTE(USBD_LANGID_STRING),
};
static __ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END      = {};
static __ALIGN_BEGIN uint8_t USBD_StringSerial[USB_SIZ_STRING_SERIAL] __ALIGN_END = {
    USB_SIZ_STRING_SERIAL,
    USB_DESC_TYPE_STRING,
};

static uint8_t *USBD_FS_DeviceDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    *length = sizeof(USBD_FS_DeviceDesc);
    return USBD_FS_DeviceDesc;
}
static uint8_t *USBD_FS_LangIDStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    *length = sizeof(USBD_LangIDDesc);
    return USBD_LangIDDesc;
}
static uint8_t *USBD_FS_ManufacturerStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString(reinterpret_cast<uint8_t *>(USBD_MANUFACTURER_STRING), USBD_StrDesc, length);
    return USBD_StrDesc;
}
static uint8_t *USBD_FS_ProductStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString(reinterpret_cast<uint8_t *>(USBD_PRODUCT_STRING_FS), USBD_StrDesc, length);
    return USBD_StrDesc;
}
static void IntToUnicode(uint32_t value, uint8_t *pbuf, const uint8_t len)
{
    uint8_t idx = 0;

    for (idx = 0; idx < len; idx++)
    {
        if (value >> 28 < 0xA)
        {
            pbuf[2 * idx] = static_cast<uint8_t>(value >> 28) + static_cast<uint8_t>('0');
        }
        else
        {
            pbuf[2 * idx] = static_cast<uint8_t>(value >> 28) + static_cast<uint8_t>('A') - 10U;
        }

        value = value << 4;

        pbuf[2 * idx + 1] = 0;
    }
}
static constexpr uint32_t DEVICE_ID1 = UID_BASE;
static constexpr uint32_t DEVICE_ID2 = UID_BASE + 0x4U;
static constexpr uint32_t DEVICE_ID3 = UID_BASE + 0x8U;

static void Get_SerialNum()
{
    uint32_t       deviceserial0 = *reinterpret_cast<uint32_t *>(DEVICE_ID1);
    const uint32_t deviceserial1 = *reinterpret_cast<uint32_t *>(DEVICE_ID2);
    const uint32_t deviceserial2 = *reinterpret_cast<uint32_t *>(DEVICE_ID3);
    deviceserial0 += deviceserial2;
    if (deviceserial0 != 0)
    {
        IntToUnicode(deviceserial0, &USBD_StringSerial[2], 8);
        IntToUnicode(deviceserial1, &USBD_StringSerial[18], 4);
    }
}
static uint8_t *USBD_FS_SerialStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    *length = USB_SIZ_STRING_SERIAL;

    /* Update the serial number string descriptor with the data from the unique
     * ID */
    Get_SerialNum();
    /* USER CODE BEGIN USBD_FS_SerialStrDescriptor */

    /* USER CODE END USBD_FS_SerialStrDescriptor */
    return USBD_StringSerial;
}
static uint8_t *USBD_FS_ConfigStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString(reinterpret_cast<uint8_t *>(USBD_CONFIGURATION_STRING_FS), USBD_StrDesc, length);
    return USBD_StrDesc;
}
static uint8_t *USBD_FS_InterfaceStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString(reinterpret_cast<uint8_t *>(USBD_INTERFACE_STRING_FS), USBD_StrDesc, length);
    return USBD_StrDesc;
}

static USBD_DescriptorsTypeDef FS_Desc = { USBD_FS_DeviceDescriptor,
                                           USBD_FS_LangIDStrDescriptor,
                                           USBD_FS_ManufacturerStrDescriptor,
                                           USBD_FS_ProductStrDescriptor,
                                           USBD_FS_SerialStrDescriptor,
                                           USBD_FS_ConfigStrDescriptor,
                                           USBD_FS_InterfaceStrDescriptor
#if (USBD_LPM_ENABLED == 1)
                                           ,
                                           USBD_FS_USR_BOSDescriptor
#endif
};

namespace hw::usb
{
ExitCode init()
{
    if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
        return ExitCode::EXIT_CODE_ERROR;
    if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK)
        return ExitCode::EXIT_CODE_ERROR;
    if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) != USBD_OK)
        return ExitCode::EXIT_CODE_ERROR;
    if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
        return ExitCode::EXIT_CODE_ERROR;
    HAL_PWREx_EnableUSBVoltageDetector();
    return ExitCode::EXIT_CODE_OK;
}

bool checkConnection()
{
    return USB_DEVICE_HANDLER.dev_state == USBD_STATE_CONFIGURED;
}

ExitCode transmit(std::span<uint8_t> msg)
{
    if (const uint8_t status = TRANSMIT(msg.data(), msg.size()); status != USBD_OK)
    {
        // LOG_WARN("USB: Transmit handle returned %d status code instead of USBD_OK.", status);
        return ExitCode::EXIT_CODE_ERROR;
    }
    return ExitCode::EXIT_CODE_OK;
}

// Connection handling

static bool         usb_connected = false;
static TaskHandle_t usb_task      = nullptr;

void connect_callback()
{
    usb_connected = true;
    if (usb_task == nullptr)
    {
        LOG_WARN("USB: No task to notify.");
        return;
    }
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    // notify the task which is waiting for the USB connection
    vTaskNotifyGiveFromISR(usb_task, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void disconnect_callback()
{
    usb_connected = false;
}

bool connected()
{
    return usb_connected;
}

void waitForConnected()
{
    if (usb_connected)
        return;

    assert(usb_task == nullptr);
    // save which task we need to notify
    usb_task = xTaskGetCurrentTaskHandle();
    // block this task until notification comes
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    UNUSED(num_notifications);
    usb_task = nullptr;
}
} // namespace hw::usb
