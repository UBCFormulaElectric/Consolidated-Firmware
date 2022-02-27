#include "App_Faults.h"

// Check to see if the inverter has a non-critical fault set on the CAN table
bool App_Faults_FaultedNonCritical(
    const struct InvCanTxInterface *const can_tx_interface)
{
    if (App_CanTx_GetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ(
            can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK100_HZ(
            can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_KHZ(
            can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX(
            can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX(
            can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_WATCHDOG_TIMEOUT(can_tx_interface))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Check to see if the inverter has a critical fault flag set on the CAN table
bool App_Faults_FaultedMotorShutdown(
    const struct InvCanTxInterface *const can_tx_interface)
{
    if (App_CanTx_GetPeriodicSignal_PHA_HI_DIAG(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_PHA_LO_DIAG(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_PHB_HI_DIAG(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_PHB_LO_DIAG(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_PHC_HI_DIAG(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_PHC_LO_DIAG(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_PHA_OC_ALARM(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_PHB_OC_ALARM(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_PHC_OC_ALARM(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_PWRSTG_OT_ALARM(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_MOTOR_OT_ALARM(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_STGAP_STATUS_FAULT(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_STGAP_SHORT_TEST(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_ROTOR_NO_RESPONSE(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_ROTOR_CRC_CHECK(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_CUR_SNS_OFFSET(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_DAC_OC_THRESHOLD(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_LL_PERIPH_FAULT(can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_MC_TORQUE_REQUEST_IMPLAUSIBLE(
            can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_MC_ROTOR_POSITION_IMPLAUSIBLE(
            can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_MC_PHC_CUR_CALC_IMPLAUSIBLE(
            can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_MC_FUND_FREQ_REQ_IMPLAUSIBLE(
            can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_MC_ID_CONTROLLER_STABILITY(
            can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_MC_IQ_CONTROLLER_STABILITY(
            can_tx_interface) ||
        App_CanTx_GetPeriodicSignal_MC_SPEED_CONTROLLER_STABILITY(
            can_tx_interface))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
