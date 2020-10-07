#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "Io_Efuse.h"
#include "configs/Io_EfuseConfig.h"

#pragma GCC diagnostic ignored "-Wconversion"

struct Efuse_Context
{
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *     nss_port;
    uint16_t           nss_pin;

    GPIO_TypeDef *fsob_port;
    uint16_t      fsob_pin;
    GPIO_TypeDef *fsb_port;
    uint16_t      fsb_pin;

    GPIO_TypeDef *channel0_port;
    uint16_t      channel0_pin;
    GPIO_TypeDef *channel1_port;
    uint16_t      channel1_pin;

    // The current state of the watchdog-in bit (bit 15). If the watchdog is
    // enabled its state must be alternated at least once within the watchdog
    // timeout period.
    bool wdin_bit_to_set;
};

/**
 * Calculate the parity of the SPI command to be sent to the Efuse and
 * set/clear the parity bit. Using the XOR sum of bits method taken from:
 * https://en.wikipedia.org/wiki/Parity_bit#Parity
 * @param spi_command Original SPI command without the parity bit set/cleared
 */
static void Io_Efuse_CalculateParityBit(uint16_t *serial_data_input);

/**
 * Write SPI data to Efuse in blocking mode
 * @param tx_data Pointer to data being sent to the Efuse
 * @param efuse_spi_handle Handle to the Efuse's SPI peripheral
 * @param chip_select_port Handle to Efuse's chip-select GPIO port
 * @param chip_select_pin Efuse's chip-select GPIO pin
 * @return EXIT_CODE_OK if the write was successful
 *         EXIT_CODE_TIMEOUT if the SPI write timed-out
 */
static ExitCode Io_Efuse_WriteToEfuse(
    uint16_t *         tx_data,
    SPI_HandleTypeDef *efuse_spi_handle,
    GPIO_TypeDef *     chip_select_port,
    uint16_t           chip_select_pin);

