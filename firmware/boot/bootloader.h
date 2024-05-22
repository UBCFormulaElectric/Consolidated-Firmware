#pragma once
#include <stdint.h>

// Keep CAN protocol in sync with:
// canup/bootloader.py

// Command IDs.
#define ERASE_SECTOR_ID 1000
#define PROGRAM_ID 1001
#define VERIFY_ID 1002

// Reply IDs.
#define ERASE_SECTOR_COMPLETE_ID 1010
#define APP_VALIDITY_ID 1011

void           bootloader_preInit(void);
void           bootloader_init(void);
_Noreturn void bootloader_runInterfaceTask(void);
_Noreturn void bootloader_runTickTask(void);
_Noreturn void bootloader_runCanTxTask(void);

void bootloader_boardSpecific_init(void);
void bootloader_boardSpecific_tick(void);
void bootloader_boardSpecific_program(uint32_t address, uint64_t data);