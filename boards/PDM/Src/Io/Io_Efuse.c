#include "Io_Efuse.h"

#pragma GCC diagnostic ignored "-Wconversion"

#define EFUSE_ADDR_MASK 0xFU
#define EFUSE_ADDR_SHIFT 0x0AU
#define EFUSE_SI_DATA_MASK 0x1FFU
#define EFUSE_SO_DATA_MASK 0xFF // Ignore Normal-Mode status bit (bit 9)

#define WATCH_DOG_BIT (1U << 15U)
#define PARITY_BIT (1U << 14U)

struct Efuse
{
    void (*configure_efuse)(struct Efuse *e_fuse);
    StatusType_e (*get_status)(struct Efuse *e_fuse);
    FaultType_e (*get_channel0_faults)(struct Efuse *e_fuse);
    FaultType_e (*get_channel1_faults)(struct Efuse *e_fuse);
    bool (*get_channel0_current)(struct Efuse *e_fuse, float *channel0_current);
    bool (*get_channel1_current)(struct Efuse *e_fuse, float *channel1_current);

    bool watch_dog_state;
};

// The SPI handle for the SPI device the E-Fuses are connected to
static SPI_HandleTypeDef *efuse_spi_handle;

void Io_Efuse_Init(SPI_HandleTypeDef *const hspi)
{
    efuse_spi_handle = hspi;
}

StatusType_e Io_Efuse_GetAux1_Aux2Status(struct Efuse *e_fuse)
{
    return Io_Efuse_Aux1Aux2ReadReg(SO_STATR_ADDR, e_fuse);
}

FaultType_e Io_Efuse_GetAux1Faults(struct Efuse *e_fuse)
{
    return Io_Efuse_Aux1Aux2ReadReg(SO_FAULTR_0_ADDR, e_fuse);
}

FaultType_e Io_Efuse_GetAux2Faults(struct Efuse *e_fuse)
{
    return Io_Efuse_Aux1Aux2ReadReg(SO_FAULTR_1_ADDR, e_fuse);
}

bool Io_Efuse_GetAux1Current(struct Efuse *e_fuse, float *aux1_current)
{
    Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(
        AUX1_CURRENT_SENSE_CHANNEL, e_fuse);

    if (HAL_GPIO_ReadPin(
            CUR_SYNC_AUX1_AUX2_GPIO_Port, CUR_SYNC_AUX1_AUX2_Pin) ==
        GPIO_PIN_RESET)
    {
        *aux1_current = Io_CurrentSense_GetAux1Current();
        return true;
    }

    return false;
}

bool Io_Efuse_GetAux2Current(struct Efuse *e_fuse, float *aux2_current)
{
    Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(
        AUX2_CURRENT_SENSE_CHANNEL, e_fuse);

    if (HAL_GPIO_ReadPin(
            CUR_SYNC_AUX1_AUX2_GPIO_Port, CUR_SYNC_AUX1_AUX2_Pin) ==
        GPIO_PIN_RESET)
    {
        *aux2_current = Io_CurrentSense_GetAux2Current();
        return true;
    }

    return false;
}

void Io_Efuse_Aux1Aux2ConfigureChannelMonitoring(
    uint8_t       selection,
    struct Efuse *e_fuse)
{
    uint16_t reg_val = 0x0000;

    // Read original content of GCR Register
    reg_val = Io_Efuse_Aux1Aux2ReadReg(SI_GCR_ADDR, e_fuse);

    CLEAR_BIT(reg_val, (CSNS1_EN_MASK | CSNS0_EN_MASK));
    SET_BIT(reg_val, (selection & (CSNS1_EN_MASK | CSNS0_EN_MASK)));

    Io_Efuse_Aux1Aux2WriteReg(SI_GCR_ADDR, reg_val, e_fuse);
}

void Io_Efuse_Aux1Aux2ConfigureEfuse(struct Efuse *e_fuse)
{
    // Global config register
    Io_Efuse_Aux1Aux2WriteReg(SI_GCR_ADDR, GCR_CONFIG, e_fuse);

    // Channel 0 config registers
    Io_Efuse_Aux1Aux2WriteReg(SI_RETRY_0_ADDR, RETRY_0_CONFIG, e_fuse);
    Io_Efuse_Aux1Aux2WriteReg(SI_CONFR_0_ADDR, CONFR_0_CONFIG, e_fuse);
    Io_Efuse_Aux1Aux2WriteReg(SI_OCR_0_ADDR, OCR_0_CONFIG, e_fuse);

    // Channel 1 config registers
    Io_Efuse_Aux1Aux2WriteReg(SI_RETRY_1_ADDR, RETRY_1_CONFIG, e_fuse);
    Io_Efuse_Aux1Aux2WriteReg(SI_CONFR_1_ADDR, CONFR_1_CONFIG, e_fuse);
    Io_Efuse_Aux1Aux2WriteReg(SI_OCR_1_ADDR, OCR_1_CONFIG, e_fuse);
}

