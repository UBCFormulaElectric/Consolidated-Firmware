/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : usbd_conf.c
 * @brief          : USB Device low level configuration for H7 (OTG HS)
 *                   and H5 (USB_DRD_FS) in a single file.
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#if defined(STM32H733xx)
    #include "stm32h7xx.h"
    #include "stm32h7xx_hal.h"
#elif defined(STM32H562xx)
    #include "stm32h5xx.h"
    #include "stm32h5xx_hal.h"
#else
    #error "Unsupported MCU: please define STM32H733xx or STM32H562xx"
#endif

#include "usbd_def.h"
#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usbd_conf.h"  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#if defined(STM32H733xx)
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;
#elif defined(STM32H562xx)
extern PCD_HandleTypeDef hpcd_USB_DRD_FS;
#endif

void Error_Handler(void);

/* Private function prototypes -----------------------------------------------*/
USBD_StatusTypeDef USBD_Get_USB_Status(HAL_StatusTypeDef hal_status);

void HAL_PCD_MspInit(PCD_HandleTypeDef *pcdHandle)
{
#if defined(STM32H733xx)
    if (pcdHandle->Instance == USB_OTG_HS)
    {
        /* Enable USB Voltage detector */
        HAL_PWREx_EnableUSBVoltageDetector();

        /* Peripheral clock enable */
        __HAL_RCC_USB_OTG_HS_CLK_ENABLE();

        /* NVIC configuration for USB OTG HS */
        HAL_NVIC_SetPriority(OTG_HS_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
    }
#elif defined(STM32H562xx)
    if (pcdHandle->Instance == USB_DRD_FS)
    {
        /* Peripheral clock enable */
        __HAL_RCC_USB_DRD_FS_CLK_ENABLE();

        /* NVIC configuration for USB DRD FS */
        HAL_NVIC_SetPriority(USB_DRD_FS_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(USB_DRD_FS_IRQn);
    }
#endif
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef *pcdHandle)
{
#if defined(STM32H733xx)
    if (pcdHandle->Instance == USB_OTG_HS)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USB_OTG_HS_CLK_DISABLE();

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(OTG_HS_IRQn);
    }
#elif defined(STM32H562xx)
    if (pcdHandle->Instance == USB_DRD_FS)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USB_DRD_FS_CLK_DISABLE();

        /* Peripheral interrupt Deinit*/
        HAL_NVIC_DisableIRQ(USB_DRD_FS_IRQn);
    }
#endif
}

/* Setup stage */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
#else
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
#endif
{
    USBD_LL_SetupStage((USBD_HandleTypeDef *)hpcd->pData, (uint8_t *)hpcd->Setup);
}

/* Data OUT stage */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#else
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#endif
{
    USBD_LL_DataOutStage((USBD_HandleTypeDef *)hpcd->pData,
                         epnum,
                         hpcd->OUT_ep[epnum].xfer_buff);
}

/* Data IN stage */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#else
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#endif
{
    USBD_LL_DataInStage((USBD_HandleTypeDef *)hpcd->pData,
                        epnum,
                        hpcd->IN_ep[epnum].xfer_buff);
}

/* SOF (only really used/registered on H7, but harmlessly shared) */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
#else
void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
#endif
{
    USBD_LL_SOF((USBD_HandleTypeDef *)hpcd->pData);
}

/* Reset */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
#else
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
#endif
{
    USBD_SpeedTypeDef speed = USBD_SPEED_FULL;

#if defined(STM32H733xx)
    if (hpcd->Init.speed == PCD_SPEED_HIGH)
    {
        speed = USBD_SPEED_HIGH;
    }
    else if (hpcd->Init.speed == PCD_SPEED_FULL)
    {
        speed = USBD_SPEED_FULL;
    }
    else
    {
        Error_Handler();
    }
#else
    /* H5 is FS only */
    (void)hpcd;
    speed = USBD_SPEED_FULL;
#endif

    USBD_LL_SetSpeed((USBD_HandleTypeDef *)hpcd->pData, speed);
    USBD_LL_Reset((USBD_HandleTypeDef *)hpcd->pData);
}

/* Suspend */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
#else
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
#endif
{
    USBD_LL_Suspend((USBD_HandleTypeDef *)hpcd->pData);

#if defined(STM32H733xx)
    __HAL_PCD_GATE_PHYCLOCK(hpcd);

    /* Optional low-power entry */
    if (hpcd->Init.low_power_enable)
    {
        SCB->SCR |= (uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk);
    }

    hw_usb_disconnect_callback();
#else
    (void)hpcd;
#endif
}

/* Resume */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
#else
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
#endif
{
#if defined(STM32H733xx)
    hw_usb_connect_callback();
#endif
    USBD_LL_Resume((USBD_HandleTypeDef *)hpcd->pData);
}

/* ISO OUT incomplete */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#else
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#endif
{
    USBD_LL_IsoOUTIncomplete((USBD_HandleTypeDef *)hpcd->pData, epnum);
}

/* ISO IN incomplete */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#else
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#endif
{
    USBD_LL_IsoINIncomplete((USBD_HandleTypeDef *)hpcd->pData, epnum);
}

