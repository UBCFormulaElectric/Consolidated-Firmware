#pragma once

void tasks_preInit();
void tasks_init();
void tasks_100Hz(void *argument);
void tasks_canTx(void *argument);
void tasks_canRx(void *argument);
void tasks_1Khz(void *argument);
void tasks_1Hz(void *argument);