void Io_Efuse_Aux1Aux2WriteReg(
    uint8_t       register_address,
    uint16_t      register_value,
    struct Efuse *e_fuse)
{
    Io_Efuse_WriteReg(
        register_address, register_value, CSB_AUX1_AUX2_GPIO_Port,
        CSB_AUX1_AUX2_Pin, e_fuse);
}

uint16_t
    Io_Efuse_Aux1Aux2ReadReg(uint8_t register_address, struct Efuse *e_fuse)
{
    return Io_Efuse_ReadReg(
        register_address, CSB_AUX1_AUX2_GPIO_Port, CSB_AUX1_AUX2_Pin, e_fuse);
}

void Io_Efuse_WriteReg(
    uint8_t       register_address,
    uint16_t      register_value,
    GPIO_TypeDef *ChipSelect_GPIO_Port,
    uint16_t      ChipSelect_GPIO_Pin,
    struct Efuse *e_fuse)
{
    HAL_StatusTypeDef status;
    uint16_t          command = 0x0000U;

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

    // Compute and set/clear parity value
    Io_Efuse_SetParityBit(&command);

    status = Io_Efuse_WriteToEfuse(
        &command, ChipSelect_GPIO_Port, ChipSelect_GPIO_Pin);
    UNUSED(status);
}

uint16_t Io_Efuse_ReadReg(
    uint8_t       register_address,
    GPIO_TypeDef *ChipSelect_GPIO_Port,
    uint16_t      ChipSelect_GPIO_Pin,
    struct Efuse *e_fuse)
{
    HAL_StatusTypeDef status;
    uint16_t          command   = 0x0000U;
    uint16_t          tx_data[] = { 0x0000U };
    uint16_t          rx_data[] = { 0x0000U };

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

    // Compute and set/clear parity bit
    Io_Efuse_SetParityBit(&command);

    tx_data[0] = command;
    status     = Io_Efuse_ReadFromEfuse(
        tx_data, rx_data, ChipSelect_GPIO_Port, ChipSelect_GPIO_Pin);
    UNUSED(status);
    // Only return register contents and clear bits 9->15
    CLEAR_BIT(rx_data[0], ~EFUSE_SO_DATA_MASK);
    return rx_data[0];
}

static void Io_Efuse_SetParityBit(uint16_t *spi_command)
{
    uint16_t spi_data = *spi_command;
    uint8_t  parity_bit;
    // Calculate parity for bits in spi command
    for (parity_bit = 0; spi_data > 0; spi_data >>= 1)
    {
        parity_bit ^= (spi_data & 1);
    }

    if (parity_bit == 1)
    {
        SET_BIT(*spi_command, PARITY_BIT);
    }
    else
    {
        CLEAR_BIT(*spi_command, PARITY_BIT);
    }
}

static HAL_StatusTypeDef Io_Efuse_WriteToEfuse(
    uint16_t *    TxData,
    GPIO_TypeDef *GPIO_Port,
    uint16_t      GPIO_Pin)
{
    HAL_StatusTypeDef status;

    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_RESET);
    status = HAL_SPI_TransmitReceive(
        efuse_spi_handle, (uint8_t *)TxData, (uint8_t *)TxData, 1U, 100U);
    HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_SET);

    return status;
}

static HAL_StatusTypeDef Io_Efuse_ReadFromEfuse(
    uint16_t *    tx_data,
    uint16_t *    rx_data,
    GPIO_TypeDef *chip_select_port,
    uint16_t      chip_select_pin)
{
    HAL_StatusTypeDef transmit_status, receive_status;

    // Send command to read from status register
    // Data is returned on the following SPI transfer
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_RESET);
    transmit_status = HAL_SPI_TransmitReceive(
        efuse_spi_handle, (uint8_t *)tx_data, (uint8_t *)rx_data, 1U, 100U);
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_SET);

    // Receive data from E-fuse by sending dummy data
    *tx_data = 0xFFFF;
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_RESET);
    receive_status = HAL_SPI_TransmitReceive(
        efuse_spi_handle, (uint8_t *)tx_data, (uint8_t *)rx_data, 1U, 100U);
    HAL_GPIO_WritePin(chip_select_port, chip_select_pin, GPIO_PIN_SET);

    UNUSED(transmit_status);
    return receive_status;
}
