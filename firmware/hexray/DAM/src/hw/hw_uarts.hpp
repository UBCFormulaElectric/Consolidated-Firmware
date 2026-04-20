#pragma once
#include "hw_uart.hpp"

#include <FreeRTOS.h>
#include <semphr.h>

extern const hw::Uart _900k_uart;

// Serializes all writes to _900k_uart so NTP and telem transmitters don't interleave.
extern SemaphoreHandle_t _900k_uart_tx_mutex;

void hw_uarts_init();
