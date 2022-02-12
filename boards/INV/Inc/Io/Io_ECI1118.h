#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "main.h"

bool     Io_ECI1118_GetMotorOTFault(void);
uint32_t Io_ECI1118_GetMotorPosition(void);
uint32_t Io_ECI1118_GetPosition(void);
void     Io_ECI1118_SelectMemory(uint32_t memory);
void     Io_ECI1118_SendParameter(uint32_t parameter);
uint32_t Io_ECI1118_GetParameter(void);
void     Io_ECI1118_Reset(void);
void     Io_ECI1118_SendTestCommand(uint32_t test_command);
uint32_t Io_ECI1118_GetTestValues(void);
void     Io_ECI1118_ClockFallingEdge(void);
void     Io_ECI1118_ClockRisingEdge(void);

enum EncoderMessages
{
    GET_POSITION      = 0x07,
    SELECT_MEMORY     = 0x0E,
    SEND_PARAMETER    = 0x1C,
    GET_PARAMETER     = 0x23,
    ENCODER_RESET     = 0x2A,
    SEND_TEST_COMMAND = 0x15,
    GET_TEST_VALUES   = 0x31,
};
