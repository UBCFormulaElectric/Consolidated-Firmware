/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SharedHeartbeat.h"
#include "cmsis_os.h"

/******************************************************************************
 * Module Preprocessor Constants
 ******************************************************************************/
// clang-format off
#define HEARTBEAT_BROADCAST_PERIOD_MS      100U
#define HEARTBEAT_TIMEOUT_ERROR_MARGIN_MS  20U
// Allow some error margin by sending slightly more than 3 heartbeats per board
// per timeout period
#define HEARTBEAT_TIMEOUT_PERIOD_MS \
    (HEARTBEAT_BROADCAST_PERIOD_MS * 3U) + HEARTBEAT_TIMEOUT_ERROR_MARGIN_MS

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
void SharedHeartbeat_BroadcastHeartbeat(
    uint32_t heartbeat_can_id,
    uint32_t heartbeat_dlc)
{
    // Since we do not know the size of the heartbeat we're sending, we just
    // use the maximum possible size for the data
    uint8_t          data[CAN_PAYLOAD_MAX_NUM_BYTES] = { 0 };
    static uint32_t  previous_broadcast_ms           = 0;
    const TickType_t current_ms                      = osKernelSysTick();

    if ((current_ms - previous_broadcast_ms) >= HEARTBEAT_BROADCAST_PERIOD_MS)
    {
        previous_broadcast_ms = current_ms;

        // TODO: (Issue #217) Test out message of size 0, as that would be
        // more semantically meaningful here
        SharedCan_TransmitDataCan(heartbeat_can_id, heartbeat_dlc, &data[0]);
    }
}

void SharedHeartbeat_ReceiveHeartbeat(PcbHeartbeatEncoding_Enum board)
{
    heartbeats_received |= board;
}

void SharedHeartbeat_CheckHeartbeatTimeout(uint8_t heartbeats_to_check)
{
    static uint32_t  previous_timeout_ms = 0;
    const TickType_t current_ms          = osKernelSysTick();

    if ((current_ms - previous_timeout_ms) >= HEARTBEAT_TIMEOUT_PERIOD_MS)
    {
        previous_timeout_ms = current_ms;

#ifdef DEBUG
        (void)heartbeats_to_check;
#else
        // Check if the board received all the heartbeats it's listening for
        if (heartbeats_received != heartbeats_to_check)
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
    UNUSED(heartbeats_received);
}

__weak void Heartbeat_HandleHeartbeatReception(uint32_t std_id)
{
    /* NOTE: This function should not be modified, instead this function should
    be implemented in the Heartbeat.c file */
    UNUSED(std_id);
}
