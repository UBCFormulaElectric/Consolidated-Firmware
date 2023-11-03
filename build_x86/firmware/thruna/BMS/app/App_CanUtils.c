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

void App_CanUtils_BMS_Warnings_Pack(const BMS_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message BMS_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_Warning_StackWaterMarkHighTask1Hz into payload (at bit 0 to bit 1).
    const bool BMS_Warning_StackWaterMarkHighTask1Hz_val = in_msg->BMS_Warning_StackWaterMarkHighTask1Hz_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask1Hz_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask1Hz_val, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Warning_StackWaterMarkHighTask1Hz_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal BMS_Warning_StackWaterMarkHighTask100Hz into payload (at bit 1 to bit 2).
    const bool BMS_Warning_StackWaterMarkHighTask100Hz_val = in_msg->BMS_Warning_StackWaterMarkHighTask100Hz_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask100Hz_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask100Hz_val, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTask100Hz_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal BMS_Warning_StackWaterMarkHighTask1kHz into payload (at bit 2 to bit 3).
    const bool BMS_Warning_StackWaterMarkHighTask1kHz_val = in_msg->BMS_Warning_StackWaterMarkHighTask1kHz_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask1kHz_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask1kHz_val, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTask1kHz_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal BMS_Warning_StackWaterMarkHighTaskCanRx into payload (at bit 3 to bit 4).
    const bool BMS_Warning_StackWaterMarkHighTaskCanRx_val = in_msg->BMS_Warning_StackWaterMarkHighTaskCanRx_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanRx_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTaskCanRx_val, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTaskCanRx_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal BMS_Warning_StackWaterMarkHighTaskCanTx into payload (at bit 4 to bit 5).
    const bool BMS_Warning_StackWaterMarkHighTaskCanTx_val = in_msg->BMS_Warning_StackWaterMarkHighTaskCanTx_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanTx_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTaskCanTx_val, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTaskCanTx_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal BMS_Warning_WatchdogTimeout into payload (at bit 5 to bit 6).
    const bool BMS_Warning_WatchdogTimeout_val = in_msg->BMS_Warning_WatchdogTimeout_value;
    const uint32_t BMS_Warning_WatchdogTimeout_raw = CAN_ENCODE(BMS_Warning_WatchdogTimeout_val, CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_WatchdogTimeout_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 1-bit signal BMS_Warning_TxOverflow into payload (at bit 6 to bit 7).
    const bool BMS_Warning_TxOverflow_val = in_msg->BMS_Warning_TxOverflow_value;
    const uint32_t BMS_Warning_TxOverflow_raw = CAN_ENCODE(BMS_Warning_TxOverflow_val, CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_TxOverflow_raw, 6, 0x40);   // Packs bits _#______ of byte 0
    
    // Pack 1-bit signal BMS_Warning_RxOverflow into payload (at bit 7 to bit 8).
    const bool BMS_Warning_RxOverflow_val = in_msg->BMS_Warning_RxOverflow_value;
    const uint32_t BMS_Warning_RxOverflow_raw = CAN_ENCODE(BMS_Warning_RxOverflow_val, CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_SCALE, CANSIG_BMS_WARNINGS_BMS_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_RxOverflow_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    
}

void App_CanUtils_BMS_Faults_Pack(const BMS_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 2-byte payload for message BMS_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____LKJI|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal BMS_Fault_MissingHeartbeat into payload (at bit 0 to bit 1).
    const bool BMS_Fault_MissingHeartbeat_val = in_msg->BMS_Fault_MissingHeartbeat_value;
    const uint32_t BMS_Fault_MissingHeartbeat_raw = CAN_ENCODE(BMS_Fault_MissingHeartbeat_val, CANSIG_BMS_FAULTS_BMS_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Fault_MissingHeartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal BMS_Fault_StateMachine into payload (at bit 1 to bit 2).
    const bool BMS_Fault_StateMachine_val = in_msg->BMS_Fault_StateMachine_value;
    const uint32_t BMS_Fault_StateMachine_raw = CAN_ENCODE(BMS_Fault_StateMachine_val, CANSIG_BMS_FAULTS_BMS_FAULT_STATE_MACHINE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_STATE_MACHINE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_StateMachine_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal BMS_Fault_CellUndervoltage into payload (at bit 2 to bit 3).
    const bool BMS_Fault_CellUndervoltage_val = in_msg->BMS_Fault_CellUndervoltage_value;
    const uint32_t BMS_Fault_CellUndervoltage_raw = CAN_ENCODE(BMS_Fault_CellUndervoltage_val, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERVOLTAGE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_CellUndervoltage_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal BMS_Fault_CellOvervoltage into payload (at bit 3 to bit 4).
    const bool BMS_Fault_CellOvervoltage_val = in_msg->BMS_Fault_CellOvervoltage_value;
    const uint32_t BMS_Fault_CellOvervoltage_raw = CAN_ENCODE(BMS_Fault_CellOvervoltage_val, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERVOLTAGE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_CellOvervoltage_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal BMS_Fault_ModuleCommunicationError into payload (at bit 4 to bit 5).
    const bool BMS_Fault_ModuleCommunicationError_val = in_msg->BMS_Fault_ModuleCommunicationError_value;
    const uint32_t BMS_Fault_ModuleCommunicationError_raw = CAN_ENCODE(BMS_Fault_ModuleCommunicationError_val, CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_ModuleCommunicationError_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal BMS_Fault_CellUndertemp into payload (at bit 5 to bit 6).
    const bool BMS_Fault_CellUndertemp_val = in_msg->BMS_Fault_CellUndertemp_value;
    const uint32_t BMS_Fault_CellUndertemp_raw = CAN_ENCODE(BMS_Fault_CellUndertemp_val, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_UNDERTEMP_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_CellUndertemp_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 1-bit signal BMS_Fault_CellOvertemp into payload (at bit 6 to bit 7).
    const bool BMS_Fault_CellOvertemp_val = in_msg->BMS_Fault_CellOvertemp_value;
    const uint32_t BMS_Fault_CellOvertemp_raw = CAN_ENCODE(BMS_Fault_CellOvertemp_val, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CELL_OVERTEMP_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_CellOvertemp_raw, 6, 0x40);   // Packs bits _#______ of byte 0
    
    // Pack 1-bit signal BMS_Fault_Charger into payload (at bit 7 to bit 8).
    const bool BMS_Fault_Charger_val = in_msg->BMS_Fault_Charger_value;
    const uint32_t BMS_Fault_Charger_raw = CAN_ENCODE(BMS_Fault_Charger_val, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_Charger_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    
    // Pack 1-bit signal BMS_Fault_ChargerDisconnectedDuringCharge into payload (at bit 8 to bit 9).
    const bool BMS_Fault_ChargerDisconnectedDuringCharge_val = in_msg->BMS_Fault_ChargerDisconnectedDuringCharge_value;
    const uint32_t BMS_Fault_ChargerDisconnectedDuringCharge_raw = CAN_ENCODE(BMS_Fault_ChargerDisconnectedDuringCharge_val, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(BMS_Fault_ChargerDisconnectedDuringCharge_raw, 0, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 1-bit signal BMS_Fault_ChargerExternalShutdown into payload (at bit 9 to bit 10).
    const bool BMS_Fault_ChargerExternalShutdown_val = in_msg->BMS_Fault_ChargerExternalShutdown_value;
    const uint32_t BMS_Fault_ChargerExternalShutdown_raw = CAN_ENCODE(BMS_Fault_ChargerExternalShutdown_val, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Fault_ChargerExternalShutdown_raw, 1, 0x2);   // Packs bits ______#_ of byte 1
    
    // Pack 1-bit signal BMS_Fault_TractiveSystemOvercurrent into payload (at bit 10 to bit 11).
    const bool BMS_Fault_TractiveSystemOvercurrent_val = in_msg->BMS_Fault_TractiveSystemOvercurrent_value;
    const uint32_t BMS_Fault_TractiveSystemOvercurrent_raw = CAN_ENCODE(BMS_Fault_TractiveSystemOvercurrent_val, CANSIG_BMS_FAULTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Fault_TractiveSystemOvercurrent_raw, 2, 0x4);   // Packs bits _____#__ of byte 1
    
    // Pack 1-bit signal BMS_Fault_PrechargeFailure into payload (at bit 11 to bit 12).
    const bool BMS_Fault_PrechargeFailure_val = in_msg->BMS_Fault_PrechargeFailure_value;
    const uint32_t BMS_Fault_PrechargeFailure_raw = CAN_ENCODE(BMS_Fault_PrechargeFailure_val, CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_FAILURE_SCALE, CANSIG_BMS_FAULTS_BMS_FAULT_PRECHARGE_FAILURE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Fault_PrechargeFailure_raw, 3, 0x8);   // Packs bits ____#___ of byte 1
    
}

void App_CanUtils_BMS_WarningsCounts_Pack(const BMS_WarningsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message BMS_WarningsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal BMS_Warning_StackWaterMarkHighTask1HzCount into payload (at bit 0 to bit 3).
    const uint32_t BMS_Warning_StackWaterMarkHighTask1HzCount_val = in_msg->BMS_Warning_StackWaterMarkHighTask1HzCount_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask1HzCount_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask1HzCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Warning_StackWaterMarkHighTask1HzCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal BMS_Warning_StackWaterMarkHighTask100HzCount into payload (at bit 3 to bit 6).
    const uint32_t BMS_Warning_StackWaterMarkHighTask100HzCount_val = in_msg->BMS_Warning_StackWaterMarkHighTask100HzCount_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask100HzCount_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask100HzCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTask100HzCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal BMS_Warning_StackWaterMarkHighTask1kHzCount into payload (at bit 6 to bit 9).
    const uint32_t BMS_Warning_StackWaterMarkHighTask1kHzCount_val = in_msg->BMS_Warning_StackWaterMarkHighTask1kHzCount_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTask1kHzCount_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTask1kHzCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTask1kHzCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(BMS_Warning_StackWaterMarkHighTask1kHzCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 3-bit signal BMS_Warning_StackWaterMarkHighTaskCanRxCount into payload (at bit 9 to bit 12).
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanRxCount_val = in_msg->BMS_Warning_StackWaterMarkHighTaskCanRxCount_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanRxCount_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTaskCanRxCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTaskCanRxCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 1
    
    // Pack 3-bit signal BMS_Warning_StackWaterMarkHighTaskCanTxCount into payload (at bit 12 to bit 15).
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanTxCount_val = in_msg->BMS_Warning_StackWaterMarkHighTaskCanTxCount_value;
    const uint32_t BMS_Warning_StackWaterMarkHighTaskCanTxCount_raw = CAN_ENCODE(BMS_Warning_StackWaterMarkHighTaskCanTxCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_StackWaterMarkHighTaskCanTxCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1
    
    // Pack 3-bit signal BMS_Warning_WatchdogTimeoutCount into payload (at bit 15 to bit 18).
    const uint32_t BMS_Warning_WatchdogTimeoutCount_val = in_msg->BMS_Warning_WatchdogTimeoutCount_value;
    const uint32_t BMS_Warning_WatchdogTimeoutCount_raw = CAN_ENCODE(BMS_Warning_WatchdogTimeoutCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Warning_WatchdogTimeoutCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    out_data[2] |= packShiftRight(BMS_Warning_WatchdogTimeoutCount_raw, 1, 0x3);   // Packs bits ______## of byte 2
    
    // Pack 3-bit signal BMS_Warning_TxOverflowCount into payload (at bit 18 to bit 21).
    const uint32_t BMS_Warning_TxOverflowCount_val = in_msg->BMS_Warning_TxOverflowCount_value;
    const uint32_t BMS_Warning_TxOverflowCount_raw = CAN_ENCODE(BMS_Warning_TxOverflowCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_TX_OVERFLOW_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_Warning_TxOverflowCount_raw, 2, 0x1c);   // Packs bits ___###__ of byte 2
    
    // Pack 3-bit signal BMS_Warning_RxOverflowCount into payload (at bit 21 to bit 24).
    const uint32_t BMS_Warning_RxOverflowCount_val = in_msg->BMS_Warning_RxOverflowCount_value;
    const uint32_t BMS_Warning_RxOverflowCount_raw = CAN_ENCODE(BMS_Warning_RxOverflowCount_val, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_RX_OVERFLOW_COUNT_SCALE, CANSIG_BMS_WARNINGS_COUNTS_BMS_WARNING_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_Warning_RxOverflowCount_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    
}

void App_CanUtils_BMS_FaultsCounts_Pack(const BMS_FaultsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message BMS_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|____LLLK|KKJJJIII|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal BMS_Fault_MissingHeartbeatCount into payload (at bit 0 to bit 3).
    const uint32_t BMS_Fault_MissingHeartbeatCount_val = in_msg->BMS_Fault_MissingHeartbeatCount_value;
    const uint32_t BMS_Fault_MissingHeartbeatCount_raw = CAN_ENCODE(BMS_Fault_MissingHeartbeatCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MISSING_HEARTBEAT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MISSING_HEARTBEAT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(BMS_Fault_MissingHeartbeatCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal BMS_Fault_StateMachineCount into payload (at bit 3 to bit 6).
    const uint32_t BMS_Fault_StateMachineCount_val = in_msg->BMS_Fault_StateMachineCount_value;
    const uint32_t BMS_Fault_StateMachineCount_raw = CAN_ENCODE(BMS_Fault_StateMachineCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_STATE_MACHINE_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_STATE_MACHINE_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_StateMachineCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal BMS_Fault_CellUndervoltageCount into payload (at bit 6 to bit 9).
    const uint32_t BMS_Fault_CellUndervoltageCount_val = in_msg->BMS_Fault_CellUndervoltageCount_value;
    const uint32_t BMS_Fault_CellUndervoltageCount_raw = CAN_ENCODE(BMS_Fault_CellUndervoltageCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BMS_Fault_CellUndervoltageCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(BMS_Fault_CellUndervoltageCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 3-bit signal BMS_Fault_CellOvervoltageCount into payload (at bit 9 to bit 12).
    const uint32_t BMS_Fault_CellOvervoltageCount_val = in_msg->BMS_Fault_CellOvervoltageCount_value;
    const uint32_t BMS_Fault_CellOvervoltageCount_raw = CAN_ENCODE(BMS_Fault_CellOvervoltageCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Fault_CellOvervoltageCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 1
    
    // Pack 3-bit signal BMS_Fault_ModuleCommunicationErrorCount into payload (at bit 12 to bit 15).
    const uint32_t BMS_Fault_ModuleCommunicationErrorCount_val = in_msg->BMS_Fault_ModuleCommunicationErrorCount_value;
    const uint32_t BMS_Fault_ModuleCommunicationErrorCount_raw = CAN_ENCODE(BMS_Fault_ModuleCommunicationErrorCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Fault_ModuleCommunicationErrorCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1
    
    // Pack 3-bit signal BMS_Fault_CellUndertempCount into payload (at bit 15 to bit 18).
    const uint32_t BMS_Fault_CellUndertempCount_val = in_msg->BMS_Fault_CellUndertempCount_value;
    const uint32_t BMS_Fault_CellUndertempCount_raw = CAN_ENCODE(BMS_Fault_CellUndertempCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERTEMP_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_UNDERTEMP_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(BMS_Fault_CellUndertempCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    out_data[2] |= packShiftRight(BMS_Fault_CellUndertempCount_raw, 1, 0x3);   // Packs bits ______## of byte 2
    
    // Pack 3-bit signal BMS_Fault_CellOvertempCount into payload (at bit 18 to bit 21).
    const uint32_t BMS_Fault_CellOvertempCount_val = in_msg->BMS_Fault_CellOvertempCount_value;
    const uint32_t BMS_Fault_CellOvertempCount_raw = CAN_ENCODE(BMS_Fault_CellOvertempCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERTEMP_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CELL_OVERTEMP_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_Fault_CellOvertempCount_raw, 2, 0x1c);   // Packs bits ___###__ of byte 2
    
    // Pack 3-bit signal BMS_Fault_ChargerCount into payload (at bit 21 to bit 24).
    const uint32_t BMS_Fault_ChargerCount_val = in_msg->BMS_Fault_ChargerCount_value;
    const uint32_t BMS_Fault_ChargerCount_raw = CAN_ENCODE(BMS_Fault_ChargerCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(BMS_Fault_ChargerCount_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    
    // Pack 3-bit signal BMS_Fault_ChargerDisconnectedDuringChargeCount into payload (at bit 24 to bit 27).
    const uint32_t BMS_Fault_ChargerDisconnectedDuringChargeCount_val = in_msg->BMS_Fault_ChargerDisconnectedDuringChargeCount_value;
    const uint32_t BMS_Fault_ChargerDisconnectedDuringChargeCount_raw = CAN_ENCODE(BMS_Fault_ChargerDisconnectedDuringChargeCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftRight(BMS_Fault_ChargerDisconnectedDuringChargeCount_raw, 0, 0x7);   // Packs bits _____### of byte 3
    
    // Pack 3-bit signal BMS_Fault_ChargerExternalShutdownCount into payload (at bit 27 to bit 30).
    const uint32_t BMS_Fault_ChargerExternalShutdownCount_val = in_msg->BMS_Fault_ChargerExternalShutdownCount_value;
    const uint32_t BMS_Fault_ChargerExternalShutdownCount_raw = CAN_ENCODE(BMS_Fault_ChargerExternalShutdownCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_CHARGER_EXTERNAL_SHUTDOWN_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_Fault_ChargerExternalShutdownCount_raw, 3, 0x38);   // Packs bits __###___ of byte 3
    
    // Pack 3-bit signal BMS_Fault_TractiveSystemOvercurrentCount into payload (at bit 30 to bit 33).
    const uint32_t BMS_Fault_TractiveSystemOvercurrentCount_val = in_msg->BMS_Fault_TractiveSystemOvercurrentCount_value;
    const uint32_t BMS_Fault_TractiveSystemOvercurrentCount_raw = CAN_ENCODE(BMS_Fault_TractiveSystemOvercurrentCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(BMS_Fault_TractiveSystemOvercurrentCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 3
    out_data[4] |= packShiftRight(BMS_Fault_TractiveSystemOvercurrentCount_raw, 2, 0x1);   // Packs bits _______# of byte 4
    
    // Pack 3-bit signal BMS_Fault_PrechargeFailureCount into payload (at bit 33 to bit 36).
    const uint32_t BMS_Fault_PrechargeFailureCount_val = in_msg->BMS_Fault_PrechargeFailureCount_value;
    const uint32_t BMS_Fault_PrechargeFailureCount_raw = CAN_ENCODE(BMS_Fault_PrechargeFailureCount_val, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_PRECHARGE_FAILURE_COUNT_SCALE, CANSIG_BMS_FAULTS_COUNTS_BMS_FAULT_PRECHARGE_FAILURE_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(BMS_Fault_PrechargeFailureCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 4
    
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
    
    // Unpack 1-bit signal DIM_Warning_StackWaterMarkHighTask1Hz from payload (at bit 0 to bit 1).
    uint32_t DIM_Warning_StackWaterMarkHighTask1Hz_raw = 0;
    DIM_Warning_StackWaterMarkHighTask1Hz_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DIM_Warning_StackWaterMarkHighTask1Hz_val = CAN_DECODE(DIM_Warning_StackWaterMarkHighTask1Hz_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_msg->DIM_Warning_StackWaterMarkHighTask1Hz_value = DIM_Warning_StackWaterMarkHighTask1Hz_val;
    
    // Unpack 1-bit signal DIM_Warning_StackWaterMarkHighTask100Hz from payload (at bit 1 to bit 2).
    uint32_t DIM_Warning_StackWaterMarkHighTask100Hz_raw = 0;
    DIM_Warning_StackWaterMarkHighTask100Hz_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool DIM_Warning_StackWaterMarkHighTask100Hz_val = CAN_DECODE(DIM_Warning_StackWaterMarkHighTask100Hz_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_msg->DIM_Warning_StackWaterMarkHighTask100Hz_value = DIM_Warning_StackWaterMarkHighTask100Hz_val;
    
    // Unpack 1-bit signal DIM_Warning_StackWaterMarkHighTask1kHz from payload (at bit 2 to bit 3).
    uint32_t DIM_Warning_StackWaterMarkHighTask1kHz_raw = 0;
    DIM_Warning_StackWaterMarkHighTask1kHz_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool DIM_Warning_StackWaterMarkHighTask1kHz_val = CAN_DECODE(DIM_Warning_StackWaterMarkHighTask1kHz_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_msg->DIM_Warning_StackWaterMarkHighTask1kHz_value = DIM_Warning_StackWaterMarkHighTask1kHz_val;
    
    // Unpack 1-bit signal DIM_Warning_StackWaterMarkHighTaskCanRx from payload (at bit 3 to bit 4).
    uint32_t DIM_Warning_StackWaterMarkHighTaskCanRx_raw = 0;
    DIM_Warning_StackWaterMarkHighTaskCanRx_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool DIM_Warning_StackWaterMarkHighTaskCanRx_val = CAN_DECODE(DIM_Warning_StackWaterMarkHighTaskCanRx_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_msg->DIM_Warning_StackWaterMarkHighTaskCanRx_value = DIM_Warning_StackWaterMarkHighTaskCanRx_val;
    
    // Unpack 1-bit signal DIM_Warning_StackWaterMarkHighTaskCanTx from payload (at bit 4 to bit 5).
    uint32_t DIM_Warning_StackWaterMarkHighTaskCanTx_raw = 0;
    DIM_Warning_StackWaterMarkHighTaskCanTx_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool DIM_Warning_StackWaterMarkHighTaskCanTx_val = CAN_DECODE(DIM_Warning_StackWaterMarkHighTaskCanTx_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_msg->DIM_Warning_StackWaterMarkHighTaskCanTx_value = DIM_Warning_StackWaterMarkHighTaskCanTx_val;
    
    // Unpack 1-bit signal DIM_Warning_WatchdogTimeout from payload (at bit 5 to bit 6).
    uint32_t DIM_Warning_WatchdogTimeout_raw = 0;
    DIM_Warning_WatchdogTimeout_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool DIM_Warning_WatchdogTimeout_val = CAN_DECODE(DIM_Warning_WatchdogTimeout_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_msg->DIM_Warning_WatchdogTimeout_value = DIM_Warning_WatchdogTimeout_val;
    
    // Unpack 1-bit signal DIM_Warning_TxOverflow from payload (at bit 6 to bit 7).
    uint32_t DIM_Warning_TxOverflow_raw = 0;
    DIM_Warning_TxOverflow_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool DIM_Warning_TxOverflow_val = CAN_DECODE(DIM_Warning_TxOverflow_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_msg->DIM_Warning_TxOverflow_value = DIM_Warning_TxOverflow_val;
    
    // Unpack 1-bit signal DIM_Warning_RxOverflow from payload (at bit 7 to bit 8).
    uint32_t DIM_Warning_RxOverflow_raw = 0;
    DIM_Warning_RxOverflow_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool DIM_Warning_RxOverflow_val = CAN_DECODE(DIM_Warning_RxOverflow_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_msg->DIM_Warning_RxOverflow_value = DIM_Warning_RxOverflow_val;
    
}

void App_CanUtils_DIM_Faults_Unpack(const uint8_t* const in_data, DIM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DIM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DIM_Fault_MissingHeartbeat from payload (at bit 0 to bit 1).
    uint32_t DIM_Fault_MissingHeartbeat_raw = 0;
    DIM_Fault_MissingHeartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DIM_Fault_MissingHeartbeat_val = CAN_DECODE(DIM_Fault_MissingHeartbeat_raw, CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_msg->DIM_Fault_MissingHeartbeat_value = DIM_Fault_MissingHeartbeat_val;
    
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

