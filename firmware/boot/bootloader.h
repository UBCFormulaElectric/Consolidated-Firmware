#pragma once
#include <stdint.h>

void bootloader_init(void);
void bootloader_runInterfaceTask(void);
void bootloader_runTickTask(void);
void bootloader_runCanTxTask(void);

void bootloader_config_init(void);
void bootloader_config_tick(void);
void bootloader_config_program(uint32_t address, uint64_t data);
void bootloader_config_flushProgramBuffer(uint32_t address);