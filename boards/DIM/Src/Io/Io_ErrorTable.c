#include <stdint.h>
#include <assert.h>
#include "App_CanRx.h"
#include "App_CanMsgs.h"
#include "Io_SharedCanMsg.h"
#include "Io_ErrorTable.h"

void Io_ErrorTable_UpdateRxTableWithMessage(
    struct ErrorTable *  error_table,
    const struct CanMsg *message)
{
    assert(error_table != NULL);
    assert(message != NULL);

    switch (message->std_id)
    {
        case CANMSGS_BMS_NON_CRITICAL_ERRORS_FRAME_ID:
        {
            struct CanMsgs_bms_non_critical_errors_t buffer;

            App_CanMsgs_bms_non_critical_errors_unpack(
                &buffer, &message->data[0],
                CANMSGS_BMS_NON_CRITICAL_ERRORS_LENGTH);

            if (buffer.stack_watermark_above_threshold_task1_hz != 0)
                App_ErrorTable_SetBmsStackWaterMarkAboveThresholdTask1Hz(
                    error_table);
            else
                App_ErrorTable_ClearBmsStackWaterMarkAboveThresholdTask1Hz(
                    error_table);
        }
        break;

        case CANMSGS_DCM_NON_CRITICAL_ERRORS_FRAME_ID:
        {
            struct CanMsgs_dcm_non_critical_errors_t buffer;

            App_CanMsgs_dcm_non_critical_errors_unpack(
                &buffer, &message->data[0],
                CANMSGS_DCM_NON_CRITICAL_ERRORS_LENGTH);

            if (buffer.stack_watermark_above_threshold_task1_hz != 0)
                App_ErrorTable_SetDcmStackWaterMarkAboveThresholdTask1Hz(
                    error_table);
            else
                App_ErrorTable_ClearDcmStackWaterMarkAboveThresholdTask1Hz(
                    error_table);
        }
        break;

        case CANMSGS_FSM_NON_CRITICAL_ERRORS_FRAME_ID:
        {
            struct CanMsgs_fsm_non_critical_errors_t buffer;

            App_CanMsgs_fsm_non_critical_errors_unpack(
                &buffer, &message->data[0],
                CANMSGS_FSM_NON_CRITICAL_ERRORS_LENGTH);

            if (buffer.stack_watermark_above_threshold_task1_hz != 0)
                App_ErrorTable_SetFsmStackWaterMarkAboveThresholdTask1Hz(
                    error_table);
            else
                App_ErrorTable_ClearFsmStackWaterMarkAboveThresholdTask1Hz(
                    error_table);
        }
        break;

        case CANMSGS_PDM_NON_CRITICAL_ERRORS_FRAME_ID:
        {
            struct CanMsgs_pdm_non_critical_errors_t buffer;

            App_CanMsgs_pdm_non_critical_errors_unpack(
                &buffer, &message->data[0],
                CANMSGS_PDM_NON_CRITICAL_ERRORS_LENGTH);

            if (buffer.stack_watermark_above_threshold_task1_hz != 0)
                App_ErrorTable_SetPdmStackWaterMarkAboveThresholdTask1Hz(
                    error_table);
            else
                App_ErrorTable_ClearPdmStackWaterMarkAboveThresholdTask1Hz(
                    error_table);
        }
        break;

        case CANMSGS_DIM_NON_CRITICAL_ERRORS_FRAME_ID:
        {
            struct CanMsgs_dim_non_critical_errors_t buffer;

            App_CanMsgs_dim_non_critical_errors_unpack(
                &buffer, &message->data[0],
                CANMSGS_DIM_NON_CRITICAL_ERRORS_LENGTH);

            if (buffer.stack_watermark_above_threshold_task1_hz != 0)
                App_ErrorTable_SetDimStackWaterMarkAboveThresholdTask1Hz(
                    error_table);
            else
                App_ErrorTable_ClearDimStackWaterMarkAboveThresholdTask1Hz(
                    error_table);
        }
        break;
    }
}
