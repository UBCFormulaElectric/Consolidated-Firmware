[#ftl]
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ${FamilyName?lower_case}xx_hal_timebase_tim.c 
  * @brief   HAL time base based on the hardware TIM.
  ******************************************************************************
[@common.optinclude name=mxTmpFolder+"/license.tmp"/][#--include License text --]
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
[#if FamilyName=="STM32WB0"]
#include "${FamilyName?lower_case}x_hal.h"
#include "${FamilyName?lower_case}x_hal_tim.h"
[#else]
#include "${FamilyName?lower_case}xx_hal.h"
#include "${FamilyName?lower_case}xx_hal_tim.h"
[/#if]

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
[#if FamilyName=="STM32WBA"]
#define TIM_CNT_FREQ 1000000U /* Timer frequency counter : 1 MHz */
#define TIM_FREQ     1000U    /* Timer frequency : 1 kHz => to have 1 ms interrupt */
[/#if]
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef        h${instance?lower_case}; 
/* Private function prototypes -----------------------------------------------*/
[#if FamilyName=="STM32WBA"]
void ${instance}_IRQHandler(void);
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1U)
void TimeBase_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#endif
[/#if]
[#if FamilyName=="STM32F1"]
void ${instance}_IRQHandler(void);
[/#if]
[#if isTim_callback?? && isTim_callback=="1"]
 void TimeBase_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
 [/#if]
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function configures the ${instance} as a time base source. 
  *         The time source is configured  to have 1ms time base with a dedicated 
  *         Tick interrupt priority. 
  * @note   This function is called  automatically at the beginning of program after
  *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig(). 
  * @param  TickPriority: Tick interrupt priority.
  * @retval HAL status
  */
[#assign APB = "APB2"]
[#if instance=="TIM2"||instance=="TIM3"||instance=="TIM4"||instance=="TIM5"||instance=="TIM6"||instance=="TIM7"||instance=="TIM12"||instance=="TIM13"||instance=="TIM14"||instance=="TIM18"]
[#assign APB = "APB1"]
[/#if]
[#if FamilyName=="STM32F0"]
[#assign APB = "APB1"]
[/#if]
[#if FamilyName=="STM32G0"]
[#assign APB = "APB1"]
[/#if]
[#if FamilyName=="STM32C0"]
[#assign APB = "APB1"]
[/#if]
[#if FamilyName=="STM32U0"]
[#assign APB = "APB1"]
[/#if]
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  RCC_ClkInitTypeDef    clkconfig;
[#if FamilyName!="STM32MP2" && FamilyName!="STM32H7" && FamilyName!="STM32F0" && FamilyName!="STM32F1" && FamilyName!="STM32F2" && FamilyName!="STM32F3" && FamilyName!="STM32F4" && FamilyName!="STM32F7" && FamilyName!="STM32G0" && FamilyName!="STM32L0" && FamilyName!="STM32L1" && FamilyName!="STM32L4" && FamilyName!="STM32H5" && FamilyName!="STM32WBA"]
  uint32_t              uwTimclock = 0;
  uint32_t              uwPrescalerValue = 0;
[/#if]
[#if FamilyName =="STM32MP2"]
  uint32_t              uwTimclock, uwAPB1Prescaler = 0;
  uint32_t              uwPrescalerValue = 0;
[/#if]
[#if FamilyName=="STM32H7" || FamilyName=="STM32G0" || FamilyName=="STM32L0" || FamilyName=="STM32L4" || FamilyName=="STM32H5" || FamilyName="STM32WBA"]
[#if APB=="APB1"]
  uint32_t              uwTimclock, uwAPB1Prescaler;
[#else]
  uint32_t              uwTimclock;
[#if FamilyName=="STM32L0"]
  uint32_t              uwAPB2Prescaler;
[/#if]
[#if FamilyName=="STM32WBA"]
  uint32_t              uwAPB2Prescaler;
[/#if]
[/#if]

  uint32_t              uwPrescalerValue;
[/#if]
[#if FamilyName=="STM32F0" || FamilyName=="STM32F1" || FamilyName=="STM32F2" || FamilyName=="STM32F3" || FamilyName=="STM32F4" || FamilyName=="STM32F7"  || FamilyName=="STM32L1"]
[#if APB=="APB1"]
  uint32_t              uwTimclock, uwAPB1Prescaler = 0U;
[#else]
  uint32_t              uwTimclock = 0U;
[/#if]

  uint32_t              uwPrescalerValue = 0U;
[/#if]
  uint32_t              pFLatency;
[#if FamilyName=="STM32U5" || FamilyName=="STM32F0"  || FamilyName=="STM32F2" || FamilyName=="STM32F3" || FamilyName=="STM32F4" || FamilyName=="STM32F7"  || FamilyName=="STM32L0" || FamilyName=="STM32G4"]
  HAL_StatusTypeDef     status;
[/#if]
[#if FamilyName=="STM32WBA"]
  HAL_StatusTypeDef     Status;
[/#if]
[#if FamilyName=="STM32WL" || FamilyName=="STM32F1" || FamilyName=="STM32G0" || FamilyName=="STM32L1" || FamilyName=="STM32L4"]
  HAL_StatusTypeDef     status = HAL_OK;

[/#if]
[#if FamilyName=="STM32H5"]
  HAL_StatusTypeDef     status;
[/#if]
[#if FamilyName!="STM32WL" && FamilyName!="STM32H7" && FamilyName!="STM32C0" && FamilyName!="STM32G4" && FamilyName!="STM32U5" && FamilyName!="STM32F0" && FamilyName!="STM32F1"  && FamilyName!="STM32F2" && FamilyName!="STM32F3" && FamilyName!="STM32F4" && FamilyName!="STM32F7" && FamilyName!="STM32G0" && FamilyName!="STM32L1" && FamilyName!="STM32L4" && FamilyName!="STM32H5" && FamilyName!="STM32WBA"]
  /*Configure the ${instance} IRQ priority */
 [#if DIE != "DIE501"] 
  HAL_NVIC_SetPriority(${timeBaseInterrupt}, TickPriority ,0); 
  [#else]
  IRQ_SetPriority(${timeBaseInterrupt}, TickPriority);
  [/#if] 
  /* Enable the ${instance} global Interrupt */
  [#if DIE != "DIE501"]
  HAL_NVIC_EnableIRQ(${timeBaseInterrupt}); 
  [#else]
  IRQ_Enable(${timeBaseInterrupt});
  [/#if]  
[/#if]
[#if FamilyName=="STM32H7" || FamilyName=="STM32C0"]
  /*Configure the ${instance} IRQ priority */
  if (TickPriority < (1UL << __NVIC_PRIO_BITS))
   {
     HAL_NVIC_SetPriority(${timeBaseInterrupt}, TickPriority ,0); 
  
     /* Enable the ${instance} global Interrupt */
     HAL_NVIC_EnableIRQ(${timeBaseInterrupt}); 
     uwTickPrio = TickPriority;
    }
  else
  {
    return HAL_ERROR;
  }
[/#if]
#n
  /* Enable ${instance} clock */
  __HAL_RCC_${instance}_CLK_ENABLE();
[#if FamilyName=="STM32MP1"]
  __HAL_RCC_${instance}_FORCE_RESET();
  __HAL_RCC_${instance}_RELEASE_RESET();
[/#if]
  /* Get clock configuration */
  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);  
[#if FamilyName=="STM32H7" || FamilyName=="STM32F0" || FamilyName=="STM32F1" || FamilyName=="STM32F2" || FamilyName=="STM32F3" || FamilyName=="STM32F4" || FamilyName=="STM32F7" || FamilyName=="STM32G0" || FamilyName=="STM32L0" || FamilyName=="STM32L1" || FamilyName=="STM32L4" || FamilyName=="STM32H5" || FamilyName=="STM32WBA"]
 [#if APB=="APB1"]
  /* Get ${APB} prescaler */
  uw${APB}Prescaler = clkconfig.${APB}CLKDivider; 
[/#if] 
[#if FamilyName=="STM32MP2"]
 [#if APB=="APB1"]
  /* Get ${APB} prescaler */
  uw${APB}Prescaler = clkconfig.${APB}_Div; 
 [#elseif APB=="APB2"]
 /* Get ${APB} prescaler */
  uw${APB}Prescaler = clkconfig.${APB}_Div;
[/#if] 
[/#if]
[#if APB=="APB2" &&  (FamilyName=="STM32L0" || FamilyName=="STM32WBA")]
  /* Get ${APB} prescaler */
  uw${APB}Prescaler = clkconfig.${APB}CLKDivider; 
[/#if] 
[/#if]
[#if FamilyName=="STM32H7" || FamilyName=="STM32F0" || FamilyName=="STM32F1" || FamilyName=="STM32F2" || FamilyName=="STM32F3" || FamilyName=="STM32F4" || FamilyName=="STM32F7" || FamilyName=="STM32G0" || FamilyName=="STM32L0" || FamilyName=="STM32L1" || FamilyName=="STM32L4" || FamilyName=="STM32H5" || FamilyName=="STM32WBA"]
  /* Compute ${instance} clock */
    [#if APB=="APB1"]
  if (uwAPB1Prescaler == RCC_HCLK_DIV1)
  {
    uwTimclock = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
  }
        
    [#else]
 [#if APB=="APB2" && FamilyName=="STM32L0"]
  if (uwAPB2Prescaler == RCC_HCLK_DIV1)
  {
    uwTimclock = HAL_RCC_GetPCLK2Freq();
  }
  else
  {
    uwTimclock = 2U * HAL_RCC_GetPCLK2Freq();
  }   
[/#if]  
    [#if APB=="APB2" && FamilyName=="STM32WBA"]
  if (uwAPB2Prescaler == RCC_HCLK_DIV1)
  {
    uwTimclock = HAL_RCC_GetPCLK2Freq();
  }
  else
  {
    uwTimclock = 2UL * HAL_RCC_GetPCLK2Freq();
  }   
    [/#if]  
    [#if TIMAPB2Presc?? && TIMAPB2Presc!="1" && FamilyName!="STM32L0"]
      uwTimclock = ${TIMAPB2Presc}*HAL_RCC_GetPCLK2Freq();
    [#else]
       [#if FamilyName!="STM32WBA"  ]
      uwTimclock = HAL_RCC_GetPCLK2Freq();
       [/#if]
    [/#if]
     
    [/#if]
   [/#if]
[#if FamilyName!="STM32MP2" && FamilyName!="STM32H7" && FamilyName!="STM32F0" && FamilyName!="STM32F1"  && FamilyName!="STM32F2" && FamilyName!="STM32F3" && FamilyName!="STM32F4" && FamilyName!="STM32F7" && FamilyName!="STM32G0" && FamilyName!="STM32L0" && FamilyName!="STM32L1" && FamilyName!="STM32L4" && FamilyName!="STM32H5" && FamilyName!="STM32WBA" && FamilyName!="STM32WB0"]
  /* Compute ${instance} clock */
    [#if APB=="APB1"]
        [#if TIMAPB1Presc?? && TIMAPB1Presc!="1"]
  uwTimclock = ${TIMAPB1Presc}*HAL_RCC_GetPCLK1Freq();
        [#else]
  uwTimclock = HAL_RCC_GetPCLK1Freq();
        [/#if]
    [#else]
        [#if TIMAPB2Presc?? && TIMAPB2Presc!="1"]
  uwTimclock = ${TIMAPB2Presc}*HAL_RCC_GetPCLK2Freq();
        [#else]
  uwTimclock = HAL_RCC_GetPCLK2Freq();
        [/#if]
    [/#if]
   [/#if]
[#if FamilyName=="STM32WB0"]
  /* Compute ${instance} clock */
  uwTimclock = HAL_RCC_GetSysClockFreq();
[/#if]
  
  [#if FamilyName=="STM32MP2"]
  /* Compute ${instance} clock */
 if (uwAPB1Prescaler == RCC_APB1_DIV1) 
  {
    uwTimclock = HAL_RCC_GetFreq(RCC_CLOCKTYPE_ICN_APB1);
  }
  else
  {
    uwTimclock = 2*HAL_RCC_GetFreq(RCC_CLOCKTYPE_ICN_APB1);
  }
  [/#if]
  
[#if FamilyName=="STM32H5"]
  /* Compute the prescaler value to have ${instance} counter clock equal to 100KHz */
  uwPrescalerValue = (uint32_t) ((uwTimclock / 100000U) - 1U);
[#elseif FamilyName=="STM32WBA"]
  /* Compute the prescaler value to have ${instance} counter clock equal to TIM_CNT_FREQ */
  uwPrescalerValue = (uint32_t) ((uwTimclock / TIM_CNT_FREQ) - 1U);
[#else]
  /* Compute the prescaler value to have ${instance} counter clock equal to 1MHz */
  uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);
[/#if]
  
  /* Initialize ${instance} */
  h${instance?lower_case}.Instance = ${instance};
  
  /* Initialize TIMx peripheral as follow:
  
[#if FamilyName!="STM32WBA"]
  + Period = [(${instance}CLK/1000) - 1]. to have a (1/1000) s time base.
[/#if]
[#if FamilyName=="STM32H5"]
  + Prescaler = (uwTimclock/100000 - 1) to have a 100KHz counter clock.
[#elseif FamilyName=="STM32WBA"]
  + Period = [(TIM_CNT_FREQ/TIM_FREQ) - 1]. to have a (1/TIM_FREQ) s time base.
  + Prescaler = (uwTimclock/TIM_CNT_FREQ - 1) to have a TIM_CNT_FREQ counter clock.
[#else]
  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
[/#if]
  + ClockDivision = 0
  + Counter direction = Up
  */
[#if FamilyName=="STM32H5"]
  h${instance?lower_case}.Init.Period = (100000U / 1000U) - 1U;
[#elseif FamilyName=="STM32WBA"]
  h${instance?lower_case}.Init.Period = (TIM_CNT_FREQ / TIM_FREQ) - 1U;
[#else]
  h${instance?lower_case}.Init.Period = (1000000U / 1000U) - 1U;
[/#if]
  h${instance?lower_case}.Init.Prescaler = uwPrescalerValue;
  h${instance?lower_case}.Init.ClockDivision = 0;
  h${instance?lower_case}.Init.CounterMode = TIM_COUNTERMODE_UP;
[#if FamilyName=="STM32F0" || FamilyName=="STM32F1" || FamilyName=="STM32F2" || FamilyName=="STM32F3" || FamilyName=="STM32F4" || FamilyName=="STM32F7" || FamilyName=="STM32G0" || FamilyName=="STM32L4"]
  h${instance?lower_case}.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
[/#if]
[#if FamilyName=="STM32WL" || FamilyName=="STM32G4" || FamilyName=="STM32U5" || FamilyName=="STM32F0" || FamilyName=="STM32F1" || FamilyName=="STM32F2" || FamilyName=="STM32F3" || FamilyName=="STM32F4" || FamilyName=="STM32F7" || FamilyName=="STM32G0" || FamilyName=="STM32L0" || FamilyName=="STM32L1" || FamilyName=="STM32L4"]

  status = HAL_TIM_Base_Init(&h${instance?lower_case});
  if (status == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    status = HAL_TIM_Base_Start_IT(&h${instance?lower_case});
    if (status == HAL_OK)
    {
    [#if FamilyName!="STM32U5" && FamilyName!="STM32L0" && FamilyName!="STM32H5"]
    /* Enable the ${instance} global Interrupt */
        [#if DIE != "DIE501"]    
        HAL_NVIC_EnableIRQ(${timeBaseInterrupt}); 
        [#else]
        IRQ_Enable(${timeBaseInterrupt});
        [/#if]
      /* Configure the SysTick IRQ priority */
     [/#if]
      if (TickPriority < (1UL << __NVIC_PRIO_BITS))
      {
      [#if FamilyName!="STM32U5" && FamilyName!="STM32H5"]
        /* Configure the TIM IRQ priority */
      [#else]
        /* Enable the ${instance} global Interrupt */
      [/#if]
      [#if DIE != "DIE501"]
        HAL_NVIC_SetPriority(${timeBaseInterrupt}, TickPriority, 0U);
      [#else]
        IRQ_SetPriority(${timeBaseInterrupt}, TickPriority);
      [/#if]        
        uwTickPrio = TickPriority;
      }
      else
      {
        status = HAL_ERROR;
      }
    }
  }

 [#if isTim_callback?? && isTim_callback=="1"]
  HAL_TIM_RegisterCallback(&h${instance?lower_case}, HAL_TIM_PERIOD_ELAPSED_CB_ID, TimeBase_TIM_PeriodElapsedCallback);
 [/#if]
[#if FamilyName=="STM32U5"]  /* Enable the ${instance} global Interrupt */
  HAL_NVIC_EnableIRQ(${timeBaseInterrupt}); 
  [/#if]

 /* Return function status */
  return status;
[#else]
#n
[#if FamilyName=="STM32H5"]
  status = HAL_TIM_Base_Init(&h${instance?lower_case});
  if (status == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    status = HAL_TIM_Base_Start_IT(&h${instance?lower_case});
    if (status == HAL_OK)
    {
      if (TickPriority < (1UL << __NVIC_PRIO_BITS))
      {
        /* Enable the ${instance} global Interrupt */
        HAL_NVIC_SetPriority(${timeBaseInterrupt}, TickPriority, 0U);
        uwTickPrio = TickPriority;
      }
      else
      {
        status = HAL_ERROR;
      }
    }
}

[#if isTim_callback?? && isTim_callback=="1"]
  HAL_TIM_RegisterCallback(&h${instance?lower_case}, HAL_TIM_PERIOD_ELAPSED_CB_ID, TimeBase_TIM_PeriodElapsedCallback);
 [/#if]
  /* Enable the ${instance} global Interrupt */
  [#if DIE != "DIE501"]
  HAL_NVIC_EnableIRQ(${timeBaseInterrupt}); 
  [#else]
  IRQ_Enable(${timeBaseInterrupt});
  [/#if]

  /* Return function status */
  return status; 
[#elseif FamilyName=="STM32WBA"]
  Status = HAL_TIM_Base_Init(&h${instance?lower_case});
  if (Status == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    Status = HAL_TIM_Base_Start_IT(&h${instance?lower_case});
    if (Status == HAL_OK)
    {
        if (TickPriority < (1UL << __NVIC_PRIO_BITS))
        {
          /* Enable the ${instance} global Interrupt */
          HAL_NVIC_SetPriority(${timeBaseInterrupt}, TickPriority, 0U);
          uwTickPrio = TickPriority;
      }
      else
      {
        Status = HAL_ERROR;
      }
    }
}
#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1U)
  HAL_TIM_RegisterCallback(&h${instance?lower_case}, HAL_TIM_PERIOD_ELAPSED_CB_ID, TimeBase_TIM_PeriodElapsedCallback);
#endif
  /* Enable the ${instance} global Interrupt */
  [#if DIE != "DIE501"]
  HAL_NVIC_EnableIRQ(${timeBaseInterrupt});
  [#else]
  IRQ_Enable(${timeBaseInterrupt});
  [/#if]  

  /* Return function Status */
  return Status; 

[#else]
  if(HAL_TIM_Base_Init(&h${instance?lower_case}) == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    return HAL_TIM_Base_Start_IT(&h${instance?lower_case});
  }
  [#if isTim_callback?? && isTim_callback=="1"]
  HAL_TIM_RegisterCallback(&h${instance?lower_case}, HAL_TIM_PERIOD_ELAPSED_CB_ID, TimeBase_TIM_PeriodElapsedCallback);
 [/#if]
 
  /* Return function status */ 
  return HAL_ERROR;
[/#if]
[/#if]
}

/**
  * @brief  Suspend Tick increment.
  * @note   Disable the tick increment by disabling ${instance} update interrupt.
  * @param  None
  * @retval None
  */
void HAL_SuspendTick(void)
{
  /* Disable ${instance} update Interrupt */
  __HAL_TIM_DISABLE_IT(&h${instance?lower_case}, TIM_IT_UPDATE);                                                  
}

/**
  * @brief  Resume Tick increment.
  * @note   Enable the tick increment by Enabling ${instance} update interrupt.
  * @param  None
  * @retval None
  */
void HAL_ResumeTick(void)
{
  /* Enable ${instance} Update interrupt */
  __HAL_TIM_ENABLE_IT(&h${instance?lower_case}, TIM_IT_UPDATE);
}

[#if isTim_callback?? && isTim_callback=="1"]
 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim TIM handle
  * @retval None
  */

void TimeBase_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  HAL_IncTick();
}
[/#if]



