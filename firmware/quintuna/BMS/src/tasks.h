#pragma once

void           tasks_preInit(void);
void           tasks_init(void);
_Noreturn void tasks_run1Hz(void);
_Noreturn void tasks_run100Hz(void);
_Noreturn void tasks_run1kHz(void);
_Noreturn void tasks_runCanTx(void);
_Noreturn void tasks_runCanRx(void);
_Noreturn void tasks_runChimera(void);
_Noreturn void tasks_runLtcVoltages(void);
_Noreturn void tasks_runLtcTemps(void);
_Noreturn void tasks_runLtcDiagnostics(void);
