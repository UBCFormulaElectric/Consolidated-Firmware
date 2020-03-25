#ifndef RETARGET_CONF_H
#define RETARGET_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

//-- Includes ------------------------------------------------------------------
#include "main.h"
#include <stdio.h>

//-- Defines -------------------------------------------------------------------

#ifdef USE_RETARGET
#define RETARGET_USART USART2
#define RETARGET_USART_CLK_ENABLE() LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
#define RETARGET_USART_GPIO_CLK_ENABLE() LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#define RETARGET_USART_TX_PIN LL_GPIO_PIN_2
#define RETARGET_USART_TX_GPIO_PORT GPIOA
#define RETARGET_USART_SET_TX_GPIO_AF() LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7)
#define RETARGET_USART_RX_PIN LL_GPIO_PIN_3
#define RETARGET_USART_RX_GPIO_PORT GPIOA
#define RETARGET_USART_SET_RX_GPIO_AF() LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7)
#define RETARGET_USART_CLK APB1Clock
#ifndef RETARGET_BAUDRATE
#define RETARGET_BAUDRATE 115200
#endif
#elif defined USE_RETARGET_ITM
#else
#define printf(...) ((void)0)
#endif // USE_RETARGET

//-- Functions -----------------------------------------------------------------
void retarget_init(void);
char retarget_get_char(void);
int retarget_put_char(char ch);

#ifdef __cplusplus
}
#endif

#endif // RETARGET_CONF_H
