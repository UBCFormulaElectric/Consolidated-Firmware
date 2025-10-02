#pragma once
#include <stdint.h>

// Keep CAN protocol in sync with:
// canup/bootloader.py

#define STATUS_10HZ_ID_LOWBITS (0x0)
#define START_UPDATE_ID_LOWBITS (0x1)
#define UPDATE_ACK_ID_LOWBITS (0x2)
#define GO_TO_APP_LOWBITS (0x3)
#define ERASE_SECTOR_ID_LOWBITS (0x4)
#define ERASE_SECTOR_COMPLETE_ID_LOWBITS (0x5)
#define PROGRAM_ID_LOWBITS (0x6)
#define VERIFY_ID_LOWBITS (0x7)
#define APP_VALIDITY_ID_LOWBITS (0x8)
#define GO_TO_BOOT (0x9)

void           bootloader_preInit(void);
void           bootloader_init(void);
_Noreturn void bootloader_runInterfaceTask(void);
_Noreturn void bootloader_runTickTask(void);
_Noreturn void bootloader_runCanTxTask(void);

void bootloader_boardSpecific_init(void);
void bootloader_boardSpecific_tick(void);
void bootloader_boardSpecific_program(uint32_t address, uint64_t data);