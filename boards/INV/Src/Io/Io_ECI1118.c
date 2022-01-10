
#include "Io_ECI1118.h"
#include "main.h"

uint32_t                 value;
extern TIM_HandleTypeDef htim1;
bool data_sending_flag;
uint8_t command;
uint32_t receive_data;
uint8_t endat_clk_count = 1;

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

/*
1. Clock starts high
2. set timer to generate clock signal
3. Falling edge of clock sets interrupt
4. first 10 interrupts, we are updating data line
5. next interrupts, looking for start bit, which is high
6. next 2 bits are fault 1, fault 2
7. now 16bit(?) position command
8. 5 bit crc
*/

uint32_t Io_ECI1118_StartGetPosition(void)
{
    data_sending_flag = 1;
    command = GET_POSITION;
    HAL_TIM_PWM_Start_IT(&htim1, 1);
    return value;
}
uint32_t Io_ECI1118_ReadPosition(void)
{

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

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        endat_clk_count++;

        //transaction is complete
        if(endat_clk_count == 60)
        {
            HAL_TIM_PWM_Stop_IT(&htim1, 1);
            endat_clk_count = 0;
        }

    }
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM1)
    {

    }
}
