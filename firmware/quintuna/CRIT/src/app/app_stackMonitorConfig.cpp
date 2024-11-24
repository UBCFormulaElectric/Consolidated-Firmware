#include "app_stackMonitorConfig.h"
#include "io_stackMonitorConfig.h"
extern "C"
{
#include "app_canTx.h"
}

namespace app
{
const StackMonitor<5> crit_stack_monitor{
    { stack_monitor_config{ task_1_hz_stack_monitor, 0.7f, app_canTx_CRIT_StackWaterMarkHighTask1HzLevel_set,
                            app_canTx_CRIT_Warning_StackWaterMarkHighTask1Hz_set },
      stack_monitor_config{ task_100_hz_stack_monitor, 0.7f, app_canTx_CRIT_StackWaterMarkHighTask100HzLevel_set,
                            app_canTx_CRIT_Warning_StackWaterMarkHighTask100Hz_set },
      stack_monitor_config{ task_1_khz_stack_monitor, 0.7f, app_canTx_CRIT_StackWaterMarkHighTask1kHzLevel_set,
                            app_canTx_CRIT_Warning_StackWaterMarkHighTask1kHz_set },
      stack_monitor_config{ task_canrx_stack_monitor, 0.7f, app_canTx_CRIT_StackWaterMarkHighTaskCanRxLevel_set,
                            app_canTx_CRIT_Warning_StackWaterMarkHighTaskCanTx_set },
      stack_monitor_config{ task_cantx_stack_monitor, 0.7f, app_canTx_CRIT_StackWaterMarkHighTaskCanTxLevel_set,
                            app_canTx_CRIT_Warning_StackWaterMarkHighTaskCanRx_set } }
};
} // namespace app
