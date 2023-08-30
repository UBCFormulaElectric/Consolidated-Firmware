#pragma once

/**
 * Check the stack high watermark for FreeRTOS tasks. Typically, you would call
 * this function at 1Hz to minimize overhead. If any stack watermark threshold
 * is exceeded, we log the error.
 */
void dev_stackWaterMark_check(void);
