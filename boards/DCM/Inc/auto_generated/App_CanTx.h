/**
 * @brief CAN TX Library
 * @note This file is auto-generated for DCM. !!! Do not modify !!!
 */
// clang-format off
#ifndef APP_CANTX_H
#define APP_CANTX_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include "App_SharedWorld.h"
#include "auto_generated/CanMsgs.h"

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/** @brief Initialize CAN TX library */
struct CanTxInterface* App_CanTx_Create(
    void (*enqueue_non_periodic_msg_DCM_STARTUP)(struct CanMsgs_dcm_startup_t * payload),
    void (*enqueue_non_periodic_msg_DCM_WATCHDOG_TIMEOUT)(struct CanMsgs_dcm_watchdog_timeout_t * payload));

/** @brief Signal getters for periodic CAN TX messages */
void App_CanTx_SetPeriodicSignal_DUMMY_VARIABLE(struct CanTxInterface* can_tx_interface, uint8_t value);
void App_CanTx_SetPeriodicSignal_TX_OVERFLOW_COUNT(struct CanTxInterface* can_tx_interface, uint32_t value);
void App_CanTx_SetPeriodicSignal_RX_OVERFLOW_COUNT(struct CanTxInterface* can_tx_interface, uint32_t value);
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ(struct CanTxInterface* can_tx_interface, uint8_t value);
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_KHZ(struct CanTxInterface* can_tx_interface, uint8_t value);
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX(struct CanTxInterface* can_tx_interface, uint8_t value);
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX(struct CanTxInterface* can_tx_interface, uint8_t value);
void App_CanTx_SetPeriodicSignal_WATCHDOG_TIMEOUT(struct CanTxInterface* can_tx_interface, uint8_t value);

struct CanMsgs_dcm_can_fifo_overflow_t* App_CanTx_GetPeriodicMsgPointer_DCM_CAN_FIFO_OVERFLOW(struct CanTxInterface* can_tx_interface);
struct CanMsgs_dcm_heartbeat_t * App_CanTx_GetPeriodicMsgPointer_DCM_HEARTBEAT(struct CanTxInterface* can_tx_interface);

void App_CanTx_SendNonPeriodicMsg_DCM_STARTUP(struct CanTxInterface* can_tx_interface, struct CanMsgs_dcm_startup_t* payload);
void App_CanTx_SendNonPeriodicMsg_DCM_WATCHDOG_TIMEOUT(struct CanTxInterface* can_tx_interface, struct CanMsgs_dcm_watchdog_timeout_t * payload);

#endif /* APP_CANTX_H */
