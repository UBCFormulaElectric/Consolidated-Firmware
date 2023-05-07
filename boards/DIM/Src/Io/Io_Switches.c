#include "Io_Switches.h"

#include <stm32f4xx.h>

#include "main.h"

bool Io_Switches_StartSwitchIsTurnedOn(void) {
  return HAL_GPIO_ReadPin(IGNTN_IN_GPIO_Port, IGNTN_IN_Pin) == GPIO_PIN_SET;
}

bool Io_Switches_AuxSwitchIsTurnedOn(void) {
  return HAL_GPIO_ReadPin(AUX_IN_GPIO_Port, AUX_IN_Pin) == GPIO_PIN_SET;
}
