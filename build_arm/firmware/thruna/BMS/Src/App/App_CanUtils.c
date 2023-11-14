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

void App_CanUtils_BMS_Vitals_Pack(const BMS_Vitals_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message BMS_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____BBBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_Heartbeat into payload (at bit 0 to bit 1).
    const bool BMS_Heartbeat_val = in_msg->BMS_Heartbeat_value;
    const uint32_t BMS_Heartbeat_raw = CAN_ENCODE(BMS_Heartbeat_val, CANSIG_BMS_VITALS_BMS_HEARTBEAT_SCALE, CANSIG_BMS_VITALS_BMS_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 3-bit signal BMS_State into payload (at bit 1 to bit 4).
    const BmsState BMS_State_val = in_msg->BMS_State_value;
    const uint32_t BMS_State_raw = CAN_ENCODE(BMS_State_val, CANSIG_BMS_VITALS_BMS_STATE_SCALE, CANSIG_BMS_VITALS_BMS_STATE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_State_raw, 1, 0xe);   // Packs bits ____###_ of byte 0
    
}

void App_CanUtils_BMS_AlertsContext_Pack(const BMS_AlertsContext_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message BMS_AlertsContext.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CCC|BBBBBBBB|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal BMS_TxOverflowCount into payload (at bit 0 to bit 8).
    const uint32_t BMS_TxOverflowCount_val = in_msg->BMS_TxOverflowCount_value;
    const uint32_t BMS_TxOverflowCount_raw = CAN_ENCODE(BMS_TxOverflowCount_val, CANSIG_BMS_ALERTS_CONTEXT_BMS_TX_OVERFLOW_COUNT_SCALE, CANSIG_BMS_ALERTS_CONTEXT_BMS_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_TxOverflowCount_raw, 0, 0xff);   // Packs bits ######## of byte 0
    
    // Pack 8-bit signal BMS_RxOverflowCount into payload (at bit 8 to bit 16).
    const uint32_t BMS_RxOverflowCount_val = in_msg->BMS_RxOverflowCount_value;
    const uint32_t BMS_RxOverflowCount_raw = CAN_ENCODE(BMS_RxOverflowCount_val, CANSIG_BMS_ALERTS_CONTEXT_BMS_RX_OVERFLOW_COUNT_SCALE, CANSIG_BMS_ALERTS_CONTEXT_BMS_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(BMS_RxOverflowCount_raw, 0, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 3-bit signal BMS_WatchdogTimeoutTaskName into payload (at bit 16 to bit 19).
    const RtosTaskName BMS_WatchdogTimeoutTaskName_val = in_msg->BMS_WatchdogTimeoutTaskName_value;
    const uint32_t BMS_WatchdogTimeoutTaskName_raw = CAN_ENCODE(BMS_WatchdogTimeoutTaskName_val, CANSIG_BMS_ALERTS_CONTEXT_BMS_WATCHDOG_TIMEOUT_TASK_NAME_SCALE, CANSIG_BMS_ALERTS_CONTEXT_BMS_WATCHDOG_TIMEOUT_TASK_NAME_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(BMS_WatchdogTimeoutTaskName_raw, 0, 0x7);   // Packs bits _____### of byte 2
    
}

void App_CanUtils_BMS_ImdStatus_Pack(const BMS_ImdStatus_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 2-byte payload for message BMS_ImdStatus.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__DDDDCB|BBBBBBBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_ImdOkHs into payload (at bit 0 to bit 1).
    const bool BMS_ImdOkHs_val = in_msg->BMS_ImdOkHs_value;
    const uint32_t BMS_ImdOkHs_raw = CAN_ENCODE(BMS_ImdOkHs_val, CANSIG_BMS_IMD_STATUS_BMS_IMD_OK_HS_SCALE, CANSIG_BMS_IMD_STATUS_BMS_IMD_OK_HS_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_ImdOkHs_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 8-bit signal BMS_ImdSecondsSincePowerOn into payload (at bit 1 to bit 9).
    const uint32_t BMS_ImdSecondsSincePowerOn_val = in_msg->BMS_ImdSecondsSincePowerOn_value;
    const uint32_t BMS_ImdSecondsSincePowerOn_raw = CAN_ENCODE(BMS_ImdSecondsSincePowerOn_val, CANSIG_BMS_IMD_STATUS_BMS_IMD_SECONDS_SINCE_POWER_ON_SCALE, CANSIG_BMS_IMD_STATUS_BMS_IMD_SECONDS_SINCE_POWER_ON_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ImdSecondsSincePowerOn_raw, 1, 0xfe);   // Packs bits #######_ of byte 0
    out_data[1] |= packShiftRight(BMS_ImdSecondsSincePowerOn_raw, 7, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 1-bit signal BMS_ImdValidDutyCycle into payload (at bit 9 to bit 10).
    const bool BMS_ImdValidDutyCycle_val = in_msg->BMS_ImdValidDutyCycle_value;
    const uint32_t BMS_ImdValidDutyCycle_raw = CAN_ENCODE(BMS_ImdValidDutyCycle_val, CANSIG_BMS_IMD_STATUS_BMS_IMD_VALID_DUTY_CYCLE_SCALE, CANSIG_BMS_IMD_STATUS_BMS_IMD_VALID_DUTY_CYCLE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_ImdValidDutyCycle_raw, 1, 0x2);   // Packs bits ______#_ of byte 1
    
    // Pack 4-bit signal BMS_ImdCondition into payload (at bit 10 to bit 14).
    const uint32_t BMS_ImdCondition_val = in_msg->BMS_ImdCondition_value;
    const uint32_t BMS_ImdCondition_raw = CAN_ENCODE(BMS_ImdCondition_val, CANSIG_BMS_IMD_STATUS_BMS_IMD_CONDITION_SCALE, CANSIG_BMS_IMD_STATUS_BMS_IMD_CONDITION_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_ImdCondition_raw, 2, 0x3c);   // Packs bits __####__ of byte 1
    
}

void App_CanUtils_BMS_ImdData_Pack(const BMS_ImdData_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 8-byte payload for message BMS_ImdData.
    // |___DDDDD|DDDDDDDD|DDDDDDDC|CCCCCCCC|CCCCCCCC|CCCBBBBB|BBBBBBBB|BBBBBBBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_ImdActiveFrequency into payload (at bit 0 to bit 1).
    const ImdActiveFrequency BMS_ImdActiveFrequency_val = in_msg->BMS_ImdActiveFrequency_value;
    const uint32_t BMS_ImdActiveFrequency_raw = CAN_ENCODE(BMS_ImdActiveFrequency_val, CANSIG_BMS_IMD_DATA_BMS_IMD_ACTIVE_FREQUENCY_SCALE, CANSIG_BMS_IMD_DATA_BMS_IMD_ACTIVE_FREQUENCY_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_ImdActiveFrequency_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 20-bit signal BMS_ImdInsulationMeasurementDcp10Hz into payload (at bit 1 to bit 21).
    const float BMS_ImdInsulationMeasurementDcp10Hz_val = in_msg->BMS_ImdInsulationMeasurementDcp10Hz_value;
    const uint32_t BMS_ImdInsulationMeasurementDcp10Hz_raw = CAN_ENCODE(BMS_ImdInsulationMeasurementDcp10Hz_val, CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_SCALE, CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP10_HZ_OFFSET, float);
    out_data[0] |= packShiftLeft(BMS_ImdInsulationMeasurementDcp10Hz_raw, 1, 0xfe);   // Packs bits #######_ of byte 0
    out_data[1] |= packShiftRight(BMS_ImdInsulationMeasurementDcp10Hz_raw, 7, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(BMS_ImdInsulationMeasurementDcp10Hz_raw, 15, 0x1f);   // Packs bits ___##### of byte 2
    
    // Pack 20-bit signal BMS_ImdInsulationMeasurementDcp20Hz into payload (at bit 21 to bit 41).
    const float BMS_ImdInsulationMeasurementDcp20Hz_val = in_msg->BMS_ImdInsulationMeasurementDcp20Hz_value;
    const uint32_t BMS_ImdInsulationMeasurementDcp20Hz_raw = CAN_ENCODE(BMS_ImdInsulationMeasurementDcp20Hz_val, CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_SCALE, CANSIG_BMS_IMD_DATA_BMS_IMD_INSULATION_MEASUREMENT_DCP20_HZ_OFFSET, float);
    out_data[2] |= packShiftLeft(BMS_ImdInsulationMeasurementDcp20Hz_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    out_data[3] |= packShiftRight(BMS_ImdInsulationMeasurementDcp20Hz_raw, 3, 0xff);   // Packs bits ######## of byte 3
    out_data[4] |= packShiftRight(BMS_ImdInsulationMeasurementDcp20Hz_raw, 11, 0xff);   // Packs bits ######## of byte 4
    out_data[5] |= packShiftRight(BMS_ImdInsulationMeasurementDcp20Hz_raw, 19, 0x1);   // Packs bits _______# of byte 5
    
    // Pack 20-bit signal BMS_ImdSpeedStartStatus30Hz into payload (at bit 41 to bit 61).
    const float BMS_ImdSpeedStartStatus30Hz_val = in_msg->BMS_ImdSpeedStartStatus30Hz_value;
    const uint32_t BMS_ImdSpeedStartStatus30Hz_raw = CAN_ENCODE(BMS_ImdSpeedStartStatus30Hz_val, CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_SCALE, CANSIG_BMS_IMD_DATA_BMS_IMD_SPEED_START_STATUS30_HZ_OFFSET, float);
    out_data[5] |= packShiftLeft(BMS_ImdSpeedStartStatus30Hz_raw, 1, 0xfe);   // Packs bits #######_ of byte 5
    out_data[6] |= packShiftRight(BMS_ImdSpeedStartStatus30Hz_raw, 7, 0xff);   // Packs bits ######## of byte 6
    out_data[7] |= packShiftRight(BMS_ImdSpeedStartStatus30Hz_raw, 15, 0x1f);   // Packs bits ___##### of byte 7
    
}

void App_CanUtils_BMS_ImdPwmOutput_Pack(const BMS_ImdPwmOutput_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 2-byte payload for message BMS_ImdPwmOutput.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|BBBBBBBB|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal BMS_ImdDutyCycle into payload (at bit 0 to bit 8).
    const float BMS_ImdDutyCycle_val = in_msg->BMS_ImdDutyCycle_value;
    const uint32_t BMS_ImdDutyCycle_raw = CAN_ENCODE(BMS_ImdDutyCycle_val, CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_DUTY_CYCLE_SCALE, CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_DUTY_CYCLE_OFFSET, float);
    out_data[0] |= packShiftRight(BMS_ImdDutyCycle_raw, 0, 0xff);   // Packs bits ######## of byte 0
    
    // Pack 8-bit signal BMS_ImdFrequency into payload (at bit 8 to bit 16).
    const float BMS_ImdFrequency_val = in_msg->BMS_ImdFrequency_value;
    const uint32_t BMS_ImdFrequency_raw = CAN_ENCODE(BMS_ImdFrequency_val, CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_FREQUENCY_SCALE, CANSIG_BMS_IMD_PWM_OUTPUT_BMS_IMD_FREQUENCY_OFFSET, float);
    out_data[1] |= packShiftRight(BMS_ImdFrequency_raw, 0, 0xff);   // Packs bits ######## of byte 1
    
}

void App_CanUtils_BMS_Charger_Pack(const BMS_Charger_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message BMS_Charger.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_ChargerConnected into payload (at bit 0 to bit 1).
    const bool BMS_ChargerConnected_val = in_msg->BMS_ChargerConnected_value;
    const uint32_t BMS_ChargerConnected_raw = CAN_ENCODE(BMS_ChargerConnected_val, CANSIG_BMS_CHARGER_BMS_CHARGER_CONNECTED_SCALE, CANSIG_BMS_CHARGER_BMS_CHARGER_CONNECTED_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_ChargerConnected_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal BMS_ChargingComplete into payload (at bit 1 to bit 2).
    const bool BMS_ChargingComplete_val = in_msg->BMS_ChargingComplete_value;
    const uint32_t BMS_ChargingComplete_raw = CAN_ENCODE(BMS_ChargingComplete_val, CANSIG_BMS_CHARGER_BMS_CHARGING_COMPLETE_SCALE, CANSIG_BMS_CHARGER_BMS_CHARGING_COMPLETE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ChargingComplete_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
}

void App_CanUtils_BMS_OkStatuses_Pack(const BMS_OkStatuses_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message BMS_OkStatuses.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__FEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_BmsOk into payload (at bit 0 to bit 1).
    const bool BMS_BmsOk_val = in_msg->BMS_BmsOk_value;
    const uint32_t BMS_BmsOk_raw = CAN_ENCODE(BMS_BmsOk_val, CANSIG_BMS_OK_STATUSES_BMS_BMS_OK_SCALE, CANSIG_BMS_OK_STATUSES_BMS_BMS_OK_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_BmsOk_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal BMS_ImdOk into payload (at bit 1 to bit 2).
    const bool BMS_ImdOk_val = in_msg->BMS_ImdOk_value;
    const uint32_t BMS_ImdOk_raw = CAN_ENCODE(BMS_ImdOk_val, CANSIG_BMS_OK_STATUSES_BMS_IMD_OK_SCALE, CANSIG_BMS_OK_STATUSES_BMS_IMD_OK_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ImdOk_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal BMS_BspdOk into payload (at bit 2 to bit 3).
    const bool BMS_BspdOk_val = in_msg->BMS_BspdOk_value;
    const uint32_t BMS_BspdOk_raw = CAN_ENCODE(BMS_BspdOk_val, CANSIG_BMS_OK_STATUSES_BMS_BSPD_OK_SCALE, CANSIG_BMS_OK_STATUSES_BMS_BSPD_OK_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_BspdOk_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal BMS_BmsLatchedFault into payload (at bit 3 to bit 4).
    const bool BMS_BmsLatchedFault_val = in_msg->BMS_BmsLatchedFault_value;
    const uint32_t BMS_BmsLatchedFault_raw = CAN_ENCODE(BMS_BmsLatchedFault_val, CANSIG_BMS_OK_STATUSES_BMS_BMS_LATCHED_FAULT_SCALE, CANSIG_BMS_OK_STATUSES_BMS_BMS_LATCHED_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_BmsLatchedFault_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal BMS_ImdLatchedFault into payload (at bit 4 to bit 5).
    const bool BMS_ImdLatchedFault_val = in_msg->BMS_ImdLatchedFault_value;
    const uint32_t BMS_ImdLatchedFault_raw = CAN_ENCODE(BMS_ImdLatchedFault_val, CANSIG_BMS_OK_STATUSES_BMS_IMD_LATCHED_FAULT_SCALE, CANSIG_BMS_OK_STATUSES_BMS_IMD_LATCHED_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ImdLatchedFault_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal BMS_BspdLatchedFault into payload (at bit 5 to bit 6).
    const bool BMS_BspdLatchedFault_val = in_msg->BMS_BspdLatchedFault_value;
    const uint32_t BMS_BspdLatchedFault_raw = CAN_ENCODE(BMS_BspdLatchedFault_val, CANSIG_BMS_OK_STATUSES_BMS_BSPD_LATCHED_FAULT_SCALE, CANSIG_BMS_OK_STATUSES_BMS_BSPD_LATCHED_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_BspdLatchedFault_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
}

void App_CanUtils_BMS_Contactors_Pack(const BMS_Contactors_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message BMS_Contactors.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_AirPositive into payload (at bit 0 to bit 1).
    const ContactorState BMS_AirPositive_val = in_msg->BMS_AirPositive_value;
    const uint32_t BMS_AirPositive_raw = CAN_ENCODE(BMS_AirPositive_val, CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_SCALE, CANSIG_BMS_CONTACTORS_BMS_AIR_POSITIVE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_AirPositive_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal BMS_AirNegative into payload (at bit 1 to bit 2).
    const ContactorState BMS_AirNegative_val = in_msg->BMS_AirNegative_value;
    const uint32_t BMS_AirNegative_raw = CAN_ENCODE(BMS_AirNegative_val, CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_SCALE, CANSIG_BMS_CONTACTORS_BMS_AIR_NEGATIVE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_AirNegative_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal BMS_PrechargeRelay into payload (at bit 2 to bit 3).
    const ContactorState BMS_PrechargeRelay_val = in_msg->BMS_PrechargeRelay_value;
    const uint32_t BMS_PrechargeRelay_raw = CAN_ENCODE(BMS_PrechargeRelay_val, CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_SCALE, CANSIG_BMS_CONTACTORS_BMS_PRECHARGE_RELAY_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_PrechargeRelay_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
}

void App_CanUtils_BMS_CellTemperatures_Pack(const BMS_CellTemperatures_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message BMS_CellTemperatures.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____BBBB|BBBBBBAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 10-bit signal BMS_MinCellTemperature into payload (at bit 0 to bit 10).
    const float BMS_MinCellTemperature_val = in_msg->BMS_MinCellTemperature_value;
    const uint32_t BMS_MinCellTemperature_raw = CAN_ENCODE(BMS_MinCellTemperature_val, CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_SCALE, CANSIG_BMS_CELL_TEMPERATURES_BMS_MIN_CELL_TEMPERATURE_OFFSET, float);
    out_data[0] |= packShiftRight(BMS_MinCellTemperature_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(BMS_MinCellTemperature_raw, 8, 0x3);   // Packs bits ______## of byte 1
    
    // Pack 10-bit signal BMS_MaxCellTemperature into payload (at bit 10 to bit 20).
    const float BMS_MaxCellTemperature_val = in_msg->BMS_MaxCellTemperature_value;
    const uint32_t BMS_MaxCellTemperature_raw = CAN_ENCODE(BMS_MaxCellTemperature_val, CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_SCALE, CANSIG_BMS_CELL_TEMPERATURES_BMS_MAX_CELL_TEMPERATURE_OFFSET, float);
    out_data[1] |= packShiftLeft(BMS_MaxCellTemperature_raw, 2, 0xfc);   // Packs bits ######__ of byte 1
    out_data[2] |= packShiftRight(BMS_MaxCellTemperature_raw, 6, 0xf);   // Packs bits ____#### of byte 2
    
}

void App_CanUtils_BMS_CellVoltages_Pack(const BMS_CellVoltages_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 4-byte payload for message BMS_CellVoltages.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal BMS_MinCellVoltage into payload (at bit 0 to bit 16).
    const float BMS_MinCellVoltage_val = in_msg->BMS_MinCellVoltage_value;
    const uint32_t BMS_MinCellVoltage_raw = CAN_ENCODE(BMS_MinCellVoltage_val, CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_SCALE, CANSIG_BMS_CELL_VOLTAGES_BMS_MIN_CELL_VOLTAGE_OFFSET, float);
    out_data[0] |= packShiftRight(BMS_MinCellVoltage_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(BMS_MinCellVoltage_raw, 8, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 16-bit signal BMS_MaxCellVoltage into payload (at bit 16 to bit 32).
    const float BMS_MaxCellVoltage_val = in_msg->BMS_MaxCellVoltage_value;
    const uint32_t BMS_MaxCellVoltage_raw = CAN_ENCODE(BMS_MaxCellVoltage_val, CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_SCALE, CANSIG_BMS_CELL_VOLTAGES_BMS_MAX_CELL_VOLTAGE_OFFSET, float);
    out_data[2] |= packShiftRight(BMS_MaxCellVoltage_raw, 0, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(BMS_MaxCellVoltage_raw, 8, 0xff);   // Packs bits ######## of byte 3
    
}

void App_CanUtils_BMS_CellStats_Pack(const BMS_CellStats_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 4-byte payload for message BMS_CellStats.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____HHHH|GGGFFFFE|EEDDDDCC|CBBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal BMS_MinCellVoltageSegment into payload (at bit 0 to bit 3).
    const uint32_t BMS_MinCellVoltageSegment_val = in_msg->BMS_MinCellVoltageSegment_value;
    const uint32_t BMS_MinCellVoltageSegment_raw = CAN_ENCODE(BMS_MinCellVoltageSegment_val, CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_SEGMENT_SCALE, CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_SEGMENT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_MinCellVoltageSegment_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 4-bit signal BMS_MinCellVoltageIdx into payload (at bit 3 to bit 7).
    const uint32_t BMS_MinCellVoltageIdx_val = in_msg->BMS_MinCellVoltageIdx_value;
    const uint32_t BMS_MinCellVoltageIdx_raw = CAN_ENCODE(BMS_MinCellVoltageIdx_val, CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_IDX_SCALE, CANSIG_BMS_CELL_STATS_BMS_MIN_CELL_VOLTAGE_IDX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_MinCellVoltageIdx_raw, 3, 0x78);   // Packs bits _####___ of byte 0
    
    // Pack 3-bit signal BMS_MaxCellVoltageSegment into payload (at bit 7 to bit 10).
    const uint32_t BMS_MaxCellVoltageSegment_val = in_msg->BMS_MaxCellVoltageSegment_value;
    const uint32_t BMS_MaxCellVoltageSegment_raw = CAN_ENCODE(BMS_MaxCellVoltageSegment_val, CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_SEGMENT_SCALE, CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_SEGMENT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_MaxCellVoltageSegment_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    out_data[1] |= packShiftRight(BMS_MaxCellVoltageSegment_raw, 1, 0x3);   // Packs bits ______## of byte 1
    
    // Pack 4-bit signal BMS_MaxCellVoltageIdx into payload (at bit 10 to bit 14).
    const uint32_t BMS_MaxCellVoltageIdx_val = in_msg->BMS_MaxCellVoltageIdx_value;
    const uint32_t BMS_MaxCellVoltageIdx_raw = CAN_ENCODE(BMS_MaxCellVoltageIdx_val, CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_IDX_SCALE, CANSIG_BMS_CELL_STATS_BMS_MAX_CELL_VOLTAGE_IDX_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_MaxCellVoltageIdx_raw, 2, 0x3c);   // Packs bits __####__ of byte 1
    
    // Pack 3-bit signal BMS_MinTempSegment into payload (at bit 14 to bit 17).
    const uint32_t BMS_MinTempSegment_val = in_msg->BMS_MinTempSegment_value;
    const uint32_t BMS_MinTempSegment_raw = CAN_ENCODE(BMS_MinTempSegment_val, CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_SEGMENT_SCALE, CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_SEGMENT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_MinTempSegment_raw, 6, 0xc0);   // Packs bits ##______ of byte 1
    out_data[2] |= packShiftRight(BMS_MinTempSegment_raw, 2, 0x1);   // Packs bits _______# of byte 2
    
    // Pack 4-bit signal BMS_MinTempIdx into payload (at bit 17 to bit 21).
    const uint32_t BMS_MinTempIdx_val = in_msg->BMS_MinTempIdx_value;
    const uint32_t BMS_MinTempIdx_raw = CAN_ENCODE(BMS_MinTempIdx_val, CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_IDX_SCALE, CANSIG_BMS_CELL_STATS_BMS_MIN_TEMP_IDX_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_MinTempIdx_raw, 1, 0x1e);   // Packs bits ___####_ of byte 2
    
    // Pack 3-bit signal BMS_MaxTempSegment into payload (at bit 21 to bit 24).
    const uint32_t BMS_MaxTempSegment_val = in_msg->BMS_MaxTempSegment_value;
    const uint32_t BMS_MaxTempSegment_raw = CAN_ENCODE(BMS_MaxTempSegment_val, CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_SEGMENT_SCALE, CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_SEGMENT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_MaxTempSegment_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    
    // Pack 4-bit signal BMS_MaxTempIdx into payload (at bit 24 to bit 28).
    const uint32_t BMS_MaxTempIdx_val = in_msg->BMS_MaxTempIdx_value;
    const uint32_t BMS_MaxTempIdx_raw = CAN_ENCODE(BMS_MaxTempIdx_val, CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_IDX_SCALE, CANSIG_BMS_CELL_STATS_BMS_MAX_TEMP_IDX_OFFSET, uint32_t);
    out_data[3] |= packShiftRight(BMS_MaxTempIdx_raw, 0, 0xf);   // Packs bits ____#### of byte 3
    
}

void App_CanUtils_BMS_TractiveSystem_Pack(const BMS_TractiveSystem_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message BMS_TractiveSystem.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|____BBBB|BBBBBBBB|BAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 23-bit signal BMS_TractiveSystemVoltage into payload (at bit 0 to bit 23).
    const float BMS_TractiveSystemVoltage_val = in_msg->BMS_TractiveSystemVoltage_value;
    const uint32_t BMS_TractiveSystemVoltage_raw = CAN_ENCODE(BMS_TractiveSystemVoltage_val, CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_VOLTAGE_OFFSET, float);
    out_data[0] |= packShiftRight(BMS_TractiveSystemVoltage_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(BMS_TractiveSystemVoltage_raw, 8, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(BMS_TractiveSystemVoltage_raw, 16, 0x7f);   // Packs bits _####### of byte 2
    
    // Pack 13-bit signal BMS_TractiveSystemCurrent into payload (at bit 23 to bit 36).
    const float BMS_TractiveSystemCurrent_val = in_msg->BMS_TractiveSystemCurrent_value;
    const uint32_t BMS_TractiveSystemCurrent_raw = CAN_ENCODE(BMS_TractiveSystemCurrent_val, CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_BMS_TRACTIVE_SYSTEM_CURRENT_OFFSET, float);
    out_data[2] |= packShiftLeft(BMS_TractiveSystemCurrent_raw, 7, 0x80);   // Packs bits #_______ of byte 2
    out_data[3] |= packShiftRight(BMS_TractiveSystemCurrent_raw, 1, 0xff);   // Packs bits ######## of byte 3
    out_data[4] |= packShiftRight(BMS_TractiveSystemCurrent_raw, 9, 0xf);   // Packs bits ____#### of byte 4
    
}

void App_CanUtils_BMS_PackVoltage_Pack(const BMS_PackVoltage_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message BMS_PackVoltage.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_AAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 23-bit signal BMS_PackVoltage into payload (at bit 0 to bit 23).
    const float BMS_PackVoltage_val = in_msg->BMS_PackVoltage_value;
    const uint32_t BMS_PackVoltage_raw = CAN_ENCODE(BMS_PackVoltage_val, CANSIG_BMS_PACK_VOLTAGE_BMS_PACK_VOLTAGE_SCALE, CANSIG_BMS_PACK_VOLTAGE_BMS_PACK_VOLTAGE_OFFSET, float);
    out_data[0] |= packShiftRight(BMS_PackVoltage_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(BMS_PackVoltage_raw, 8, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(BMS_PackVoltage_raw, 16, 0x7f);   // Packs bits _####### of byte 2
    
}

void App_CanUtils_BMS_AvailablePower_Pack(const BMS_AvailablePower_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message BMS_AvailablePower.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____AAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 20-bit signal BMS_AvailablePower into payload (at bit 0 to bit 20).
    const float BMS_AvailablePower_val = in_msg->BMS_AvailablePower_value;
    const uint32_t BMS_AvailablePower_raw = CAN_ENCODE(BMS_AvailablePower_val, CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_SCALE, CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_OFFSET, float);
    out_data[0] |= packShiftRight(BMS_AvailablePower_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(BMS_AvailablePower_raw, 8, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(BMS_AvailablePower_raw, 16, 0xf);   // Packs bits ____#### of byte 2
    
}

void App_CanUtils_BMS_AuxThermistors_Pack(const BMS_AuxThermistors_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 8-byte payload for message BMS_AuxThermistors.
    // |____FFFF|FFFFFFEE|EEEEEEEE|DDDDDDDD|DDCCCCCC|CCCCBBBB|BBBBBBAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 10-bit signal BMS_ThermTemp0 into payload (at bit 0 to bit 10).
    const float BMS_ThermTemp0_val = in_msg->BMS_ThermTemp0_value;
    const uint32_t BMS_ThermTemp0_raw = CAN_ENCODE(BMS_ThermTemp0_val, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP0_SCALE, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP0_OFFSET, float);
    out_data[0] |= packShiftRight(BMS_ThermTemp0_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(BMS_ThermTemp0_raw, 8, 0x3);   // Packs bits ______## of byte 1
    
    // Pack 10-bit signal BMS_ThermTemp1 into payload (at bit 10 to bit 20).
    const float BMS_ThermTemp1_val = in_msg->BMS_ThermTemp1_value;
    const uint32_t BMS_ThermTemp1_raw = CAN_ENCODE(BMS_ThermTemp1_val, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP1_SCALE, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP1_OFFSET, float);
    out_data[1] |= packShiftLeft(BMS_ThermTemp1_raw, 2, 0xfc);   // Packs bits ######__ of byte 1
    out_data[2] |= packShiftRight(BMS_ThermTemp1_raw, 6, 0xf);   // Packs bits ____#### of byte 2
    
    // Pack 10-bit signal BMS_ThermTemp2 into payload (at bit 20 to bit 30).
    const float BMS_ThermTemp2_val = in_msg->BMS_ThermTemp2_value;
    const uint32_t BMS_ThermTemp2_raw = CAN_ENCODE(BMS_ThermTemp2_val, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP2_SCALE, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP2_OFFSET, float);
    out_data[2] |= packShiftLeft(BMS_ThermTemp2_raw, 4, 0xf0);   // Packs bits ####____ of byte 2
    out_data[3] |= packShiftRight(BMS_ThermTemp2_raw, 4, 0x3f);   // Packs bits __###### of byte 3
    
    // Pack 10-bit signal BMS_ThermTemp3 into payload (at bit 30 to bit 40).
    const float BMS_ThermTemp3_val = in_msg->BMS_ThermTemp3_value;
    const uint32_t BMS_ThermTemp3_raw = CAN_ENCODE(BMS_ThermTemp3_val, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP3_SCALE, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP3_OFFSET, float);
    out_data[3] |= packShiftLeft(BMS_ThermTemp3_raw, 6, 0xc0);   // Packs bits ##______ of byte 3
    out_data[4] |= packShiftRight(BMS_ThermTemp3_raw, 2, 0xff);   // Packs bits ######## of byte 4
    
    // Pack 10-bit signal BMS_ThermTemp4 into payload (at bit 40 to bit 50).
    const float BMS_ThermTemp4_val = in_msg->BMS_ThermTemp4_value;
    const uint32_t BMS_ThermTemp4_raw = CAN_ENCODE(BMS_ThermTemp4_val, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP4_SCALE, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP4_OFFSET, float);
    out_data[5] |= packShiftRight(BMS_ThermTemp4_raw, 0, 0xff);   // Packs bits ######## of byte 5
    out_data[6] |= packShiftRight(BMS_ThermTemp4_raw, 8, 0x3);   // Packs bits ______## of byte 6
    
    // Pack 10-bit signal BMS_ThermTemp5 into payload (at bit 50 to bit 60).
    const float BMS_ThermTemp5_val = in_msg->BMS_ThermTemp5_value;
    const uint32_t BMS_ThermTemp5_raw = CAN_ENCODE(BMS_ThermTemp5_val, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP5_SCALE, CANSIG_BMS_AUX_THERMISTORS_BMS_THERM_TEMP5_OFFSET, float);
    out_data[6] |= packShiftLeft(BMS_ThermTemp5_raw, 2, 0xfc);   // Packs bits ######__ of byte 6
    out_data[7] |= packShiftRight(BMS_ThermTemp5_raw, 6, 0xf);   // Packs bits ____#### of byte 7
    
}

void App_CanUtils_BMS_CommitInfo_Pack(const BMS_CommitInfo_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message BMS_CommitInfo.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_______B|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 32-bit signal BMS_Hash into payload (at bit 0 to bit 32).
    const uint32_t BMS_Hash_val = in_msg->BMS_Hash_value;
    const uint32_t BMS_Hash_raw = CAN_ENCODE(BMS_Hash_val, CANSIG_BMS_COMMIT_INFO_BMS_HASH_SCALE, CANSIG_BMS_COMMIT_INFO_BMS_HASH_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Hash_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(BMS_Hash_raw, 8, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(BMS_Hash_raw, 16, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(BMS_Hash_raw, 24, 0xff);   // Packs bits ######## of byte 3
    
    // Pack 1-bit signal BMS_Clean into payload (at bit 32 to bit 33).
    const bool BMS_Clean_val = in_msg->BMS_Clean_value;
    const uint32_t BMS_Clean_raw = CAN_ENCODE(BMS_Clean_val, CANSIG_BMS_COMMIT_INFO_BMS_CLEAN_SCALE, CANSIG_BMS_COMMIT_INFO_BMS_CLEAN_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(BMS_Clean_raw, 0, 0x1);   // Packs bits _______# of byte 4
    
}

void App_CanUtils_BMS_StackRemaining_Pack(const BMS_StackRemaining_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message BMS_StackRemaining.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_____EEE|EEEEDDDD|DDDCCCCC|CCBBBBBB|BAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 7-bit signal BMS_StackRemainingTask1Hz into payload (at bit 0 to bit 7).
    const uint32_t BMS_StackRemainingTask1Hz_val = in_msg->BMS_StackRemainingTask1Hz_value;
    const uint32_t BMS_StackRemainingTask1Hz_raw = CAN_ENCODE(BMS_StackRemainingTask1Hz_val, CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK1_HZ_SCALE, CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_StackRemainingTask1Hz_raw, 0, 0x7f);   // Packs bits _####### of byte 0
    
    // Pack 7-bit signal BMS_StackRemainingTask100Hz into payload (at bit 7 to bit 14).
    const uint32_t BMS_StackRemainingTask100Hz_val = in_msg->BMS_StackRemainingTask100Hz_value;
    const uint32_t BMS_StackRemainingTask100Hz_raw = CAN_ENCODE(BMS_StackRemainingTask100Hz_val, CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK100_HZ_SCALE, CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_StackRemainingTask100Hz_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    out_data[1] |= packShiftRight(BMS_StackRemainingTask100Hz_raw, 1, 0x3f);   // Packs bits __###### of byte 1
    
    // Pack 7-bit signal BMS_StackRemainingTask1kHz into payload (at bit 14 to bit 21).
    const uint32_t BMS_StackRemainingTask1kHz_val = in_msg->BMS_StackRemainingTask1kHz_value;
    const uint32_t BMS_StackRemainingTask1kHz_raw = CAN_ENCODE(BMS_StackRemainingTask1kHz_val, CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK1K_HZ_SCALE, CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK1K_HZ_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_StackRemainingTask1kHz_raw, 6, 0xc0);   // Packs bits ##______ of byte 1
    out_data[2] |= packShiftRight(BMS_StackRemainingTask1kHz_raw, 2, 0x1f);   // Packs bits ___##### of byte 2
    
    // Pack 7-bit signal BMS_StackRemainingTaskCanRx into payload (at bit 21 to bit 28).
    const uint32_t BMS_StackRemainingTaskCanRx_val = in_msg->BMS_StackRemainingTaskCanRx_value;
    const uint32_t BMS_StackRemainingTaskCanRx_raw = CAN_ENCODE(BMS_StackRemainingTaskCanRx_val, CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK_CAN_RX_SCALE, CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK_CAN_RX_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_StackRemainingTaskCanRx_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    out_data[3] |= packShiftRight(BMS_StackRemainingTaskCanRx_raw, 3, 0xf);   // Packs bits ____#### of byte 3
    
    // Pack 7-bit signal BMS_StackRemainingTaskCanTx into payload (at bit 28 to bit 35).
    const uint32_t BMS_StackRemainingTaskCanTx_val = in_msg->BMS_StackRemainingTaskCanTx_value;
    const uint32_t BMS_StackRemainingTaskCanTx_raw = CAN_ENCODE(BMS_StackRemainingTaskCanTx_val, CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK_CAN_TX_SCALE, CANSIG_BMS_STACK_REMAINING_BMS_STACK_REMAINING_TASK_CAN_TX_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_StackRemainingTaskCanTx_raw, 4, 0xf0);   // Packs bits ####____ of byte 3
    out_data[4] |= packShiftRight(BMS_StackRemainingTaskCanTx_raw, 4, 0x7);   // Packs bits _____### of byte 4
    
}

void App_CanUtils_BMS_Warnings_Pack(const BMS_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message BMS_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_StackWaterMarkHighTask1HzWarning into payload (at bit 0 to bit 1).
    const bool BMS_StackWaterMarkHighTask1HzWarning_val = in_msg->BMS_StackWaterMarkHighTask1HzWarning_value;
    const uint32_t BMS_StackWaterMarkHighTask1HzWarning_raw = CAN_ENCODE(BMS_StackWaterMarkHighTask1HzWarning_val, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_StackWaterMarkHighTask1HzWarning_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal BMS_StackWaterMarkHighTask100HzWarning into payload (at bit 1 to bit 2).
    const bool BMS_StackWaterMarkHighTask100HzWarning_val = in_msg->BMS_StackWaterMarkHighTask100HzWarning_value;
    const uint32_t BMS_StackWaterMarkHighTask100HzWarning_raw = CAN_ENCODE(BMS_StackWaterMarkHighTask100HzWarning_val, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_StackWaterMarkHighTask100HzWarning_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal BMS_StackWaterMarkHighTask1kHzWarning into payload (at bit 2 to bit 3).
    const bool BMS_StackWaterMarkHighTask1kHzWarning_val = in_msg->BMS_StackWaterMarkHighTask1kHzWarning_value;
    const uint32_t BMS_StackWaterMarkHighTask1kHzWarning_raw = CAN_ENCODE(BMS_StackWaterMarkHighTask1kHzWarning_val, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_StackWaterMarkHighTask1kHzWarning_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal BMS_StackWaterMarkHighTaskCanRxWarning into payload (at bit 3 to bit 4).
    const bool BMS_StackWaterMarkHighTaskCanRxWarning_val = in_msg->BMS_StackWaterMarkHighTaskCanRxWarning_value;
    const uint32_t BMS_StackWaterMarkHighTaskCanRxWarning_raw = CAN_ENCODE(BMS_StackWaterMarkHighTaskCanRxWarning_val, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_StackWaterMarkHighTaskCanRxWarning_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal BMS_StackWaterMarkHighTaskCanTxWarning into payload (at bit 4 to bit 5).
    const bool BMS_StackWaterMarkHighTaskCanTxWarning_val = in_msg->BMS_StackWaterMarkHighTaskCanTxWarning_value;
    const uint32_t BMS_StackWaterMarkHighTaskCanTxWarning_raw = CAN_ENCODE(BMS_StackWaterMarkHighTaskCanTxWarning_val, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_StackWaterMarkHighTaskCanTxWarning_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal BMS_WatchdogTimeoutWarning into payload (at bit 5 to bit 6).
    const bool BMS_WatchdogTimeoutWarning_val = in_msg->BMS_WatchdogTimeoutWarning_value;
    const uint32_t BMS_WatchdogTimeoutWarning_raw = CAN_ENCODE(BMS_WatchdogTimeoutWarning_val, CANSIG_BMS_WARNINGS_BMS_WATCHDOG_TIMEOUT_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_WATCHDOG_TIMEOUT_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_WatchdogTimeoutWarning_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 1-bit signal BMS_TxOverflowWarning into payload (at bit 6 to bit 7).
    const bool BMS_TxOverflowWarning_val = in_msg->BMS_TxOverflowWarning_value;
    const uint32_t BMS_TxOverflowWarning_raw = CAN_ENCODE(BMS_TxOverflowWarning_val, CANSIG_BMS_WARNINGS_BMS_TX_OVERFLOW_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_TX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_TxOverflowWarning_raw, 6, 0x40);   // Packs bits _#______ of byte 0
    
    // Pack 1-bit signal BMS_RxOverflowWarning into payload (at bit 7 to bit 8).
    const bool BMS_RxOverflowWarning_val = in_msg->BMS_RxOverflowWarning_value;
    const uint32_t BMS_RxOverflowWarning_raw = CAN_ENCODE(BMS_RxOverflowWarning_val, CANSIG_BMS_WARNINGS_BMS_RX_OVERFLOW_WARNING_SCALE, CANSIG_BMS_WARNINGS_BMS_RX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_RxOverflowWarning_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    
}

void App_CanUtils_BMS_Faults_Pack(const BMS_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 2-byte payload for message BMS_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____LKJI|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_MissingHeartbeatFault into payload (at bit 0 to bit 1).
    const bool BMS_MissingHeartbeatFault_val = in_msg->BMS_MissingHeartbeatFault_value;
    const uint32_t BMS_MissingHeartbeatFault_raw = CAN_ENCODE(BMS_MissingHeartbeatFault_val, CANSIG_BMS_FAULTS_BMS_MISSING_HEARTBEAT_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_MISSING_HEARTBEAT_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_MissingHeartbeatFault_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal BMS_StateMachineFault into payload (at bit 1 to bit 2).
    const bool BMS_StateMachineFault_val = in_msg->BMS_StateMachineFault_value;
    const uint32_t BMS_StateMachineFault_raw = CAN_ENCODE(BMS_StateMachineFault_val, CANSIG_BMS_FAULTS_BMS_STATE_MACHINE_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_STATE_MACHINE_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_StateMachineFault_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal BMS_CellUndervoltageFault into payload (at bit 2 to bit 3).
    const bool BMS_CellUndervoltageFault_val = in_msg->BMS_CellUndervoltageFault_value;
    const uint32_t BMS_CellUndervoltageFault_raw = CAN_ENCODE(BMS_CellUndervoltageFault_val, CANSIG_BMS_FAULTS_BMS_CELL_UNDERVOLTAGE_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CELL_UNDERVOLTAGE_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_CellUndervoltageFault_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal BMS_CellOvervoltageFault into payload (at bit 3 to bit 4).
    const bool BMS_CellOvervoltageFault_val = in_msg->BMS_CellOvervoltageFault_value;
    const uint32_t BMS_CellOvervoltageFault_raw = CAN_ENCODE(BMS_CellOvervoltageFault_val, CANSIG_BMS_FAULTS_BMS_CELL_OVERVOLTAGE_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CELL_OVERVOLTAGE_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_CellOvervoltageFault_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal BMS_ModuleCommunicationFault into payload (at bit 4 to bit 5).
    const bool BMS_ModuleCommunicationFault_val = in_msg->BMS_ModuleCommunicationFault_value;
    const uint32_t BMS_ModuleCommunicationFault_raw = CAN_ENCODE(BMS_ModuleCommunicationFault_val, CANSIG_BMS_FAULTS_BMS_MODULE_COMMUNICATION_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_MODULE_COMMUNICATION_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ModuleCommunicationFault_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal BMS_CellUndertempFault into payload (at bit 5 to bit 6).
    const bool BMS_CellUndertempFault_val = in_msg->BMS_CellUndertempFault_value;
    const uint32_t BMS_CellUndertempFault_raw = CAN_ENCODE(BMS_CellUndertempFault_val, CANSIG_BMS_FAULTS_BMS_CELL_UNDERTEMP_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CELL_UNDERTEMP_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_CellUndertempFault_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 1-bit signal BMS_CellOvertempFault into payload (at bit 6 to bit 7).
    const bool BMS_CellOvertempFault_val = in_msg->BMS_CellOvertempFault_value;
    const uint32_t BMS_CellOvertempFault_raw = CAN_ENCODE(BMS_CellOvertempFault_val, CANSIG_BMS_FAULTS_BMS_CELL_OVERTEMP_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CELL_OVERTEMP_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_CellOvertempFault_raw, 6, 0x40);   // Packs bits _#______ of byte 0
    
    // Pack 1-bit signal BMS_ChargerFault into payload (at bit 7 to bit 8).
    const bool BMS_ChargerFault_val = in_msg->BMS_ChargerFault_value;
    const uint32_t BMS_ChargerFault_raw = CAN_ENCODE(BMS_ChargerFault_val, CANSIG_BMS_FAULTS_BMS_CHARGER_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CHARGER_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_ChargerFault_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    
    // Pack 1-bit signal BMS_ChargerDisconnectedDuringChargeFault into payload (at bit 8 to bit 9).
    const bool BMS_ChargerDisconnectedDuringChargeFault_val = in_msg->BMS_ChargerDisconnectedDuringChargeFault_value;
    const uint32_t BMS_ChargerDisconnectedDuringChargeFault_raw = CAN_ENCODE(BMS_ChargerDisconnectedDuringChargeFault_val, CANSIG_BMS_FAULTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(BMS_ChargerDisconnectedDuringChargeFault_raw, 0, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 1-bit signal BMS_ChargerExternalShutdownFault into payload (at bit 9 to bit 10).
    const bool BMS_ChargerExternalShutdownFault_val = in_msg->BMS_ChargerExternalShutdownFault_value;
    const uint32_t BMS_ChargerExternalShutdownFault_raw = CAN_ENCODE(BMS_ChargerExternalShutdownFault_val, CANSIG_BMS_FAULTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_ChargerExternalShutdownFault_raw, 1, 0x2);   // Packs bits ______#_ of byte 1
    
    // Pack 1-bit signal BMS_TractiveSystemOvercurrentFault into payload (at bit 10 to bit 11).
    const bool BMS_TractiveSystemOvercurrentFault_val = in_msg->BMS_TractiveSystemOvercurrentFault_value;
    const uint32_t BMS_TractiveSystemOvercurrentFault_raw = CAN_ENCODE(BMS_TractiveSystemOvercurrentFault_val, CANSIG_BMS_FAULTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_TractiveSystemOvercurrentFault_raw, 2, 0x4);   // Packs bits _____#__ of byte 1
    
    // Pack 1-bit signal BMS_PrechargeFailureFault into payload (at bit 11 to bit 12).
    const bool BMS_PrechargeFailureFault_val = in_msg->BMS_PrechargeFailureFault_value;
    const uint32_t BMS_PrechargeFailureFault_raw = CAN_ENCODE(BMS_PrechargeFailureFault_val, CANSIG_BMS_FAULTS_BMS_PRECHARGE_FAILURE_FAULT_SCALE, CANSIG_BMS_FAULTS_BMS_PRECHARGE_FAILURE_FAULT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_PrechargeFailureFault_raw, 3, 0x8);   // Packs bits ____#___ of byte 1
    
}

void App_CanUtils_BMS_WarningsCounts_Pack(const BMS_WarningsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message BMS_WarningsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal BMS_StackWaterMarkHighTask1HzWarningCount into payload (at bit 0 to bit 3).
    const uint32_t BMS_StackWaterMarkHighTask1HzWarningCount_val = in_msg->BMS_StackWaterMarkHighTask1HzWarningCount_value;
    const uint32_t BMS_StackWaterMarkHighTask1HzWarningCount_raw = CAN_ENCODE(BMS_StackWaterMarkHighTask1HzWarningCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_StackWaterMarkHighTask1HzWarningCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal BMS_StackWaterMarkHighTask100HzWarningCount into payload (at bit 3 to bit 6).
    const uint32_t BMS_StackWaterMarkHighTask100HzWarningCount_val = in_msg->BMS_StackWaterMarkHighTask100HzWarningCount_value;
    const uint32_t BMS_StackWaterMarkHighTask100HzWarningCount_raw = CAN_ENCODE(BMS_StackWaterMarkHighTask100HzWarningCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_StackWaterMarkHighTask100HzWarningCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal BMS_StackWaterMarkHighTask1kHzWarningCount into payload (at bit 6 to bit 9).
    const uint32_t BMS_StackWaterMarkHighTask1kHzWarningCount_val = in_msg->BMS_StackWaterMarkHighTask1kHzWarningCount_value;
    const uint32_t BMS_StackWaterMarkHighTask1kHzWarningCount_raw = CAN_ENCODE(BMS_StackWaterMarkHighTask1kHzWarningCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_StackWaterMarkHighTask1kHzWarningCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(BMS_StackWaterMarkHighTask1kHzWarningCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 3-bit signal BMS_StackWaterMarkHighTaskCanRxWarningCount into payload (at bit 9 to bit 12).
    const uint32_t BMS_StackWaterMarkHighTaskCanRxWarningCount_val = in_msg->BMS_StackWaterMarkHighTaskCanRxWarningCount_value;
    const uint32_t BMS_StackWaterMarkHighTaskCanRxWarningCount_raw = CAN_ENCODE(BMS_StackWaterMarkHighTaskCanRxWarningCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_StackWaterMarkHighTaskCanRxWarningCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 1
    
    // Pack 3-bit signal BMS_StackWaterMarkHighTaskCanTxWarningCount into payload (at bit 12 to bit 15).
    const uint32_t BMS_StackWaterMarkHighTaskCanTxWarningCount_val = in_msg->BMS_StackWaterMarkHighTaskCanTxWarningCount_value;
    const uint32_t BMS_StackWaterMarkHighTaskCanTxWarningCount_raw = CAN_ENCODE(BMS_StackWaterMarkHighTaskCanTxWarningCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_StackWaterMarkHighTaskCanTxWarningCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1
    
    // Pack 3-bit signal BMS_WatchdogTimeoutWarningCount into payload (at bit 15 to bit 18).
    const uint32_t BMS_WatchdogTimeoutWarningCount_val = in_msg->BMS_WatchdogTimeoutWarningCount_value;
    const uint32_t BMS_WatchdogTimeoutWarningCount_raw = CAN_ENCODE(BMS_WatchdogTimeoutWarningCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_WATCHDOG_TIMEOUT_WARNING_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_WATCHDOG_TIMEOUT_WARNING_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_WatchdogTimeoutWarningCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    out_data[2] |= packShiftRight(BMS_WatchdogTimeoutWarningCount_raw, 1, 0x3);   // Packs bits ______## of byte 2
    
    // Pack 3-bit signal BMS_TxOverflowWarningCount into payload (at bit 18 to bit 21).
    const uint32_t BMS_TxOverflowWarningCount_val = in_msg->BMS_TxOverflowWarningCount_value;
    const uint32_t BMS_TxOverflowWarningCount_raw = CAN_ENCODE(BMS_TxOverflowWarningCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_TX_OVERFLOW_WARNING_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_TX_OVERFLOW_WARNING_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_TxOverflowWarningCount_raw, 2, 0x1c);   // Packs bits ___###__ of byte 2
    
    // Pack 3-bit signal BMS_RxOverflowWarningCount into payload (at bit 21 to bit 24).
    const uint32_t BMS_RxOverflowWarningCount_val = in_msg->BMS_RxOverflowWarningCount_value;
    const uint32_t BMS_RxOverflowWarningCount_raw = CAN_ENCODE(BMS_RxOverflowWarningCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_RX_OVERFLOW_WARNING_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_RX_OVERFLOW_WARNING_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_RxOverflowWarningCount_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    
}

void App_CanUtils_BMS_FaultsCounts_Pack(const BMS_FaultsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message BMS_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|____LLLK|KKJJJIII|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal BMS_MissingHeartbeatFaultCount into payload (at bit 0 to bit 3).
    const uint32_t BMS_MissingHeartbeatFaultCount_val = in_msg->BMS_MissingHeartbeatFaultCount_value;
    const uint32_t BMS_MissingHeartbeatFaultCount_raw = CAN_ENCODE(BMS_MissingHeartbeatFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_MISSING_HEARTBEAT_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_MISSING_HEARTBEAT_FAULT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_MissingHeartbeatFaultCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal BMS_StateMachineFaultCount into payload (at bit 3 to bit 6).
    const uint32_t BMS_StateMachineFaultCount_val = in_msg->BMS_StateMachineFaultCount_value;
    const uint32_t BMS_StateMachineFaultCount_raw = CAN_ENCODE(BMS_StateMachineFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_STATE_MACHINE_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_STATE_MACHINE_FAULT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_StateMachineFaultCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal BMS_CellUndervoltageFaultCount into payload (at bit 6 to bit 9).
    const uint32_t BMS_CellUndervoltageFaultCount_val = in_msg->BMS_CellUndervoltageFaultCount_value;
    const uint32_t BMS_CellUndervoltageFaultCount_raw = CAN_ENCODE(BMS_CellUndervoltageFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_UNDERVOLTAGE_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_UNDERVOLTAGE_FAULT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_CellUndervoltageFaultCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(BMS_CellUndervoltageFaultCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 3-bit signal BMS_CellOvervoltageFaultCount into payload (at bit 9 to bit 12).
    const uint32_t BMS_CellOvervoltageFaultCount_val = in_msg->BMS_CellOvervoltageFaultCount_value;
    const uint32_t BMS_CellOvervoltageFaultCount_raw = CAN_ENCODE(BMS_CellOvervoltageFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_OVERVOLTAGE_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_OVERVOLTAGE_FAULT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_CellOvervoltageFaultCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 1
    
    // Pack 3-bit signal BMS_ModuleCommunicationFaultCount into payload (at bit 12 to bit 15).
    const uint32_t BMS_ModuleCommunicationFaultCount_val = in_msg->BMS_ModuleCommunicationFaultCount_value;
    const uint32_t BMS_ModuleCommunicationFaultCount_raw = CAN_ENCODE(BMS_ModuleCommunicationFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_MODULE_COMMUNICATION_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_MODULE_COMMUNICATION_FAULT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_ModuleCommunicationFaultCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1
    
    // Pack 3-bit signal BMS_CellUndertempFaultCount into payload (at bit 15 to bit 18).
    const uint32_t BMS_CellUndertempFaultCount_val = in_msg->BMS_CellUndertempFaultCount_value;
    const uint32_t BMS_CellUndertempFaultCount_raw = CAN_ENCODE(BMS_CellUndertempFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_UNDERTEMP_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_UNDERTEMP_FAULT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_CellUndertempFaultCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    out_data[2] |= packShiftRight(BMS_CellUndertempFaultCount_raw, 1, 0x3);   // Packs bits ______## of byte 2
    
    // Pack 3-bit signal BMS_CellOvertempFaultCount into payload (at bit 18 to bit 21).
    const uint32_t BMS_CellOvertempFaultCount_val = in_msg->BMS_CellOvertempFaultCount_value;
    const uint32_t BMS_CellOvertempFaultCount_raw = CAN_ENCODE(BMS_CellOvertempFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_OVERTEMP_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_CELL_OVERTEMP_FAULT_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_CellOvertempFaultCount_raw, 2, 0x1c);   // Packs bits ___###__ of byte 2
    
    // Pack 3-bit signal BMS_ChargerFaultCount into payload (at bit 21 to bit 24).
    const uint32_t BMS_ChargerFaultCount_val = in_msg->BMS_ChargerFaultCount_value;
    const uint32_t BMS_ChargerFaultCount_raw = CAN_ENCODE(BMS_ChargerFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_FAULT_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_ChargerFaultCount_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    
    // Pack 3-bit signal BMS_ChargerDisconnectedDuringChargeFaultCount into payload (at bit 24 to bit 27).
    const uint32_t BMS_ChargerDisconnectedDuringChargeFaultCount_val = in_msg->BMS_ChargerDisconnectedDuringChargeFaultCount_value;
    const uint32_t BMS_ChargerDisconnectedDuringChargeFaultCount_raw = CAN_ENCODE(BMS_ChargerDisconnectedDuringChargeFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_DISCONNECTED_DURING_CHARGE_FAULT_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftRight(BMS_ChargerDisconnectedDuringChargeFaultCount_raw, 0, 0x7);   // Packs bits _____### of byte 3
    
    // Pack 3-bit signal BMS_ChargerExternalShutdownFaultCount into payload (at bit 27 to bit 30).
    const uint32_t BMS_ChargerExternalShutdownFaultCount_val = in_msg->BMS_ChargerExternalShutdownFaultCount_value;
    const uint32_t BMS_ChargerExternalShutdownFaultCount_raw = CAN_ENCODE(BMS_ChargerExternalShutdownFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_CHARGER_EXTERNAL_SHUTDOWN_FAULT_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_ChargerExternalShutdownFaultCount_raw, 3, 0x38);   // Packs bits __###___ of byte 3
    
    // Pack 3-bit signal BMS_TractiveSystemOvercurrentFaultCount into payload (at bit 30 to bit 33).
    const uint32_t BMS_TractiveSystemOvercurrentFaultCount_val = in_msg->BMS_TractiveSystemOvercurrentFaultCount_value;
    const uint32_t BMS_TractiveSystemOvercurrentFaultCount_raw = CAN_ENCODE(BMS_TractiveSystemOvercurrentFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_TRACTIVE_SYSTEM_OVERCURRENT_FAULT_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_TractiveSystemOvercurrentFaultCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 3
    out_data[4] |= packShiftRight(BMS_TractiveSystemOvercurrentFaultCount_raw, 2, 0x1);   // Packs bits _______# of byte 4
    
    // Pack 3-bit signal BMS_PrechargeFailureFaultCount into payload (at bit 33 to bit 36).
    const uint32_t BMS_PrechargeFailureFaultCount_val = in_msg->BMS_PrechargeFailureFaultCount_value;
    const uint32_t BMS_PrechargeFailureFaultCount_raw = CAN_ENCODE(BMS_PrechargeFailureFaultCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_PRECHARGE_FAILURE_FAULT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_PRECHARGE_FAILURE_FAULT_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(BMS_PrechargeFailureFaultCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 4
    
}

void App_CanUtils_DIM_Vitals_Unpack(const uint8_t* const in_data, DIM_Vitals_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DIM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DIM_Heartbeat from payload (at bit 0 to bit 1).
    uint32_t DIM_Heartbeat_raw = 0;
    DIM_Heartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DIM_Heartbeat_val = CAN_DECODE(DIM_Heartbeat_raw, CANSIG_DIM_VITALS_DIM_HEARTBEAT_SCALE, CANSIG_DIM_VITALS_DIM_HEARTBEAT_OFFSET, uint32_t);
    out_msg->DIM_Heartbeat_value = DIM_Heartbeat_val;
    
    // Unpack 1-bit signal DIM_State from payload (at bit 1 to bit 2).
    uint32_t DIM_State_raw = 0;
    DIM_State_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    DimState DIM_State_val = CAN_DECODE(DIM_State_raw, CANSIG_DIM_VITALS_DIM_STATE_SCALE, CANSIG_DIM_VITALS_DIM_STATE_OFFSET, uint32_t);
    out_msg->DIM_State_value = DIM_State_val;
    
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

void App_CanUtils_PDM_Vitals_Unpack(const uint8_t* const in_data, PDM_Vitals_Signals* const out_msg)
{
    // Unpack 1-byte payload for message PDM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal PDM_Heartbeat from payload (at bit 0 to bit 1).
    uint32_t PDM_Heartbeat_raw = 0;
    PDM_Heartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool PDM_Heartbeat_val = CAN_DECODE(PDM_Heartbeat_raw, CANSIG_PDM_VITALS_PDM_HEARTBEAT_SCALE, CANSIG_PDM_VITALS_PDM_HEARTBEAT_OFFSET, uint32_t);
    out_msg->PDM_Heartbeat_value = PDM_Heartbeat_val;
    
    // Unpack 1-bit signal PDM_State from payload (at bit 1 to bit 2).
    uint32_t PDM_State_raw = 0;
    PDM_State_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    PdmState PDM_State_val = CAN_DECODE(PDM_State_raw, CANSIG_PDM_VITALS_PDM_STATE_SCALE, CANSIG_PDM_VITALS_PDM_STATE_OFFSET, uint32_t);
    out_msg->PDM_State_value = PDM_State_val;
    
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

void App_CanUtils_Debug_Charging_Unpack(const uint8_t* const in_data, Debug_Charging_Signals* const out_msg)
{
    // Unpack 1-byte payload for message Debug_Charging.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal Debug_StartCharging from payload (at bit 0 to bit 1).
    uint32_t Debug_StartCharging_raw = 0;
    Debug_StartCharging_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool Debug_StartCharging_val = CAN_DECODE(Debug_StartCharging_raw, CANSIG_DEBUG_CHARGING_DEBUG_START_CHARGING_SCALE, CANSIG_DEBUG_CHARGING_DEBUG_START_CHARGING_OFFSET, uint32_t);
    out_msg->Debug_StartCharging_value = Debug_StartCharging_val;
    
}

void App_CanUtils_Debug_CellBalancing_Unpack(const uint8_t* const in_data, Debug_CellBalancing_Signals* const out_msg)
{
    // Unpack 5-byte payload for message Debug_CellBalancing.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|______FF|FFFFFEEE|EEEEEEED|CCCCCCCC|CCCCCCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal Debug_CellBalancingRequest from payload (at bit 0 to bit 1).
    uint32_t Debug_CellBalancingRequest_raw = 0;
    Debug_CellBalancingRequest_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool Debug_CellBalancingRequest_val = CAN_DECODE(Debug_CellBalancingRequest_raw, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_REQUEST_SCALE, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_REQUEST_OFFSET, uint32_t);
    out_msg->Debug_CellBalancingRequest_value = Debug_CellBalancingRequest_val;
    
    // Unpack 1-bit signal Debug_CellBalancingOverrideTarget from payload (at bit 1 to bit 2).
    uint32_t Debug_CellBalancingOverrideTarget_raw = 0;
    Debug_CellBalancingOverrideTarget_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool Debug_CellBalancingOverrideTarget_val = CAN_DECODE(Debug_CellBalancingOverrideTarget_raw, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_SCALE, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_OFFSET, uint32_t);
    out_msg->Debug_CellBalancingOverrideTarget_value = Debug_CellBalancingOverrideTarget_val;
    
    // Unpack 14-bit signal Debug_CellBalancingOverrideTargetValue from payload (at bit 2 to bit 16).
    uint32_t Debug_CellBalancingOverrideTargetValue_raw = 0;
    Debug_CellBalancingOverrideTargetValue_raw |= unpackShiftRight(in_data[0], 2, 0xfc);   // Unpacks bits ######__ of msg byte 0
    Debug_CellBalancingOverrideTargetValue_raw |= unpackShiftLeft(in_data[1], 6, 0xff);   // Unpacks bits ######## of msg byte 1
    float Debug_CellBalancingOverrideTargetValue_val = CAN_DECODE(Debug_CellBalancingOverrideTargetValue_raw, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_VALUE_SCALE, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_TARGET_VALUE_OFFSET, float);
    out_msg->Debug_CellBalancingOverrideTargetValue_value = Debug_CellBalancingOverrideTargetValue_val;
    
    // Unpack 1-bit signal Debug_CellBalancingOverridePWM from payload (at bit 16 to bit 17).
    uint32_t Debug_CellBalancingOverridePWM_raw = 0;
    Debug_CellBalancingOverridePWM_raw |= unpackShiftRight(in_data[2], 0, 0x1);   // Unpacks bits _______# of msg byte 2
    bool Debug_CellBalancingOverridePWM_val = CAN_DECODE(Debug_CellBalancingOverridePWM_raw, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_SCALE, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_OFFSET, uint32_t);
    out_msg->Debug_CellBalancingOverridePWM_value = Debug_CellBalancingOverridePWM_val;
    
    // Unpack 10-bit signal Debug_CellBalancingOverridePWMFrequency from payload (at bit 17 to bit 27).
    uint32_t Debug_CellBalancingOverridePWMFrequency_raw = 0;
    Debug_CellBalancingOverridePWMFrequency_raw |= unpackShiftRight(in_data[2], 1, 0xfe);   // Unpacks bits #######_ of msg byte 2
    Debug_CellBalancingOverridePWMFrequency_raw |= unpackShiftLeft(in_data[3], 7, 0x7);   // Unpacks bits _____### of msg byte 3
    float Debug_CellBalancingOverridePWMFrequency_val = CAN_DECODE(Debug_CellBalancingOverridePWMFrequency_raw, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_FREQUENCY_SCALE, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_FREQUENCY_OFFSET, float);
    out_msg->Debug_CellBalancingOverridePWMFrequency_value = Debug_CellBalancingOverridePWMFrequency_val;
    
    // Unpack 7-bit signal Debug_CellBalancingOverridePWMDuty from payload (at bit 27 to bit 34).
    uint32_t Debug_CellBalancingOverridePWMDuty_raw = 0;
    Debug_CellBalancingOverridePWMDuty_raw |= unpackShiftRight(in_data[3], 3, 0xf8);   // Unpacks bits #####___ of msg byte 3
    Debug_CellBalancingOverridePWMDuty_raw |= unpackShiftLeft(in_data[4], 5, 0x3);   // Unpacks bits ______## of msg byte 4
    uint32_t Debug_CellBalancingOverridePWMDuty_val = CAN_DECODE(Debug_CellBalancingOverridePWMDuty_raw, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_DUTY_SCALE, CANSIG_DEBUG_CELL_BALANCING_DEBUG_CELL_BALANCING_OVERRIDE_PWM_DUTY_OFFSET, uint32_t);
    out_msg->Debug_CellBalancingOverridePWMDuty_value = Debug_CellBalancingOverridePWMDuty_val;
    
}

void App_CanUtils_FSM_Vitals_Unpack(const uint8_t* const in_data, FSM_Vitals_Signals* const out_msg)
{
    // Unpack 1-byte payload for message FSM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_Heartbeat from payload (at bit 0 to bit 1).
    uint32_t FSM_Heartbeat_raw = 0;
    FSM_Heartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_Heartbeat_val = CAN_DECODE(FSM_Heartbeat_raw, CANSIG_FSM_VITALS_FSM_HEARTBEAT_SCALE, CANSIG_FSM_VITALS_FSM_HEARTBEAT_OFFSET, uint32_t);
    out_msg->FSM_Heartbeat_value = FSM_Heartbeat_val;
    
    // Unpack 1-bit signal FSM_State from payload (at bit 1 to bit 2).
    uint32_t FSM_State_raw = 0;
    FSM_State_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    State_Machine FSM_State_val = CAN_DECODE(FSM_State_raw, CANSIG_FSM_VITALS_FSM_STATE_SCALE, CANSIG_FSM_VITALS_FSM_STATE_OFFSET, uint32_t);
    out_msg->FSM_State_value = FSM_State_val;
    
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

