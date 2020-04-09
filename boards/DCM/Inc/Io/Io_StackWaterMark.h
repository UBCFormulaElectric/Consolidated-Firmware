#pragma once

#include "App_CanTx.h"

/**
 * Init the StackWaterMark monitor
 * @param can_tx_interface
 */
void App_StackWaterMark_Init(struct DCMCanTxInterface *can_tx_interface);

/**
 * @brief Check the stack high watermark for FreeRTOS tasks. Typically, you
 *        would call this function at 1Hz to minimize overhead. If any stack
 *        watermark threshold is exceeded, we log the error.
 */
void App_StackWaterMark_Check(void);
