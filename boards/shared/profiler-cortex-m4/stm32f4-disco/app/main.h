//-- Define to prevent recursive inclusion -------------------------------------
#ifndef __MAIN_H
#define __MAIN_H

//-- Includes ------------------------------------------------------------------
#include <stdint.h>
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_conf.h"
#include "retarget_conf.h"

//-- Exported constants --------------------------------------------------------
#define LED3_PIN                           LL_GPIO_PIN_13
#define LED3_GPIO_PORT                     GPIOD
#define LED3_GPIO_CLK_ENABLE()             LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD)

//-- Exported variables --------------------------------------------------------
extern uint32_t APB1Clock;
extern uint32_t APB2Clock;

//-- Exported functions --------------------------------------------------------
// TIM2 update interrupt processing
void TimerUpdate_Callback(void);

#endif // __MAIN_H
