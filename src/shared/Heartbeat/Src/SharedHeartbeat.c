/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SharedHeartbeat.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// Allow some margin of error by sending 3 heartbeats per board per timeout
// period
#define HEARTBEAT_TIMEOUT_PERIOD    320     // Period in ms
#define HEARTBEAT_BROADCAST_PERIOD  100     // Period in ms

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
    uint8_t data[PCB_HEARTBEAT_DLC] = { 0 };
    static uint32_t heartbeat_broadcast_ticks = 0;

    heartbeat_broadcast_ticks++;

    if (heartbeat_broadcast_ticks >= HEARTBEAT_BROADCAST_PERIOD)
    {
        heartbeat_broadcast_ticks = 0;
        SharedCan_TransmitDataCan(PCB_HEARTBEAT_STDID, PCB_HEARTBEAT_DLC, &data[0]);
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

        // Check if the board didn't receive all the heartbeats it's listening for
        if (heartbeats_received != PCB_HEARTBEAT_LISTENER)
        {
            SharedHeartbeat_MissedHeartbeatsHandler(heartbeats_received);
        }

        // Reset list of heartbeats received
        heartbeats_received = 0;
    }
}

__weak void SharedHeartbeat_MissedHeartbeatsHandler(uint8_t heartbeats_received)
{
    /* NOTE: This function should not be modified, instead this function should
    be implemented in the Heartbeat.c file */
}
