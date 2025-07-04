#pragma once

#ifdef __cplusplus
#define EXTERNC extern "C"
#define EXTERNCNORET extern "C" [[noreturn]]
#else
#define EXTERNC
#define EXTERNCNORET _Noreturn
#endif

EXTERNC void      tasks_init();
EXTERNCNORET void tasks_default();
EXTERNCNORET void tasks_canRx();
EXTERNCNORET void tasks_canTx();

#undef EXTERNC
#undef EXTERNCNORET