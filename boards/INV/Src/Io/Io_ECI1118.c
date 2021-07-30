
#include "Io_ECI1118.h"
#include "main.h"

uint32_t                 value;
extern SPI_HandleTypeDef hspi3;

bool Io_ECI1118_GetMotorOTFault(void)
{
    if (HAL_GPIO_ReadPin(nMOD_OT_ALARM_GPIO_Port, nMOD_OT_ALARM_Pin))
    {
        return false;
    }
    else
    {
        return true;
    }
}

uint32_t Io_ECI1118_GetPosition(void)
{
    uint8_t send_data = GET_POSITION;
    uint8_t receive_data;
    HAL_SPI_TransmitReceive(&hspi3, &send_data, &receive_data, 1, 100);
    return value;
}

void Io_ECI1118_SelectMemory(uint32_t memory)
{
    value = memory;
}

void Io_ECI1118_SendParameter(uint32_t parameter)
{
    value = parameter;
}

uint32_t Io_ECI1118_GetParameter(void)
{
    return value;
}

void Io_ECI1118_Reset(void)
{
    value = 1;
}

void Io_ECI1118_SendTestCommand(uint32_t test_command)
{
    value = test_command;
}

uint32_t Io_ECI1118_GetTestValues(void)
{
    return value;
}
