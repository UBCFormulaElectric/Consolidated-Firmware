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

struct Efuse;

//! Enumeration for the status types
enum Efuse_Status
{
    StatusType_NoFault = (0 << 0), //!< No fault
    StatusType_OUT0    = (1 << 0), //!< Channel 0 Output state (ON/OFF)
    StatusType_OUT1    = (1 << 1), //!< Channel 0 Output state (ON/OFF)
    StatusType_FAULT0  = (1 << 2), //!< Fault on Channel 0
    StatusType_FAULT1  = (1 << 3), //!< Fault on Channel 1
    StatusType_R_FULL0 = (1 << 4), //!< Channel 0 Auto-retry Counter Full
    StatusType_R_FULL1 = (1 << 5), //!< Channel 1 Auto-retry Counter Full
    StatusType_POR     = (1 << 6), //!< Power-on Reset Has Occurred
    StatusType_UV      = (1 << 7), //!< Under Voltage Fault
    StatusType_OV      = (1 << 8)  //!< Over Voltage Fault
};

//! Enumeration for the fault types
enum Efuse_Fault
{
    FaultType_NoFault = (0 << 0), //!< No fault
    FaultType_OC      = (1 << 0), //!< Over Current fault
    FaultType_SC      = (1 << 1), //!< Severe Short Circuit fault
    FaultType_OT      = (1 << 2), //!< Over Temperature fault
    FaultType_OS      = (1 << 3), //!< Output Shorted to Vpwr fault
    FaultType_OLOFF   = (1 << 4), //!< Open Load in OFF State fault
    FaultType_OLON    = (1 << 5), //!< Open Load in ON State fault
    FaultType_OTW     = (1 << 8)  //!< Over Temperature Warning fault
};

/**
 * Initializes the SPI handle for the Efuses
 * @param hspi Handle to the SPI peripheral used for the Efuses
 */
void Io_Efuse_Init(SPI_HandleTypeDef *const hspi);

/**
 * Get the value of the Aux 1 and Aux 2 Efuse's Status Register over SPI
 * @param e_fuse Pointer to the Aux1/Aux2 Efuse structure being checked
 * @return Contents of the status register
 */
enum Efuse_Status Io_Efuse_GetAux1_Aux2Status(struct Efuse *e_fuse);

/**
 * Get the value of the Aux 1 Fault Register over SPI
 * @note Reading from the fault register reset's non-latchable faults
 * @param e_fuse Pointer to the Aux1/Aux2 Efuse structure being checked
 * @return Contents of the Aux 1 fault register
 */
enum Efuse_Fault Io_Efuse_GetAux1Faults(struct Efuse *e_fuse);

/**
 * Get the value of the Aux 2 Fault Register over SPI
 * @param e_fuse Pointer to the Aux1/Aux2 Efuse structure being checked
 * @return Contents of the Aux 2 fault register
 */
enum Efuse_Fault Io_Efuse_GetAux2Faults(struct Efuse *e_fuse);

/**
 * Get the current readings for the Aux 1 channel
 * @param e_fuse Pointer to the Aux1/Aux2 Efuse structure being checked
 * @param aux1_current Pointer to the Aux 1 current
 * @return true if the current was measured, else false
 */
bool Io_Efuse_GetAux1Current(struct Efuse *e_fuse, float *aux1_current);

/**
 * Get the current readings for the Aux 2 channel
 * @param e_fuse Pointer to the Aux1/Aux2 Efuse structure being checked
 * @param aux1_current Pointer to the Aux 2 current
 * @return true if the current was measured, else false
 */
bool Io_Efuse_GetAux2Current(struct Efuse *e_fuse, float *aux2_current);

/**
 * Sets current/temperature monitoring option of CSNS pin
 * @param selection The desired monitoring function which can be selected by
 * specifying the following values:
 *                  CSNS_FUNCTION_DISABLED - Current sensing disabled
 *                  AUX1_CURRENT_SENSE_CHANNEL - Current sensing for Aux 1
 *                  AUX2_CURRENT_SENSE_CHANNEL - Current sensing for Aux 2
 *                  CSNS_FUNCTION_TEMPERATURE - Ffuse temperature sensing
 *                  CSNS_FUNCTION_CURRENT_SUM - Current sensing for summed
 * channels (for Parallel mode)
 * @param e_fuse Pointer to Aux1/Aux2 Efuse structure being configured
 */
static void Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(
    uint8_t       selection,
    struct Efuse *e_fuse);

