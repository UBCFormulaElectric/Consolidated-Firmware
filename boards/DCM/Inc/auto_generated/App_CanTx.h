/**
 * @brief CAN TX Library for the application layer
 * @note This file is auto-generated for DCM. !!! Do not modify !!!
 */
// clang-format off
#pragma once

#include <stdint.h>
#include "auto_generated/CanMsgs.h"

/** @brief Allocate and initialize a CAN TX interface */
struct CanTxInterface* App_CanTx_Create(
    void (*send_non_periodic_msg_DCM_STARTUP)(struct CanMsgs_dcm_startup_t* payload),
    void (*send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT)(struct CanMsgs_dcm_watchdog_timeout_t* payload));

/** @brief Signal getters for periodic CAN TX messages */
void App_CanTx_SetPeriodicSignal_DUMMY_VARIABLE(struct CanTxInterface* can_tx_interface, uint8_t value);
void App_CanTx_SetPeriodicSignal_TX_OVERFLOW_COUNT(struct CanTxInterface* can_tx_interface, uint32_t value);
void App_CanTx_SetPeriodicSignal_RX_OVERFLOW_COUNT(struct CanTxInterface* can_tx_interface, uint32_t value);
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ(struct CanTxInterface* can_tx_interface, uint8_t value);
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_KHZ(struct CanTxInterface* can_tx_interface, uint8_t value);
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX(struct CanTxInterface* can_tx_interface, uint8_t value);
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX(struct CanTxInterface* can_tx_interface, uint8_t value);
void App_CanTx_SetPeriodicSignal_WATCHDOG_TIMEOUT(struct CanTxInterface* can_tx_interface, uint8_t value);

/** @brief Getter for pointer to an entry in the periodic CAN TX message table */
struct CanMsgs_dcm_heartbeat_t* App_CanTx_GetPeriodicMsgPointer_DCM_HEARTBEAT(struct CanTxInterface* can_tx_interface);
struct CanMsgs_dcm_can_fifo_overflow_t* App_CanTx_GetPeriodicMsgPointer_DCM_CAN_FIFO_OVERFLOW(struct CanTxInterface* can_tx_interface);
struct CanMsgs_dcm_errors_t* App_CanTx_GetPeriodicMsgPointer_DCM_ERRORS(struct CanTxInterface* can_tx_interface);

/** @brief Send a non-periodic CAN TX message */
void App_CanTx_SendNonPeriodicMsg_DCM_STARTUP(struct CanTxInterface* can_tx_interface, struct CanMsgs_dcm_startup_t* payload);
void App_CanTx_SendNonPeriodicMsg_DCM_WATCHDOG_TIMEOUT(struct CanTxInterface* can_tx_interface, struct CanMsgs_dcm_watchdog_timeout_t* payload);
