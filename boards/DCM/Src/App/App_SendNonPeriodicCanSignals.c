#include "App_SendNonPeriodicCanSignals.h"

#define CLEAR_INV_FAULT_PARAM_ADDRESS (20U)
#define WRITE_CMD (1U)

void App_SendNonPeriodicCanSignals_ClearInverterFaults(
    const struct DcmWorld *world)
{
    struct DcmCanTxInterface *can_tx = App_DcmWorld_GetCanTx(world);
    struct DcmCanRxInterface *can_rx = App_DcmWorld_GetCanRx(world);

    if (App_CanRx_CLEAR_INV_FAULT_CMD_GetSignal_CLEAR_LEFT_FAULT(can_rx) ==
        CANMSGS_CLEAR_INV_FAULT_CMD_CLEAR_LEFT_FAULT_YES_CHOICE)
    {
        const struct CanMsgs_dcm_invl_read_write_param_command_t
            invl_cmd_msg = { .d1_parameter_address_command =
                                 CLEAR_INV_FAULT_PARAM_ADDRESS,
                             .d2_read_write_command = WRITE_CMD,
                             .d3_data_command       = 0 };

        App_CanTx_SendNonPeriodicMsg_DCM_INVL_READ_WRITE_PARAM_COMMAND(
            can_tx, &invl_cmd_msg);

        // Send the clear fault command to the left inverter once
        App_CanRx_CLEAR_INV_FAULT_CMD_SetSignal_CLEAR_LEFT_FAULT(
            can_rx, CANMSGS_CLEAR_INV_FAULT_CMD_CLEAR_LEFT_FAULT_NO_CHOICE);
    }

    if (App_CanRx_CLEAR_INV_FAULT_CMD_GetSignal_CLEAR_RIGHT_FAULT(can_rx) ==
        CANMSGS_CLEAR_INV_FAULT_CMD_CLEAR_RIGHT_FAULT_YES_CHOICE)
    {
        const struct CanMsgs_dcm_invr_read_write_param_command_t
            invr_cmd_msg = { .d1_parameter_address_command =
                                 CLEAR_INV_FAULT_PARAM_ADDRESS,
                             .d2_read_write_command = WRITE_CMD,
                             .d3_data_command       = 0 };

        App_CanTx_SendNonPeriodicMsg_DCM_INVR_READ_WRITE_PARAM_COMMAND(
            can_tx, &invr_cmd_msg);

        // Send the clear fault command to the right inverter once
        App_CanRx_CLEAR_INV_FAULT_CMD_SetSignal_CLEAR_RIGHT_FAULT(
            can_rx, CANMSGS_CLEAR_INV_FAULT_CMD_CLEAR_RIGHT_FAULT_NO_CHOICE);
    }
}
