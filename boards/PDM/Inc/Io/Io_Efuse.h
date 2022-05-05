/**
 * @brief Driver for interfacing with the 22XS4200 efuse via SPI
 * @Settings Efuse max SPI transfer rate: 8MHz
 *           SPI Clock Polarity: 0 (SCK low-level idle state)
 *           SPI Clock Phase: 2nd edge (slave samples MOSI on SCK falling edge)
 *           Slave Select: Active Low (must be toggled between SPI messages)
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "App_SharedExitCode.h"

struct Efuse_Context;

/**
 * Allocate and initialize an efuse.
 * @param get_channel_0_current A function that can be called to get the
 * measured current for channel 0
 * @param get_channel_1_current A function that can be called to get the
 * measured current for channel 1
 * @param hspi Handle to the SPI peripheral used for the efuse
 * @param chip_select_port Handle to efuse's chip-select GPIO port
 * @param chip_select_port Handle to efuse's chip-select GPIO pin
 * @param fsob_port Handle to efuse's fail-safe output GPIO port
 * @param fsob_pin Handle to efuse's fail-safe output GPIO pin
 * @param fsb_port Handle to efuse's fault status GPIO port
 * @param fsb_pin Handle to efuse's fault status GPIO pin
 * @param channel0_port Handle to efuse's channel 0 GPIO port
 * @param channel0_pin Handle to efuse's channel 0 GPIO pin
 * @param channel1_port Handle to efuse's channel 1 GPIO port
 * @param channel1_pin Handle to efuse's channel 1 GPIO pin
 * @return The created efuse, whose ownership is given to the caller
 */
struct Efuse_Context *Io_Efuse_Create(
    float (*get_channel_0_current)(void),
    float (*get_channel_1_current)(void),
    SPI_HandleTypeDef *spi_handle,
    GPIO_TypeDef *     nss_port,
    uint16_t           nss_pin,
    GPIO_TypeDef *     fsob_port,
    uint16_t           fsob_pin,
    GPIO_TypeDef *     fsb_port,
    uint16_t           fsb_pin,
    GPIO_TypeDef *     current_sync_port,
    uint16_t           current_sync_pin,
    GPIO_TypeDef *     channel_0_port,
    uint16_t           channel_0_pin,
    GPIO_TypeDef *     channel_1_port,
    uint16_t           channel_1_pin);

/**
 * Deallocate the memory used by the given efuse.
 * @param e_fuse The efuse to deallocate
 */
void Io_Efuse_Destroy(struct Efuse_Context *efuse);

/**
 * Enable the given efuse's channel 0 output.
 * @param efuse Pointer to the given efuse
 */
void Io_Efuse_EnableChannel0(const struct Efuse_Context *const efuse);

/**
 * Disable the given efuse's channel 0 output.
 * @param efuse Pointer to the given efuse
 */
void Io_Efuse_DisableChannel0(const struct Efuse_Context *const efuse);

/**
 * Enable the given efuse's channel 1 output.
 * @param efuse Pointer to the given efuse
 */
void Io_Efuse_EnableChannel1(const struct Efuse_Context *const efuse);

/**
 * Disable the given efuse's channel 1 output.
 * @param efuse Pointer to the given efuse
 */
void Io_Efuse_DisableChannel1(const struct Efuse_Context *const efuse);

/**
 * Set the current or temperature monitoring option of the CSNS pin for the
 * given efuse.
 * @param monitoring_function The desired monitoring function which can be
 * selected by specifying the following values:
 *                  CSNS_FUNCTION_DISABLED - Current sensing disabled
 *                  CSNS_FUNCTION_CURRENT_CH0 - Current sensing for Channel 0
 *                  CSNS_FUNCTION_CURRENT_CH1 - Current sensing for Channel 1
 *                  CSNS_FUNCTION_TEMPERATURE - Efuse temperature sensing
 *                  CSNS_FUNCTION_CURRENT_SUM - Current sensing for summed
 * channels (for Parallel mode)
 * @param efuse Pointer to the efuse structure for the efuse being configured
 * @return EXIT_CODE_OK if the channel configuration was successful
 *         EXIT_CODE_TIMEOUT if one of the SPI writes timed-out
 */
ExitCode Io_Efuse_ConfigureChannelMonitoring(uint8_t monitoring_function, struct Efuse_Context *const efuse);

/**
 * Exit fail-safe mode and disable the watchdog timer for the given efuse.
 * @param efuse Pointer to the efuse structure for the efuse being configured
 * @return EXIT_CODE_OK if the efuse exited fail-safe mode
 *         EXIT_CODE_TIMEOUT if the one of the SPI writes timed-out
 *         EXIT_CODE_UNIMPLEMENTED if the efuse failed to exit fail-safe mode
 */
ExitCode Io_Efuse_ExitFailSafeMode(struct Efuse_Context *const efuse);

/**
 * Check if the given efuse's channel 0 or channel 1 enters fault mode.
 * @param efuse Pointer to the efuse structure for the efuse being checked
 * @return true if the efuse's channel 0 or channel 1 enters fault mode, else
 * false if both channels do not enter fault mode
 */
bool Io_Efuse_IsEfuseInFaultMode(const struct Efuse_Context *const efuse);

/**
 * Check if the the given efuse entered the fail-safe mode.
 * @param efuse Pointer to the efuse structure for the efuse being checked
 * @return true if the efuse enters fail-safe mode, else false
 * if the efuse does not enter fail-safe mode
 */
bool Io_Efuse_IsEfuseInFailSafeMode(const struct Efuse_Context *const efuse);

/**
 * Delatch the given efuse's latchable faults.
 * @param efuse Pointer to the efuse structure for the efuse being checked
 * @note Non-latchable faults are reset by reading from the fault registers
 */
void Io_Efuse_DelatchFaults(const struct Efuse_Context *const efuse);

/**
 * Get the channel 0 current reading in Amps [A] for the given efuse.
 * @param efuse Pointer to the given efuse
 * @return The channel 0 current if the current was measured, else NAN
 */
float Io_Efuse_GetChannel0Current(struct Efuse_Context *const efuse);

/**
 * Get the channel 1 current reading in Amps [A] for the given efuse.
 * @param efuse Pointer to the given efuse
 * @return The channel 1 current if the current was measured, else NAN
 */
float Io_Efuse_GetChannel1Current(struct Efuse_Context *const efuse);

/**
 * Write data to a specific serial input register on the given efuse.
 * @param register_address Serial input register being written to
 * @param register_value The value being written to the serial input register
 * @param efuse Pointer to efuse structure being written to
 * @return EXIT_CODE_OK if the write was successful
 *         EXIT_CODE_TIMEOUT if the SPI write timed-out
 */
ExitCode Io_Efuse_WriteRegister(uint8_t register_address, uint16_t register_value, struct Efuse_Context *const efuse);

/**
 * Read data from a specific serial output register for the given efuse.
 * @param register_address Serial output register being read from
 * @param register_value The value read back from the serial output register
 * @param efuse Pointer to efuse structure being read from
 * @return EXIT_CODE_OK if the read was successful
 *         EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
ExitCode Io_Efuse_ReadRegister(uint8_t register_address, uint16_t *register_value, struct Efuse_Context *const efuse);
