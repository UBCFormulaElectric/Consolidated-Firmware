/**
 * @brief CAN TX Library for the IO layer
 * @note This file is auto-generated for DCM. !!! Do not modify !!!
 */
// clang-format off
#pragma once

#include <stdint.h>
#include "auto_generated/CanMsgs.h"

struct CanTxInterface;

/** @brief Transmits periodic CAN TX messages according to the cycle time specified in the DBC. This should be called in a 1kHz task. */
void Io_CanTx_TransmitPeriodicMessages(struct CanTxInterface* can_tx_interface, const uint32_t current_ms);
/** @brief Enqueue non-periodic CAN message to the CAN TX queue */
void Io_CanTx_EnqueueNonPeriodicMsg_DCM_STARTUP(struct CanMsgs_dcm_startup_t* payload);
void Io_CanTx_EnqueueNonPeriodicMsg_DCM_WATCHDOG_TIMEOUT(struct CanMsgs_dcm_watchdog_timeout_t* payload);
