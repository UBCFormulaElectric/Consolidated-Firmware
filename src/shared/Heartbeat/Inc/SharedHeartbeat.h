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
    #define PCB_HEARTBEAT_STDID         PDM_HEARTBEAT_STDID
    #define PCB_HEARTBEAT_DLC           PDM_HEARTBEAT_DLC
    #define PCB_HEARTBEAT_LISTENER      BMS_HEARTBEAT_ENCODING
#elif FSM
    #define PCB_HEARTBEAT_STDID         FSM_HEARTBEAT_STDID
    #define PCB_HEARTBEAT_DLC           FSM_HEARTBEAT_DLC
    #define PCB_HEARTBEAT_LISTENER      BMS_HEARTBEAT_ENCODING
#elif BMS
    #define PCB_HEARTBEAT_STDID         BMS_HEARTBEAT_STDID
    #define PCB_HEARTBEAT_DLC           BMS_HEARTBEAT_DLC
    #define PCB_HEARTBEAT_LISTENER      (DCM_HEARTBEAT_ENCODING | PDM_HEARTBEAT_ENCODING | FSM_HEARTBEAT_ENCODING)
#elif DCM
    #define PCB_HEARTBEAT_STDID         DCM_HEARTBEAT_STDID
    #define PCB_HEARTBEAT_DLC           DCM_HEARTBEAT_DLC
    #define PCB_HEARTBEAT_LISTENER      (BMS_HEARTBEAT_ENCODING | FSM_HEARTBEAT_ENCODING)
#else
    #error "No valid PCB name selected"
#endif

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format  on
/** @brief One-hot PCB encoding */
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
  * @brief Broadcast heartbeat message for the current PCB
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
 void SharedHeartbeat_CheckHeartbeatsReceived(void);
 
 /**
  * @brief  Shared handler for missed heartbeats
  * @param  heartbeats_received One-hot encoding of heartbeats received
  */
 void SharedHeartbeat_MissedHeartbeatsHandler(uint8_t heartbeats_received);

#endif /* SHARED_HEARTBEAT_H */
