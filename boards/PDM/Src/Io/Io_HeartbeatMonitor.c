#include <cmsis_os.h>

uint32_t Io_HeartbeatMonitor_GetCurrentMs(void) {
  return osKernelSysTick() * portTICK_RATE_MS;
}
