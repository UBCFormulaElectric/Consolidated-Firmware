

void write7seg(uint8_t command[])
{
    HAL_GPIO_WritePin(RCK_GPIO_Port, RCK_Pin, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi2, command, 3U, 100U);

    HAL_GPIO_WritePin(RCK_GPIO_Port, RCK_Pin, GPIO_PIN_SET);
}
