#pragma once

void bootloader_init(void);
void bootloader_runInterfaceTask(void);
void bootloader_runTickTask(void);
void bootloader_runCanTxTask(void);

void bootloader_boardSpecific_init(void);
void bootloader_boardSpecific_deinit(void);
void bootloader_boardSpecific_tick(void);