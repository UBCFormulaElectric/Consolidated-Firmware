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
 
// pack function for BMS_Warnings
void app_canUtils_BMS_Warnings_pack(const BMS_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_ONMLKJI|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_Warning_StackWaterMarkHighTask1Hz into payload at [ bit 0 to bit 1 ).
    const bool BMS_Warning_StackWaterMarkHighTask1Hz_val = in_msg->BMS_Warning_StackWaterMarkHighTask1Hz_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask1Hz_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask1Hz_val, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Warning_StackWaterMarkHighTask1Hz_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 1-bit signal BMS_Warning_StackWaterMarkHighTask100Hz into payload at [ bit 1 to bit 2 ).
    const bool BMS_Warning_StackWaterMarkHighTask100Hz_val = in_msg->BMS_Warning_StackWaterMarkHighTask100Hz_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask100Hz_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask100Hz_val, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTask100Hz_raw, 1, 0x2);   // Packs bits ______#_ of byte 0 
    
    // Pack 1-bit signal BMS_Warning_StackWaterMarkHighTask1kHz into payload at [ bit 2 to bit 3 ).
    const bool BMS_Warning_StackWaterMarkHighTask1kHz_val = in_msg->BMS_Warning_StackWaterMarkHighTask1kHz_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask1kHz_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask1kHz_val, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTask1kHz_raw, 2, 0x4);   // Packs bits _____#__ of byte 0 
    
    // Pack 1-bit signal BMS_Warning_StackWaterMarkHighTaskCanRx into payload at [ bit 3 to bit 4 ).
    const bool BMS_Warning_StackWaterMarkHighTaskCanRx_val = in_msg->BMS_Warning_StackWaterMarkHighTaskCanRx_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanRx_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTaskCanRx_val, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTaskCanRx_raw, 3, 0x8);   // Packs bits ____#___ of byte 0 
    
    // Pack 1-bit signal BMS_Warning_StackWaterMarkHighTaskCanTx into payload at [ bit 4 to bit 5 ).
    const bool BMS_Warning_StackWaterMarkHighTaskCanTx_val = in_msg->BMS_Warning_StackWaterMarkHighTaskCanTx_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanTx_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTaskCanTx_val, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTaskCanTx_raw, 4, 0x10);   // Packs bits ___#____ of byte 0 
    
    // Pack 1-bit signal BMS_Warning_WatchdogTimeout into payload at [ bit 5 to bit 6 ).
    const bool BMS_Warning_WatchdogTimeout_val = in_msg->BMS_Warning_WatchdogTimeout_value;
    const uint32_t BMS_Warning_WatchdogTimeout_raw = CAN_ENCODE(BMS_Warning_WatchdogTimeout_val, CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_WatchdogTimeout_raw, 5, 0x20);   // Packs bits __#_____ of byte 0 
    
    // Pack 1-bit signal BMS_Warning_TxOverflow into payload at [ bit 6 to bit 7 ).
    const bool BMS_Warning_TxOverflow_val = in_msg->BMS_Warning_TxOverflow_value;
    const uint32_t BMS_Warning_TxOverflow_raw = CAN_ENCODE(BMS_Warning_TxOverflow_val, CANSIG_BMS_WARNING_TX_OVERFLOW_SCALE, CANSIG_BMS_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_TxOverflow_raw, 6, 0x40);   // Packs bits _#______ of byte 0 
    
    // Pack 1-bit signal BMS_Warning_RxOverflow into payload at [ bit 7 to bit 8 ).
    const bool BMS_Warning_RxOverflow_val = in_msg->BMS_Warning_RxOverflow_value;
    const uint32_t BMS_Warning_RxOverflow_raw = CAN_ENCODE(BMS_Warning_RxOverflow_val, CANSIG_BMS_WARNING_RX_OVERFLOW_SCALE, CANSIG_BMS_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_RxOverflow_raw, 7, 0x80);   // Packs bits #_______ of byte 0 
    
    // Pack 1-bit signal BMS_Warning_OpenWireCheckFault into payload at [ bit 8 to bit 9 ).
    const bool BMS_Warning_OpenWireCheckFault_val = in_msg->BMS_Warning_OpenWireCheckFault_value;
    const uint32_t BMS_Warning_OpenWireCheckFault_raw = CAN_ENCODE(BMS_Warning_OpenWireCheckFault_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(BMS_Warning_OpenWireCheckFault_raw, 0, 0x1);   // Packs bits _______# of byte 1 
    
    // Pack 1-bit signal BMS_Warning_OpenWireCheck_Segment0_GND into payload at [ bit 9 to bit 10 ).
    const bool BMS_Warning_OpenWireCheck_Segment0_GND_val = in_msg->BMS_Warning_OpenWireCheck_Segment0_GND_value;
    const uint32_t BMS_Warning_OpenWireCheck_Segment0_GND_raw = CAN_ENCODE(BMS_Warning_OpenWireCheck_Segment0_GND_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_OpenWireCheck_Segment0_GND_raw, 1, 0x2);   // Packs bits ______#_ of byte 1 
    
    // Pack 1-bit signal BMS_Warning_OpenWireCheck_Segment1_GND into payload at [ bit 10 to bit 11 ).
    const bool BMS_Warning_OpenWireCheck_Segment1_GND_val = in_msg->BMS_Warning_OpenWireCheck_Segment1_GND_value;
    const uint32_t BMS_Warning_OpenWireCheck_Segment1_GND_raw = CAN_ENCODE(BMS_Warning_OpenWireCheck_Segment1_GND_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_OpenWireCheck_Segment1_GND_raw, 2, 0x4);   // Packs bits _____#__ of byte 1 
    
    // Pack 1-bit signal BMS_Warning_OpenWireCheck_Segment2_GND into payload at [ bit 11 to bit 12 ).
    const bool BMS_Warning_OpenWireCheck_Segment2_GND_val = in_msg->BMS_Warning_OpenWireCheck_Segment2_GND_value;
    const uint32_t BMS_Warning_OpenWireCheck_Segment2_GND_raw = CAN_ENCODE(BMS_Warning_OpenWireCheck_Segment2_GND_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_OpenWireCheck_Segment2_GND_raw, 3, 0x8);   // Packs bits ____#___ of byte 1 
    
    // Pack 1-bit signal BMS_Warning_OpenWireCheck_Segment3_GND into payload at [ bit 12 to bit 13 ).
    const bool BMS_Warning_OpenWireCheck_Segment3_GND_val = in_msg->BMS_Warning_OpenWireCheck_Segment3_GND_value;
    const uint32_t BMS_Warning_OpenWireCheck_Segment3_GND_raw = CAN_ENCODE(BMS_Warning_OpenWireCheck_Segment3_GND_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_OpenWireCheck_Segment3_GND_raw, 4, 0x10);   // Packs bits ___#____ of byte 1 
    
    // Pack 1-bit signal BMS_Warning_OpenWireCheck_Segment4_GND into payload at [ bit 13 to bit 14 ).
    const bool BMS_Warning_OpenWireCheck_Segment4_GND_val = in_msg->BMS_Warning_OpenWireCheck_Segment4_GND_value;
    const uint32_t BMS_Warning_OpenWireCheck_Segment4_GND_raw = CAN_ENCODE(BMS_Warning_OpenWireCheck_Segment4_GND_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_OpenWireCheck_Segment4_GND_raw, 5, 0x20);   // Packs bits __#_____ of byte 1 
    
    // Pack 1-bit signal BMS_Warning_MissingVCHeartbeat into payload at [ bit 14 to bit 15 ).
    const bool BMS_Warning_MissingVCHeartbeat_val = in_msg->BMS_Warning_MissingVCHeartbeat_value;
    const uint32_t BMS_Warning_MissingVCHeartbeat_raw = CAN_ENCODE(BMS_Warning_MissingVCHeartbeat_val, CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_SCALE, CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_MissingVCHeartbeat_raw, 6, 0x40);   // Packs bits _#______ of byte 1 
    
    
}
 
