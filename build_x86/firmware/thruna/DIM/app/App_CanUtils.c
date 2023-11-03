/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#include <stddef.h>
#include "App_CanUtils.h"

/* ------------------ Static Packing/Unpacking Functions ------------------ */

/**
 * Shift input left and apply mask, for packing.
 */
static inline uint8_t packShiftLeft(uint32_t input, uint8_t shift, uint8_t mask)
{
    return (uint8_t)((uint8_t)(input << shift) & (uint8_t)mask);
}

/**
 * Shift input right and apply mask, for packing.
 */
static inline uint8_t packShiftRight(uint32_t input, uint8_t shift, uint8_t mask)
{
    return (uint8_t)((uint8_t)(input >> shift) & (uint8_t)mask);
}

/**
 * Apply mask, then shift input left by shift bits, for unpacking.
 */
static inline uint32_t unpackShiftLeft(uint8_t input, uint8_t shift, uint8_t mask)
{
    return (uint32_t)((uint32_t)(input & mask) << shift);
}

/**
 * Apply mask, then shift input left by shift bits, for unpacking.
 */
static inline uint32_t unpackShiftRight(uint8_t input, uint8_t shift, uint8_t mask)
{
    return (uint32_t)((uint32_t)(input & mask) >> shift);
}


/* ----------------------- Encoding/Decoding Macros ----------------------- */

/**
 * Encode real signal value to payload representation, w/ scale and offset.
 */
#define CAN_ENCODE(input, scale, offset, type) ((uint32_t)((type)(input - offset) / (type)scale))

/**
 * Decode payload representation of signal to signal value, w/ scale and offset.
 */
#define CAN_DECODE(input, scale, offset, type) ((type)((type)input * (type)scale + (type)offset))

/**
 * Interpret input as a signed number of length bits via 2s complement.
 */
#define SIGNED_DECODE(input, bits, mask) (((int)((input & (1 << (bits - 1))) ? -((~input & mask) + 1) : input)))

/* ------------------------- Function Definitions ------------------------- */

