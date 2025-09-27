#include "app_canUtils.h"
#include <stdint.h>

void hardware_reset (inv_type inverter);
void inverter_retry (inv_type inverter);
fault_handler invfr_error_handling (uint32_t *INVFR_ErrorInfo_val, uint32_t *INVFL_ErrorInfo_val, uint32_t *INVRR_ErrorInfo_val, uint32_t *INVRL_ErrorInfo_val);
