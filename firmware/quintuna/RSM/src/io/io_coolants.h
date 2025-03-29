#pragma once

#include <stdbool.h>

void  io_coolant_init();
void  io_coolant_inputCaptureCallback();
void  io_coolant_checkIfFlowMeterActive();
float io_coolant_getFlowRate();
