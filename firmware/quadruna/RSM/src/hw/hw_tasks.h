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
 //   FDCAN_HandleTypeDef *    hfdcan1;
    IWDG_HandleTypeDef *     hiwdg1;
    const CanConfig *        can_config;
    struct StateMachine *    state_machine;
    struct HeartbeatMonitor *heartbeat_monitor;
} HwTasksConfig;