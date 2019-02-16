/**
 * @file  SharedHeartbeat.h
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
    #define PCB_HEARTBEAT_STDID         CANMSGS_pdm_heartbeat_FRAME_ID
    #define PCB_HEARTBEAT_LISTENER      BMS_HEARTBEAT_ENCODING
#elif FSM
    #define PCB_HEARTBEAT_STDID         CANMSGS_fsm_heartbeat_FRAME_ID
    #define PCB_HEARTBEAT_LISTENER      BMS_HEARTBEAT_ENCODING
#elif BMS
    #define PCB_HEARTBEAT_STDID         CANMSGS_bms_heartbeat_FRAME_ID 
    #define PCB_HEARTBEAT_LISTENER      (DCM_HEARTBEAT_ENCODING | PDM_HEARTBEAT_ENCODING | FSM_HEARTBEAT_ENCODING)
#elif DCM
    #define PCB_HEARTBEAT_STDID         CANMSGS_dcm_heartbeat_FRAME_ID 
    #define PCB_HEARTBEAT_LISTENER      (BMS_HEARTBEAT_ENCODING | FSM_HEARTBEAT_ENCODING)
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
// clang-format  on
/** @brief One-hot board encoding */
typedef enum
{
    BMS_HEARTBEAT_ENCODING = 0b0001,
    DCM_HEARTBEAT_ENCODING = 0b0010,
    PDM_HEARTBEAT_ENCODING = 0b0100,
    FSM_HEARTBEAT_ENCODING = 0b1000
} PcbHeartbeatEncoding_Enum;

/******************************************************************************
 * Global Variables
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Periodically broadcast heartbeat message for the current PCB
 */
void SharedHeartbeat_BroadcastHeartbeat(void);

/**
 * @brief Upon heartbeat reception, update the list of heartbeats received
 * @param board One-hot board encoding from which the heartbeat was received
 */
void SharedHeartbeat_ReceiveHeartbeat(PcbHeartbeatEncoding_Enum board);

/**
 * @brief Periodically check that all heartbeats the the PCB listens for were
 *        received
 */
void SharedHeartbeat_CheckHeartbeatTimeout(void);
 
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
