/**
 * @brief Shared heartbeat module
 */

#ifndef SHARED_HEARTBEAT_H
#define SHARED_HEARTBEAT_H
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "Io_SharedCan.h"
#include "Io_BoardSpecifics.h"

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
// Each board listens to certain other boards' CAN messages through the filters.
#ifndef PCB_HEARTBEAT_LISTENER
    #error "Please define the boards whose heartbeats we are checking for using PCB_HEARTBEAT_LISTENER!"
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
enum PcbHeartBeatEncoding
{
    BMS_HEARTBEAT_ENCODING = (1U << 0U),
    DCM_HEARTBEAT_ENCODING = (1U << 1U),
    PDM_HEARTBEAT_ENCODING = (1U << 2U),
    FSM_HEARTBEAT_ENCODING = (1U << 3U),
};

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
void Io_SharedHeartbeat_BroadcastHeartbeat(
    uint32_t heartbeat_can_id,
    uint32_t heartbeat_dlc);

/**
 * @brief Upon heartbeat reception, update the list of heartbeats received
 * @param board One-hot board encoding from which the heartbeat was received
 */
void Io_SharedHeartbeat_ReceiveHeartbeat(enum PcbHeartBeatEncoding board);

/**
 * @brief Periodically check that all heartbeats the the PCB listens for were
 *        received. This functon checks for time differences by incrementing an
 *        internal variable every 1ms, which means it should be placed inside
 *        the 1kHz task.
 * @param heartbeats_to_check The PCBs to listen heartbeats for. Perform a
 *        logical OR on the value(s) of enum PcbHeartBeatEncoding to specify the
 *        PCBs.
 */
void Io_SharedHeartbeat_CheckHeartbeatTimeout(uint8_t heartbeats_to_check);

/**
 * @brief  Shared handler for missed heartbeats
 * @param  heartbeats_received One-hot encoding of heartbeats received
 */
void Io_Heartbeat_HandleHeartbeatTimeout(uint8_t heartbeats_received);

/**
 * @brief Handle heartbeat reception for each board listened to
 * @param std_id Standard ID of the received CAN message
 */
void Io_Heartbeat_HandleHeartbeatReception(uint32_t std_id);

#endif /* SHARED_HEARTBEAT_H */
