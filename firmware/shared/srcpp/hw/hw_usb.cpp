#include "hw_usb.hpp"
#include "usbd_cdc.h"
#include "usbd_def.h"

#include "main.h"
#include <FreeRTOS.h>
#include <task.h>
#include "io_log.hpp"

#include <cassert>
#include <array>
#include <optional>

// Select between FS or HS methods.
// F4s (CM4s) only support FS (Full Speed), while H7s (CM7s) support HS (High Speed).
// This macro exposes the USB_DEVICE_HANDLER macro (hUsbDeviceFS or hUsbDeviceHS)
// Everything is derived from usbd_cdc_if.h.
#if defined(STM32H562xx)
#define FS
#elif defined(STM32H733xx)
#define HS
#else
#error Either STM32H733xx or STM32F412Rx must be defined, \
        so that hw_usb knows which handlers to use.
#endif

#ifdef FS
static USBD_HandleTypeDef hUsbDeviceFS;
#define USB_DEVICE_HANDLER (hUsbDeviceFS)
#define DEVICE_ID 0 // DEVICE_FS
#elif defined(HS)
static USBD_HandleTypeDef hUsbDeviceHS;
#define USB_DEVICE_HANDLER (hUsbDeviceHS)
#define DEVICE_ID 1 // DEVICE_HS
#endif

static constexpr size_t                      APP_RX_DATA_SIZE = 2048;
static constexpr size_t                      APP_TX_DATA_SIZE = 2048;
static std::array<uint8_t, APP_RX_DATA_SIZE> UserRxBufferFS;
static std::array<uint8_t, APP_TX_DATA_SIZE> UserTxBufferFS;
/**
 * @brief  Initializes the CDC media low layer over the USB FS IP
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t CDC_Init()
{
    USBD_CDC_SetTxBuffer(&USB_DEVICE_HANDLER, UserTxBufferFS.data(), 0);
    USBD_CDC_SetRxBuffer(&USB_DEVICE_HANDLER, UserRxBufferFS.data());
    return USBD_OK;
}
/**
 * @brief  Manage the CDC class requests
 * @param  cmd: Command code
 * @param  pbuf: Buffer containing command data (request parameters)
 * @param  length: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
// ReSharper disable once CppParameterMayBeConstPtrOrRef
static int8_t
    CDC_Control(const uint8_t cmd, uint8_t *const pbuf, const uint16_t length) // NOLINT(*-non-const-parameter)
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
static int8_t CDC_Receive(uint8_t *Buf, uint32_t *Len) // NOLINT(*-non-const-parameter)
{
    hw::usb::receive({ Buf, *Len });
    USBD_CDC_SetRxBuffer(&USB_DEVICE_HANDLER, &Buf[0]);
    USBD_CDC_ReceivePacket(&USB_DEVICE_HANDLER);
    return USBD_OK;
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
// ReSharper disable once CppParameterMayBeConstPtrOrRef
static int8_t
    CDC_TransmitCplt(uint8_t *const Buf, uint32_t *const Len, const uint8_t epnum) // NOLINT(*-non-const-parameter)
{
    constexpr int8_t result = USBD_OK;
    const std::span  buffer(Buf, *Len);
    UNUSED(buffer);
    UNUSED(epnum);
    return result;
}
static USBD_CDC_ItfTypeDef USBD_Interface_fops = {
    CDC_Init, [] {return static_cast<int8_t>(USBD_OK); }, CDC_Control, CDC_Receive, CDC_TransmitCplt,
};

extern char USBD_PRODUCT_STRING_FS[]; // TODO make clear this must be configured per board

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

static uint8_t *USBD_DeviceDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    *length = sizeof(USBD_FS_DeviceDesc);
    return USBD_FS_DeviceDesc;
}
static uint8_t *USBD_LangIDStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    *length = sizeof(USBD_LangIDDesc);
    return USBD_LangIDDesc;
}
static uint8_t *USBD_ManufacturerStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString(reinterpret_cast<uint8_t *>(USBD_MANUFACTURER_STRING), USBD_StrDesc, length);
    return USBD_StrDesc;
}
static uint8_t *USBD_ProductStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
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
static uint8_t *USBD_SerialStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
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
static uint8_t *USBD_GetConfigurationStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString(reinterpret_cast<uint8_t *>(USBD_CONFIGURATION_STRING_FS), USBD_StrDesc, length);
    return USBD_StrDesc;
}
static uint8_t *USBD_InterfaceStrDescriptor(const USBD_SpeedTypeDef speed, uint16_t *length)
{
    UNUSED(speed);
    USBD_GetString(reinterpret_cast<uint8_t *>(USBD_INTERFACE_STRING_FS), USBD_StrDesc, length);
    return USBD_StrDesc;
}

static USBD_DescriptorsTypeDef pdesc = { USBD_DeviceDescriptor,
                                         USBD_LangIDStrDescriptor,
                                         USBD_ManufacturerStrDescriptor,
                                         USBD_ProductStrDescriptor,
                                         USBD_SerialStrDescriptor,
                                         USBD_GetConfigurationStrDescriptor,
                                         USBD_InterfaceStrDescriptor
#if (USBD_LPM_ENABLED == 1)
                                         ,
                                         USBD_FS_USR_BOSDescriptor
#endif
};

extern "C"
{
    static USBD_StatusTypeDef USBD_Get_USB_Status(const HAL_StatusTypeDef hal_status)
    {
        USBD_StatusTypeDef usb_status = USBD_OK;
        switch (hal_status)
        {
            case HAL_OK:
                usb_status = USBD_OK;
                break;
            case HAL_ERROR:
                usb_status = USBD_FAIL;
                break;
            case HAL_BUSY:
                usb_status = USBD_BUSY;
                break;
            case HAL_TIMEOUT:
            default:
                usb_status = USBD_FAIL;
                break;
        }
        return usb_status;
    }
    USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev)
    {
        /* Link USB Device <-> PCD */
