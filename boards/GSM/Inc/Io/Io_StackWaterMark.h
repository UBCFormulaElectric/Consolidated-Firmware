#pragma once

struct GsmCanTxInterface;

/**
 * Register the CAN TX interface to be used in stack high watermark error
 * callbacks
 * @param can_tx_interface The CAN TX interface to register
 */
void Io_StackWaterMark_Init(struct GsmCanTxInterface *can_tx_interface);

/**
 * Check the stack high watermark for FreeRTOS tasks. Typically, you would call
 * this function at 1Hz to minimize overhead. If any stack watermark threshold
 * is exceeded, we log the error.
 */
void Io_StackWaterMark_Check(void);
