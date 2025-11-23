#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(STM32H733xx)
    #include "stm32h7xx.h"
    #include "stm32h7xx_hal.h"
    #include "stm32h7xx_hal_pcd.h"

#elif defined(STM32H562xx)
    #include "stm32h5xx.h"
    #include "stm32h5xx_hal.h"
    #include "stm32h5xx_hal_pcd.h"

#else
    #error "Unsupported STM32 family: please define an H5 or H7 part number"
#endif

#define USBD_MAX_NUM_INTERFACES        1U
#define USBD_MAX_NUM_CONFIGURATION     1U
#define USBD_MAX_STR_DESC_SIZ          512U
#define USBD_LPM_ENABLED               1U
#define USBD_SELF_POWERED              1U
#define USBD_DEBUG_LEVEL               0U

6

#define USBD_CDC_INTERVAL              2000U 

#define USBD_malloc                    USBD_static_malloc
#define USBD_free                      USBD_static_free
#define USBD_memset                    memset
#define USBD_memcpy                    memcpy
#define USBD_Delay                     HAL_Delay

void *USBD_static_malloc(uint32_t size);
void  USBD_static_free(void *p);

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CONF__H__ */