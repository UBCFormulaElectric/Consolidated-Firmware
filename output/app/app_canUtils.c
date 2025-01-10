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




 
// pack function for BMS_Vitals
void app_canUtils_BMS_Vitals_pack(const BMS_Vitals_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____BBBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_Heartbeat into payload at [ bit 0 to bit 1 ).
    const bool BMS_Heartbeat_val = in_msg->BMS_Heartbeat_value;
    const uint32_t BMS_Heartbeat_raw = CAN_ENCODE(BMS_Heartbeat_val, CANSIG_BMS_HEARTBEAT_SCALE, CANSIG_BMS_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 3-bit signal BMS_State into payload at [ bit 1 to bit 4 ).
    const BmsState BMS_State_val = in_msg->BMS_State_value;
    const uint32_t BMS_State_raw = CAN_ENCODE(BMS_State_val, CANSIG_BMS_STATE_SCALE, CANSIG_BMS_STATE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_State_raw, 1, 0xE);   // Packs bits ____###_ of byte 0 
    
    
}
 
// pack function for BMS_AlertsContext
void app_canUtils_BMS_AlertsContext_pack(const BMS_AlertsContext_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_AlertsContext.
    // |xxxxxxxx|____EDDD|DDDDDDDD|DDDDDCCC|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal BMS_TxOverflowCount into payload at [ bit 0 to bit 16 ).
    const uint32_t BMS_TxOverflowCount_val = in_msg->BMS_TxOverflowCount_value;
    const uint32_t BMS_TxOverflowCount_raw = CAN_ENCODE(BMS_TxOverflowCount_val, CANSIG_BMS_TX_OVERFLOW_COUNT_SCALE, CANSIG_BMS_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_TxOverflowCount_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(BMS_TxOverflowCount_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    
    // Pack 16-bit signal BMS_RxOverflowCount into payload at [ bit 16 to bit 32 ).
    const uint32_t BMS_RxOverflowCount_val = in_msg->BMS_RxOverflowCount_value;
    const uint32_t BMS_RxOverflowCount_raw = CAN_ENCODE(BMS_RxOverflowCount_val, CANSIG_BMS_RX_OVERFLOW_COUNT_SCALE, CANSIG_BMS_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(BMS_RxOverflowCount_raw, 0, 0xFF);   // Packs bits ######## of byte 2 
    out_data[3] |= packShiftRight(BMS_RxOverflowCount_raw, 8, 0xFF);   // Packs bits ######## of byte 3 
    
    // Pack 3-bit signal BMS_WatchdogTimeoutTaskName into payload at [ bit 32 to bit 35 ).
    const RtosTaskName BMS_WatchdogTimeoutTaskName_val = in_msg->BMS_WatchdogTimeoutTaskName_value;
    const uint32_t BMS_WatchdogTimeoutTaskName_raw = CAN_ENCODE(BMS_WatchdogTimeoutTaskName_val, CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_SCALE, CANSIG_BMS_WATCHDOG_TIMEOUT_TASK_NAME_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(BMS_WatchdogTimeoutTaskName_raw, 0, 0x7);   // Packs bits _____### of byte 4 
    
    // Pack 16-bit signal BMS_ModuleCommunication_NumCommTries into payload at [ bit 35 to bit 51 ).
    const uint32_t BMS_ModuleCommunication_NumCommTries_val = in_msg->BMS_ModuleCommunication_NumCommTries_value;
    const uint32_t BMS_ModuleCommunication_NumCommTries_raw = CAN_ENCODE(BMS_ModuleCommunication_NumCommTries_val, CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_SCALE, CANSIG_BMS_MODULE_COMMUNICATION_NUM_COMM_TRIES_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(BMS_ModuleCommunication_NumCommTries_raw, 3, 0xF8);   // Packs bits #####___ of byte 4 
    out_data[5] |= packShiftRight(BMS_ModuleCommunication_NumCommTries_raw, 5, 0xFF);   // Packs bits ######## of byte 5 
    out_data[6] |= packShiftRight(BMS_ModuleCommunication_NumCommTries_raw, 13, 0x7);   // Packs bits _____### of byte 6 
    
    // Pack 1-bit signal BMS_ModuleCommunication_MonitorState into payload at [ bit 51 to bit 52 ).
    const CAN_AccumulatorMonitorState BMS_ModuleCommunication_MonitorState_val = in_msg->BMS_ModuleCommunication_MonitorState_value;
    const uint32_t BMS_ModuleCommunication_MonitorState_raw = CAN_ENCODE(BMS_ModuleCommunication_MonitorState_val, CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_SCALE, CANSIG_BMS_MODULE_COMMUNICATION_MONITOR_STATE_OFFSET, uint32_t);
    out_data[6] |= packShiftLeft(BMS_ModuleCommunication_MonitorState_raw, 3, 0x8);   // Packs bits ____#___ of byte 6 
    
    
}
 
// pack function for BMS_ImdStatus
void app_canUtils_BMS_ImdStatus_pack(const BMS_ImdStatus_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_ImdStatus.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_____FFF|FFFFFEEE|EEEEEDDD|CBBBBBBB|BBBBBBBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_ImdOkHs into payload at [ bit 0 to bit 1 ).
    const bool BMS_ImdOkHs_val = in_msg->BMS_ImdOkHs_value;
    const uint32_t BMS_ImdOkHs_raw = CAN_ENCODE(BMS_ImdOkHs_val, CANSIG_BMS_IMD_OK_HS_SCALE, CANSIG_BMS_IMD_OK_HS_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_ImdOkHs_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 14-bit signal BMS_ImdTimeSincePowerOn into payload at [ bit 1 to bit 15 ).
    const uint32_t BMS_ImdTimeSincePowerOn_val = in_msg->BMS_ImdTimeSincePowerOn_value;
    const uint32_t BMS_ImdTimeSincePowerOn_raw = CAN_ENCODE(BMS_ImdTimeSincePowerOn_val, CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_SCALE, CANSIG_BMS_IMD_TIME_SINCE_POWER_ON_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ImdTimeSincePowerOn_raw, 1, 0xFE);   // Packs bits #######_ of byte 0 
    out_data[1] |= packShiftRight(BMS_ImdTimeSincePowerOn_raw, 7, 0x7F);   // Packs bits _####### of byte 1 
    
    // Pack 1-bit signal BMS_ImdValidDutyCycle into payload at [ bit 15 to bit 16 ).
    const bool BMS_ImdValidDutyCycle_val = in_msg->BMS_ImdValidDutyCycle_value;
    const uint32_t BMS_ImdValidDutyCycle_raw = CAN_ENCODE(BMS_ImdValidDutyCycle_val, CANSIG_BMS_IMD_VALID_DUTY_CYCLE_SCALE, CANSIG_BMS_IMD_VALID_DUTY_CYCLE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_ImdValidDutyCycle_raw, 7, 0x80);   // Packs bits #_______ of byte 1 
    
    // Pack 3-bit signal BMS_ImdCondition into payload at [ bit 16 to bit 19 ).
    const ImdConditionName BMS_ImdCondition_val = in_msg->BMS_ImdCondition_value;
    const uint32_t BMS_ImdCondition_raw = CAN_ENCODE(BMS_ImdCondition_val, CANSIG_BMS_IMD_CONDITION_SCALE, CANSIG_BMS_IMD_CONDITION_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(BMS_ImdCondition_raw, 0, 0x7);   // Packs bits _____### of byte 2 
    
    // Pack 8-bit signal BMS_ImdDutyCycle into payload at [ bit 19 to bit 27 ).
    const float BMS_ImdDutyCycle_val = in_msg->BMS_ImdDutyCycle_value;
    const uint32_t BMS_ImdDutyCycle_raw = CAN_ENCODE(BMS_ImdDutyCycle_val, CANSIG_BMS_IMD_DUTY_CYCLE_SCALE, CANSIG_BMS_IMD_DUTY_CYCLE_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_ImdDutyCycle_raw, 3, 0xF8);   // Packs bits #####___ of byte 2 
    out_data[3] |= packShiftRight(BMS_ImdDutyCycle_raw, 5, 0x7);   // Packs bits _____### of byte 3 
    
    // Pack 8-bit signal BMS_ImdFrequency into payload at [ bit 27 to bit 35 ).
    const float BMS_ImdFrequency_val = in_msg->BMS_ImdFrequency_value;
    const uint32_t BMS_ImdFrequency_raw = CAN_ENCODE(BMS_ImdFrequency_val, CANSIG_BMS_IMD_FREQUENCY_SCALE, CANSIG_BMS_IMD_FREQUENCY_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_ImdFrequency_raw, 3, 0xF8);   // Packs bits #####___ of byte 3 
    out_data[4] |= packShiftRight(BMS_ImdFrequency_raw, 5, 0x7);   // Packs bits _____### of byte 4 
    
    
}
 
// pack function for BMS_ImdData
void app_canUtils_BMS_ImdData_pack(const BMS_ImdData_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_ImdData.
    // |_DDDDDDD|DDDDDDDD|DDDDDCCC|CCCCCCCC|CCCCCCCC|CBBBBBBB|BBBBBBBB|BBBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal BMS_ImdActiveFrequency into payload at [ bit 0 to bit 3 ).
    const ImdActiveFrequency BMS_ImdActiveFrequency_val = in_msg->BMS_ImdActiveFrequency_value;
    const uint32_t BMS_ImdActiveFrequency_raw = CAN_ENCODE(BMS_ImdActiveFrequency_val, CANSIG_BMS_IMD_ACTIVE_FREQUENCY_SCALE, CANSIG_BMS_IMD_ACTIVE_FREQUENCY_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_ImdActiveFrequency_raw, 0, 0x7);   // Packs bits _____### of byte 0 
    
    // Pack 20-bit signal BMS_ImdInsulationMeasurementDcp10Hz into payload at [ bit 3 to bit 23 ).
    const float BMS_ImdInsulationMeasurementDcp10Hz_val = in_msg->BMS_ImdInsulationMeasurementDcp10Hz_value;
    const uint32_t BMS_ImdInsulationMeasurementDcp10Hz_raw = CAN_ENCODE(BMS_ImdInsulationMeasurementDcp10Hz_val, CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_SCALE, CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ImdInsulationMeasurementDcp10Hz_raw, 3, 0xF8);   // Packs bits #####___ of byte 0 
    out_data[1] |= packShiftRight(BMS_ImdInsulationMeasurementDcp10Hz_raw, 5, 0xFF);   // Packs bits ######## of byte 1 
    out_data[2] |= packShiftRight(BMS_ImdInsulationMeasurementDcp10Hz_raw, 13, 0x7F);   // Packs bits _####### of byte 2 
    
    // Pack 20-bit signal BMS_ImdInsulationMeasurementDcp20Hz into payload at [ bit 23 to bit 43 ).
    const float BMS_ImdInsulationMeasurementDcp20Hz_val = in_msg->BMS_ImdInsulationMeasurementDcp20Hz_value;
    const uint32_t BMS_ImdInsulationMeasurementDcp20Hz_raw = CAN_ENCODE(BMS_ImdInsulationMeasurementDcp20Hz_val, CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_SCALE, CANSIG_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_ImdInsulationMeasurementDcp20Hz_raw, 7, 0x80);   // Packs bits #_______ of byte 2 
    out_data[3] |= packShiftRight(BMS_ImdInsulationMeasurementDcp20Hz_raw, 1, 0xFF);   // Packs bits ######## of byte 3 
    out_data[4] |= packShiftRight(BMS_ImdInsulationMeasurementDcp20Hz_raw, 9, 0xFF);   // Packs bits ######## of byte 4 
    out_data[5] |= packShiftRight(BMS_ImdInsulationMeasurementDcp20Hz_raw, 17, 0x7);   // Packs bits _____### of byte 5 
    
    // Pack 20-bit signal BMS_ImdSpeedStartStatus30Hz into payload at [ bit 43 to bit 63 ).
    const float BMS_ImdSpeedStartStatus30Hz_val = in_msg->BMS_ImdSpeedStartStatus30Hz_value;
    const uint32_t BMS_ImdSpeedStartStatus30Hz_raw = CAN_ENCODE(BMS_ImdSpeedStartStatus30Hz_val, CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_SCALE, CANSIG_BMS_IMD_SPEED_START_STATUS30_HZ_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(BMS_ImdSpeedStartStatus30Hz_raw, 3, 0xF8);   // Packs bits #####___ of byte 5 
    out_data[6] |= packShiftRight(BMS_ImdSpeedStartStatus30Hz_raw, 5, 0xFF);   // Packs bits ######## of byte 6 
    out_data[7] |= packShiftRight(BMS_ImdSpeedStartStatus30Hz_raw, 13, 0x7F);   // Packs bits _####### of byte 7 
    
    
}
 
// pack function for BMS_Charger
void app_canUtils_BMS_Charger_pack(const BMS_Charger_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_Charger.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_ChargerConnected into payload at [ bit 0 to bit 1 ).
    const bool BMS_ChargerConnected_val = in_msg->BMS_ChargerConnected_value;
    const uint32_t BMS_ChargerConnected_raw = CAN_ENCODE(BMS_ChargerConnected_val, CANSIG_BMS_CHARGER_CONNECTED_SCALE, CANSIG_BMS_CHARGER_CONNECTED_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_ChargerConnected_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    
}
 
// pack function for BMS_BrusaControls
void app_canUtils_BMS_BrusaControls_pack(const BMS_BrusaControls_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_BrusaControls.
    // |xxxxxxxx|EEEEEEEE|EEEEEEEE|DDDDDDDD|DDDDDDDD|CCCCCCCC|CCCCCCCC|AB______|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_ChargerEnable into payload at [ bit 7 to bit 8 ).
    const bool BMS_ChargerEnable_val = in_msg->BMS_ChargerEnable_value;
    const uint32_t BMS_ChargerEnable_raw = CAN_ENCODE(BMS_ChargerEnable_val, CANSIG_BMS_CHARGER_ENABLE_SCALE, CANSIG_BMS_CHARGER_ENABLE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ChargerEnable_raw, 7, 0x80);   // Packs bits #_______ of byte 0 
    
    // Pack 1-bit signal BMS_ClearLatch into payload at [ bit 6 to bit 7 ).
    const bool BMS_ClearLatch_val = in_msg->BMS_ClearLatch_value;
    const uint32_t BMS_ClearLatch_raw = CAN_ENCODE(BMS_ClearLatch_val, CANSIG_BMS_CLEAR_LATCH_SCALE, CANSIG_BMS_CLEAR_LATCH_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ClearLatch_raw, 6, 0x40);   // Packs bits _#______ of byte 0 
    
    // Pack 16-bit signal BMS_MaxMainsCurrent into payload at [ bit 8 to bit 24 ).
    const float BMS_MaxMainsCurrent_val = in_msg->BMS_MaxMainsCurrent_value;
    const uint32_t BMS_MaxMainsCurrent_raw = CAN_ENCODE(BMS_MaxMainsCurrent_val, CANSIG_BMS_MAX_MAINS_CURRENT_SCALE, CANSIG_BMS_MAX_MAINS_CURRENT_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(BMS_MaxMainsCurrent_raw, 0, 0xFF);   // Packs bits ######## of byte 1 
    out_data[2] |= packShiftRight(BMS_MaxMainsCurrent_raw, 8, 0xFF);   // Packs bits ######## of byte 2 
    
    // Pack 16-bit signal BMS_ChargingVoltage into payload at [ bit 24 to bit 40 ).
    const float BMS_ChargingVoltage_val = in_msg->BMS_ChargingVoltage_value;
    const uint32_t BMS_ChargingVoltage_raw = CAN_ENCODE(BMS_ChargingVoltage_val, CANSIG_BMS_CHARGING_VOLTAGE_SCALE, CANSIG_BMS_CHARGING_VOLTAGE_OFFSET, uint32_t);
    out_data[3] |= packShiftRight(BMS_ChargingVoltage_raw, 0, 0xFF);   // Packs bits ######## of byte 3 
    out_data[4] |= packShiftRight(BMS_ChargingVoltage_raw, 8, 0xFF);   // Packs bits ######## of byte 4 
    
    // Pack 16-bit signal BMS_ChargingCurrent into payload at [ bit 40 to bit 56 ).
    const float BMS_ChargingCurrent_val = in_msg->BMS_ChargingCurrent_value;
    const uint32_t BMS_ChargingCurrent_raw = CAN_ENCODE(BMS_ChargingCurrent_val, CANSIG_BMS_CHARGING_CURRENT_SCALE, CANSIG_BMS_CHARGING_CURRENT_OFFSET, uint32_t);
    out_data[5] |= packShiftRight(BMS_ChargingCurrent_raw, 0, 0xFF);   // Packs bits ######## of byte 5 
    out_data[6] |= packShiftRight(BMS_ChargingCurrent_raw, 8, 0xFF);   // Packs bits ######## of byte 6 
    
    
}
 
// pack function for BMS_OkStatuses
void app_canUtils_BMS_OkStatuses_pack(const BMS_OkStatuses_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_OkStatuses.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__FEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_BmsOk into payload at [ bit 0 to bit 1 ).
    const bool BMS_BmsOk_val = in_msg->BMS_BmsOk_value;
    const uint32_t BMS_BmsOk_raw = CAN_ENCODE(BMS_BmsOk_val, CANSIG_BMS_BMS_OK_SCALE, CANSIG_BMS_BMS_OK_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_BmsOk_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 1-bit signal BMS_ImdOk into payload at [ bit 1 to bit 2 ).
    const bool BMS_ImdOk_val = in_msg->BMS_ImdOk_value;
    const uint32_t BMS_ImdOk_raw = CAN_ENCODE(BMS_ImdOk_val, CANSIG_BMS_IMD_OK_SCALE, CANSIG_BMS_IMD_OK_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ImdOk_raw, 1, 0x2);   // Packs bits ______#_ of byte 0 
    
    // Pack 1-bit signal BMS_BspdOk into payload at [ bit 2 to bit 3 ).
    const bool BMS_BspdOk_val = in_msg->BMS_BspdOk_value;
    const uint32_t BMS_BspdOk_raw = CAN_ENCODE(BMS_BspdOk_val, CANSIG_BMS_BSPD_OK_SCALE, CANSIG_BMS_BSPD_OK_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_BspdOk_raw, 2, 0x4);   // Packs bits _____#__ of byte 0 
    
    // Pack 1-bit signal BMS_BmsLatchedFault into payload at [ bit 3 to bit 4 ).
    const bool BMS_BmsLatchedFault_val = in_msg->BMS_BmsLatchedFault_value;
    const uint32_t BMS_BmsLatchedFault_raw = CAN_ENCODE(BMS_BmsLatchedFault_val, CANSIG_BMS_BMS_LATCHED_FAULT_SCALE, CANSIG_BMS_BMS_LATCHED_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_BmsLatchedFault_raw, 3, 0x8);   // Packs bits ____#___ of byte 0 
    
    // Pack 1-bit signal BMS_ImdLatchedFault into payload at [ bit 4 to bit 5 ).
    const bool BMS_ImdLatchedFault_val = in_msg->BMS_ImdLatchedFault_value;
    const uint32_t BMS_ImdLatchedFault_raw = CAN_ENCODE(BMS_ImdLatchedFault_val, CANSIG_BMS_IMD_LATCHED_FAULT_SCALE, CANSIG_BMS_IMD_LATCHED_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ImdLatchedFault_raw, 4, 0x10);   // Packs bits ___#____ of byte 0 
    
    // Pack 1-bit signal BMS_BspdLatchedFault into payload at [ bit 5 to bit 6 ).
    const bool BMS_BspdLatchedFault_val = in_msg->BMS_BspdLatchedFault_value;
    const uint32_t BMS_BspdLatchedFault_raw = CAN_ENCODE(BMS_BspdLatchedFault_val, CANSIG_BMS_BSPD_LATCHED_FAULT_SCALE, CANSIG_BMS_BSPD_LATCHED_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_BspdLatchedFault_raw, 5, 0x20);   // Packs bits __#_____ of byte 0 
    
    
}
 
// pack function for BMS_Contactors
void app_canUtils_BMS_Contactors_pack(const BMS_Contactors_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_Contactors.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_AirPositive into payload at [ bit 0 to bit 1 ).
    const ContactorState BMS_AirPositive_val = in_msg->BMS_AirPositive_value;
    const uint32_t BMS_AirPositive_raw = CAN_ENCODE(BMS_AirPositive_val, CANSIG_BMS_AIR_POSITIVE_SCALE, CANSIG_BMS_AIR_POSITIVE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_AirPositive_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 1-bit signal BMS_AirNegative into payload at [ bit 1 to bit 2 ).
    const ContactorState BMS_AirNegative_val = in_msg->BMS_AirNegative_value;
    const uint32_t BMS_AirNegative_raw = CAN_ENCODE(BMS_AirNegative_val, CANSIG_BMS_AIR_NEGATIVE_SCALE, CANSIG_BMS_AIR_NEGATIVE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_AirNegative_raw, 1, 0x2);   // Packs bits ______#_ of byte 0 
    
    // Pack 1-bit signal BMS_PrechargeRelay into payload at [ bit 2 to bit 3 ).
    const ContactorState BMS_PrechargeRelay_val = in_msg->BMS_PrechargeRelay_value;
    const uint32_t BMS_PrechargeRelay_raw = CAN_ENCODE(BMS_PrechargeRelay_val, CANSIG_BMS_PRECHARGE_RELAY_SCALE, CANSIG_BMS_PRECHARGE_RELAY_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_PrechargeRelay_raw, 2, 0x4);   // Packs bits _____#__ of byte 0 
    
    
}
 
// pack function for BMS_CellTemperatures
void app_canUtils_BMS_CellTemperatures_pack(const BMS_CellTemperatures_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_CellTemperatures.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____BBBB|BBBBBBAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 10-bit signal BMS_MinCellTemperature into payload at [ bit 0 to bit 10 ).
    const float BMS_MinCellTemperature_val = in_msg->BMS_MinCellTemperature_value;
    const uint32_t BMS_MinCellTemperature_raw = CAN_ENCODE(BMS_MinCellTemperature_val, CANSIG_BMS_MIN_CELL_TEMPERATURE_SCALE, CANSIG_BMS_MIN_CELL_TEMPERATURE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_MinCellTemperature_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(BMS_MinCellTemperature_raw, 8, 0x3);   // Packs bits ______## of byte 1 
    
    // Pack 10-bit signal BMS_MaxCellTemperature into payload at [ bit 10 to bit 20 ).
    const float BMS_MaxCellTemperature_val = in_msg->BMS_MaxCellTemperature_value;
    const uint32_t BMS_MaxCellTemperature_raw = CAN_ENCODE(BMS_MaxCellTemperature_val, CANSIG_BMS_MAX_CELL_TEMPERATURE_SCALE, CANSIG_BMS_MAX_CELL_TEMPERATURE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_MaxCellTemperature_raw, 2, 0xFC);   // Packs bits ######__ of byte 1 
    out_data[2] |= packShiftRight(BMS_MaxCellTemperature_raw, 6, 0xF);   // Packs bits ____#### of byte 2 
    
    
}
 
// pack function for BMS_VoltageAndChargeStats
void app_canUtils_BMS_VoltageAndChargeStats_pack(const BMS_VoltageAndChargeStats_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_VoltageAndChargeStats.
    // |xxxxxxxx|___EEEEE|EEEEEEEE|DDDDDDDD|DDDDDCBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal BMS_PackVoltage into payload at [ bit 0 to bit 16 ).
    const float BMS_PackVoltage_val = in_msg->BMS_PackVoltage_value;
    const uint32_t BMS_PackVoltage_raw = CAN_ENCODE(BMS_PackVoltage_val, CANSIG_BMS_PACK_VOLTAGE_SCALE, CANSIG_BMS_PACK_VOLTAGE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_PackVoltage_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(BMS_PackVoltage_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    
    // Pack 10-bit signal BMS_Soc into payload at [ bit 16 to bit 26 ).
    const float BMS_Soc_val = in_msg->BMS_Soc_value;
    const uint32_t BMS_Soc_raw = CAN_ENCODE(BMS_Soc_val, CANSIG_BMS_SOC_SCALE, CANSIG_BMS_SOC_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(BMS_Soc_raw, 0, 0xFF);   // Packs bits ######## of byte 2 
    out_data[3] |= packShiftRight(BMS_Soc_raw, 8, 0x3);   // Packs bits ______## of byte 3 
    
    // Pack 1-bit signal BMS_SocCorrupt into payload at [ bit 26 to bit 27 ).
    const bool BMS_SocCorrupt_val = in_msg->BMS_SocCorrupt_value;
    const uint32_t BMS_SocCorrupt_raw = CAN_ENCODE(BMS_SocCorrupt_val, CANSIG_BMS_SOC_CORRUPT_SCALE, CANSIG_BMS_SOC_CORRUPT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_SocCorrupt_raw, 2, 0x4);   // Packs bits _____#__ of byte 3 
    
    // Pack 13-bit signal BMS_MinCellVoltage into payload at [ bit 27 to bit 40 ).
    const float BMS_MinCellVoltage_val = in_msg->BMS_MinCellVoltage_value;
    const uint32_t BMS_MinCellVoltage_raw = CAN_ENCODE(BMS_MinCellVoltage_val, CANSIG_BMS_MIN_CELL_VOLTAGE_SCALE, CANSIG_BMS_MIN_CELL_VOLTAGE_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_MinCellVoltage_raw, 3, 0xF8);   // Packs bits #####___ of byte 3 
    out_data[4] |= packShiftRight(BMS_MinCellVoltage_raw, 5, 0xFF);   // Packs bits ######## of byte 4 
    
    // Pack 13-bit signal BMS_MaxCellVoltage into payload at [ bit 40 to bit 53 ).
    const float BMS_MaxCellVoltage_val = in_msg->BMS_MaxCellVoltage_value;
    const uint32_t BMS_MaxCellVoltage_raw = CAN_ENCODE(BMS_MaxCellVoltage_val, CANSIG_BMS_MAX_CELL_VOLTAGE_SCALE, CANSIG_BMS_MAX_CELL_VOLTAGE_OFFSET, uint32_t);
    out_data[5] |= packShiftRight(BMS_MaxCellVoltage_raw, 0, 0xFF);   // Packs bits ######## of byte 5 
    out_data[6] |= packShiftRight(BMS_MaxCellVoltage_raw, 8, 0x1F);   // Packs bits ___##### of byte 6 
    
    
}
 
// pack function for BMS_CellStats
void app_canUtils_BMS_CellStats_pack(const BMS_CellStats_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_CellStats.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____HHHH|GGGFFFFE|EEDDDDCC|CBBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal BMS_MinCellVoltageSegment into payload at [ bit 0 to bit 3 ).
    const uint32_t BMS_MinCellVoltageSegment_val = in_msg->BMS_MinCellVoltageSegment_value;
    const uint32_t BMS_MinCellVoltageSegment_raw = CAN_ENCODE(BMS_MinCellVoltageSegment_val, CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_SCALE, CANSIG_BMS_MIN_CELL_VOLTAGE_SEGMENT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_MinCellVoltageSegment_raw, 0, 0x7);   // Packs bits _____### of byte 0 
    
    // Pack 4-bit signal BMS_MinCellVoltageIdx into payload at [ bit 3 to bit 7 ).
    const uint32_t BMS_MinCellVoltageIdx_val = in_msg->BMS_MinCellVoltageIdx_value;
    const uint32_t BMS_MinCellVoltageIdx_raw = CAN_ENCODE(BMS_MinCellVoltageIdx_val, CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_SCALE, CANSIG_BMS_MIN_CELL_VOLTAGE_IDX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_MinCellVoltageIdx_raw, 3, 0x78);   // Packs bits _####___ of byte 0 
    
    // Pack 3-bit signal BMS_MaxCellVoltageSegment into payload at [ bit 7 to bit 10 ).
    const uint32_t BMS_MaxCellVoltageSegment_val = in_msg->BMS_MaxCellVoltageSegment_value;
    const uint32_t BMS_MaxCellVoltageSegment_raw = CAN_ENCODE(BMS_MaxCellVoltageSegment_val, CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_SCALE, CANSIG_BMS_MAX_CELL_VOLTAGE_SEGMENT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_MaxCellVoltageSegment_raw, 7, 0x80);   // Packs bits #_______ of byte 0 
    out_data[1] |= packShiftRight(BMS_MaxCellVoltageSegment_raw, 1, 0x3);   // Packs bits ______## of byte 1 
    
    // Pack 4-bit signal BMS_MaxCellVoltageIdx into payload at [ bit 10 to bit 14 ).
    const uint32_t BMS_MaxCellVoltageIdx_val = in_msg->BMS_MaxCellVoltageIdx_value;
    const uint32_t BMS_MaxCellVoltageIdx_raw = CAN_ENCODE(BMS_MaxCellVoltageIdx_val, CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_SCALE, CANSIG_BMS_MAX_CELL_VOLTAGE_IDX_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_MaxCellVoltageIdx_raw, 2, 0x3C);   // Packs bits __####__ of byte 1 
    
    // Pack 3-bit signal BMS_MinTempSegment into payload at [ bit 14 to bit 17 ).
    const uint32_t BMS_MinTempSegment_val = in_msg->BMS_MinTempSegment_value;
    const uint32_t BMS_MinTempSegment_raw = CAN_ENCODE(BMS_MinTempSegment_val, CANSIG_BMS_MIN_TEMP_SEGMENT_SCALE, CANSIG_BMS_MIN_TEMP_SEGMENT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_MinTempSegment_raw, 6, 0xC0);   // Packs bits ##______ of byte 1 
    out_data[2] |= packShiftRight(BMS_MinTempSegment_raw, 2, 0x1);   // Packs bits _______# of byte 2 
    
    // Pack 4-bit signal BMS_MinTempIdx into payload at [ bit 17 to bit 21 ).
    const uint32_t BMS_MinTempIdx_val = in_msg->BMS_MinTempIdx_value;
    const uint32_t BMS_MinTempIdx_raw = CAN_ENCODE(BMS_MinTempIdx_val, CANSIG_BMS_MIN_TEMP_IDX_SCALE, CANSIG_BMS_MIN_TEMP_IDX_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_MinTempIdx_raw, 1, 0x1E);   // Packs bits ___####_ of byte 2 
    
    // Pack 3-bit signal BMS_MaxTempSegment into payload at [ bit 21 to bit 24 ).
    const uint32_t BMS_MaxTempSegment_val = in_msg->BMS_MaxTempSegment_value;
    const uint32_t BMS_MaxTempSegment_raw = CAN_ENCODE(BMS_MaxTempSegment_val, CANSIG_BMS_MAX_TEMP_SEGMENT_SCALE, CANSIG_BMS_MAX_TEMP_SEGMENT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_MaxTempSegment_raw, 5, 0xE0);   // Packs bits ###_____ of byte 2 
    
    // Pack 4-bit signal BMS_MaxTempIdx into payload at [ bit 24 to bit 28 ).
    const uint32_t BMS_MaxTempIdx_val = in_msg->BMS_MaxTempIdx_value;
    const uint32_t BMS_MaxTempIdx_raw = CAN_ENCODE(BMS_MaxTempIdx_val, CANSIG_BMS_MAX_TEMP_IDX_SCALE, CANSIG_BMS_MAX_TEMP_IDX_OFFSET, uint32_t);
    out_data[3] |= packShiftRight(BMS_MaxTempIdx_raw, 0, 0xF);   // Packs bits ____#### of byte 3 
    
    
}
 
// pack function for BMS_TractiveSystem
void app_canUtils_BMS_TractiveSystem_pack(const BMS_TractiveSystem_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_TractiveSystem.
    // |xxxxxxxx|_EDDDDDD|DDDDDDDD|DDDCCCCC|CCCCCCBB|BBBBBBBB|BBBAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 13-bit signal BMS_TractiveSystemVoltage into payload at [ bit 0 to bit 13 ).
    const float BMS_TractiveSystemVoltage_val = in_msg->BMS_TractiveSystemVoltage_value;
    const uint32_t BMS_TractiveSystemVoltage_raw = CAN_ENCODE(BMS_TractiveSystemVoltage_val, CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_TractiveSystemVoltage_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(BMS_TractiveSystemVoltage_raw, 8, 0x1F);   // Packs bits ___##### of byte 1 
    
    // Pack 13-bit signal BMS_TractiveSystemCurrent into payload at [ bit 13 to bit 26 ).
    const float BMS_TractiveSystemCurrent_val = in_msg->BMS_TractiveSystemCurrent_value;
    const uint32_t BMS_TractiveSystemCurrent_raw = CAN_ENCODE(BMS_TractiveSystemCurrent_val, CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_TractiveSystemCurrent_raw, 5, 0xE0);   // Packs bits ###_____ of byte 1 
    out_data[2] |= packShiftRight(BMS_TractiveSystemCurrent_raw, 3, 0xFF);   // Packs bits ######## of byte 2 
    out_data[3] |= packShiftRight(BMS_TractiveSystemCurrent_raw, 11, 0x3);   // Packs bits ______## of byte 3 
    
    // Pack 11-bit signal BMS_TractiveSystemPower into payload at [ bit 26 to bit 37 ).
    const float BMS_TractiveSystemPower_val = in_msg->BMS_TractiveSystemPower_value;
    const uint32_t BMS_TractiveSystemPower_raw = CAN_ENCODE(BMS_TractiveSystemPower_val, CANSIG_BMS_TRACTIVE_SYSTEM_POWER_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_POWER_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_TractiveSystemPower_raw, 2, 0xFC);   // Packs bits ######__ of byte 3 
    out_data[4] |= packShiftRight(BMS_TractiveSystemPower_raw, 6, 0x1F);   // Packs bits ___##### of byte 4 
    
    // Pack 17-bit signal BMS_AvailablePower into payload at [ bit 37 to bit 54 ).
    const uint32_t BMS_AvailablePower_val = in_msg->BMS_AvailablePower_value;
    const uint32_t BMS_AvailablePower_raw = CAN_ENCODE(BMS_AvailablePower_val, CANSIG_BMS_AVAILABLE_POWER_SCALE, CANSIG_BMS_AVAILABLE_POWER_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(BMS_AvailablePower_raw, 5, 0xE0);   // Packs bits ###_____ of byte 4 
    out_data[5] |= packShiftRight(BMS_AvailablePower_raw, 3, 0xFF);   // Packs bits ######## of byte 5 
    out_data[6] |= packShiftRight(BMS_AvailablePower_raw, 11, 0x3F);   // Packs bits __###### of byte 6 
    
    // Pack 1-bit signal BMS_BSPDCurrentThresholdExceeded into payload at [ bit 54 to bit 55 ).
    const bool BMS_BSPDCurrentThresholdExceeded_val = in_msg->BMS_BSPDCurrentThresholdExceeded_value;
    const uint32_t BMS_BSPDCurrentThresholdExceeded_raw = CAN_ENCODE(BMS_BSPDCurrentThresholdExceeded_val, CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_SCALE, CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_OFFSET, uint32_t);
    out_data[6] |= packShiftLeft(BMS_BSPDCurrentThresholdExceeded_raw, 6, 0x40);   // Packs bits _#______ of byte 6 
    
    
}
 
// pack function for BMS_CommitInfo
void app_canUtils_BMS_CommitInfo_pack(const BMS_CommitInfo_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_CommitInfo.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_______B|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 32-bit signal BMS_Hash into payload at [ bit 0 to bit 32 ).
    const uint32_t BMS_Hash_val = in_msg->BMS_Hash_value;
    const uint32_t BMS_Hash_raw = CAN_ENCODE(BMS_Hash_val, CANSIG_BMS_HASH_SCALE, CANSIG_BMS_HASH_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Hash_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(BMS_Hash_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    out_data[2] |= packShiftRight(BMS_Hash_raw, 16, 0xFF);   // Packs bits ######## of byte 2 
    out_data[3] |= packShiftRight(BMS_Hash_raw, 24, 0xFF);   // Packs bits ######## of byte 3 
    
    // Pack 1-bit signal BMS_Clean into payload at [ bit 32 to bit 33 ).
    const bool BMS_Clean_val = in_msg->BMS_Clean_value;
    const uint32_t BMS_Clean_raw = CAN_ENCODE(BMS_Clean_val, CANSIG_BMS_CLEAN_SCALE, CANSIG_BMS_CLEAN_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(BMS_Clean_raw, 0, 0x1);   // Packs bits _______# of byte 4 
    
    
}
 
// pack function for BMS_OWC_Segment0to2_Status
void app_canUtils_BMS_OWC_Segment0to2_Status_pack(const BMS_OWC_Segment0to2_Status_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_OWC_Segment0to2_Status.
    // |xxxxxxxx|xxxxxxxx|CCCCCCCC|CCCCCCCC|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal BMS_Segment0_OWC_Cells_Status into payload at [ bit 0 to bit 16 ).
    const uint32_t BMS_Segment0_OWC_Cells_Status_val = in_msg->BMS_Segment0_OWC_Cells_Status_value;
    const uint32_t BMS_Segment0_OWC_Cells_Status_raw = CAN_ENCODE(BMS_Segment0_OWC_Cells_Status_val, CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_SCALE, CANSIG_BMS_SEGMENT0_OWC_CELLS_STATUS_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Segment0_OWC_Cells_Status_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(BMS_Segment0_OWC_Cells_Status_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    
    // Pack 16-bit signal BMS_Segment1_OWC_Cells_Status into payload at [ bit 16 to bit 32 ).
    const uint32_t BMS_Segment1_OWC_Cells_Status_val = in_msg->BMS_Segment1_OWC_Cells_Status_value;
    const uint32_t BMS_Segment1_OWC_Cells_Status_raw = CAN_ENCODE(BMS_Segment1_OWC_Cells_Status_val, CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_SCALE, CANSIG_BMS_SEGMENT1_OWC_CELLS_STATUS_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(BMS_Segment1_OWC_Cells_Status_raw, 0, 0xFF);   // Packs bits ######## of byte 2 
    out_data[3] |= packShiftRight(BMS_Segment1_OWC_Cells_Status_raw, 8, 0xFF);   // Packs bits ######## of byte 3 
    
    // Pack 16-bit signal BMS_Segment2_OWC_Cells_Status into payload at [ bit 32 to bit 48 ).
    const uint32_t BMS_Segment2_OWC_Cells_Status_val = in_msg->BMS_Segment2_OWC_Cells_Status_value;
    const uint32_t BMS_Segment2_OWC_Cells_Status_raw = CAN_ENCODE(BMS_Segment2_OWC_Cells_Status_val, CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_SCALE, CANSIG_BMS_SEGMENT2_OWC_CELLS_STATUS_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(BMS_Segment2_OWC_Cells_Status_raw, 0, 0xFF);   // Packs bits ######## of byte 4 
    out_data[5] |= packShiftRight(BMS_Segment2_OWC_Cells_Status_raw, 8, 0xFF);   // Packs bits ######## of byte 5 
    
    
}
 
// pack function for BMS_OWC_Segment3to4_Status
void app_canUtils_BMS_OWC_Segment3to4_Status_pack(const BMS_OWC_Segment3to4_Status_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_OWC_Segment3to4_Status.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal BMS_Segment3_OWC_Cells_Status into payload at [ bit 0 to bit 16 ).
    const uint32_t BMS_Segment3_OWC_Cells_Status_val = in_msg->BMS_Segment3_OWC_Cells_Status_value;
    const uint32_t BMS_Segment3_OWC_Cells_Status_raw = CAN_ENCODE(BMS_Segment3_OWC_Cells_Status_val, CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_SCALE, CANSIG_BMS_SEGMENT3_OWC_CELLS_STATUS_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Segment3_OWC_Cells_Status_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(BMS_Segment3_OWC_Cells_Status_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    
    // Pack 16-bit signal BMS_Segment4_OWC_Cells_Status into payload at [ bit 16 to bit 32 ).
    const uint32_t BMS_Segment4_OWC_Cells_Status_val = in_msg->BMS_Segment4_OWC_Cells_Status_value;
    const uint32_t BMS_Segment4_OWC_Cells_Status_raw = CAN_ENCODE(BMS_Segment4_OWC_Cells_Status_val, CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_SCALE, CANSIG_BMS_SEGMENT4_OWC_CELLS_STATUS_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(BMS_Segment4_OWC_Cells_Status_raw, 0, 0xFF);   // Packs bits ######## of byte 2 
    out_data[3] |= packShiftRight(BMS_Segment4_OWC_Cells_Status_raw, 8, 0xFF);   // Packs bits ######## of byte 3 
    
    
}
 
// pack function for BMS_BMSShdnNodeStatus
void app_canUtils_BMS_BMSShdnNodeStatus_pack(const BMS_BMSShdnNodeStatus_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_BMSShdnNodeStatus.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_HVDShdnOKStatus into payload at [ bit 0 to bit 1 ).
    const bool BMS_HVDShdnOKStatus_val = in_msg->BMS_HVDShdnOKStatus_value;
    const uint32_t BMS_HVDShdnOKStatus_raw = CAN_ENCODE(BMS_HVDShdnOKStatus_val, CANSIG_BMS_HVD_SHDN_OK_STATUS_SCALE, CANSIG_BMS_HVD_SHDN_OK_STATUS_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_HVDShdnOKStatus_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 1-bit signal BMS_TSIlckOKStatus into payload at [ bit 1 to bit 2 ).
    const bool BMS_TSIlckOKStatus_val = in_msg->BMS_TSIlckOKStatus_value;
    const uint32_t BMS_TSIlckOKStatus_raw = CAN_ENCODE(BMS_TSIlckOKStatus_val, CANSIG_BMS_TS_ILCK_OK_STATUS_SCALE, CANSIG_BMS_TS_ILCK_OK_STATUS_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_TSIlckOKStatus_raw, 1, 0x2);   // Packs bits ______#_ of byte 0 
    
    
}




// unpack function for VC_Vitals
void app_canUtils_VC_Vitals_unpack(const uint8_t* const in_data, VC_Vitals_Signals* const out_msg)
{
    // Unpack message VC_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______EE|EEDDDDDD|DDDDCBBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t VC_Heartbeat_raw = 0; // Temporary raw variable for VC_Heartbeat
    VC_Heartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1); // Unpacks bits _______# from byte 0
    out_msg->VC_Heartbeat_value = CAN_DECODE(VC_Heartbeat_raw, CANSIG_VC_HEARTBEAT_SCALE, CANSIG_VC_HEARTBEAT_OFFSET, uint32_t);
    
    uint32_t VC_State_raw = 0; // Temporary raw variable for VC_State
    VC_State_raw |= unpackShiftLeft(in_data[0], 1, 0x6); // Unpacks bits _____##_ from byte 0
    out_msg->VC_State_value = CAN_DECODE(VC_State_raw, CANSIG_VC_STATE_SCALE, CANSIG_VC_STATE_OFFSET, uint32_t);
    
    uint32_t VC_BuzzerOn_raw = 0; // Temporary raw variable for VC_BuzzerOn
    VC_BuzzerOn_raw |= unpackShiftLeft(in_data[0], 3, 0x8); // Unpacks bits ____#___ from byte 0
    out_msg->VC_BuzzerOn_value = CAN_DECODE(VC_BuzzerOn_raw, CANSIG_VC_BUZZER_ON_SCALE, CANSIG_VC_BUZZER_ON_OFFSET, uint32_t);
    
    uint32_t VC_NumberOfCanDataLogs_raw = 0; // Temporary raw variable for VC_NumberOfCanDataLogs
    VC_NumberOfCanDataLogs_raw |= unpackShiftLeft(in_data[0], 4, 0xF0); // Unpacks bits ####____ from byte 0
    VC_NumberOfCanDataLogs_raw |= unpackShiftRight(in_data[1], 4, 0x3F); // Unpacks bits __###### from byte 1
    out_msg->VC_NumberOfCanDataLogs_value = CAN_DECODE(VC_NumberOfCanDataLogs_raw, CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_SCALE, CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_OFFSET, uint32_t);
    
    uint32_t VC_CanLoggingRemainingErrors_raw = 0; // Temporary raw variable for VC_CanLoggingRemainingErrors
    VC_CanLoggingRemainingErrors_raw |= unpackShiftLeft(in_data[1], 6, 0xC0); // Unpacks bits ##______ from byte 1
    VC_CanLoggingRemainingErrors_raw |= unpackShiftRight(in_data[2], 2, 0x3); // Unpacks bits ______## from byte 2
    out_msg->VC_CanLoggingRemainingErrors_value = CAN_DECODE(VC_CanLoggingRemainingErrors_raw, CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_SCALE, CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_OFFSET, uint32_t);
    
    
}
