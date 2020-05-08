#include "Io_Efuse_22XS4200.h"

// The SPI handle for the SPI device the E-Fuses are connected to
static SPI_HandleTypeDef *efuse_spi_handle;

void Io_Efuse_22XS4200_Init(SPI_HandleTypeDef *const hspi)
{
    efuse_spi_handle = hspi;
}

void Io_Efuse_22XS4200_configureEfuse(struct E_Fuse *e_fuse)
{
    // Global config register
    Io_Efuse_22XS4200_WriteReg(SI_GCR_ADDR, GCR_CONFIG, e_fuse);

    // Channel 0 config registers
    Io_Efuse_22XS4200_WriteReg(SI_RETRY_0_ADDR, RETRY_0_CONFIG, e_fuse);
    Io_Efuse_22XS4200_WriteReg(SI_CONFR_0_ADDR, CONFR_0_CONFIG, e_fuse);
    Io_Efuse_22XS4200_WriteReg(SI_OCR_0_ADDR, OCR_0_CONFIG, e_fuse);

    // Channel 1 config registers
    Io_Efuse_22XS4200_WriteReg(SI_RETRY_1_ADDR, RETRY_1_CONFIG, e_fuse);
    Io_Efuse_22XS4200_WriteReg(SI_CONFR_1_ADDR, CONFR_1_CONFIG, e_fuse);
    Io_Efuse_22XS4200_WriteReg(SI_OCR_1_ADDR, OCR_1_CONFIG, e_fuse);
}

void Io_Efuse_22XS4200_configure_channel_monitoring(
    uint8_t        selection,
    struct E_Fuse *e_fuse)
{
    uint16_t reg_val = 0x0000;

    // Read original content of GCR Register
    reg_val = Io_Efuse_22XS4200_ReadReg(SI_GCR_ADDR, e_fuse);

    reg_val &= ~(CSNS1_EN_MASK | CSNS0_EN_MASK);
    reg_val |= (selection & (CSNS1_EN_MASK | CSNS0_EN_MASK));

    Io_Efuse_22XS4200_WriteReg(SI_GCR_ADDR, reg_val, e_fuse);
}

void Io_Efuse_22XS4200_UpdateStatus(struct E_Fuse *e_fuse)
{
    uint16_t status = StatusType_NoFault;

    status = Io_Efuse_22XS4200_ReadReg(SO_STATR_ADDR, e_fuse);

    /* Clear other bits */
    status &= EFUSE_SO_DATA_MASK;

    e_fuse->status = status;
}

void Io_Efuse_22XS4200_UpdateFaults(struct E_Fuse *e_fuse)
{
    uint16_t channel_0_faults = FaultType_NoFault;
    uint16_t channel_1_faults = FaultType_NoFault;

    channel_0_faults = Io_Efuse_22XS4200_ReadReg(SO_FAULTR_0_ADDR, e_fuse);
    channel_1_faults = Io_Efuse_22XS4200_ReadReg(SO_FAULTR_1_ADDR, e_fuse);

    /* Clear other bits */
    channel_0_faults &= EFUSE_SO_DATA_MASK;
    channel_1_faults &= EFUSE_SO_DATA_MASK;

    e_fuse->faults.channel_0_faults = channel_0_faults;
    e_fuse->faults.channel_1_faults = channel_1_faults;
}

void Io_Efuse_22XS4200_WriteReg(
    uint8_t        register_address,
    uint16_t       register_value,
    struct E_Fuse *e_fuse)
{
    HAL_StatusTypeDef status;
    uint16_t          command    = 0x0000U;
    uint8_t           parity_bit = 0;

