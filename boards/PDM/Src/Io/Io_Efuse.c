#include "Io_Efuse.h"

#define EFUSE_ADDR_MASK 0xFU
#define EFUSE_ADDR_SHIFT 0x0AU
#define EFUSE_SI_DATA_MASK 0x1FFU
#define EFUSE_SO_DATA_MASK 0xFF // Ignore Normal-Mode status bit (bit 9)

#define WATCH_DOG_BIT (1U << 15U)
#define PARITY_BIT (1U << 14U)

// The SPI handle for the SPI device the E-Fuses are connected to
static SPI_HandleTypeDef *efuse_spi_handle;

void Io_Efuse_Init(SPI_HandleTypeDef *const hspi)
{
    efuse_spi_handle = hspi;
}

void Io_Efuse_ConfigureEfuse(struct Efuse *e_fuse)
{
    // Global config register
    Io_Efuse_WriteReg(SI_GCR_ADDR, GCR_CONFIG, e_fuse);

    // Channel 0 config registers
    Io_Efuse_WriteReg(SI_RETRY_0_ADDR, RETRY_0_CONFIG, e_fuse);
    Io_Efuse_WriteReg(SI_CONFR_0_ADDR, CONFR_0_CONFIG, e_fuse);
    Io_Efuse_WriteReg(SI_OCR_0_ADDR, OCR_0_CONFIG, e_fuse);

    // Channel 1 config registers
    Io_Efuse_WriteReg(SI_RETRY_1_ADDR, RETRY_1_CONFIG, e_fuse);
    Io_Efuse_WriteReg(SI_CONFR_1_ADDR, CONFR_1_CONFIG, e_fuse);
    Io_Efuse_WriteReg(SI_OCR_1_ADDR, OCR_1_CONFIG, e_fuse);
}

void Io_Efuse_Configure_Channel_Monitoring(
    uint8_t       selection,
    struct Efuse *e_fuse)
{
    uint16_t reg_val = 0x0000;

    // Read original content of GCR Register
    reg_val = Io_Efuse_ReadReg(SI_GCR_ADDR, e_fuse);

    CLEAR_BIT(reg_val, (CSNS1_EN_MASK | CSNS0_EN_MASK));
    SET_BIT(reg_val, (selection & (CSNS1_EN_MASK | CSNS0_EN_MASK)));

    Io_Efuse_WriteReg(SI_GCR_ADDR, reg_val, e_fuse);
}

void Io_Efuse_UpdateStatus(struct Efuse *e_fuse)
{
    uint16_t status = StatusType_NoFault;

    status = Io_Efuse_ReadReg(SO_STATR_ADDR, e_fuse);

    e_fuse->status = (StatusType_e)status;
}

void Io_Efuse_UpdateFaults(struct Efuse *e_fuse)
{
    uint16_t channel_0_faults = FaultType_NoFault;
    uint16_t channel_1_faults = FaultType_NoFault;

    channel_0_faults = Io_Efuse_ReadReg(SO_FAULTR_0_ADDR, e_fuse);
    channel_1_faults = Io_Efuse_ReadReg(SO_FAULTR_1_ADDR, e_fuse);

    e_fuse->faults.channel_0_faults = (FaultType_e)channel_0_faults;
    e_fuse->faults.channel_1_faults = (FaultType_e)channel_1_faults;
}

void Io_Efuse_WriteReg(
    uint8_t       register_address,
    uint16_t      register_value,
    struct Efuse *e_fuse)
{
    HAL_StatusTypeDef status;
    uint16_t          command        = 0x0000U;
    bool              set_parity_bit = false;

    // Place the register address into bits 10->13
    command =
        (uint16_t)((register_address & EFUSE_ADDR_MASK) << EFUSE_ADDR_SHIFT);
    // Place register value to be written into bits 0->8
    command = (uint16_t)(command | (register_value & EFUSE_SI_DATA_MASK));
    // Invert watchdog bit state
    if (e_fuse->watch_dog_state)
    {
        SET_BIT(command, WATCH_DOG_BIT);
    }
    else
    {
        CLEAR_BIT(command, WATCH_DOG_BIT);
    }
    // Invert watchdog bit state for next write
    e_fuse->watch_dog_state = !e_fuse->watch_dog_state;
    // Compute parity
    set_parity_bit = Io_Efuse_Set_Parity_Bit(command);
    // Set parity value
    if (set_parity_bit)
    {
        SET_BIT(command, PARITY_BIT);
    }
    else
    {
        CLEAR_BIT(command, PARITY_BIT);
    }
    status = Io_Efuse_WriteToEfuse(
        &command, e_fuse->chip_select.GPIO_Port, e_fuse->chip_select.GPIO_Pin);
    UNUSED(status);
}

