#pragma once
#include "util_utils.hpp"

CFUNC void bootloader_preinit();
CFUNC void bootloader_init();
CFUNC NORET void bootloader_runInterfaceTask(void);
CFUNC NORET void bootloader_runTickTask(void);
CFUNC NORET void bootloader_runCanTxTask(void);