/* Connect */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
#else
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
#endif
{
    USBD_LL_DevConnected((USBD_HandleTypeDef *)hpcd->pData);
}

/* Disconnect */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
#else
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
#endif
{
    USBD_LL_DevDisconnected((USBD_HandleTypeDef *)hpcd->pData);
}

USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev)
{
#if defined(STM32H733xx)
    /* H7: OTG HS, DEVICE_HS assumed in usbd_conf.h */
    if (pdev->id == DEVICE_HS)
    {
        hpcd_USB_OTG_HS.Instance                 = USB_OTG_HS;
        hpcd_USB_OTG_HS.Init.dev_endpoints       = 9;
        hpcd_USB_OTG_HS.Init.speed               = PCD_SPEED_FULL;  /* Using embedded FS PHY */
        hpcd_USB_OTG_HS.Init.dma_enable          = DISABLE;
        hpcd_USB_OTG_HS.Init.phy_itface          = USB_OTG_EMBEDDED_PHY;
        hpcd_USB_OTG_HS.Init.Sof_enable          = DISABLE;
        hpcd_USB_OTG_HS.Init.low_power_enable    = DISABLE;
        hpcd_USB_OTG_HS.Init.lpm_enable          = DISABLE;
        hpcd_USB_OTG_HS.Init.vbus_sensing_enable = DISABLE;
        hpcd_USB_OTG_HS.Init.use_dedicated_ep1   = DISABLE;
        hpcd_USB_OTG_HS.Init.use_external_vbus   = DISABLE;

        hpcd_USB_OTG_HS.pData = pdev;
        pdev->pData           = &hpcd_USB_OTG_HS;

        if (HAL_PCD_Init(&hpcd_USB_OTG_HS) != HAL_OK)
        {
            Error_Handler();
        }

#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
        /* Register callbacks */
        HAL_PCD_RegisterCallback(&hpcd_USB_OTG_HS, HAL_PCD_SOF_CB_ID,         PCD_SOFCallback);
        HAL_PCD_RegisterCallback(&hpcd_USB_OTG_HS, HAL_PCD_SETUPSTAGE_CB_ID,  PCD_SetupStageCallback);
        HAL_PCD_RegisterCallback(&hpcd_USB_OTG_HS, HAL_PCD_RESET_CB_ID,       PCD_ResetCallback);
        HAL_PCD_RegisterCallback(&hpcd_USB_OTG_HS, HAL_PCD_SUSPEND_CB_ID,     PCD_SuspendCallback);
        HAL_PCD_RegisterCallback(&hpcd_USB_OTG_HS, HAL_PCD_RESUME_CB_ID,      PCD_ResumeCallback);
        HAL_PCD_RegisterCallback(&hpcd_USB_OTG_HS, HAL_PCD_CONNECT_CB_ID,     PCD_ConnectCallback);
        HAL_PCD_RegisterCallback(&hpcd_USB_OTG_HS, HAL_PCD_DISCONNECT_CB_ID,  PCD_DisconnectCallback);

        HAL_PCD_RegisterDataOutStageCallback(&hpcd_USB_OTG_HS, PCD_DataOutStageCallback);
        HAL_PCD_RegisterDataInStageCallback(&hpcd_USB_OTG_HS,  PCD_DataInStageCallback);
        HAL_PCD_RegisterIsoOutIncpltCallback(&hpcd_USB_OTG_HS, PCD_ISOOUTIncompleteCallback);
        HAL_PCD_RegisterIsoInIncpltCallback(&hpcd_USB_OTG_HS,  PCD_ISOINIncompleteCallback);
#endif

        /* FIFO configuration (tune as needed) */
        HAL_PCDEx_SetRxFiFo(&hpcd_USB_OTG_HS, 0x200);
        HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 0, 0x80);
        HAL_PCDEx_SetTxFiFo(&hpcd_USB_OTG_HS, 1, 0x174);
    }
#elif defined(STM32H562xx)
    /* H5: USB_DRD_FS, assume DEVICE_FS */
    (void)pdev->id; /* Not used if only FS */

    hpcd_USB_DRD_FS.Instance                 = USB_DRD_FS;
    hpcd_USB_DRD_FS.Init.dev_endpoints       = 6;
    hpcd_USB_DRD_FS.Init.speed               = PCD_SPEED_FULL;
    hpcd_USB_DRD_FS.Init.phy_itface          = PCD_PHY_EMBEDDED;
    hpcd_USB_DRD_FS.Init.Sof_enable          = DISABLE;
    hpcd_USB_DRD_FS.Init.low_power_enable    = DISABLE;
    hpcd_USB_DRD_FS.Init.lpm_enable          = DISABLE;
    hpcd_USB_DRD_FS.Init.battery_charging_enable = DISABLE;
    hpcd_USB_DRD_FS.Init.vbus_sensing_enable = DISABLE;

    hpcd_USB_DRD_FS.pData = pdev;
    pdev->pData           = &hpcd_USB_DRD_FS;

    if (HAL_PCD_Init(&hpcd_USB_DRD_FS) != HAL_OK)
    {
        Error_Handler();
    }

    /* PMA configuration for FS endpoints (tune offsets as needed) */
    HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x00,      PCD_SNG_BUF, 0x20); /* EP0 OUT */
    HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x80,      PCD_SNG_BUF, 0x60); /* EP0 IN  */
    HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, CDC_OUT_EP, PCD_SNG_BUF, 0xA0); /* CDC OUT */
    HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, CDC_IN_EP,  PCD_SNG_BUF, 0xE0); /* CDC IN  */
