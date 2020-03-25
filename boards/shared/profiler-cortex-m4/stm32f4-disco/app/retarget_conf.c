//-- Includes ------------------------------------------------------------------
#include "retarget_conf.h"

//-- Functions -----------------------------------------------------------------
void retarget_init()
{
#ifdef USE_RETARGET
    // (1) Enable the peripheral clock of GPIO Port
    RETARGET_USART_GPIO_CLK_ENABLE();
    // Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up
    LL_GPIO_SetPinMode(RETARGET_USART_TX_GPIO_PORT, RETARGET_USART_TX_PIN, LL_GPIO_MODE_ALTERNATE);
    RETARGET_USART_SET_TX_GPIO_AF();
    LL_GPIO_SetPinSpeed(RETARGET_USART_TX_GPIO_PORT, RETARGET_USART_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(RETARGET_USART_TX_GPIO_PORT, RETARGET_USART_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(RETARGET_USART_TX_GPIO_PORT, RETARGET_USART_TX_PIN, LL_GPIO_PULL_UP);
    // Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up
    LL_GPIO_SetPinMode(RETARGET_USART_RX_GPIO_PORT, RETARGET_USART_RX_PIN, LL_GPIO_MODE_ALTERNATE);
    RETARGET_USART_SET_RX_GPIO_AF();
    LL_GPIO_SetPinSpeed(RETARGET_USART_RX_GPIO_PORT, RETARGET_USART_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(RETARGET_USART_RX_GPIO_PORT, RETARGET_USART_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(RETARGET_USART_RX_GPIO_PORT, RETARGET_USART_RX_PIN, LL_GPIO_PULL_UP);

    // (2) Enable USART peripheral clock and clock source
    RETARGET_USART_CLK_ENABLE();

    // (3) Configure USART functional parameters
    // TX/RX direction
    LL_USART_SetTransferDirection(RETARGET_USART, LL_USART_DIRECTION_TX_RX);
    // 8 data bit, 1 start bit, 1 stop bit, no parity
    LL_USART_ConfigCharacter(RETARGET_USART, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
    // Set Baudrate using APB frequency
    LL_USART_SetBaudRate(RETARGET_USART, RETARGET_USART_CLK, LL_USART_OVERSAMPLING_16, RETARGET_BAUDRATE);

    // (4) Enable USART
    LL_USART_Enable(RETARGET_USART);
#elif defined USE_RETARGET_ITM
#endif
}

char retarget_get_char()
{
#ifdef USE_RETARGET
    while (!LL_USART_IsActiveFlag_RXNE(RETARGET_USART)) {
    }
    return (char)LL_USART_ReceiveData8(RETARGET_USART);
#else
    return 0;
#endif
}

int retarget_put_char(char ch)
{
#ifdef USE_RETARGET
    while (!LL_USART_IsActiveFlag_TXE(RETARGET_USART)) {
    }
    LL_USART_TransmitData8(RETARGET_USART, ch);
#elif defined USE_RETARGET_ITM
    ITM_SendChar((uint32_t)ch);
#else
    (void)ch;
#endif
    return 0;
}
