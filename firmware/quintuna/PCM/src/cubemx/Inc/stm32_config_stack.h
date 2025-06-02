
/**
 ******************************************************************************
 * @file    stm32_config_stack.h
 * @author  MCD Application Team
 * @version V2.0.0
 * @date    8-August-2016
 * @brief   This file provides a set of functions needed to manage the SMBUS STACK.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONFIG_STACK_H
#define __CONFIG_STACK_H

#ifdef __cplusplus
extern "C"
{
#endif

    /* Includes ------------------------------------------------------------------*/

    /** @addtogroup STM32_SMBUS_STACK     SMBus 2.0 stack implementation
     * @{
     */

    /** @defgroup STM32_SMBUS_STACK_Defines SMBus stack configuration
     * @{
     */

#define DEV_DIS

#define DEV_PSA

#define DENSE_CMD_TABLE

#define DEV_PEC

#define PMBUS13
    /**
     * @}
     */

    /**
     * @}
     */

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_STACK_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
