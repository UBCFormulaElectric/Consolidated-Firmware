#pragma once

#ifdef __cplusplus
#define EXTERNC extern "C"
#define EXTERNCNORET extern "C" [[noreturn]]
#else
#define EXTERNC
#define EXTERNCNORET _Noreturn
#endif

EXTERNC void      tasks_preInit();
EXTERNC void      tasks_init();
EXTERNCNORET void tasks_run1Hz();
EXTERNCNORET void tasks_run100Hz();
EXTERNCNORET void tasks_run1kHz();
EXTERNCNORET void tasks_runCanTx();
EXTERNCNORET void tasks_runCanRx();

#undef EXTERNC
#undef EXTERNCNORET