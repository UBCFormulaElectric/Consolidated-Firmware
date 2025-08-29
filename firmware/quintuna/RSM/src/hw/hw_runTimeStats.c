#include "hw_runTimeStats.h"
#include "app_canTx.h"

const RunTimeStatsPublish publish_info = {
    .cpu_usage_setter = app_canTx_RSM_CpuUsage_set,
    .cpu_usage_max_setter = app_canTx_RSM_CpuUsageMax_set,
    .stack_usage_max_setter = app_canTx_RSM_StackUsage_set,
};