void App_CanUtils_DIM_Vitals_Pack(const DIM_Vitals_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DIM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DIM_Heartbeat into payload (at bit 0 to bit 1).
    const bool DIM_Heartbeat_val = in_msg->DIM_Heartbeat_value;
    const uint32_t DIM_Heartbeat_raw = CAN_ENCODE(DIM_Heartbeat_val, CANSIG_DIM_VITALS_DIM_HEARTBEAT_SCALE, CANSIG_DIM_VITALS_DIM_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal DIM_State into payload (at bit 1 to bit 2).
    const DimState DIM_State_val = in_msg->DIM_State_value;
    const uint32_t DIM_State_raw = CAN_ENCODE(DIM_State_val, CANSIG_DIM_VITALS_DIM_STATE_SCALE, CANSIG_DIM_VITALS_DIM_STATE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_State_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
}

void App_CanUtils_DIM_Switches_Pack(const DIM_Switches_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DIM_Switches.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DIM_StartSwitch into payload (at bit 0 to bit 1).
    const SwitchState DIM_StartSwitch_val = in_msg->DIM_StartSwitch_value;
    const uint32_t DIM_StartSwitch_raw = CAN_ENCODE(DIM_StartSwitch_val, CANSIG_DIM_SWITCHES_DIM_START_SWITCH_SCALE, CANSIG_DIM_SWITCHES_DIM_START_SWITCH_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_StartSwitch_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal DIM_AuxSwitch into payload (at bit 1 to bit 2).
    const SwitchState DIM_AuxSwitch_val = in_msg->DIM_AuxSwitch_value;
    const uint32_t DIM_AuxSwitch_raw = CAN_ENCODE(DIM_AuxSwitch_val, CANSIG_DIM_SWITCHES_DIM_AUX_SWITCH_SCALE, CANSIG_DIM_SWITCHES_DIM_AUX_SWITCH_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_AuxSwitch_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
}

void App_CanUtils_DIM_AlertsContext_Pack(const DIM_AlertsContext_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message DIM_AlertsContext.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CCC|BBBBBBBB|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal DIM_TxOverflowCount into payload (at bit 0 to bit 8).
    const uint32_t DIM_TxOverflowCount_val = in_msg->DIM_TxOverflowCount_value;
    const uint32_t DIM_TxOverflowCount_raw = CAN_ENCODE(DIM_TxOverflowCount_val, CANSIG_DIM_ALERTS_CONTEXT_DIM_TX_OVERFLOW_COUNT_SCALE, CANSIG_DIM_ALERTS_CONTEXT_DIM_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_TxOverflowCount_raw, 0, 0xff);   // Packs bits ######## of byte 0
    
    // Pack 8-bit signal DIM_RxOverflowCount into payload (at bit 8 to bit 16).
    const uint32_t DIM_RxOverflowCount_val = in_msg->DIM_RxOverflowCount_value;
    const uint32_t DIM_RxOverflowCount_raw = CAN_ENCODE(DIM_RxOverflowCount_val, CANSIG_DIM_ALERTS_CONTEXT_DIM_RX_OVERFLOW_COUNT_SCALE, CANSIG_DIM_ALERTS_CONTEXT_DIM_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(DIM_RxOverflowCount_raw, 0, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 3-bit signal DIM_WatchdogTimeoutTaskName into payload (at bit 16 to bit 19).
    const RtosTaskName DIM_WatchdogTimeoutTaskName_val = in_msg->DIM_WatchdogTimeoutTaskName_value;
    const uint32_t DIM_WatchdogTimeoutTaskName_raw = CAN_ENCODE(DIM_WatchdogTimeoutTaskName_val, CANSIG_DIM_ALERTS_CONTEXT_DIM_WATCHDOG_TIMEOUT_TASK_NAME_SCALE, CANSIG_DIM_ALERTS_CONTEXT_DIM_WATCHDOG_TIMEOUT_TASK_NAME_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(DIM_WatchdogTimeoutTaskName_raw, 0, 0x7);   // Packs bits _____### of byte 2
    
}

void App_CanUtils_DIM_CommitInfo_Pack(const DIM_CommitInfo_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message DIM_CommitInfo.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_______B|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 32-bit signal DIM_Hash into payload (at bit 0 to bit 32).
    const uint32_t DIM_Hash_val = in_msg->DIM_Hash_value;
    const uint32_t DIM_Hash_raw = CAN_ENCODE(DIM_Hash_val, CANSIG_DIM_COMMIT_INFO_DIM_HASH_SCALE, CANSIG_DIM_COMMIT_INFO_DIM_HASH_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_Hash_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(DIM_Hash_raw, 8, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(DIM_Hash_raw, 16, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(DIM_Hash_raw, 24, 0xff);   // Packs bits ######## of byte 3
    
    // Pack 1-bit signal DIM_Clean into payload (at bit 32 to bit 33).
    const bool DIM_Clean_val = in_msg->DIM_Clean_value;
    const uint32_t DIM_Clean_raw = CAN_ENCODE(DIM_Clean_val, CANSIG_DIM_COMMIT_INFO_DIM_CLEAN_SCALE, CANSIG_DIM_COMMIT_INFO_DIM_CLEAN_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(DIM_Clean_raw, 0, 0x1);   // Packs bits _______# of byte 4
    
}

void App_CanUtils_DIM_Warnings_Pack(const DIM_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DIM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DIM_Warning_StackWaterMarkHighTask1Hz into payload (at bit 0 to bit 1).
    const bool DIM_Warning_StackWaterMarkHighTask1Hz_val = in_msg->DIM_Warning_StackWaterMarkHighTask1Hz_value;
    const uint32_t DIM_Warning_StackWaterMarkHighTask1Hz_raw = CAN_ENCODE(DIM_Warning_StackWaterMarkHighTask1Hz_val, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_Warning_StackWaterMarkHighTask1Hz_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal DIM_Warning_StackWaterMarkHighTask100Hz into payload (at bit 1 to bit 2).
    const bool DIM_Warning_StackWaterMarkHighTask100Hz_val = in_msg->DIM_Warning_StackWaterMarkHighTask100Hz_value;
    const uint32_t DIM_Warning_StackWaterMarkHighTask100Hz_raw = CAN_ENCODE(DIM_Warning_StackWaterMarkHighTask100Hz_val, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_Warning_StackWaterMarkHighTask100Hz_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal DIM_Warning_StackWaterMarkHighTask1kHz into payload (at bit 2 to bit 3).
    const bool DIM_Warning_StackWaterMarkHighTask1kHz_val = in_msg->DIM_Warning_StackWaterMarkHighTask1kHz_value;
    const uint32_t DIM_Warning_StackWaterMarkHighTask1kHz_raw = CAN_ENCODE(DIM_Warning_StackWaterMarkHighTask1kHz_val, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_Warning_StackWaterMarkHighTask1kHz_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal DIM_Warning_StackWaterMarkHighTaskCanRx into payload (at bit 3 to bit 4).
    const bool DIM_Warning_StackWaterMarkHighTaskCanRx_val = in_msg->DIM_Warning_StackWaterMarkHighTaskCanRx_value;
    const uint32_t DIM_Warning_StackWaterMarkHighTaskCanRx_raw = CAN_ENCODE(DIM_Warning_StackWaterMarkHighTaskCanRx_val, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_Warning_StackWaterMarkHighTaskCanRx_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal DIM_Warning_StackWaterMarkHighTaskCanTx into payload (at bit 4 to bit 5).
    const bool DIM_Warning_StackWaterMarkHighTaskCanTx_val = in_msg->DIM_Warning_StackWaterMarkHighTaskCanTx_value;
    const uint32_t DIM_Warning_StackWaterMarkHighTaskCanTx_raw = CAN_ENCODE(DIM_Warning_StackWaterMarkHighTaskCanTx_val, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_Warning_StackWaterMarkHighTaskCanTx_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal DIM_Warning_WatchdogTimeout into payload (at bit 5 to bit 6).
    const bool DIM_Warning_WatchdogTimeout_val = in_msg->DIM_Warning_WatchdogTimeout_value;
    const uint32_t DIM_Warning_WatchdogTimeout_raw = CAN_ENCODE(DIM_Warning_WatchdogTimeout_val, CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_Warning_WatchdogTimeout_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 1-bit signal DIM_Warning_TxOverflow into payload (at bit 6 to bit 7).
    const bool DIM_Warning_TxOverflow_val = in_msg->DIM_Warning_TxOverflow_value;
    const uint32_t DIM_Warning_TxOverflow_raw = CAN_ENCODE(DIM_Warning_TxOverflow_val, CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_Warning_TxOverflow_raw, 6, 0x40);   // Packs bits _#______ of byte 0
    
    // Pack 1-bit signal DIM_Warning_RxOverflow into payload (at bit 7 to bit 8).
    const bool DIM_Warning_RxOverflow_val = in_msg->DIM_Warning_RxOverflow_value;
    const uint32_t DIM_Warning_RxOverflow_raw = CAN_ENCODE(DIM_Warning_RxOverflow_val, CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_Warning_RxOverflow_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    
}

void App_CanUtils_DIM_Faults_Pack(const DIM_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DIM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DIM_Fault_MissingHeartbeat into payload (at bit 0 to bit 1).
    const bool DIM_Fault_MissingHeartbeat_val = in_msg->DIM_Fault_MissingHeartbeat_value;
    const uint32_t DIM_Fault_MissingHeartbeat_raw = CAN_ENCODE(DIM_Fault_MissingHeartbeat_val, CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_Fault_MissingHeartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
}

void App_CanUtils_DIM_WarningsCounts_Pack(const DIM_WarningsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message DIM_WarningsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal DIM_Warning_StackWaterMarkHighTask1HzCount into payload (at bit 0 to bit 3).
    const uint32_t DIM_Warning_StackWaterMarkHighTask1HzCount_val = in_msg->DIM_Warning_StackWaterMarkHighTask1HzCount_value;
    const uint32_t DIM_Warning_StackWaterMarkHighTask1HzCount_raw = CAN_ENCODE(DIM_Warning_StackWaterMarkHighTask1HzCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_Warning_StackWaterMarkHighTask1HzCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal DIM_Warning_StackWaterMarkHighTask100HzCount into payload (at bit 3 to bit 6).
    const uint32_t DIM_Warning_StackWaterMarkHighTask100HzCount_val = in_msg->DIM_Warning_StackWaterMarkHighTask100HzCount_value;
    const uint32_t DIM_Warning_StackWaterMarkHighTask100HzCount_raw = CAN_ENCODE(DIM_Warning_StackWaterMarkHighTask100HzCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_Warning_StackWaterMarkHighTask100HzCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal DIM_Warning_StackWaterMarkHighTask1kHzCount into payload (at bit 6 to bit 9).
    const uint32_t DIM_Warning_StackWaterMarkHighTask1kHzCount_val = in_msg->DIM_Warning_StackWaterMarkHighTask1kHzCount_value;
    const uint32_t DIM_Warning_StackWaterMarkHighTask1kHzCount_raw = CAN_ENCODE(DIM_Warning_StackWaterMarkHighTask1kHzCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_Warning_StackWaterMarkHighTask1kHzCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(DIM_Warning_StackWaterMarkHighTask1kHzCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 3-bit signal DIM_Warning_StackWaterMarkHighTaskCanRxCount into payload (at bit 9 to bit 12).
    const uint32_t DIM_Warning_StackWaterMarkHighTaskCanRxCount_val = in_msg->DIM_Warning_StackWaterMarkHighTaskCanRxCount_value;
    const uint32_t DIM_Warning_StackWaterMarkHighTaskCanRxCount_raw = CAN_ENCODE(DIM_Warning_StackWaterMarkHighTaskCanRxCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(DIM_Warning_StackWaterMarkHighTaskCanRxCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 1
    
    // Pack 3-bit signal DIM_Warning_StackWaterMarkHighTaskCanTxCount into payload (at bit 12 to bit 15).
    const uint32_t DIM_Warning_StackWaterMarkHighTaskCanTxCount_val = in_msg->DIM_Warning_StackWaterMarkHighTaskCanTxCount_value;
    const uint32_t DIM_Warning_StackWaterMarkHighTaskCanTxCount_raw = CAN_ENCODE(DIM_Warning_StackWaterMarkHighTaskCanTxCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(DIM_Warning_StackWaterMarkHighTaskCanTxCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1
    
    // Pack 3-bit signal DIM_Warning_WatchdogTimeoutCount into payload (at bit 15 to bit 18).
    const uint32_t DIM_Warning_WatchdogTimeoutCount_val = in_msg->DIM_Warning_WatchdogTimeoutCount_value;
    const uint32_t DIM_Warning_WatchdogTimeoutCount_raw = CAN_ENCODE(DIM_Warning_WatchdogTimeoutCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_WATCHDOG_TIMEOUT_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_WATCHDOG_TIMEOUT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(DIM_Warning_WatchdogTimeoutCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    out_data[2] |= packShiftRight(DIM_Warning_WatchdogTimeoutCount_raw, 1, 0x3);   // Packs bits ______## of byte 2
    
    // Pack 3-bit signal DIM_Warning_TxOverflowCount into payload (at bit 18 to bit 21).
    const uint32_t DIM_Warning_TxOverflowCount_val = in_msg->DIM_Warning_TxOverflowCount_value;
    const uint32_t DIM_Warning_TxOverflowCount_raw = CAN_ENCODE(DIM_Warning_TxOverflowCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_TX_OVERFLOW_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(DIM_Warning_TxOverflowCount_raw, 2, 0x1c);   // Packs bits ___###__ of byte 2
    
    // Pack 3-bit signal DIM_Warning_RxOverflowCount into payload (at bit 21 to bit 24).
    const uint32_t DIM_Warning_RxOverflowCount_val = in_msg->DIM_Warning_RxOverflowCount_value;
    const uint32_t DIM_Warning_RxOverflowCount_raw = CAN_ENCODE(DIM_Warning_RxOverflowCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_RX_OVERFLOW_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_WARNING_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(DIM_Warning_RxOverflowCount_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    
}

void App_CanUtils_DIM_FaultsCounts_Pack(const DIM_FaultsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DIM_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____AAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal DIM_Fault_MissingHeartbeatCount into payload (at bit 0 to bit 3).
    const uint32_t DIM_Fault_MissingHeartbeatCount_val = in_msg->DIM_Fault_MissingHeartbeatCount_value;
    const uint32_t DIM_Fault_MissingHeartbeatCount_raw = CAN_ENCODE(DIM_Fault_MissingHeartbeatCount_val, CANSIG_DIM_FAULTS_COUNTS_DIM_FAULT_MISSING_HEARTBEAT_COUNT_SCALE, CANSIG_DIM_FAULTS_COUNTS_DIM_FAULT_MISSING_HEARTBEAT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_Fault_MissingHeartbeatCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
}

void App_CanUtils_DCM_Vitals_Unpack(const uint8_t* const in_data, DCM_Vitals_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DCM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DCM_Heartbeat from payload (at bit 0 to bit 1).
    uint32_t DCM_Heartbeat_raw = 0;
    DCM_Heartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DCM_Heartbeat_val = CAN_DECODE(DCM_Heartbeat_raw, CANSIG_DCM_VITALS_DCM_HEARTBEAT_SCALE, CANSIG_DCM_VITALS_DCM_HEARTBEAT_OFFSET, uint32_t);
    out_msg->DCM_Heartbeat_value = DCM_Heartbeat_val;
    
    // Unpack 1-bit signal DCM_State from payload (at bit 1 to bit 2).
    uint32_t DCM_State_raw = 0;
    DCM_State_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    DcmState DCM_State_val = CAN_DECODE(DCM_State_raw, CANSIG_DCM_VITALS_DCM_STATE_SCALE, CANSIG_DCM_VITALS_DCM_STATE_OFFSET, uint32_t);
    out_msg->DCM_State_value = DCM_State_val;
    
}

void App_CanUtils_DCM_Warnings_Unpack(const uint8_t* const in_data, DCM_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DCM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DCM_Warning_StackWaterMarkHighTask1Hz from payload (at bit 0 to bit 1).
    uint32_t DCM_Warning_StackWaterMarkHighTask1Hz_raw = 0;
    DCM_Warning_StackWaterMarkHighTask1Hz_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DCM_Warning_StackWaterMarkHighTask1Hz_val = CAN_DECODE(DCM_Warning_StackWaterMarkHighTask1Hz_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_msg->DCM_Warning_StackWaterMarkHighTask1Hz_value = DCM_Warning_StackWaterMarkHighTask1Hz_val;
    
    // Unpack 1-bit signal DCM_Warning_StackWaterMarkHighTask100Hz from payload (at bit 1 to bit 2).
    uint32_t DCM_Warning_StackWaterMarkHighTask100Hz_raw = 0;
    DCM_Warning_StackWaterMarkHighTask100Hz_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool DCM_Warning_StackWaterMarkHighTask100Hz_val = CAN_DECODE(DCM_Warning_StackWaterMarkHighTask100Hz_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_msg->DCM_Warning_StackWaterMarkHighTask100Hz_value = DCM_Warning_StackWaterMarkHighTask100Hz_val;
    
    // Unpack 1-bit signal DCM_Warning_StackWaterMarkHighTask1kHz from payload (at bit 2 to bit 3).
    uint32_t DCM_Warning_StackWaterMarkHighTask1kHz_raw = 0;
    DCM_Warning_StackWaterMarkHighTask1kHz_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool DCM_Warning_StackWaterMarkHighTask1kHz_val = CAN_DECODE(DCM_Warning_StackWaterMarkHighTask1kHz_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_msg->DCM_Warning_StackWaterMarkHighTask1kHz_value = DCM_Warning_StackWaterMarkHighTask1kHz_val;
    
    // Unpack 1-bit signal DCM_Warning_StackWaterMarkHighTaskCanRx from payload (at bit 3 to bit 4).
    uint32_t DCM_Warning_StackWaterMarkHighTaskCanRx_raw = 0;
    DCM_Warning_StackWaterMarkHighTaskCanRx_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool DCM_Warning_StackWaterMarkHighTaskCanRx_val = CAN_DECODE(DCM_Warning_StackWaterMarkHighTaskCanRx_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_msg->DCM_Warning_StackWaterMarkHighTaskCanRx_value = DCM_Warning_StackWaterMarkHighTaskCanRx_val;
    
    // Unpack 1-bit signal DCM_Warning_StackWaterMarkHighTaskCanTx from payload (at bit 4 to bit 5).
    uint32_t DCM_Warning_StackWaterMarkHighTaskCanTx_raw = 0;
    DCM_Warning_StackWaterMarkHighTaskCanTx_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool DCM_Warning_StackWaterMarkHighTaskCanTx_val = CAN_DECODE(DCM_Warning_StackWaterMarkHighTaskCanTx_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_msg->DCM_Warning_StackWaterMarkHighTaskCanTx_value = DCM_Warning_StackWaterMarkHighTaskCanTx_val;
    
    // Unpack 1-bit signal DCM_Warning_WatchdogTimeout from payload (at bit 5 to bit 6).
    uint32_t DCM_Warning_WatchdogTimeout_raw = 0;
    DCM_Warning_WatchdogTimeout_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool DCM_Warning_WatchdogTimeout_val = CAN_DECODE(DCM_Warning_WatchdogTimeout_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_msg->DCM_Warning_WatchdogTimeout_value = DCM_Warning_WatchdogTimeout_val;
    
    // Unpack 1-bit signal DCM_Warning_TxOverflow from payload (at bit 6 to bit 7).
    uint32_t DCM_Warning_TxOverflow_raw = 0;
    DCM_Warning_TxOverflow_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool DCM_Warning_TxOverflow_val = CAN_DECODE(DCM_Warning_TxOverflow_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_msg->DCM_Warning_TxOverflow_value = DCM_Warning_TxOverflow_val;
    
    // Unpack 1-bit signal DCM_Warning_RxOverflow from payload (at bit 7 to bit 8).
    uint32_t DCM_Warning_RxOverflow_raw = 0;
    DCM_Warning_RxOverflow_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool DCM_Warning_RxOverflow_val = CAN_DECODE(DCM_Warning_RxOverflow_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_msg->DCM_Warning_RxOverflow_value = DCM_Warning_RxOverflow_val;
    
}

void App_CanUtils_DCM_Faults_Unpack(const uint8_t* const in_data, DCM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DCM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DCM_Fault_MissingHeartbeat from payload (at bit 0 to bit 1).
    uint32_t DCM_Fault_MissingHeartbeat_raw = 0;
    DCM_Fault_MissingHeartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DCM_Fault_MissingHeartbeat_val = CAN_DECODE(DCM_Fault_MissingHeartbeat_raw, CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_msg->DCM_Fault_MissingHeartbeat_value = DCM_Fault_MissingHeartbeat_val;
    
    // Unpack 1-bit signal DCM_Fault_LeftInverterFault from payload (at bit 1 to bit 2).
    uint32_t DCM_Fault_LeftInverterFault_raw = 0;
    DCM_Fault_LeftInverterFault_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool DCM_Fault_LeftInverterFault_val = CAN_DECODE(DCM_Fault_LeftInverterFault_raw, CANSIG_DCM_FAULTS_DCM_FAULT_LEFT_INVERTER_FAULT_SCALE, CANSIG_DCM_FAULTS_DCM_FAULT_LEFT_INVERTER_FAULT_OFFSET, uint32_t);
    out_msg->DCM_Fault_LeftInverterFault_value = DCM_Fault_LeftInverterFault_val;
    
    // Unpack 1-bit signal DCM_Fault_RightInverterFault from payload (at bit 2 to bit 3).
    uint32_t DCM_Fault_RightInverterFault_raw = 0;
    DCM_Fault_RightInverterFault_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool DCM_Fault_RightInverterFault_val = CAN_DECODE(DCM_Fault_RightInverterFault_raw, CANSIG_DCM_FAULTS_DCM_FAULT_RIGHT_INVERTER_FAULT_SCALE, CANSIG_DCM_FAULTS_DCM_FAULT_RIGHT_INVERTER_FAULT_OFFSET, uint32_t);
    out_msg->DCM_Fault_RightInverterFault_value = DCM_Fault_RightInverterFault_val;
    
}

void App_CanUtils_INVR_Temperatures1_Unpack(const uint8_t* const in_data, INVR_Temperatures1_Signals* const out_msg)
{
    // Unpack 8-byte payload for message INVR_Temperatures1.
    // |DDDDDDDD|DDDDDDDD|CCCCCCCC|CCCCCCCC|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 16-bit signal INVR_ModuleATemperature from payload (at bit 0 to bit 16).
    uint32_t INVR_ModuleATemperature_raw = 0;
    INVR_ModuleATemperature_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    INVR_ModuleATemperature_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    const int INVR_ModuleATemperature_signed = SIGNED_DECODE(INVR_ModuleATemperature_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float INVR_ModuleATemperature_val = CAN_DECODE(INVR_ModuleATemperature_signed, CANSIG_INVR_TEMPERATURES1_INVR_MODULE_A_TEMPERATURE_SCALE, CANSIG_INVR_TEMPERATURES1_INVR_MODULE_A_TEMPERATURE_OFFSET, float);
    out_msg->INVR_ModuleATemperature_value = INVR_ModuleATemperature_val;
    
    // Unpack 16-bit signal INVR_ModuleBTemperature from payload (at bit 16 to bit 32).
    uint32_t INVR_ModuleBTemperature_raw = 0;
    INVR_ModuleBTemperature_raw |= unpackShiftRight(in_data[2], 0, 0xff);   // Unpacks bits ######## of msg byte 2
    INVR_ModuleBTemperature_raw |= unpackShiftLeft(in_data[3], 8, 0xff);   // Unpacks bits ######## of msg byte 3
    const int INVR_ModuleBTemperature_signed = SIGNED_DECODE(INVR_ModuleBTemperature_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float INVR_ModuleBTemperature_val = CAN_DECODE(INVR_ModuleBTemperature_signed, CANSIG_INVR_TEMPERATURES1_INVR_MODULE_B_TEMPERATURE_SCALE, CANSIG_INVR_TEMPERATURES1_INVR_MODULE_B_TEMPERATURE_OFFSET, float);
    out_msg->INVR_ModuleBTemperature_value = INVR_ModuleBTemperature_val;
    
    // Unpack 16-bit signal INVR_ModuleCTemperature from payload (at bit 32 to bit 48).
    uint32_t INVR_ModuleCTemperature_raw = 0;
    INVR_ModuleCTemperature_raw |= unpackShiftRight(in_data[4], 0, 0xff);   // Unpacks bits ######## of msg byte 4
    INVR_ModuleCTemperature_raw |= unpackShiftLeft(in_data[5], 8, 0xff);   // Unpacks bits ######## of msg byte 5
    const int INVR_ModuleCTemperature_signed = SIGNED_DECODE(INVR_ModuleCTemperature_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float INVR_ModuleCTemperature_val = CAN_DECODE(INVR_ModuleCTemperature_signed, CANSIG_INVR_TEMPERATURES1_INVR_MODULE_C_TEMPERATURE_SCALE, CANSIG_INVR_TEMPERATURES1_INVR_MODULE_C_TEMPERATURE_OFFSET, float);
    out_msg->INVR_ModuleCTemperature_value = INVR_ModuleCTemperature_val;
    
    // Unpack 16-bit signal INVR_GateDriverBoardTemperature from payload (at bit 48 to bit 64).
    uint32_t INVR_GateDriverBoardTemperature_raw = 0;
    INVR_GateDriverBoardTemperature_raw |= unpackShiftRight(in_data[6], 0, 0xff);   // Unpacks bits ######## of msg byte 6
    INVR_GateDriverBoardTemperature_raw |= unpackShiftLeft(in_data[7], 8, 0xff);   // Unpacks bits ######## of msg byte 7
    const int INVR_GateDriverBoardTemperature_signed = SIGNED_DECODE(INVR_GateDriverBoardTemperature_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float INVR_GateDriverBoardTemperature_val = CAN_DECODE(INVR_GateDriverBoardTemperature_signed, CANSIG_INVR_TEMPERATURES1_INVR_GATE_DRIVER_BOARD_TEMPERATURE_SCALE, CANSIG_INVR_TEMPERATURES1_INVR_GATE_DRIVER_BOARD_TEMPERATURE_OFFSET, float);
    out_msg->INVR_GateDriverBoardTemperature_value = INVR_GateDriverBoardTemperature_val;
    
}

void App_CanUtils_INVR_MotorPositionInfo_Unpack(const uint8_t* const in_data, INVR_MotorPositionInfo_Signals* const out_msg)
{
    // Unpack 8-byte payload for message INVR_MotorPositionInfo.
    // |DDDDDDDD|DDDDDDDD|CCCCCCCC|CCCCCCCC|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 16-bit signal INVR_MotorAngle from payload (at bit 0 to bit 16).
    uint32_t INVR_MotorAngle_raw = 0;
    INVR_MotorAngle_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    INVR_MotorAngle_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    const int INVR_MotorAngle_signed = SIGNED_DECODE(INVR_MotorAngle_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float INVR_MotorAngle_val = CAN_DECODE(INVR_MotorAngle_signed, CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_SCALE, CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_ANGLE_OFFSET, float);
    out_msg->INVR_MotorAngle_value = INVR_MotorAngle_val;
    
    // Unpack 16-bit signal INVR_MotorSpeed from payload (at bit 16 to bit 32).
    uint32_t INVR_MotorSpeed_raw = 0;
    INVR_MotorSpeed_raw |= unpackShiftRight(in_data[2], 0, 0xff);   // Unpacks bits ######## of msg byte 2
    INVR_MotorSpeed_raw |= unpackShiftLeft(in_data[3], 8, 0xff);   // Unpacks bits ######## of msg byte 3
    const int INVR_MotorSpeed_signed = SIGNED_DECODE(INVR_MotorSpeed_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    int INVR_MotorSpeed_val = CAN_DECODE(INVR_MotorSpeed_signed, CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_SCALE, CANSIG_INVR_MOTOR_POSITION_INFO_INVR_MOTOR_SPEED_OFFSET, int);
    out_msg->INVR_MotorSpeed_value = INVR_MotorSpeed_val;
    
    // Unpack 16-bit signal INVR_ElectricalOutputFrequency from payload (at bit 32 to bit 48).
    uint32_t INVR_ElectricalOutputFrequency_raw = 0;
    INVR_ElectricalOutputFrequency_raw |= unpackShiftRight(in_data[4], 0, 0xff);   // Unpacks bits ######## of msg byte 4
    INVR_ElectricalOutputFrequency_raw |= unpackShiftLeft(in_data[5], 8, 0xff);   // Unpacks bits ######## of msg byte 5
    const int INVR_ElectricalOutputFrequency_signed = SIGNED_DECODE(INVR_ElectricalOutputFrequency_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float INVR_ElectricalOutputFrequency_val = CAN_DECODE(INVR_ElectricalOutputFrequency_signed, CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_SCALE, CANSIG_INVR_MOTOR_POSITION_INFO_INVR_ELECTRICAL_OUTPUT_FREQUENCY_OFFSET, float);
    out_msg->INVR_ElectricalOutputFrequency_value = INVR_ElectricalOutputFrequency_val;
    
    // Unpack 16-bit signal INVR_DeltaResolverFiltered from payload (at bit 48 to bit 64).
    uint32_t INVR_DeltaResolverFiltered_raw = 0;
    INVR_DeltaResolverFiltered_raw |= unpackShiftRight(in_data[6], 0, 0xff);   // Unpacks bits ######## of msg byte 6
    INVR_DeltaResolverFiltered_raw |= unpackShiftLeft(in_data[7], 8, 0xff);   // Unpacks bits ######## of msg byte 7
    const int INVR_DeltaResolverFiltered_signed = SIGNED_DECODE(INVR_DeltaResolverFiltered_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float INVR_DeltaResolverFiltered_val = CAN_DECODE(INVR_DeltaResolverFiltered_signed, CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_SCALE, CANSIG_INVR_MOTOR_POSITION_INFO_INVR_DELTA_RESOLVER_FILTERED_OFFSET, float);
    out_msg->INVR_DeltaResolverFiltered_value = INVR_DeltaResolverFiltered_val;
    
}

void App_CanUtils_BMS_Vitals_Unpack(const uint8_t* const in_data, BMS_Vitals_Signals* const out_msg)
{
    // Unpack 1-byte payload for message BMS_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____BBBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal BMS_Heartbeat from payload (at bit 0 to bit 1).
    uint32_t BMS_Heartbeat_raw = 0;
    BMS_Heartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool BMS_Heartbeat_val = CAN_DECODE(BMS_Heartbeat_raw, CANSIG_BMS_VITALS_BMS_HEARTBEAT_SCALE, CANSIG_BMS_VITALS_BMS_HEARTBEAT_OFFSET, uint32_t);
    out_msg->BMS_Heartbeat_value = BMS_Heartbeat_val;
    
    // Unpack 3-bit signal BMS_State from payload (at bit 1 to bit 4).
    uint32_t BMS_State_raw = 0;
    BMS_State_raw |= unpackShiftRight(in_data[0], 1, 0xe);   // Unpacks bits ____###_ of msg byte 0
    BmsState BMS_State_val = CAN_DECODE(BMS_State_raw, CANSIG_BMS_VITALS_BMS_STATE_SCALE, CANSIG_BMS_VITALS_BMS_STATE_OFFSET, uint32_t);
    out_msg->BMS_State_value = BMS_State_val;
    
}

void App_CanUtils_BMS_OkStatuses_Unpack(const uint8_t* const in_data, BMS_OkStatuses_Signals* const out_msg)
{
    // Unpack 1-byte payload for message BMS_OkStatuses.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__FEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal BMS_BmsOk from payload (at bit 0 to bit 1).
    uint32_t BMS_BmsOk_raw = 0;
    BMS_BmsOk_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool BMS_BmsOk_val = CAN_DECODE(BMS_BmsOk_raw, CANSIG_BMS_OK_STATUSES_BMS_BMS_OK_SCALE, CANSIG_BMS_OK_STATUSES_BMS_BMS_OK_OFFSET, uint32_t);
    out_msg->BMS_BmsOk_value = BMS_BmsOk_val;
    
    // Unpack 1-bit signal BMS_ImdOk from payload (at bit 1 to bit 2).
    uint32_t BMS_ImdOk_raw = 0;
    BMS_ImdOk_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool BMS_ImdOk_val = CAN_DECODE(BMS_ImdOk_raw, CANSIG_BMS_OK_STATUSES_BMS_IMD_OK_SCALE, CANSIG_BMS_OK_STATUSES_BMS_IMD_OK_OFFSET, uint32_t);
    out_msg->BMS_ImdOk_value = BMS_ImdOk_val;
    
    // Unpack 1-bit signal BMS_BspdOk from payload (at bit 2 to bit 3).
    uint32_t BMS_BspdOk_raw = 0;
    BMS_BspdOk_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool BMS_BspdOk_val = CAN_DECODE(BMS_BspdOk_raw, CANSIG_BMS_OK_STATUSES_BMS_BSPD_OK_SCALE, CANSIG_BMS_OK_STATUSES_BMS_BSPD_OK_OFFSET, uint32_t);
    out_msg->BMS_BspdOk_value = BMS_BspdOk_val;
    
    // Unpack 1-bit signal BMS_BmsLatchedFault from payload (at bit 3 to bit 4).
    uint32_t BMS_BmsLatchedFault_raw = 0;
    BMS_BmsLatchedFault_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool BMS_BmsLatchedFault_val = CAN_DECODE(BMS_BmsLatchedFault_raw, CANSIG_BMS_OK_STATUSES_BMS_BMS_LATCHED_FAULT_SCALE, CANSIG_BMS_OK_STATUSES_BMS_BMS_LATCHED_FAULT_OFFSET, uint32_t);
    out_msg->BMS_BmsLatchedFault_value = BMS_BmsLatchedFault_val;
    
    // Unpack 1-bit signal BMS_ImdLatchedFault from payload (at bit 4 to bit 5).
    uint32_t BMS_ImdLatchedFault_raw = 0;
    BMS_ImdLatchedFault_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool BMS_ImdLatchedFault_val = CAN_DECODE(BMS_ImdLatchedFault_raw, CANSIG_BMS_OK_STATUSES_BMS_IMD_LATCHED_FAULT_SCALE, CANSIG_BMS_OK_STATUSES_BMS_IMD_LATCHED_FAULT_OFFSET, uint32_t);
    out_msg->BMS_ImdLatchedFault_value = BMS_ImdLatchedFault_val;
    
    // Unpack 1-bit signal BMS_BspdLatchedFault from payload (at bit 5 to bit 6).
    uint32_t BMS_BspdLatchedFault_raw = 0;
    BMS_BspdLatchedFault_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool BMS_BspdLatchedFault_val = CAN_DECODE(BMS_BspdLatchedFault_raw, CANSIG_BMS_OK_STATUSES_BMS_BSPD_LATCHED_FAULT_SCALE, CANSIG_BMS_OK_STATUSES_BMS_BSPD_LATCHED_FAULT_OFFSET, uint32_t);
    out_msg->BMS_BspdLatchedFault_value = BMS_BspdLatchedFault_val;
    
}

void App_CanUtils_BMS_Contactors_Unpack(const uint8_t* const in_data, BMS_Contactors_Signals* const out_msg)
{
    // Unpack 1-byte payload for message BMS_Contactors.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal BMS_AirPositive from payload (at bit 0 to bit 1).
    uint32_t BMS_AirPositive_raw = 0;
    BMS_AirPositive_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    ContactorState BMS_AirPositive_val = CAN_DECODE(BMS_AirPositive_raw, CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_SCALE, CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_OFFSET, uint32_t);
    out_msg->BMS_AirPositive_value = BMS_AirPositive_val;
    
    // Unpack 1-bit signal BMS_AirNegative from payload (at bit 1 to bit 2).
    uint32_t BMS_AirNegative_raw = 0;
    BMS_AirNegative_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    ContactorState BMS_AirNegative_val = CAN_DECODE(BMS_AirNegative_raw, CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_SCALE, CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_OFFSET, uint32_t);
    out_msg->BMS_AirNegative_value = BMS_AirNegative_val;
    
    // Unpack 1-bit signal BMS_PrechargeRelay from payload (at bit 2 to bit 3).
    uint32_t BMS_PrechargeRelay_raw = 0;
    BMS_PrechargeRelay_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    ContactorState BMS_PrechargeRelay_val = CAN_DECODE(BMS_PrechargeRelay_raw, CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_SCALE, CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_OFFSET, uint32_t);
    out_msg->BMS_PrechargeRelay_value = BMS_PrechargeRelay_val;
    
}

void App_CanUtils_BMS_CellVoltages_Unpack(const uint8_t* const in_data, BMS_CellVoltages_Signals* const out_msg)
{
    // Unpack 4-byte payload for message BMS_CellVoltages.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 16-bit signal BMS_MinCellVoltage from payload (at bit 0 to bit 16).
    uint32_t BMS_MinCellVoltage_raw = 0;
    BMS_MinCellVoltage_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    BMS_MinCellVoltage_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    float BMS_MinCellVoltage_val = CAN_DECODE(BMS_MinCellVoltage_raw, CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_SCALE, CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_OFFSET, float);
    out_msg->BMS_MinCellVoltage_value = BMS_MinCellVoltage_val;
    
    // Unpack 16-bit signal BMS_MaxCellVoltage from payload (at bit 16 to bit 32).
    uint32_t BMS_MaxCellVoltage_raw = 0;
    BMS_MaxCellVoltage_raw |= unpackShiftRight(in_data[2], 0, 0xff);   // Unpacks bits ######## of msg byte 2
    BMS_MaxCellVoltage_raw |= unpackShiftLeft(in_data[3], 8, 0xff);   // Unpacks bits ######## of msg byte 3
    float BMS_MaxCellVoltage_val = CAN_DECODE(BMS_MaxCellVoltage_raw, CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_SCALE, CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_OFFSET, float);
    out_msg->BMS_MaxCellVoltage_value = BMS_MaxCellVoltage_val;
    
}

void App_CanUtils_BMS_TractiveSystem_Unpack(const uint8_t* const in_data, BMS_TractiveSystem_Signals* const out_msg)
{
    // Unpack 5-byte payload for message BMS_TractiveSystem.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|____BBBB|BBBBBBBB|BAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 23-bit signal BMS_TractiveSystemVoltage from payload (at bit 0 to bit 23).
    uint32_t BMS_TractiveSystemVoltage_raw = 0;
    BMS_TractiveSystemVoltage_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    BMS_TractiveSystemVoltage_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    BMS_TractiveSystemVoltage_raw |= unpackShiftLeft(in_data[2], 16, 0x7f);   // Unpacks bits _####### of msg byte 2
    float BMS_TractiveSystemVoltage_val = CAN_DECODE(BMS_TractiveSystemVoltage_raw, CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_OFFSET, float);
    out_msg->BMS_TractiveSystemVoltage_value = BMS_TractiveSystemVoltage_val;
    
    // Unpack 13-bit signal BMS_TractiveSystemCurrent from payload (at bit 23 to bit 36).
    uint32_t BMS_TractiveSystemCurrent_raw = 0;
    BMS_TractiveSystemCurrent_raw |= unpackShiftRight(in_data[2], 7, 0x80);   // Unpacks bits #_______ of msg byte 2
    BMS_TractiveSystemCurrent_raw |= unpackShiftLeft(in_data[3], 1, 0xff);   // Unpacks bits ######## of msg byte 3
    BMS_TractiveSystemCurrent_raw |= unpackShiftLeft(in_data[4], 9, 0xf);   // Unpacks bits ____#### of msg byte 4
    float BMS_TractiveSystemCurrent_val = CAN_DECODE(BMS_TractiveSystemCurrent_raw, CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_OFFSET, float);
    out_msg->BMS_TractiveSystemCurrent_value = BMS_TractiveSystemCurrent_val;
    
}

void App_CanUtils_BMS_Warnings_Unpack(const uint8_t* const in_data, BMS_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message BMS_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal BMS_Warning_StackWaterMarkHighTask1Hz from payload (at bit 0 to bit 1).
    uint32_t BMS_Warning_StackWaterMarkHighTask1Hz_raw = 0;
    BMS_Warning_StackWaterMarkHighTask1Hz_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool BMS_Warning_StackWaterMarkHighTask1Hz_val = CAN_DECODE(BMS_Warning_StackWaterMarkHighTask1Hz_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_msg->BMS_Warning_StackWaterMarkHighTask1Hz_value = BMS_Warning_StackWaterMarkHighTask1Hz_val;
    
    // Unpack 1-bit signal BMS_Warning_StackWaterMarkHighTask100Hz from payload (at bit 1 to bit 2).
    uint32_t BMS_Warning_StackWaterMarkHighTask100Hz_raw = 0;
    BMS_Warning_StackWaterMarkHighTask100Hz_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool BMS_Warning_StackWaterMarkHighTask100Hz_val = CAN_DECODE(BMS_Warning_StackWaterMarkHighTask100Hz_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_msg->BMS_Warning_StackWaterMarkHighTask100Hz_value = BMS_Warning_StackWaterMarkHighTask100Hz_val;
    
    // Unpack 1-bit signal BMS_Warning_StackWaterMarkHighTask1kHz from payload (at bit 2 to bit 3).
    uint32_t BMS_Warning_StackWaterMarkHighTask1kHz_raw = 0;
    BMS_Warning_StackWaterMarkHighTask1kHz_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool BMS_Warning_StackWaterMarkHighTask1kHz_val = CAN_DECODE(BMS_Warning_StackWaterMarkHighTask1kHz_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_msg->BMS_Warning_StackWaterMarkHighTask1kHz_value = BMS_Warning_StackWaterMarkHighTask1kHz_val;
    
    // Unpack 1-bit signal BMS_Warning_StackWaterMarkHighTaskCanRx from payload (at bit 3 to bit 4).
    uint32_t BMS_Warning_StackWaterMarkHighTaskCanRx_raw = 0;
    BMS_Warning_StackWaterMarkHighTaskCanRx_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool BMS_Warning_StackWaterMarkHighTaskCanRx_val = CAN_DECODE(BMS_Warning_StackWaterMarkHighTaskCanRx_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_msg->BMS_Warning_StackWaterMarkHighTaskCanRx_value = BMS_Warning_StackWaterMarkHighTaskCanRx_val;
    
    // Unpack 1-bit signal BMS_Warning_StackWaterMarkHighTaskCanTx from payload (at bit 4 to bit 5).
    uint32_t BMS_Warning_StackWaterMarkHighTaskCanTx_raw = 0;
    BMS_Warning_StackWaterMarkHighTaskCanTx_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool BMS_Warning_StackWaterMarkHighTaskCanTx_val = CAN_DECODE(BMS_Warning_StackWaterMarkHighTaskCanTx_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_msg->BMS_Warning_StackWaterMarkHighTaskCanTx_value = BMS_Warning_StackWaterMarkHighTaskCanTx_val;
    
    // Unpack 1-bit signal BMS_Warning_WatchdogTimeout from payload (at bit 5 to bit 6).
    uint32_t BMS_Warning_WatchdogTimeout_raw = 0;
    BMS_Warning_WatchdogTimeout_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool BMS_Warning_WatchdogTimeout_val = CAN_DECODE(BMS_Warning_WatchdogTimeout_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_msg->BMS_Warning_WatchdogTimeout_value = BMS_Warning_WatchdogTimeout_val;
    
    // Unpack 1-bit signal BMS_Warning_TxOverflow from payload (at bit 6 to bit 7).
    uint32_t BMS_Warning_TxOverflow_raw = 0;
    BMS_Warning_TxOverflow_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool BMS_Warning_TxOverflow_val = CAN_DECODE(BMS_Warning_TxOverflow_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_msg->BMS_Warning_TxOverflow_value = BMS_Warning_TxOverflow_val;
    
    // Unpack 1-bit signal BMS_Warning_RxOverflow from payload (at bit 7 to bit 8).
    uint32_t BMS_Warning_RxOverflow_raw = 0;
    BMS_Warning_RxOverflow_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool BMS_Warning_RxOverflow_val = CAN_DECODE(BMS_Warning_RxOverflow_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_msg->BMS_Warning_RxOverflow_value = BMS_Warning_RxOverflow_val;
    
}

void App_CanUtils_BMS_Faults_Unpack(const uint8_t* const in_data, BMS_Faults_Signals* const out_msg)
{
    // Unpack 2-byte payload for message BMS_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____LKJI|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal BMS_Fault_MissingHeartbeat from payload (at bit 0 to bit 1).
    uint32_t BMS_Fault_MissingHeartbeat_raw = 0;
    BMS_Fault_MissingHeartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool BMS_Fault_MissingHeartbeat_val = CAN_DECODE(BMS_Fault_MissingHeartbeat_raw, CANSIG_BMS_FAULTS_BMS_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_msg->BMS_Fault_MissingHeartbeat_value = BMS_Fault_MissingHeartbeat_val;
    
    // Unpack 1-bit signal BMS_Fault_StateMachine from payload (at bit 1 to bit 2).
    uint32_t BMS_Fault_StateMachine_raw = 0;
    BMS_Fault_StateMachine_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool BMS_Fault_StateMachine_val = CAN_DECODE(BMS_Fault_StateMachine_raw, CANSIG_BMS_FAULTS_BMS_FAULT_STATE_MACHINE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_STATE_MACHINE_OFFSET, uint32_t);
    out_msg->BMS_Fault_StateMachine_value = BMS_Fault_StateMachine_val;
    
    // Unpack 1-bit signal BMS_Fault_CellUndervoltage from payload (at bit 2 to bit 3).
    uint32_t BMS_Fault_CellUndervoltage_raw = 0;
    BMS_Fault_CellUndervoltage_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool BMS_Fault_CellUndervoltage_val = CAN_DECODE(BMS_Fault_CellUndervoltage_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_OFFSET, uint32_t);
    out_msg->BMS_Fault_CellUndervoltage_value = BMS_Fault_CellUndervoltage_val;
    
    // Unpack 1-bit signal BMS_Fault_CellOvervoltage from payload (at bit 3 to bit 4).
    uint32_t BMS_Fault_CellOvervoltage_raw = 0;
    BMS_Fault_CellOvervoltage_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool BMS_Fault_CellOvervoltage_val = CAN_DECODE(BMS_Fault_CellOvervoltage_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_OFFSET, uint32_t);
    out_msg->BMS_Fault_CellOvervoltage_value = BMS_Fault_CellOvervoltage_val;
    
    // Unpack 1-bit signal BMS_Fault_ModuleCommunicationError from payload (at bit 4 to bit 5).
    uint32_t BMS_Fault_ModuleCommunicationError_raw = 0;
    BMS_Fault_ModuleCommunicationError_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool BMS_Fault_ModuleCommunicationError_val = CAN_DECODE(BMS_Fault_ModuleCommunicationError_raw, CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_OFFSET, uint32_t);
    out_msg->BMS_Fault_ModuleCommunicationError_value = BMS_Fault_ModuleCommunicationError_val;
    
    // Unpack 1-bit signal BMS_Fault_CellUndertemp from payload (at bit 5 to bit 6).
    uint32_t BMS_Fault_CellUndertemp_raw = 0;
    BMS_Fault_CellUndertemp_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool BMS_Fault_CellUndertemp_val = CAN_DECODE(BMS_Fault_CellUndertemp_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_OFFSET, uint32_t);
    out_msg->BMS_Fault_CellUndertemp_value = BMS_Fault_CellUndertemp_val;
    
    // Unpack 1-bit signal BMS_Fault_CellOvertemp from payload (at bit 6 to bit 7).
    uint32_t BMS_Fault_CellOvertemp_raw = 0;
    BMS_Fault_CellOvertemp_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool BMS_Fault_CellOvertemp_val = CAN_DECODE(BMS_Fault_CellOvertemp_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_OFFSET, uint32_t);
    out_msg->BMS_Fault_CellOvertemp_value = BMS_Fault_CellOvertemp_val;
    
    // Unpack 1-bit signal BMS_Fault_Charger from payload (at bit 7 to bit 8).
    uint32_t BMS_Fault_Charger_raw = 0;
    BMS_Fault_Charger_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool BMS_Fault_Charger_val = CAN_DECODE(BMS_Fault_Charger_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_OFFSET, uint32_t);
    out_msg->BMS_Fault_Charger_value = BMS_Fault_Charger_val;
    
    // Unpack 1-bit signal BMS_Fault_ChargerDisconnectedDuringCharge from payload (at bit 8 to bit 9).
    uint32_t BMS_Fault_ChargerDisconnectedDuringCharge_raw = 0;
    BMS_Fault_ChargerDisconnectedDuringCharge_raw |= unpackShiftRight(in_data[1], 0, 0x1);   // Unpacks bits _______# of msg byte 1
    bool BMS_Fault_ChargerDisconnectedDuringCharge_val = CAN_DECODE(BMS_Fault_ChargerDisconnectedDuringCharge_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_OFFSET, uint32_t);
    out_msg->BMS_Fault_ChargerDisconnectedDuringCharge_value = BMS_Fault_ChargerDisconnectedDuringCharge_val;
    
    // Unpack 1-bit signal BMS_Fault_ChargerExternalShutdown from payload (at bit 9 to bit 10).
    uint32_t BMS_Fault_ChargerExternalShutdown_raw = 0;
    BMS_Fault_ChargerExternalShutdown_raw |= unpackShiftRight(in_data[1], 1, 0x2);   // Unpacks bits ______#_ of msg byte 1
    bool BMS_Fault_ChargerExternalShutdown_val = CAN_DECODE(BMS_Fault_ChargerExternalShutdown_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_OFFSET, uint32_t);
    out_msg->BMS_Fault_ChargerExternalShutdown_value = BMS_Fault_ChargerExternalShutdown_val;
    
    // Unpack 1-bit signal BMS_Fault_TractiveSystemOvercurrent from payload (at bit 10 to bit 11).
    uint32_t BMS_Fault_TractiveSystemOvercurrent_raw = 0;
    BMS_Fault_TractiveSystemOvercurrent_raw |= unpackShiftRight(in_data[1], 2, 0x4);   // Unpacks bits _____#__ of msg byte 1
    bool BMS_Fault_TractiveSystemOvercurrent_val = CAN_DECODE(BMS_Fault_TractiveSystemOvercurrent_raw, CANSIG_BMS_FAULTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_OFFSET, uint32_t);
    out_msg->BMS_Fault_TractiveSystemOvercurrent_value = BMS_Fault_TractiveSystemOvercurrent_val;
    
    // Unpack 1-bit signal BMS_Fault_PrechargeFailure from payload (at bit 11 to bit 12).
    uint32_t BMS_Fault_PrechargeFailure_raw = 0;
    BMS_Fault_PrechargeFailure_raw |= unpackShiftRight(in_data[1], 3, 0x8);   // Unpacks bits ____#___ of msg byte 1
    bool BMS_Fault_PrechargeFailure_val = CAN_DECODE(BMS_Fault_PrechargeFailure_raw, CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_FAILURE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_FAILURE_OFFSET, uint32_t);
    out_msg->BMS_Fault_PrechargeFailure_value = BMS_Fault_PrechargeFailure_val;
    
}

void App_CanUtils_FSM_Warnings_Unpack(const uint8_t* const in_data, FSM_Warnings_Signals* const out_msg)
{
    // Unpack 3-byte payload for message FSM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______Q|PONMLKJI|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_Warning_StackWaterMarkHighTask1Hz from payload (at bit 0 to bit 1).
    uint32_t FSM_Warning_StackWaterMarkHighTask1Hz_raw = 0;
    FSM_Warning_StackWaterMarkHighTask1Hz_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_Warning_StackWaterMarkHighTask1Hz_val = CAN_DECODE(FSM_Warning_StackWaterMarkHighTask1Hz_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_msg->FSM_Warning_StackWaterMarkHighTask1Hz_value = FSM_Warning_StackWaterMarkHighTask1Hz_val;
    
    // Unpack 1-bit signal FSM_Warning_StackWaterMarkHighTask100Hz from payload (at bit 1 to bit 2).
    uint32_t FSM_Warning_StackWaterMarkHighTask100Hz_raw = 0;
    FSM_Warning_StackWaterMarkHighTask100Hz_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool FSM_Warning_StackWaterMarkHighTask100Hz_val = CAN_DECODE(FSM_Warning_StackWaterMarkHighTask100Hz_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_msg->FSM_Warning_StackWaterMarkHighTask100Hz_value = FSM_Warning_StackWaterMarkHighTask100Hz_val;
    
    // Unpack 1-bit signal FSM_Warning_StackWaterMarkHighTask1kHz from payload (at bit 2 to bit 3).
    uint32_t FSM_Warning_StackWaterMarkHighTask1kHz_raw = 0;
    FSM_Warning_StackWaterMarkHighTask1kHz_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool FSM_Warning_StackWaterMarkHighTask1kHz_val = CAN_DECODE(FSM_Warning_StackWaterMarkHighTask1kHz_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_msg->FSM_Warning_StackWaterMarkHighTask1kHz_value = FSM_Warning_StackWaterMarkHighTask1kHz_val;
    
    // Unpack 1-bit signal FSM_Warning_StackWaterMarkHighTaskCanRx from payload (at bit 3 to bit 4).
    uint32_t FSM_Warning_StackWaterMarkHighTaskCanRx_raw = 0;
    FSM_Warning_StackWaterMarkHighTaskCanRx_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool FSM_Warning_StackWaterMarkHighTaskCanRx_val = CAN_DECODE(FSM_Warning_StackWaterMarkHighTaskCanRx_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_msg->FSM_Warning_StackWaterMarkHighTaskCanRx_value = FSM_Warning_StackWaterMarkHighTaskCanRx_val;
    
    // Unpack 1-bit signal FSM_Warning_StackWaterMarkHighTaskCanTx from payload (at bit 4 to bit 5).
    uint32_t FSM_Warning_StackWaterMarkHighTaskCanTx_raw = 0;
    FSM_Warning_StackWaterMarkHighTaskCanTx_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool FSM_Warning_StackWaterMarkHighTaskCanTx_val = CAN_DECODE(FSM_Warning_StackWaterMarkHighTaskCanTx_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_msg->FSM_Warning_StackWaterMarkHighTaskCanTx_value = FSM_Warning_StackWaterMarkHighTaskCanTx_val;
    
    // Unpack 1-bit signal FSM_Warning_WatchdogTimeout from payload (at bit 5 to bit 6).
    uint32_t FSM_Warning_WatchdogTimeout_raw = 0;
    FSM_Warning_WatchdogTimeout_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool FSM_Warning_WatchdogTimeout_val = CAN_DECODE(FSM_Warning_WatchdogTimeout_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_msg->FSM_Warning_WatchdogTimeout_value = FSM_Warning_WatchdogTimeout_val;
    
    // Unpack 1-bit signal FSM_Warning_TxOverflow from payload (at bit 6 to bit 7).
    uint32_t FSM_Warning_TxOverflow_raw = 0;
    FSM_Warning_TxOverflow_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool FSM_Warning_TxOverflow_val = CAN_DECODE(FSM_Warning_TxOverflow_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_TX_OVERFLOW_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_msg->FSM_Warning_TxOverflow_value = FSM_Warning_TxOverflow_val;
    
    // Unpack 1-bit signal FSM_Warning_RxOverflow from payload (at bit 7 to bit 8).
    uint32_t FSM_Warning_RxOverflow_raw = 0;
    FSM_Warning_RxOverflow_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool FSM_Warning_RxOverflow_val = CAN_DECODE(FSM_Warning_RxOverflow_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_RX_OVERFLOW_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_msg->FSM_Warning_RxOverflow_value = FSM_Warning_RxOverflow_val;
    
    // Unpack 1-bit signal FSM_Warning_LeftWheelSpeedOutOfRange from payload (at bit 8 to bit 9).
    uint32_t FSM_Warning_LeftWheelSpeedOutOfRange_raw = 0;
    FSM_Warning_LeftWheelSpeedOutOfRange_raw |= unpackShiftRight(in_data[1], 0, 0x1);   // Unpacks bits _______# of msg byte 1
    bool FSM_Warning_LeftWheelSpeedOutOfRange_val = CAN_DECODE(FSM_Warning_LeftWheelSpeedOutOfRange_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_Warning_LeftWheelSpeedOutOfRange_value = FSM_Warning_LeftWheelSpeedOutOfRange_val;
    
    // Unpack 1-bit signal FSM_Warning_RightWheelSpeedOutOfRange from payload (at bit 9 to bit 10).
    uint32_t FSM_Warning_RightWheelSpeedOutOfRange_raw = 0;
    FSM_Warning_RightWheelSpeedOutOfRange_raw |= unpackShiftRight(in_data[1], 1, 0x2);   // Unpacks bits ______#_ of msg byte 1
    bool FSM_Warning_RightWheelSpeedOutOfRange_val = CAN_DECODE(FSM_Warning_RightWheelSpeedOutOfRange_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_Warning_RightWheelSpeedOutOfRange_value = FSM_Warning_RightWheelSpeedOutOfRange_val;
    
    // Unpack 1-bit signal FSM_Warning_SteeringAngleOCSC from payload (at bit 10 to bit 11).
    uint32_t FSM_Warning_SteeringAngleOCSC_raw = 0;
    FSM_Warning_SteeringAngleOCSC_raw |= unpackShiftRight(in_data[1], 2, 0x4);   // Unpacks bits _____#__ of msg byte 1
    bool FSM_Warning_SteeringAngleOCSC_val = CAN_DECODE(FSM_Warning_SteeringAngleOCSC_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OCSC_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OCSC_OFFSET, uint32_t);
    out_msg->FSM_Warning_SteeringAngleOCSC_value = FSM_Warning_SteeringAngleOCSC_val;
    
    // Unpack 1-bit signal FSM_Warning_SteeringAngleOutOfRange from payload (at bit 11 to bit 12).
    uint32_t FSM_Warning_SteeringAngleOutOfRange_raw = 0;
    FSM_Warning_SteeringAngleOutOfRange_raw |= unpackShiftRight(in_data[1], 3, 0x8);   // Unpacks bits ____#___ of msg byte 1
    bool FSM_Warning_SteeringAngleOutOfRange_val = CAN_DECODE(FSM_Warning_SteeringAngleOutOfRange_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_Warning_SteeringAngleOutOfRange_value = FSM_Warning_SteeringAngleOutOfRange_val;
    
    // Unpack 1-bit signal FSM_Warning_FrontBrakePressureOutOfRange from payload (at bit 12 to bit 13).
    uint32_t FSM_Warning_FrontBrakePressureOutOfRange_raw = 0;
    FSM_Warning_FrontBrakePressureOutOfRange_raw |= unpackShiftRight(in_data[1], 4, 0x10);   // Unpacks bits ___#____ of msg byte 1
    bool FSM_Warning_FrontBrakePressureOutOfRange_val = CAN_DECODE(FSM_Warning_FrontBrakePressureOutOfRange_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_Warning_FrontBrakePressureOutOfRange_value = FSM_Warning_FrontBrakePressureOutOfRange_val;
    
    // Unpack 1-bit signal FSM_Warning_RearBrakePressureOutOfRangeWarning from payload (at bit 13 to bit 14).
    uint32_t FSM_Warning_RearBrakePressureOutOfRangeWarning_raw = 0;
    FSM_Warning_RearBrakePressureOutOfRangeWarning_raw |= unpackShiftRight(in_data[1], 5, 0x20);   // Unpacks bits __#_____ of msg byte 1
    bool FSM_Warning_RearBrakePressureOutOfRangeWarning_val = CAN_DECODE(FSM_Warning_RearBrakePressureOutOfRangeWarning_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_msg->FSM_Warning_RearBrakePressureOutOfRangeWarning_value = FSM_Warning_RearBrakePressureOutOfRangeWarning_val;
    
    // Unpack 1-bit signal FSM_Warning_FlowRateOutOfRange from payload (at bit 14 to bit 15).
    uint32_t FSM_Warning_FlowRateOutOfRange_raw = 0;
    FSM_Warning_FlowRateOutOfRange_raw |= unpackShiftRight(in_data[1], 6, 0x40);   // Unpacks bits _#______ of msg byte 1
    bool FSM_Warning_FlowRateOutOfRange_val = CAN_DECODE(FSM_Warning_FlowRateOutOfRange_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_Warning_FlowRateOutOfRange_value = FSM_Warning_FlowRateOutOfRange_val;
    
    // Unpack 1-bit signal FSM_Warning_BrakeAppsDisagreement from payload (at bit 15 to bit 16).
    uint32_t FSM_Warning_BrakeAppsDisagreement_raw = 0;
    FSM_Warning_BrakeAppsDisagreement_raw |= unpackShiftRight(in_data[1], 7, 0x80);   // Unpacks bits #_______ of msg byte 1
    bool FSM_Warning_BrakeAppsDisagreement_val = CAN_DECODE(FSM_Warning_BrakeAppsDisagreement_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_OFFSET, uint32_t);
    out_msg->FSM_Warning_BrakeAppsDisagreement_value = FSM_Warning_BrakeAppsDisagreement_val;
    
    // Unpack 1-bit signal FSM_Warning_AppsDisagreement from payload (at bit 16 to bit 17).
    uint32_t FSM_Warning_AppsDisagreement_raw = 0;
    FSM_Warning_AppsDisagreement_raw |= unpackShiftRight(in_data[2], 0, 0x1);   // Unpacks bits _______# of msg byte 2
    bool FSM_Warning_AppsDisagreement_val = CAN_DECODE(FSM_Warning_AppsDisagreement_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_APPS_DISAGREEMENT_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_APPS_DISAGREEMENT_OFFSET, uint32_t);
    out_msg->FSM_Warning_AppsDisagreement_value = FSM_Warning_AppsDisagreement_val;
    
}

void App_CanUtils_FSM_Faults_Unpack(const uint8_t* const in_data, FSM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message FSM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__FEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_Fault_MissingHeartbeat from payload (at bit 0 to bit 1).
    uint32_t FSM_Fault_MissingHeartbeat_raw = 0;
    FSM_Fault_MissingHeartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_Fault_MissingHeartbeat_val = CAN_DECODE(FSM_Fault_MissingHeartbeat_raw, CANSIG_FSM_FAULTS_FSM_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_msg->FSM_Fault_MissingHeartbeat_value = FSM_Fault_MissingHeartbeat_val;
    
    // Unpack 1-bit signal FSM_Fault_StateMachine from payload (at bit 1 to bit 2).
    uint32_t FSM_Fault_StateMachine_raw = 0;
    FSM_Fault_StateMachine_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool FSM_Fault_StateMachine_val = CAN_DECODE(FSM_Fault_StateMachine_raw, CANSIG_FSM_FAULTS_FSM_FAULT_STATE_MACHINE_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_STATE_MACHINE_OFFSET, uint32_t);
    out_msg->FSM_Fault_StateMachine_value = FSM_Fault_StateMachine_val;
    
    // Unpack 1-bit signal FSM_Fault_PappsOCSC from payload (at bit 2 to bit 3).
    uint32_t FSM_Fault_PappsOCSC_raw = 0;
    FSM_Fault_PappsOCSC_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool FSM_Fault_PappsOCSC_val = CAN_DECODE(FSM_Fault_PappsOCSC_raw, CANSIG_FSM_FAULTS_FSM_FAULT_PAPPS_OCSC_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_PAPPS_OCSC_OFFSET, uint32_t);
    out_msg->FSM_Fault_PappsOCSC_value = FSM_Fault_PappsOCSC_val;
    
    // Unpack 1-bit signal FSM_Fault_SappsOCSCFault from payload (at bit 3 to bit 4).
    uint32_t FSM_Fault_SappsOCSCFault_raw = 0;
    FSM_Fault_SappsOCSCFault_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool FSM_Fault_SappsOCSCFault_val = CAN_DECODE(FSM_Fault_SappsOCSCFault_raw, CANSIG_FSM_FAULTS_FSM_FAULT_SAPPS_OCSC_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_SAPPS_OCSC_FAULT_OFFSET, uint32_t);
    out_msg->FSM_Fault_SappsOCSCFault_value = FSM_Fault_SappsOCSCFault_val;
    
    // Unpack 1-bit signal FSM_Fault_FlowMeterUnderflow from payload (at bit 4 to bit 5).
    uint32_t FSM_Fault_FlowMeterUnderflow_raw = 0;
    FSM_Fault_FlowMeterUnderflow_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool FSM_Fault_FlowMeterUnderflow_val = CAN_DECODE(FSM_Fault_FlowMeterUnderflow_raw, CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_UNDERFLOW_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_UNDERFLOW_OFFSET, uint32_t);
    out_msg->FSM_Fault_FlowMeterUnderflow_value = FSM_Fault_FlowMeterUnderflow_val;
    
    // Unpack 1-bit signal FSM_Fault_TorquePlausabilityFailed from payload (at bit 5 to bit 6).
    uint32_t FSM_Fault_TorquePlausabilityFailed_raw = 0;
    FSM_Fault_TorquePlausabilityFailed_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool FSM_Fault_TorquePlausabilityFailed_val = CAN_DECODE(FSM_Fault_TorquePlausabilityFailed_raw, CANSIG_FSM_FAULTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_OFFSET, uint32_t);
    out_msg->FSM_Fault_TorquePlausabilityFailed_value = FSM_Fault_TorquePlausabilityFailed_val;
    
}

void App_CanUtils_INVL_MotorPositionInfo_Unpack(const uint8_t* const in_data, INVL_MotorPositionInfo_Signals* const out_msg)
{
    // Unpack 8-byte payload for message INVL_MotorPositionInfo.
    // |DDDDDDDD|DDDDDDDD|CCCCCCCC|CCCCCCCC|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 16-bit signal INVL_MotorAngle from payload (at bit 0 to bit 16).
    uint32_t INVL_MotorAngle_raw = 0;
    INVL_MotorAngle_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    INVL_MotorAngle_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    const int INVL_MotorAngle_signed = SIGNED_DECODE(INVL_MotorAngle_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float INVL_MotorAngle_val = CAN_DECODE(INVL_MotorAngle_signed, CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_SCALE, CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_ANGLE_OFFSET, float);
    out_msg->INVL_MotorAngle_value = INVL_MotorAngle_val;
    
    // Unpack 16-bit signal INVL_MotorSpeed from payload (at bit 16 to bit 32).
    uint32_t INVL_MotorSpeed_raw = 0;
    INVL_MotorSpeed_raw |= unpackShiftRight(in_data[2], 0, 0xff);   // Unpacks bits ######## of msg byte 2
    INVL_MotorSpeed_raw |= unpackShiftLeft(in_data[3], 8, 0xff);   // Unpacks bits ######## of msg byte 3
    const int INVL_MotorSpeed_signed = SIGNED_DECODE(INVL_MotorSpeed_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    int INVL_MotorSpeed_val = CAN_DECODE(INVL_MotorSpeed_signed, CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_SCALE, CANSIG_INVL_MOTOR_POSITION_INFO_INVL_MOTOR_SPEED_OFFSET, int);
    out_msg->INVL_MotorSpeed_value = INVL_MotorSpeed_val;
    
    // Unpack 16-bit signal INVL_ElectricalOutputFrequency from payload (at bit 32 to bit 48).
    uint32_t INVL_ElectricalOutputFrequency_raw = 0;
    INVL_ElectricalOutputFrequency_raw |= unpackShiftRight(in_data[4], 0, 0xff);   // Unpacks bits ######## of msg byte 4
    INVL_ElectricalOutputFrequency_raw |= unpackShiftLeft(in_data[5], 8, 0xff);   // Unpacks bits ######## of msg byte 5
    const int INVL_ElectricalOutputFrequency_signed = SIGNED_DECODE(INVL_ElectricalOutputFrequency_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float INVL_ElectricalOutputFrequency_val = CAN_DECODE(INVL_ElectricalOutputFrequency_signed, CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_SCALE, CANSIG_INVL_MOTOR_POSITION_INFO_INVL_ELECTRICAL_OUTPUT_FREQUENCY_OFFSET, float);
    out_msg->INVL_ElectricalOutputFrequency_value = INVL_ElectricalOutputFrequency_val;
    
    // Unpack 16-bit signal INVL_DeltaResolverFiltered from payload (at bit 48 to bit 64).
    uint32_t INVL_DeltaResolverFiltered_raw = 0;
    INVL_DeltaResolverFiltered_raw |= unpackShiftRight(in_data[6], 0, 0xff);   // Unpacks bits ######## of msg byte 6
    INVL_DeltaResolverFiltered_raw |= unpackShiftLeft(in_data[7], 8, 0xff);   // Unpacks bits ######## of msg byte 7
    const int INVL_DeltaResolverFiltered_signed = SIGNED_DECODE(INVL_DeltaResolverFiltered_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float INVL_DeltaResolverFiltered_val = CAN_DECODE(INVL_DeltaResolverFiltered_signed, CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_SCALE, CANSIG_INVL_MOTOR_POSITION_INFO_INVL_DELTA_RESOLVER_FILTERED_OFFSET, float);
    out_msg->INVL_DeltaResolverFiltered_value = INVL_DeltaResolverFiltered_val;
    
}

void App_CanUtils_Debug_CanMode_Unpack(const uint8_t* const in_data, Debug_CanMode_Signals* const out_msg)
{
    // Unpack 1-byte payload for message Debug_CanMode.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal Debug_EnableDebugMode from payload (at bit 0 to bit 1).
    uint32_t Debug_EnableDebugMode_raw = 0;
    Debug_EnableDebugMode_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool Debug_EnableDebugMode_val = CAN_DECODE(Debug_EnableDebugMode_raw, CANSIG_DEBUG_CAN_MODE_DEBUG_ENABLE_DEBUG_MODE_SCALE, CANSIG_DEBUG_CAN_MODE_DEBUG_ENABLE_DEBUG_MODE_OFFSET, uint32_t);
    out_msg->Debug_EnableDebugMode_value = Debug_EnableDebugMode_val;
    
}

void App_CanUtils_PDM_Warnings_Unpack(const uint8_t* const in_data, PDM_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message PDM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal PDM_Warning_StackWaterMarkHighTask1Hz from payload (at bit 0 to bit 1).
    uint32_t PDM_Warning_StackWaterMarkHighTask1Hz_raw = 0;
    PDM_Warning_StackWaterMarkHighTask1Hz_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool PDM_Warning_StackWaterMarkHighTask1Hz_val = CAN_DECODE(PDM_Warning_StackWaterMarkHighTask1Hz_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_msg->PDM_Warning_StackWaterMarkHighTask1Hz_value = PDM_Warning_StackWaterMarkHighTask1Hz_val;
    
    // Unpack 1-bit signal PDM_Warning_StackWaterMarkHighTask100Hz from payload (at bit 1 to bit 2).
    uint32_t PDM_Warning_StackWaterMarkHighTask100Hz_raw = 0;
    PDM_Warning_StackWaterMarkHighTask100Hz_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool PDM_Warning_StackWaterMarkHighTask100Hz_val = CAN_DECODE(PDM_Warning_StackWaterMarkHighTask100Hz_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_msg->PDM_Warning_StackWaterMarkHighTask100Hz_value = PDM_Warning_StackWaterMarkHighTask100Hz_val;
    
    // Unpack 1-bit signal PDM_Warning_StackWaterMarkHighTask1kHz from payload (at bit 2 to bit 3).
    uint32_t PDM_Warning_StackWaterMarkHighTask1kHz_raw = 0;
    PDM_Warning_StackWaterMarkHighTask1kHz_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool PDM_Warning_StackWaterMarkHighTask1kHz_val = CAN_DECODE(PDM_Warning_StackWaterMarkHighTask1kHz_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_msg->PDM_Warning_StackWaterMarkHighTask1kHz_value = PDM_Warning_StackWaterMarkHighTask1kHz_val;
    
    // Unpack 1-bit signal PDM_Warning_StackWaterMarkHighTaskCanRx from payload (at bit 3 to bit 4).
    uint32_t PDM_Warning_StackWaterMarkHighTaskCanRx_raw = 0;
    PDM_Warning_StackWaterMarkHighTaskCanRx_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool PDM_Warning_StackWaterMarkHighTaskCanRx_val = CAN_DECODE(PDM_Warning_StackWaterMarkHighTaskCanRx_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_msg->PDM_Warning_StackWaterMarkHighTaskCanRx_value = PDM_Warning_StackWaterMarkHighTaskCanRx_val;
    
    // Unpack 1-bit signal PDM_Warning_StackWaterMarkHighTaskCanTx from payload (at bit 4 to bit 5).
    uint32_t PDM_Warning_StackWaterMarkHighTaskCanTx_raw = 0;
    PDM_Warning_StackWaterMarkHighTaskCanTx_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool PDM_Warning_StackWaterMarkHighTaskCanTx_val = CAN_DECODE(PDM_Warning_StackWaterMarkHighTaskCanTx_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_msg->PDM_Warning_StackWaterMarkHighTaskCanTx_value = PDM_Warning_StackWaterMarkHighTaskCanTx_val;
    
    // Unpack 1-bit signal PDM_Warning_WatchdogTimeout from payload (at bit 5 to bit 6).
    uint32_t PDM_Warning_WatchdogTimeout_raw = 0;
    PDM_Warning_WatchdogTimeout_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool PDM_Warning_WatchdogTimeout_val = CAN_DECODE(PDM_Warning_WatchdogTimeout_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_msg->PDM_Warning_WatchdogTimeout_value = PDM_Warning_WatchdogTimeout_val;
    
    // Unpack 1-bit signal PDM_Warning_TxOverflow from payload (at bit 6 to bit 7).
    uint32_t PDM_Warning_TxOverflow_raw = 0;
    PDM_Warning_TxOverflow_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool PDM_Warning_TxOverflow_val = CAN_DECODE(PDM_Warning_TxOverflow_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_TX_OVERFLOW_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_msg->PDM_Warning_TxOverflow_value = PDM_Warning_TxOverflow_val;
    
    // Unpack 1-bit signal PDM_Warning_RxOverflow from payload (at bit 7 to bit 8).
    uint32_t PDM_Warning_RxOverflow_raw = 0;
    PDM_Warning_RxOverflow_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool PDM_Warning_RxOverflow_val = CAN_DECODE(PDM_Warning_RxOverflow_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_RX_OVERFLOW_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_msg->PDM_Warning_RxOverflow_value = PDM_Warning_RxOverflow_val;
    
}

void App_CanUtils_PDM_Faults_Unpack(const uint8_t* const in_data, PDM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message PDM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal PDM_Fault_DummyFault from payload (at bit 0 to bit 1).
    uint32_t PDM_Fault_DummyFault_raw = 0;
    PDM_Fault_DummyFault_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool PDM_Fault_DummyFault_val = CAN_DECODE(PDM_Fault_DummyFault_raw, CANSIG_PDM_FAULTS_PDM_FAULT_DUMMY_FAULT_SCALE, CANSIG_PDM_FAULTS_PDM_FAULT_DUMMY_FAULT_OFFSET, uint32_t);
    out_msg->PDM_Fault_DummyFault_value = PDM_Fault_DummyFault_val;
    
}

