#pragma once
#include "hw_uart.hpp"

#include <FreeRTOS.h>
#include <semphr.h>

extern const hw::Uart _900k_uart;

// Serializes all writes to _900k_uart so NTP and telem transmitters don't interleave.
//
// Exposed (not hidden inside _900k_uart.transmit()) because the NTP path needs to
// hold the lock across more than the transmit call: it must block on any in-flight
// TX *before* sampling t0 from the RTC, otherwise t0 includes queue-wait time and
// the NTP round-trip estimate is skewed. See io::telemRx::transmitNTPStartMsg.
// Plain telem TX callers just wrap the transmit() call in a hw::MutexGuard.
extern SemaphoreHandle_t _900k_uart_tx_mutex;

void hw_uarts_init();
