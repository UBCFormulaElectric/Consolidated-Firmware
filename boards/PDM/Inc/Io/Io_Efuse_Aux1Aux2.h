#pragma once

#include <assert.h>
#include <math.h>
#include "Io_Efuse.h"
#include "Io_CurrentSense.h"

/**
 * Initialize the Aux1/Aux2 Efuse
 * @param hspi Handle to the SPI peripheral used for the Efuse
 */
void Io_Efuse_Init_Aux1Aux2(SPI_HandleTypeDef *const hspi);

/**
 * Enable the Aux 1 channel output
 */
void Io_Efuse_Aux1Enable(void);

/**
 * Disable the Aux 1 channel output
 */
void Io_Efuse_Aux1Disable(void);

/**
 * Disable the Aux 2 channel output
 */
void Io_Efuse_Aux1Disable(void);

/**
 * Enable the Aux 2 channel output
 */
void Io_Efuse_Aux2Enable(void);

/**
 * Get the value of the Aux 1 and Aux 2 Efuse's Status Register over SPI
 * @param status Pointer to the Aux1 and Aux2 channel's status
 * @return EXIT_CODE_OK if the read was successful,
 *         else EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
ExitCode Io_Efuse_GetAux1Aux2Status(enum Efuse_Status *status);

/**
 * Get the value of the Aux 1 Fault Register over SPI
 * @note Reading from the fault register reset's non-latchable faults
 * @param fault Pointer to the Aux 1 fault status
 * @return EXIT_CODE_OK if the read was successful,
 *         else EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
ExitCode Io_Efuse_GetAux1Faults(enum Efuse_Fault *fault);

/**
 * Get the value of the Aux 2 Fault Register over SPI
 * @note Reading from the fault register reset's non-latchable faults
 * @param fault Pointer to the Aux 2 fault status
 * @return EXIT_CODE_OK if the read was successful,
 *         else EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
ExitCode Io_Efuse_GetAux2Faults(enum Efuse_Fault *fault);

/**
 * Get the current readings for the Aux 1 channel
 * @return The Aux1 current if the current was measured, else NAN
 */
float Io_Efuse_GetAux1Current(void);

/**
 * Get the current readings for the Aux 2 channel
 * @return The Aux2 current if the current was measured, else NAN
 */
float Io_Efuse_GetAux2Current(void);

/**
 * Configure the Aux1/Aux2 Efuse's registers using the values defined in:
 *                  -GCR_CONFIG
 *                  -RETRY_CONFIG
 *                  -CONFR_CONFIG
 *                  -OCR_LOW_CURRENT_SENSE_CONFIG
 * @return EXIT_CODE_OK if the configuration was successful,
 *         else EXIT_CODE_TIMEOUT if one of the SPI writes timed-out
 */
ExitCode Io_Efuse_ConfigureAux1Aux2Efuse(void);
