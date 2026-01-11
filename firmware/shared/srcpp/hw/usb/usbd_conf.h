#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(STM32H733xx)
#include "stm32h7xx.h"
#define DEVICE_FS 0
#define DEVICE_HS 1

#elif defined(STM32H562xx)
#include "stm32h5xx.h"
#define DEVICE_FS 0
#else
#error "Unsupported STM32 family: please define an H5 or H7 part number"
#endif

#define USBD_MAX_NUM_INTERFACES 1U
#define USBD_MAX_NUM_CONFIGURATION 1U
#define USBD_MAX_STR_DESC_SIZ 0x100u
#define USBD_SELF_POWERED 1U
#define USBD_DEBUG_LEVEL 3U
#define USBD_CDC_INTERVAL 2000U
#ifndef USBD_LPM_ENABLED
#define USBD_LPM_ENABLED 0U
#endif
#ifndef USBD_CLASS_BOS_ENABLED
#define USBD_CLASS_BOS_ENABLED 0U
#endif
#ifndef USBD_USER_REGISTER_CALLBACK
#define USBD_USER_REGISTER_CALLBACK 0U
#endif

#define USBD_malloc (void *)USBD_static_malloc
#define USBD_free USBD_static_free
#define USBD_memset memset
#define USBD_memcpy memcpy
#define USBD_Delay HAL_Delay

/* DEBUG macros */
#if (USBD_DEBUG_LEVEL > 0U)
#define USBD_UsrLog(...)     \
    do                       \
    {                        \
        printf(__VA_ARGS__); \
        printf("\n");        \
    } while (0)
#else
#define USBD_UsrLog(...) \
    do                   \
    {                    \
    } while (0)
#endif
#if (USBD_DEBUG_LEVEL > 1U)
#define USBD_ErrLog(...)     \
    do                       \
    {                        \
        printf("ERROR: ");   \
        printf(__VA_ARGS__); \
        printf("\n");        \
    } while (0)
#else
#define USBD_ErrLog(...) \
    do                   \
    {                    \
    } while (0)
#endif
#if (USBD_DEBUG_LEVEL > 2U)
#define USBD_DbgLog(...)     \
    do                       \
    {                        \
        printf("DEBUG : ");  \
        printf(__VA_ARGS__); \
        printf("\n");        \
    } while (0)
#else
#define USBD_DbgLog(...) \
    do                   \
    {                    \
    } while (0)
#endif

    void *USBD_static_malloc(uint32_t size);
    void  USBD_static_free(void *p);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CONF__H__ */
