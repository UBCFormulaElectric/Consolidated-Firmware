#include "Io_ECI1118.h"
#include "main.h"
#include <stdlib.h>
#include <string.h>

static float             rotor_position_rad;
extern TIM_HandleTypeDef htim1;
extern DMA_HandleTypeDef hdma_tim1_ch2;
extern DMA_HandleTypeDef hdma_tim1_ch3;
static uint8_t           endat_command;
static uint32_t          endat_receive_data[ENDAT_DMA_SEND_SIZE];
static uint32_t          endat_send_data[ENDAT_DMA_SEND_SIZE];
static uint8_t           endat_clk_count     = 0;
static uint8_t           endat_received_bits = 0;

static const struct EncoderCommands encoder_commands = {
    .get_position      = 0x3 << 8 | GET_POSITION << 2 | 0x3,
    .select_memory     = 0x3 << 8 | SELECT_MEMORY << 2 | 0x3,
    .send_parameter    = 0x3 << 8 | SEND_PARAMETER << 2 | 0x3,
    .get_parameter     = 0x3 << 8 | GET_PARAMETER << 2 | 0x3,
    .encoder_reset     = 0x3 << 8 | ENCODER_RESET << 2 | 0x3,
    .send_test_command = 0x3 << 8 | SEND_TEST_COMMAND << 2 | 0x3,
    .get_test_values   = 0x3 << 8 | GET_TEST_VALUES << 2 | 0x3,
};

static struct DmaPackedEncoderCommands dma_packed_encoder_commands = {
    .dma_get_position      = { 0 },
    .dma_select_memory     = { 0 },
    .dma_send_parameter    = { 0 },
    .dma_get_parameter     = { 0 },
    .dma_encoder_reset     = { 0 },
    .dma_send_test_command = { 0 },
    .dma_get_test_values   = { 0 },
};

/*
ECI1118 Encoder EnDat 2.1 Protocol Overview:
1. Clock starts high
2. Generate clock signal with DMA
3. On the first falling edge of the clock the encoder saves the position
4. Transmit command to encoder with first 10 clock pulses
5. Keep sending clock pulses until encoder responds with full 18bit message +
5bit CRC
6. Read this data in with another DMA channel
7. Verify data with CRC TODO
*/

void Io_ECI1118_Init(void)
{
    Io_ECI1118_GeneratePacketBits(encoder_commands.get_position);
    Io_ECI1118_GeneratePacketBits(encoder_commands.encoder_reset);
    Io_ECI1118_GeneratePacketBits(encoder_commands.get_parameter);
    Io_ECI1118_GeneratePacketBits(encoder_commands.get_test_values);
    Io_ECI1118_GeneratePacketBits(encoder_commands.select_memory);
    Io_ECI1118_GeneratePacketBits(encoder_commands.send_parameter);
    Io_ECI1118_GeneratePacketBits(encoder_commands.send_test_command);

    HAL_GPIO_WritePin(ENDAT_CLK_EN_GPIO_Port, ENDAT_CLK_EN_Pin, GPIO_PIN_SET);
}

