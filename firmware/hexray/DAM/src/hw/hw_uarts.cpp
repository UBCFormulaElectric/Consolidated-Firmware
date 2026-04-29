#include "hw_uarts.hpp"

#include "main.h"
#include <cassert>

const hw::Uart _900k_uart(huart2);

// Because of NTP needing the h
static StaticSemaphore_t _900k_uart_tx_mutex_storage;
SemaphoreHandle_t        _900k_uart_tx_mutex = nullptr;

void hw_uarts_init()
{
    _900k_uart_tx_mutex = xSemaphoreCreateMutexStatic(&_900k_uart_tx_mutex_storage);
}

const hw::Uart &hw::getUartFromHandle(const UART_HandleTypeDef *handle)
{
    assert(handle == &_900k_uart.getHandle());
    return _900k_uart;
}
