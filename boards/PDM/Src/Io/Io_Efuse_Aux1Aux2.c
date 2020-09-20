#include "Io_Efuse_Aux1Aux2.h"
#include "configs/Io_EfuseConfig.h"

static struct Efuse_Context *aux1_aux2_efuse;

void Io_Efuse_Init_Aux1Aux2(SPI_HandleTypeDef *const hspi)
{
    assert(hspi != NULL);

    aux1_aux2_efuse = Io_Efuse_Create(
        hspi, CSB_AUX1_AUX2_GPIO_Port, CSB_AUX1_AUX2_Pin,
        FSOB_AUX1_AUX2_GPIO_Port, FSOB_AUX1_AUX2_Pin, FSB_AUX1_AUX2_GPIO_Port,
        FSB_AUX1_AUX2_Pin, PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, PIN_AUX2_GPIO_Port,
        PIN_AUX2_Pin);
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
    return Io_Efuse_ReadRegister(
        SO_STATR_ADDR, (uint16_t *)status, aux1_aux2_efuse);
}

ExitCode Io_Efuse_GetAux1Faults(enum Efuse_Fault *fault)
{
    return Io_Efuse_ReadRegister(
        SO_FAULTR_0_ADDR, (uint16_t *)fault, aux1_aux2_efuse);
}

ExitCode Io_Efuse_GetAux2Faults(enum Efuse_Fault *fault)
{
    return Io_Efuse_ReadRegister(
        SO_FAULTR_1_ADDR, (uint16_t *)fault, aux1_aux2_efuse);
}

bool Io_Efuse_IsAux1Aux2InFaultMode(void)
{
    return Io_Efuse_IsEfuseInFaultMode(aux1_aux2_efuse);
}

bool Io_Efuse_IsAux1Aux2InFailSafeMode(void)
{
    return Io_Efuse_IsEfuseInFailSafeMode(aux1_aux2_efuse);
}

void Io_Efuse_DelatchAux1Aux2Faults(void)
{
    Io_Efuse_DelatchFaults(aux1_aux2_efuse);
}

float Io_Efuse_GetAux1Current(void)
{
    if (Io_Efuse_ConfigureChannelMonitoring(
            AUX1_CURRENT_SENSE_CHANNEL, aux1_aux2_efuse) != EXIT_CODE_OK)
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
    if (Io_Efuse_ConfigureChannelMonitoring(
            AUX2_CURRENT_SENSE_CHANNEL, aux1_aux2_efuse) != EXIT_CODE_OK)
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
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_ExitFailSafeMode(aux1_aux2_efuse));

    // Configure the global configuration register
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_WriteRegister(SI_GCR_ADDR, GCR_CONFIG, aux1_aux2_efuse));

    // Configure the channel 0 configuration registers
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_WriteRegister(SI_RETRY_0_ADDR, RETRY_CONFIG, aux1_aux2_efuse));
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_WriteRegister(SI_CONFR_0_ADDR, CONFR_CONFIG, aux1_aux2_efuse));
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_WriteRegister(
        SI_OCR_0_ADDR, OCR_LOW_CURRENT_SENSE_CONFIG, aux1_aux2_efuse));

    // Configure the channel  configuration registers
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_WriteRegister(SI_RETRY_1_ADDR, RETRY_CONFIG, aux1_aux2_efuse));
    RETURN_CODE_IF_EXIT_NOT_OK(
        Io_Efuse_WriteRegister(SI_CONFR_1_ADDR, CONFR_CONFIG, aux1_aux2_efuse));
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_WriteRegister(
        SI_OCR_1_ADDR, OCR_LOW_CURRENT_SENSE_CONFIG, aux1_aux2_efuse));

    return EXIT_CODE_OK;
}
