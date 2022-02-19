#include "states/App_AllStates.h"
#include "App_GateDrive.h"
#include "App_PowerStage.h"
#include "App_Motor.h"
#include "control/App_ControlLoop.h"
#include "App_SharedMacros.h"
#include "main.h"
#include "Io_AdcDac.h"

static float rotor_speed_request;
static uint8_t mode_request;
static float mod_index_request;
static float ph_cur_peak_request;
static float fund_freq_request;

static const struct PhaseValues *phase_currents;
static const struct StgapFaults *stgap_faults;

static const struct ControllerValues * iq_controller;
static const struct ControllerValues * id_controller;
static const struct ControllerValues * speed_controller;

void App_AllStatesRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct PowerStage *power_stage = App_InvWorld_GetPowerStage(world);
    struct GateDrive *gate_drive = App_InvWorld_GetGateDrive(world);

    App_CanTx_SetPeriodicSignal_CUR_SNS_OFFSET_OUT_OF_RANGE(can_tx_interface, 0);
    App_CanTx_SetPeriodicSignal_PWRSTG_TEMP(can_tx_interface, App_PowerStage_GetTemperature(power_stage));
    App_CanTx_SetPeriodicSignal_MOTOR_TEMP(can_tx_interface, App_Motor_GetTemperature());
    App_CanTx_SetPeriodicSignal_PHA_HI_DIAG(can_tx_interface, App_GateDrive_GetPhaLoDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHA_LO_DIAG(can_tx_interface, App_GateDrive_GetPhaHiDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHB_HI_DIAG(can_tx_interface, App_GateDrive_GetPhbLoDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHB_LO_DIAG(can_tx_interface, App_GateDrive_GetPhbHiDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHC_HI_DIAG(can_tx_interface, App_GateDrive_GetPhcLoDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHC_LO_DIAG(can_tx_interface, App_GateDrive_GetPhcHiDiag(gate_drive));
    App_CanTx_SetPeriodicSignal_PHA_OC_ALARM(can_tx_interface, App_PowerStage_GetPhaOCFault(power_stage));
    App_CanTx_SetPeriodicSignal_PHB_OC_ALARM(can_tx_interface, App_PowerStage_GetPhbOCFault(power_stage));
    App_CanTx_SetPeriodicSignal_PHC_OC_ALARM(can_tx_interface, App_PowerStage_GetPhcOCFault(power_stage));
    App_CanTx_SetPeriodicSignal_PWRSTG_OT_ALARM(can_tx_interface, App_PowerStage_GetPowerStageOTFault(power_stage));
    App_CanTx_SetPeriodicSignal_MOTOR_OT_ALARM(can_tx_interface, App_Motor_GetOTAlarm());
    App_CanTx_SetPeriodicSignal_GPIOD_1(can_tx_interface, HAL_GPIO_ReadPin(GPIOD_1_GPIO_Port, GPIOD_1_Pin));
    App_CanTx_SetPeriodicSignal_GPIOD_2(can_tx_interface, HAL_GPIO_ReadPin(GPIOD_2_GPIO_Port, GPIOD_2_Pin));
    App_CanTx_SetPeriodicSignal_DAC_OUT1_VOLTAGE(can_tx_interface, Io_AdcDac_Dac1GetVoltage());
    App_CanTx_SetPeriodicSignal_DAC_OUT1_CURRENT_LIM(can_tx_interface, App_PowerStage_GetPosCurrentLimit());
    App_CanTx_SetPeriodicSignal_DAC_OUT2_VOLTAGE(can_tx_interface, Io_AdcDac_Dac2GetVoltage());
    App_CanTx_SetPeriodicSignal_DAC_OUT2_CURRENT_LIM(can_tx_interface, App_PowerStage_GetNegCurrentLimit());

    //stgap message setters here
}

void App_AllStatesRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct InvCanRxInterface *can_rx_interface = App_InvWorld_GetCanRx(world);
    struct PowerStage *power_stage = App_InvWorld_GetPowerStage(world);

    App_ControlLoop_GetIqControllerValues(iq_controller);
    App_ControlLoop_GetIdControllerValues(id_controller);
    App_ControlLoop_GetSpeedControllerValues(speed_controller);
    App_PowerStage_GetPhaseCurrents(power_stage, phase_currents);

    App_CanTx_SetPeriodicSignal_HEARTBEAT(can_tx_interface, 1);
    App_CanTx_SetPeriodicSignal_PID_IQ_INTEGRAL(can_tx_interface, iq_controller->integral_sum);
    App_CanTx_SetPeriodicSignal_PID_ID_INTEGRAL(can_tx_interface, id_controller->integral_sum);
    App_CanTx_SetPeriodicSignal_PID_SPEED_INTEGRAL(can_tx_interface, speed_controller->integral_sum);
    App_CanTx_SetPeriodicSignal_MOD_INDEX(can_tx_interface, App_ControlLoop_GetModIndex());
    App_CanTx_SetPeriodicSignal_BUS_VOLTAGE(can_tx_interface, App_PowerStage_GetBusVoltage(power_stage));
    App_CanTx_SetPeriodicSignal_PHA_CUR_DC(can_tx_interface, phase_currents->a);
    App_CanTx_SetPeriodicSignal_PHB_CUR_DC(can_tx_interface, phase_currents->b);
    App_CanTx_SetPeriodicSignal_PHC_CUR_DC(can_tx_interface, phase_currents->c);
    App_CanTx_SetPeriodicSignal_MOTOR_POS(can_tx_interface, App_Motor_GetPosition());
    App_CanTx_SetPeriodicSignal_MOTOR_SPEED(can_tx_interface, App_ControlLoop_GetRotorSpeed());
    App_CanTx_SetPeriodicSignal_MODE(can_tx_interface, App_ControlLoop_GetMode());
    App_CanTx_SetPeriodicSignal_GPIOA(can_tx_interface, Io_AdcDac_GetGpioVal());
    App_CanTx_SetPeriodicSignal_PID_IQ_OUTPUT(can_tx_interface, iq_controller->output);
    App_CanTx_SetPeriodicSignal_PID_ID_OUTPUT(can_tx_interface, id_controller->output);
    App_CanTx_SetPeriodicSignal_PID_SPEED_OUTPUT(can_tx_interface, speed_controller->output);

    rotor_speed_request = App_CanRx_INV_ROTOR_SPEED_REQ_GetSignal_ROTOR_SPEED_REQ(can_rx_interface);
    mode_request = App_CanRx_INV_MODE_REQ_GetSignal_MODE_REQ(can_rx_interface);
    mod_index_request = App_CanRx_INV_MOD_INDEX_REQ_GetSignal_MOD_INDEX_REQ(can_rx_interface);
    ph_cur_peak_request = App_CanRx_INV_PH_CUR_PEAK_REQ_GetSignal_PH_CUR_PEAK_REQ(can_rx_interface);
    fund_freq_request = App_CanRx_INV_FUND_FREQ_REQ_GetSignal_FUND_FREQ_REQ(can_rx_interface);

    App_CanTx_SetPeriodicSignal_










}
