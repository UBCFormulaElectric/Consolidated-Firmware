#include <assert.h>
#include <math.h>
#include "Io_Efuse.h"
#include "configs/Io_EfuseConfig.h"
#include "Io_CurrentSense.h"

#pragma GCC diagnostic ignored "-Wconversion"

struct Efuse_Context
{
    // The SPI handle for the SPI device the E-Fuse is connected to
    SPI_HandleTypeDef *efuse_spi_handle;

    // The current state of the watchdog-in bit (bit 15). If the watchdog is
    // enabled its state must be alternated at least once within the watchdog
    // timeout period.
    bool wdin_bit_to_set;
};

static struct Efuse_Context aux1_aux2_efuse;

/**
 * Set the current/temperature monitoring option of CSNS pin
 * @param selection The desired monitoring function which can be selected by
 * specifying the following values:
 *                  CSNS_FUNCTION_DISABLED - Current sensing disabled
 *                  AUX1_CURRENT_SENSE_CHANNEL - Current sensing for Aux 1
 *                  AUX2_CURRENT_SENSE_CHANNEL - Current sensing for Aux 2
 *                  CSNS_FUNCTION_TEMPERATURE - Efuse temperature sensing
 *                  CSNS_FUNCTION_CURRENT_SUM - Current sensing for summed
 * channels (for Parallel mode)
 * @return EXIT_CODE_OK if the channel configuration was successful
 *         EXIT_CODE_TIMEOUT if one of the SPI writes timed-out
 */
static ExitCode Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(uint8_t selection);

/**
 * Exit fail-safe mode and disable the watchdog timer
 * @return EXIT_CODE_OK if the efuse exited fail-safe mode
 *         EXIT_CODE_TIMEOUT if the one of the SPI writes timed-out
 *         EXIT_CODE_UNIMPLEMENTED if the efuse failed to exit fail-safe mode
 */
static ExitCode Io_Efuse_Aux1Aux2ExitFailSafeMode(void);

/**
 * Write data to a specific Serial Input register on the Aux1/Aux2 Efuse
 * @param register_address Serial Input register being written to
 * @param register_value Value being written to the Serial Input register
 * @return EXIT_CODE_OK if the write was successful
 *         EXIT_CODE_TIMEOUT if the SPI write timed-out
 */
static ExitCode Io_Efuse_Aux1Aux2WriteRegister(
    uint8_t  register_address,
    uint16_t register_value);

/**
 * Read data from a specific Serial Output register on the Aux1/Aux2 Efuse
 * @param register_address Serial Output register being read from
 * @param register_value register contents of Serial Output register
 * @return EXIT_CODE_OK if the read was successful
 *         EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
static ExitCode Io_Efuse_Aux1Aux2ReadRegister(
    uint8_t   register_address,
    uint16_t *register_value);

/**
 * Write data to a specific Serial Input register on the Efuse
 * @param register_address Serial Input register being written to
 * @param register_value Value being written to the Serial Input register
 * @return EXIT_CODE_OK if the write was successful
 *         EXIT_CODE_TIMEOUT if the SPI write timed-out
 */
static ExitCode Io_Efuse_WriteRegister(
    uint8_t               register_address,
    uint16_t              register_value,
    GPIO_TypeDef *        chip_select_port,
    uint16_t              chip_select_pin,
    struct Efuse_Context *e_fuse);

/**
 * Read data from a specific Serial Output register on the Efuse
 * @param register_address Serial Output register being read from
 * @param register_value register contents
 * @param e_fuse Pointer to Efuse structure being read from
 * @return EXIT_CODE_OK if the read was successful
 *         EXIT_CODE_TIMEOUT if the SPI read timed-out
 */
static ExitCode Io_Efuse_ReadRegister(
    uint8_t               register_address,
    uint16_t *            register_value,
    GPIO_TypeDef *        chip_select_port,
    uint16_t              chip_select_pin,
    struct Efuse_Context *e_fuse);

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

static ExitCode Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(uint8_t selection)
{
    uint16_t register_value = 0x0000;

    // Read original content of GCR Register
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_Aux1Aux2ReadRegister(SI_GCR_ADDR, &register_value));

    CLEAR_BIT(register_value, (CSNS1_EN_MASK | CSNS0_EN_MASK));
    SET_BIT(register_value, (selection & (CSNS1_EN_MASK | CSNS0_EN_MASK)));

    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_Aux1Aux2WriteRegister(SI_GCR_ADDR, register_value));

    return EXIT_CODE_OK;
}

