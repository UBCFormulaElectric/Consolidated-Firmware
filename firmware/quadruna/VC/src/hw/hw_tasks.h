#include "io_can.h"
#include "hw_can.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "App_SharedStateMachine.h"
#include "App_SharedHeartbeatMonitor.h"

extern struct StateMachine *    state_machine;
extern struct HeartbeatMonitor *heartbeat_monitor;

typedef struct
{
    ADC_HandleTypeDef *      hadc1;
    ADC_HandleTypeDef *      hadc3;
    FDCAN_HandleTypeDef *    hfdcan1;
    IWDG_HandleTypeDef *     hiwdg1;
    const CanConfig *        can_config;
    struct StateMachine *    state_machine;
    struct HeartbeatMonitor *heartbeat_monitor;
} HwTasksConfig;

void hw_tasks_preinit();
void hw_tasks_init(HwTasksConfig *hw_tasks_config);
void hw_tasks_100hz(void *argument);
void hw_tasks_cantx(void *argument);
void hw_tasks_canrx(void *argument);
void hw_tasks_1khz(void *argument);
void hw_tasks_1hz(void *argument);
