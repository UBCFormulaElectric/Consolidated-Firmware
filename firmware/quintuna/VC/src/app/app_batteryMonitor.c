#include "app_batteryMonitor.h"
#include "io_lowVoltageBattery.h"
#include "app_canTx.h"

void app_batteryMonitor_broadcast(void)
{
    Battery_Status bat_status;
    SafetyStatusA  safetyA;
    SafetyStatusB  safetyB;
    SafetyStatusC  safetyC;

    io_lowvoltageBattery_batteryStatus(&bat_status);
    io_lowVoltageBattery_SafetyStatusCheck(&safetyA, &safetyB, &safetyC);

    // set battery status can messages
    // is casting sus here
    app_canTx_VC_StateOfCharge_set((float)io_lowVoltageBattery_get_SOC());
    app_canTx_VC_PermanentFault_set(bat_status.PF);
    app_canTx_VC_SafetyFault_set(bat_status.SS);
    app_canTx_VC_OpenWireCheck_set(bat_status.COW_CHECK);
    app_canTx_VC_InternalWatchdog_set(bat_status.WD);
    app_canTx_VC_PrechargeMode_set(bat_status.PCHG_MODE);

    // set battery safety can messages

    // safety messages A
    app_canTx_VC_Undervoltage_set(safetyA.CUV);
    app_canTx_VC_Overvoltage_set(safetyA.COV);
    app_canTx_VC_ChargingOvercurrent_set(safetyA.OCC);
    app_canTx_VC_OvercurrentInDischarge1_set(safetyA.OCD1);
    app_canTx_VC_OvercurrentInDischarge2_set(safetyA.OCD2);
    app_canTx_VC_ShortCircuitInDischarge_set(safetyA.SCD);

    // safety messages B
    app_canTx_VC_UndertemperatureInCharge_set(safetyB.UTC);
    app_canTx_VC_UndertemperatureInDischarge_set(safetyB.UTD);
    app_canTx_VC_InternalUndertemperature_set(safetyB.UTINT);
    app_canTx_VC_OvertemperatureInCharge_set(safetyB.OTC);
    app_canTx_VC_OvertemperatureInDischarge_set(safetyB.OTD);
    app_canTx_VC_InternalOvertemperature_set(safetyB.OTINT);
    app_canTx_VC_FETOvertemperature_set(safetyB.OTF);

    // safety messages C
    app_canTx_VC_PrechargeTimeout_set(safetyC.PTOS);
    app_canTx_VC_CellOvervoltageLatch_set(safetyC.COVL);
    app_canTx_VC_OvercurrentDischargeLatch_set(safetyC.OCDL);
    app_canTx_VC_ShortCircuitDischargeLatch_set(safetyC.SCDL);
    app_canTx_VC_OvercurrentDischarge3_set(safetyC.OCD3);

    // battery and pack voltage
    app_canTx_VC_StackVoltage_set(io_lowVoltageBattery_get_voltage(CELL0_VOLTAGE_COMMAND));
    app_canTx_VC_StackVoltage_set(io_lowVoltageBattery_get_voltage(CELL1_VOLTAGE_COMMAND));
    app_canTx_VC_StackVoltage_set(io_lowVoltageBattery_get_voltage(CELL2_VOLTAGE_COMMAND));
    app_canTx_VC_StackVoltage_set(io_lowVoltageBattery_get_voltage(CELL3_VOLTAGE_COMMAND));
    app_canTx_VC_StackVoltage_set(io_lowVoltageBattery_get_voltage(STACK_VOLTAGE_COMMAND));
}