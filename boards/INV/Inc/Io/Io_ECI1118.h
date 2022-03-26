#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "main.h"

void  Io_ECI1118_Init(void);
void  Io_ECI1118_GeneratePacketBits(uint16_t command);
bool  Io_ECI1118_GetMotorOTFault(void);
void  Io_ECI1118_StartGetPosition(void);
float Io_ECI1118_ReadPosition(void);
// void     Io_ECI1118_SelectMemory(uint32_t memory);
// void     Io_ECI1118_SendParameter(uint32_t parameter);
// uint32_t Io_ECI1118_GetParameter(void);
// void     Io_ECI1118_Reset(void);
// void     Io_ECI1118_SendTestCommand(uint32_t test_command);
// uint32_t Io_ECI1118_GetTestValues(void);
void Io_ECI1118_EncoderTransactionCompleteCallback(DMA_HandleTypeDef *hdma);

#define ROTOR_POS_CONVERSION_FACTOR (2 * (float)M_PI / (float)262144)
#define ENDAT_NUM_PACKET_BITS 45
#define ENDAT_START_RECEIVE_BIT 12
#define ENDAT_NUM_SEND_BITS 10
#define ENDAT_NUM_START_WAIT_CYCLES 5
#define ENDAT_NUM_END_WAIT_CYCLES 5
#define ENDAT_DMA_SEND_SIZE                                    \
    (ENDAT_NUM_PACKET_BITS * 2 + ENDAT_NUM_START_WAIT_CYCLES + \
     ENDAT_NUM_END_WAIT_CYCLES)

enum EncoderMessages
{
    GET_POSITION      = 0x07,
    SELECT_MEMORY     = 0x0E,
    SEND_PARAMETER    = 0x1C,
    GET_PARAMETER     = 0x23,
    ENCODER_RESET     = 0x2A,
    SEND_TEST_COMMAND = 0x15,
    GET_TEST_VALUES   = 0x31,
    ENDAT_NUM_COMMANDS,
};

struct EncoderCommands
{
    uint32_t get_position;
    uint32_t select_memory;
    uint32_t send_parameter;
    uint32_t get_parameter;
    uint32_t encoder_reset;
    uint32_t send_test_command;
    uint32_t get_test_values;
};

struct DmaPackedEncoderCommands
{
    uint32_t dma_get_position[ENDAT_DMA_SEND_SIZE];
    uint32_t dma_select_memory[ENDAT_DMA_SEND_SIZE];
    uint32_t dma_send_parameter[ENDAT_DMA_SEND_SIZE];
    uint32_t dma_get_parameter[ENDAT_DMA_SEND_SIZE];
    uint32_t dma_encoder_reset[ENDAT_DMA_SEND_SIZE];
    uint32_t dma_send_test_command[ENDAT_DMA_SEND_SIZE];
    uint32_t dma_get_test_values[ENDAT_DMA_SEND_SIZE];
};
