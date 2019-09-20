/**
 * @brief This file is auto-generated for PDM. !!! Do not modify !!!
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
    .pdm_air_shutdown = { 0 }, /* Warning: This DLC of this message is 0 so its struct contains only a dummy variable */
    .pdm_motor_shutdown = { 0 }, /* Warning: This DLC of this message is 0 so its struct contains only a dummy variable */
    .pdm_errors = { 0 },
    .pdm_can_tx_fifo_overflow = { 0 },
    .pdm_heartbeat = { 0 },
    .pdm_startup = { 0 }, /* Warning: This DLC of this message is 0 so its struct contains only a dummy variable */
    .pdm_aux1_aux2_current = { 0 },
    .pdm_cooling_pdmfan_current = { 0 },
    .pdm_airshdn_canglv_current = { 0 },
    .pdm_accumulator_fan_current = { 0 },
    .pdm_inverter_io_current = { 0 },
    .pdm_glv_vbat = { 0 },
    .pdm_flywire = { 0 },
};

/**
 *@brief Table containing information about CAN messages that need to be
 *       transmitted at regular intervals
 */
struct CanTxPeriodicTableEntry CanTxPeriodicTable[] =
{
    INIT_TABLE_ENTRY(CanMsgs_pdm_errors_pack, 96U, 8U, 1000U, &CanTxPayloads.pdm_errors)
    INIT_TABLE_ENTRY(CanMsgs_pdm_heartbeat_pack, 97U, 8U, 300U, &CanTxPayloads.pdm_heartbeat)
    INIT_TABLE_ENTRY(CanMsgs_pdm_aux1_aux2_current_pack, 102U, 8U, 1000U, &CanTxPayloads.pdm_aux1_aux2_current)
    INIT_TABLE_ENTRY(CanMsgs_pdm_cooling_pdmfan_current_pack, 103U, 8U, 1000U, &CanTxPayloads.pdm_cooling_pdmfan_current)
    INIT_TABLE_ENTRY(CanMsgs_pdm_airshdn_canglv_current_pack, 104U, 8U, 1000U, &CanTxPayloads.pdm_airshdn_canglv_current)
    INIT_TABLE_ENTRY(CanMsgs_pdm_accumulator_fan_current_pack, 105U, 8U, 1000U, &CanTxPayloads.pdm_accumulator_fan_current)
    INIT_TABLE_ENTRY(CanMsgs_pdm_inverter_io_current_pack, 106U, 8U, 1000U, &CanTxPayloads.pdm_inverter_io_current)
    INIT_TABLE_ENTRY(CanMsgs_pdm_glv_vbat_pack, 107U, 8U, 1000U, &CanTxPayloads.pdm_glv_vbat)
    INIT_TABLE_ENTRY(CanMsgs_pdm_flywire_pack, 108U, 4U, 1000U, &CanTxPayloads.pdm_flywire)
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

