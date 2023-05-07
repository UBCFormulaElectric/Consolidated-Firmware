#include "Io_LTC3786.h"

#include "Io_Hal.h"
#include "main.h"

bool Io_LTC3786_HasFault(void) {
  return HAL_GPIO_ReadPin(PGOOD_GPIO_Port, PGOOD_Pin) == GPIO_PIN_RESET;
}
