#pragma once

#include <stm32f3xx_hal.h>
#include "App_SharedExitCode.h"

/**
 * Initialize the LTC6813 daisy chain.
 * @param hspi The SPI handle for the LTC6813 daisy chain.
 * @param nss_port A pointer to the GPIO port corresponding to the LTC6813 daisy
 * chain.
 * @param nss_pin The GPIO pin corresponding to the LTC6813 daisy chain.
 */
void Io_LTC6813_Init(
    SPI_HandleTypeDef *hspi,
    GPIO_TypeDef *     nss_port,
    uint16_t           nss_pin);

/**
 * Configure all LTC6813 chips on the daisy chain.
 */
void Io_LTC6813_Configure(void);

/**
 * Read all cell register groups for each chip on the daisy chain.
 * @return EXIT_CODE_OK if no PEC15 mismatches occur. Else, return
 * EXIT_CODE_ERROR.
 */
ExitCode Io_LTC6813_ReadAllCellRegisterGroups(void);

/**
 * Get the pointer to the double array containing converted cell voltages for
 * each chip on the daisy chain.
 * @return The pointer to the double array containing converted chip cell
 * voltages.
 */
uint16_t *Io_LTC6813_GetCellVoltages(void);