#endif

    return USBD_OK;
}

USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev)
{
    HAL_StatusTypeDef  hal_status = HAL_PCD_DeInit((PCD_HandleTypeDef *)pdev->pData);
    return USBD_Get_USB_Status(hal_status);
}

USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev)
{
    HAL_StatusTypeDef  hal_status = HAL_PCD_Start((PCD_HandleTypeDef *)pdev->pData);
    return USBD_Get_USB_Status(hal_status);
}

USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev)
{
    HAL_StatusTypeDef  hal_status = HAL_PCD_Stop((PCD_HandleTypeDef *)pdev->pData);
    return USBD_Get_USB_Status(hal_status);
}

USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev,
                                  uint8_t ep_addr,
                                  uint8_t ep_type,
                                  uint16_t ep_mps)
{
    HAL_StatusTypeDef  hal_status = HAL_PCD_EP_Open((PCD_HandleTypeDef *)pdev->pData,
                                                    ep_addr, ep_mps, ep_type);
    return USBD_Get_USB_Status(hal_status);
}

USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    HAL_StatusTypeDef  hal_status = HAL_PCD_EP_Close((PCD_HandleTypeDef *)pdev->pData,
                                                     ep_addr);
    return USBD_Get_USB_Status(hal_status);
}

USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    HAL_StatusTypeDef  hal_status = HAL_PCD_EP_Flush((PCD_HandleTypeDef *)pdev->pData,
                                                     ep_addr);
    return USBD_Get_USB_Status(hal_status);
}

USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    HAL_StatusTypeDef  hal_status = HAL_PCD_EP_SetStall((PCD_HandleTypeDef *)pdev->pData,
                                                        ep_addr);
    return USBD_Get_USB_Status(hal_status);
}

USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    HAL_StatusTypeDef  hal_status = HAL_PCD_EP_ClrStall((PCD_HandleTypeDef *)pdev->pData,
                                                        ep_addr);
    return USBD_Get_USB_Status(hal_status);
}

uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    PCD_HandleTypeDef *hpcd = (PCD_HandleTypeDef *)pdev->pData;

    if ((ep_addr & 0x80U) == 0x80U)
    {
        return hpcd->IN_ep[ep_addr & 0x7FU].is_stall;
    }
    else
    {
        return hpcd->OUT_ep[ep_addr & 0x7FU].is_stall;
    }
}

USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev, uint8_t dev_addr)
{
    HAL_StatusTypeDef  hal_status = HAL_PCD_SetAddress((PCD_HandleTypeDef *)pdev->pData,
                                                       dev_addr);
    return USBD_Get_USB_Status(hal_status);
}

USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev,
                                    uint8_t ep_addr,
                                    uint8_t *pbuf,
                                    uint32_t size)
{
    HAL_StatusTypeDef  hal_status = HAL_PCD_EP_Transmit((PCD_HandleTypeDef *)pdev->pData,
                                                        ep_addr, pbuf, size);
    return USBD_Get_USB_Status(hal_status);
}

USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev,
                                          uint8_t ep_addr,
                                          uint8_t *pbuf,
                                          uint32_t size)
{
    HAL_StatusTypeDef  hal_status = HAL_PCD_EP_Receive((PCD_HandleTypeDef *)pdev->pData,
                                                       ep_addr, pbuf, size);
    return USBD_Get_USB_Status(hal_status);
}

uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
    return HAL_PCD_EP_GetRxCount((PCD_HandleTypeDef *)pdev->pData, ep_addr);
}

#ifdef USBD_HS_TESTMODE_ENABLE
USBD_StatusTypeDef USBD_LL_SetTestMode(USBD_HandleTypeDef *pdev, uint8_t testmode)
{
    (void)pdev;
    (void)testmode;
    return USBD_OK;
}
#endif

void *USBD_static_malloc(uint32_t size)
{
    (void)size;
    static uint32_t mem[(sizeof(USBD_CDC_HandleTypeDef) / 4U) + 1U];
    return mem;
}

void USBD_static_free(void *p)
{
    (void)p;
}

void USBD_LL_Delay(uint32_t Delay)
{
    HAL_Delay(Delay);
}

USBD_StatusTypeDef USBD_Get_USB_Status(HAL_StatusTypeDef hal_status)
{
    switch (hal_status)
    {
        case HAL_OK:
            return USBD_OK;
        case HAL_ERROR:
            return USBD_FAIL;
        case HAL_BUSY:
            return USBD_BUSY;
        case HAL_TIMEOUT:
        default:
            return USBD_FAIL;
    }
}