/**
 * Configures the Aux1/Aux2 Efuse's registers using the values defined in:
 *                  -GCR_CONFIG
 *                  -RETRY_CONFIG
 *                  -CONFR_CONFIG
 *                  -OCR_LOW_CURRENT_SENSE_CONFIG
 * @param e_fuse Pointer to Aux1/Aux2 Efuse structure being configured
 */
void Io_Efuse_Aux1Aux2ConfigureEfuse(struct Efuse *e_fuse);

/**
 * Exits fail-safe mode and disables the watchdog timer
 * @param e_fuse Pointer to Aux1/Aux2 Efuse structure being configured
 */
static void Io_Efuse_Aux1Aux2ExitFailSafeMode(struct Efuse *efuse);

/**
 * Write data to a specific Serial Input register on the Aux1/Aux2 Efuse
 * @param register_address Serial Input register being written to
 * @param register_value Value being written to the Serial Input register
 * @param e_fuse Pointer to Aux1/Aux2 Efuse structure being written to
 */
static ExitCode Io_Efuse_Aux1Aux2WriteReg(
    uint8_t       register_address,
    uint16_t      register_value,
    struct Efuse *e_fuse);

/**
 * Read data from a specific Serial Output register on the Aux1/Aux2 Efuse
 * @param register_address Serial Output register being read from
 * @param register_value register contents of Serial Output register
 * @param e_fuse Pointer to Aux1/Aux2 Efuse structure being read from
 * @return Contents of the addressed register
 */
static ExitCode Io_Efuse_Aux1Aux2ReadReg(
    uint8_t       register_address,
    uint16_t *    register_value,
    struct Efuse *e_fuse);

/**
 * Write data to a specific Serial Input register on the Efuse
 * @param register_address Serial Input register being written to
 * @param register_value Value being written to the Serial Input register
 * @param e_fuse Pointer to Efuse structure being written to
 * @return EXIT_CODE_OK if the write was successful
 *         EXIT_CODE_TIMEOUT if the SPI write timed-out
 */
static ExitCode Io_Efuse_WriteReg(
    uint8_t       register_address,
    uint16_t      register_value,
    GPIO_TypeDef *ChipSelect_GPIO_Port,
    uint16_t      ChipSelect_GPIO_Pin,
    struct Efuse *e_fuse);

/**
 * Read data from a specific Serial Output register on the Efuse
 * @param register_address Serial Output register being read from
 * @param register_value register contents
 * @param e_fuse Pointer to Efuse structure being read from
 * @return EXIT_CODE_OK if the read was successful
 *         EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
static ExitCode Io_Efuse_ReadReg(
    uint8_t       register_address,
    uint16_t *    register_value,
    GPIO_TypeDef *ChipSelect_GPIO_Port,
    uint16_t      ChipSelect_GPIO_Pin,
    struct Efuse *e_fuse);

/**
 * Calculates the parity of the SPI command to be sent to the Efuse and
 * sets/clears the parity bit. Using the XOR sum of bits method taken from:
 * https://en.wikipedia.org/wiki/Parity_bit#Parity
 * @param spi_command Original SPI command without the parity bit
 */
static void Io_Efuse_SetParityBit(uint16_t *spi_command);

/**
 * Write SPI data to Efuse in blocking mode
 * @param tx_data Pointer to data being sent to the Efuse
 * @param chip_select_port Handle to Efuse's chip-select GPIO port
 * @param chip_select_pin Efuse's chip-select GPIO pin
 * @return EXIT_CODE_OK if the write was successful
 *         EXIT_CODE_TIMEOUT if the SPI write timed-out
 */
static ExitCode Io_Efuse_WriteToEfuse(
    uint16_t *    tx_data,
    GPIO_TypeDef *chip_select_port,
    uint16_t      chip_select_pin);

/**
 * Read SPI data from Efuse in blocking mode
 * @param tx_data Pointer to data being sent to the Efuse
 * @param rx_data Pointer to data being received from the Efuse
 * @param chip_select_port Handle to Efuse's chip-select GPIO port
 * @param chip_select_pin Efuse's chip-select GPIO pin
 * @return EXIT_CODE_OK if the read was successful
 *         EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
static ExitCode Io_Efuse_ReadFromEfuse(
    uint16_t *    tx_data,
    uint16_t *    rx_data,
    GPIO_TypeDef *chip_select_port,
    uint16_t      chip_select_pin);