void Io_ECI1118_GeneratePacketBits(uint16_t command)
{
    // Reset send data to all zeros
    memset(&endat_send_data, 0, sizeof(endat_send_data));

    for (size_t i = 0; i < ENDAT_NUM_START_WAIT_CYCLES; i++)
    {
        // Set negative clock cycles for beginning of packet
        endat_send_data[i] |= ENDAT_CLK_TX_Pin << 16;
    }

    for (size_t i = 0; i < ENDAT_NUM_PACKET_BITS * 2; i += 2)
    {
        uint8_t j = i / 2; // j is skipping one edge of the clock

        // Send part of the packet
        if (j < ENDAT_NUM_SEND_BITS)
        {
            endat_send_data[i + ENDAT_NUM_START_WAIT_CYCLES] |=
                (command & 0x1 << j) ? ENDAT_DATA_TX_Pin
                                     : ENDAT_DATA_TX_Pin << 16;
        }

        // Pull receive line low to start receiving data from encoder
        if (j == ENDAT_START_RECEIVE_BIT)
        {
            // Start receiving data on the 12th bit
            endat_send_data[i + ENDAT_NUM_START_WAIT_CYCLES] |=
                ENDAT_DATA_SEND_Pin << 16;
        }

        // Set negative edge of the clock to zero
        endat_send_data[i + ENDAT_NUM_START_WAIT_CYCLES] |= ENDAT_CLK_TX_Pin
                                                            << 16;

        // Set positive edge of the clock to zero
        endat_send_data[i + ENDAT_NUM_START_WAIT_CYCLES + 1] |=
            ENDAT_CLK_TX_Pin;
    }

    for (size_t i = 0; i < ENDAT_NUM_START_WAIT_CYCLES; i++)
    {
        // Set positive clock cycles for beginning of packet
        endat_send_data
            [i + (ENDAT_NUM_START_WAIT_CYCLES + ENDAT_NUM_PACKET_BITS * 2)] |=
            ENDAT_CLK_TX_Pin;
    }

    if (command == encoder_commands.get_position)
    {
        for (size_t i = 0; i < ENDAT_DMA_SEND_SIZE; i++)
        {
            dma_packed_encoder_commands.dma_get_position[i] =
                endat_send_data[i];
        }
    }
    else if (command == encoder_commands.encoder_reset)
    {
        for (size_t i = 0; i < ENDAT_DMA_SEND_SIZE; i++)
        {
            dma_packed_encoder_commands.dma_encoder_reset[i] =
                endat_send_data[i];
        }
    }
    else if (command == encoder_commands.get_parameter)
    {
        for (size_t i = 0; i < ENDAT_DMA_SEND_SIZE; i++)
        {
            dma_packed_encoder_commands.dma_get_parameter[i] =
                endat_send_data[i];
        }
    }
    else if (command == encoder_commands.get_test_values)
    {
        for (size_t i = 0; i < ENDAT_DMA_SEND_SIZE; i++)
        {
            dma_packed_encoder_commands.dma_get_test_values[i] =
                endat_send_data[i];
        }
    }
    else if (command == encoder_commands.select_memory)
    {
        for (size_t i = 0; i < ENDAT_DMA_SEND_SIZE; i++)
        {
            dma_packed_encoder_commands.dma_select_memory[i] =
                endat_send_data[i];
        }
    }
    else if (command == encoder_commands.send_parameter)
    {
        for (size_t i = 0; i < ENDAT_DMA_SEND_SIZE; i++)
        {
            dma_packed_encoder_commands.dma_send_parameter[i] =
                endat_send_data[i];
        }
    }
    else if (command == encoder_commands.send_test_command)
    {
        for (size_t i = 0; i < ENDAT_DMA_SEND_SIZE; i++)
        {
            dma_packed_encoder_commands.dma_send_test_command[i] =
                endat_send_data[i];
        }
    }
    else
    {
        return;
    }
}

void Io_ECI1118_StartGetPosition(void)
{
    HAL_GPIO_WritePin(
        ENDAT_DATA_SEND_GPIO_Port, ENDAT_DATA_SEND_Pin, GPIO_PIN_SET);

    HAL_DMA_Start_IT(
        &hdma_tim1_ch3, (uint32_t) & (ENDAT_DATA_RX_GPIO_Port->IDR),
        (uint32_t)endat_receive_data, ENDAT_DMA_SEND_SIZE);
    HAL_DMA_Start_IT(
        &hdma_tim1_ch2,
        (uint32_t)dma_packed_encoder_commands.dma_get_position,
        (uint32_t) & (ENDAT_DATA_TX_GPIO_Port->BSRR),
        ENDAT_DMA_SEND_SIZE);

    TIM1->DIER |= (1 << TIM_DIER_CC2DE_Pos) | (1 << TIM_DIER_CC3DE_Pos);
    TIM1->CNT = 0;

    uint32_t tmp = (TIM_CCER_CC1E << (TIM_CHANNEL_2 & 0x1FU)) |
                   (TIM_CCER_CC1E << (TIM_CHANNEL_3 & 0x1FU));

    TIM1->CCER &= ~tmp;
    TIM1->CCER |= (uint32_t)(TIM_CCx_ENABLE << (TIM_CHANNEL_2 & 0x1FU)) |
                  (TIM_CCx_ENABLE << (TIM_CHANNEL_3 & 0x1FU));

    __HAL_TIM_MOE_ENABLE(&htim1);
    __HAL_TIM_ENABLE(&htim1);
}

