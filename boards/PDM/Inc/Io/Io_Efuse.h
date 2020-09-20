/**
 * @brief Driver for interfacing with the 22XS4200 E-Fuse via SPI
 * @Settings Efuse max SPI transfer rate: 8MHz
 *           SPI Clock Polarity: 0 (SCK low-level idle state)
 *           SPI Clock Phase: 2nd edge (slave samples MOSI on SCK falling edge)
 *           Slave Select: Active Low (Must be toggled between SPI messages)
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "App_SharedExitCode.h"
#include "App_Efuse.h"

struct Efuse_Context;

/**
 * Initialize the SPI handle for the Efuses
 * @param hspi Handle to the SPI peripheral used for the Efuses
 */
struct Efuse_Context *Io_Efuse_Create(
    SPI_HandleTypeDef *const hspi,
    GPIO_TypeDef *           chip_select_port,
    uint16_t                 chip_select_pin);

/**
 * Initialize the SPI handle for the Efuses
 * @param hspi Handle to the SPI peripheral used for the Efuses
 */
void Io_Efuse_Init(SPI_HandleTypeDef *hspi);

/**
 * Set the current/temperature monitoring option of CSNS pin
 * @param selection The desired monitoring function which can be selected by
 * specifying the following values:
 *                  CSNS_FUNCTION_DISABLED - Current sensing disabled
 *                  CSNS_FUNCTION_CURRENT_CH0 - Current sensing for Channel 0
 *                  CSNS_FUNCTION_CURRENT_CH1 - Current sensing for Channel 1
 *                  CSNS_FUNCTION_TEMPERATURE - Efuse temperature sensing
 *                  CSNS_FUNCTION_CURRENT_SUM - Current sensing for summed
 * channels (for Parallel mode)
 * @return EXIT_CODE_OK if the channel configuration was successful
 *         EXIT_CODE_TIMEOUT if one of the SPI writes timed-out
 */
ExitCode Io_Efuse_ConfigureChannelMonitoring(
    uint8_t               selection,
    struct Efuse_Context *e_fuse);

/**
 * Exit fail-safe mode and disable the watchdog timer
 * @return EXIT_CODE_OK if the efuse exited fail-safe mode
 *         EXIT_CODE_TIMEOUT if the one of the SPI writes timed-out
 *         EXIT_CODE_UNIMPLEMENTED if the efuse failed to exit fail-safe mode
 */
ExitCode Io_Efuse_ExitFailSafeMode(struct Efuse_Context *e_fuse);

/**
 * Write data to a specific Serial Input register on the Efuse
 * @param register_address Serial Input register being written to
 * @param register_value Value being written to the Serial Input register
 * @return EXIT_CODE_OK if the write was successful
 *         EXIT_CODE_TIMEOUT if the SPI write timed-out
 */
ExitCode Io_Efuse_WriteRegister(
    uint8_t               register_address,
    uint16_t              register_value,
    struct Efuse_Context *e_fuse);

/**
 * Read data from a specific Serial Output register on the Efuse
 * @param register_address Serial Output register being read from
 * @param register_value register contents
 * @param e_fuse Pointer to Efuse structure being read from
 * @return EXIT_CODE_OK if the read was successful
 *         EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
ExitCode Io_Efuse_ReadRegister(
    uint8_t               register_address,
    uint16_t *            register_value,
    struct Efuse_Context *e_fuse);
