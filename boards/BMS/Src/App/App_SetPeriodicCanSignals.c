#include "App_SetPeriodicCanSignals.h"
#include "App_SharedSetPeriodicCanSignals.h"
#include "App_InRangeCheck.h"

STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(BmsCanTxInterface)

void App_SetPeriodicCanSignals_Imd(
    struct BmsCanTxInterface *can_tx,
    struct Imd *              imd)
{
    App_CanTx_SetPeriodicSignal_SECONDS_SINCE_POWER_ON(
        can_tx, App_Imd_GetSecondsSincePowerOn(imd));
    App_CanTx_SetPeriodicSignal_FREQUENCY(can_tx, App_Imd_GetPwmFrequency(imd));
    App_CanTx_SetPeriodicSignal_DUTY_CYCLE(
        can_tx, App_Imd_GetPwmDutyCycle(imd));

    const struct Imd_Condition condition = App_Imd_GetCondition(imd);
    App_CanTx_SetPeriodicSignal_CONDITION(can_tx, (uint8_t)condition.name);

    App_CanTx_SetPeriodicSignal_VALID_DUTY_CYCLE(
        can_tx, condition.pwm_encoding.valid_duty_cycle);

    switch (condition.name)
    {
        case IMD_NORMAL:
        {
            if (condition.pwm_encoding.valid_duty_cycle == true)
            {
                App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_10_HZ(
                    can_tx,
                    condition.pwm_encoding.insulation_measurement_dcp_kohms);
            }
        }
        break;
        case IMD_UNDERVOLTAGE_DETECTED:
        {
            if (condition.pwm_encoding.valid_duty_cycle == true)
            {
                App_CanTx_SetPeriodicSignal_INSULATION_MEASUREMENT_DCP_20_HZ(
                    can_tx,
                    condition.pwm_encoding.insulation_measurement_dcp_kohms);
            }
        }
        break;
        case IMD_SST:
        {
            App_CanTx_SetPeriodicSignal_SPEED_START_STATUS_30_HZ(
                can_tx, condition.pwm_encoding.speed_start_status);
        }
        break;
        case IMD_SHORT_CIRCUIT:
        case IMD_DEVICE_ERROR:
        case IMD_EARTH_FAULT:
        case IMD_INVALID:
        {
            // Nothing to do for conditions that don't carry a payload
        }
        break;
    }
}

void App_SetPeriodicSignals_CellMonitorInRangeChecks(
    struct BmsCanTxInterface *const can_tx,
    const struct CellMonitor *const cell_monitor)
{
    App_CellMonitor_ReadCellVoltages(cell_monitor);

    struct InRangeCheck *min_cell_voltage_in_range_check =
        App_CellMonitor_GetMinCellVoltageInRangeCheck(cell_monitor);
    struct InRangeCheck *max_cell_voltage_in_range_check =
        App_CellMonitor_GetMaxCellVoltageInRangeCheck(cell_monitor);
    struct InRangeCheck *average_cell_voltage_in_range_check =
        App_CellMonitor_GetAverageCellVoltageInRangeCheck(cell_monitor);
    struct InRangeCheck *pack_voltage_in_range_check =
        App_CellMonitor_GetPackVoltageInRangeCheck(cell_monitor);
    struct InRangeCheck *segment_0_voltage_in_range_check =
        App_CellMonitor_GetSegment0VoltageInRangeCheck(cell_monitor);
    struct InRangeCheck *segment_1_voltage_in_range_check =
        App_CellMonitor_GetSegment1VoltageInRangeCheck(cell_monitor);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, min_cell_voltage_in_range_check,
        App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE,
        App_CanTx_SetPeriodicSignal_MIN_CELL_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MIN_CELL_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, max_cell_voltage_in_range_check,
        App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE,
        App_CanTx_SetPeriodicSignal_MAX_CELL_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_MAX_CELL_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, average_cell_voltage_in_range_check,
        App_CanTx_SetPeriodicSignal_AVERAGE_CELL_VOLTAGE,
        App_CanTx_SetPeriodicSignal_AVERAGE_CELL_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_AVERAGE_CELL_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_AVERAGE_CELL_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_AVERAGE_CELL_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, pack_voltage_in_range_check,
        App_CanTx_SetPeriodicSignal_PACK_VOLTAGE,
        App_CanTx_SetPeriodicSignal_PACK_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_PACK_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_PACK_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_PACK_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, segment_0_voltage_in_range_check,
        App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE,
        App_CanTx_SetPeriodicSignal_SEGMENT_0_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_SEGMENT_0_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_SEGMENT_0_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_SEGMENT_0_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);

    App_SetPeriodicCanSignals_InRangeCheck(
        can_tx, segment_1_voltage_in_range_check,
        App_CanTx_SetPeriodicSignal_SEGMENT_1_VOLTAGE,
        App_CanTx_SetPeriodicSignal_SEGMENT_1_VOLTAGE_OUT_OF_RANGE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_SEGMENT_1_VOLTAGE_OUT_OF_RANGE_OK_CHOICE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_SEGMENT_1_VOLTAGE_OUT_OF_RANGE_UNDERFLOW_CHOICE,
        CANMSGS_BMS_CELL_VOLTAGE_ERRORS_SEGMENT_1_VOLTAGE_OUT_OF_RANGE_OVERFLOW_CHOICE);
}
