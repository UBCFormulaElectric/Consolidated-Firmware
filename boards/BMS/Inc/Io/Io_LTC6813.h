#pragma once

#include <stm32f3xx_hal.h>
#include "App_SharedExitCode.h"

/**
 * Initialize all chips on the LTC6813 daisy chain.
 * @param spi_handle The given SPI handle for the LTC6813 daisy chain.
 * @param nss_port The given GPIO port of the NSS pin.
 * @param nss_pin The given GPIO NSS pin.
 */
void Io_LTC6813_Init(
    SPI_HandleTypeDef *spi_handle,
    GPIO_TypeDef *     nss_port,
    uint16_t           nss_pin);

/**
 * Configure all chips on the LTC6813 daisy chain.
 * @return EXIT_CODE_OK if all chips on the daisy chain are configured
 * successfully. Else, return EXIT_CODE_TIMEOUT if the data cannot be
 * transmitted to the chips on the LTC6813 daisy chain.
 */
ExitCode Io_LTC6813_Configure(void);

/**
 * Read all cell register groups for each chip on the LTC6813 daisy chain.
 * @return If data cannot be transmitted to or received from the chips on the
 * LTC6813 daisy chain, return EXIT_CODE_TIMEOUT. Else if PEC15 mismatches
 * are detected, return EXIT_CODE_ERROR. Else, return EXIT_CODE_OK.
 */
ExitCode Io_LTC6813_ReadAllCellRegisterGroups(void);

/**
 * Get the pointer to the 2D array containing converted cell voltages for
 * each chip on the daisy chain.
 *
 * @note The 2D array has its first subscript value as NUM_OF_LTC6813 (the total
 * number of LTC6813 chips connected on the daisy chain), and its second
 * subscript value as NUM_OF_CELLS_PER_LTC6813 (the number of cell voltages that
 * can be monitored per LTC6813 chip).
 * Each cell voltage read back from the LTC6813 daisy chain is a 16-bit unsigned
 * integer where the LSB represents 100µV.
 *
 * @return A pointer to the 2D array containing converted chip cell
 * voltages.
 */
uint16_t *Io_LTC6813_GetCellVoltages(void);
