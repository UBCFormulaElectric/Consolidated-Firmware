/**
 * @brief This file is auto-generated for FSM. !!! Do not modify !!!
 */
// clang-format off
/******************************************************************************
 * Includes
 ******************************************************************************/
#include <sched.h>
#include "App_CanMsgsTx.h"
#include "SharedMacros.h"
#include "SharedCan.h"
#include "stm32f3xx_hal.h"
/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/
/**
 * @brief Helper macro for initialize a single CAN transmit table entry.
 */
#define INIT_TABLE_ENTRY(FN_PTR, STDID, DLC, PERIOD, PAYLOAD) \
    {.pack_payload_fn=(pack_payload_fn_ptr)FN_PTR, .stdid=STDID, .dlc=DLC, .period=PERIOD, .payload=(const void*)PAYLOAD},

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
/** @brief All CAN periodic and non-periodic messages being transmitted */
struct CanTxPayloads CanTxPayloads =
{
    .fsm_errors = { 0 },
    .fsm_can_tx_fifo_overflow = { 0 },
    .fsm_startup = { 0 }, /* Warning: This DLC of this message is 0 so its struct contains only a dummy variable */
    .fsm_air_shutdown = { 0 }, /* Warning: This DLC of this message is 0 so its struct contains only a dummy variable */
    .fsm_heartbeat = { 0 }, /* Warning: This DLC of this message is 0 so its struct contains only a dummy variable */
};

/**
 *@brief Table containing information about CAN messages that need to be
 *       transmitted at regular intervals
 */
struct CanTxPeriodicTableEntry CanTxPeriodicTable[] =
{
    INIT_TABLE_ENTRY(CanMsgs_fsm_errors_pack, 64U, 8U, 1000U, &CanTxPayloads.fsm_errors)
    INIT_TABLE_ENTRY(CanMsgs_fsm_heartbeat_pack, 65U, 0U, 300U, &CanTxPayloads.fsm_heartbeat)
};

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void App_CanMsgsTx_PeriodicTransmit(void)
{
    for (uint32_t i = 0; i < NUM_ELEMENTS_IN_ARRAY(CanTxPeriodicTable); i++)
    {
        // Is it time to transmit this particular CAN message?
        if ((HAL_GetTick() % CanTxPeriodicTable[i].period) == 0)
        {
            uint8_t data[CAN_PAYLOAD_MAX_NUM_BYTES] = { 0 };
            // Populate the CAN payload
            CanTxPeriodicTable[i].pack_payload_fn(
                &data[0],
                CanTxPeriodicTable[i].payload,
                CanTxPeriodicTable[i].dlc);
            // Transmit the CAN payload with the appropriate ID and DLC
            SharedCan_TransmitDataCan(
                CanTxPeriodicTable[i].stdid,
                CanTxPeriodicTable[i].dlc,
                &data[0]);
        }
    }
}

struct CanTxPeriodicTableEntry *App_CanMsgsTx_GetCanTxPeriodicTable(void)
{
    return &CanTxPeriodicTable[0];
}

struct CanTxPayloads *App_CanMsgsTx_GetCanTxPayloads(void)
{
    return &CanTxPayloads;
}

