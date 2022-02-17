
#include "Io_ECI1118.h"
#include "main.h"

float                 rotor_position_rad;
extern TIM_HandleTypeDef htim1;
bool                     data_transaction_flag;
uint8_t                  endat_command;
uint32_t                 endat_receive_data  = 0;
uint8_t                  endat_clk_count     = 0;
uint8_t                  endat_received_bits = 0;

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
ECI1118 Encoder EnDat 2.1 Protocol Overview:
1. Clock starts high
2. set timer to generate clock signal
3. Falling edge of clock sets interrupt
4. first 10 interrupts, we are updating data line
5. next interrupts, looking for start bit, which is high
6. next 2 bits are fault 1, fault 2
7. now 16bit(?) position command
8. 5 bit crc
*/

void Io_ECI1118_StartGetPosition(void)
{
    // TODO return error if data is already sending
    if (!data_transaction_flag)
    {
        data_transaction_flag = 1;
        HAL_GPIO_WritePin(ENDAT_CLK_EN_GPIO_Port, ENDAT_CLK_EN_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ENDAT_DATA_SEND_GPIO_Port, ENDAT_DATA_SEND_Pin, GPIO_PIN_SET);
        endat_command     = GET_POSITION;
        HAL_TIM_OC_Start_IT(&htim1, 1);
    }
    else
    {
        while (data_transaction_flag)
            ;
    }
}
float Io_ECI1118_ReadPosition(void)
{
    if (!data_transaction_flag)
    {
        rotor_position_rad =
                (float)(endat_receive_data >> 3 & 0x3FFFF)*ROTOR_POS_CONVERSION_FACTOR; // 18 bit encoder rotor_position_rad = 0x3FFFF
        return rotor_position_rad;
    }
    else
    {
        while (data_transaction_flag)
            ;
        rotor_position_rad =
                (float)(endat_receive_data >> 3 & 0x3FFFF)*ROTOR_POS_CONVERSION_FACTOR; // 18 bit encoder rotor_position_rad = 0x3FFFF
        return rotor_position_rad;
    }
}

float Io_ECI1118_GetPositionBlocking(void)
{
    // TODO return error if data is already sending
    if (!data_transaction_flag)
    {
        data_transaction_flag = 1;
        endat_command     = GET_POSITION;
        HAL_TIM_OC_Start_IT(&htim1, 1);
        // This function is blocking until it's done
        while (data_transaction_flag)
            ;
        rotor_position_rad =
                (float)(endat_receive_data >> 3 & 0x3FFFF)*ROTOR_POS_CONVERSION_FACTOR; // 18 bit encoder rotor_position_rad = 0x3FFFF
        return rotor_position_rad;
    }
    else
    {
        while (data_transaction_flag)
            ;
    }
}

/*void Io_ECI1118_SelectMemory(float memory)
{
    rotor_position_rad = memory;
}

void Io_ECI1118_SendParameter(uint32_t parameter)
{
    rotor_position_rad = parameter;
}

uint32_t Io_ECI1118_GetParameter(void)
{
    return rotor_position_rad;
}

void Io_ECI1118_Reset(void)
{
    rotor_position_rad = 1;
}

void Io_ECI1118_SendTestCommand(uint32_t test_command)
{
    rotor_position_rad = test_command;
}

uint32_t Io_ECI1118_GetTestrotor_position_rads(void)
{
    return rotor_position_rad;
}*/

// send on rising edge, read on falling edge

// Clock cycle finished (falling edge)
void Io_ECI1118_ClockFallingEdge(void)
{
    {
        // Wait until the data rx line goes high, which indicates the start bit
        // of the received data
        bool data_rx_pinstatus =
            HAL_GPIO_ReadPin(ENDAT_DATA_RX_GPIO_Port, ENDAT_DATA_RX_Pin);
        if (endat_clk_count >= 11 && data_rx_pinstatus)
        {
            // ex. |crc here - 5bit|position here - ?bit|fault bits here -
            // 2bit|start bit here - 1bit| = 32bit or less total
            endat_receive_data |= (data_rx_pinstatus << endat_received_bits);
            endat_received_bits++;
        }
    }
}

// Rising edge of the clk
void Io_ECI1118_ClockRisingEdge(void)
{
    endat_clk_count++;
    // First and last two bits, send 1s
    if (endat_clk_count <= 2 || endat_clk_count == 9 || endat_clk_count == 10)
    {
        HAL_GPIO_WritePin(
            ENDAT_DATA_TX_GPIO_Port, ENDAT_DATA_TX_Pin, GPIO_PIN_SET);
    }
    // Send command to encoder
    else if (endat_clk_count >= 3 && endat_clk_count <= 8)
    {
        uint8_t bit_to_send = (endat_command >> (endat_clk_count - 3)) & 1;
        // send mode command
        if (bit_to_send)
        {
            HAL_GPIO_WritePin(
                ENDAT_DATA_TX_GPIO_Port, ENDAT_DATA_TX_Pin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(
                ENDAT_DATA_TX_GPIO_Port, ENDAT_DATA_TX_Pin, GPIO_PIN_RESET);
        }
    }
    else if (endat_clk_count > 8 && endat_clk_count < 42)
    {
        HAL_GPIO_WritePin(ENDAT_DATA_SEND_GPIO_Port, ENDAT_DATA_SEND_Pin, GPIO_PIN_RESET);
    }
    //End of transaction with encoder
    else
    {
        HAL_TIM_OC_Stop_IT(&htim1, 1);
        endat_clk_count     = 0;
        endat_received_bits = 0;
        HAL_GPIO_WritePin(ENDAT_CLK_EN_GPIO_Port, ENDAT_CLK_EN_Pin, GPIO_PIN_RESET);
        data_transaction_flag   = 0;
    }
}