float Io_ECI1118_ReadPosition(void)
{
    uint8_t  position_start_index = 0;
    uint32_t rotor_position_bits  = 0;

    for (size_t i = 0; i < ENDAT_NUM_PACKET_BITS; i++)
    {
        if (endat_receive_data
                [i * 2 + ENDAT_NUM_START_WAIT_CYCLES +
                 ENDAT_START_RECEIVE_BIT * 2 + 2] &
            ENDAT_DATA_RX_Pin)
        {
            position_start_index = i * 2 + ENDAT_NUM_START_WAIT_CYCLES +
                                   ENDAT_START_RECEIVE_BIT * 2 + 8;
            break;
        }
    }

    rotor_position_bits = (endat_receive_data[position_start_index + 0 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 0) |
                          (endat_receive_data[position_start_index + 1 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 1) |
                          (endat_receive_data[position_start_index + 2 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 2) |
                          (endat_receive_data[position_start_index + 3 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 3) |
                          (endat_receive_data[position_start_index + 4 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 4) |
                          (endat_receive_data[position_start_index + 5 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 5) |
                          (endat_receive_data[position_start_index + 6 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 6) |
                          (endat_receive_data[position_start_index + 7 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 7) |
                          (endat_receive_data[position_start_index + 8 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 8) |
                          (endat_receive_data[position_start_index + 9 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 9) |
                          (endat_receive_data[position_start_index + 10 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 10) |
                          (endat_receive_data[position_start_index + 11 * 2] &
                           ENDAT_DATA_RX_Pin) >>
                              (11 - 11) |
                          (endat_receive_data[position_start_index + 12 * 2] &
                           ENDAT_DATA_RX_Pin) <<
                              (1) |
                          (endat_receive_data[position_start_index + 13 * 2] &
                           ENDAT_DATA_RX_Pin) <<
                              (2) |
                          (endat_receive_data[position_start_index + 14 * 2] &
                           ENDAT_DATA_RX_Pin) <<
                              (3) |
                          (endat_receive_data[position_start_index + 15 * 2] &
                           ENDAT_DATA_RX_Pin) <<
                              (4) |
                          (endat_receive_data[position_start_index + 16 * 2] &
                           ENDAT_DATA_RX_Pin) <<
                              (5) |
                          (endat_receive_data[position_start_index + 17 * 2] &
                           ENDAT_DATA_RX_Pin) <<
                              (6);

    return (float)rotor_position_bits * ROTOR_POS_CONVERSION_FACTOR;
}

bool Io_ECI1118_GetMotorOTFault(void)
{
    return HAL_GPIO_ReadPin(nMOTOR_OT_ALARM_GPIO_Port, nMOTOR_OT_ALARM_Pin);
}

void Io_ECI1118_EncoderTransactionCompleteCallback(DMA_HandleTypeDef *hdma)
{
    // Disable the interrupt and DMA trigger for timer1 channels 2 and 3
    TIM1->DIER &= ~(1 << TIM_DIER_CC2DE_Pos | 1 << TIM_DIER_CC3DE_Pos);

    HAL_GPIO_WritePin(
        ENDAT_DATA_SEND_GPIO_Port, ENDAT_DATA_SEND_Pin, GPIO_PIN_RESET);
}
