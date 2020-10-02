#pragma once

#include <stm32f3xx_hal.h>
#include "App_SharedExitCode.h"

/**
 * Initialize all LTC6813 chips on the daisy chain.
 * @param spi_handle The given SPI handle for the LTC6813 daisy chain.
 * @param nss_port The given GPIO port of the NSS pin.
 * @param nss_pin The given GPIO NSS pin.
 */
void Io_LTC6813_Init(
    SPI_HandleTypeDef *spi_handle,
    GPIO_TypeDef *     nss_port,
    uint16_t           nss_pin);

/**
 * Configure all LTC6813 chips on the daisy chain.
 * @return EXIT_CODE_OK if all chips on the daisy chain are configured
 * successfully. Else, return EXIT_CODE_UNIMPLEMENTED.
 */
ExitCode Io_LTC6813_Configure(void);

/**
 * Read all cell register groups for each chip on the daisy chain.
 * @return  If data can not be transmitted and received from the LTC6813 chips
 * on the daisy chain, return EXIT_CODE_UNIMPLEMENTED. Else if no PEC15
 * mismatches occur, return EXIT_CODE_OK. Else, return EXIT_CODE_ERROR.
 */
ExitCode Io_LTC6813_ReadAllCellRegisterGroups(void);

/**
 * Get the pointer to the 2D array containing converted cell voltages for
 * each chip on the daisy chain.
 * @return The pointer to the 2D array containing converted chip cell
 * voltages.
 */
uint16_t *Io_LTC6813_GetCellVoltages(void);
