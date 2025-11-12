#include "hw_rtosTaskHandler.hpp"
#include "tasks.h"

static StaticTask<512, osPriorityNormal, "FDCanTx">    FDCanTx{ tasks_runCanFDTx };
static StaticTask<512, osPriorityNormal, "CanRx">      CanRx{ tasks_runcanRx };
static StaticTask<512, osPriorityHigh, "SxCanTxTask">  SxCanTxTask{ tasks_runCanSxTx };
static StaticTask<512, osPriorityHigh, "InvCanTxTask"> InvCanTxTask{ tasks_runCanInvTx };
static StaticTask<512, osPriorityNormal, "Task1Hz">    Task1Hz{ tasks_run1Hz };
static StaticTask<512, osPriorityRealtime, "Task1kHz"> Task1kHz{ tasks_run1kHz };

extern "C" void VCR_StartAllTasks(void)
{
    FDCanTx.start();
    CanRx.start();
    SxCanTxTask.start();
    InvCanTxTask.start();
    Task1Hz.start();
    Task1kHz.start();
}
