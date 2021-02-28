#pragma once

#include <assert.h>
#include <math.h>
#include "App_Efuse.h"
#include "Io_Efuse.h"
#include "Io_CurrentSense.h"

/**
 * Initialize the Aux1Aux2 efuse.
 * @param spi_handle Handle to the SPI peripheral used for the Efuse
 */
void Io_Aux1Aux2Efuse_Init(SPI_HandleTypeDef *spi_handle);

/**
 * Enable the Aux 1 channel output for the Aux1Aux2 efuse.
 */
void Io_Aux1Aux2Efuse_EnableAux1(void);

/**
 * Disable the Aux 1 channel output for the Aux1Aux2 efuse.
 */
void Io_Aux1Aux2Efuse_DisableAux1(void);

/**
 * Enable the Aux 2 channel output for the Aux1Aux2 efuse.
 */
void Io_Aux1Aux2Efuse_EnableAux2(void);

/**
 * Disable the Aux 2 channel output for the Aux1Aux2 efuse.
 */
void Io_Aux1Aux2Efuse_DisableAux2(void);

/**
 * Get the value of the Aux1Aux2 efuse's status register for the Aux1Aux2 efuse.
 * @param channel_status Pointer to the Aux1 and Aux2 channel's status
 * @return EXIT_CODE_OK if the read was successful,
 *         else EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
ExitCode Io_Aux1Aux2Efuse_GetStatus(enum Efuse_Status *channel_status);

/**
 * Get the value of the Aux 1 fault register for the Aux1Aux2 efuse.
 * @note Reading from the fault register reset's non-latchable faults
 * @param aux1_fault_status Pointer to the Aux 1 fault status
 * @return EXIT_CODE_OK if the read was successful,
 *         else EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
ExitCode Io_Aux1Aux2Efuse_GetAux1Faults(enum Efuse_Fault *aux1_fault_status);

/**
 * Get the value of the Aux 2 fault register for the Aux1Aux2 efuse.
 * @note Reading from the fault register reset's non-latchable faults
 * @param aux2_fault_status Pointer to the Aux 2 fault status
 * @return EXIT_CODE_OK if the read was successful,
 *         else EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
ExitCode Io_Aux1Aux2Efuse_GetAux2Faults(enum Efuse_Fault *aux2_fault_status);

/**
 * Check if the efuse's Aux 1 or Aux 2 channel on the Aux1Aux2 efuse enters
 * fault mode.
 * @return true if the efuse's Aux 1 or Aux 2 channel enters fault mode, else
 * false if both channels do not enter fault mode
 */
bool Io_Aux1Aux2Efuse_IsInFaultMode(void);

/**
 * Check if the Aux1/Aux2 efuse enters fail-safe mode.
 * @return true if the efuse enters fail-safe mode, else false
 * if the efuse does not enter fail-safe mode
 */
bool Io_Aux1Aux2Efuse_IsInFailSafeMode(void);

/**
 * Delatch the Aux1Aux2 efuse's latchable faults.
 */
void Io_Aux1Aux2Efuse_DelatchFaults(void);

/**
 * Get the current reading for the Aux 1 channel in Amps [A] on the Aux1Aux2
 * efuse.
 * @return The Aux 1 current if the current was measured, else NAN
 */
float Io_Aux1Aux2Efuse_GetAux1Current(void);

/**
 * Get the current reading for the Aux 2 channel in Amps [A] on the Aux1Aux2
 * efuse.
 * @return The Aux 2 current if the current was measured, else NAN
 */
float Io_Aux1Aux2Efuse_GetAux2Current(void);

/**
 * Configure the Aux1Aux2 efuse's registers using the values defined in:
 *                  -GCR_CONFIG
 *                  -RETRY_CONFIG
 *                  -CONFR_CONFIG
 *                  -OCR_LOW_CURRENT_SENSE_CONFIG
 * @return EXIT_CODE_OK if the configuration was successful,
 *         else EXIT_CODE_TIMEOUT if one of the SPI writes timed-out
 */
ExitCode Io_Aux1Aux2Efuse_ConfigureEfuse(void);
