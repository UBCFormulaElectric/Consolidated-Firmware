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

void App_CanUtils_GSM_GSM_Vitals_Pack(const GSM_GSM_Vitals_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message GSM_GSM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal GSM_Heartbeat into payload (at bit 0 to bit 1).
    const bool GSM_Heartbeat_val = in_msg->GSM_Heartbeat_value;
    const uint32_t GSM_Heartbeat_raw = CAN_ENCODE(GSM_Heartbeat_val, CANSIG_GSM_GSM_VITALS_GSM_HEARTBEAT_SCALE, CANSIG_GSM_GSM_VITALS_GSM_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(GSM_Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
}

void App_CanUtils_GSM_GSM_Warnings_Pack(const GSM_GSM_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 4-byte payload for message GSM_GSM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______I|IIHHHHHH|HHGGGGGG|GGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal GSM_StackWatermarkAboveThresholdTask1Hz into payload (at bit 0 to bit 1).
    const bool GSM_StackWatermarkAboveThresholdTask1Hz_val = in_msg->GSM_StackWatermarkAboveThresholdTask1Hz_value;
    const uint32_t GSM_StackWatermarkAboveThresholdTask1Hz_raw = CAN_ENCODE(GSM_StackWatermarkAboveThresholdTask1Hz_val, CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_SCALE, CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(GSM_StackWatermarkAboveThresholdTask1Hz_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal GSM_StackWatermarkAboveThresholdTask100Hz into payload (at bit 1 to bit 2).
    const bool GSM_StackWatermarkAboveThresholdTask100Hz_val = in_msg->GSM_StackWatermarkAboveThresholdTask100Hz_value;
    const uint32_t GSM_StackWatermarkAboveThresholdTask100Hz_raw = CAN_ENCODE(GSM_StackWatermarkAboveThresholdTask100Hz_val, CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK100_HZ_SCALE, CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(GSM_StackWatermarkAboveThresholdTask100Hz_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal GSM_StackWatermarkAboveThresholdTask1kHz into payload (at bit 2 to bit 3).
    const bool GSM_StackWatermarkAboveThresholdTask1kHz_val = in_msg->GSM_StackWatermarkAboveThresholdTask1kHz_value;
    const uint32_t GSM_StackWatermarkAboveThresholdTask1kHz_raw = CAN_ENCODE(GSM_StackWatermarkAboveThresholdTask1kHz_val, CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_SCALE, CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(GSM_StackWatermarkAboveThresholdTask1kHz_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal GSM_StackWatermarkAboveThresholdTaskCanTx into payload (at bit 3 to bit 4).
    const bool GSM_StackWatermarkAboveThresholdTaskCanTx_val = in_msg->GSM_StackWatermarkAboveThresholdTaskCanTx_value;
    const uint32_t GSM_StackWatermarkAboveThresholdTaskCanTx_raw = CAN_ENCODE(GSM_StackWatermarkAboveThresholdTaskCanTx_val, CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_SCALE, CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(GSM_StackWatermarkAboveThresholdTaskCanTx_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal GSM_StackWatermarkAboveThresholdTaskCanRx into payload (at bit 4 to bit 5).
    const bool GSM_StackWatermarkAboveThresholdTaskCanRx_val = in_msg->GSM_StackWatermarkAboveThresholdTaskCanRx_value;
    const uint32_t GSM_StackWatermarkAboveThresholdTaskCanRx_raw = CAN_ENCODE(GSM_StackWatermarkAboveThresholdTaskCanRx_val, CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_SCALE, CANSIG_GSM_GSM_WARNINGS_GSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(GSM_StackWatermarkAboveThresholdTaskCanRx_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal GSM_WatchdogTimeout into payload (at bit 5 to bit 6).
    const bool GSM_WatchdogTimeout_val = in_msg->GSM_WatchdogTimeout_value;
    const uint32_t GSM_WatchdogTimeout_raw = CAN_ENCODE(GSM_WatchdogTimeout_val, CANSIG_GSM_GSM_WARNINGS_GSM_WATCHDOG_TIMEOUT_SCALE, CANSIG_GSM_GSM_WARNINGS_GSM_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(GSM_WatchdogTimeout_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 8-bit signal GSM_TxOverflowCount into payload (at bit 6 to bit 14).
    const uint32_t GSM_TxOverflowCount_val = in_msg->GSM_TxOverflowCount_value;
    const uint32_t GSM_TxOverflowCount_raw = CAN_ENCODE(GSM_TxOverflowCount_val, CANSIG_GSM_GSM_WARNINGS_GSM_TX_OVERFLOW_COUNT_SCALE, CANSIG_GSM_GSM_WARNINGS_GSM_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(GSM_TxOverflowCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(GSM_TxOverflowCount_raw, 2, 0x3f);   // Packs bits __###### of byte 1
    
    // Pack 8-bit signal GSM_RxOverflowCount into payload (at bit 14 to bit 22).
    const uint32_t GSM_RxOverflowCount_val = in_msg->GSM_RxOverflowCount_value;
    const uint32_t GSM_RxOverflowCount_raw = CAN_ENCODE(GSM_RxOverflowCount_val, CANSIG_GSM_GSM_WARNINGS_GSM_RX_OVERFLOW_COUNT_SCALE, CANSIG_GSM_GSM_WARNINGS_GSM_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(GSM_RxOverflowCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 1
    out_data[2] |= packShiftRight(GSM_RxOverflowCount_raw, 2, 0x3f);   // Packs bits __###### of byte 2
    
    // Pack 3-bit signal GSM_WatchdogTimeoutTaskName into payload (at bit 22 to bit 25).
    const RtosTaskName GSM_WatchdogTimeoutTaskName_val = in_msg->GSM_WatchdogTimeoutTaskName_value;
    const uint32_t GSM_WatchdogTimeoutTaskName_raw = CAN_ENCODE(GSM_WatchdogTimeoutTaskName_val, CANSIG_GSM_GSM_WARNINGS_GSM_WATCHDOG_TIMEOUT_TASK_NAME_SCALE, CANSIG_GSM_GSM_WARNINGS_GSM_WATCHDOG_TIMEOUT_TASK_NAME_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(GSM_WatchdogTimeoutTaskName_raw, 6, 0xc0);   // Packs bits ##______ of byte 2
    out_data[3] |= packShiftRight(GSM_WatchdogTimeoutTaskName_raw, 2, 0x1);   // Packs bits _______# of byte 3
    
}

void App_CanUtils_GSM_StackRemaining_Pack(const GSM_StackRemaining_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message GSM_StackRemaining.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_____EEE|EEEEDDDD|DDDCCCCC|CCBBBBBB|BAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 7-bit signal GSM_StackRemainingTask1Hz into payload (at bit 0 to bit 7).
    const uint32_t GSM_StackRemainingTask1Hz_val = in_msg->GSM_StackRemainingTask1Hz_value;
    const uint32_t GSM_StackRemainingTask1Hz_raw = CAN_ENCODE(GSM_StackRemainingTask1Hz_val, CANSIG_GSM_STACK_REMAINING_GSM_STACK_REMAINING_TASK1_HZ_SCALE, CANSIG_GSM_STACK_REMAINING_GSM_STACK_REMAINING_TASK1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(GSM_StackRemainingTask1Hz_raw, 0, 0x7f);   // Packs bits _####### of byte 0
    
    // Pack 7-bit signal GSM_StackRemainingTask100Hz into payload (at bit 7 to bit 14).
    const uint32_t GSM_StackRemainingTask100Hz_val = in_msg->GSM_StackRemainingTask100Hz_value;
    const uint32_t GSM_StackRemainingTask100Hz_raw = CAN_ENCODE(GSM_StackRemainingTask100Hz_val, CANSIG_GSM_STACK_REMAINING_GSM_STACK_REMAINING_TASK100_HZ_SCALE, CANSIG_GSM_STACK_REMAINING_GSM_STACK_REMAINING_TASK100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(GSM_StackRemainingTask100Hz_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    out_data[1] |= packShiftRight(GSM_StackRemainingTask100Hz_raw, 1, 0x3f);   // Packs bits __###### of byte 1
    
    // Pack 7-bit signal GSM_StackRemainingTask1kHz into payload (at bit 14 to bit 21).
    const uint32_t GSM_StackRemainingTask1kHz_val = in_msg->GSM_StackRemainingTask1kHz_value;
    const uint32_t GSM_StackRemainingTask1kHz_raw = CAN_ENCODE(GSM_StackRemainingTask1kHz_val, CANSIG_GSM_STACK_REMAINING_GSM_STACK_REMAINING_TASK1K_HZ_SCALE, CANSIG_GSM_STACK_REMAINING_GSM_STACK_REMAINING_TASK1K_HZ_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(GSM_StackRemainingTask1kHz_raw, 6, 0xc0);   // Packs bits ##______ of byte 1
    out_data[2] |= packShiftRight(GSM_StackRemainingTask1kHz_raw, 2, 0x1f);   // Packs bits ___##### of byte 2
    
    // Pack 7-bit signal GSM_StackRemainingTaskCanRx into payload (at bit 21 to bit 28).
    const uint32_t GSM_StackRemainingTaskCanRx_val = in_msg->GSM_StackRemainingTaskCanRx_value;
    const uint32_t GSM_StackRemainingTaskCanRx_raw = CAN_ENCODE(GSM_StackRemainingTaskCanRx_val, CANSIG_GSM_STACK_REMAINING_GSM_STACK_REMAINING_TASK_CAN_RX_SCALE, CANSIG_GSM_STACK_REMAINING_GSM_STACK_REMAINING_TASK_CAN_RX_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(GSM_StackRemainingTaskCanRx_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    out_data[3] |= packShiftRight(GSM_StackRemainingTaskCanRx_raw, 3, 0xf);   // Packs bits ____#### of byte 3
    
    // Pack 7-bit signal GSM_StackRemainingTaskCanTx into payload (at bit 28 to bit 35).
    const uint32_t GSM_StackRemainingTaskCanTx_val = in_msg->GSM_StackRemainingTaskCanTx_value;
    const uint32_t GSM_StackRemainingTaskCanTx_raw = CAN_ENCODE(GSM_StackRemainingTaskCanTx_val, CANSIG_GSM_STACK_REMAINING_GSM_STACK_REMAINING_TASK_CAN_TX_SCALE, CANSIG_GSM_STACK_REMAINING_GSM_STACK_REMAINING_TASK_CAN_TX_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(GSM_StackRemainingTaskCanTx_raw, 4, 0xf0);   // Packs bits ####____ of byte 3
    out_data[4] |= packShiftRight(GSM_StackRemainingTaskCanTx_raw, 4, 0x7);   // Packs bits _____### of byte 4
    
}

void App_CanUtils_GSM_CommitInfo_Pack(const GSM_CommitInfo_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message GSM_CommitInfo.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_______B|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 32-bit signal GSM_Hash into payload (at bit 0 to bit 32).
    const uint32_t GSM_Hash_val = in_msg->GSM_Hash_value;
    const uint32_t GSM_Hash_raw = CAN_ENCODE(GSM_Hash_val, CANSIG_GSM_COMMIT_INFO_GSM_HASH_SCALE, CANSIG_GSM_COMMIT_INFO_GSM_HASH_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(GSM_Hash_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(GSM_Hash_raw, 8, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(GSM_Hash_raw, 16, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(GSM_Hash_raw, 24, 0xff);   // Packs bits ######## of byte 3
    
    // Pack 1-bit signal GSM_Clean into payload (at bit 32 to bit 33).
    const bool GSM_Clean_val = in_msg->GSM_Clean_value;
    const uint32_t GSM_Clean_raw = CAN_ENCODE(GSM_Clean_val, CANSIG_GSM_COMMIT_INFO_GSM_CLEAN_SCALE, CANSIG_GSM_COMMIT_INFO_GSM_CLEAN_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(GSM_Clean_raw, 0, 0x1);   // Packs bits _______# of byte 4
    
}

void App_CanUtils_DIM_Warnings_Unpack(const uint8_t* const in_data, DIM_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DIM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DIM_StackWaterMarkHighTask1HzWarning from payload (at bit 0 to bit 1).
    uint32_t DIM_StackWaterMarkHighTask1HzWarning_raw = 0;
    DIM_StackWaterMarkHighTask1HzWarning_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DIM_StackWaterMarkHighTask1HzWarning_val = CAN_DECODE(DIM_StackWaterMarkHighTask1HzWarning_raw, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_OFFSET, uint32_t);
    out_msg->DIM_StackWaterMarkHighTask1HzWarning_value = DIM_StackWaterMarkHighTask1HzWarning_val;
    
    // Unpack 1-bit signal DIM_StackWaterMarkHighTask100HzWarning from payload (at bit 1 to bit 2).
    uint32_t DIM_StackWaterMarkHighTask100HzWarning_raw = 0;
    DIM_StackWaterMarkHighTask100HzWarning_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool DIM_StackWaterMarkHighTask100HzWarning_val = CAN_DECODE(DIM_StackWaterMarkHighTask100HzWarning_raw, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_OFFSET, uint32_t);
    out_msg->DIM_StackWaterMarkHighTask100HzWarning_value = DIM_StackWaterMarkHighTask100HzWarning_val;
    
    // Unpack 1-bit signal DIM_StackWaterMarkHighTask1kHzWarning from payload (at bit 2 to bit 3).
    uint32_t DIM_StackWaterMarkHighTask1kHzWarning_raw = 0;
    DIM_StackWaterMarkHighTask1kHzWarning_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool DIM_StackWaterMarkHighTask1kHzWarning_val = CAN_DECODE(DIM_StackWaterMarkHighTask1kHzWarning_raw, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_OFFSET, uint32_t);
    out_msg->DIM_StackWaterMarkHighTask1kHzWarning_value = DIM_StackWaterMarkHighTask1kHzWarning_val;
    
    // Unpack 1-bit signal DIM_StackWaterMarkHighTaskCanRxWarning from payload (at bit 3 to bit 4).
    uint32_t DIM_StackWaterMarkHighTaskCanRxWarning_raw = 0;
    DIM_StackWaterMarkHighTaskCanRxWarning_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool DIM_StackWaterMarkHighTaskCanRxWarning_val = CAN_DECODE(DIM_StackWaterMarkHighTaskCanRxWarning_raw, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_OFFSET, uint32_t);
    out_msg->DIM_StackWaterMarkHighTaskCanRxWarning_value = DIM_StackWaterMarkHighTaskCanRxWarning_val;
    
    // Unpack 1-bit signal DIM_StackWaterMarkHighTaskCanTxWarning from payload (at bit 4 to bit 5).
    uint32_t DIM_StackWaterMarkHighTaskCanTxWarning_raw = 0;
    DIM_StackWaterMarkHighTaskCanTxWarning_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool DIM_StackWaterMarkHighTaskCanTxWarning_val = CAN_DECODE(DIM_StackWaterMarkHighTaskCanTxWarning_raw, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_OFFSET, uint32_t);
    out_msg->DIM_StackWaterMarkHighTaskCanTxWarning_value = DIM_StackWaterMarkHighTaskCanTxWarning_val;
    
    // Unpack 1-bit signal DIM_WatchdogTimeoutWarning from payload (at bit 5 to bit 6).
    uint32_t DIM_WatchdogTimeoutWarning_raw = 0;
    DIM_WatchdogTimeoutWarning_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool DIM_WatchdogTimeoutWarning_val = CAN_DECODE(DIM_WatchdogTimeoutWarning_raw, CANSIG_DIM_WARNINGS_DIM_WATCHDOG_TIMEOUT_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_WATCHDOG_TIMEOUT_WARNING_OFFSET, uint32_t);
    out_msg->DIM_WatchdogTimeoutWarning_value = DIM_WatchdogTimeoutWarning_val;
    
    // Unpack 1-bit signal DIM_TxOverflowWarning from payload (at bit 6 to bit 7).
    uint32_t DIM_TxOverflowWarning_raw = 0;
    DIM_TxOverflowWarning_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool DIM_TxOverflowWarning_val = CAN_DECODE(DIM_TxOverflowWarning_raw, CANSIG_DIM_WARNINGS_DIM_TX_OVERFLOW_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_TX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_msg->DIM_TxOverflowWarning_value = DIM_TxOverflowWarning_val;
    
    // Unpack 1-bit signal DIM_RxOverflowWarning from payload (at bit 7 to bit 8).
    uint32_t DIM_RxOverflowWarning_raw = 0;
    DIM_RxOverflowWarning_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool DIM_RxOverflowWarning_val = CAN_DECODE(DIM_RxOverflowWarning_raw, CANSIG_DIM_WARNINGS_DIM_RX_OVERFLOW_WARNING_SCALE, CANSIG_DIM_WARNINGS_DIM_RX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_msg->DIM_RxOverflowWarning_value = DIM_RxOverflowWarning_val;
    
}

void App_CanUtils_DIM_Faults_Unpack(const uint8_t* const in_data, DIM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DIM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DIM_MissingHeartbeatFault from payload (at bit 0 to bit 1).
    uint32_t DIM_MissingHeartbeatFault_raw = 0;
    DIM_MissingHeartbeatFault_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DIM_MissingHeartbeatFault_val = CAN_DECODE(DIM_MissingHeartbeatFault_raw, CANSIG_DIM_FAULTS_DIM_MISSING_HEARTBEAT_FAULT_SCALE, CANSIG_DIM_FAULTS_DIM_MISSING_HEARTBEAT_FAULT_OFFSET, uint32_t);
    out_msg->DIM_MissingHeartbeatFault_value = DIM_MissingHeartbeatFault_val;
    
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

