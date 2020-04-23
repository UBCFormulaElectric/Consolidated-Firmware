#pragma once

#include <stdbool.h>

bool has_fsm_critical_fault(void* can_rx);
bool has_dcm_critical_fault(void* can_rx);
bool has_pdm_critical_fault(void* can_rx);
bool has_bms_critical_fault(void* can_rx);
bool has_critical_fault(void *can_rx);
