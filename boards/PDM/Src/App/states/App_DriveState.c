
#include "states/App_AllStates.h"
#include "states/App_DriveState.h"
#include "App_SharedMacros.h"
#include "states/App_FaultState.h"

#define NUM_CYCLES_TO_SETTLE 0U

void Efuse_EnableChannelsPcmRunning(
    struct Efuse *efuse1,
    struct Efuse *efuse2,
    struct Efuse *efuse3,
    struct Efuse *efuse4)
{
    App_Efuse_EnableChannel0(efuse1, true);
    App_Efuse_EnableChannel1(efuse1, true);
    App_Efuse_EnableChannel0(efuse2, true);
    App_Efuse_EnableChannel1(efuse2, false);
    App_Efuse_EnableChannel0(efuse3, true);
    App_Efuse_EnableChannel1(efuse3, true);
    App_Efuse_EnableChannel0(efuse4, true);
    App_Efuse_EnableChannel1(efuse4, true);
}

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct PdmWorld *world  = App_SharedStateMachine_GetWorld(state_machine);
    struct Efuse *   efuse1 = App_PdmWorld_GetEfuse1(world);
    struct Efuse *   efuse2 = App_PdmWorld_GetEfuse2(world);
    struct Efuse *   efuse3 = App_PdmWorld_GetEfuse3(world);
    struct Efuse *   efuse4 = App_PdmWorld_GetEfuse4(world);

    Efuse_EnableChannelsPcmRunning(efuse1, efuse2, efuse3, efuse4);
    App_CanTx_PDM_Vitals_State_Set(PDM_STATE_DRIVE);
}

void DriveErrorsWarningsCANTX(
    struct Efuse *            efuse1,
    struct Efuse *            efuse2,
    struct Efuse *            efuse3,
    struct Efuse *            efuse4,
    struct LowVoltageBattery *LVB)
{
    App_CanTx_PDM_EfuseFaultCheck_AIR_Set(App_Efuse_FaultCheckChannel0(efuse1));
    App_CanTx_PDM_EfuseFaultCheck_LVPWR_Set(App_Efuse_FaultCheckChannel1(efuse1));
    App_CanTx_PDM_EfuseFaultCheck_EMETER_Set(App_Efuse_FaultCheckChannel0(efuse2));
    App_CanTx_PDM_EfuseFaultCheck_AUX_Set(App_Efuse_FaultCheckChannel1(efuse2));
    App_CanTx_PDM_EfuseFaultCheck_LEFT_INVERTER_Set(App_Efuse_FaultCheckChannel0(efuse3));
    App_CanTx_PDM_EfuseFaultCheck_RIGHT_INVERTER_Set(App_Efuse_FaultCheckChannel1(efuse3));
    App_CanTx_PDM_EfuseFaultCheck_DRS_Set(App_Efuse_FaultCheckChannel0(efuse4));
    App_CanTx_PDM_EfuseFaultCheck_FAN_Set(App_Efuse_FaultCheckChannel1(efuse4));

    App_CanTx_PDM_LowVoltageBattery_Faults_Charge_Fault_Set(App_LowVoltageBattery_HasChargeFault(LVB));
    App_CanTx_PDM_LowVoltageBattery_Faults_Boost_Fault_Set(App_LowVoltageBattery_HasBoostControllerFault(LVB));
}

bool DriveFaultDetection(
    struct Efuse *            efuse1,
    struct Efuse *            efuse2,
    struct Efuse *            efuse3,
    struct Efuse *            efuse4,
    struct RailMonitoring *   rail_monitor,
    struct LowVoltageBattery *LVB)
{
    if (App_Efuse_FaultCheckChannel0(efuse1) == false &&                   // AIR
        App_Efuse_FaultCheckChannel0(efuse2) == false &&                   // EMETER
        App_Efuse_FaultCheckChannel0(efuse3) == false &&                   // LEFT INVERTER
        App_Efuse_FaultCheckChannel1(efuse3) == false &&                   // RIGHT INVERTER
        !App_RailMonitoring_VbatVoltageLowCheck(rail_monitor) &&           // VBAT LOW
        !App_RailMonitoring_24VAccumulatorVoltageLowCheck(rail_monitor) && // ACC LOW
        !App_RailMonitoring_22VAuxiliaryVoltageLowCheck(rail_monitor) &&   // AUX LOW
        !App_LowVoltageBattery_HasChargeFault(LVB) &&                      // CHARGE FAULT
        !App_LowVoltageBattery_HasBoostControllerFault(LVB))               // BOOST FAULT
        return false;                                                      // No Error
    return true;                                                           // Error
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct PdmWorld *         world        = App_SharedStateMachine_GetWorld(state_machine);
    struct RailMonitoring *   rail_monitor = App_PdmWorld_GetRailMonitoring(world);
    struct LowVoltageBattery *LVB          = App_PdmWorld_GetLowVoltageBattery(world);
    struct Efuse *            efuse1       = App_PdmWorld_GetEfuse1(world);
    struct Efuse *            efuse2       = App_PdmWorld_GetEfuse2(world);
    struct Efuse *            efuse3       = App_PdmWorld_GetEfuse3(world);
    struct Efuse *            efuse4       = App_PdmWorld_GetEfuse4(world);
    bool                      has_fault;

    DriveErrorsWarningsCANTX(efuse1, efuse2, efuse3, efuse4, LVB);
    has_fault = DriveFaultDetection(efuse1, efuse2, efuse3, efuse4, rail_monitor, LVB);

    if (has_fault)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = DriveStateRunOnEntry,
        .run_on_tick_1Hz   = DriveStateRunOnTick1Hz,
        .run_on_tick_100Hz = DriveStateRunOnTick100Hz,
        .run_on_exit       = DriveStateRunOnExit,
    };

    return &drive_state;
}