/**
 * Read SPI data from Efuse in blocking mode
 * @param tx_data Pointer to data being sent to the Efuse
 * @param rx_data Pointer to data being received from the Efuse
 * @param efuse_spi_handle Handle to the Efuse's SPI peripheral
 * @param chip_select_port Handle to Efuse's chip-select GPIO port
 * @param chip_select_pin Efuse's chip-select GPIO pin
 * @return EXIT_CODE_OK if the read was successful
 *         EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
static ExitCode Io_Efuse_ReadFromEfuse(
    uint16_t *         tx_data,
    uint16_t *         rx_data,
    SPI_HandleTypeDef *efuse_spi_handle,
    GPIO_TypeDef *     chip_select_port,
    uint16_t           chip_select_pin);

static void Io_Efuse_CalculateParityBit(uint16_t *serial_data_input)
{
    uint16_t _serial_data_input = *serial_data_input;
    bool     parity_bit;

    for (parity_bit = 0; _serial_data_input > 0; _serial_data_input >>= 1)
    {
        parity_bit ^= READ_BIT(_serial_data_input, 1U);
    }

    if (parity_bit)
    {
        SET_BIT(*serial_data_input, PARITY_BIT);
    }
    else
    {
        CLEAR_BIT(*serial_data_input, PARITY_BIT);
    }
}

static ExitCode Io_Efuse_WriteToEfuse(
    uint16_t *         tx_data,
    SPI_HandleTypeDef *efuse_spi_handle,
    GPIO_TypeDef *     chip_select_port,
    uint16_t           chip_select_pin)
{
    HAL_StatusTypeDef status = HAL_OK;

    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive(
        efuse_spi_handle, (uint8_t *)tx_data, (uint8_t *)tx_data, 1U, 100U);
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_SET);

    if (status != HAL_OK)
    {
        return EXIT_CODE_TIMEOUT;
    }

    return EXIT_CODE_OK;
}

static ExitCode Io_Efuse_ReadFromEfuse(
    uint16_t *         tx_data,
    uint16_t *         rx_data,
    SPI_HandleTypeDef *efuse_spi_handle,
    GPIO_TypeDef *     chip_select_port,
    uint16_t           chip_select_pin)
{
    // Send the command stored in tx_data to the status register, to read the
    // data from the register address specified in tx_data
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(
        efuse_spi_handle, (uint8_t *)tx_data, (uint8_t *)rx_data, 1U, 100U);
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_SET);

    if (status != HAL_OK)
    {
        return EXIT_CODE_TIMEOUT;
    }

    // Receive data from the register address specified in tx_data by sending
    // dummy data
    *tx_data = 0xFFFF;
    // The data read from the register specified in tx_data is stored in rx_data
    // after the SPI transfer
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive(
        efuse_spi_handle, (uint8_t *)tx_data, (uint8_t *)rx_data, 1U, 100U);
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_SET);

    if (status != HAL_OK)
    {
        return EXIT_CODE_TIMEOUT;
    }

    return EXIT_CODE_OK;
}

struct Efuse_Context *Io_Efuse_Create(
    SPI_HandleTypeDef *const spi_handle,
    GPIO_TypeDef *           nss_port,
    uint16_t                 nss_pin,
    GPIO_TypeDef *           fsob_port,
    uint16_t                 fsob_pin,
    GPIO_TypeDef *           fsb_port,
    uint16_t                 fsb_pin,
    GPIO_TypeDef *           channel0_port,
    uint16_t                 channel0_pin,
    GPIO_TypeDef *           channel1_port,
    uint16_t                 channel1_pin)
{
    assert(spi_handle != NULL);

    struct Efuse_Context *efuse_context = malloc(sizeof(struct Efuse_Context));
    assert(efuse_context != NULL);

    efuse_context->hspi            = spi_handle;
    efuse_context->nss_port        = nss_port;
    efuse_context->nss_pin         = nss_pin;
    efuse_context->fsob_port       = fsob_port;
    efuse_context->fsob_pin        = fsob_pin;
    efuse_context->fsb_port        = fsb_port;
    efuse_context->fsb_pin         = fsb_pin;
    efuse_context->channel0_port   = channel0_port;
    efuse_context->channel0_pin    = channel0_pin;
    efuse_context->channel1_port   = channel1_port;
    efuse_context->channel1_pin    = channel1_pin;
    efuse_context->wdin_bit_to_set = true;

    return efuse_context;
}

void Io_Efuse_Destroy(struct Efuse_Context *efuse)
{
    free(efuse);
}

void Io_Efuse_EnableChannel0(const struct Efuse_Context *const efuse)
{
    HAL_GPIO_WritePin(efuse->channel0_port, efuse->channel0_pin, GPIO_PIN_SET);
}

void Io_Efuse_DisableChannel0(const struct Efuse_Context *const efuse)
{
    HAL_GPIO_WritePin(
        efuse->channel0_port, efuse->channel0_pin, GPIO_PIN_RESET);
}

void Io_Efuse_EnableChannel1(const struct Efuse_Context *const efuse)
{
    HAL_GPIO_WritePin(efuse->channel1_port, efuse->channel1_pin, GPIO_PIN_SET);
}

void Io_Efuse_DisableChannel1(const struct Efuse_Context *const efuse)
{
    HAL_GPIO_WritePin(
        efuse->channel1_port, efuse->channel1_pin, GPIO_PIN_RESET);
}

ExitCode Io_Efuse_ConfigureChannelMonitoring(
    uint8_t                     monitoring_function,
    struct Efuse_Context *const efuse)
{
    uint16_t register_value;

    // Read original content of GCR Register
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_ReadRegister(SI_GCR_ADDR, &register_value, efuse));

    CLEAR_BIT(register_value, (CSNS1_EN_MASK | CSNS0_EN_MASK));
    SET_BIT(
        register_value,
        (monitoring_function & (CSNS1_EN_MASK | CSNS0_EN_MASK)));

    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_WriteRegister(SI_GCR_ADDR, register_value, efuse));

    return EXIT_CODE_OK;
}

ExitCode Io_Efuse_ExitFailSafeMode(struct Efuse_Context *const efuse)
{
    // Set the WDIN bit (15th bit) to exit out of fail-safe mode. The parity bit
    // (14th bit) will automatically be set in Io_Efuse_WriteRegister.
    efuse->wdin_bit_to_set = true;

    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_WriteRegister(SI_STATR_0_ADDR, 0x0000, efuse));

    // Disable the watchdog timer
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_WriteRegister(SI_GCR_ADDR, GCR_CONFIG, efuse));

    // Check if the the efuse is still in fail-safe mode
    if (Io_Efuse_IsEfuseInFailSafeMode(efuse))
    {
        return EXIT_CODE_UNIMPLEMENTED;
    }

    return EXIT_CODE_OK;
}

bool Io_Efuse_IsEfuseInFaultMode(const struct Efuse_Context *const efuse)
{
    return HAL_GPIO_ReadPin(efuse->fsb_port, efuse->fsb_pin);
}

bool Io_Efuse_IsEfuseInFailSafeMode(const struct Efuse_Context *const efuse)
{
    return HAL_GPIO_ReadPin(efuse->fsob_port, efuse->fsob_pin);
}

void Io_Efuse_DelatchFaults(const struct Efuse_Context *const efuse)
{
    // Delatch the latchable faults by alternating channel 0's and channel 1's
    // input pins high->low->high.
    HAL_GPIO_WritePin(efuse->channel0_port, efuse->channel0_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(
        efuse->channel0_port, efuse->channel0_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(efuse->channel0_port, efuse->channel0_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(efuse->channel1_port, efuse->channel1_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(
        efuse->channel1_port, efuse->channel1_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(efuse->channel1_port, efuse->channel1_pin, GPIO_PIN_SET);
}

ExitCode Io_Efuse_WriteRegister(
    uint8_t                     register_address,
    uint16_t                    register_value,
    struct Efuse_Context *const efuse)
{
    uint16_t tx_data = 0x0000U;

    // Place the register address into bits 10->13
    tx_data = (uint16_t)(
        tx_data | ((register_address & EFUSE_ADDR_MASK) << EFUSE_ADDR_SHIFT));

    // Place register value to be written into bits 0->8
    tx_data = (uint16_t)(tx_data | (register_value & EFUSE_SI_DATA_MASK));

    // Invert watchdog bit state (Note: It is safe to do so even if the watchdog
    // is disabled)
    if (efuse->wdin_bit_to_set)
    {
        SET_BIT(tx_data, WATCHDOG_BIT);
    }
    else
    {
        CLEAR_BIT(tx_data, WATCHDOG_BIT);
    }

    // Invert watchdog bit state for next write
    efuse->wdin_bit_to_set = !efuse->wdin_bit_to_set;
    Io_Efuse_CalculateParityBit(&tx_data);

    return Io_Efuse_WriteToEfuse(
        &tx_data, efuse->hspi, efuse->nss_port, efuse->nss_pin);
}

ExitCode Io_Efuse_ReadRegister(
    uint8_t                     register_address,
    uint16_t *                  register_value,
    struct Efuse_Context *const efuse)
{
    // Place the Status Register address into bits 10->13
    uint16_t tx_data =
        (uint16_t)((SI_STATR_0_ADDR & EFUSE_ADDR_MASK) << EFUSE_ADDR_SHIFT);

    // Shift bit 3 of the address (SOA3: the channel number) to the 13th bit
    tx_data = (uint16_t)(
        tx_data | ((register_address & SOA3_MASK) << EFUSE_ADDR_SHIFT) |
        (register_address & (SOA2_MASK | SOA1_MASK | SOA0_MASK)));

    // Invert watchdog bit state (Note: It is safe to do so even if the watchdog
    // is disabled)
    if (efuse->wdin_bit_to_set)
    {
        SET_BIT(tx_data, WATCHDOG_BIT);
    }
    else
    {
        CLEAR_BIT(tx_data, WATCHDOG_BIT);
    }

    // Invert watchdog bit state for next write
    efuse->wdin_bit_to_set = !efuse->wdin_bit_to_set;
    Io_Efuse_CalculateParityBit(&tx_data);
    ExitCode exit_code = Io_Efuse_ReadFromEfuse(
        &tx_data, register_value, efuse->hspi, efuse->nss_port, efuse->nss_pin);

    // Only return register contents and clear bits 9->15
    *register_value = READ_BIT(*register_value, EFUSE_SO_DATA_MASK);

    return exit_code;
}
