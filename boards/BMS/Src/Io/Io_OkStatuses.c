#include "Io_OkStatuses.h"

#include "Io_Hal.h"
#include "main.h"

ExitCode Io_OkStatuses_EnableBmsOk(void) {
  HAL_GPIO_WritePin(BMS_OK_3V3_GPIO_Port, BMS_OK_3V3_Pin, GPIO_PIN_SET);
  return EXIT_CODE_OK;
}

ExitCode Io_OkStatuses_DisableBmsOk(void) {
  HAL_GPIO_WritePin(BMS_OK_3V3_GPIO_Port, BMS_OK_3V3_Pin, GPIO_PIN_RESET);
  return EXIT_CODE_OK;
}

bool Io_OkStatuses_IsBmsOkEnabled(void) {
  return HAL_GPIO_ReadPin(BMS_OK_3V3_GPIO_Port, BMS_OK_3V3_Pin) == GPIO_PIN_SET;
}

ExitCode Io_OkStatuses_EnableImdOk(void) {
  // Unlike BMS_OK, we can't control IMD_OK using the BMS
  return EXIT_CODE_UNIMPLEMENTED;
}

ExitCode Io_OkStatuses_DisableImdOk(void) {
  // Unlike BMS_OK, we can't control IMD_OK using the BMS
  return EXIT_CODE_UNIMPLEMENTED;
}

bool Io_OkStatuses_IsImdOkEnabled(void) {
  return HAL_GPIO_ReadPin(IMD_OK_3V3_GPIO_Port, IMD_OK_3V3_Pin) == GPIO_PIN_SET;
}

ExitCode Io_OkStatuses_EnableBspdOk(void) {
  // Unlike BMS_OK, we can't control BSPD_OK using the BMS
  return EXIT_CODE_UNIMPLEMENTED;
}

ExitCode Io_OkStatuses_DisableBspdOk(void) {
  // Unlike BMS_OK, we can't control BSPD_OK using the BMS
  return EXIT_CODE_UNIMPLEMENTED;
}

bool Io_OkStatuses_IsBspdOkEnabled(void) {
  return HAL_GPIO_ReadPin(BSPD_OK_3V3_GPIO_Port, BSPD_OK_3V3_Pin) ==
         GPIO_PIN_SET;
}
