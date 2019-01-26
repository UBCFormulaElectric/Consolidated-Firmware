/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SharedHeartbeat.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// Allow some margin of error by sending slightly more than 3 heartbeats per
// board per timeout period
#define HEARTBEAT_BROADCAST_PERIOD 100    // Period in ms
#define HEARTBEAT_TIMEOUT_ERROR_MARGIN 20 // Period in ms
#define HEARTBEAT_TIMEOUT_PERIOD \
    (HEARTBEAT_BROADCAST_PERIOD * 3) + HEARTBEAT_TIMEOUT_ERROR_MARGIN

/******************************************************************************
 * Module Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Module Typedefs
 ******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
// clang-format on
// One-hot encoding of heartbeats received
static volatile uint8_t heartbeats_received = 0;

/******************************************************************************
 * Private Function Prototypes
 ******************************************************************************/

/******************************************************************************
 * Private Function Definitions
 ******************************************************************************/

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
void SharedHeartbeat_BroadcastHeartbeat(void)
{
    // TODO: Is using the max size appropriate here??
    uint8_t         data[CAN_PAYLOAD_BYTE_SIZE]   = { 0 };
    static uint32_t heartbeat_broadcast_ticks = 0;

    heartbeat_broadcast_ticks++;

    if (heartbeat_broadcast_ticks >= HEARTBEAT_BROADCAST_PERIOD)
    {
        heartbeat_broadcast_ticks = 0;

    // TODO: Is using the max size appropriate here??
        SharedCan_TransmitDataCan(
            PCB_HEARTBEAT_STDID, CAN_PAYLOAD_BYTE_SIZE, &data[0]);
    }
}

void SharedHeartbeat_ReceiveHeartbeat(PcbHeartbeatEncoding_Enum board)
{
    heartbeats_received |= board;
}

void SharedHeartbeat_CheckHeartbeatTimeout(void)
{
    static uint32_t heartbeat_timeout_ticks = 0;

    heartbeat_timeout_ticks++;

    if (heartbeat_timeout_ticks >= HEARTBEAT_TIMEOUT_PERIOD)
    {
        heartbeat_timeout_ticks = 0;

#ifndef DEBUG
        // Check if the board received all the heartbeats it's listening for
        if (heartbeats_received != PCB_HEARTBEAT_LISTENER)
        {
            Heartbeat_HandleHeartbeatTimeout(heartbeats_received);
        }
#endif /* DEBUG */

        // Reset list of heartbeats received
        heartbeats_received = 0;
    }
}

__weak void Heartbeat_HandleHeartbeatTimeout(uint8_t heartbeats_received)
{
    /* NOTE: This function should not be modified, instead this function should
    be implemented in the Heartbeat.c file */
}

__weak void Heartbeat_HandleHeartbeatReception(uint32_t std_id)
{
    /* NOTE: This function should not be modified, instead this function should
    be implemented in the Heartbeat.c file */
}
