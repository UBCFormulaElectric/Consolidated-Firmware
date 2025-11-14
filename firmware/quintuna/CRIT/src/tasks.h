#pragma once

void tasks_preInit();
void tasks_init();

#ifdef __cplusplus
extern "C"
{
#endif

    _Noreturn tasks_run1kHz(void *argument);
    _Noreturn tasks_run100Hz(void *argument);
    _Noreturn tasks_run1Hz(void *argument);
    _Noreturn tasks_runCanRx(void *argument);
    _Noreturn tasks_runCanTx(void *argument);

#ifdef __cplusplus
}
#endif
