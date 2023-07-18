/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off


/* ------------------------------- Includes ------------------------------- */

#include <math.h>
#include <string.h>
#include "App_CanRx.h"

/* ------------------------------- Structs -------------------------------- */

/**
 * Struct for holding all messages received by JCT (i.e. the RX table).
 */
typedef struct
{
    FsmApps_Signals FsmApps_signals;
    FsmWarnings_Signals FsmWarnings_signals;
    FSM_Warnings_Signals FSM_Warnings_signals;
    FSM_Faults_Signals FSM_Faults_signals;
} JCT_RxMsgs;


/* -------------------------- Private Variables --------------------------- */

static JCT_RxMsgs rx_table;

/* ------------------------- Function Definitions ------------------------- */

void App_CanRx_Init()
{
    memset(&rx_table, 0, sizeof(JCT_RxMsgs));
    App_CanRx_FsmApps_PappsMappedPedalPercentage_Update(CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL);
    App_CanRx_FsmApps_SappsMappedPedalPercentage_Update(CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_START_VAL);
    App_CanRx_FsmWarnings_PappsOutOfRange_Update(CANSIG_FSM_WARNINGS_PAPPS_OUT_OF_RANGE_START_VAL);
    App_CanRx_FsmWarnings_SappsOutOfRange_Update(CANSIG_FSM_WARNINGS_SAPPS_OUT_OF_RANGE_START_VAL);
    App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1Hz_Update(CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_START_VAL);
    App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1kHz_Update(CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_START_VAL);
    App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanRx_Update(CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_START_VAL);
    App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanTx_Update(CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_START_VAL);
    App_CanRx_FsmWarnings_WatchdogFault_Update(CANSIG_FSM_WARNINGS_WATCHDOG_FAULT_START_VAL);
    App_CanRx_FsmWarnings_BspdFault_Update(CANSIG_FSM_WARNINGS_BSPD_FAULT_START_VAL);
    App_CanRx_FsmWarnings_LeftWheelSpeedOutOfRange_Update(CANSIG_FSM_WARNINGS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanRx_FsmWarnings_RightWheelSpeedOutOfRange_Update(CANSIG_FSM_WARNINGS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_START_VAL);
    App_CanRx_FsmWarnings_FlowRateOutOfRange_Update(CANSIG_FSM_WARNINGS_FLOW_RATE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FsmWarnings_SteeringAngleOutOfRange_Update(CANSIG_FSM_WARNINGS_STEERING_ANGLE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FsmWarnings_BrakePressureOutOfRange_Update(CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OUT_OF_RANGE_START_VAL);
    App_CanRx_FsmWarnings_BrakePressureSc_Update(CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_SC_START_VAL);
    App_CanRx_FsmWarnings_BrakePressureOc_Update(CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OC_START_VAL);
    App_CanRx_FsmWarnings_SteeringWheelBroke_Update(CANSIG_FSM_WARNINGS_STEERING_WHEEL_BROKE_START_VAL);
    App_CanRx_FSM_Warnings_FSM_TEST1_Update(CANSIG_FSM_WARNINGS_FSM_TEST1_START_VAL);
    App_CanRx_FSM_Warnings_FSM_TEST2_Update(CANSIG_FSM_WARNINGS_FSM_TEST2_START_VAL);
    App_CanRx_FSM_Faults_FSM_FAULT_TEST3_Update(CANSIG_FSM_FAULTS_FSM_FAULT_TEST3_START_VAL);
}

void App_CanRx_FsmApps_PappsMappedPedalPercentage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FsmApps_signals.PappsMappedPedalPercentage_value = tmp > CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanRx_FsmApps_SappsMappedPedalPercentage_Update(float value)
{
    if (value == NAN)
    {
        return;
    }
    
    const float tmp = value < CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN ? CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_MIN : value;
    rx_table.FsmApps_signals.SappsMappedPedalPercentage_value = tmp > CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX ? CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_MAX : tmp;
}

void App_CanRx_FsmWarnings_PappsOutOfRange_Update(bool value)
{
    rx_table.FsmWarnings_signals.PappsOutOfRange_value = value;
}

void App_CanRx_FsmWarnings_SappsOutOfRange_Update(bool value)
{
    rx_table.FsmWarnings_signals.SappsOutOfRange_value = value;
}

void App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1Hz_Update(bool value)
{
    rx_table.FsmWarnings_signals.StackWatermarkAboveThresholdTask1Hz_value = value;
}

void App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1kHz_Update(bool value)
{
    rx_table.FsmWarnings_signals.StackWatermarkAboveThresholdTask1kHz_value = value;
}

void App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanRx_Update(bool value)
{
    rx_table.FsmWarnings_signals.StackWatermarkAboveThresholdTaskCanRx_value = value;
}

void App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanTx_Update(bool value)
{
    rx_table.FsmWarnings_signals.StackWatermarkAboveThresholdTaskCanTx_value = value;
}

void App_CanRx_FsmWarnings_WatchdogFault_Update(bool value)
{
    rx_table.FsmWarnings_signals.WatchdogFault_value = value;
}

void App_CanRx_FsmWarnings_BspdFault_Update(bool value)
{
    rx_table.FsmWarnings_signals.BspdFault_value = value;
}

void App_CanRx_FsmWarnings_LeftWheelSpeedOutOfRange_Update(uint32_t value)
{
    rx_table.FsmWarnings_signals.LeftWheelSpeedOutOfRange_value = (value > CANSIG_FSM_WARNINGS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_MAX) ? CANSIG_FSM_WARNINGS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_MAX : value;
}

void App_CanRx_FsmWarnings_RightWheelSpeedOutOfRange_Update(uint32_t value)
{
    rx_table.FsmWarnings_signals.RightWheelSpeedOutOfRange_value = (value > CANSIG_FSM_WARNINGS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_MAX) ? CANSIG_FSM_WARNINGS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_MAX : value;
}

void App_CanRx_FsmWarnings_FlowRateOutOfRange_Update(uint32_t value)
{
    rx_table.FsmWarnings_signals.FlowRateOutOfRange_value = (value > CANSIG_FSM_WARNINGS_FLOW_RATE_OUT_OF_RANGE_MAX) ? CANSIG_FSM_WARNINGS_FLOW_RATE_OUT_OF_RANGE_MAX : value;
}

void App_CanRx_FsmWarnings_SteeringAngleOutOfRange_Update(uint32_t value)
{
    rx_table.FsmWarnings_signals.SteeringAngleOutOfRange_value = (value > CANSIG_FSM_WARNINGS_STEERING_ANGLE_OUT_OF_RANGE_MAX) ? CANSIG_FSM_WARNINGS_STEERING_ANGLE_OUT_OF_RANGE_MAX : value;
}

void App_CanRx_FsmWarnings_BrakePressureOutOfRange_Update(uint32_t value)
{
    rx_table.FsmWarnings_signals.BrakePressureOutOfRange_value = (value > CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OUT_OF_RANGE_MAX) ? CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OUT_OF_RANGE_MAX : value;
}

void App_CanRx_FsmWarnings_BrakePressureSc_Update(bool value)
{
    rx_table.FsmWarnings_signals.BrakePressureSc_value = value;
}

void App_CanRx_FsmWarnings_BrakePressureOc_Update(bool value)
{
    rx_table.FsmWarnings_signals.BrakePressureOc_value = value;
}

void App_CanRx_FsmWarnings_SteeringWheelBroke_Update(bool value)
{
    rx_table.FsmWarnings_signals.SteeringWheelBroke_value = value;
}

void App_CanRx_FSM_Warnings_FSM_TEST1_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_TEST1_value = value;
}

void App_CanRx_FSM_Warnings_FSM_TEST2_Update(bool value)
{
    rx_table.FSM_Warnings_signals.FSM_TEST2_value = value;
}

void App_CanRx_FSM_Faults_FSM_FAULT_TEST3_Update(bool value)
{
    rx_table.FSM_Faults_signals.FSM_FAULT_TEST3_value = value;
}

float App_CanRx_FsmApps_PappsMappedPedalPercentage_Get()
{
    return rx_table.FsmApps_signals.PappsMappedPedalPercentage_value;
}

float App_CanRx_FsmApps_SappsMappedPedalPercentage_Get()
{
    return rx_table.FsmApps_signals.SappsMappedPedalPercentage_value;
}

bool App_CanRx_FsmWarnings_PappsOutOfRange_Get()
{
    return rx_table.FsmWarnings_signals.PappsOutOfRange_value;
}

bool App_CanRx_FsmWarnings_SappsOutOfRange_Get()
{
    return rx_table.FsmWarnings_signals.SappsOutOfRange_value;
}

bool App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1Hz_Get()
{
    return rx_table.FsmWarnings_signals.StackWatermarkAboveThresholdTask1Hz_value;
}

bool App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTask1kHz_Get()
{
    return rx_table.FsmWarnings_signals.StackWatermarkAboveThresholdTask1kHz_value;
}

bool App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanRx_Get()
{
    return rx_table.FsmWarnings_signals.StackWatermarkAboveThresholdTaskCanRx_value;
}

bool App_CanRx_FsmWarnings_StackWatermarkAboveThresholdTaskCanTx_Get()
{
    return rx_table.FsmWarnings_signals.StackWatermarkAboveThresholdTaskCanTx_value;
}

bool App_CanRx_FsmWarnings_WatchdogFault_Get()
{
    return rx_table.FsmWarnings_signals.WatchdogFault_value;
}

bool App_CanRx_FsmWarnings_BspdFault_Get()
{
    return rx_table.FsmWarnings_signals.BspdFault_value;
}

uint32_t App_CanRx_FsmWarnings_LeftWheelSpeedOutOfRange_Get()
{
    return rx_table.FsmWarnings_signals.LeftWheelSpeedOutOfRange_value;
}

uint32_t App_CanRx_FsmWarnings_RightWheelSpeedOutOfRange_Get()
{
    return rx_table.FsmWarnings_signals.RightWheelSpeedOutOfRange_value;
}

uint32_t App_CanRx_FsmWarnings_FlowRateOutOfRange_Get()
{
    return rx_table.FsmWarnings_signals.FlowRateOutOfRange_value;
}

uint32_t App_CanRx_FsmWarnings_SteeringAngleOutOfRange_Get()
{
    return rx_table.FsmWarnings_signals.SteeringAngleOutOfRange_value;
}

uint32_t App_CanRx_FsmWarnings_BrakePressureOutOfRange_Get()
{
    return rx_table.FsmWarnings_signals.BrakePressureOutOfRange_value;
}

bool App_CanRx_FsmWarnings_BrakePressureSc_Get()
{
    return rx_table.FsmWarnings_signals.BrakePressureSc_value;
}

bool App_CanRx_FsmWarnings_BrakePressureOc_Get()
{
    return rx_table.FsmWarnings_signals.BrakePressureOc_value;
}

bool App_CanRx_FsmWarnings_SteeringWheelBroke_Get()
{
    return rx_table.FsmWarnings_signals.SteeringWheelBroke_value;
}

bool App_CanRx_FSM_Warnings_FSM_TEST1_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_TEST1_value;
}

bool App_CanRx_FSM_Warnings_FSM_TEST2_Get()
{
    return rx_table.FSM_Warnings_signals.FSM_TEST2_value;
}

bool App_CanRx_FSM_Faults_FSM_FAULT_TEST3_Get()
{
    return rx_table.FSM_Faults_signals.FSM_FAULT_TEST3_value;
}

