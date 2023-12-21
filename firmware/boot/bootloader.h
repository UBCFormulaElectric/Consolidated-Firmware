#pragma once
#include <stdint.h>

void bootloader_init(void);
void bootloader_runInterfaceTask(void);
void bootloader_runTickTask(void);
void bootloader_runCanTxTask(void);

void bootloader_boardSpecific_init(void);
void bootloader_boardSpecific_tick(void);
void bootloader_boardSpecific_program(uint32_t address, uint64_t data);