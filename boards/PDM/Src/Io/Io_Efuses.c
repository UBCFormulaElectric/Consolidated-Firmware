#include "Io_Efuses.h"
#include "configs/Io_EfuseConfig.h"

#pragma GCC diagnostic ignored "-Wconversion"

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
