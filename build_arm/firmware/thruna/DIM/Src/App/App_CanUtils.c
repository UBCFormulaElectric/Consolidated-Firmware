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
    
    // Pack 1-bit signal Heartbeat into payload (at bit 0 to bit 1).
    const bool Heartbeat_val = in_msg->Heartbeat_value;
    const uint32_t Heartbeat_raw = CAN_ENCODE(Heartbeat_val, CANSIG_DIM_VITALS_HEARTBEAT_SCALE, CANSIG_DIM_VITALS_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal State into payload (at bit 1 to bit 2).
    const DimState State_val = in_msg->State_value;
    const uint32_t State_raw = CAN_ENCODE(State_val, CANSIG_DIM_VITALS_STATE_SCALE, CANSIG_DIM_VITALS_STATE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(State_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
}

void App_CanUtils_DIM_Switches_Pack(const DIM_Switches_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DIM_Switches.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal StartSwitch into payload (at bit 0 to bit 1).
    const SwitchState StartSwitch_val = in_msg->StartSwitch_value;
    const uint32_t StartSwitch_raw = CAN_ENCODE(StartSwitch_val, CANSIG_DIM_SWITCHES_START_SWITCH_SCALE, CANSIG_DIM_SWITCHES_START_SWITCH_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(StartSwitch_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal AuxSwitch into payload (at bit 1 to bit 2).
    const SwitchState AuxSwitch_val = in_msg->AuxSwitch_value;
    const uint32_t AuxSwitch_raw = CAN_ENCODE(AuxSwitch_val, CANSIG_DIM_SWITCHES_AUX_SWITCH_SCALE, CANSIG_DIM_SWITCHES_AUX_SWITCH_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(AuxSwitch_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
}

void App_CanUtils_DIM_AlertsContext_Pack(const DIM_AlertsContext_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message DIM_AlertsContext.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CCC|BBBBBBBB|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal TxOverflowCount into payload (at bit 0 to bit 8).
    const uint32_t TxOverflowCount_val = in_msg->TxOverflowCount_value;
    const uint32_t TxOverflowCount_raw = CAN_ENCODE(TxOverflowCount_val, CANSIG_DIM_ALERTS_CONTEXT_TX_OVERFLOW_COUNT_SCALE, CANSIG_DIM_ALERTS_CONTEXT_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(TxOverflowCount_raw, 0, 0xff);   // Packs bits ######## of byte 0
    
    // Pack 8-bit signal RxOverflowCount into payload (at bit 8 to bit 16).
    const uint32_t RxOverflowCount_val = in_msg->RxOverflowCount_value;
    const uint32_t RxOverflowCount_raw = CAN_ENCODE(RxOverflowCount_val, CANSIG_DIM_ALERTS_CONTEXT_RX_OVERFLOW_COUNT_SCALE, CANSIG_DIM_ALERTS_CONTEXT_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(RxOverflowCount_raw, 0, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 3-bit signal WatchdogTimeoutTaskName into payload (at bit 16 to bit 19).
    const RtosTaskName WatchdogTimeoutTaskName_val = in_msg->WatchdogTimeoutTaskName_value;
    const uint32_t WatchdogTimeoutTaskName_raw = CAN_ENCODE(WatchdogTimeoutTaskName_val, CANSIG_DIM_ALERTS_CONTEXT_WATCHDOG_TIMEOUT_TASK_NAME_SCALE, CANSIG_DIM_ALERTS_CONTEXT_WATCHDOG_TIMEOUT_TASK_NAME_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(WatchdogTimeoutTaskName_raw, 0, 0x7);   // Packs bits _____### of byte 2
    
}

void App_CanUtils_DIM_Warnings_Pack(const DIM_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DIM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ into payload (at bit 0 to bit 1).
    const bool DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val = in_msg->DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
    const uint32_t DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw = CAN_ENCODE(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ into payload (at bit 1 to bit 2).
    const bool DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val = in_msg->DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
    const uint32_t DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw = CAN_ENCODE(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ into payload (at bit 2 to bit 3).
    const bool DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val = in_msg->DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
    const uint32_t DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw = CAN_ENCODE(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX into payload (at bit 3 to bit 4).
    const bool DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val = in_msg->DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
    const uint32_t DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw = CAN_ENCODE(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX into payload (at bit 4 to bit 5).
    const bool DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val = in_msg->DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
    const uint32_t DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw = CAN_ENCODE(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal DIM_WARNING_WATCHDOG_TIMEOUT into payload (at bit 5 to bit 6).
    const bool DIM_WARNING_WATCHDOG_TIMEOUT_val = in_msg->DIM_WARNING_WATCHDOG_TIMEOUT_value;
    const uint32_t DIM_WARNING_WATCHDOG_TIMEOUT_raw = CAN_ENCODE(DIM_WARNING_WATCHDOG_TIMEOUT_val, CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_WARNING_WATCHDOG_TIMEOUT_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 1-bit signal DIM_WARNING_TX_OVERFLOW into payload (at bit 6 to bit 7).
    const bool DIM_WARNING_TX_OVERFLOW_val = in_msg->DIM_WARNING_TX_OVERFLOW_value;
    const uint32_t DIM_WARNING_TX_OVERFLOW_raw = CAN_ENCODE(DIM_WARNING_TX_OVERFLOW_val, CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_WARNING_TX_OVERFLOW_raw, 6, 0x40);   // Packs bits _#______ of byte 0
    
    // Pack 1-bit signal DIM_WARNING_RX_OVERFLOW into payload (at bit 7 to bit 8).
    const bool DIM_WARNING_RX_OVERFLOW_val = in_msg->DIM_WARNING_RX_OVERFLOW_value;
    const uint32_t DIM_WARNING_RX_OVERFLOW_raw = CAN_ENCODE(DIM_WARNING_RX_OVERFLOW_val, CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DIM_WARNING_RX_OVERFLOW_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    
}

void App_CanUtils_DIM_Faults_Pack(const DIM_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DIM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DIM_FAULT_MISSING_HEARTBEAT into payload (at bit 0 to bit 1).
    const bool DIM_FAULT_MISSING_HEARTBEAT_val = in_msg->DIM_FAULT_MISSING_HEARTBEAT_value;
    const uint32_t DIM_FAULT_MISSING_HEARTBEAT_raw = CAN_ENCODE(DIM_FAULT_MISSING_HEARTBEAT_val, CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DIM_FAULT_MISSING_HEARTBEAT_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
}

void App_CanUtils_Debug_CanModes_Unpack(const uint8_t* const in_data, Debug_CanModes_Signals* const out_msg)
{
    // Unpack 1-byte payload for message Debug_CanModes.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal EnableDebugMode from payload (at bit 0 to bit 1).
    uint32_t EnableDebugMode_raw = 0;
    EnableDebugMode_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool EnableDebugMode_val = CAN_DECODE(EnableDebugMode_raw, CANSIG_DEBUG_CAN_MODES_ENABLE_DEBUG_MODE_SCALE, CANSIG_DEBUG_CAN_MODES_ENABLE_DEBUG_MODE_OFFSET, uint32_t);
    out_msg->EnableDebugMode_value = EnableDebugMode_val;
    
}

void App_CanUtils_BMS_Vitals_Unpack(const uint8_t* const in_data, BMS_Vitals_Signals* const out_msg)
{
    // Unpack 1-byte payload for message BMS_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_CCCBBBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal Heartbeat from payload (at bit 0 to bit 1).
    uint32_t Heartbeat_raw = 0;
    Heartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool Heartbeat_val = CAN_DECODE(Heartbeat_raw, CANSIG_BMS_VITALS_HEARTBEAT_SCALE, CANSIG_BMS_VITALS_HEARTBEAT_OFFSET, uint32_t);
    out_msg->Heartbeat_value = Heartbeat_val;
    
    // Unpack 3-bit signal CurrentState from payload (at bit 1 to bit 4).
    uint32_t CurrentState_raw = 0;
    CurrentState_raw |= unpackShiftRight(in_data[0], 1, 0xe);   // Unpacks bits ____###_ of msg byte 0
    BmsState CurrentState_val = CAN_DECODE(CurrentState_raw, CANSIG_BMS_VITALS_CURRENT_STATE_SCALE, CANSIG_BMS_VITALS_CURRENT_STATE_OFFSET, uint32_t);
    out_msg->CurrentState_value = CurrentState_val;
    
    // Unpack 3-bit signal StateOfCharge from payload (at bit 4 to bit 7).
    uint32_t StateOfCharge_raw = 0;
    StateOfCharge_raw |= unpackShiftRight(in_data[0], 4, 0x70);   // Unpacks bits _###____ of msg byte 0
    float StateOfCharge_val = CAN_DECODE(StateOfCharge_raw, CANSIG_BMS_VITALS_STATE_OF_CHARGE_SCALE, CANSIG_BMS_VITALS_STATE_OF_CHARGE_OFFSET, float);
    out_msg->StateOfCharge_value = StateOfCharge_val;
    
}

void App_CanUtils_BMS_OkStatuses_Unpack(const uint8_t* const in_data, BMS_OkStatuses_Signals* const out_msg)
{
    // Unpack 1-byte payload for message BMS_OkStatuses.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|___EDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal BmsOk from payload (at bit 0 to bit 1).
    uint32_t BmsOk_raw = 0;
    BmsOk_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool BmsOk_val = CAN_DECODE(BmsOk_raw, CANSIG_BMS_OK_STATUSES_BMS_OK_SCALE, CANSIG_BMS_OK_STATUSES_BMS_OK_OFFSET, uint32_t);
    out_msg->BmsOk_value = BmsOk_val;
    
    // Unpack 1-bit signal ImdOk from payload (at bit 1 to bit 2).
    uint32_t ImdOk_raw = 0;
    ImdOk_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool ImdOk_val = CAN_DECODE(ImdOk_raw, CANSIG_BMS_OK_STATUSES_IMD_OK_SCALE, CANSIG_BMS_OK_STATUSES_IMD_OK_OFFSET, uint32_t);
    out_msg->ImdOk_value = ImdOk_val;
    
    // Unpack 1-bit signal BspdOk from payload (at bit 2 to bit 3).
    uint32_t BspdOk_raw = 0;
    BspdOk_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool BspdOk_val = CAN_DECODE(BspdOk_raw, CANSIG_BMS_OK_STATUSES_BSPD_OK_SCALE, CANSIG_BMS_OK_STATUSES_BSPD_OK_OFFSET, uint32_t);
    out_msg->BspdOk_value = BspdOk_val;
    
    // Unpack 1-bit signal ImdLatchedFaultStatus from payload (at bit 3 to bit 4).
    uint32_t ImdLatchedFaultStatus_raw = 0;
    ImdLatchedFaultStatus_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool ImdLatchedFaultStatus_val = CAN_DECODE(ImdLatchedFaultStatus_raw, CANSIG_BMS_OK_STATUSES_IMD_LATCHED_FAULT_STATUS_SCALE, CANSIG_BMS_OK_STATUSES_IMD_LATCHED_FAULT_STATUS_OFFSET, uint32_t);
    out_msg->ImdLatchedFaultStatus_value = ImdLatchedFaultStatus_val;
    
    // Unpack 1-bit signal BspdLatchedFaultStatus from payload (at bit 4 to bit 5).
    uint32_t BspdLatchedFaultStatus_raw = 0;
    BspdLatchedFaultStatus_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool BspdLatchedFaultStatus_val = CAN_DECODE(BspdLatchedFaultStatus_raw, CANSIG_BMS_OK_STATUSES_BSPD_LATCHED_FAULT_STATUS_SCALE, CANSIG_BMS_OK_STATUSES_BSPD_LATCHED_FAULT_STATUS_OFFSET, uint32_t);
    out_msg->BspdLatchedFaultStatus_value = BspdLatchedFaultStatus_val;
    
}

void App_CanUtils_BMS_Contactors_Unpack(const uint8_t* const in_data, BMS_Contactors_Signals* const out_msg)
{
    // Unpack 1-byte payload for message BMS_Contactors.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal AirPositive from payload (at bit 0 to bit 1).
    uint32_t AirPositive_raw = 0;
    AirPositive_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    ContactorState AirPositive_val = CAN_DECODE(AirPositive_raw, CANSIG_BMS_CONTACTORS_AIR_POSITIVE_SCALE, CANSIG_BMS_CONTACTORS_AIR_POSITIVE_OFFSET, uint32_t);
    out_msg->AirPositive_value = AirPositive_val;
    
    // Unpack 1-bit signal AirNegative from payload (at bit 1 to bit 2).
    uint32_t AirNegative_raw = 0;
    AirNegative_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    ContactorState AirNegative_val = CAN_DECODE(AirNegative_raw, CANSIG_BMS_CONTACTORS_AIR_NEGATIVE_SCALE, CANSIG_BMS_CONTACTORS_AIR_NEGATIVE_OFFSET, uint32_t);
    out_msg->AirNegative_value = AirNegative_val;
    
    // Unpack 1-bit signal PrechargeRelay from payload (at bit 2 to bit 3).
    uint32_t PrechargeRelay_raw = 0;
    PrechargeRelay_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    ContactorState PrechargeRelay_val = CAN_DECODE(PrechargeRelay_raw, CANSIG_BMS_CONTACTORS_PRECHARGE_RELAY_SCALE, CANSIG_BMS_CONTACTORS_PRECHARGE_RELAY_OFFSET, uint32_t);
    out_msg->PrechargeRelay_value = PrechargeRelay_val;
    
}

void App_CanUtils_BMS_CellVoltages_Unpack(const uint8_t* const in_data, BMS_CellVoltages_Signals* const out_msg)
{
    // Unpack 4-byte payload for message BMS_CellVoltages.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 16-bit signal MinCellVoltage from payload (at bit 0 to bit 16).
    uint32_t MinCellVoltage_raw = 0;
    MinCellVoltage_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    MinCellVoltage_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    float MinCellVoltage_val = CAN_DECODE(MinCellVoltage_raw, CANSIG_BMS_CELL_VOLTAGES_MIN_CELL_VOLTAGE_SCALE, CANSIG_BMS_CELL_VOLTAGES_MIN_CELL_VOLTAGE_OFFSET, float);
    out_msg->MinCellVoltage_value = MinCellVoltage_val;
    
    // Unpack 16-bit signal MaxCellVoltage from payload (at bit 16 to bit 32).
    uint32_t MaxCellVoltage_raw = 0;
    MaxCellVoltage_raw |= unpackShiftRight(in_data[2], 0, 0xff);   // Unpacks bits ######## of msg byte 2
    MaxCellVoltage_raw |= unpackShiftLeft(in_data[3], 8, 0xff);   // Unpacks bits ######## of msg byte 3
    float MaxCellVoltage_val = CAN_DECODE(MaxCellVoltage_raw, CANSIG_BMS_CELL_VOLTAGES_MAX_CELL_VOLTAGE_SCALE, CANSIG_BMS_CELL_VOLTAGES_MAX_CELL_VOLTAGE_OFFSET, float);
    out_msg->MaxCellVoltage_value = MaxCellVoltage_val;
    
}

void App_CanUtils_BMS_TractiveSystem_Unpack(const uint8_t* const in_data, BMS_TractiveSystem_Signals* const out_msg)
{
    // Unpack 5-byte payload for message BMS_TractiveSystem.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|____BBBB|BBBBBBBB|BAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 23-bit signal TsVoltage from payload (at bit 0 to bit 23).
    uint32_t TsVoltage_raw = 0;
    TsVoltage_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    TsVoltage_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    TsVoltage_raw |= unpackShiftLeft(in_data[2], 16, 0x7f);   // Unpacks bits _####### of msg byte 2
    float TsVoltage_val = CAN_DECODE(TsVoltage_raw, CANSIG_BMS_TRACTIVE_SYSTEM_TS_VOLTAGE_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_TS_VOLTAGE_OFFSET, float);
    out_msg->TsVoltage_value = TsVoltage_val;
    
    // Unpack 13-bit signal TsCurrent from payload (at bit 23 to bit 36).
    uint32_t TsCurrent_raw = 0;
    TsCurrent_raw |= unpackShiftRight(in_data[2], 7, 0x80);   // Unpacks bits #_______ of msg byte 2
    TsCurrent_raw |= unpackShiftLeft(in_data[3], 1, 0xff);   // Unpacks bits ######## of msg byte 3
    TsCurrent_raw |= unpackShiftLeft(in_data[4], 9, 0xf);   // Unpacks bits ____#### of msg byte 4
    float TsCurrent_val = CAN_DECODE(TsCurrent_raw, CANSIG_BMS_TRACTIVE_SYSTEM_TS_CURRENT_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_TS_CURRENT_OFFSET, float);
    out_msg->TsCurrent_value = TsCurrent_val;
    
}

void App_CanUtils_BMS_Warnings_Unpack(const uint8_t* const in_data, BMS_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message BMS_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ from payload (at bit 0 to bit 1).
    uint32_t BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw = 0;
    BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val = CAN_DECODE(BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_OFFSET, uint32_t);
    out_msg->BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val;
    
    // Unpack 1-bit signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ from payload (at bit 1 to bit 2).
    uint32_t BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw = 0;
    BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val = CAN_DECODE(BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_OFFSET, uint32_t);
    out_msg->BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val;
    
    // Unpack 1-bit signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ from payload (at bit 2 to bit 3).
    uint32_t BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw = 0;
    BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val = CAN_DECODE(BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_OFFSET, uint32_t);
    out_msg->BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val;
    
    // Unpack 1-bit signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX from payload (at bit 3 to bit 4).
    uint32_t BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw = 0;
    BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val = CAN_DECODE(BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_OFFSET, uint32_t);
    out_msg->BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val;
    
    // Unpack 1-bit signal BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX from payload (at bit 4 to bit 5).
    uint32_t BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw = 0;
    BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val = CAN_DECODE(BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_OFFSET, uint32_t);
    out_msg->BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val;
    
    // Unpack 1-bit signal BMS_WARNING_WATCHDOG_TIMEOUT from payload (at bit 5 to bit 6).
    uint32_t BMS_WARNING_WATCHDOG_TIMEOUT_raw = 0;
    BMS_WARNING_WATCHDOG_TIMEOUT_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool BMS_WARNING_WATCHDOG_TIMEOUT_val = CAN_DECODE(BMS_WARNING_WATCHDOG_TIMEOUT_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_msg->BMS_WARNING_WATCHDOG_TIMEOUT_value = BMS_WARNING_WATCHDOG_TIMEOUT_val;
    
    // Unpack 1-bit signal BMS_WARNING_TX_OVERFLOW from payload (at bit 6 to bit 7).
    uint32_t BMS_WARNING_TX_OVERFLOW_raw = 0;
    BMS_WARNING_TX_OVERFLOW_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool BMS_WARNING_TX_OVERFLOW_val = CAN_DECODE(BMS_WARNING_TX_OVERFLOW_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_msg->BMS_WARNING_TX_OVERFLOW_value = BMS_WARNING_TX_OVERFLOW_val;
    
    // Unpack 1-bit signal BMS_WARNING_RX_OVERFLOW from payload (at bit 7 to bit 8).
    uint32_t BMS_WARNING_RX_OVERFLOW_raw = 0;
    BMS_WARNING_RX_OVERFLOW_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool BMS_WARNING_RX_OVERFLOW_val = CAN_DECODE(BMS_WARNING_RX_OVERFLOW_raw, CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_msg->BMS_WARNING_RX_OVERFLOW_value = BMS_WARNING_RX_OVERFLOW_val;
    
}

void App_CanUtils_BMS_Faults_Unpack(const uint8_t* const in_data, BMS_Faults_Signals* const out_msg)
{
    // Unpack 2-byte payload for message BMS_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__NMLKJI|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal BMS_FAULT_MISSING_HEARTBEAT from payload (at bit 0 to bit 1).
    uint32_t BMS_FAULT_MISSING_HEARTBEAT_raw = 0;
    BMS_FAULT_MISSING_HEARTBEAT_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool BMS_FAULT_MISSING_HEARTBEAT_val = CAN_DECODE(BMS_FAULT_MISSING_HEARTBEAT_raw, CANSIG_BMS_FAULTS_BMS_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_msg->BMS_FAULT_MISSING_HEARTBEAT_value = BMS_FAULT_MISSING_HEARTBEAT_val;
    
    // Unpack 1-bit signal BMS_FAULT_STATE_FAULT from payload (at bit 1 to bit 2).
    uint32_t BMS_FAULT_STATE_FAULT_raw = 0;
    BMS_FAULT_STATE_FAULT_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool BMS_FAULT_STATE_FAULT_val = CAN_DECODE(BMS_FAULT_STATE_FAULT_raw, CANSIG_BMS_FAULTS_BMS_FAULT_STATE_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_STATE_FAULT_OFFSET, uint32_t);
    out_msg->BMS_FAULT_STATE_FAULT_value = BMS_FAULT_STATE_FAULT_val;
    
    // Unpack 1-bit signal BMS_FAULT_CELL_UNDERVOLTAGE from payload (at bit 2 to bit 3).
    uint32_t BMS_FAULT_CELL_UNDERVOLTAGE_raw = 0;
    BMS_FAULT_CELL_UNDERVOLTAGE_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool BMS_FAULT_CELL_UNDERVOLTAGE_val = CAN_DECODE(BMS_FAULT_CELL_UNDERVOLTAGE_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_OFFSET, uint32_t);
    out_msg->BMS_FAULT_CELL_UNDERVOLTAGE_value = BMS_FAULT_CELL_UNDERVOLTAGE_val;
    
    // Unpack 1-bit signal BMS_FAULT_CELL_OVERVOLTAGE from payload (at bit 3 to bit 4).
    uint32_t BMS_FAULT_CELL_OVERVOLTAGE_raw = 0;
    BMS_FAULT_CELL_OVERVOLTAGE_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool BMS_FAULT_CELL_OVERVOLTAGE_val = CAN_DECODE(BMS_FAULT_CELL_OVERVOLTAGE_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_OFFSET, uint32_t);
    out_msg->BMS_FAULT_CELL_OVERVOLTAGE_value = BMS_FAULT_CELL_OVERVOLTAGE_val;
    
    // Unpack 1-bit signal BMS_FAULT_MODULE_COMM_ERROR from payload (at bit 4 to bit 5).
    uint32_t BMS_FAULT_MODULE_COMM_ERROR_raw = 0;
    BMS_FAULT_MODULE_COMM_ERROR_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool BMS_FAULT_MODULE_COMM_ERROR_val = CAN_DECODE(BMS_FAULT_MODULE_COMM_ERROR_raw, CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMM_ERROR_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMM_ERROR_OFFSET, uint32_t);
    out_msg->BMS_FAULT_MODULE_COMM_ERROR_value = BMS_FAULT_MODULE_COMM_ERROR_val;
    
    // Unpack 1-bit signal BMS_FAULT_CELL_UNDERTEMP from payload (at bit 5 to bit 6).
    uint32_t BMS_FAULT_CELL_UNDERTEMP_raw = 0;
    BMS_FAULT_CELL_UNDERTEMP_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool BMS_FAULT_CELL_UNDERTEMP_val = CAN_DECODE(BMS_FAULT_CELL_UNDERTEMP_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_OFFSET, uint32_t);
    out_msg->BMS_FAULT_CELL_UNDERTEMP_value = BMS_FAULT_CELL_UNDERTEMP_val;
    
    // Unpack 1-bit signal BMS_FAULT_CELL_OVERTEMP from payload (at bit 6 to bit 7).
    uint32_t BMS_FAULT_CELL_OVERTEMP_raw = 0;
    BMS_FAULT_CELL_OVERTEMP_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool BMS_FAULT_CELL_OVERTEMP_val = CAN_DECODE(BMS_FAULT_CELL_OVERTEMP_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_OFFSET, uint32_t);
    out_msg->BMS_FAULT_CELL_OVERTEMP_value = BMS_FAULT_CELL_OVERTEMP_val;
    
    // Unpack 1-bit signal BMS_FAULT_CHARGER_FAULT from payload (at bit 7 to bit 8).
    uint32_t BMS_FAULT_CHARGER_FAULT_raw = 0;
    BMS_FAULT_CHARGER_FAULT_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool BMS_FAULT_CHARGER_FAULT_val = CAN_DECODE(BMS_FAULT_CHARGER_FAULT_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_FAULT_OFFSET, uint32_t);
    out_msg->BMS_FAULT_CHARGER_FAULT_value = BMS_FAULT_CHARGER_FAULT_val;
    
    // Unpack 1-bit signal BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE from payload (at bit 8 to bit 9).
    uint32_t BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_raw = 0;
    BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_raw |= unpackShiftRight(in_data[1], 0, 0x1);   // Unpacks bits _______# of msg byte 1
    bool BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_val = CAN_DECODE(BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_OFFSET, uint32_t);
    out_msg->BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_value = BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_val;
    
    // Unpack 1-bit signal BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN from payload (at bit 9 to bit 10).
    uint32_t BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_raw = 0;
    BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_raw |= unpackShiftRight(in_data[1], 1, 0x2);   // Unpacks bits ______#_ of msg byte 1
    bool BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_val = CAN_DECODE(BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_raw, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_OFFSET, uint32_t);
    out_msg->BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_value = BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_val;
    
    // Unpack 1-bit signal BMS_FAULT_HAS_REACHED_MAX_V from payload (at bit 10 to bit 11).
    uint32_t BMS_FAULT_HAS_REACHED_MAX_V_raw = 0;
    BMS_FAULT_HAS_REACHED_MAX_V_raw |= unpackShiftRight(in_data[1], 2, 0x4);   // Unpacks bits _____#__ of msg byte 1
    bool BMS_FAULT_HAS_REACHED_MAX_V_val = CAN_DECODE(BMS_FAULT_HAS_REACHED_MAX_V_raw, CANSIG_BMS_FAULTS_BMS_FAULT_HAS_REACHED_MAX_V_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_HAS_REACHED_MAX_V_OFFSET, uint32_t);
    out_msg->BMS_FAULT_HAS_REACHED_MAX_V_value = BMS_FAULT_HAS_REACHED_MAX_V_val;
    
    // Unpack 1-bit signal BMS_FAULT_TS_OVERCURRENT from payload (at bit 11 to bit 12).
    uint32_t BMS_FAULT_TS_OVERCURRENT_raw = 0;
    BMS_FAULT_TS_OVERCURRENT_raw |= unpackShiftRight(in_data[1], 3, 0x8);   // Unpacks bits ____#___ of msg byte 1
    bool BMS_FAULT_TS_OVERCURRENT_val = CAN_DECODE(BMS_FAULT_TS_OVERCURRENT_raw, CANSIG_BMS_FAULTS_BMS_FAULT_TS_OVERCURRENT_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_TS_OVERCURRENT_OFFSET, uint32_t);
    out_msg->BMS_FAULT_TS_OVERCURRENT_value = BMS_FAULT_TS_OVERCURRENT_val;
    
    // Unpack 1-bit signal BMS_FAULT_PRECHARGE_ERROR from payload (at bit 12 to bit 13).
    uint32_t BMS_FAULT_PRECHARGE_ERROR_raw = 0;
    BMS_FAULT_PRECHARGE_ERROR_raw |= unpackShiftRight(in_data[1], 4, 0x10);   // Unpacks bits ___#____ of msg byte 1
    bool BMS_FAULT_PRECHARGE_ERROR_val = CAN_DECODE(BMS_FAULT_PRECHARGE_ERROR_raw, CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_ERROR_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_ERROR_OFFSET, uint32_t);
    out_msg->BMS_FAULT_PRECHARGE_ERROR_value = BMS_FAULT_PRECHARGE_ERROR_val;
    
    // Unpack 1-bit signal BMS_FAULT_LATCHED_BMS_FAULT from payload (at bit 13 to bit 14).
    uint32_t BMS_FAULT_LATCHED_BMS_FAULT_raw = 0;
    BMS_FAULT_LATCHED_BMS_FAULT_raw |= unpackShiftRight(in_data[1], 5, 0x20);   // Unpacks bits __#_____ of msg byte 1
    bool BMS_FAULT_LATCHED_BMS_FAULT_val = CAN_DECODE(BMS_FAULT_LATCHED_BMS_FAULT_raw, CANSIG_BMS_FAULTS_BMS_FAULT_LATCHED_BMS_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_LATCHED_BMS_FAULT_OFFSET, uint32_t);
    out_msg->BMS_FAULT_LATCHED_BMS_FAULT_value = BMS_FAULT_LATCHED_BMS_FAULT_val;
    
}

void App_CanUtils_DCM_Vitals_Unpack(const uint8_t* const in_data, DCM_Vitals_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DCM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____BBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal Heartbeat from payload (at bit 0 to bit 1).
    uint32_t Heartbeat_raw = 0;
    Heartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool Heartbeat_val = CAN_DECODE(Heartbeat_raw, CANSIG_DCM_VITALS_HEARTBEAT_SCALE, CANSIG_DCM_VITALS_HEARTBEAT_OFFSET, uint32_t);
    out_msg->Heartbeat_value = Heartbeat_val;
    
    // Unpack 2-bit signal CurrentState from payload (at bit 1 to bit 3).
    uint32_t CurrentState_raw = 0;
    CurrentState_raw |= unpackShiftRight(in_data[0], 1, 0x6);   // Unpacks bits _____##_ of msg byte 0
    DcmState CurrentState_val = CAN_DECODE(CurrentState_raw, CANSIG_DCM_VITALS_CURRENT_STATE_SCALE, CANSIG_DCM_VITALS_CURRENT_STATE_OFFSET, uint32_t);
    out_msg->CurrentState_value = CurrentState_val;
    
}

void App_CanUtils_DCM_Warnings_Unpack(const uint8_t* const in_data, DCM_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DCM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ from payload (at bit 0 to bit 1).
    uint32_t DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw = 0;
    DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val = CAN_DECODE(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_OFFSET, uint32_t);
    out_msg->DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val;
    
    // Unpack 1-bit signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ from payload (at bit 1 to bit 2).
    uint32_t DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw = 0;
    DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val = CAN_DECODE(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_OFFSET, uint32_t);
    out_msg->DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val;
    
    // Unpack 1-bit signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ from payload (at bit 2 to bit 3).
    uint32_t DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw = 0;
    DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val = CAN_DECODE(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_OFFSET, uint32_t);
    out_msg->DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val;
    
    // Unpack 1-bit signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX from payload (at bit 3 to bit 4).
    uint32_t DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw = 0;
    DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val = CAN_DECODE(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_OFFSET, uint32_t);
    out_msg->DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val;
    
    // Unpack 1-bit signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX from payload (at bit 4 to bit 5).
    uint32_t DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw = 0;
    DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val = CAN_DECODE(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_OFFSET, uint32_t);
    out_msg->DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val;
    
    // Unpack 1-bit signal DCM_WARNING_WATCHDOG_TIMEOUT from payload (at bit 5 to bit 6).
    uint32_t DCM_WARNING_WATCHDOG_TIMEOUT_raw = 0;
    DCM_WARNING_WATCHDOG_TIMEOUT_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool DCM_WARNING_WATCHDOG_TIMEOUT_val = CAN_DECODE(DCM_WARNING_WATCHDOG_TIMEOUT_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_msg->DCM_WARNING_WATCHDOG_TIMEOUT_value = DCM_WARNING_WATCHDOG_TIMEOUT_val;
    
    // Unpack 1-bit signal DCM_WARNING_TX_OVERFLOW from payload (at bit 6 to bit 7).
    uint32_t DCM_WARNING_TX_OVERFLOW_raw = 0;
    DCM_WARNING_TX_OVERFLOW_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool DCM_WARNING_TX_OVERFLOW_val = CAN_DECODE(DCM_WARNING_TX_OVERFLOW_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_msg->DCM_WARNING_TX_OVERFLOW_value = DCM_WARNING_TX_OVERFLOW_val;
    
    // Unpack 1-bit signal DCM_WARNING_RX_OVERFLOW from payload (at bit 7 to bit 8).
    uint32_t DCM_WARNING_RX_OVERFLOW_raw = 0;
    DCM_WARNING_RX_OVERFLOW_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool DCM_WARNING_RX_OVERFLOW_val = CAN_DECODE(DCM_WARNING_RX_OVERFLOW_raw, CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_msg->DCM_WARNING_RX_OVERFLOW_value = DCM_WARNING_RX_OVERFLOW_val;
    
}

void App_CanUtils_DCM_Faults_Unpack(const uint8_t* const in_data, DCM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DCM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DCM_FAULT_STATE_FAULT from payload (at bit 0 to bit 1).
    uint32_t DCM_FAULT_STATE_FAULT_raw = 0;
    DCM_FAULT_STATE_FAULT_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DCM_FAULT_STATE_FAULT_val = CAN_DECODE(DCM_FAULT_STATE_FAULT_raw, CANSIG_DCM_FAULTS_DCM_FAULT_STATE_FAULT_SCALE, CANSIG_DCM_FAULTS_DCM_FAULT_STATE_FAULT_OFFSET, uint32_t);
    out_msg->DCM_FAULT_STATE_FAULT_value = DCM_FAULT_STATE_FAULT_val;
    
    // Unpack 1-bit signal DCM_FAULT_MISSING_HEARTBEAT from payload (at bit 1 to bit 2).
    uint32_t DCM_FAULT_MISSING_HEARTBEAT_raw = 0;
    DCM_FAULT_MISSING_HEARTBEAT_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool DCM_FAULT_MISSING_HEARTBEAT_val = CAN_DECODE(DCM_FAULT_MISSING_HEARTBEAT_raw, CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_msg->DCM_FAULT_MISSING_HEARTBEAT_value = DCM_FAULT_MISSING_HEARTBEAT_val;
    
}

void App_CanUtils_FSM_Warnings_Unpack(const uint8_t* const in_data, FSM_Warnings_Signals* const out_msg)
{
    // Unpack 3-byte payload for message FSM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______Q|PONMLKJI|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ from payload (at bit 0 to bit 1).
    uint32_t FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw = 0;
    FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val = CAN_DECODE(FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_OFFSET, uint32_t);
    out_msg->FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val;
    
    // Unpack 1-bit signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ from payload (at bit 1 to bit 2).
    uint32_t FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw = 0;
    FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val = CAN_DECODE(FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_OFFSET, uint32_t);
    out_msg->FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val;
    
    // Unpack 1-bit signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ from payload (at bit 2 to bit 3).
    uint32_t FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw = 0;
    FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val = CAN_DECODE(FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_OFFSET, uint32_t);
    out_msg->FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val;
    
    // Unpack 1-bit signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX from payload (at bit 3 to bit 4).
    uint32_t FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw = 0;
    FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val = CAN_DECODE(FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_OFFSET, uint32_t);
    out_msg->FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val;
    
    // Unpack 1-bit signal FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX from payload (at bit 4 to bit 5).
    uint32_t FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw = 0;
    FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val = CAN_DECODE(FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_OFFSET, uint32_t);
    out_msg->FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val;
    
    // Unpack 1-bit signal FSM_WARNING_WATCHDOG_TIMEOUT from payload (at bit 5 to bit 6).
    uint32_t FSM_WARNING_WATCHDOG_TIMEOUT_raw = 0;
    FSM_WARNING_WATCHDOG_TIMEOUT_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool FSM_WARNING_WATCHDOG_TIMEOUT_val = CAN_DECODE(FSM_WARNING_WATCHDOG_TIMEOUT_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_msg->FSM_WARNING_WATCHDOG_TIMEOUT_value = FSM_WARNING_WATCHDOG_TIMEOUT_val;
    
    // Unpack 1-bit signal FSM_WARNING_TX_OVERFLOW from payload (at bit 6 to bit 7).
    uint32_t FSM_WARNING_TX_OVERFLOW_raw = 0;
    FSM_WARNING_TX_OVERFLOW_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool FSM_WARNING_TX_OVERFLOW_val = CAN_DECODE(FSM_WARNING_TX_OVERFLOW_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_TX_OVERFLOW_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_msg->FSM_WARNING_TX_OVERFLOW_value = FSM_WARNING_TX_OVERFLOW_val;
    
    // Unpack 1-bit signal FSM_WARNING_RX_OVERFLOW from payload (at bit 7 to bit 8).
    uint32_t FSM_WARNING_RX_OVERFLOW_raw = 0;
    FSM_WARNING_RX_OVERFLOW_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool FSM_WARNING_RX_OVERFLOW_val = CAN_DECODE(FSM_WARNING_RX_OVERFLOW_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_RX_OVERFLOW_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_msg->FSM_WARNING_RX_OVERFLOW_value = FSM_WARNING_RX_OVERFLOW_val;
    
    // Unpack 1-bit signal FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE from payload (at bit 8 to bit 9).
    uint32_t FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 0, 0x1);   // Unpacks bits _______# of msg byte 1
    bool FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_value = FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_val;
    
    // Unpack 1-bit signal FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE from payload (at bit 9 to bit 10).
    uint32_t FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 1, 0x2);   // Unpacks bits ______#_ of msg byte 1
    bool FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_value = FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_val;
    
    // Unpack 1-bit signal FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC from payload (at bit 10 to bit 11).
    uint32_t FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_raw = 0;
    FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_raw |= unpackShiftRight(in_data[1], 2, 0x4);   // Unpacks bits _____#__ of msg byte 1
    bool FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_val = CAN_DECODE(FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_OFFSET, uint32_t);
    out_msg->FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_value = FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_val;
    
    // Unpack 1-bit signal FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE from payload (at bit 11 to bit 12).
    uint32_t FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 3, 0x8);   // Unpacks bits ____#___ of msg byte 1
    bool FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_value = FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_val;
    
    // Unpack 1-bit signal FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE from payload (at bit 12 to bit 13).
    uint32_t FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 4, 0x10);   // Unpacks bits ___#____ of msg byte 1
    bool FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_value = FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_val;
    
    // Unpack 1-bit signal FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE from payload (at bit 13 to bit 14).
    uint32_t FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 5, 0x20);   // Unpacks bits __#_____ of msg byte 1
    bool FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_value = FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_val;
    
    // Unpack 1-bit signal FSM_WARNING_FLOW_RATE_OUT_OF_RANGE from payload (at bit 14 to bit 15).
    uint32_t FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 6, 0x40);   // Unpacks bits _#______ of msg byte 1
    bool FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_value = FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_val;
    
    // Unpack 1-bit signal FSM_WARNING_BRAKE_ACC_DISAGREEMENT from payload (at bit 15 to bit 16).
    uint32_t FSM_WARNING_BRAKE_ACC_DISAGREEMENT_raw = 0;
    FSM_WARNING_BRAKE_ACC_DISAGREEMENT_raw |= unpackShiftRight(in_data[1], 7, 0x80);   // Unpacks bits #_______ of msg byte 1
    bool FSM_WARNING_BRAKE_ACC_DISAGREEMENT_val = CAN_DECODE(FSM_WARNING_BRAKE_ACC_DISAGREEMENT_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_OFFSET, uint32_t);
    out_msg->FSM_WARNING_BRAKE_ACC_DISAGREEMENT_value = FSM_WARNING_BRAKE_ACC_DISAGREEMENT_val;
    
    // Unpack 1-bit signal FSM_WARNING_APPS_HAS_DISAGREEMENT from payload (at bit 16 to bit 17).
    uint32_t FSM_WARNING_APPS_HAS_DISAGREEMENT_raw = 0;
    FSM_WARNING_APPS_HAS_DISAGREEMENT_raw |= unpackShiftRight(in_data[2], 0, 0x1);   // Unpacks bits _______# of msg byte 2
    bool FSM_WARNING_APPS_HAS_DISAGREEMENT_val = CAN_DECODE(FSM_WARNING_APPS_HAS_DISAGREEMENT_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_APPS_HAS_DISAGREEMENT_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_APPS_HAS_DISAGREEMENT_OFFSET, uint32_t);
    out_msg->FSM_WARNING_APPS_HAS_DISAGREEMENT_value = FSM_WARNING_APPS_HAS_DISAGREEMENT_val;
    
}

void App_CanUtils_FSM_Faults_Unpack(const uint8_t* const in_data, FSM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message FSM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__FEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_FAULT_MISSING_HEARTBEAT from payload (at bit 0 to bit 1).
    uint32_t FSM_FAULT_MISSING_HEARTBEAT_raw = 0;
    FSM_FAULT_MISSING_HEARTBEAT_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_FAULT_MISSING_HEARTBEAT_val = CAN_DECODE(FSM_FAULT_MISSING_HEARTBEAT_raw, CANSIG_FSM_FAULTS_FSM_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_msg->FSM_FAULT_MISSING_HEARTBEAT_value = FSM_FAULT_MISSING_HEARTBEAT_val;
    
    // Unpack 1-bit signal FSM_FAULT_STATE_FAULT from payload (at bit 1 to bit 2).
    uint32_t FSM_FAULT_STATE_FAULT_raw = 0;
    FSM_FAULT_STATE_FAULT_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool FSM_FAULT_STATE_FAULT_val = CAN_DECODE(FSM_FAULT_STATE_FAULT_raw, CANSIG_FSM_FAULTS_FSM_FAULT_STATE_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_STATE_FAULT_OFFSET, uint32_t);
    out_msg->FSM_FAULT_STATE_FAULT_value = FSM_FAULT_STATE_FAULT_val;
    
    // Unpack 1-bit signal FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE from payload (at bit 2 to bit 3).
    uint32_t FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_raw = 0;
    FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_val = CAN_DECODE(FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_raw, CANSIG_FSM_FAULTS_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_OFFSET, uint32_t);
    out_msg->FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_value = FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE_val;
    
    // Unpack 1-bit signal FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE from payload (at bit 3 to bit 4).
    uint32_t FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_raw = 0;
    FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_val = CAN_DECODE(FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_raw, CANSIG_FSM_FAULTS_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_OFFSET, uint32_t);
    out_msg->FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_value = FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE_val;
    
    // Unpack 1-bit signal FSM_FAULT_FLOW_METER_HAS_UNDERFLOW from payload (at bit 4 to bit 5).
    uint32_t FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_raw = 0;
    FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_val = CAN_DECODE(FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_raw, CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_OFFSET, uint32_t);
    out_msg->FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_value = FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_val;
    
    // Unpack 1-bit signal FSM_FAULT_TORQUE_PLAUSABILITY_FAILED from payload (at bit 5 to bit 6).
    uint32_t FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_raw = 0;
    FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_val = CAN_DECODE(FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_raw, CANSIG_FSM_FAULTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_OFFSET, uint32_t);
    out_msg->FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_value = FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_val;
    
}

void App_CanUtils_PDM_Warnings_Unpack(const uint8_t* const in_data, PDM_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message PDM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ from payload (at bit 0 to bit 1).
    uint32_t PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw = 0;
    PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val = CAN_DECODE(PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_OFFSET, uint32_t);
    out_msg->PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val;
    
    // Unpack 1-bit signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ from payload (at bit 1 to bit 2).
    uint32_t PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw = 0;
    PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val = CAN_DECODE(PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_OFFSET, uint32_t);
    out_msg->PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val;
    
    // Unpack 1-bit signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ from payload (at bit 2 to bit 3).
    uint32_t PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw = 0;
    PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val = CAN_DECODE(PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_OFFSET, uint32_t);
    out_msg->PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val;
    
    // Unpack 1-bit signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX from payload (at bit 3 to bit 4).
    uint32_t PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw = 0;
    PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val = CAN_DECODE(PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_OFFSET, uint32_t);
    out_msg->PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val;
    
    // Unpack 1-bit signal PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX from payload (at bit 4 to bit 5).
    uint32_t PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw = 0;
    PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val = CAN_DECODE(PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_OFFSET, uint32_t);
    out_msg->PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val;
    
    // Unpack 1-bit signal PDM_WARNING_WATCHDOG_TIMEOUT from payload (at bit 5 to bit 6).
    uint32_t PDM_WARNING_WATCHDOG_TIMEOUT_raw = 0;
    PDM_WARNING_WATCHDOG_TIMEOUT_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool PDM_WARNING_WATCHDOG_TIMEOUT_val = CAN_DECODE(PDM_WARNING_WATCHDOG_TIMEOUT_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_msg->PDM_WARNING_WATCHDOG_TIMEOUT_value = PDM_WARNING_WATCHDOG_TIMEOUT_val;
    
    // Unpack 1-bit signal PDM_WARNING_TX_OVERFLOW from payload (at bit 6 to bit 7).
    uint32_t PDM_WARNING_TX_OVERFLOW_raw = 0;
    PDM_WARNING_TX_OVERFLOW_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool PDM_WARNING_TX_OVERFLOW_val = CAN_DECODE(PDM_WARNING_TX_OVERFLOW_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_TX_OVERFLOW_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_msg->PDM_WARNING_TX_OVERFLOW_value = PDM_WARNING_TX_OVERFLOW_val;
    
    // Unpack 1-bit signal PDM_WARNING_RX_OVERFLOW from payload (at bit 7 to bit 8).
    uint32_t PDM_WARNING_RX_OVERFLOW_raw = 0;
    PDM_WARNING_RX_OVERFLOW_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool PDM_WARNING_RX_OVERFLOW_val = CAN_DECODE(PDM_WARNING_RX_OVERFLOW_raw, CANSIG_PDM_WARNINGS_PDM_WARNING_RX_OVERFLOW_SCALE, CANSIG_PDM_WARNINGS_PDM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_msg->PDM_WARNING_RX_OVERFLOW_value = PDM_WARNING_RX_OVERFLOW_val;
    
}

void App_CanUtils_PDM_Faults_Unpack(const uint8_t* const in_data, PDM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message PDM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal PDM_FAULT_DUMMY from payload (at bit 0 to bit 1).
    uint32_t PDM_FAULT_DUMMY_raw = 0;
    PDM_FAULT_DUMMY_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool PDM_FAULT_DUMMY_val = CAN_DECODE(PDM_FAULT_DUMMY_raw, CANSIG_PDM_FAULTS_PDM_FAULT_DUMMY_SCALE, CANSIG_PDM_FAULTS_PDM_FAULT_DUMMY_OFFSET, uint32_t);
    out_msg->PDM_FAULT_DUMMY_value = PDM_FAULT_DUMMY_val;
    
}

void App_CanUtils_INVR_Temperatures1_Unpack(const uint8_t* const in_data, INVR_Temperatures1_Signals* const out_msg)
{
    // Unpack 8-byte payload for message INVR_Temperatures1.
    // |DDDDDDDD|DDDDDDDD|CCCCCCCC|CCCCCCCC|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 16-bit signal ModuleATemperature from payload (at bit 0 to bit 16).
    uint32_t ModuleATemperature_raw = 0;
    ModuleATemperature_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    ModuleATemperature_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    const int ModuleATemperature_signed = SIGNED_DECODE(ModuleATemperature_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float ModuleATemperature_val = CAN_DECODE(ModuleATemperature_signed, CANSIG_INVR_TEMPERATURES1_MODULE_A_TEMPERATURE_SCALE, CANSIG_INVR_TEMPERATURES1_MODULE_A_TEMPERATURE_OFFSET, float);
    out_msg->ModuleATemperature_value = ModuleATemperature_val;
    
    // Unpack 16-bit signal ModuleBTemperature from payload (at bit 16 to bit 32).
    uint32_t ModuleBTemperature_raw = 0;
    ModuleBTemperature_raw |= unpackShiftRight(in_data[2], 0, 0xff);   // Unpacks bits ######## of msg byte 2
    ModuleBTemperature_raw |= unpackShiftLeft(in_data[3], 8, 0xff);   // Unpacks bits ######## of msg byte 3
    const int ModuleBTemperature_signed = SIGNED_DECODE(ModuleBTemperature_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float ModuleBTemperature_val = CAN_DECODE(ModuleBTemperature_signed, CANSIG_INVR_TEMPERATURES1_MODULE_B_TEMPERATURE_SCALE, CANSIG_INVR_TEMPERATURES1_MODULE_B_TEMPERATURE_OFFSET, float);
    out_msg->ModuleBTemperature_value = ModuleBTemperature_val;
    
    // Unpack 16-bit signal ModuleCTemperature from payload (at bit 32 to bit 48).
    uint32_t ModuleCTemperature_raw = 0;
    ModuleCTemperature_raw |= unpackShiftRight(in_data[4], 0, 0xff);   // Unpacks bits ######## of msg byte 4
    ModuleCTemperature_raw |= unpackShiftLeft(in_data[5], 8, 0xff);   // Unpacks bits ######## of msg byte 5
    const int ModuleCTemperature_signed = SIGNED_DECODE(ModuleCTemperature_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float ModuleCTemperature_val = CAN_DECODE(ModuleCTemperature_signed, CANSIG_INVR_TEMPERATURES1_MODULE_C_TEMPERATURE_SCALE, CANSIG_INVR_TEMPERATURES1_MODULE_C_TEMPERATURE_OFFSET, float);
    out_msg->ModuleCTemperature_value = ModuleCTemperature_val;
    
    // Unpack 16-bit signal GateDriverBoardTemperature from payload (at bit 48 to bit 64).
    uint32_t GateDriverBoardTemperature_raw = 0;
    GateDriverBoardTemperature_raw |= unpackShiftRight(in_data[6], 0, 0xff);   // Unpacks bits ######## of msg byte 6
    GateDriverBoardTemperature_raw |= unpackShiftLeft(in_data[7], 8, 0xff);   // Unpacks bits ######## of msg byte 7
    const int GateDriverBoardTemperature_signed = SIGNED_DECODE(GateDriverBoardTemperature_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float GateDriverBoardTemperature_val = CAN_DECODE(GateDriverBoardTemperature_signed, CANSIG_INVR_TEMPERATURES1_GATE_DRIVER_BOARD_TEMPERATURE_SCALE, CANSIG_INVR_TEMPERATURES1_GATE_DRIVER_BOARD_TEMPERATURE_OFFSET, float);
    out_msg->GateDriverBoardTemperature_value = GateDriverBoardTemperature_val;
    
}

void App_CanUtils_INVR_MotorPositionInfo_Unpack(const uint8_t* const in_data, INVR_MotorPositionInfo_Signals* const out_msg)
{
    // Unpack 8-byte payload for message INVR_MotorPositionInfo.
    // |DDDDDDDD|DDDDDDDD|CCCCCCCC|CCCCCCCC|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 16-bit signal MotorAngle from payload (at bit 0 to bit 16).
    uint32_t MotorAngle_raw = 0;
    MotorAngle_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    MotorAngle_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    const int MotorAngle_signed = SIGNED_DECODE(MotorAngle_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float MotorAngle_val = CAN_DECODE(MotorAngle_signed, CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_SCALE, CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_ANGLE_OFFSET, float);
    out_msg->MotorAngle_value = MotorAngle_val;
    
    // Unpack 16-bit signal MotorSpeed from payload (at bit 16 to bit 32).
    uint32_t MotorSpeed_raw = 0;
    MotorSpeed_raw |= unpackShiftRight(in_data[2], 0, 0xff);   // Unpacks bits ######## of msg byte 2
    MotorSpeed_raw |= unpackShiftLeft(in_data[3], 8, 0xff);   // Unpacks bits ######## of msg byte 3
    const int MotorSpeed_signed = SIGNED_DECODE(MotorSpeed_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    int MotorSpeed_val = CAN_DECODE(MotorSpeed_signed, CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_SCALE, CANSIG_INVR_MOTOR_POSITION_INFO_MOTOR_SPEED_OFFSET, int);
    out_msg->MotorSpeed_value = MotorSpeed_val;
    
    // Unpack 16-bit signal ElectricalOutputFrequency from payload (at bit 32 to bit 48).
    uint32_t ElectricalOutputFrequency_raw = 0;
    ElectricalOutputFrequency_raw |= unpackShiftRight(in_data[4], 0, 0xff);   // Unpacks bits ######## of msg byte 4
    ElectricalOutputFrequency_raw |= unpackShiftLeft(in_data[5], 8, 0xff);   // Unpacks bits ######## of msg byte 5
    const int ElectricalOutputFrequency_signed = SIGNED_DECODE(ElectricalOutputFrequency_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float ElectricalOutputFrequency_val = CAN_DECODE(ElectricalOutputFrequency_signed, CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_SCALE, CANSIG_INVR_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_OFFSET, float);
    out_msg->ElectricalOutputFrequency_value = ElectricalOutputFrequency_val;
    
    // Unpack 16-bit signal DeltaResolverFiltered from payload (at bit 48 to bit 64).
    uint32_t DeltaResolverFiltered_raw = 0;
    DeltaResolverFiltered_raw |= unpackShiftRight(in_data[6], 0, 0xff);   // Unpacks bits ######## of msg byte 6
    DeltaResolverFiltered_raw |= unpackShiftLeft(in_data[7], 8, 0xff);   // Unpacks bits ######## of msg byte 7
    const int DeltaResolverFiltered_signed = SIGNED_DECODE(DeltaResolverFiltered_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float DeltaResolverFiltered_val = CAN_DECODE(DeltaResolverFiltered_signed, CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_SCALE, CANSIG_INVR_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_OFFSET, float);
    out_msg->DeltaResolverFiltered_value = DeltaResolverFiltered_val;
    
}

void App_CanUtils_INVL_MotorPositionInfo_Unpack(const uint8_t* const in_data, INVL_MotorPositionInfo_Signals* const out_msg)
{
    // Unpack 8-byte payload for message INVL_MotorPositionInfo.
    // |DDDDDDDD|DDDDDDDD|CCCCCCCC|CCCCCCCC|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 16-bit signal MotorAngle from payload (at bit 0 to bit 16).
    uint32_t MotorAngle_raw = 0;
    MotorAngle_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    MotorAngle_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    const int MotorAngle_signed = SIGNED_DECODE(MotorAngle_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float MotorAngle_val = CAN_DECODE(MotorAngle_signed, CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_SCALE, CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_ANGLE_OFFSET, float);
    out_msg->MotorAngle_value = MotorAngle_val;
    
    // Unpack 16-bit signal MotorSpeed from payload (at bit 16 to bit 32).
    uint32_t MotorSpeed_raw = 0;
    MotorSpeed_raw |= unpackShiftRight(in_data[2], 0, 0xff);   // Unpacks bits ######## of msg byte 2
    MotorSpeed_raw |= unpackShiftLeft(in_data[3], 8, 0xff);   // Unpacks bits ######## of msg byte 3
    const int MotorSpeed_signed = SIGNED_DECODE(MotorSpeed_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    int MotorSpeed_val = CAN_DECODE(MotorSpeed_signed, CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_SCALE, CANSIG_INVL_MOTOR_POSITION_INFO_MOTOR_SPEED_OFFSET, int);
    out_msg->MotorSpeed_value = MotorSpeed_val;
    
    // Unpack 16-bit signal ElectricalOutputFrequency from payload (at bit 32 to bit 48).
    uint32_t ElectricalOutputFrequency_raw = 0;
    ElectricalOutputFrequency_raw |= unpackShiftRight(in_data[4], 0, 0xff);   // Unpacks bits ######## of msg byte 4
    ElectricalOutputFrequency_raw |= unpackShiftLeft(in_data[5], 8, 0xff);   // Unpacks bits ######## of msg byte 5
    const int ElectricalOutputFrequency_signed = SIGNED_DECODE(ElectricalOutputFrequency_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float ElectricalOutputFrequency_val = CAN_DECODE(ElectricalOutputFrequency_signed, CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_SCALE, CANSIG_INVL_MOTOR_POSITION_INFO_ELECTRICAL_OUTPUT_FREQUENCY_OFFSET, float);
    out_msg->ElectricalOutputFrequency_value = ElectricalOutputFrequency_val;
    
    // Unpack 16-bit signal DeltaResolverFiltered from payload (at bit 48 to bit 64).
    uint32_t DeltaResolverFiltered_raw = 0;
    DeltaResolverFiltered_raw |= unpackShiftRight(in_data[6], 0, 0xff);   // Unpacks bits ######## of msg byte 6
    DeltaResolverFiltered_raw |= unpackShiftLeft(in_data[7], 8, 0xff);   // Unpacks bits ######## of msg byte 7
    const int DeltaResolverFiltered_signed = SIGNED_DECODE(DeltaResolverFiltered_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float DeltaResolverFiltered_val = CAN_DECODE(DeltaResolverFiltered_signed, CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_SCALE, CANSIG_INVL_MOTOR_POSITION_INFO_DELTA_RESOLVER_FILTERED_OFFSET, float);
    out_msg->DeltaResolverFiltered_value = DeltaResolverFiltered_val;
    
}