uint16_t Io_Efuse_ReadReg(uint8_t register_address, struct Efuse *e_fuse)
{
    HAL_StatusTypeDef status;
    uint16_t          command        = 0x0000U;
    bool              set_parity_bit = false;
    uint16_t          tx_data[]      = { 0x0000U };
    uint16_t          rx_data[]      = { 0x0000U };

    // Place the Status Register address into bits 10->13
    command =
        (uint16_t)((SI_STATR_0_ADDR & EFUSE_ADDR_MASK) << EFUSE_ADDR_SHIFT);
    // Shift bit 3 of the address (SOA3: the channel number) to the 13th bit
    command = (uint16_t)(
        command | ((register_address & SOA3_MASK) << EFUSE_ADDR_SHIFT) |
        (register_address & (SOA2_MASK | SOA1_MASK | SOA0_MASK)));
    // Invert watchdog bit state
    if (e_fuse->watch_dog_state)
    {
        command |= WATCH_DOG_BIT;
    }
    else
    {
        command &= ~(WATCH_DOG_BIT);
    }
    // Invert watchdog bit state for next write
    e_fuse->watch_dog_state = !e_fuse->watch_dog_state;
    // Compute parity
    set_parity_bit = Io_Efuse_Set_Parity_Bit(command);
    // Set parity value
    if (set_parity_bit)
    {
        SET_BIT(command, PARITY_BIT);
    }
    else
    {
        CLEAR_BIT(command, PARITY_BIT);
    }
    tx_data[0] = command;
    status     = Io_Efuse_ReadFromEfuse(
        tx_data, rx_data, e_fuse->chip_select.GPIO_Port,
        e_fuse->chip_select.GPIO_Pin);
    UNUSED(status);
    // Only return register contents and clear bits 9->15
    CLEAR_BIT(rx_data[0], ~EFUSE_SO_DATA_MASK);
    return rx_data[0];
}

bool Io_Efuse_Set_Parity_Bit(uint16_t spi_command)
{
    uint8_t parity_bit;
    // Calculate parity for bits in spi command
    for (parity_bit = 0; spi_command > 0; spi_command >>= 1)
    {
        parity_bit ^= (spi_command & 1);
    }
    return ((parity_bit == 1) ? true : false);
}

HAL_StatusTypeDef Io_Efuse_WriteToEfuse(
    uint16_t *    TxData,
    GPIO_TypeDef *GPIO_Port,
    uint16_t      GPIO_Pin)
{
    HAL_StatusTypeDef status;

    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_RESET);
    status = HAL_SPI_Transmit(efuse_spi_handle, (uint8_t *)TxData, 1U, 100U);
    while (efuse_spi_handle->State != HAL_SPI_STATE_READY)
    {
    }
    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_SET);

    return status;
}

HAL_StatusTypeDef Io_Efuse_ReadFromEfuse(
    uint16_t *    TxData,
    uint16_t *    RxData,
    GPIO_TypeDef *GPIO_Port,
    uint16_t      GPIO_Pin)
{
    HAL_StatusTypeDef transmit_status, receive_status;
    // Send command to read from status register
    // Data is returned on the following SPI transfer
    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_RESET);
    transmit_status = HAL_SPI_Transmit(efuse_spi_handle, (uint8_t *)TxData, 1U, 100U);
    while (efuse_spi_handle->State != HAL_SPI_STATE_READY)
    {
    }
    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_SET);
    // Receive data from E-fuse
    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_RESET);
    receive_status = HAL_SPI_Receive(efuse_spi_handle, (uint8_t *)RxData, 1U, 100U);
    while (efuse_spi_handle->State != HAL_SPI_STATE_READY)
    {
    }
    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_SET);
    UNUSED(transmit_status);
    return receive_status;
}
