#pragma once

#include "Io_Hal.h"

// clang-format off

/* @brief Set a software breakpoint if debugger is connected */
#define BREAK_IF_DEBUGGER_CONNECTED() \
if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) \
{ \
    __asm__ __volatile__ ("bkpt #0"); \
}