    command =
        (uint16_t)((register_address & EFUSE_ADDR_MASK) << EFUSE_ADDR_SHIFT);
    /* place register value to be written (bits 0->8) */
    command = (uint16_t)(command | (register_value & EFUSE_DATA_MASK));
    /* invert watchdog bit state */
    if (e_fuse->watch_dog_state)
    {
        command |= WATCH_DOG_BIT;
    }
    else
    {
        command &= ~(WATCH_DOG_BIT);
    }
    /* invert watchdog bit state for next write */
    e_fuse->watch_dog_state = !e_fuse->watch_dog_state;
    /* compute parity */
    parity_bit = Io_Efuse_Cal_Parity_Bit(command);
    /* add parity value */
    command = (uint16_t)(command | ((parity_bit & 1) << 14));
    status  = Io_Efuse_22XS4200_WriteToEfuse(
        &command, e_fuse->chip_select.GPIO_Port, e_fuse->chip_select.GPIO_Pin);
    UNUSED(status);
}

uint16_t
    Io_Efuse_22XS4200_ReadReg(uint8_t register_address, struct E_Fuse *e_fuse)
{
    HAL_StatusTypeDef status;
    uint16_t          command    = 0x0000U;
    uint8_t           parity_bit = 0;
    uint16_t          tx_data[]  = { 0x0000U };
    uint16_t          rx_data[]  = { 0x0000U };

    command =
        (uint16_t)((SI_STATR_0_ADDR & EFUSE_ADDR_MASK) << EFUSE_ADDR_SHIFT);
    /* shift bit 3 of the address (SOA3: the channel number) to the D13 position
     */
    command = (uint16_t)(
        command | ((register_address & SOA3_MASK) << EFUSE_ADDR_SHIFT) |
        (register_address & (SOA2_MASK | SOA1_MASK | SOA0_MASK)));
    /* invert watchdog bit state */
    if (e_fuse->watch_dog_state)
    {
        command |= WATCH_DOG_BIT;
    }
    else
    {
        command &= ~(WATCH_DOG_BIT);
    }
    /* invert watchdog bit state for next write */
    e_fuse->watch_dog_state = !e_fuse->watch_dog_state;
    /* compute parity */
    parity_bit = Io_Efuse_Cal_Parity_Bit(command);
    /* add parity value */
    command    = (uint16_t)command | ((parity_bit & 1U) << PARITY_BIT_SHIFT);
    tx_data[0] = command;
    status     = Io_Efuse_22XS4200_ReadFromEfuse(
        tx_data, rx_data, e_fuse->chip_select.GPIO_Port,
        e_fuse->chip_select.GPIO_Pin);
    UNUSED(status);
    return rx_data[0];
}

uint8_t Io_Efuse_Cal_Parity_Bit(uint16_t spi_command)
{
    uint8_t parity_bit;
    /* calculate parity for bits in spi command */
    for (parity_bit = 0; spi_command > 0; spi_command >>= 1)
    {
        parity_bit ^= (spi_command & 1);
    }
    return parity_bit;
}

HAL_StatusTypeDef Io_Efuse_22XS4200_WriteToEfuse(
    uint16_t *    TxData,
    GPIO_TypeDef *GPIO_Port,
    uint16_t      GPIO_Pin)
{
    HAL_StatusTypeDef status;

    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_RESET);
    status = HAL_SPI_Transmit(efuse_spi_handle, (uint8_t *)TxData, 1U, 100U);
    while (efuse_spi_handle->State != HAL_SPI_STATE_READY)
        ;
    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_SET);

    return status;
}

HAL_StatusTypeDef Io_Efuse_22XS4200_ReadFromEfuse(
    uint16_t *    TxData,
    uint16_t *    RxData,
    GPIO_TypeDef *GPIO_Port,
    uint16_t      GPIO_Pin)
{
    HAL_StatusTypeDef status;
    // Send command to read from status register
    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive(
        efuse_spi_handle, (uint8_t *)TxData, (uint8_t *)RxData, 1U, 100U);
    while (efuse_spi_handle->State != HAL_SPI_STATE_READY)
        ;
    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_SET);
    // Receive data from E-fuse (transmitted data is ignored) RxData(MISO) is
    // correct
    *TxData = 0xFFFF;
    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_RESET);
    status &= HAL_SPI_TransmitReceive(
        efuse_spi_handle, (uint8_t *)TxData, (uint8_t *)RxData, 1U, 100U);
    while (efuse_spi_handle->State != HAL_SPI_STATE_READY)
        ;
    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_SET);
    return status;
}
