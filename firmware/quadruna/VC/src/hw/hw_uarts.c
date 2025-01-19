#include "hw_uarts.h"
#include "main.h"

const UART debug_uart    = { .handle = &huart7 };
const UART sbg_uart      = { .handle = &huart2 };
const UART modem2G4_uart = { .handle = &huart3 };
const UART modem900_uart = { .handle = &huart1 };
