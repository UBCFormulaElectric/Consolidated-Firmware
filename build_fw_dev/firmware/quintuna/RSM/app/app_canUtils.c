/**
 * @note This file is auto-generated. Do not modify!
 */
// clang-format off

#include <stddef.h>
#include "app_canUtils.h"

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
 * Encode real signal value to payload representation, w/ scale and offset (unsigned).
 */
#define CAN_ENCODE(input, scale, offset, type) ((uint32_t)((input - offset) / scale))

/**
 * Encode real signal value to payload representation, w/ scale and offset (signed).
 */
#define CAN_SIGNED_ENCODE(input, scale, offset, type) ((int32_t)((input - offset) / scale))

/**
 * Decode payload representation of signal to signal value, w/ scale and offset.
 */
#define CAN_DECODE(input, scale, offset, type) ((type)((type)input * (type)scale + (type)offset))

/**
 * Interpret input as a signed number of length bits via 2s complement.
 */
#define SIGNED_DECODE(input, bits, mask) (((int)((input & (1 << (bits - 1))) ? -((~input & mask) + 1) : input)))

/* ------------------------- Function Definitions ------------------------- */

void app_canUtils_RSM_Vitals_pack(const RSM_Vitals_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message RSM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal RSM_Heartbeat into payload (at bit 0 to bit 1).
    const bool RSM_Heartbeat_val = in_msg->RSM_Heartbeat_value;
    const uint32_t RSM_Heartbeat_raw = CAN_ENCODE(RSM_Heartbeat_val, CANSIG_RSM_VITALS_RSM_HEARTBEAT_SCALE, CANSIG_RSM_VITALS_RSM_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(RSM_Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal RSM_State into payload (at bit 1 to bit 2).
    const RsmStates RSM_State_val = in_msg->RSM_State_value;
    const uint32_t RSM_State_raw = CAN_ENCODE(RSM_State_val, CANSIG_RSM_VITALS_RSM_STATE_SCALE, CANSIG_RSM_VITALS_RSM_STATE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(RSM_State_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
}

void app_canUtils_RSM_Warnings_pack(const RSM_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // No data to pack!
}

void app_canUtils_RSM_Faults_pack(const RSM_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message RSM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal RSM_Fault_DummyFault into payload (at bit 0 to bit 1).
    const bool RSM_Fault_DummyFault_val = in_msg->RSM_Fault_DummyFault_value;
    const uint32_t RSM_Fault_DummyFault_raw = CAN_ENCODE(RSM_Fault_DummyFault_val, CANSIG_RSM_FAULTS_RSM_FAULT_DUMMY_FAULT_SCALE, CANSIG_RSM_FAULTS_RSM_FAULT_DUMMY_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(RSM_Fault_DummyFault_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
}

void app_canUtils_RSM_WarningsCounts_pack(const RSM_WarningsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // No data to pack!
}

void app_canUtils_RSM_FaultsCounts_pack(const RSM_FaultsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message RSM_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____AAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal RSM_Fault_DummyFaultCount into payload (at bit 0 to bit 3).
    const uint32_t RSM_Fault_DummyFaultCount_val = in_msg->RSM_Fault_DummyFaultCount_value;
    const uint32_t RSM_Fault_DummyFaultCount_raw = CAN_ENCODE(RSM_Fault_DummyFaultCount_val, CANSIG_RSM_FAULTS_COUNTS_RSM_FAULT_DUMMY_FAULT_COUNT_SCALE, CANSIG_RSM_FAULTS_COUNTS_RSM_FAULT_DUMMY_FAULT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(RSM_Fault_DummyFaultCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
}

void app_canUtils_BMS_Warnings_unpack(const uint8_t* const in_data, BMS_Warnings_Signals* const out_msg)
{
    // Unpack 0-byte payload for message BMS_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
}

void app_canUtils_BMS_Faults_unpack(const uint8_t* const in_data, BMS_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message BMS_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal BMS_Fault_DummyFault from payload (at bit 0 to bit 1).
    uint32_t BMS_Fault_DummyFault_raw = 0;
    BMS_Fault_DummyFault_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool BMS_Fault_DummyFault_val = CAN_DECODE(BMS_Fault_DummyFault_raw, CANSIG_BMS_FAULTS_BMS_FAULT_DUMMY_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_DUMMY_FAULT_OFFSET, uint32_t);
    out_msg->BMS_Fault_DummyFault_value = BMS_Fault_DummyFault_val;
    
}

void app_canUtils_FSM_Warnings_unpack(const uint8_t* const in_data, FSM_Warnings_Signals* const out_msg)
{
    // Unpack 0-byte payload for message FSM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
}

void app_canUtils_FSM_Faults_unpack(const uint8_t* const in_data, FSM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message FSM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_Fault_DummyFault from payload (at bit 0 to bit 1).
    uint32_t FSM_Fault_DummyFault_raw = 0;
    FSM_Fault_DummyFault_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_Fault_DummyFault_val = CAN_DECODE(FSM_Fault_DummyFault_raw, CANSIG_FSM_FAULTS_FSM_FAULT_DUMMY_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_DUMMY_FAULT_OFFSET, uint32_t);
    out_msg->FSM_Fault_DummyFault_value = FSM_Fault_DummyFault_val;
    
}

void app_canUtils_CRIT_Warnings_unpack(const uint8_t* const in_data, CRIT_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message CRIT_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal CRIT_Warning_StackWaterMarkHighTask1Hz from payload (at bit 0 to bit 1).
    uint32_t CRIT_Warning_StackWaterMarkHighTask1Hz_raw = 0;
    CRIT_Warning_StackWaterMarkHighTask1Hz_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool CRIT_Warning_StackWaterMarkHighTask1Hz_val = CAN_DECODE(CRIT_Warning_StackWaterMarkHighTask1Hz_raw, CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_msg->CRIT_Warning_StackWaterMarkHighTask1Hz_value = CRIT_Warning_StackWaterMarkHighTask1Hz_val;
    
    // Unpack 1-bit signal CRIT_Warning_StackWaterMarkHighTask100Hz from payload (at bit 1 to bit 2).
    uint32_t CRIT_Warning_StackWaterMarkHighTask100Hz_raw = 0;
    CRIT_Warning_StackWaterMarkHighTask100Hz_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool CRIT_Warning_StackWaterMarkHighTask100Hz_val = CAN_DECODE(CRIT_Warning_StackWaterMarkHighTask100Hz_raw, CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_msg->CRIT_Warning_StackWaterMarkHighTask100Hz_value = CRIT_Warning_StackWaterMarkHighTask100Hz_val;
    
    // Unpack 1-bit signal CRIT_Warning_StackWaterMarkHighTask1kHz from payload (at bit 2 to bit 3).
    uint32_t CRIT_Warning_StackWaterMarkHighTask1kHz_raw = 0;
    CRIT_Warning_StackWaterMarkHighTask1kHz_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool CRIT_Warning_StackWaterMarkHighTask1kHz_val = CAN_DECODE(CRIT_Warning_StackWaterMarkHighTask1kHz_raw, CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_msg->CRIT_Warning_StackWaterMarkHighTask1kHz_value = CRIT_Warning_StackWaterMarkHighTask1kHz_val;
    
    // Unpack 1-bit signal CRIT_Warning_StackWaterMarkHighTaskCanRx from payload (at bit 3 to bit 4).
    uint32_t CRIT_Warning_StackWaterMarkHighTaskCanRx_raw = 0;
    CRIT_Warning_StackWaterMarkHighTaskCanRx_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool CRIT_Warning_StackWaterMarkHighTaskCanRx_val = CAN_DECODE(CRIT_Warning_StackWaterMarkHighTaskCanRx_raw, CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_msg->CRIT_Warning_StackWaterMarkHighTaskCanRx_value = CRIT_Warning_StackWaterMarkHighTaskCanRx_val;
    
    // Unpack 1-bit signal CRIT_Warning_StackWaterMarkHighTaskCanTx from payload (at bit 4 to bit 5).
    uint32_t CRIT_Warning_StackWaterMarkHighTaskCanTx_raw = 0;
    CRIT_Warning_StackWaterMarkHighTaskCanTx_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool CRIT_Warning_StackWaterMarkHighTaskCanTx_val = CAN_DECODE(CRIT_Warning_StackWaterMarkHighTaskCanTx_raw, CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_CRIT_WARNINGS_CRIT_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_msg->CRIT_Warning_StackWaterMarkHighTaskCanTx_value = CRIT_Warning_StackWaterMarkHighTaskCanTx_val;
    
    // Unpack 1-bit signal CRIT_Warning_WatchdogTimeout from payload (at bit 5 to bit 6).
    uint32_t CRIT_Warning_WatchdogTimeout_raw = 0;
    CRIT_Warning_WatchdogTimeout_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool CRIT_Warning_WatchdogTimeout_val = CAN_DECODE(CRIT_Warning_WatchdogTimeout_raw, CANSIG_CRIT_WARNINGS_CRIT_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_CRIT_WARNINGS_CRIT_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_msg->CRIT_Warning_WatchdogTimeout_value = CRIT_Warning_WatchdogTimeout_val;
    
    // Unpack 1-bit signal CRIT_Warning_TxOverflow from payload (at bit 6 to bit 7).
    uint32_t CRIT_Warning_TxOverflow_raw = 0;
    CRIT_Warning_TxOverflow_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool CRIT_Warning_TxOverflow_val = CAN_DECODE(CRIT_Warning_TxOverflow_raw, CANSIG_CRIT_WARNINGS_CRIT_WARNING_TX_OVERFLOW_SCALE, CANSIG_CRIT_WARNINGS_CRIT_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_msg->CRIT_Warning_TxOverflow_value = CRIT_Warning_TxOverflow_val;
    
    // Unpack 1-bit signal CRIT_Warning_RxOverflow from payload (at bit 7 to bit 8).
    uint32_t CRIT_Warning_RxOverflow_raw = 0;
    CRIT_Warning_RxOverflow_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool CRIT_Warning_RxOverflow_val = CAN_DECODE(CRIT_Warning_RxOverflow_raw, CANSIG_CRIT_WARNINGS_CRIT_WARNING_RX_OVERFLOW_SCALE, CANSIG_CRIT_WARNINGS_CRIT_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_msg->CRIT_Warning_RxOverflow_value = CRIT_Warning_RxOverflow_val;
    
}

void app_canUtils_CRIT_Faults_unpack(const uint8_t* const in_data, CRIT_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message CRIT_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____DCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal CRIT_Fault_MissingBMSHeartbeat from payload (at bit 0 to bit 1).
    uint32_t CRIT_Fault_MissingBMSHeartbeat_raw = 0;
    CRIT_Fault_MissingBMSHeartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool CRIT_Fault_MissingBMSHeartbeat_val = CAN_DECODE(CRIT_Fault_MissingBMSHeartbeat_raw, CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_BMS_HEARTBEAT_SCALE, CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_BMS_HEARTBEAT_OFFSET, uint32_t);
    out_msg->CRIT_Fault_MissingBMSHeartbeat_value = CRIT_Fault_MissingBMSHeartbeat_val;
    
    // Unpack 1-bit signal CRIT_Fault_MissingFSMHeartbeat from payload (at bit 1 to bit 2).
    uint32_t CRIT_Fault_MissingFSMHeartbeat_raw = 0;
    CRIT_Fault_MissingFSMHeartbeat_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool CRIT_Fault_MissingFSMHeartbeat_val = CAN_DECODE(CRIT_Fault_MissingFSMHeartbeat_raw, CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_FSM_HEARTBEAT_SCALE, CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_FSM_HEARTBEAT_OFFSET, uint32_t);
    out_msg->CRIT_Fault_MissingFSMHeartbeat_value = CRIT_Fault_MissingFSMHeartbeat_val;
    
    // Unpack 1-bit signal CRIT_Fault_MissingVCHeartbeat from payload (at bit 2 to bit 3).
    uint32_t CRIT_Fault_MissingVCHeartbeat_raw = 0;
    CRIT_Fault_MissingVCHeartbeat_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool CRIT_Fault_MissingVCHeartbeat_val = CAN_DECODE(CRIT_Fault_MissingVCHeartbeat_raw, CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_VC_HEARTBEAT_SCALE, CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_VC_HEARTBEAT_OFFSET, uint32_t);
    out_msg->CRIT_Fault_MissingVCHeartbeat_value = CRIT_Fault_MissingVCHeartbeat_val;
    
    // Unpack 1-bit signal CRIT_Fault_MissingRSMHeartbeat from payload (at bit 3 to bit 4).
    uint32_t CRIT_Fault_MissingRSMHeartbeat_raw = 0;
    CRIT_Fault_MissingRSMHeartbeat_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool CRIT_Fault_MissingRSMHeartbeat_val = CAN_DECODE(CRIT_Fault_MissingRSMHeartbeat_raw, CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_RSM_HEARTBEAT_SCALE, CANSIG_CRIT_FAULTS_CRIT_FAULT_MISSING_RSM_HEARTBEAT_OFFSET, uint32_t);
    out_msg->CRIT_Fault_MissingRSMHeartbeat_value = CRIT_Fault_MissingRSMHeartbeat_val;
    
}

void app_canUtils_VC_Warnings_unpack(const uint8_t* const in_data, VC_Warnings_Signals* const out_msg)
{
    // Unpack 0-byte payload for message VC_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
}

void app_canUtils_VC_Faults_unpack(const uint8_t* const in_data, VC_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message VC_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal VC_Fault_DummyFault from payload (at bit 0 to bit 1).
    uint32_t VC_Fault_DummyFault_raw = 0;
    VC_Fault_DummyFault_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool VC_Fault_DummyFault_val = CAN_DECODE(VC_Fault_DummyFault_raw, CANSIG_VC_FAULTS_VC_FAULT_DUMMY_FAULT_SCALE, CANSIG_VC_FAULTS_VC_FAULT_DUMMY_FAULT_OFFSET, uint32_t);
    out_msg->VC_Fault_DummyFault_value = VC_Fault_DummyFault_val;
    
}

