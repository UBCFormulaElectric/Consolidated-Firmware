/**
 * @note This file is auto-generated for DCM. !!! Do not modify !!!
 */
// clang-format off

#include <string.h>
#include <stdlib.h>
#include "auto_generated/App_CanTx.h"
#include "SharedAssert.h"
/** @brief Periodic CAN TX message */
struct PeriodicCanTxMsgs
{
    struct CanMsgs_dcm_heartbeat_t dcm_heartbeat;
    struct CanMsgs_dcm_can_fifo_overflow_t dcm_can_fifo_overflow;
    struct CanMsgs_dcm_errors_t dcm_errors;
};

/** @brief Can TX interface */
struct CanTxInterface
{
    struct PeriodicCanTxMsgs periodic_can_tx_table;
    void (*send_non_periodic_msg_DCM_STARTUP)(struct CanMsgs_dcm_startup_t* payload) ;
    void (*send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT)(struct CanMsgs_dcm_watchdog_timeout_t* payload) ;
};
/**
 * @brief Initialize a periodic CAN TX message
 */
#define INIT_PERIODIC_CANTX_MSG()     {0}

/**
 * @brief Maximum number for CAN TX interfaces
 */
#define MAX_NUM_OF_CANTX_INTERFACES  1



struct CanTxInterface* App_CanTx_Create(
    void (*send_non_periodic_msg_DCM_STARTUP)(struct CanMsgs_dcm_startup_t* payload),
    void (*send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT)(struct CanMsgs_dcm_watchdog_timeout_t* payload))
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

void App_CanTx_SetPeriodicSignal_DUMMY_VARIABLE(struct CanTxInterface* can_tx_interface, uint8_t value)
{
    can_tx_interface->periodic_can_tx_table.dcm_heartbeat.dummy_variable = value;
}

void App_CanTx_SetPeriodicSignal_TX_OVERFLOW_COUNT(struct CanTxInterface* can_tx_interface, uint32_t value)
{
    can_tx_interface->periodic_can_tx_table.dcm_can_fifo_overflow.tx_overflow_count = value;
}

void App_CanTx_SetPeriodicSignal_RX_OVERFLOW_COUNT(struct CanTxInterface* can_tx_interface, uint32_t value)
{
    can_tx_interface->periodic_can_tx_table.dcm_can_fifo_overflow.rx_overflow_count = value;
}

void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ(struct CanTxInterface* can_tx_interface, uint8_t value)
{
    can_tx_interface->periodic_can_tx_table.dcm_errors.stack_watermark_above_threshold_task1_hz = value;
}

void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_KHZ(struct CanTxInterface* can_tx_interface, uint8_t value)
{
    can_tx_interface->periodic_can_tx_table.dcm_errors.stack_watermark_above_threshold_task1_khz = value;
}

void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX(struct CanTxInterface* can_tx_interface, uint8_t value)
{
    can_tx_interface->periodic_can_tx_table.dcm_errors.stack_watermark_above_threshold_taskcanrx = value;
}

void App_CanTx_SetPeriodicSignal_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX(struct CanTxInterface* can_tx_interface, uint8_t value)
{
    can_tx_interface->periodic_can_tx_table.dcm_errors.stack_watermark_above_threshold_taskcantx = value;
}

void App_CanTx_SetPeriodicSignal_WATCHDOG_TIMEOUT(struct CanTxInterface* can_tx_interface, uint8_t value)
{
    can_tx_interface->periodic_can_tx_table.dcm_errors.watchdog_timeout = value;
}

struct CanMsgs_dcm_heartbeat_t* App_CanTx_GetPeriodicMsgPointer_DCM_HEARTBEAT(struct CanTxInterface* can_tx_interface)
{
    return &can_tx_interface->periodic_can_tx_table.dcm_heartbeat;
}

struct CanMsgs_dcm_can_fifo_overflow_t* App_CanTx_GetPeriodicMsgPointer_DCM_CAN_FIFO_OVERFLOW(struct CanTxInterface* can_tx_interface)
{
    return &can_tx_interface->periodic_can_tx_table.dcm_can_fifo_overflow;
}

struct CanMsgs_dcm_errors_t* App_CanTx_GetPeriodicMsgPointer_DCM_ERRORS(struct CanTxInterface* can_tx_interface)
{
    return &can_tx_interface->periodic_can_tx_table.dcm_errors;
}

void App_CanTx_SendNonPeriodicMsg_DCM_STARTUP(struct CanTxInterface* can_tx_interface, struct CanMsgs_dcm_startup_t* payload)
{
    can_tx_interface->send_non_periodic_msg_DCM_STARTUP(payload);
}

void App_CanTx_SendNonPeriodicMsg_DCM_WATCHDOG_TIMEOUT(struct CanTxInterface* can_tx_interface, struct CanMsgs_dcm_watchdog_timeout_t* payload)
{
    can_tx_interface->send_non_periodic_msg_DCM_WATCHDOG_TIMEOUT(payload);
}
