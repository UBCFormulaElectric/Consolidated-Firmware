#include "states/App_AllStates.h"
#include "App_GateDrive.h"
#include "App_PowerStage.h"
#include "App_Motor.h"
#include "App_Faults.h"
#include "states/App_FaultState.h"
#include "control/App_ControlLoop.h"
#include "App_SharedMacros.h"
#include "main.h"
#include "Io_AdcDac.h"

static float   rotor_speed_request;
static uint8_t mode_request;
static float   mod_index_request;
static float   ph_cur_peak_request;
static float   fund_freq_request;
static uint8_t state_request;

static struct DqsValues dqs_ref_currents;
static struct DqsValues dqs_actual_currents;
static struct PhaseValues phase_currents;
static struct PhaseValues phase_cur_offsets;
static struct StgapFaults stgap_faults;

static struct ControllerValues iq_controller;
static struct ControllerValues id_controller;
static struct ControllerValues speed_controller;

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct PowerStage *       power_stage = App_InvWorld_GetPowerStage(world);
    struct GateDrive *        gate_drive  = App_InvWorld_GetGateDrive(world);

    App_PowerStage_GetPhaseCurOffsets(&phase_cur_offsets);

    App_CanTx_SetPeriodicSignal_PWRSTG_TEMP(
        can_tx_interface, App_PowerStage_GetTemperature(power_stage));
    App_CanTx_SetPeriodicSignal_MOTOR_TEMP(
        can_tx_interface, App_Motor_GetTemperature());
    App_CanTx_SetPeriodicSignal_PHA_HI_DIAG(
        can_tx_interface, App_GateDrive_GetPhaLoDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHA_LO_DIAG(
        can_tx_interface, App_GateDrive_GetPhaHiDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHB_HI_DIAG(
        can_tx_interface, App_GateDrive_GetPhbLoDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHB_LO_DIAG(
        can_tx_interface, App_GateDrive_GetPhbHiDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHC_HI_DIAG(
        can_tx_interface, App_GateDrive_GetPhcLoDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHC_LO_DIAG(
        can_tx_interface, App_GateDrive_GetPhcHiDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHA_OC_ALARM(
        can_tx_interface, App_PowerStage_GetPhaOCFault(power_stage));
    App_CanTx_SetPeriodicSignal_PHB_OC_ALARM(
        can_tx_interface, App_PowerStage_GetPhbOCFault(power_stage));
    App_CanTx_SetPeriodicSignal_PHC_OC_ALARM(
        can_tx_interface, App_PowerStage_GetPhcOCFault(power_stage));
    App_CanTx_SetPeriodicSignal_PWRSTG_OT_ALARM(
        can_tx_interface, App_PowerStage_GetOTFault(power_stage));
    App_CanTx_SetPeriodicSignal_MOTOR_OT_ALARM(
        can_tx_interface, App_Motor_GetOTAlarm());
    App_CanTx_SetPeriodicSignal_PHA_CUR_OFFSET(can_tx_interface, phase_cur_offsets.a);
    App_CanTx_SetPeriodicSignal_PHB_CUR_OFFSET(can_tx_interface, phase_cur_offsets.b);
    App_CanTx_SetPeriodicSignal_PHC_CUR_OFFSET(can_tx_interface, phase_cur_offsets.c);
    App_CanTx_SetPeriodicSignal_GPIOD_1(
        can_tx_interface, HAL_GPIO_ReadPin(GPIOD_1_GPIO_Port, GPIOD_1_Pin));
    App_CanTx_SetPeriodicSignal_GPIOD_2(
        can_tx_interface, HAL_GPIO_ReadPin(GPIOD_2_GPIO_Port, GPIOD_2_Pin));
    App_CanTx_SetPeriodicSignal_DAC_OUT1_VOLTAGE(
        can_tx_interface, Io_AdcDac_Dac1GetVoltage());
    App_CanTx_SetPeriodicSignal_DAC_OUT1_CURRENT_LIM(
        can_tx_interface, App_PowerStage_GetPosCurrentLimit());
    App_CanTx_SetPeriodicSignal_DAC_OUT2_VOLTAGE(
        can_tx_interface, Io_AdcDac_Dac2GetVoltage());
    App_CanTx_SetPeriodicSignal_DAC_OUT2_CURRENT_LIM(
        can_tx_interface, App_PowerStage_GetNegCurrentLimit());

    // Only send stgap fault messages after STGAPs are configured, i.e.
    // DRIVE/STANDBY states only
    if (App_CanTx_GetPeriodicMsgPointer_INV_STATE_MACHINE(can_tx_interface)
            ->state == (CANMSGS_INV_STATE_MACHINE_STATE_DRIVE_CHOICE |
                        CANMSGS_INV_STATE_MACHINE_STATE_STANDBY_CHOICE))
    {
        if (App_GateDrive_IsFaulted())
        {
            App_CanTx_SetPeriodicSignal_STGAP_STATUS_FAULT(can_tx_interface, 1);

            App_GateDrive_GetFaults(gate_drive, &stgap_faults);
            App_CanTx_SetPeriodicSignal_A_LO_THERM_WARN(
                can_tx_interface, stgap_faults.twn[0]);
            App_CanTx_SetPeriodicSignal_A_LO_THERM_SHDN(
                can_tx_interface, stgap_faults.tsd[0]);
            App_CanTx_SetPeriodicSignal_A_LO_UVLOL(
                can_tx_interface, stgap_faults.uvlol[0]);
            App_CanTx_SetPeriodicSignal_A_LO_UVLOH(
                can_tx_interface, stgap_faults.uvloh[0]);
            App_CanTx_SetPeriodicSignal_A_LO_SENSE(
                can_tx_interface, stgap_faults.sense[0]);
            App_CanTx_SetPeriodicSignal_A_LO_DESAT(
                can_tx_interface, stgap_faults.desat[0]);
            App_CanTx_SetPeriodicSignal_A_LO_OVLOL(
                can_tx_interface, stgap_faults.ovlol[0]);
            App_CanTx_SetPeriodicSignal_A_LO_OVLOH(
                can_tx_interface, stgap_faults.ovloh[0]);
            App_CanTx_SetPeriodicSignal_A_LO_ASC(
                can_tx_interface, stgap_faults.asc[0]);
            App_CanTx_SetPeriodicSignal_A_LO_REGERR_ISO(
                can_tx_interface, stgap_faults.regerrr[0]);
            App_CanTx_SetPeriodicSignal_A_LO_UVLOD(
                can_tx_interface, stgap_faults.uvlod[0]);
            App_CanTx_SetPeriodicSignal_A_LO_OVLOD(
                can_tx_interface, stgap_faults.ovlod[0]);
            App_CanTx_SetPeriodicSignal_A_LO_REGERR_LV(
                can_tx_interface, stgap_faults.reg_errl[0]);
            App_CanTx_SetPeriodicSignal_A_LO_SPI_ERR(
                can_tx_interface, stgap_faults.spi_err[0]);
            App_CanTx_SetPeriodicSignal_A_LO_DEADTIME_ERR(
                can_tx_interface, stgap_faults.dt_err[0]);
            App_CanTx_SetPeriodicSignal_A_HI_THERM_WARN(
                can_tx_interface, stgap_faults.twn[1]);
            App_CanTx_SetPeriodicSignal_A_HI_THERM_SHDN(
                can_tx_interface, stgap_faults.tsd[1]);
            App_CanTx_SetPeriodicSignal_A_HI_UVLOL(
                can_tx_interface, stgap_faults.uvlol[1]);
            App_CanTx_SetPeriodicSignal_A_HI_UVLOH(
                can_tx_interface, stgap_faults.uvloh[1]);
            App_CanTx_SetPeriodicSignal_A_HI_SENSE(
                can_tx_interface, stgap_faults.sense[1]);
            App_CanTx_SetPeriodicSignal_A_HI_DESAT(
                can_tx_interface, stgap_faults.desat[1]);
            App_CanTx_SetPeriodicSignal_A_HI_OVLOL(
                can_tx_interface, stgap_faults.ovlol[1]);
            App_CanTx_SetPeriodicSignal_A_HI_OVLOH(
                can_tx_interface, stgap_faults.ovloh[1]);
            App_CanTx_SetPeriodicSignal_A_HI_ASC(
                can_tx_interface, stgap_faults.asc[1]);
            App_CanTx_SetPeriodicSignal_A_HI_REGERR_ISO(
                can_tx_interface, stgap_faults.regerrr[1]);
            App_CanTx_SetPeriodicSignal_A_HI_UVLOD(
                can_tx_interface, stgap_faults.uvlod[1]);
            App_CanTx_SetPeriodicSignal_A_HI_OVLOD(
                can_tx_interface, stgap_faults.ovlod[1]);
            App_CanTx_SetPeriodicSignal_A_HI_REGERR_LV(
                can_tx_interface, stgap_faults.reg_errl[1]);
            App_CanTx_SetPeriodicSignal_A_HI_SPI_ERR(
                can_tx_interface, stgap_faults.spi_err[1]);
            App_CanTx_SetPeriodicSignal_A_HI_DEADTIME_ERR(
                can_tx_interface, stgap_faults.dt_err[1]);

            App_CanTx_SetPeriodicSignal_B_LO_THERM_WARN(
                can_tx_interface, stgap_faults.twn[2]);
            App_CanTx_SetPeriodicSignal_B_LO_THERM_SHDN(
                can_tx_interface, stgap_faults.tsd[2]);
            App_CanTx_SetPeriodicSignal_B_LO_UVLOL(
                can_tx_interface, stgap_faults.uvlol[2]);
            App_CanTx_SetPeriodicSignal_B_LO_UVLOH(
                can_tx_interface, stgap_faults.uvloh[2]);
            App_CanTx_SetPeriodicSignal_B_LO_SENSE(
                can_tx_interface, stgap_faults.sense[2]);
            App_CanTx_SetPeriodicSignal_B_LO_DESAT(
                can_tx_interface, stgap_faults.desat[2]);
            App_CanTx_SetPeriodicSignal_B_LO_OVLOL(
                can_tx_interface, stgap_faults.ovlol[2]);
            App_CanTx_SetPeriodicSignal_B_LO_OVLOH(
                can_tx_interface, stgap_faults.ovloh[2]);
            App_CanTx_SetPeriodicSignal_B_LO_ASC(
                can_tx_interface, stgap_faults.asc[2]);
            App_CanTx_SetPeriodicSignal_B_LO_REGERR_ISO(
                can_tx_interface, stgap_faults.regerrr[2]);
            App_CanTx_SetPeriodicSignal_B_LO_UVLOD(
                can_tx_interface, stgap_faults.uvlod[2]);
            App_CanTx_SetPeriodicSignal_B_LO_OVLOD(
                can_tx_interface, stgap_faults.ovlod[2]);
            App_CanTx_SetPeriodicSignal_B_LO_REGERR_LV(
                can_tx_interface, stgap_faults.reg_errl[2]);
            App_CanTx_SetPeriodicSignal_B_LO_SPI_ERR(
                can_tx_interface, stgap_faults.spi_err[2]);
            App_CanTx_SetPeriodicSignal_B_LO_DEADTIME_ERR(
                can_tx_interface, stgap_faults.dt_err[2]);
            App_CanTx_SetPeriodicSignal_B_HI_THERM_WARN(
                can_tx_interface, stgap_faults.twn[3]);
            App_CanTx_SetPeriodicSignal_B_HI_THERM_SHDN(
                can_tx_interface, stgap_faults.tsd[3]);
            App_CanTx_SetPeriodicSignal_B_HI_UVLOL(
                can_tx_interface, stgap_faults.uvlol[3]);
            App_CanTx_SetPeriodicSignal_B_HI_UVLOH(
                can_tx_interface, stgap_faults.uvloh[3]);
            App_CanTx_SetPeriodicSignal_B_HI_SENSE(
                can_tx_interface, stgap_faults.sense[3]);
            App_CanTx_SetPeriodicSignal_B_HI_DESAT(
                can_tx_interface, stgap_faults.desat[3]);
            App_CanTx_SetPeriodicSignal_B_HI_OVLOL(
                can_tx_interface, stgap_faults.ovlol[3]);
            App_CanTx_SetPeriodicSignal_B_HI_OVLOH(
                can_tx_interface, stgap_faults.ovloh[3]);
            App_CanTx_SetPeriodicSignal_B_HI_ASC(
                can_tx_interface, stgap_faults.asc[3]);
            App_CanTx_SetPeriodicSignal_B_HI_REGERR_ISO(
                can_tx_interface, stgap_faults.regerrr[3]);
            App_CanTx_SetPeriodicSignal_B_HI_UVLOD(
                can_tx_interface, stgap_faults.uvlod[3]);
            App_CanTx_SetPeriodicSignal_B_HI_OVLOD(
                can_tx_interface, stgap_faults.ovlod[3]);
            App_CanTx_SetPeriodicSignal_B_HI_REGERR_LV(
                can_tx_interface, stgap_faults.reg_errl[3]);
            App_CanTx_SetPeriodicSignal_B_HI_SPI_ERR(
                can_tx_interface, stgap_faults.spi_err[3]);
            App_CanTx_SetPeriodicSignal_B_HI_DEADTIME_ERR(
                can_tx_interface, stgap_faults.dt_err[3]);

            App_CanTx_SetPeriodicSignal_C_LO_THERM_WARN(
                can_tx_interface, stgap_faults.twn[4]);
            App_CanTx_SetPeriodicSignal_C_LO_THERM_SHDN(
                can_tx_interface, stgap_faults.tsd[4]);
            App_CanTx_SetPeriodicSignal_C_LO_UVLOL(
                can_tx_interface, stgap_faults.uvlol[4]);
            App_CanTx_SetPeriodicSignal_C_LO_UVLOH(
                can_tx_interface, stgap_faults.uvloh[4]);
            App_CanTx_SetPeriodicSignal_C_LO_SENSE(
                can_tx_interface, stgap_faults.sense[4]);
            App_CanTx_SetPeriodicSignal_C_LO_DESAT(
                can_tx_interface, stgap_faults.desat[4]);
            App_CanTx_SetPeriodicSignal_C_LO_OVLOL(
                can_tx_interface, stgap_faults.ovlol[4]);
            App_CanTx_SetPeriodicSignal_C_LO_OVLOH(
                can_tx_interface, stgap_faults.ovloh[4]);
            App_CanTx_SetPeriodicSignal_C_LO_ASC(
                can_tx_interface, stgap_faults.asc[4]);
            App_CanTx_SetPeriodicSignal_C_LO_REGERR_ISO(
                can_tx_interface, stgap_faults.regerrr[4]);
            App_CanTx_SetPeriodicSignal_C_LO_UVLOD(
                can_tx_interface, stgap_faults.uvlod[4]);
            App_CanTx_SetPeriodicSignal_C_LO_OVLOD(
                can_tx_interface, stgap_faults.ovlod[4]);
            App_CanTx_SetPeriodicSignal_C_LO_REGERR_LV(
                can_tx_interface, stgap_faults.reg_errl[4]);
            App_CanTx_SetPeriodicSignal_C_LO_SPI_ERR(
                can_tx_interface, stgap_faults.spi_err[4]);
            App_CanTx_SetPeriodicSignal_C_LO_DEADTIME_ERR(
                can_tx_interface, stgap_faults.dt_err[4]);
            App_CanTx_SetPeriodicSignal_C_HI_THERM_WARN(
                can_tx_interface, stgap_faults.twn[5]);
            App_CanTx_SetPeriodicSignal_C_HI_THERM_SHDN(
                can_tx_interface, stgap_faults.tsd[5]);
            App_CanTx_SetPeriodicSignal_C_HI_UVLOL(
                can_tx_interface, stgap_faults.uvlol[5]);
            App_CanTx_SetPeriodicSignal_C_HI_UVLOH(
                can_tx_interface, stgap_faults.uvloh[5]);
            App_CanTx_SetPeriodicSignal_C_HI_SENSE(
                can_tx_interface, stgap_faults.sense[5]);
            App_CanTx_SetPeriodicSignal_C_HI_DESAT(
                can_tx_interface, stgap_faults.desat[5]);
            App_CanTx_SetPeriodicSignal_C_HI_OVLOL(
                can_tx_interface, stgap_faults.ovlol[5]);
            App_CanTx_SetPeriodicSignal_C_HI_OVLOH(
                can_tx_interface, stgap_faults.ovloh[5]);
            App_CanTx_SetPeriodicSignal_C_HI_ASC(
                can_tx_interface, stgap_faults.asc[5]);
            App_CanTx_SetPeriodicSignal_C_HI_REGERR_ISO(
                can_tx_interface, stgap_faults.regerrr[5]);
            App_CanTx_SetPeriodicSignal_C_HI_UVLOD(
                can_tx_interface, stgap_faults.uvlod[5]);
            App_CanTx_SetPeriodicSignal_C_HI_REGERR_LV(
                can_tx_interface, stgap_faults.reg_errl[5]);
            App_CanTx_SetPeriodicSignal_C_HI_SPI_ERR(
                can_tx_interface, stgap_faults.spi_err[5]);
            App_CanTx_SetPeriodicSignal_C_HI_DEADTIME_ERR(
                can_tx_interface, stgap_faults.dt_err[5]);
        }
        else
        {
            App_CanTx_SetPeriodicSignal_STGAP_STATUS_FAULT(can_tx_interface, 0);
        }
    }
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct InvCanRxInterface *can_rx_interface = App_InvWorld_GetCanRx(world);
    struct PowerStage *       power_stage = App_InvWorld_GetPowerStage(world);
    struct GateDrive *        gate_drive  = App_InvWorld_GetGateDrive(world);

    // Check for faults at 100Hz, all states
    if (App_Faults_FaultedMotorShutdown(can_tx_interface) && App_CanTx_GetPeriodicSignal_STATE(can_tx_interface) != CANMSGS_INV_STATE_MACHINE_STATE_INIT_CHOICE && App_CanTx_GetPeriodicSignal_STATE(can_tx_interface) != CANMSGS_INV_STATE_MACHINE_STATE_FAULT_CHOICE)
    {
        App_GateDrive_Shutdown(gate_drive);
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
    else
    {
        App_ControlLoop_GetIqControllerValues(&iq_controller);
        App_ControlLoop_GetIdControllerValues(&id_controller);
        App_ControlLoop_GetSpeedControllerValues(&speed_controller);
        App_PowerStage_GetPhaseCurrents(power_stage, &phase_currents);
        App_ControlLoop_GetDqsRefCurrents(&dqs_ref_currents);
        App_ControlLoop_GetDqsActualCurrents(&dqs_actual_currents);

        App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx_interface, 1);
        App_CanTx_SetPeriodicSignal_PID_IQ_PROP(
                can_tx_interface, iq_controller.output-iq_controller.integral_sum);
        App_CanTx_SetPeriodicSignal_PID_ID_PROP(
                can_tx_interface, id_controller.output-id_controller.integral_sum);
        App_CanTx_SetPeriodicSignal_PID_SPEED_PROP(
                can_tx_interface, speed_controller.output-speed_controller.integral_sum);
        App_CanTx_SetPeriodicSignal_PID_IQ_INTEGRAL(
            can_tx_interface, iq_controller.integral_sum);
        App_CanTx_SetPeriodicSignal_PID_ID_INTEGRAL(
            can_tx_interface, id_controller.integral_sum);
        App_CanTx_SetPeriodicSignal_PID_SPEED_INTEGRAL(
            can_tx_interface, speed_controller.integral_sum);
        App_CanTx_SetPeriodicSignal_PID_IQ_OUTPUT(
            can_tx_interface, iq_controller.output);
        App_CanTx_SetPeriodicSignal_PID_ID_OUTPUT(
            can_tx_interface, id_controller.output);
        App_CanTx_SetPeriodicSignal_PID_SPEED_OUTPUT(
            can_tx_interface, speed_controller.output);
        App_CanTx_SetPeriodicSignal_MOD_INDEX(
            can_tx_interface, App_ControlLoop_GetModIndex()*100.0f);
        App_CanTx_SetPeriodicSignal_BUS_VOLTAGE(
            can_tx_interface, App_PowerStage_GetBusVoltage(power_stage));
        App_CanTx_SetPeriodicSignal_PHA_CUR_DC(
            can_tx_interface, phase_currents.a);
        App_CanTx_SetPeriodicSignal_PHB_CUR_DC(
            can_tx_interface, phase_currents.b);
        App_CanTx_SetPeriodicSignal_PHC_CUR_DC(
            can_tx_interface, phase_currents.c);
        App_CanTx_SetPeriodicSignal_ROTOR_POSITION(
            can_tx_interface, App_ControlLoop_GetRotorPosition());
        App_CanTx_SetPeriodicSignal_ROTOR_SPEED(
            can_tx_interface, App_ControlLoop_GetRotorSpeed());
        App_CanTx_SetPeriodicSignal_MODE(
            can_tx_interface, App_ControlLoop_GetMode());
        App_CanTx_SetPeriodicSignal_GPIOA_VOLTAGE(
            can_tx_interface, Io_AdcDac_GetGpioVal());
        App_CanTx_SetPeriodicSignal_ID_REF(can_tx_interface, dqs_ref_currents.d);
        App_CanTx_SetPeriodicSignal_IQ_REF(can_tx_interface, dqs_ref_currents.q);
        App_CanTx_SetPeriodicSignal_IS_REF(can_tx_interface, dqs_ref_currents.s);
        App_CanTx_SetPeriodicSignal_ID(can_tx_interface, dqs_actual_currents.d);
        App_CanTx_SetPeriodicSignal_IQ(can_tx_interface, dqs_actual_currents.q);
        App_CanTx_SetPeriodicSignal_IS(can_tx_interface, dqs_actual_currents.s);
        App_CanTx_SetPeriodicSignal_VPHASE_LN(can_tx_interface, App_ControlLoop_GetLineToNeutralVoltage());

        rotor_speed_request =
            App_CanRx_INV_ROTOR_SPEED_REQ_GetSignal_ROTOR_SPEED_REQ(
                can_rx_interface);
        mode_request =
            App_CanRx_INV_MODE_REQ_GetSignal_MODE_REQ(can_rx_interface);
        mod_index_request = App_CanRx_INV_MOD_INDEX_REQ_GetSignal_MOD_INDEX_REQ(
            can_rx_interface);
        ph_cur_peak_request =
            App_CanRx_INV_PH_CUR_PEAK_REQ_GetSignal_PH_CUR_PEAK_REQ(
                can_rx_interface);
        fund_freq_request = App_CanRx_INV_FUND_FREQ_REQ_GetSignal_FUND_FREQ_REQ(
            can_rx_interface);
        state_request = App_CanRx_INV_STATE_REQ_GetSignal_STATE_REQ(can_rx_interface);

        App_CanTx_SetPeriodicSignal_COMMAND(can_tx_interface, state_request);

    }
}
