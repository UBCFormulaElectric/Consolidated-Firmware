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

struct Efuse_Context;

/**
 * Allocate and initialize an Efuse
 * @param hspi Handle to the SPI peripheral used for the Efuse
 * @param chip_select_port Handle to Efuse's chip-select GPIO port
 * @param chip_select_port Handle to Efuse's chip-select GPIO pin
 * @param fsob_port Handle to Efuse's fail-safe output GPIO port
 * @param fsob_pin Handle to Efuse's fail-safe output GPIO pin
 * @param fsb_port Handle to Efuse's fault status GPIO port
 * @param fsb_pin Handle to Efuse's fault status GPIO pin
 * @param channel0_port Handle to Efuse's channel 0 GPIO port
 * @param channel0_pin Handle to Efuse's channel 0 GPIO pin
 * @param channel1_port Handle to Efuse's channel 1 GPIO port
 * @param channel1_pin Handle to Efuse's channel 1 GPIO pin
 * @return The created Efuse, whose ownership is given to the caller
 */
struct Efuse_Context *Io_Efuse_Create(
    SPI_HandleTypeDef *spi_handle,
    GPIO_TypeDef *     nss_port,
    uint16_t           nss_pin,
    GPIO_TypeDef *     fsob_port,
    uint16_t           fsob_pin,
    GPIO_TypeDef *     fsb_port,
    uint16_t           fsb_pin,
    GPIO_TypeDef *     channel0_port,
    uint16_t           channel0_pin,
    GPIO_TypeDef *     channel1_port,
    uint16_t           channel1_pin);

/**
 * Deallocate the memory used by the given Efuse
 * @param e_fuse The Efuse to deallocate
 */
void Io_Efuse_Destroy(struct Efuse_Context *e_fuse);

/**
 * Enable the given efuse's channel 0 output.
 */
void Io_Efuse_EnableChannel0(const struct Efuse_Context *const e_fuse);

/**
 * Disable the given efuse's channel 0 output.
 */
void Io_Efuse_DisableChannel0(const struct Efuse_Context *const e_fuse);

/**
 * Enable the given efuse's channel 1 output.
 */
void Io_Efuse_EnableChannel1(const struct Efuse_Context *const e_fuse);

/**
 * Disable the given efuse's channel 1 output.
 */
void Io_Efuse_DisableChannel1(const struct Efuse_Context *const e_fuse);

/**
 * Set the current or temperature monitoring option of the CSNS pin.
 * @param selection The desired monitoring function which can be selected by
 * specifying the following values:
 *                  CSNS_FUNCTION_DISABLED - Current sensing disabled
 *                  CSNS_FUNCTION_CURRENT_CH0 - Current sensing for Channel 0
 *                  CSNS_FUNCTION_CURRENT_CH1 - Current sensing for Channel 1
 *                  CSNS_FUNCTION_TEMPERATURE - Efuse temperature sensing
 *                  CSNS_FUNCTION_CURRENT_SUM - Current sensing for summed
 * channels (for Parallel mode)
 * @param e_fuse Pointer to the Efuse structure for the Efuse being configured
 * @return EXIT_CODE_OK if the channel configuration was successful
 *         EXIT_CODE_TIMEOUT if one of the SPI writes timed-out
 */
ExitCode Io_Efuse_ConfigureChannelMonitoring(
    uint8_t                     selection,
    struct Efuse_Context *const e_fuse);

/**
 * Exit fail-safe mode and disable the watchdog timer
 * @param e_fuse Pointer to the Efuse structure for the Efuse being configured
 * @return EXIT_CODE_OK if the efuse exited fail-safe mode
 *         EXIT_CODE_TIMEOUT if the one of the SPI writes timed-out
 *         EXIT_CODE_UNIMPLEMENTED if the efuse failed to exit fail-safe mode
 */
ExitCode Io_Efuse_ExitFailSafeMode(struct Efuse_Context *const e_fuse);

/**
 * Check if the efuse's channel 0 or channel 1 enters fault mode
 * @param e_fuse Pointer to the Efuse structure for the Efuse being checked
 * @return true if the fuse's channel 0 or channel 1 enters fault mode, else
 * false if both channels do not enter fault mode
 */
bool Io_Efuse_IsEfuseInFaultMode(const struct Efuse_Context *const e_fuse);

/**
 * Check if the efuse enters fail-safe mode
 * @param e_fuse Pointer to the Efuse structure for the Efuse being checked
 * @return true if the efuse enters fail-safe mode, else false
 * if the efuse does not enter fail-safe mode
 */
bool Io_Efuse_IsEfuseInFailSafeMode(const struct Efuse_Context *const e_fuse);

/**
 * Delatch the efuse's latchable faults
 * @param e_fuse Pointer to the Efuse structure for the Efuse being checked
 * @note Non-latchable faults are reset by reading from the fault registers
 */
void Io_Efuse_DelatchFaults(const struct Efuse_Context *const e_fuse);

/**
 * Write data to a specific Serial Input register on the Efuse
 * @param register_address Serial Input register being written to
 * @param register_value Value being written to the Serial Input register
 * @param e_fuse Pointer to Efuse structure being written to
 * @return EXIT_CODE_OK if the write was successful
 *         EXIT_CODE_TIMEOUT if the SPI write timed-out
 */
ExitCode Io_Efuse_WriteRegister(
    uint8_t                     register_address,
    uint16_t                    register_value,
    struct Efuse_Context *const e_fuse);

/**
 * Read data from a specific Serial Output register on the Efuse
 * @param register_address Serial Output register being read from
 * @param register_value register contents
 * @param e_fuse Pointer to Efuse structure being read from
 * @return EXIT_CODE_OK if the read was successful
 *         EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
ExitCode Io_Efuse_ReadRegister(
    uint8_t                     register_address,
    uint16_t *                  register_value,
    struct Efuse_Context *const e_fuse);
