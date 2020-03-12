// TODO: Actually autogen this file and its header
/**
 * @note This file is auto-generated for DCM. !!! Do not modify !!!
 */

#include <stdlib.h>
#include <string.h>

// TODO: SharedAssert it not compilable on x86 at the moment
#include "SharedAssert.h"
#include "auto_generated/App_CanTx.h"

struct PeriodicCanTxMsgs
{
    struct CanMsgs_dcm_heartbeat_t dcm_heartbeat;
    struct CanMsgs_dcm_can_fifo_overflow_t dcm_can_fifo_overflow;
    struct CanMsgs_dcm_errors_t dcm_errors;
};

struct CanTxInterface
{
    struct PeriodicCanTxMsgs periodic_can_tx_table;
    void (*send_non_periodic_msg_DCM_STARTUP)(struct CanMsgs_dcm_startup_t * payload);
    void (*send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT)(struct CanMsgs_dcm_watchdog_timeout_t * payload);
};

#define MAX_NUM_OF_CANTX_INTERFACES 1
struct CanTxInterface* App_CanTx_Create(
    void (*send_non_periodic_msg_DCM_STARTUP)(struct CanMsgs_dcm_startup_t * payload),
    void (*send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT)(struct CanMsgs_dcm_watchdog_timeout_t * payload)
    )
{
    static struct CanTxInterface can_tx_interfaces[MAX_NUM_OF_CANTX_INTERFACES];
    static size_t alloc_index = 0;

    shared_assert(alloc_index < MAX_NUM_OF_CANTX_INTERFACES);

    struct CanTxInterface* can_tx_interface = &can_tx_interfaces[alloc_index++];

    // TODO: Instead of clearing the table to 0's, use .dbc to fill in the
    // initial signal values
    memset(&can_tx_interface->periodic_can_tx_table, 0, sizeof(can_tx_interface->periodic_can_tx_table));

    can_tx_interface->send_non_periodic_msg_DCM_STARTUP = send_non_periodic_msg_DCM_STARTUP;
    can_tx_interface->send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT = send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT;

    return can_tx_interface;
}

// These helper functions are auto-generated according to DBC
// - Periodic signal setters
// - Non-periodic enqueue message helper
void App_CanTx_SetPeriodicSignal_DUMMY_VARIABLE(struct CanTxInterface* can_tx_interface, uint8_t value) {
    can_tx_interface->periodic_can_tx_table.dcm_heartbeat.dummy_variable = value;
}
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ(struct CanTxInterface* can_tx_interface, uint8_t value) {
    can_tx_interface->periodic_can_tx_table.dcm_errors.stack_watermark_above_threshold_task1_hz = value;
}
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_KHZ(struct CanTxInterface* can_tx_interface, uint8_t value) {
    can_tx_interface->periodic_can_tx_table.dcm_errors.stack_watermark_above_threshold_task1_khz = value;
}
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX(struct CanTxInterface* can_tx_interface, uint8_t value) {
    can_tx_interface->periodic_can_tx_table.dcm_errors.stack_watermark_above_threshold_taskcanrx = value;
}
void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX(struct CanTxInterface* can_tx_interface, uint8_t value) {
    can_tx_interface->periodic_can_tx_table.dcm_errors.stack_watermark_above_threshold_taskcantx = value;
}
void App_CanTx_SetPeriodicSignal_TX_OVERFLOW_COUNT(struct CanTxInterface* can_tx_interface, uint32_t value) {
    can_tx_interface->periodic_can_tx_table.dcm_can_fifo_overflow.tx_overflow_count = value;
}
void App_CanTx_SetPeriodicSignal_RX_OVERFLOW_COUNT(struct CanTxInterface* can_tx_interface, uint32_t value) {
    can_tx_interface->periodic_can_tx_table.dcm_can_fifo_overflow.rx_overflow_count= value;
}
void App_CanTx_SetPeriodicSignal_WATCHDOG_TIMEOUT(struct CanTxInterface* can_tx_interface, uint8_t value) {
    can_tx_interface->periodic_can_tx_table.dcm_errors.watchdog_timeout = value;
}

struct CanMsgs_dcm_can_fifo_overflow_t* App_CanTx_GetPeriodicMsgPointer_DCM_CAN_FIFO_OVERFLOW(struct CanTxInterface* can_tx_interface)
{
    return &can_tx_interface->periodic_can_tx_table.dcm_can_fifo_overflow;
}

struct CanMsgs_dcm_heartbeat_t * App_CanTx_GetPeriodicMsgPointer_DCM_HEARTBEAT(struct CanTxInterface* can_tx_interface)
{
    return &can_tx_interface->periodic_can_tx_table.dcm_heartbeat;
}

void App_CanTx_SendNonPeriodicMsg_DCM_STARTUP(struct CanTxInterface* can_tx_interface, struct CanMsgs_dcm_startup_t* payload)
{
    can_tx_interface->send_non_periodic_msg_DCM_STARTUP(payload);
}
void App_CanTx_SendNonPeriodicMsg_DCM_WATCHDOG_TIMEOUT(struct CanTxInterface* can_tx_interface, struct CanMsgs_dcm_watchdog_timeout_t * payload)
{
    can_tx_interface->send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT(payload);
}
