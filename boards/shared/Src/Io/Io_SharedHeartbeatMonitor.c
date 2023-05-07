#include "Io_SharedHeartbeatMonitor.h"

#include <cmsis_os.h>

uint32_t Io_SharedHeartbeatMonitor_GetCurrentMs(void) {
  return osKernelSysTick() * portTICK_RATE_MS;
}
