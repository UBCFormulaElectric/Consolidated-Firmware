#ifndef __USBD_DESC_H__
#define __USBD_DESC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_def.h"
#define DEVICE_ID1 (UID_BASE)
#define DEVICE_ID2 (UID_BASE + 0x4)
#define DEVICE_ID3 (UID_BASE + 0x8)

#define USB_SIZ_STRING_SERIAL 0x1A

#if defined(STM32H733xx)
    extern USBD_DescriptorsTypeDef HS_Desc;
    extern USBD_DescriptorsTypeDef CDC_Desc;
#elif defined(STM32H562xx)
extern USBD_DescriptorsTypeDef FS_Desc;
extern USBD_DescriptorsTypeDef CDC_Desc;
#endif

#ifdef __cplusplus
}
#endif

#endif
