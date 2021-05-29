#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "main.h"

bool Io_ECI1118_GetMotorOTFault(void);
uint32_t Io_ECI1118_GetMotorPosition(void);
uint32_t Io_ECI1118_GetPosition(void);
void Io_ECI1118_SelectMemory(uint32_t memory);
void Io_ECI1118_SendParameter(uint32_t parameter);
uint32_t Io_ECI1118_GetParameter(void);
void Io_ECI1118_Reset(void);
void Io_ECI1118_SendTestCommand(uint32_t test_command);
uint32_t Io_ECI1118_GetTestValues(void);

enum EncoderMessages
{
    GetPosition = 0x0F,
    SelectMemory = 0x0E,
    SendParameter = 0x1C,
    GetParameter = 0x23,
    Reset = 0x2A,
    SendTestCommand = 0x15,
    GetTestValues = 0x31,
};