#ifdef FS
        pdev->pData           = &hpcd_USB_DRD_FS;
        hpcd_USB_DRD_FS.pData = pdev;
#endif
#ifdef HS
        pdev->pData           = &hpcd_USB_OTG_HS;
        hpcd_USB_OTG_HS.pData = pdev;
#endif
        /* Initialize LL Driver */
        // MX_USB_PCD_Init(); // TODO dawg...

        /* PMA buffer configuration */
#ifdef FS
        HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x00, PCD_SNG_BUF, 0x20); /* EP0 OUT */
        HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x80, PCD_SNG_BUF, 0x60); /* EP0 IN */
        HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, CDC_OUT_EP, PCD_SNG_BUF, 0xA0);
        HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, CDC_IN_EP, PCD_SNG_BUF, 0xE0);
        HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, CDC_CMD_EP, PCD_SNG_BUF, 0x120);
#endif
#ifdef HS
        HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_HS, 0x200);
        HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 0, 0x80);
        HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 1, 0x174);
#endif
        return USBD_OK;
    }
    USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev)
    {
        return USBD_Get_USB_Status(HAL_PCD_DeInit(static_cast<PCD_HandleTypeDef *>(pdev->pData)));
    }
    USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev)
    {
        return USBD_Get_USB_Status(HAL_PCD_Start(static_cast<PCD_HandleTypeDef *>(pdev->pData)));
    }
    USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev)
    {
        return USBD_Get_USB_Status(HAL_PCD_Stop(static_cast<PCD_HandleTypeDef *>(pdev->pData)));
    }
    USBD_StatusTypeDef
        USBD_LL_OpenEP(USBD_HandleTypeDef *pdev, const uint8_t ep_addr, const uint8_t ep_type, const uint16_t ep_mps)
    {
        return USBD_Get_USB_Status(
            HAL_PCD_EP_Open(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, ep_mps, ep_type));
    }
    USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev, const uint8_t ep_addr)
    {
        return USBD_Get_USB_Status(HAL_PCD_EP_Close(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr));
    }
    USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, const uint8_t ep_addr)
    {
        return USBD_Get_USB_Status(HAL_PCD_EP_Flush(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr));
    }
    USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, const uint8_t ep_addr)
    {
        return USBD_Get_USB_Status(HAL_PCD_EP_SetStall(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr));
    }
    USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev, const uint8_t ep_addr)
    {
        return USBD_Get_USB_Status(HAL_PCD_EP_ClrStall(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr));
    }
    uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, const uint8_t ep_addr)
    {
        const PCD_HandleTypeDef *hpcd = static_cast<PCD_HandleTypeDef *>(pdev->pData);
        if ((ep_addr & 0x80) == 0x80)
        {
            return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
        }
        return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
    }
    USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev, const uint8_t dev_addr)
    {
        return USBD_Get_USB_Status(HAL_PCD_SetAddress(static_cast<PCD_HandleTypeDef *>(pdev->pData), dev_addr));
    }
    USBD_StatusTypeDef
        USBD_LL_Transmit(USBD_HandleTypeDef *pdev, const uint8_t ep_addr, uint8_t *pbuf, const uint32_t size)
    {
        return USBD_Get_USB_Status(
            HAL_PCD_EP_Transmit(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, pbuf, size));
    }
    USBD_StatusTypeDef
        USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, const uint8_t ep_addr, uint8_t *pbuf, const uint32_t size)
    {
        return USBD_Get_USB_Status(
            HAL_PCD_EP_Receive(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, pbuf, size));
    }
    uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, const uint8_t ep_addr)
    {
        return HAL_PCD_EP_GetRxCount(static_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr);
    }
    void *USBD_static_malloc(const uint32_t size)
    {
        UNUSED(size);
        static uint32_t mem[sizeof(USBD_CDC_HandleTypeDef) / 4 + 1]; /* On 32-bit boundary */
        return mem;
    }
    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    void USBD_static_free(void *const p)
    {
        UNUSED(p);
    }
    void USBD_LL_Delay(const uint32_t Delay)
    {
        HAL_Delay(Delay);
    }
}

