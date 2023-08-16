#pragma once

#include <stm32f3xx_hal.h>
#include "App_SharedExitCode.h"

/**
 * Initialize all chips on the LTC6813 daisy chain.
 * @param spi_handle The given SPI handle for the LTC6813 daisy chain.
 * @param nss_port The given GPIO port of the NSS pin.
 * @param nss_pin The given GPIO NSS pin.
 */
void Io_LTC6813_Init(SPI_HandleTypeDef *spi_handle, GPIO_TypeDef *nss_port, uint16_t nss_pin);

/**
 * Calculate the 15-bit packet error code (PEC15) for the given data buffer.
 * @param data_buffer A pointer to the buffer containing data used to calculate
 * the PEC15 code.
 * @param size The number of data elements used to calculate the PEC15 code
 * @note The size can be a positive integer less than or equal to the size
 * of the data buffer.
 * @return The calculated PEC15 code for the given data buffer.
 */
uint16_t Io_LTC6813_CalculatePec15(uint8_t *data_buffer, uint32_t size);

/**
 * Transition all LTC6813 chips on the daisy chain from the IDLE state to the
 * READY state
 * @return EXIT_CODE_OK if the SCK and NSS pin can be toggled successfully.
 * Else, EXIT_CODE_ERROR
 */
ExitCode Io_LTC6813_EnterReadyState(void);

/**
 * Send a command to all LTC6813 chips on the daisy chain
 * @param tx_cmd The given command that is transmitted to the LTC6813 chips on
 * the daisy chain
 * @return EXIT_CODE_OK if the command was transmitted successfully. Else,
 * EXIT_CODE_ERROR
 */
ExitCode Io_LTC6813_SendCommand(uint32_t tx_cmd);

/**
 * Wait for the completion of all ADC conversions for the LTC6813 chips on the
 * daisy chain.
 * @return EXIT_CODE_OK if all ADC conversions have completed successfully.
 * Else, EXIT_CODE_TIMEOUT.
 */
ExitCode Io_LTC6813_PollConversions(void);

/**
 * Configure register A for all LTC6813 chips on the LTC6813 daisy chain.
 * @return EXIT_CODE_OK if all chips on the daisy chain are configured
 * successfully. Else, EXIT_CODE_ERROR.
 */
ExitCode Io_LTC6813_ConfigureRegisterA(void);

/**
 * Get the SPI interface configured for the LTC6813 daisy chain.
 * @return The SPI interface configured for the LTC6813 daisy chain.
 */
struct SharedSpi *Io_LTC6813_GetSpiInterface(void);