static ExitCode Io_Efuse_Aux1Aux2ExitFailSafeMode(void)
{
    // Set WDIN bit for next write
    // 1_1_00000_00000_0000
    aux1_aux2_efuse.wdin_bit_to_set = true;

    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_Aux1Aux2WriteRegister(SI_STATR_0_ADDR, 0x0000));

    // Disable the watchdog timer
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_Aux1Aux2WriteRegister(SI_GCR_ADDR, GCR_CONFIG));

    // Check if the the efuse is still in fail-safe mode
    if (HAL_GPIO_ReadPin(FSOB_AUX1_AUX2_GPIO_Port, FSOB_AUX1_AUX2_Pin) ==
        GPIO_PIN_RESET)
    {
        return EXIT_CODE_UNIMPLEMENTED;
    }

    return EXIT_CODE_OK;
}

static ExitCode Io_Efuse_Aux1Aux2WriteRegister(
    uint8_t  register_address,
    uint16_t register_value)
{
    return Io_Efuse_WriteRegister(
        register_address, register_value, CSB_AUX1_AUX2_GPIO_Port,
        CSB_AUX1_AUX2_Pin, &aux1_aux2_efuse);
}

static ExitCode Io_Efuse_Aux1Aux2ReadRegister(
    uint8_t   register_address,
    uint16_t *register_value)
{
    return Io_Efuse_ReadRegister(
        register_address, register_value, CSB_AUX1_AUX2_GPIO_Port,
        CSB_AUX1_AUX2_Pin, &aux1_aux2_efuse);
}

static ExitCode Io_Efuse_WriteRegister(
    uint8_t               register_address,
    uint16_t              register_value,
    GPIO_TypeDef *        chip_select_port,
    uint16_t              chip_select_pin,
    struct Efuse_Context *e_fuse)
{
    uint16_t serial_input_data = 0x0000U;

    // Place the register address into bits 10->13
    serial_input_data = (uint16_t)(
        serial_input_data |
        ((register_address & EFUSE_ADDR_MASK) << EFUSE_ADDR_SHIFT));

    // Place register value to be written into bits 0->8
    serial_input_data =
        (uint16_t)(serial_input_data | (register_value & EFUSE_SI_DATA_MASK));

    // Invert watchdog bit state (Note: It is safe to do so even if the watchdog
    // is disabled)
    if (e_fuse->wdin_bit_to_set)
    {
        SET_BIT(serial_input_data, WATCHDOG_BIT);
    }
    else
    {
        CLEAR_BIT(serial_input_data, WATCHDOG_BIT);
    }

    // Invert watchdog bit state for next write
    e_fuse->wdin_bit_to_set = !e_fuse->wdin_bit_to_set;

    Io_Efuse_CalculateParityBit(&serial_input_data);

    return Io_Efuse_WriteToEfuse(
        &serial_input_data, e_fuse->efuse_spi_handle, chip_select_port,
        chip_select_pin);
}

static ExitCode Io_Efuse_ReadRegister(
    uint8_t               register_address,
    uint16_t *            register_value,
    GPIO_TypeDef *        chip_select_port,
    uint16_t              chip_select_pin,
    struct Efuse_Context *e_fuse)
{
    uint16_t serial_input_data = 0x0000U;

    // Place the Status Register address into bits 10->13
    serial_input_data =
        (uint16_t)((SI_STATR_0_ADDR & EFUSE_ADDR_MASK) << EFUSE_ADDR_SHIFT);

    // Shift bit 3 of the address (SOA3: the channel number) to the 13th bit
    serial_input_data = (uint16_t)(
        serial_input_data |
        ((register_address & SOA3_MASK) << EFUSE_ADDR_SHIFT) |
        (register_address & (SOA2_MASK | SOA1_MASK | SOA0_MASK)));

    // Invert watchdog bit state (Note: It is safe to do so even if the watchdog
    // is disabled)
    if (e_fuse->wdin_bit_to_set)
    {
        SET_BIT(serial_input_data, WATCHDOG_BIT);
    }
    else
    {
        CLEAR_BIT(serial_input_data, WATCHDOG_BIT);
    }

    // Invert watchdog bit state for next write
    e_fuse->wdin_bit_to_set = !e_fuse->wdin_bit_to_set;

    Io_Efuse_CalculateParityBit(&serial_input_data);

    ExitCode exit_code = Io_Efuse_ReadFromEfuse(
        &serial_input_data, register_value, e_fuse->efuse_spi_handle,
        chip_select_port, chip_select_pin);

    // Only return register contents and clear bits 9->15
    *register_value = READ_BIT(*register_value, EFUSE_SO_DATA_MASK);

