
/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    smbus.c
 * @brief   This file provides code for the configuration
 *          of the I2C instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "stm32_config_stack.h"
#include "stm32_PMBUS_stack.h"
#include "main.h"

/* ARP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SMBUS_StackHandleTypeDef  context;
SMBUS_StackHandleTypeDef *pcontext;
#define SMBUS_HANDLE_instance hsmbus1
extern SMBUS_HandleTypeDef SMBUS_HANDLE_instance;

/* stack init function */
void MX_STACK_SMBUS_Init(void)
{
    /* USER CODE BEGIN SMBUS_Init 0 */

    /* USER CODE END SMBUS_Init 0 */

    /* USER CODE BEGIN SMBUS_Init 1 */

    /* USER CODE END SMBUS_Init 1 */

    context.CMD_table      = (st_command_t *)&PMBUS_COMMANDS_TAB[0];
    context.CMD_tableSize  = PMBUS_COMMANDS_TAB_SIZE;
    context.Device         = &SMBUS_HANDLE_instance;
    context.SRByte         = 0x55U;
    context.CurrentCommand = NULL;

    /* In SMBUS 10-bit addressing is reserved for future use */
    assert_param(SMBUS_HANDLE_instance.Init.AddressingMode == SMBUS_ADDRESSINGMODE_7BIT);
    context.OwnAddress = SMBUS_HANDLE_instance.Init.OwnAddress1;
    /* Address resolved state */
    context.StateMachine = SMBUS_SMS_ARP_AR;
    /* checking the HAL slave setting */
    assert_param(SMBUS_HANDLE_instance.Init.PeripheralMode != SMBUS_PERIPHERAL_MODE_SMBUS_HOST);
    context.StateMachine |= SMBUS_SMS_PEC_ACTIVE;
    /* checking the HAL is in accord */
    assert_param(SMBUS_HANDLE_instance.Init.PacketErrorCheckMode == SMBUS_PEC_ENABLE);
    pcontext = &context;

    if (STACK_SMBUS_Init(pcontext) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN SMBUS_Init 2 */

    /* USER CODE END SMBUS_Init 2 */
}

/**
  * @brief  Callback function notifying about extended command incoming, implementation
    is supporting extended command defined by PMBus
  * @param  pStackContext : Pointer to a SMBUS_StackHandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval HAL_StatusTypeDef response code. Equal STACK_OK if success, any other value means problem
  */
HAL_StatusTypeDef STACK_SMBUS_ExtendCommand(SMBUS_StackHandleTypeDef *pStackContext)
{
    uint16_t size   = 0U;
    uint8_t *piobuf = NULL;

    /* accessing the IO buffer */
    piobuf = STACK_SMBUS_GetBuffer(pStackContext);

    /*
      Extended command must be identified by the value of the actual command code
     */
    switch (piobuf[0])
    {
        case 0:
            pStackContext->CurrentCommand = (st_command_t *)&EXTENDED_READ_BYTE;
            break;
        case 1:
            pStackContext->CurrentCommand = (st_command_t *)&EXTENDED_READ_WORD;
            break;
        case 2:
            pStackContext->CurrentCommand = (st_command_t *)&EXTENDED_WRITE_BYTE;
            /*
              size of the bytes yet to be received
             */
            size = 1U;
            break;
        case 3:
            pStackContext->CurrentCommand = (st_command_t *)&EXTENDED_WRITE_WORD;
            size                          = 2U;
            break;
        default:
            pStackContext->StateMachine |= SMBUS_SMS_ERROR;
            return STACK_ERROR;
    }

    /*
      reading the remaining data (stack won't do that for us this time
    */
    if ((pStackContext->CurrentCommand->cmnd_query & WRITE) == WRITE)
    {
        /*
          To make sure the executecommand is called again once the remaining data
          is in the buffer
        */
        STACK_PMBUS_ExtendExecution(pStackContext);

        /*
        PEC byte y/n?
        */
        if ((pStackContext->StateMachine & SMBUS_SMS_PEC_ACTIVE) == SMBUS_SMS_PEC_ACTIVE)
        {
            size += PEC_SIZE;
        }

        /*
          asking the HAL for more bytes to receive
        */
        pStackContext->Byte_count += size;
        HAL_SMBUS_Slave_Receive_IT(pStackContext->Device, &(pStackContext->Buffer[2]), size, SMBUS_NEXT_FRAME);
    }

    return STACK_OK;
}

/**
  * @brief  Callback function notifying slave about command incoming, implementation
    is supporting extended command defined by PMBus
  * @param  pStackContext : Pointer to a SMBUS_StackHandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval HAL_StatusTypeDef response code. Equal STACK_OK if success, any other value means problem
  */
HAL_StatusTypeDef STACK_SMBUS_ExecuteCommand(SMBUS_StackHandleTypeDef *pStackContext)
{
    uint8_t *piobuf = NULL;

    /* accessing the IO buffer */
    piobuf = STACK_SMBUS_GetBuffer(pStackContext);

    if (piobuf == NULL)
    {
        pStackContext->StateMachine |= SMBUS_SMS_ERROR;
    }
    else if (pStackContext->CurrentCommand == (st_command_t *)&HOST_NOTIFY_PROTOCOL)
    {
        /* host notify command */
        if (pStackContext->Buffer[0] == SMBUS_ADDR_DEFAULT)
        {
            /* Usually the Host notify is used for ARP, but may not be limited to it */

            /* ARP */
        }
    }
    else /* Normal device command execution */
    {
        /* Zone config command example implementation */
        if (pStackContext->CurrentCommand->cmnd_code == PMBC_ZONE_CONFIG)
        {
            pStackContext->TheZone.writeZone = pStackContext->Buffer[1];
            pStackContext->TheZone.readZone  = pStackContext->Buffer[2];
        }
        else if (pStackContext->CurrentCommand->cmnd_code == PMBC_ZONE_ACTIVE)
        {
            pStackContext->TheZone.activeWriteZone = pStackContext->Buffer[1];
            pStackContext->TheZone.activeReadZone  = pStackContext->Buffer[2];
        }

        /* PMBUS13 */
        /*
          first step is to see if we have a case of extended command
        */
        if (pStackContext->CurrentCommand->cmnd_code == PMBC_PMBUS_COMMAND_EXT)
        {
            /*
             implement here whatever is to be done on reception of extended command
             before actually receiving the extension
            */
        }
        else /* regular command case */
        {
            if ((pStackContext->CurrentCommand->cmnd_query & BLOCK) == BLOCK)
            {
                *piobuf = (pStackContext->CurrentCommand->cmnd_master_Rx_size) - 1U;
                /* byte size of reply for block read command */
                /* One byte for size, rest are [size] of data */
            }
        }
    }
    return STACK_OK;
}

/**
  * @brief  Stub of an error treatment function - set to ignore most errors.
            Defined "weak" for suggested replacement.
  * @param  pStackContext : Pointer to a SMBUS_StackHandleTypeDef structure that contains
  *                the configuration information for the specified SMBUS.
  * @retval None
  */
__weak void MX_SMBUS_Error_Check(SMBUS_StackHandleTypeDef *pStackContext)
{
    if ((STACK_SMBUS_IsBlockingError(pStackContext)) || (STACK_SMBUS_IsCmdError(pStackContext)))
    {
        /* No action, error symptoms are ignored */
        pStackContext->StateMachine &= ~(SMBUS_ERROR_CRITICAL | SMBUS_COM_ERROR);
    }
    else if ((pStackContext->StateMachine & SMBUS_SMS_ERR_PECERR) == SMBUS_SMS_ERR_PECERR) /* PEC error, we won't wait
                                                                                              for any more action */
    {
        pStackContext->StateMachine |= SMBUS_SMS_READY;
        pStackContext->CurrentCommand = NULL;
        pStackContext->StateMachine &= ~(SMBUS_SMS_ACTIVE_MASK | SMBUS_SMS_ERR_PECERR);
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