namespace hw::usb
{
std::expected<void, ErrorCode> init()
{
    if (USBD_Init(&USB_DEVICE_HANDLER, &pdesc, DEVICE_ID) != USBD_OK)
        return std::unexpected(ErrorCode::ERROR);
    if (USBD_RegisterClass(&USB_DEVICE_HANDLER, &USBD_CDC) != USBD_OK)
        return std::unexpected(ErrorCode::ERROR);
    if (USBD_CDC_RegisterInterface(&USB_DEVICE_HANDLER, &USBD_Interface_fops) != USBD_OK)
        return std::unexpected(ErrorCode::ERROR);
    if (USBD_Start(&USB_DEVICE_HANDLER) != USBD_OK)
        return std::unexpected(ErrorCode::ERROR);
    HAL_PWREx_EnableUSBVoltageDetector();
    return {};
}

bool checkConnection()
{
    return USB_DEVICE_HANDLER.dev_state == USBD_STATE_CONFIGURED;
}

std::expected<void, ErrorCode> transmit(std::span<uint8_t> msg)
{
    if (USB_DEVICE_HANDLER.pClassData == nullptr)
    {
        return std::unexpected(ErrorCode::BUSY);
    }
    if (static_cast<const USBD_CDC_HandleTypeDef *>(USB_DEVICE_HANDLER.pClassData)->TxState != 0)
        return std::unexpected(ErrorCode::BUSY);
    USBD_CDC_SetTxBuffer(&USB_DEVICE_HANDLER, msg.data(), msg.size());
    if (const uint8_t status = USBD_CDC_TransmitPacket(&USB_DEVICE_HANDLER); status != USBD_OK)
    {
        // LOG_WARN("USB: Transmit handle returned %d status code instead of USBD_OK.", status);
        return std::unexpected(ErrorCode::ERROR);
    }
    return {};
}

// Connection handling

static bool                        usb_connected = false;
static std::optional<TaskHandle_t> usb_task      = std::nullopt;

static void connect_callback()
{
    usb_connected = true;
    if (not usb_task.has_value())
    {
        LOG_WARN("USB: No task to notify.");
        return;
    }
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    // notify the task which is waiting for the USB connection
    vTaskNotifyGiveFromISR(usb_task.value(), &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

static void disconnect_callback()
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

    assert(usb_task == std::nullopt);
    // save which task we need to notify
    usb_task = xTaskGetCurrentTaskHandle();
    // block this task until notification comes
    const uint32_t num_notifications = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    UNUSED(num_notifications);
    usb_task = std::nullopt;
}
} // namespace hw::usb

// HAL INTERRUPT SETUP
extern "C"
{
    void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
    {
        USBD_LL_SetupStage(static_cast<USBD_HandleTypeDef *>(hpcd->pData), reinterpret_cast<uint8_t *>(hpcd->Setup));
    }
    void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, const uint8_t epnum)
    {
        USBD_LL_DataOutStage(static_cast<USBD_HandleTypeDef *>(hpcd->pData), epnum, hpcd->OUT_ep[epnum].xfer_buff);
    }
    void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, const uint8_t epnum)
    {
        USBD_LL_DataInStage(static_cast<USBD_HandleTypeDef *>(hpcd->pData), epnum, hpcd->IN_ep[epnum].xfer_buff);
    }
    void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
    {
        USBD_LL_SOF(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
    }
    void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
    {
        constexpr USBD_SpeedTypeDef speed = USBD_SPEED_FULL;
        if (hpcd->Init.speed != PCD_SPEED_FULL)
        {
            Error_Handler();
        }
        /* Set Speed. */
        USBD_LL_SetSpeed(static_cast<USBD_HandleTypeDef *>(hpcd->pData), speed);
        /* Reset Device. */
        USBD_LL_Reset(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
    }
    void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
    {
        USBD_LL_Suspend(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
#ifdef __HAL_PCD_GATE_PHYCLOCK
        __HAL_PCD_GATE_PHYCLOCK(hpcd);
#endif
        hw::usb::disconnect_callback();
        if (hpcd->Init.low_power_enable)
        {
            SCB->SCR |= (uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk);
        }
    }
    void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
    {
        hw::usb::connect_callback();
        USBD_LL_Resume(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
    }
    void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
    {
        USBD_LL_DevConnected(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
    }
    void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
    {
        USBD_LL_DevDisconnected(static_cast<USBD_HandleTypeDef *>(hpcd->pData));
    }
}