    return exit_code;
}

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
    HAL_StatusTypeDef status = HAL_OK;

    // Send the command stored in tx_data to the status register, to read the
    // data from the register address specified in tx_data
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive(
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

void Io_Efuse_Init(SPI_HandleTypeDef *const hspi)
{
    assert(hspi != NULL);

    aux1_aux2_efuse.efuse_spi_handle = hspi;
    aux1_aux2_efuse.wdin_bit_to_set  = true;
}

void Io_Efuse_EnableAux1(void)
{
    HAL_GPIO_WritePin(PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, GPIO_PIN_SET);
}

void Io_Efuse_DisableAux1(void)
{
    HAL_GPIO_WritePin(PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, GPIO_PIN_RESET);
}

void Io_Efuse_EnableAux2(void)
{
    HAL_GPIO_WritePin(PIN_AUX2_GPIO_Port, PIN_AUX2_Pin, GPIO_PIN_SET);
}

void Io_Efuse_DisableAux2(void)
{
    HAL_GPIO_WritePin(PIN_AUX2_GPIO_Port, PIN_AUX2_Pin, GPIO_PIN_RESET);
}

ExitCode Io_Efuse_GetAux1Aux2Status(enum Efuse_Status *status)
{
    return Io_Efuse_Aux1Aux2ReadRegister(SO_STATR_ADDR, (uint16_t *)status);
}

ExitCode Io_Efuse_GetAux1Faults(enum Efuse_Fault *fault)
{
    return Io_Efuse_Aux1Aux2ReadRegister(SO_FAULTR_0_ADDR, (uint16_t *)fault);
}

ExitCode Io_Efuse_GetAux2Faults(enum Efuse_Fault *fault)
{
    return Io_Efuse_Aux1Aux2ReadRegister(SO_FAULTR_1_ADDR, (uint16_t *)fault);
}

bool Io_Efuse_IsAux1Aux2InFaultMode(void)
{
    return HAL_GPIO_ReadPin(FSB_AUX1_AUX2_GPIO_Port, FSB_AUX1_AUX2_Pin);
}

bool Io_Efuse_IsAux1Aux2InFailSafeMode(void)
{
    return HAL_GPIO_ReadPin(FSOB_AUX1_AUX2_GPIO_Port, FSOB_AUX1_AUX2_Pin);
}

void Io_Efuse_DelatchAux1Aux2Faults(void)
{
    // Delatch the latchable faults by alternating the inputs high-low-high
    HAL_GPIO_WritePin(PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(PIN_AUX2_GPIO_Port, PIN_AUX2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PIN_AUX2_GPIO_Port, PIN_AUX2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PIN_AUX2_GPIO_Port, PIN_AUX2_Pin, GPIO_PIN_SET);
}

float Io_Efuse_GetAux1Current(void)
{
    if (Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(
            AUX1_CURRENT_SENSE_CHANNEL) != EXIT_CODE_OK)
    {
        return NAN;
    }

    if (HAL_GPIO_ReadPin(
            CUR_SYNC_AUX1_AUX2_GPIO_Port, CUR_SYNC_AUX1_AUX2_Pin) ==
        GPIO_PIN_RESET)
    {
        return Io_CurrentSense_GetAux1Current();
    }

    return NAN;
}

float Io_Efuse_GetAux2Current(void)
{
    if (Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(
            AUX2_CURRENT_SENSE_CHANNEL) != EXIT_CODE_OK)
    {
        return NAN;
    }

    if (HAL_GPIO_ReadPin(
            CUR_SYNC_AUX1_AUX2_GPIO_Port, CUR_SYNC_AUX1_AUX2_Pin) ==
        GPIO_PIN_RESET)
    {
        return Io_CurrentSense_GetAux2Current();
    }

    return NAN;
}

ExitCode Io_Efuse_ConfigureAux1Aux2Efuse(void)
{
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_Aux1Aux2ExitFailSafeMode());

    // Configure the global configuration register
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_Aux1Aux2WriteRegister(SI_GCR_ADDR, GCR_CONFIG));

    // Configure the channel 0 configuration registers
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_Aux1Aux2WriteRegister(SI_RETRY_0_ADDR, RETRY_CONFIG));
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_Aux1Aux2WriteRegister(SI_CONFR_0_ADDR, CONFR_CONFIG));
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_Aux1Aux2WriteRegister(
        SI_OCR_0_ADDR, OCR_LOW_CURRENT_SENSE_CONFIG));

    // Configure the channel  configuration registers
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_Aux1Aux2WriteRegister(SI_RETRY_1_ADDR, RETRY_CONFIG));
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_Aux1Aux2WriteRegister(SI_CONFR_1_ADDR, CONFR_CONFIG));
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_Aux1Aux2WriteRegister(
        SI_OCR_1_ADDR, OCR_LOW_CURRENT_SENSE_CONFIG));

    return EXIT_CODE_OK;
}
