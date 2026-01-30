#pragma once
#include <cstdint>
extern "C"
{
#include "io_powerMonitoring.h"
#include "app_canTx.h"
}

/**
 * @brief Update power monitoring CAN signals.
 */
void app_powerMonitoring_update(void);
