void write7seg(uint8_t command[])
{
    // disable display
    HAL_GPIO_WritePin(RCK_GPIO_Port, RCK_Pin, GPIO_PIN_RESET);
    // shift in value
    HAL_SPI_Transmit(&hspi2, command, 3U, 100U);
    // enable display
    HAL_GPIO_WritePin(RCK_GPIO_Port, RCK_Pin, GPIO_PIN_SET);
}
