/**
 * @brief Shared heartbeat module
 */

#ifndef SHARED_HEARTBEAT_H
#define SHARED_HEARTBEAT_H
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SharedCan.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
// Each board listens to certain other boards' CAN messages through the filters.
// Listen to the heartbeats of those other boards. This is hardcoded as there
// isn't a nice way to put the board-specific heartbeat listeners in the
// INIT_MASK_FILTER macro with the shared/BAMOCAR filters.
#ifdef PDM
    #define PCB_HEARTBEAT_LISTENER      BMS_HEARTBEAT_ENCODING
#elif FSM
    #define PCB_HEARTBEAT_LISTENER      BMS_HEARTBEAT_ENCODING
#elif BMS
    #define PCB_HEARTBEAT_LISTENER      (DCM_HEARTBEAT_ENCODING | PDM_HEARTBEAT_ENCODING | FSM_HEARTBEAT_ENCODING)
#elif DCM
#else
    #error "No valid PCB name selected"
#endif

// Uncomment this define to enable debug mode, which disables heartbeat
// timeouts. This is useful when stepping through code on a board that is
// providing heartbeats to other boards.
//#define DEBUG

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/
 /**
  * @brief Check if a board's heartbeat wasn't received
  * @param heartbeats_received One-hot encoding of heartbeats received
  * @param encoding One-hot board encoding
  */
 #define HEARTBEAT_TIMEOUT(heartbeats_received, encoding) \
    ((heartbeats_received & encoding) != encoding)

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on
/** @brief One-hot board encoding */
typedef enum
{
    BMS_HEARTBEAT_ENCODING = (1U << 0U),
    DCM_HEARTBEAT_ENCODING = (1U << 1U),
    PDM_HEARTBEAT_ENCODING = (1U << 2U),
    FSM_HEARTBEAT_ENCODING = (1U << 3U),
} PcbHeartbeatEncoding_Enum;

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Periodically broadcast heartbeat message for the current PCB. This
 *        functon checks for time differences by incrementing an internal
 *        variable every 1ms, which means it should be placed inside the 1kHz
 *        task.
 * @param heartbeat_can_id The CAN ID of the heartbeat CAN message.
 * @param heartbeat_tx_dlc The CAN DLC of the heartbeat CAN message.
 */
void SharedHeartbeat_BroadcastHeartbeat(
    uint32_t heartbeat_can_id,
    uint32_t heartbeat_dlc);

/**
 * @brief Upon heartbeat reception, update the list of heartbeats received
 * @param board One-hot board encoding from which the heartbeat was received
 */
void SharedHeartbeat_ReceiveHeartbeat(PcbHeartbeatEncoding_Enum board);

/**
 * @brief Periodically check that all heartbeats the the PCB listens for were
 *        received. This functon checks for time differences by incrementing an
 *        internal variable every 1ms, which means it should be placed inside
 *        the 1kHz task.
 * @param heartbeats_to_check The PCBs to listen heartbeats for. Perform a
 *        logical OR on the value(s) of PcbHeartbeatEncoding_Enum to specify the
 *        PCBs.
 */
void SharedHeartbeat_CheckHeartbeatTimeout(uint8_t heartbeats_to_check);

/**
 * @brief  Shared handler for missed heartbeats
 * @param  heartbeats_received One-hot encoding of heartbeats received
 */
void Heartbeat_HandleHeartbeatTimeout(uint8_t heartbeats_received);

/**
 * @brief Handle heartbeat reception for each board listened to
 * @param std_id Standard ID of the received CAN message
 */
void Heartbeat_HandleHeartbeatReception(uint32_t std_id);

#endif /* SHARED_HEARTBEAT_H */
