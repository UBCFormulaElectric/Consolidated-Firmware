#include "Io_Aux1Aux2Efuse.h"
#include "configs/Io_EfuseConfig.h"

static struct Efuse_Context *aux1_aux2_efuse;

void Io_Aux1Aux2Efuse_Init(SPI_HandleTypeDef *const spi_handle)
{
    assert(spi_handle != NULL);

    aux1_aux2_efuse = Io_Efuse_Create(
        Io_CurrentSense_GetAux1Current, Io_CurrentSense_GetAux2Current, spi_handle, CSB_AUX1_AUX2_GPIO_Port,
        CSB_AUX1_AUX2_Pin, FSOB_AUX1_AUX2_GPIO_Port, FSOB_AUX1_AUX2_Pin, FSB_AUX1_AUX2_GPIO_Port, FSB_AUX1_AUX2_Pin,
        CUR_SYNC_AUX1_AUX2_GPIO_Port, CUR_SYNC_AUX1_AUX2_Pin, PIN_AUX1_GPIO_Port, PIN_AUX1_Pin, PIN_AUX2_GPIO_Port,
        PIN_AUX2_Pin);
}

void Io_Aux1Aux2Efuse_EnableAux1(void)
{
    Io_Efuse_EnableChannel0(aux1_aux2_efuse);
}

void Io_Aux1Aux2Efuse_DisableAux1(void)
{
    Io_Efuse_DisableChannel0(aux1_aux2_efuse);
}

void Io_Aux1Aux2Efuse_EnableAux2(void)
{
    Io_Efuse_EnableChannel1(aux1_aux2_efuse);
}

void Io_Aux1Aux2Efuse_DisableAux2(void)
{
    Io_Efuse_DisableChannel1(aux1_aux2_efuse);
}

ExitCode Io_Aux1Aux2Efuse_GetStatus(enum Efuse_Status *channel_status)
{
    return Io_Efuse_ReadRegister(SO_STATR_ADDR, (uint16_t *)channel_status, aux1_aux2_efuse);
}

ExitCode Io_Aux1Aux2Efuse_GetAux1Faults(enum Efuse_Fault *fault_status)
{
    return Io_Efuse_ReadRegister(SO_FAULTR_0_ADDR, (uint16_t *)fault_status, aux1_aux2_efuse);
}

ExitCode Io_Aux1Aux2Efuse_GetAux2Faults(enum Efuse_Fault *fault_status)
{
    return Io_Efuse_ReadRegister(SO_FAULTR_1_ADDR, (uint16_t *)fault_status, aux1_aux2_efuse);
}

bool Io_Aux1Aux2Efuse_IsInFaultMode(void)
{
    return Io_Efuse_IsEfuseInFaultMode(aux1_aux2_efuse);
}

bool Io_Aux1Aux2Efuse_IsInFailSafeMode(void)
{
    return Io_Efuse_IsEfuseInFailSafeMode(aux1_aux2_efuse);
}

void Io_Aux1Aux2Efuse_DelatchFaults(void)
{
    Io_Efuse_DelatchFaults(aux1_aux2_efuse);
}

float Io_Aux1Aux2Efuse_GetAux1Current(void)
{
    return Io_Efuse_GetChannel0Current(aux1_aux2_efuse);
}

float Io_Aux1Aux2Efuse_GetAux2Current(void)
{
    return Io_Efuse_GetChannel1Current(aux1_aux2_efuse);
}

ExitCode Io_Aux1Aux2Efuse_ConfigureEfuse(void)
{
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_ExitFailSafeMode(aux1_aux2_efuse));

    // Configure the global configuration register
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_WriteRegister(SI_GCR_ADDR, GCR_CONFIG, aux1_aux2_efuse));

    // Configure channel 0's configuration registers
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_WriteRegister(SI_RETRY_0_ADDR, RETRY_CONFIG, aux1_aux2_efuse));
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_WriteRegister(SI_CONFR_0_ADDR, CONFR_CONFIG, aux1_aux2_efuse));
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_WriteRegister(SI_OCR_0_ADDR, OCR_LOW_CURRENT_SENSE_CONFIG, aux1_aux2_efuse));

    // Configure the channel 1's configuration registers
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_WriteRegister(SI_RETRY_1_ADDR, RETRY_CONFIG, aux1_aux2_efuse));
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_WriteRegister(SI_CONFR_1_ADDR, CONFR_CONFIG, aux1_aux2_efuse));
    RETURN_CODE_IF_EXIT_NOT_OK(Io_Efuse_WriteRegister(SI_OCR_1_ADDR, OCR_LOW_CURRENT_SENSE_CONFIG, aux1_aux2_efuse));

    return EXIT_CODE_OK;
}