// pack function for BMS_Faults
void app_canUtils_BMS_Faults_pack(const BMS_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____KJI|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_Fault_StateMachine into payload at [ bit 0 to bit 1 ).
    const bool BMS_Fault_StateMachine_val = in_msg->BMS_Fault_StateMachine_value;
    const uint32_t BMS_Fault_StateMachine_raw = CAN_ENCODE(BMS_Fault_StateMachine_val, CANSIG_BMS_FAULT_STATE_MACHINE_SCALE, CANSIG_BMS_FAULT_STATE_MACHINE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Fault_StateMachine_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 1-bit signal BMS_Fault_CellUndervoltage into payload at [ bit 1 to bit 2 ).
    const bool BMS_Fault_CellUndervoltage_val = in_msg->BMS_Fault_CellUndervoltage_value;
    const uint32_t BMS_Fault_CellUndervoltage_raw = CAN_ENCODE(BMS_Fault_CellUndervoltage_val, CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_SCALE, CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_CellUndervoltage_raw, 1, 0x2);   // Packs bits ______#_ of byte 0 
    
    // Pack 1-bit signal BMS_Fault_CellOvervoltage into payload at [ bit 2 to bit 3 ).
    const bool BMS_Fault_CellOvervoltage_val = in_msg->BMS_Fault_CellOvervoltage_value;
    const uint32_t BMS_Fault_CellOvervoltage_raw = CAN_ENCODE(BMS_Fault_CellOvervoltage_val, CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_SCALE, CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_CellOvervoltage_raw, 2, 0x4);   // Packs bits _____#__ of byte 0 
    
    // Pack 1-bit signal BMS_Fault_ModuleCommunicationError into payload at [ bit 3 to bit 4 ).
    const bool BMS_Fault_ModuleCommunicationError_val = in_msg->BMS_Fault_ModuleCommunicationError_value;
    const uint32_t BMS_Fault_ModuleCommunicationError_raw = CAN_ENCODE(BMS_Fault_ModuleCommunicationError_val, CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_SCALE, CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_ModuleCommunicationError_raw, 3, 0x8);   // Packs bits ____#___ of byte 0 
    
    // Pack 1-bit signal BMS_Fault_CellUndertemp into payload at [ bit 4 to bit 5 ).
    const bool BMS_Fault_CellUndertemp_val = in_msg->BMS_Fault_CellUndertemp_value;
    const uint32_t BMS_Fault_CellUndertemp_raw = CAN_ENCODE(BMS_Fault_CellUndertemp_val, CANSIG_BMS_FAULT_CELL_UNDERTEMP_SCALE, CANSIG_BMS_FAULT_CELL_UNDERTEMP_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_CellUndertemp_raw, 4, 0x10);   // Packs bits ___#____ of byte 0 
    
    // Pack 1-bit signal BMS_Fault_CellOvertemp into payload at [ bit 5 to bit 6 ).
    const bool BMS_Fault_CellOvertemp_val = in_msg->BMS_Fault_CellOvertemp_value;
    const uint32_t BMS_Fault_CellOvertemp_raw = CAN_ENCODE(BMS_Fault_CellOvertemp_val, CANSIG_BMS_FAULT_CELL_OVERTEMP_SCALE, CANSIG_BMS_FAULT_CELL_OVERTEMP_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_CellOvertemp_raw, 5, 0x20);   // Packs bits __#_____ of byte 0 
    
    // Pack 1-bit signal BMS_Fault_ChargerReportedError into payload at [ bit 6 to bit 7 ).
    const bool BMS_Fault_ChargerReportedError_val = in_msg->BMS_Fault_ChargerReportedError_value;
    const uint32_t BMS_Fault_ChargerReportedError_raw = CAN_ENCODE(BMS_Fault_ChargerReportedError_val, CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_SCALE, CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_ChargerReportedError_raw, 6, 0x40);   // Packs bits _#______ of byte 0 
    
    // Pack 1-bit signal BMS_Fault_ChargerDisconnectedDuringCharge into payload at [ bit 7 to bit 8 ).
    const bool BMS_Fault_ChargerDisconnectedDuringCharge_val = in_msg->BMS_Fault_ChargerDisconnectedDuringCharge_value;
    const uint32_t BMS_Fault_ChargerDisconnectedDuringCharge_raw = CAN_ENCODE(BMS_Fault_ChargerDisconnectedDuringCharge_val, CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_SCALE, CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_ChargerDisconnectedDuringCharge_raw, 7, 0x80);   // Packs bits #_______ of byte 0 
    
    // Pack 1-bit signal BMS_Fault_ChargerShutdownLoopOpen into payload at [ bit 8 to bit 9 ).
    const bool BMS_Fault_ChargerShutdownLoopOpen_val = in_msg->BMS_Fault_ChargerShutdownLoopOpen_value;
    const uint32_t BMS_Fault_ChargerShutdownLoopOpen_raw = CAN_ENCODE(BMS_Fault_ChargerShutdownLoopOpen_val, CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_SCALE, CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(BMS_Fault_ChargerShutdownLoopOpen_raw, 0, 0x1);   // Packs bits _______# of byte 1 
    
    // Pack 1-bit signal BMS_Fault_TractiveSystemOvercurrent into payload at [ bit 9 to bit 10 ).
    const bool BMS_Fault_TractiveSystemOvercurrent_val = in_msg->BMS_Fault_TractiveSystemOvercurrent_value;
    const uint32_t BMS_Fault_TractiveSystemOvercurrent_raw = CAN_ENCODE(BMS_Fault_TractiveSystemOvercurrent_val, CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_SCALE, CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Fault_TractiveSystemOvercurrent_raw, 1, 0x2);   // Packs bits ______#_ of byte 1 
    
    // Pack 1-bit signal BMS_Fault_PrechargeFailure into payload at [ bit 10 to bit 11 ).
    const bool BMS_Fault_PrechargeFailure_val = in_msg->BMS_Fault_PrechargeFailure_value;
    const uint32_t BMS_Fault_PrechargeFailure_raw = CAN_ENCODE(BMS_Fault_PrechargeFailure_val, CANSIG_BMS_FAULT_PRECHARGE_FAILURE_SCALE, CANSIG_BMS_FAULT_PRECHARGE_FAILURE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Fault_PrechargeFailure_raw, 2, 0x4);   // Packs bits _____#__ of byte 1 
    
    
}
 
// pack function for BMS_WarningsCounts
void app_canUtils_BMS_WarningsCounts_pack(const BMS_WarningsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_WarningsCounts.
    // |xxxxxxxx|xxxxxxxx|___OOONN|NMMMLLLK|KKJJJIII|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal BMS_Warning_StackWaterMarkHighTask1HzCount into payload at [ bit 0 to bit 3 ).
    const uint32_t BMS_Warning_StackWaterMarkHighTask1HzCount_val = in_msg->BMS_Warning_StackWaterMarkHighTask1HzCount_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask1HzCount_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask1HzCount_val, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Warning_StackWaterMarkHighTask1HzCount_raw, 0, 0x7);   // Packs bits _____### of byte 0 
    
    // Pack 3-bit signal BMS_Warning_StackWaterMarkHighTask100HzCount into payload at [ bit 3 to bit 6 ).
    const uint32_t BMS_Warning_StackWaterMarkHighTask100HzCount_val = in_msg->BMS_Warning_StackWaterMarkHighTask100HzCount_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask100HzCount_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask100HzCount_val, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTask100HzCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0 
    
    // Pack 3-bit signal BMS_Warning_StackWaterMarkHighTask1kHzCount into payload at [ bit 6 to bit 9 ).
    const uint32_t BMS_Warning_StackWaterMarkHighTask1kHzCount_val = in_msg->BMS_Warning_StackWaterMarkHighTask1kHzCount_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask1kHzCount_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask1kHzCount_val, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTask1kHzCount_raw, 6, 0xC0);   // Packs bits ##______ of byte 0 
    out_data[1] |= packShiftRight(BMS_Warning_StackWaterMarkHighTask1kHzCount_raw, 2, 0x1);   // Packs bits _______# of byte 1 
    
    // Pack 3-bit signal BMS_Warning_StackWaterMarkHighTaskCanRxCount into payload at [ bit 9 to bit 12 ).
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanRxCount_val = in_msg->BMS_Warning_StackWaterMarkHighTaskCanRxCount_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanRxCount_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTaskCanRxCount_val, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTaskCanRxCount_raw, 1, 0xE);   // Packs bits ____###_ of byte 1 
    
    // Pack 3-bit signal BMS_Warning_StackWaterMarkHighTaskCanTxCount into payload at [ bit 12 to bit 15 ).
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanTxCount_val = in_msg->BMS_Warning_StackWaterMarkHighTaskCanTxCount_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanTxCount_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTaskCanTxCount_val, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTaskCanTxCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1 
    
    // Pack 3-bit signal BMS_Warning_WatchdogTimeoutCount into payload at [ bit 15 to bit 18 ).
    const uint32_t BMS_Warning_WatchdogTimeoutCount_val = in_msg->BMS_Warning_WatchdogTimeoutCount_value;
    const uint32_t BMS_Warning_WatchdogTimeoutCount_raw = CAN_ENCODE(BMS_Warning_WatchdogTimeoutCount_val, CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_SCALE, CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_WatchdogTimeoutCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1 
    out_data[2] |= packShiftRight(BMS_Warning_WatchdogTimeoutCount_raw, 1, 0x3);   // Packs bits ______## of byte 2 
    
    // Pack 3-bit signal BMS_Warning_TxOverflowCount into payload at [ bit 18 to bit 21 ).
    const uint32_t BMS_Warning_TxOverflowCount_val = in_msg->BMS_Warning_TxOverflowCount_value;
    const uint32_t BMS_Warning_TxOverflowCount_raw = CAN_ENCODE(BMS_Warning_TxOverflowCount_val, CANSIG_BMS_WARNING_TX_OVERFLOW_COUNT_SCALE, CANSIG_BMS_WARNING_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_Warning_TxOverflowCount_raw, 2, 0x1C);   // Packs bits ___###__ of byte 2 
    
    // Pack 3-bit signal BMS_Warning_RxOverflowCount into payload at [ bit 21 to bit 24 ).
    const uint32_t BMS_Warning_RxOverflowCount_val = in_msg->BMS_Warning_RxOverflowCount_value;
    const uint32_t BMS_Warning_RxOverflowCount_raw = CAN_ENCODE(BMS_Warning_RxOverflowCount_val, CANSIG_BMS_WARNING_RX_OVERFLOW_COUNT_SCALE, CANSIG_BMS_WARNING_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_Warning_RxOverflowCount_raw, 5, 0xE0);   // Packs bits ###_____ of byte 2 
    
    // Pack 3-bit signal BMS_Warning_OpenWireCheckFaultCount into payload at [ bit 24 to bit 27 ).
    const uint32_t BMS_Warning_OpenWireCheckFaultCount_val = in_msg->BMS_Warning_OpenWireCheckFaultCount_value;
    const uint32_t BMS_Warning_OpenWireCheckFaultCount_raw = CAN_ENCODE(BMS_Warning_OpenWireCheckFaultCount_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftRight(BMS_Warning_OpenWireCheckFaultCount_raw, 0, 0x7);   // Packs bits _____### of byte 3 
    
    // Pack 3-bit signal BMS_Warning_OpenWireCheck_Segment0_GNDCount into payload at [ bit 27 to bit 30 ).
    const uint32_t BMS_Warning_OpenWireCheck_Segment0_GNDCount_val = in_msg->BMS_Warning_OpenWireCheck_Segment0_GNDCount_value;
    const uint32_t BMS_Warning_OpenWireCheck_Segment0_GNDCount_raw = CAN_ENCODE(BMS_Warning_OpenWireCheck_Segment0_GNDCount_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_Warning_OpenWireCheck_Segment0_GNDCount_raw, 3, 0x38);   // Packs bits __###___ of byte 3 
    
    // Pack 3-bit signal BMS_Warning_OpenWireCheck_Segment1_GNDCount into payload at [ bit 30 to bit 33 ).
    const uint32_t BMS_Warning_OpenWireCheck_Segment1_GNDCount_val = in_msg->BMS_Warning_OpenWireCheck_Segment1_GNDCount_value;
    const uint32_t BMS_Warning_OpenWireCheck_Segment1_GNDCount_raw = CAN_ENCODE(BMS_Warning_OpenWireCheck_Segment1_GNDCount_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_Warning_OpenWireCheck_Segment1_GNDCount_raw, 6, 0xC0);   // Packs bits ##______ of byte 3 
    out_data[4] |= packShiftRight(BMS_Warning_OpenWireCheck_Segment1_GNDCount_raw, 2, 0x1);   // Packs bits _______# of byte 4 
    
    // Pack 3-bit signal BMS_Warning_OpenWireCheck_Segment2_GNDCount into payload at [ bit 33 to bit 36 ).
    const uint32_t BMS_Warning_OpenWireCheck_Segment2_GNDCount_val = in_msg->BMS_Warning_OpenWireCheck_Segment2_GNDCount_value;
    const uint32_t BMS_Warning_OpenWireCheck_Segment2_GNDCount_raw = CAN_ENCODE(BMS_Warning_OpenWireCheck_Segment2_GNDCount_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(BMS_Warning_OpenWireCheck_Segment2_GNDCount_raw, 1, 0xE);   // Packs bits ____###_ of byte 4 
    
    // Pack 3-bit signal BMS_Warning_OpenWireCheck_Segment3_GNDCount into payload at [ bit 36 to bit 39 ).
    const uint32_t BMS_Warning_OpenWireCheck_Segment3_GNDCount_val = in_msg->BMS_Warning_OpenWireCheck_Segment3_GNDCount_value;
    const uint32_t BMS_Warning_OpenWireCheck_Segment3_GNDCount_raw = CAN_ENCODE(BMS_Warning_OpenWireCheck_Segment3_GNDCount_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(BMS_Warning_OpenWireCheck_Segment3_GNDCount_raw, 4, 0x70);   // Packs bits _###____ of byte 4 
    
    // Pack 3-bit signal BMS_Warning_OpenWireCheck_Segment4_GNDCount into payload at [ bit 39 to bit 42 ).
    const uint32_t BMS_Warning_OpenWireCheck_Segment4_GNDCount_val = in_msg->BMS_Warning_OpenWireCheck_Segment4_GNDCount_value;
    const uint32_t BMS_Warning_OpenWireCheck_Segment4_GNDCount_raw = CAN_ENCODE(BMS_Warning_OpenWireCheck_Segment4_GNDCount_val, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(BMS_Warning_OpenWireCheck_Segment4_GNDCount_raw, 7, 0x80);   // Packs bits #_______ of byte 4 
    out_data[5] |= packShiftRight(BMS_Warning_OpenWireCheck_Segment4_GNDCount_raw, 1, 0x3);   // Packs bits ______## of byte 5 
    
    // Pack 3-bit signal BMS_Warning_MissingVCHeartbeatCount into payload at [ bit 42 to bit 45 ).
    const uint32_t BMS_Warning_MissingVCHeartbeatCount_val = in_msg->BMS_Warning_MissingVCHeartbeatCount_value;
    const uint32_t BMS_Warning_MissingVCHeartbeatCount_raw = CAN_ENCODE(BMS_Warning_MissingVCHeartbeatCount_val, CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_SCALE, CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(BMS_Warning_MissingVCHeartbeatCount_raw, 2, 0x1C);   // Packs bits ___###__ of byte 5 
    
    
}
 
// pack function for BMS_FaultsCounts
void app_canUtils_BMS_FaultsCounts_pack(const BMS_FaultsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message BMS_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_______K|KKJJJIII|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal BMS_Fault_StateMachineCount into payload at [ bit 0 to bit 3 ).
    const uint32_t BMS_Fault_StateMachineCount_val = in_msg->BMS_Fault_StateMachineCount_value;
    const uint32_t BMS_Fault_StateMachineCount_raw = CAN_ENCODE(BMS_Fault_StateMachineCount_val, CANSIG_BMS_FAULT_STATE_MACHINE_COUNT_SCALE, CANSIG_BMS_FAULT_STATE_MACHINE_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Fault_StateMachineCount_raw, 0, 0x7);   // Packs bits _____### of byte 0 
    
    // Pack 3-bit signal BMS_Fault_CellUndervoltageCount into payload at [ bit 3 to bit 6 ).
    const uint32_t BMS_Fault_CellUndervoltageCount_val = in_msg->BMS_Fault_CellUndervoltageCount_value;
    const uint32_t BMS_Fault_CellUndervoltageCount_raw = CAN_ENCODE(BMS_Fault_CellUndervoltageCount_val, CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_SCALE, CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_CellUndervoltageCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0 
    
    // Pack 3-bit signal BMS_Fault_CellOvervoltageCount into payload at [ bit 6 to bit 9 ).
    const uint32_t BMS_Fault_CellOvervoltageCount_val = in_msg->BMS_Fault_CellOvervoltageCount_value;
    const uint32_t BMS_Fault_CellOvervoltageCount_raw = CAN_ENCODE(BMS_Fault_CellOvervoltageCount_val, CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_SCALE, CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_CellOvervoltageCount_raw, 6, 0xC0);   // Packs bits ##______ of byte 0 
    out_data[1] |= packShiftRight(BMS_Fault_CellOvervoltageCount_raw, 2, 0x1);   // Packs bits _______# of byte 1 
    
    // Pack 3-bit signal BMS_Fault_ModuleCommunicationErrorCount into payload at [ bit 9 to bit 12 ).
    const uint32_t BMS_Fault_ModuleCommunicationErrorCount_val = in_msg->BMS_Fault_ModuleCommunicationErrorCount_value;
    const uint32_t BMS_Fault_ModuleCommunicationErrorCount_raw = CAN_ENCODE(BMS_Fault_ModuleCommunicationErrorCount_val, CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_SCALE, CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Fault_ModuleCommunicationErrorCount_raw, 1, 0xE);   // Packs bits ____###_ of byte 1 
    
    // Pack 3-bit signal BMS_Fault_CellUndertempCount into payload at [ bit 12 to bit 15 ).
    const uint32_t BMS_Fault_CellUndertempCount_val = in_msg->BMS_Fault_CellUndertempCount_value;
    const uint32_t BMS_Fault_CellUndertempCount_raw = CAN_ENCODE(BMS_Fault_CellUndertempCount_val, CANSIG_BMS_FAULT_CELL_UNDERTEMP_COUNT_SCALE, CANSIG_BMS_FAULT_CELL_UNDERTEMP_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Fault_CellUndertempCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1 
    
    // Pack 3-bit signal BMS_Fault_CellOvertempCount into payload at [ bit 15 to bit 18 ).
    const uint32_t BMS_Fault_CellOvertempCount_val = in_msg->BMS_Fault_CellOvertempCount_value;
    const uint32_t BMS_Fault_CellOvertempCount_raw = CAN_ENCODE(BMS_Fault_CellOvertempCount_val, CANSIG_BMS_FAULT_CELL_OVERTEMP_COUNT_SCALE, CANSIG_BMS_FAULT_CELL_OVERTEMP_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Fault_CellOvertempCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1 
    out_data[2] |= packShiftRight(BMS_Fault_CellOvertempCount_raw, 1, 0x3);   // Packs bits ______## of byte 2 
    
    // Pack 3-bit signal BMS_Fault_ChargerReportedErrorCount into payload at [ bit 18 to bit 21 ).
    const uint32_t BMS_Fault_ChargerReportedErrorCount_val = in_msg->BMS_Fault_ChargerReportedErrorCount_value;
    const uint32_t BMS_Fault_ChargerReportedErrorCount_raw = CAN_ENCODE(BMS_Fault_ChargerReportedErrorCount_val, CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_SCALE, CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_Fault_ChargerReportedErrorCount_raw, 2, 0x1C);   // Packs bits ___###__ of byte 2 
    
    // Pack 3-bit signal BMS_Fault_ChargerDisconnectedDuringChargeCount into payload at [ bit 21 to bit 24 ).
    const uint32_t BMS_Fault_ChargerDisconnectedDuringChargeCount_val = in_msg->BMS_Fault_ChargerDisconnectedDuringChargeCount_value;
    const uint32_t BMS_Fault_ChargerDisconnectedDuringChargeCount_raw = CAN_ENCODE(BMS_Fault_ChargerDisconnectedDuringChargeCount_val, CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_SCALE, CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_Fault_ChargerDisconnectedDuringChargeCount_raw, 5, 0xE0);   // Packs bits ###_____ of byte 2 
    
    // Pack 3-bit signal BMS_Fault_ChargerShutdownLoopOpenCount into payload at [ bit 24 to bit 27 ).
    const uint32_t BMS_Fault_ChargerShutdownLoopOpenCount_val = in_msg->BMS_Fault_ChargerShutdownLoopOpenCount_value;
    const uint32_t BMS_Fault_ChargerShutdownLoopOpenCount_raw = CAN_ENCODE(BMS_Fault_ChargerShutdownLoopOpenCount_val, CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_SCALE, CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftRight(BMS_Fault_ChargerShutdownLoopOpenCount_raw, 0, 0x7);   // Packs bits _____### of byte 3 
    
    // Pack 3-bit signal BMS_Fault_TractiveSystemOvercurrentCount into payload at [ bit 27 to bit 30 ).
    const uint32_t BMS_Fault_TractiveSystemOvercurrentCount_val = in_msg->BMS_Fault_TractiveSystemOvercurrentCount_value;
    const uint32_t BMS_Fault_TractiveSystemOvercurrentCount_raw = CAN_ENCODE(BMS_Fault_TractiveSystemOvercurrentCount_val, CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_SCALE, CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_Fault_TractiveSystemOvercurrentCount_raw, 3, 0x38);   // Packs bits __###___ of byte 3 
    
    // Pack 3-bit signal BMS_Fault_PrechargeFailureCount into payload at [ bit 30 to bit 33 ).
    const uint32_t BMS_Fault_PrechargeFailureCount_val = in_msg->BMS_Fault_PrechargeFailureCount_value;
    const uint32_t BMS_Fault_PrechargeFailureCount_raw = CAN_ENCODE(BMS_Fault_PrechargeFailureCount_val, CANSIG_BMS_FAULT_PRECHARGE_FAILURE_COUNT_SCALE, CANSIG_BMS_FAULT_PRECHARGE_FAILURE_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_Fault_PrechargeFailureCount_raw, 6, 0xC0);   // Packs bits ##______ of byte 3 
    out_data[4] |= packShiftRight(BMS_Fault_PrechargeFailureCount_raw, 2, 0x1);   // Packs bits _______# of byte 4 
    
    
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


// unpack function for VC_Warnings
void app_canUtils_VC_Warnings_unpack(const uint8_t* const in_data, VC_Warnings_Signals* const out_msg)
{
    // Unpack message VC_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____TSRQ|PONMLKJI|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t VC_Warning_StackWaterMarkHighTask1Hz_raw = 0; // Temporary raw variable for VC_Warning_StackWaterMarkHighTask1Hz
    VC_Warning_StackWaterMarkHighTask1Hz_raw |= unpackShiftRight(in_data[0], 0, 0x1); // Unpacks bits _______# from byte 0
    out_msg->VC_Warning_StackWaterMarkHighTask1Hz_value = CAN_DECODE(VC_Warning_StackWaterMarkHighTask1Hz_raw, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    
    uint32_t VC_Warning_StackWaterMarkHighTask100Hz_raw = 0; // Temporary raw variable for VC_Warning_StackWaterMarkHighTask100Hz
    VC_Warning_StackWaterMarkHighTask100Hz_raw |= unpackShiftLeft(in_data[0], 1, 0x2); // Unpacks bits ______#_ from byte 0
    out_msg->VC_Warning_StackWaterMarkHighTask100Hz_value = CAN_DECODE(VC_Warning_StackWaterMarkHighTask100Hz_raw, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    
    uint32_t VC_Warning_StackWaterMarkHighTask1kHz_raw = 0; // Temporary raw variable for VC_Warning_StackWaterMarkHighTask1kHz
    VC_Warning_StackWaterMarkHighTask1kHz_raw |= unpackShiftLeft(in_data[0], 2, 0x4); // Unpacks bits _____#__ from byte 0
    out_msg->VC_Warning_StackWaterMarkHighTask1kHz_value = CAN_DECODE(VC_Warning_StackWaterMarkHighTask1kHz_raw, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    
    uint32_t VC_Warning_StackWaterMarkHighTaskCanRx_raw = 0; // Temporary raw variable for VC_Warning_StackWaterMarkHighTaskCanRx
    VC_Warning_StackWaterMarkHighTaskCanRx_raw |= unpackShiftLeft(in_data[0], 3, 0x8); // Unpacks bits ____#___ from byte 0
    out_msg->VC_Warning_StackWaterMarkHighTaskCanRx_value = CAN_DECODE(VC_Warning_StackWaterMarkHighTaskCanRx_raw, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    
    uint32_t VC_Warning_StackWaterMarkHighTaskCanTx_raw = 0; // Temporary raw variable for VC_Warning_StackWaterMarkHighTaskCanTx
    VC_Warning_StackWaterMarkHighTaskCanTx_raw |= unpackShiftLeft(in_data[0], 4, 0x10); // Unpacks bits ___#____ from byte 0
    out_msg->VC_Warning_StackWaterMarkHighTaskCanTx_value = CAN_DECODE(VC_Warning_StackWaterMarkHighTaskCanTx_raw, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    
    uint32_t VC_Warning_WatchdogTimeout_raw = 0; // Temporary raw variable for VC_Warning_WatchdogTimeout
    VC_Warning_WatchdogTimeout_raw |= unpackShiftLeft(in_data[0], 5, 0x20); // Unpacks bits __#_____ from byte 0
    out_msg->VC_Warning_WatchdogTimeout_value = CAN_DECODE(VC_Warning_WatchdogTimeout_raw, CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    
    uint32_t VC_Warning_TxOverflow_raw = 0; // Temporary raw variable for VC_Warning_TxOverflow
    VC_Warning_TxOverflow_raw |= unpackShiftLeft(in_data[0], 6, 0x40); // Unpacks bits _#______ from byte 0
    out_msg->VC_Warning_TxOverflow_value = CAN_DECODE(VC_Warning_TxOverflow_raw, CANSIG_VC_WARNING_TX_OVERFLOW_SCALE, CANSIG_VC_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    
    uint32_t VC_Warning_RxOverflow_raw = 0; // Temporary raw variable for VC_Warning_RxOverflow
    VC_Warning_RxOverflow_raw |= unpackShiftLeft(in_data[0], 7, 0x80); // Unpacks bits #_______ from byte 0
    out_msg->VC_Warning_RxOverflow_value = CAN_DECODE(VC_Warning_RxOverflow_raw, CANSIG_VC_WARNING_RX_OVERFLOW_SCALE, CANSIG_VC_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    
    uint32_t VC_Warning_RegenNotAvailable_raw = 0; // Temporary raw variable for VC_Warning_RegenNotAvailable
    VC_Warning_RegenNotAvailable_raw |= unpackShiftRight(in_data[1], 0, 0x1); // Unpacks bits _______# from byte 1
    out_msg->VC_Warning_RegenNotAvailable_value = CAN_DECODE(VC_Warning_RegenNotAvailable_raw, CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_SCALE, CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_OFFSET, uint32_t);
    
    uint32_t VC_Warning_ImuInitFailed_raw = 0; // Temporary raw variable for VC_Warning_ImuInitFailed
    VC_Warning_ImuInitFailed_raw |= unpackShiftLeft(in_data[1], 1, 0x2); // Unpacks bits ______#_ from byte 1
    out_msg->VC_Warning_ImuInitFailed_value = CAN_DECODE(VC_Warning_ImuInitFailed_raw, CANSIG_VC_WARNING_IMU_INIT_FAILED_SCALE, CANSIG_VC_WARNING_IMU_INIT_FAILED_OFFSET, uint32_t);
    
    uint32_t VC_Warning_FlowRateInputOutOfRange_raw = 0; // Temporary raw variable for VC_Warning_FlowRateInputOutOfRange
    VC_Warning_FlowRateInputOutOfRange_raw |= unpackShiftLeft(in_data[1], 2, 0x4); // Unpacks bits _____#__ from byte 1
    out_msg->VC_Warning_FlowRateInputOutOfRange_value = CAN_DECODE(VC_Warning_FlowRateInputOutOfRange_raw, CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_SCALE, CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_OFFSET, uint32_t);
    
    uint32_t VC_Warning_BatteryRailOvercurrentFault_raw = 0; // Temporary raw variable for VC_Warning_BatteryRailOvercurrentFault
    VC_Warning_BatteryRailOvercurrentFault_raw |= unpackShiftLeft(in_data[1], 3, 0x8); // Unpacks bits ____#___ from byte 1
    out_msg->VC_Warning_BatteryRailOvercurrentFault_value = CAN_DECODE(VC_Warning_BatteryRailOvercurrentFault_raw, CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_SCALE, CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_OFFSET, uint32_t);
    
    uint32_t VC_Warning_AccumulatorRailOvercurrentFault_raw = 0; // Temporary raw variable for VC_Warning_AccumulatorRailOvercurrentFault
    VC_Warning_AccumulatorRailOvercurrentFault_raw |= unpackShiftLeft(in_data[1], 4, 0x10); // Unpacks bits ___#____ from byte 1
    out_msg->VC_Warning_AccumulatorRailOvercurrentFault_value = CAN_DECODE(VC_Warning_AccumulatorRailOvercurrentFault_raw, CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_SCALE, CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_OFFSET, uint32_t);
    
    uint32_t VC_Warning_LvChargeFault_raw = 0; // Temporary raw variable for VC_Warning_LvChargeFault
    VC_Warning_LvChargeFault_raw |= unpackShiftLeft(in_data[1], 5, 0x20); // Unpacks bits __#_____ from byte 1
    out_msg->VC_Warning_LvChargeFault_value = CAN_DECODE(VC_Warning_LvChargeFault_raw, CANSIG_VC_WARNING_LV_CHARGE_FAULT_SCALE, CANSIG_VC_WARNING_LV_CHARGE_FAULT_OFFSET, uint32_t);
    
    uint32_t VC_Warning_BoostControllerFault_raw = 0; // Temporary raw variable for VC_Warning_BoostControllerFault
    VC_Warning_BoostControllerFault_raw |= unpackShiftLeft(in_data[1], 6, 0x40); // Unpacks bits _#______ from byte 1
    out_msg->VC_Warning_BoostControllerFault_value = CAN_DECODE(VC_Warning_BoostControllerFault_raw, CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_SCALE, CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_OFFSET, uint32_t);
    
    uint32_t VC_Warning_HighNumberOfCanDataLogs_raw = 0; // Temporary raw variable for VC_Warning_HighNumberOfCanDataLogs
    VC_Warning_HighNumberOfCanDataLogs_raw |= unpackShiftLeft(in_data[1], 7, 0x80); // Unpacks bits #_______ from byte 1
    out_msg->VC_Warning_HighNumberOfCanDataLogs_value = CAN_DECODE(VC_Warning_HighNumberOfCanDataLogs_raw, CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_SCALE, CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_OFFSET, uint32_t);
    
    uint32_t VC_Warning_CanLoggingSdCardNotPresent_raw = 0; // Temporary raw variable for VC_Warning_CanLoggingSdCardNotPresent
    VC_Warning_CanLoggingSdCardNotPresent_raw |= unpackShiftRight(in_data[2], 0, 0x1); // Unpacks bits _______# from byte 2
    out_msg->VC_Warning_CanLoggingSdCardNotPresent_value = CAN_DECODE(VC_Warning_CanLoggingSdCardNotPresent_raw, CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_SCALE, CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_OFFSET, uint32_t);
    
    uint32_t VC_Warning_CanLoggingErrored_raw = 0; // Temporary raw variable for VC_Warning_CanLoggingErrored
    VC_Warning_CanLoggingErrored_raw |= unpackShiftLeft(in_data[2], 1, 0x2); // Unpacks bits ______#_ from byte 2
    out_msg->VC_Warning_CanLoggingErrored_value = CAN_DECODE(VC_Warning_CanLoggingErrored_raw, CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_SCALE, CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_OFFSET, uint32_t);
    
    uint32_t VC_Warning_BrakeAppsDisagreement_raw = 0; // Temporary raw variable for VC_Warning_BrakeAppsDisagreement
    VC_Warning_BrakeAppsDisagreement_raw |= unpackShiftLeft(in_data[2], 2, 0x4); // Unpacks bits _____#__ from byte 2
    out_msg->VC_Warning_BrakeAppsDisagreement_value = CAN_DECODE(VC_Warning_BrakeAppsDisagreement_raw, CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_SCALE, CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_OFFSET, uint32_t);
    
    uint32_t VC_Warning_SbgInitFailed_raw = 0; // Temporary raw variable for VC_Warning_SbgInitFailed
    VC_Warning_SbgInitFailed_raw |= unpackShiftLeft(in_data[2], 3, 0x8); // Unpacks bits ____#___ from byte 2
    out_msg->VC_Warning_SbgInitFailed_value = CAN_DECODE(VC_Warning_SbgInitFailed_raw, CANSIG_VC_WARNING_SBG_INIT_FAILED_SCALE, CANSIG_VC_WARNING_SBG_INIT_FAILED_OFFSET, uint32_t);
    
    
}


// unpack function for VC_Faults
void app_canUtils_VC_Faults_unpack(const uint8_t* const in_data, VC_Faults_Signals* const out_msg)
{
    // Unpack message VC_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__FEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t VC_Fault_MissingBMSHeartbeat_raw = 0; // Temporary raw variable for VC_Fault_MissingBMSHeartbeat
    VC_Fault_MissingBMSHeartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1); // Unpacks bits _______# from byte 0
    out_msg->VC_Fault_MissingBMSHeartbeat_value = CAN_DECODE(VC_Fault_MissingBMSHeartbeat_raw, CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_SCALE, CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_OFFSET, uint32_t);
    
    uint32_t VC_Fault_MissingFSMHeartbeat_raw = 0; // Temporary raw variable for VC_Fault_MissingFSMHeartbeat
    VC_Fault_MissingFSMHeartbeat_raw |= unpackShiftLeft(in_data[0], 1, 0x2); // Unpacks bits ______#_ from byte 0
    out_msg->VC_Fault_MissingFSMHeartbeat_value = CAN_DECODE(VC_Fault_MissingFSMHeartbeat_raw, CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_SCALE, CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_OFFSET, uint32_t);
    
    uint32_t VC_Fault_MissingRSMHeartbeat_raw = 0; // Temporary raw variable for VC_Fault_MissingRSMHeartbeat
    VC_Fault_MissingRSMHeartbeat_raw |= unpackShiftLeft(in_data[0], 2, 0x4); // Unpacks bits _____#__ from byte 0
    out_msg->VC_Fault_MissingRSMHeartbeat_value = CAN_DECODE(VC_Fault_MissingRSMHeartbeat_raw, CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_SCALE, CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_OFFSET, uint32_t);
    
    uint32_t VC_Fault_MissingCRITHeartbeat_raw = 0; // Temporary raw variable for VC_Fault_MissingCRITHeartbeat
    VC_Fault_MissingCRITHeartbeat_raw |= unpackShiftLeft(in_data[0], 3, 0x8); // Unpacks bits ____#___ from byte 0
    out_msg->VC_Fault_MissingCRITHeartbeat_value = CAN_DECODE(VC_Fault_MissingCRITHeartbeat_raw, CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_SCALE, CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_OFFSET, uint32_t);
    
    uint32_t VC_Fault_LeftInverterFault_raw = 0; // Temporary raw variable for VC_Fault_LeftInverterFault
    VC_Fault_LeftInverterFault_raw |= unpackShiftLeft(in_data[0], 4, 0x10); // Unpacks bits ___#____ from byte 0
    out_msg->VC_Fault_LeftInverterFault_value = CAN_DECODE(VC_Fault_LeftInverterFault_raw, CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_SCALE, CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_OFFSET, uint32_t);
    
    uint32_t VC_Fault_RightInverterFault_raw = 0; // Temporary raw variable for VC_Fault_RightInverterFault
    VC_Fault_RightInverterFault_raw |= unpackShiftLeft(in_data[0], 5, 0x20); // Unpacks bits __#_____ from byte 0
    out_msg->VC_Fault_RightInverterFault_value = CAN_DECODE(VC_Fault_RightInverterFault_raw, CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_SCALE, CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_OFFSET, uint32_t);
    
    
}
