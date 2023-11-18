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
    
    // Pack 1-bit signal DIM_StackWaterMarkHighTask1HzWarning into payload (at bit 0 to bit 1).
    const bool DIM_StackWaterMarkHighTask1HzWarning_val = in_msg->DIM_StackWaterMarkHighTask1HzWarning_value;
    const uint32_t DIM_StackWaterMarkHighTask1HzWarning_raw = CAN_ENCODE(DIM_StackWaterMarkHighTask1HzWarning_val, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_StackWaterMarkHighTask1HzWarning_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal DIM_StackWaterMarkHighTask100HzWarning into payload (at bit 1 to bit 2).
    const bool DIM_StackWaterMarkHighTask100HzWarning_val = in_msg->DIM_StackWaterMarkHighTask100HzWarning_value;
    const uint32_t DIM_StackWaterMarkHighTask100HzWarning_raw = CAN_ENCODE(DIM_StackWaterMarkHighTask100HzWarning_val, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_StackWaterMarkHighTask100HzWarning_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal DIM_StackWaterMarkHighTask1kHzWarning into payload (at bit 2 to bit 3).
    const bool DIM_StackWaterMarkHighTask1kHzWarning_val = in_msg->DIM_StackWaterMarkHighTask1kHzWarning_value;
    const uint32_t DIM_StackWaterMarkHighTask1kHzWarning_raw = CAN_ENCODE(DIM_StackWaterMarkHighTask1kHzWarning_val, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_StackWaterMarkHighTask1kHzWarning_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal DIM_StackWaterMarkHighTaskCanRxWarning into payload (at bit 3 to bit 4).
    const bool DIM_StackWaterMarkHighTaskCanRxWarning_val = in_msg->DIM_StackWaterMarkHighTaskCanRxWarning_value;
    const uint32_t DIM_StackWaterMarkHighTaskCanRxWarning_raw = CAN_ENCODE(DIM_StackWaterMarkHighTaskCanRxWarning_val, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_StackWaterMarkHighTaskCanRxWarning_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal DIM_StackWaterMarkHighTaskCanTxWarning into payload (at bit 4 to bit 5).
    const bool DIM_StackWaterMarkHighTaskCanTxWarning_val = in_msg->DIM_StackWaterMarkHighTaskCanTxWarning_value;
    const uint32_t DIM_StackWaterMarkHighTaskCanTxWarning_raw = CAN_ENCODE(DIM_StackWaterMarkHighTaskCanTxWarning_val, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_StackWaterMarkHighTaskCanTxWarning_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal DIM_WatchdogTimeoutWarning into payload (at bit 5 to bit 6).
    const bool DIM_WatchdogTimeoutWarning_val = in_msg->DIM_WatchdogTimeoutWarning_value;
    const uint32_t DIM_WatchdogTimeoutWarning_raw = CAN_ENCODE(DIM_WatchdogTimeoutWarning_val, CANSIG_DIM_WARNINGS_DIM_WATCHDOG_TIMEOUT_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_WATCHDOG_TIMEOUT_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_WatchdogTimeoutWarning_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 1-bit signal DIM_TxOverflowWarning into payload (at bit 6 to bit 7).
    const bool DIM_TxOverflowWarning_val = in_msg->DIM_TxOverflowWarning_value;
    const uint32_t DIM_TxOverflowWarning_raw = CAN_ENCODE(DIM_TxOverflowWarning_val, CANSIG_DIM_WARNINGS_DIM_TX_OVERFLOW_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_TX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_TxOverflowWarning_raw, 6, 0x40);   // Packs bits _#______ of byte 0
    
    // Pack 1-bit signal DIM_RxOverflowWarning into payload (at bit 7 to bit 8).
    const bool DIM_RxOverflowWarning_val = in_msg->DIM_RxOverflowWarning_value;
    const uint32_t DIM_RxOverflowWarning_raw = CAN_ENCODE(DIM_RxOverflowWarning_val, CANSIG_DIM_WARNINGS_DIM_RX_OVERFLOW_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_RX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_RxOverflowWarning_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    
}

void App_CanUtils_DIM_Faults_Pack(const DIM_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DIM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DIM_MissingHeartbeatFault into payload (at bit 0 to bit 1).
    const bool DIM_MissingHeartbeatFault_val = in_msg->DIM_MissingHeartbeatFault_value;
    const uint32_t DIM_MissingHeartbeatFault_raw = CAN_ENCODE(DIM_MissingHeartbeatFault_val, CANSIG_DIM_FAULTS_DIM_MISSING_HEARTBEAT_FAULT_SCALE, CANSIG_DIM_FAULTS_DIM_MISSING_HEARTBEAT_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_MissingHeartbeatFault_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
}

void App_CanUtils_DIM_WarningsCounts_Pack(const DIM_WarningsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message DIM_WarningsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal DIM_StackWaterMarkHighTask1HzWarningCount into payload (at bit 0 to bit 3).
    const uint32_t DIM_StackWaterMarkHighTask1HzWarningCount_val = in_msg->DIM_StackWaterMarkHighTask1HzWarningCount_value;
    const uint32_t DIM_StackWaterMarkHighTask1HzWarningCount_raw = CAN_ENCODE(DIM_StackWaterMarkHighTask1HzWarningCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_StackWaterMarkHighTask1HzWarningCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal DIM_StackWaterMarkHighTask100HzWarningCount into payload (at bit 3 to bit 6).
    const uint32_t DIM_StackWaterMarkHighTask100HzWarningCount_val = in_msg->DIM_StackWaterMarkHighTask100HzWarningCount_value;
    const uint32_t DIM_StackWaterMarkHighTask100HzWarningCount_raw = CAN_ENCODE(DIM_StackWaterMarkHighTask100HzWarningCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_StackWaterMarkHighTask100HzWarningCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal DIM_StackWaterMarkHighTask1kHzWarningCount into payload (at bit 6 to bit 9).
    const uint32_t DIM_StackWaterMarkHighTask1kHzWarningCount_val = in_msg->DIM_StackWaterMarkHighTask1kHzWarningCount_value;
    const uint32_t DIM_StackWaterMarkHighTask1kHzWarningCount_raw = CAN_ENCODE(DIM_StackWaterMarkHighTask1kHzWarningCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_StackWaterMarkHighTask1kHzWarningCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(DIM_StackWaterMarkHighTask1kHzWarningCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 3-bit signal DIM_StackWaterMarkHighTaskCanRxWarningCount into payload (at bit 9 to bit 12).
    const uint32_t DIM_StackWaterMarkHighTaskCanRxWarningCount_val = in_msg->DIM_StackWaterMarkHighTaskCanRxWarningCount_value;
    const uint32_t DIM_StackWaterMarkHighTaskCanRxWarningCount_raw = CAN_ENCODE(DIM_StackWaterMarkHighTaskCanRxWarningCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(DIM_StackWaterMarkHighTaskCanRxWarningCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 1
    
    // Pack 3-bit signal DIM_StackWaterMarkHighTaskCanTxWarningCount into payload (at bit 12 to bit 15).
    const uint32_t DIM_StackWaterMarkHighTaskCanTxWarningCount_val = in_msg->DIM_StackWaterMarkHighTaskCanTxWarningCount_value;
    const uint32_t DIM_StackWaterMarkHighTaskCanTxWarningCount_raw = CAN_ENCODE(DIM_StackWaterMarkHighTaskCanTxWarningCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(DIM_StackWaterMarkHighTaskCanTxWarningCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1
    
    // Pack 3-bit signal DIM_WatchdogTimeoutWarningCount into payload (at bit 15 to bit 18).
    const uint32_t DIM_WatchdogTimeoutWarningCount_val = in_msg->DIM_WatchdogTimeoutWarningCount_value;
    const uint32_t DIM_WatchdogTimeoutWarningCount_raw = CAN_ENCODE(DIM_WatchdogTimeoutWarningCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_WATCHDOG_TIMEOUT_WARNING_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_WATCHDOG_TIMEOUT_WARNING_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(DIM_WatchdogTimeoutWarningCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    out_data[2] |= packShiftRight(DIM_WatchdogTimeoutWarningCount_raw, 1, 0x3);   // Packs bits ______## of byte 2
    
    // Pack 3-bit signal DIM_TxOverflowWarningCount into payload (at bit 18 to bit 21).
    const uint32_t DIM_TxOverflowWarningCount_val = in_msg->DIM_TxOverflowWarningCount_value;
    const uint32_t DIM_TxOverflowWarningCount_raw = CAN_ENCODE(DIM_TxOverflowWarningCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_TX_OVERFLOW_WARNING_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_TX_OVERFLOW_WARNING_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(DIM_TxOverflowWarningCount_raw, 2, 0x1c);   // Packs bits ___###__ of byte 2
    
    // Pack 3-bit signal DIM_RxOverflowWarningCount into payload (at bit 21 to bit 24).
    const uint32_t DIM_RxOverflowWarningCount_val = in_msg->DIM_RxOverflowWarningCount_value;
    const uint32_t DIM_RxOverflowWarningCount_raw = CAN_ENCODE(DIM_RxOverflowWarningCount_val, CANSIG_DIM_WARNINGS_COUNTS_DIM_RX_OVERFLOW_WARNING_COUNT_SCALE, CANSIG_DIM_WARNINGS_COUNTS_DIM_RX_OVERFLOW_WARNING_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(DIM_RxOverflowWarningCount_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    
}

void App_CanUtils_DIM_FaultsCounts_Pack(const DIM_FaultsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DIM_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____AAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal DIM_MissingHeartbeatFaultCount into payload (at bit 0 to bit 3).
    const uint32_t DIM_MissingHeartbeatFaultCount_val = in_msg->DIM_MissingHeartbeatFaultCount_value;
    const uint32_t DIM_MissingHeartbeatFaultCount_raw = CAN_ENCODE(DIM_MissingHeartbeatFaultCount_val, CANSIG_DIM_FAULTS_COUNTS_DIM_MISSING_HEARTBEAT_FAULT_COUNT_SCALE, CANSIG_DIM_FAULTS_COUNTS_DIM_MISSING_HEARTBEAT_FAULT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_MissingHeartbeatFaultCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
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
    
    // Unpack 1-bit signal BMS_StackWaterMarkHighTask1HzWarning from payload (at bit 0 to bit 1).
    uint32_t BMS_StackWaterMarkHighTask1HzWarning_raw = 0;
    BMS_StackWaterMarkHighTask1HzWarning_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool BMS_StackWaterMarkHighTask1HzWarning_val = CAN_DECODE(BMS_StackWaterMarkHighTask1HzWarning_raw, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_OFFSET, uint32_t);
    out_msg->BMS_StackWaterMarkHighTask1HzWarning_value = BMS_StackWaterMarkHighTask1HzWarning_val;
    
    // Unpack 1-bit signal BMS_StackWaterMarkHighTask100HzWarning from payload (at bit 1 to bit 2).
    uint32_t BMS_StackWaterMarkHighTask100HzWarning_raw = 0;
    BMS_StackWaterMarkHighTask100HzWarning_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool BMS_StackWaterMarkHighTask100HzWarning_val = CAN_DECODE(BMS_StackWaterMarkHighTask100HzWarning_raw, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_OFFSET, uint32_t);
    out_msg->BMS_StackWaterMarkHighTask100HzWarning_value = BMS_StackWaterMarkHighTask100HzWarning_val;
    
    // Unpack 1-bit signal BMS_StackWaterMarkHighTask1kHzWarning from payload (at bit 2 to bit 3).
    uint32_t BMS_StackWaterMarkHighTask1kHzWarning_raw = 0;
    BMS_StackWaterMarkHighTask1kHzWarning_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool BMS_StackWaterMarkHighTask1kHzWarning_val = CAN_DECODE(BMS_StackWaterMarkHighTask1kHzWarning_raw, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_OFFSET, uint32_t);
    out_msg->BMS_StackWaterMarkHighTask1kHzWarning_value = BMS_StackWaterMarkHighTask1kHzWarning_val;
    
    // Unpack 1-bit signal BMS_StackWaterMarkHighTaskCanRxWarning from payload (at bit 3 to bit 4).
    uint32_t BMS_StackWaterMarkHighTaskCanRxWarning_raw = 0;
    BMS_StackWaterMarkHighTaskCanRxWarning_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool BMS_StackWaterMarkHighTaskCanRxWarning_val = CAN_DECODE(BMS_StackWaterMarkHighTaskCanRxWarning_raw, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_OFFSET, uint32_t);
    out_msg->BMS_StackWaterMarkHighTaskCanRxWarning_value = BMS_StackWaterMarkHighTaskCanRxWarning_val;
    
    // Unpack 1-bit signal BMS_StackWaterMarkHighTaskCanTxWarning from payload (at bit 4 to bit 5).
    uint32_t BMS_StackWaterMarkHighTaskCanTxWarning_raw = 0;
    BMS_StackWaterMarkHighTaskCanTxWarning_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool BMS_StackWaterMarkHighTaskCanTxWarning_val = CAN_DECODE(BMS_StackWaterMarkHighTaskCanTxWarning_raw, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_OFFSET, uint32_t);
    out_msg->BMS_StackWaterMarkHighTaskCanTxWarning_value = BMS_StackWaterMarkHighTaskCanTxWarning_val;
    
    // Unpack 1-bit signal BMS_WatchdogTimeoutWarning from payload (at bit 5 to bit 6).
    uint32_t BMS_WatchdogTimeoutWarning_raw = 0;
    BMS_WatchdogTimeoutWarning_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool BMS_WatchdogTimeoutWarning_val = CAN_DECODE(BMS_WatchdogTimeoutWarning_raw, CANSIG_BMS_WARNINGS_BMS_WATCHDOG_TIMEOUT_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_WATCHDOG_TIMEOUT_WARNING_OFFSET, uint32_t);
    out_msg->BMS_WatchdogTimeoutWarning_value = BMS_WatchdogTimeoutWarning_val;
    
    // Unpack 1-bit signal BMS_TxOverflowWarning from payload (at bit 6 to bit 7).
    uint32_t BMS_TxOverflowWarning_raw = 0;
    BMS_TxOverflowWarning_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool BMS_TxOverflowWarning_val = CAN_DECODE(BMS_TxOverflowWarning_raw, CANSIG_BMS_WARNINGS_BMS_TX_OVERFLOW_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_TX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_msg->BMS_TxOverflowWarning_value = BMS_TxOverflowWarning_val;
    
    // Unpack 1-bit signal BMS_RxOverflowWarning from payload (at bit 7 to bit 8).
    uint32_t BMS_RxOverflowWarning_raw = 0;
    BMS_RxOverflowWarning_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool BMS_RxOverflowWarning_val = CAN_DECODE(BMS_RxOverflowWarning_raw, CANSIG_BMS_WARNINGS_BMS_RX_OVERFLOW_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_RX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_msg->BMS_RxOverflowWarning_value = BMS_RxOverflowWarning_val;
    
}

void App_CanUtils_BMS_Faults_Unpack(const uint8_t* const in_data, BMS_Faults_Signals* const out_msg)
{
    // Unpack 2-byte payload for message BMS_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____LKJI|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal BMS_MissingHeartbeatFault from payload (at bit 0 to bit 1).
    uint32_t BMS_MissingHeartbeatFault_raw = 0;
    BMS_MissingHeartbeatFault_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool BMS_MissingHeartbeatFault_val = CAN_DECODE(BMS_MissingHeartbeatFault_raw, CANSIG_BMS_FAULTS_BMS_MISSING_HEARTBEAT_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_MISSING_HEARTBEAT_FAULT_OFFSET, uint32_t);
    out_msg->BMS_MissingHeartbeatFault_value = BMS_MissingHeartbeatFault_val;
    
    // Unpack 1-bit signal BMS_StateMachineFault from payload (at bit 1 to bit 2).
    uint32_t BMS_StateMachineFault_raw = 0;
    BMS_StateMachineFault_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool BMS_StateMachineFault_val = CAN_DECODE(BMS_StateMachineFault_raw, CANSIG_BMS_FAULTS_BMS_STATE_MACHINE_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_STATE_MACHINE_FAULT_OFFSET, uint32_t);
    out_msg->BMS_StateMachineFault_value = BMS_StateMachineFault_val;
    
    // Unpack 1-bit signal BMS_CellUndervoltageFault from payload (at bit 2 to bit 3).
    uint32_t BMS_CellUndervoltageFault_raw = 0;
    BMS_CellUndervoltageFault_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool BMS_CellUndervoltageFault_val = CAN_DECODE(BMS_CellUndervoltageFault_raw, CANSIG_BMS_FAULTS_BMS_CELL_UNDERVOLTAGE_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CELL_UNDERVOLTAGE_FAULT_OFFSET, uint32_t);
    out_msg->BMS_CellUndervoltageFault_value = BMS_CellUndervoltageFault_val;
    
    // Unpack 1-bit signal BMS_CellOvervoltageFault from payload (at bit 3 to bit 4).
    uint32_t BMS_CellOvervoltageFault_raw = 0;
    BMS_CellOvervoltageFault_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool BMS_CellOvervoltageFault_val = CAN_DECODE(BMS_CellOvervoltageFault_raw, CANSIG_BMS_FAULTS_BMS_CELL_OVERVOLTAGE_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CELL_OVERVOLTAGE_FAULT_OFFSET, uint32_t);
    out_msg->BMS_CellOvervoltageFault_value = BMS_CellOvervoltageFault_val;
    
    // Unpack 1-bit signal BMS_ModuleCommunicationFault from payload (at bit 4 to bit 5).
    uint32_t BMS_ModuleCommunicationFault_raw = 0;
    BMS_ModuleCommunicationFault_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool BMS_ModuleCommunicationFault_val = CAN_DECODE(BMS_ModuleCommunicationFault_raw, CANSIG_BMS_FAULTS_BMS_MODULE_COMMUNICATION_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_MODULE_COMMUNICATION_FAULT_OFFSET, uint32_t);
    out_msg->BMS_ModuleCommunicationFault_value = BMS_ModuleCommunicationFault_val;
    
    // Unpack 1-bit signal BMS_CellUndertempFault from payload (at bit 5 to bit 6).
    uint32_t BMS_CellUndertempFault_raw = 0;
    BMS_CellUndertempFault_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool BMS_CellUndertempFault_val = CAN_DECODE(BMS_CellUndertempFault_raw, CANSIG_BMS_FAULTS_BMS_CELL_UNDERTEMP_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CELL_UNDERTEMP_FAULT_OFFSET, uint32_t);
    out_msg->BMS_CellUndertempFault_value = BMS_CellUndertempFault_val;
    
    // Unpack 1-bit signal BMS_CellOvertempFault from payload (at bit 6 to bit 7).
    uint32_t BMS_CellOvertempFault_raw = 0;
    BMS_CellOvertempFault_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool BMS_CellOvertempFault_val = CAN_DECODE(BMS_CellOvertempFault_raw, CANSIG_BMS_FAULTS_BMS_CELL_OVERTEMP_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CELL_OVERTEMP_FAULT_OFFSET, uint32_t);
    out_msg->BMS_CellOvertempFault_value = BMS_CellOvertempFault_val;
    
    // Unpack 1-bit signal BMS_ChargerFault from payload (at bit 7 to bit 8).
    uint32_t BMS_ChargerFault_raw = 0;
    BMS_ChargerFault_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool BMS_ChargerFault_val = CAN_DECODE(BMS_ChargerFault_raw, CANSIG_BMS_FAULTS_BMS_CHARGER_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CHARGER_FAULT_OFFSET, uint32_t);
    out_msg->BMS_ChargerFault_value = BMS_ChargerFault_val;
    
    // Unpack 1-bit signal BMS_ChargerDisconnectedDuringChargeFault from payload (at bit 8 to bit 9).
    uint32_t BMS_ChargerDisconnectedDuringChargeFault_raw = 0;
    BMS_ChargerDisconnectedDuringChargeFault_raw |= unpackShiftRight(in_data[1], 0, 0x1);   // Unpacks bits _______# of msg byte 1
    bool BMS_ChargerDisconnectedDuringChargeFault_val = CAN_DECODE(BMS_ChargerDisconnectedDuringChargeFault_raw, CANSIG_BMS_FAULTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_OFFSET, uint32_t);
    out_msg->BMS_ChargerDisconnectedDuringChargeFault_value = BMS_ChargerDisconnectedDuringChargeFault_val;
    
    // Unpack 1-bit signal BMS_ChargerExternalShutdownFault from payload (at bit 9 to bit 10).
    uint32_t BMS_ChargerExternalShutdownFault_raw = 0;
    BMS_ChargerExternalShutdownFault_raw |= unpackShiftRight(in_data[1], 1, 0x2);   // Unpacks bits ______#_ of msg byte 1
    bool BMS_ChargerExternalShutdownFault_val = CAN_DECODE(BMS_ChargerExternalShutdownFault_raw, CANSIG_BMS_FAULTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_OFFSET, uint32_t);
    out_msg->BMS_ChargerExternalShutdownFault_value = BMS_ChargerExternalShutdownFault_val;
    
    // Unpack 1-bit signal BMS_TractiveSystemOvercurrentFault from payload (at bit 10 to bit 11).
    uint32_t BMS_TractiveSystemOvercurrentFault_raw = 0;
    BMS_TractiveSystemOvercurrentFault_raw |= unpackShiftRight(in_data[1], 2, 0x4);   // Unpacks bits _____#__ of msg byte 1
    bool BMS_TractiveSystemOvercurrentFault_val = CAN_DECODE(BMS_TractiveSystemOvercurrentFault_raw, CANSIG_BMS_FAULTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_OFFSET, uint32_t);
    out_msg->BMS_TractiveSystemOvercurrentFault_value = BMS_TractiveSystemOvercurrentFault_val;
    
    // Unpack 1-bit signal BMS_PrechargeFailureFault from payload (at bit 11 to bit 12).
    uint32_t BMS_PrechargeFailureFault_raw = 0;
    BMS_PrechargeFailureFault_raw |= unpackShiftRight(in_data[1], 3, 0x8);   // Unpacks bits ____#___ of msg byte 1
    bool BMS_PrechargeFailureFault_val = CAN_DECODE(BMS_PrechargeFailureFault_raw, CANSIG_BMS_FAULTS_BMS_PRECHARGE_FAILURE_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_PRECHARGE_FAILURE_FAULT_OFFSET, uint32_t);
    out_msg->BMS_PrechargeFailureFault_value = BMS_PrechargeFailureFault_val;
    
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
    
    // Unpack 1-bit signal DCM_StackWaterMarkHighTask1HzWarning from payload (at bit 0 to bit 1).
    uint32_t DCM_StackWaterMarkHighTask1HzWarning_raw = 0;
    DCM_StackWaterMarkHighTask1HzWarning_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DCM_StackWaterMarkHighTask1HzWarning_val = CAN_DECODE(DCM_StackWaterMarkHighTask1HzWarning_raw, CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_SCALE, CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_OFFSET, uint32_t);
    out_msg->DCM_StackWaterMarkHighTask1HzWarning_value = DCM_StackWaterMarkHighTask1HzWarning_val;
    
    // Unpack 1-bit signal DCM_StackWaterMarkHighTask100HzWarning from payload (at bit 1 to bit 2).
    uint32_t DCM_StackWaterMarkHighTask100HzWarning_raw = 0;
    DCM_StackWaterMarkHighTask100HzWarning_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool DCM_StackWaterMarkHighTask100HzWarning_val = CAN_DECODE(DCM_StackWaterMarkHighTask100HzWarning_raw, CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_SCALE, CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_OFFSET, uint32_t);
    out_msg->DCM_StackWaterMarkHighTask100HzWarning_value = DCM_StackWaterMarkHighTask100HzWarning_val;
    
    // Unpack 1-bit signal DCM_StackWaterMarkHighTask1kHzWarning from payload (at bit 2 to bit 3).
    uint32_t DCM_StackWaterMarkHighTask1kHzWarning_raw = 0;
    DCM_StackWaterMarkHighTask1kHzWarning_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool DCM_StackWaterMarkHighTask1kHzWarning_val = CAN_DECODE(DCM_StackWaterMarkHighTask1kHzWarning_raw, CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_SCALE, CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_OFFSET, uint32_t);
    out_msg->DCM_StackWaterMarkHighTask1kHzWarning_value = DCM_StackWaterMarkHighTask1kHzWarning_val;
    
    // Unpack 1-bit signal DCM_StackWaterMarkHighTaskCanRxWarning from payload (at bit 3 to bit 4).
    uint32_t DCM_StackWaterMarkHighTaskCanRxWarning_raw = 0;
    DCM_StackWaterMarkHighTaskCanRxWarning_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool DCM_StackWaterMarkHighTaskCanRxWarning_val = CAN_DECODE(DCM_StackWaterMarkHighTaskCanRxWarning_raw, CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_SCALE, CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_OFFSET, uint32_t);
    out_msg->DCM_StackWaterMarkHighTaskCanRxWarning_value = DCM_StackWaterMarkHighTaskCanRxWarning_val;
    
    // Unpack 1-bit signal DCM_StackWaterMarkHighTaskCanTxWarning from payload (at bit 4 to bit 5).
    uint32_t DCM_StackWaterMarkHighTaskCanTxWarning_raw = 0;
    DCM_StackWaterMarkHighTaskCanTxWarning_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool DCM_StackWaterMarkHighTaskCanTxWarning_val = CAN_DECODE(DCM_StackWaterMarkHighTaskCanTxWarning_raw, CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_SCALE, CANSIG_DCM_WARNINGS_DCM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_OFFSET, uint32_t);
    out_msg->DCM_StackWaterMarkHighTaskCanTxWarning_value = DCM_StackWaterMarkHighTaskCanTxWarning_val;
    
    // Unpack 1-bit signal DCM_WatchdogTimeoutWarning from payload (at bit 5 to bit 6).
    uint32_t DCM_WatchdogTimeoutWarning_raw = 0;
    DCM_WatchdogTimeoutWarning_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool DCM_WatchdogTimeoutWarning_val = CAN_DECODE(DCM_WatchdogTimeoutWarning_raw, CANSIG_DCM_WARNINGS_DCM_WATCHDOG_TIMEOUT_WARNING_SCALE, CANSIG_DCM_WARNINGS_DCM_WATCHDOG_TIMEOUT_WARNING_OFFSET, uint32_t);
    out_msg->DCM_WatchdogTimeoutWarning_value = DCM_WatchdogTimeoutWarning_val;
    
    // Unpack 1-bit signal DCM_TxOverflowWarning from payload (at bit 6 to bit 7).
    uint32_t DCM_TxOverflowWarning_raw = 0;
    DCM_TxOverflowWarning_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool DCM_TxOverflowWarning_val = CAN_DECODE(DCM_TxOverflowWarning_raw, CANSIG_DCM_WARNINGS_DCM_TX_OVERFLOW_WARNING_SCALE, CANSIG_DCM_WARNINGS_DCM_TX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_msg->DCM_TxOverflowWarning_value = DCM_TxOverflowWarning_val;
    
    // Unpack 1-bit signal DCM_RxOverflowWarning from payload (at bit 7 to bit 8).
    uint32_t DCM_RxOverflowWarning_raw = 0;
    DCM_RxOverflowWarning_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool DCM_RxOverflowWarning_val = CAN_DECODE(DCM_RxOverflowWarning_raw, CANSIG_DCM_WARNINGS_DCM_RX_OVERFLOW_WARNING_SCALE, CANSIG_DCM_WARNINGS_DCM_RX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_msg->DCM_RxOverflowWarning_value = DCM_RxOverflowWarning_val;
    
}

void App_CanUtils_DCM_Faults_Unpack(const uint8_t* const in_data, DCM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DCM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DCM_MissingHeartbeatFault from payload (at bit 0 to bit 1).
    uint32_t DCM_MissingHeartbeatFault_raw = 0;
    DCM_MissingHeartbeatFault_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DCM_MissingHeartbeatFault_val = CAN_DECODE(DCM_MissingHeartbeatFault_raw, CANSIG_DCM_FAULTS_DCM_MISSING_HEARTBEAT_FAULT_SCALE, CANSIG_DCM_FAULTS_DCM_MISSING_HEARTBEAT_FAULT_OFFSET, uint32_t);
    out_msg->DCM_MissingHeartbeatFault_value = DCM_MissingHeartbeatFault_val;
    
    // Unpack 1-bit signal DCM_LeftInverterFault from payload (at bit 1 to bit 2).
    uint32_t DCM_LeftInverterFault_raw = 0;
    DCM_LeftInverterFault_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool DCM_LeftInverterFault_val = CAN_DECODE(DCM_LeftInverterFault_raw, CANSIG_DCM_FAULTS_DCM_LEFT_INVERTER_FAULT_SCALE, CANSIG_DCM_FAULTS_DCM_LEFT_INVERTER_FAULT_OFFSET, uint32_t);
    out_msg->DCM_LeftInverterFault_value = DCM_LeftInverterFault_val;
    
    // Unpack 1-bit signal DCM_RightInverterFault from payload (at bit 2 to bit 3).
    uint32_t DCM_RightInverterFault_raw = 0;
    DCM_RightInverterFault_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool DCM_RightInverterFault_val = CAN_DECODE(DCM_RightInverterFault_raw, CANSIG_DCM_FAULTS_DCM_RIGHT_INVERTER_FAULT_SCALE, CANSIG_DCM_FAULTS_DCM_RIGHT_INVERTER_FAULT_OFFSET, uint32_t);
    out_msg->DCM_RightInverterFault_value = DCM_RightInverterFault_val;
    
}

void App_CanUtils_FSM_Warnings_Unpack(const uint8_t* const in_data, FSM_Warnings_Signals* const out_msg)
{
    // Unpack 3-byte payload for message FSM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______Q|PONMLKJI|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_StackWaterMarkHighTask1HzWarning from payload (at bit 0 to bit 1).
    uint32_t FSM_StackWaterMarkHighTask1HzWarning_raw = 0;
    FSM_StackWaterMarkHighTask1HzWarning_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_StackWaterMarkHighTask1HzWarning_val = CAN_DECODE(FSM_StackWaterMarkHighTask1HzWarning_raw, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_OFFSET, uint32_t);
    out_msg->FSM_StackWaterMarkHighTask1HzWarning_value = FSM_StackWaterMarkHighTask1HzWarning_val;
    
    // Unpack 1-bit signal FSM_StackWaterMarkHighTask100HzWarning from payload (at bit 1 to bit 2).
    uint32_t FSM_StackWaterMarkHighTask100HzWarning_raw = 0;
    FSM_StackWaterMarkHighTask100HzWarning_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool FSM_StackWaterMarkHighTask100HzWarning_val = CAN_DECODE(FSM_StackWaterMarkHighTask100HzWarning_raw, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_OFFSET, uint32_t);
    out_msg->FSM_StackWaterMarkHighTask100HzWarning_value = FSM_StackWaterMarkHighTask100HzWarning_val;
    
    // Unpack 1-bit signal FSM_StackWaterMarkHighTask1kHzWarning from payload (at bit 2 to bit 3).
    uint32_t FSM_StackWaterMarkHighTask1kHzWarning_raw = 0;
    FSM_StackWaterMarkHighTask1kHzWarning_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool FSM_StackWaterMarkHighTask1kHzWarning_val = CAN_DECODE(FSM_StackWaterMarkHighTask1kHzWarning_raw, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_OFFSET, uint32_t);
    out_msg->FSM_StackWaterMarkHighTask1kHzWarning_value = FSM_StackWaterMarkHighTask1kHzWarning_val;
    
    // Unpack 1-bit signal FSM_StackWaterMarkHighTaskCanRxWarning from payload (at bit 3 to bit 4).
    uint32_t FSM_StackWaterMarkHighTaskCanRxWarning_raw = 0;
    FSM_StackWaterMarkHighTaskCanRxWarning_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool FSM_StackWaterMarkHighTaskCanRxWarning_val = CAN_DECODE(FSM_StackWaterMarkHighTaskCanRxWarning_raw, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_OFFSET, uint32_t);
    out_msg->FSM_StackWaterMarkHighTaskCanRxWarning_value = FSM_StackWaterMarkHighTaskCanRxWarning_val;
    
    // Unpack 1-bit signal FSM_StackWaterMarkHighTaskCanTxWarning from payload (at bit 4 to bit 5).
    uint32_t FSM_StackWaterMarkHighTaskCanTxWarning_raw = 0;
    FSM_StackWaterMarkHighTaskCanTxWarning_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool FSM_StackWaterMarkHighTaskCanTxWarning_val = CAN_DECODE(FSM_StackWaterMarkHighTaskCanTxWarning_raw, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_OFFSET, uint32_t);
    out_msg->FSM_StackWaterMarkHighTaskCanTxWarning_value = FSM_StackWaterMarkHighTaskCanTxWarning_val;
    
    // Unpack 1-bit signal FSM_WatchdogTimeoutWarning from payload (at bit 5 to bit 6).
    uint32_t FSM_WatchdogTimeoutWarning_raw = 0;
    FSM_WatchdogTimeoutWarning_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool FSM_WatchdogTimeoutWarning_val = CAN_DECODE(FSM_WatchdogTimeoutWarning_raw, CANSIG_FSM_WARNINGS_FSM_WATCHDOG_TIMEOUT_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_WATCHDOG_TIMEOUT_WARNING_OFFSET, uint32_t);
    out_msg->FSM_WatchdogTimeoutWarning_value = FSM_WatchdogTimeoutWarning_val;
    
    // Unpack 1-bit signal FSM_TxOverflowWarning from payload (at bit 6 to bit 7).
    uint32_t FSM_TxOverflowWarning_raw = 0;
    FSM_TxOverflowWarning_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool FSM_TxOverflowWarning_val = CAN_DECODE(FSM_TxOverflowWarning_raw, CANSIG_FSM_WARNINGS_FSM_TX_OVERFLOW_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_TX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_msg->FSM_TxOverflowWarning_value = FSM_TxOverflowWarning_val;
    
    // Unpack 1-bit signal FSM_RxOverflowWarning from payload (at bit 7 to bit 8).
    uint32_t FSM_RxOverflowWarning_raw = 0;
    FSM_RxOverflowWarning_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool FSM_RxOverflowWarning_val = CAN_DECODE(FSM_RxOverflowWarning_raw, CANSIG_FSM_WARNINGS_FSM_RX_OVERFLOW_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_RX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_msg->FSM_RxOverflowWarning_value = FSM_RxOverflowWarning_val;
    
    // Unpack 1-bit signal FSM_LeftWheelSpeedOutOfRangeWarning from payload (at bit 8 to bit 9).
    uint32_t FSM_LeftWheelSpeedOutOfRangeWarning_raw = 0;
    FSM_LeftWheelSpeedOutOfRangeWarning_raw |= unpackShiftRight(in_data[1], 0, 0x1);   // Unpacks bits _______# of msg byte 1
    bool FSM_LeftWheelSpeedOutOfRangeWarning_val = CAN_DECODE(FSM_LeftWheelSpeedOutOfRangeWarning_raw, CANSIG_FSM_WARNINGS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_msg->FSM_LeftWheelSpeedOutOfRangeWarning_value = FSM_LeftWheelSpeedOutOfRangeWarning_val;
    
    // Unpack 1-bit signal FSM_RightWheelSpeedOutOfRangeWarning from payload (at bit 9 to bit 10).
    uint32_t FSM_RightWheelSpeedOutOfRangeWarning_raw = 0;
    FSM_RightWheelSpeedOutOfRangeWarning_raw |= unpackShiftRight(in_data[1], 1, 0x2);   // Unpacks bits ______#_ of msg byte 1
    bool FSM_RightWheelSpeedOutOfRangeWarning_val = CAN_DECODE(FSM_RightWheelSpeedOutOfRangeWarning_raw, CANSIG_FSM_WARNINGS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_msg->FSM_RightWheelSpeedOutOfRangeWarning_value = FSM_RightWheelSpeedOutOfRangeWarning_val;
    
    // Unpack 1-bit signal FSM_SteeringAngleOCSCWarning from payload (at bit 10 to bit 11).
    uint32_t FSM_SteeringAngleOCSCWarning_raw = 0;
    FSM_SteeringAngleOCSCWarning_raw |= unpackShiftRight(in_data[1], 2, 0x4);   // Unpacks bits _____#__ of msg byte 1
    bool FSM_SteeringAngleOCSCWarning_val = CAN_DECODE(FSM_SteeringAngleOCSCWarning_raw, CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OCSC_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OCSC_WARNING_OFFSET, uint32_t);
    out_msg->FSM_SteeringAngleOCSCWarning_value = FSM_SteeringAngleOCSCWarning_val;
    
    // Unpack 1-bit signal FSM_SteeringAngleOutOfRangeWarning from payload (at bit 11 to bit 12).
    uint32_t FSM_SteeringAngleOutOfRangeWarning_raw = 0;
    FSM_SteeringAngleOutOfRangeWarning_raw |= unpackShiftRight(in_data[1], 3, 0x8);   // Unpacks bits ____#___ of msg byte 1
    bool FSM_SteeringAngleOutOfRangeWarning_val = CAN_DECODE(FSM_SteeringAngleOutOfRangeWarning_raw, CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_msg->FSM_SteeringAngleOutOfRangeWarning_value = FSM_SteeringAngleOutOfRangeWarning_val;
    
    // Unpack 1-bit signal FSM_FrontBrakePressureOutOfRangeWarning from payload (at bit 12 to bit 13).
    uint32_t FSM_FrontBrakePressureOutOfRangeWarning_raw = 0;
    FSM_FrontBrakePressureOutOfRangeWarning_raw |= unpackShiftRight(in_data[1], 4, 0x10);   // Unpacks bits ___#____ of msg byte 1
    bool FSM_FrontBrakePressureOutOfRangeWarning_val = CAN_DECODE(FSM_FrontBrakePressureOutOfRangeWarning_raw, CANSIG_FSM_WARNINGS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_msg->FSM_FrontBrakePressureOutOfRangeWarning_value = FSM_FrontBrakePressureOutOfRangeWarning_val;
    
    // Unpack 1-bit signal FSM_RearBrakePressureOutOfRangeWarning from payload (at bit 13 to bit 14).
    uint32_t FSM_RearBrakePressureOutOfRangeWarning_raw = 0;
    FSM_RearBrakePressureOutOfRangeWarning_raw |= unpackShiftRight(in_data[1], 5, 0x20);   // Unpacks bits __#_____ of msg byte 1
    bool FSM_RearBrakePressureOutOfRangeWarning_val = CAN_DECODE(FSM_RearBrakePressureOutOfRangeWarning_raw, CANSIG_FSM_WARNINGS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_msg->FSM_RearBrakePressureOutOfRangeWarning_value = FSM_RearBrakePressureOutOfRangeWarning_val;
    
    // Unpack 1-bit signal FSM_FlowRateOutOfRangeWarning from payload (at bit 14 to bit 15).
    uint32_t FSM_FlowRateOutOfRangeWarning_raw = 0;
    FSM_FlowRateOutOfRangeWarning_raw |= unpackShiftRight(in_data[1], 6, 0x40);   // Unpacks bits _#______ of msg byte 1
    bool FSM_FlowRateOutOfRangeWarning_val = CAN_DECODE(FSM_FlowRateOutOfRangeWarning_raw, CANSIG_FSM_WARNINGS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_msg->FSM_FlowRateOutOfRangeWarning_value = FSM_FlowRateOutOfRangeWarning_val;
    
    // Unpack 1-bit signal FSM_BrakeAppsDisagreementWarning from payload (at bit 15 to bit 16).
    uint32_t FSM_BrakeAppsDisagreementWarning_raw = 0;
    FSM_BrakeAppsDisagreementWarning_raw |= unpackShiftRight(in_data[1], 7, 0x80);   // Unpacks bits #_______ of msg byte 1
    bool FSM_BrakeAppsDisagreementWarning_val = CAN_DECODE(FSM_BrakeAppsDisagreementWarning_raw, CANSIG_FSM_WARNINGS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_OFFSET, uint32_t);
    out_msg->FSM_BrakeAppsDisagreementWarning_value = FSM_BrakeAppsDisagreementWarning_val;
    
    // Unpack 1-bit signal FSM_AppsDisagreementWarning from payload (at bit 16 to bit 17).
    uint32_t FSM_AppsDisagreementWarning_raw = 0;
    FSM_AppsDisagreementWarning_raw |= unpackShiftRight(in_data[2], 0, 0x1);   // Unpacks bits _______# of msg byte 2
    bool FSM_AppsDisagreementWarning_val = CAN_DECODE(FSM_AppsDisagreementWarning_raw, CANSIG_FSM_WARNINGS_FSM_APPS_DISAGREEMENT_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_APPS_DISAGREEMENT_WARNING_OFFSET, uint32_t);
    out_msg->FSM_AppsDisagreementWarning_value = FSM_AppsDisagreementWarning_val;
    
}

void App_CanUtils_FSM_Faults_Unpack(const uint8_t* const in_data, FSM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message FSM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__FEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_MissingHeartbeatFault from payload (at bit 0 to bit 1).
    uint32_t FSM_MissingHeartbeatFault_raw = 0;
    FSM_MissingHeartbeatFault_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_MissingHeartbeatFault_val = CAN_DECODE(FSM_MissingHeartbeatFault_raw, CANSIG_FSM_FAULTS_FSM_MISSING_HEARTBEAT_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_MISSING_HEARTBEAT_FAULT_OFFSET, uint32_t);
    out_msg->FSM_MissingHeartbeatFault_value = FSM_MissingHeartbeatFault_val;
    
    // Unpack 1-bit signal FSM_StateMachineFault from payload (at bit 1 to bit 2).
    uint32_t FSM_StateMachineFault_raw = 0;
    FSM_StateMachineFault_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool FSM_StateMachineFault_val = CAN_DECODE(FSM_StateMachineFault_raw, CANSIG_FSM_FAULTS_FSM_STATE_MACHINE_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_STATE_MACHINE_FAULT_OFFSET, uint32_t);
    out_msg->FSM_StateMachineFault_value = FSM_StateMachineFault_val;
    
    // Unpack 1-bit signal FSM_PappsOCSCFault from payload (at bit 2 to bit 3).
    uint32_t FSM_PappsOCSCFault_raw = 0;
    FSM_PappsOCSCFault_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool FSM_PappsOCSCFault_val = CAN_DECODE(FSM_PappsOCSCFault_raw, CANSIG_FSM_FAULTS_FSM_PAPPS_OCSC_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_PAPPS_OCSC_FAULT_OFFSET, uint32_t);
    out_msg->FSM_PappsOCSCFault_value = FSM_PappsOCSCFault_val;
    
    // Unpack 1-bit signal FSM_SappsOCSCFault from payload (at bit 3 to bit 4).
    uint32_t FSM_SappsOCSCFault_raw = 0;
    FSM_SappsOCSCFault_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool FSM_SappsOCSCFault_val = CAN_DECODE(FSM_SappsOCSCFault_raw, CANSIG_FSM_FAULTS_FSM_SAPPS_OCSC_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_SAPPS_OCSC_FAULT_OFFSET, uint32_t);
    out_msg->FSM_SappsOCSCFault_value = FSM_SappsOCSCFault_val;
    
    // Unpack 1-bit signal FSM_FlowMeterUnderflowFault from payload (at bit 4 to bit 5).
    uint32_t FSM_FlowMeterUnderflowFault_raw = 0;
    FSM_FlowMeterUnderflowFault_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool FSM_FlowMeterUnderflowFault_val = CAN_DECODE(FSM_FlowMeterUnderflowFault_raw, CANSIG_FSM_FAULTS_FSM_FLOW_METER_UNDERFLOW_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_FLOW_METER_UNDERFLOW_FAULT_OFFSET, uint32_t);
    out_msg->FSM_FlowMeterUnderflowFault_value = FSM_FlowMeterUnderflowFault_val;
    
    // Unpack 1-bit signal FSM_TorquePlausabilityFailedFault from payload (at bit 5 to bit 6).
    uint32_t FSM_TorquePlausabilityFailedFault_raw = 0;
    FSM_TorquePlausabilityFailedFault_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool FSM_TorquePlausabilityFailedFault_val = CAN_DECODE(FSM_TorquePlausabilityFailedFault_raw, CANSIG_FSM_FAULTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_OFFSET, uint32_t);
    out_msg->FSM_TorquePlausabilityFailedFault_value = FSM_TorquePlausabilityFailedFault_val;
    
}

void App_CanUtils_PDM_Warnings_Unpack(const uint8_t* const in_data, PDM_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message PDM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal PDM_StackWaterMarkHighTask1HzWarning from payload (at bit 0 to bit 1).
    uint32_t PDM_StackWaterMarkHighTask1HzWarning_raw = 0;
    PDM_StackWaterMarkHighTask1HzWarning_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool PDM_StackWaterMarkHighTask1HzWarning_val = CAN_DECODE(PDM_StackWaterMarkHighTask1HzWarning_raw, CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_SCALE, CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_OFFSET, uint32_t);
    out_msg->PDM_StackWaterMarkHighTask1HzWarning_value = PDM_StackWaterMarkHighTask1HzWarning_val;
    
    // Unpack 1-bit signal PDM_StackWaterMarkHighTask100HzWarning from payload (at bit 1 to bit 2).
    uint32_t PDM_StackWaterMarkHighTask100HzWarning_raw = 0;
    PDM_StackWaterMarkHighTask100HzWarning_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool PDM_StackWaterMarkHighTask100HzWarning_val = CAN_DECODE(PDM_StackWaterMarkHighTask100HzWarning_raw, CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_SCALE, CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_OFFSET, uint32_t);
    out_msg->PDM_StackWaterMarkHighTask100HzWarning_value = PDM_StackWaterMarkHighTask100HzWarning_val;
    
    // Unpack 1-bit signal PDM_StackWaterMarkHighTask1kHzWarning from payload (at bit 2 to bit 3).
    uint32_t PDM_StackWaterMarkHighTask1kHzWarning_raw = 0;
    PDM_StackWaterMarkHighTask1kHzWarning_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool PDM_StackWaterMarkHighTask1kHzWarning_val = CAN_DECODE(PDM_StackWaterMarkHighTask1kHzWarning_raw, CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_SCALE, CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_OFFSET, uint32_t);
    out_msg->PDM_StackWaterMarkHighTask1kHzWarning_value = PDM_StackWaterMarkHighTask1kHzWarning_val;
    
    // Unpack 1-bit signal PDM_StackWaterMarkHighTaskCanRxWarning from payload (at bit 3 to bit 4).
    uint32_t PDM_StackWaterMarkHighTaskCanRxWarning_raw = 0;
    PDM_StackWaterMarkHighTaskCanRxWarning_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool PDM_StackWaterMarkHighTaskCanRxWarning_val = CAN_DECODE(PDM_StackWaterMarkHighTaskCanRxWarning_raw, CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_SCALE, CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_OFFSET, uint32_t);
    out_msg->PDM_StackWaterMarkHighTaskCanRxWarning_value = PDM_StackWaterMarkHighTaskCanRxWarning_val;
    
    // Unpack 1-bit signal PDM_StackWaterMarkHighTaskCanTxWarning from payload (at bit 4 to bit 5).
    uint32_t PDM_StackWaterMarkHighTaskCanTxWarning_raw = 0;
    PDM_StackWaterMarkHighTaskCanTxWarning_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool PDM_StackWaterMarkHighTaskCanTxWarning_val = CAN_DECODE(PDM_StackWaterMarkHighTaskCanTxWarning_raw, CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_SCALE, CANSIG_PDM_WARNINGS_PDM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_OFFSET, uint32_t);
    out_msg->PDM_StackWaterMarkHighTaskCanTxWarning_value = PDM_StackWaterMarkHighTaskCanTxWarning_val;
    
    // Unpack 1-bit signal PDM_WatchdogTimeoutWarning from payload (at bit 5 to bit 6).
    uint32_t PDM_WatchdogTimeoutWarning_raw = 0;
    PDM_WatchdogTimeoutWarning_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool PDM_WatchdogTimeoutWarning_val = CAN_DECODE(PDM_WatchdogTimeoutWarning_raw, CANSIG_PDM_WARNINGS_PDM_WATCHDOG_TIMEOUT_WARNING_SCALE, CANSIG_PDM_WARNINGS_PDM_WATCHDOG_TIMEOUT_WARNING_OFFSET, uint32_t);
    out_msg->PDM_WatchdogTimeoutWarning_value = PDM_WatchdogTimeoutWarning_val;
    
    // Unpack 1-bit signal PDM_TxOverflowWarning from payload (at bit 6 to bit 7).
    uint32_t PDM_TxOverflowWarning_raw = 0;
    PDM_TxOverflowWarning_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool PDM_TxOverflowWarning_val = CAN_DECODE(PDM_TxOverflowWarning_raw, CANSIG_PDM_WARNINGS_PDM_TX_OVERFLOW_WARNING_SCALE, CANSIG_PDM_WARNINGS_PDM_TX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_msg->PDM_TxOverflowWarning_value = PDM_TxOverflowWarning_val;
    
    // Unpack 1-bit signal PDM_RxOverflowWarning from payload (at bit 7 to bit 8).
    uint32_t PDM_RxOverflowWarning_raw = 0;
    PDM_RxOverflowWarning_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool PDM_RxOverflowWarning_val = CAN_DECODE(PDM_RxOverflowWarning_raw, CANSIG_PDM_WARNINGS_PDM_RX_OVERFLOW_WARNING_SCALE, CANSIG_PDM_WARNINGS_PDM_RX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_msg->PDM_RxOverflowWarning_value = PDM_RxOverflowWarning_val;
    
}

void App_CanUtils_PDM_Faults_Unpack(const uint8_t* const in_data, PDM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message PDM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal PDM_DummyFault from payload (at bit 0 to bit 1).
    uint32_t PDM_DummyFault_raw = 0;
    PDM_DummyFault_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool PDM_DummyFault_val = CAN_DECODE(PDM_DummyFault_raw, CANSIG_PDM_FAULTS_PDM_DUMMY_FAULT_SCALE, CANSIG_PDM_FAULTS_PDM_DUMMY_FAULT_OFFSET, uint32_t);
    out_msg->PDM_DummyFault_value = PDM_DummyFault_val;
    
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

