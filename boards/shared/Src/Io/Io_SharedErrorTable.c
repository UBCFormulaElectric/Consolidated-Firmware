#include "App_SharedErrorTable.h"
#include "App_CanMsgs.h"
#include "Io_SharedErrorTable.h"

static void Io_ProcessFsmCriticalErrorMsg(
    struct ErrorTable *          error_table,
    struct CanMsgs_fsm_errors_t *data)
{
    // TODO: Fill this out once I'm sure about other parts of the design
    (void)error_table;
    (void)data;
}

void Io_SharedErrorTable_SetErrorsBasedOnCanMsg(
    struct ErrorTable *error_table,
    struct CanMsg *    can_msg)
{
    // TODO: Add errors for other boards once I'm sure about other parts of the
    // design
    switch (can_msg->std_id)
    {
        case (CANMSGS_FSM_ERRORS_FRAME_ID):
        {
            struct CanMsgs_fsm_errors_t data;

            App_CanMsgs_fsm_errors_unpack(
                &data, can_msg->data, CANMSGS_FSM_ERRORS_LENGTH);

            Io_ProcessFsmCriticalErrorMsg(error_table, &data);
        }
        break;
    }
}
