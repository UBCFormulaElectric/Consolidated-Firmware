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

void App_CanUtils_FSM_Vitals_Pack(const FSM_Vitals_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message FSM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal FSM_Heartbeat into payload (at bit 0 to bit 1).
    const bool FSM_Heartbeat_val = in_msg->FSM_Heartbeat_value;
    const uint32_t FSM_Heartbeat_raw = CAN_ENCODE(FSM_Heartbeat_val, CANSIG_FSM_VITALS_FSM_HEARTBEAT_SCALE, CANSIG_FSM_VITALS_FSM_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal FSM_State into payload (at bit 1 to bit 2).
    const State_Machine FSM_State_val = in_msg->FSM_State_value;
    const uint32_t FSM_State_raw = CAN_ENCODE(FSM_State_val, CANSIG_FSM_VITALS_FSM_STATE_SCALE, CANSIG_FSM_VITALS_FSM_STATE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_State_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
}

void App_CanUtils_FSM_AlertsContext_Pack(const FSM_AlertsContext_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message FSM_AlertsContext.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CCC|CCCCCBBB|BBBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal FSM_WatchdogTimeoutTaskName into payload (at bit 0 to bit 3).
    const RtosTaskName FSM_WatchdogTimeoutTaskName_val = in_msg->FSM_WatchdogTimeoutTaskName_value;
    const uint32_t FSM_WatchdogTimeoutTaskName_raw = CAN_ENCODE(FSM_WatchdogTimeoutTaskName_val, CANSIG_FSM_ALERTS_CONTEXT_FSM_WATCHDOG_TIMEOUT_TASK_NAME_SCALE, CANSIG_FSM_ALERTS_CONTEXT_FSM_WATCHDOG_TIMEOUT_TASK_NAME_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_WatchdogTimeoutTaskName_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 8-bit signal FSM_TxOverflowCount into payload (at bit 3 to bit 11).
    const uint32_t FSM_TxOverflowCount_val = in_msg->FSM_TxOverflowCount_value;
    const uint32_t FSM_TxOverflowCount_raw = CAN_ENCODE(FSM_TxOverflowCount_val, CANSIG_FSM_ALERTS_CONTEXT_FSM_TX_OVERFLOW_COUNT_SCALE, CANSIG_FSM_ALERTS_CONTEXT_FSM_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_TxOverflowCount_raw, 3, 0xf8);   // Packs bits #####___ of byte 0
    out_data[1] |= packShiftRight(FSM_TxOverflowCount_raw, 5, 0x7);   // Packs bits _____### of byte 1
    
    // Pack 8-bit signal FSM_RxOverflowCount into payload (at bit 11 to bit 19).
    const uint32_t FSM_RxOverflowCount_val = in_msg->FSM_RxOverflowCount_value;
    const uint32_t FSM_RxOverflowCount_raw = CAN_ENCODE(FSM_RxOverflowCount_val, CANSIG_FSM_ALERTS_CONTEXT_FSM_RX_OVERFLOW_COUNT_SCALE, CANSIG_FSM_ALERTS_CONTEXT_FSM_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_RxOverflowCount_raw, 3, 0xf8);   // Packs bits #####___ of byte 1
    out_data[2] |= packShiftRight(FSM_RxOverflowCount_raw, 5, 0x7);   // Packs bits _____### of byte 2
    
}

void App_CanUtils_FSM_Apps_Pack(const FSM_Apps_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 7-byte payload for message FSM_Apps.
    // |xxxxxxxx|____FEEE|EEEEEEEE|DDDDDDDD|DDCCCCCC|CCCCBBBB|BBBBBBAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 10-bit signal FSM_PappsMappedPedalPercentage into payload (at bit 0 to bit 10).
    const float FSM_PappsMappedPedalPercentage_val = in_msg->FSM_PappsMappedPedalPercentage_value;
    const uint32_t FSM_PappsMappedPedalPercentage_raw = CAN_ENCODE(FSM_PappsMappedPedalPercentage_val, CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET, float);
    out_data[0] |= packShiftRight(FSM_PappsMappedPedalPercentage_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(FSM_PappsMappedPedalPercentage_raw, 8, 0x3);   // Packs bits ______## of byte 1
    
    // Pack 10-bit signal FSM_SappsMappedPedalPercentage into payload (at bit 10 to bit 20).
    const float FSM_SappsMappedPedalPercentage_val = in_msg->FSM_SappsMappedPedalPercentage_value;
    const uint32_t FSM_SappsMappedPedalPercentage_raw = CAN_ENCODE(FSM_SappsMappedPedalPercentage_val, CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET, float);
    out_data[1] |= packShiftLeft(FSM_SappsMappedPedalPercentage_raw, 2, 0xfc);   // Packs bits ######__ of byte 1
    out_data[2] |= packShiftRight(FSM_SappsMappedPedalPercentage_raw, 6, 0xf);   // Packs bits ____#### of byte 2
    
    // Pack 10-bit signal FSM_PappsRawPedalPercentage into payload (at bit 20 to bit 30).
    const float FSM_PappsRawPedalPercentage_val = in_msg->FSM_PappsRawPedalPercentage_value;
    const uint32_t FSM_PappsRawPedalPercentage_raw = CAN_ENCODE(FSM_PappsRawPedalPercentage_val, CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_OFFSET, float);
    out_data[2] |= packShiftLeft(FSM_PappsRawPedalPercentage_raw, 4, 0xf0);   // Packs bits ####____ of byte 2
    out_data[3] |= packShiftRight(FSM_PappsRawPedalPercentage_raw, 4, 0x3f);   // Packs bits __###### of byte 3
    
    // Pack 10-bit signal FSM_SappsRawPedalPercentage into payload (at bit 30 to bit 40).
    const float FSM_SappsRawPedalPercentage_val = in_msg->FSM_SappsRawPedalPercentage_value;
    const uint32_t FSM_SappsRawPedalPercentage_raw = CAN_ENCODE(FSM_SappsRawPedalPercentage_val, CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_OFFSET, float);
    out_data[3] |= packShiftLeft(FSM_SappsRawPedalPercentage_raw, 6, 0xc0);   // Packs bits ##______ of byte 3
    out_data[4] |= packShiftRight(FSM_SappsRawPedalPercentage_raw, 2, 0xff);   // Packs bits ######## of byte 4
    
    // Pack 11-bit signal FSM_TorqueLimit into payload (at bit 40 to bit 51).
    const float FSM_TorqueLimit_val = in_msg->FSM_TorqueLimit_value;
    const uint32_t FSM_TorqueLimit_raw = CAN_ENCODE(FSM_TorqueLimit_val, CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_SCALE, CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_OFFSET, float);
    out_data[5] |= packShiftRight(FSM_TorqueLimit_raw, 0, 0xff);   // Packs bits ######## of byte 5
    out_data[6] |= packShiftRight(FSM_TorqueLimit_raw, 8, 0x7);   // Packs bits _____### of byte 6
    
    // Pack 1-bit signal FSM_TorquePlausabilityFailed into payload (at bit 51 to bit 52).
    const bool FSM_TorquePlausabilityFailed_val = in_msg->FSM_TorquePlausabilityFailed_value;
    const uint32_t FSM_TorquePlausabilityFailed_raw = CAN_ENCODE(FSM_TorquePlausabilityFailed_val, CANSIG_FSM_APPS_FSM_TORQUE_PLAUSABILITY_FAILED_SCALE, CANSIG_FSM_APPS_FSM_TORQUE_PLAUSABILITY_FAILED_OFFSET, uint32_t);
    out_data[6] |= packShiftLeft(FSM_TorquePlausabilityFailed_raw, 3, 0x8);   // Packs bits ____#___ of byte 6
    
}

void App_CanUtils_FSM_Brake_Pack(const FSM_Brake_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message FSM_Brake.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|___FEDCC|CCCCCCCC|BBBBBBBB|BBBBAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 12-bit signal FSM_FrontBrakePressure into payload (at bit 0 to bit 12).
    const uint32_t FSM_FrontBrakePressure_val = in_msg->FSM_FrontBrakePressure_value;
    const uint32_t FSM_FrontBrakePressure_raw = CAN_ENCODE(FSM_FrontBrakePressure_val, CANSIG_FSM_BRAKE_FSM_FRONT_BRAKE_PRESSURE_SCALE, CANSIG_FSM_BRAKE_FSM_FRONT_BRAKE_PRESSURE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_FrontBrakePressure_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(FSM_FrontBrakePressure_raw, 8, 0xf);   // Packs bits ____#### of byte 1
    
    // Pack 12-bit signal FSM_RearBrakePressure into payload (at bit 12 to bit 24).
    const uint32_t FSM_RearBrakePressure_val = in_msg->FSM_RearBrakePressure_value;
    const uint32_t FSM_RearBrakePressure_raw = CAN_ENCODE(FSM_RearBrakePressure_val, CANSIG_FSM_BRAKE_FSM_REAR_BRAKE_PRESSURE_SCALE, CANSIG_FSM_BRAKE_FSM_REAR_BRAKE_PRESSURE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_RearBrakePressure_raw, 4, 0xf0);   // Packs bits ####____ of byte 1
    out_data[2] |= packShiftRight(FSM_RearBrakePressure_raw, 4, 0xff);   // Packs bits ######## of byte 2
    
    // Pack 10-bit signal FSM_BrakePedalPercentage into payload (at bit 24 to bit 34).
    const float FSM_BrakePedalPercentage_val = in_msg->FSM_BrakePedalPercentage_value;
    const uint32_t FSM_BrakePedalPercentage_raw = CAN_ENCODE(FSM_BrakePedalPercentage_val, CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_OFFSET, float);
    out_data[3] |= packShiftRight(FSM_BrakePedalPercentage_raw, 0, 0xff);   // Packs bits ######## of byte 3
    out_data[4] |= packShiftRight(FSM_BrakePedalPercentage_raw, 8, 0x3);   // Packs bits ______## of byte 4
    
    // Pack 1-bit signal FSM_BrakeActuated into payload (at bit 34 to bit 35).
    const bool FSM_BrakeActuated_val = in_msg->FSM_BrakeActuated_value;
    const uint32_t FSM_BrakeActuated_raw = CAN_ENCODE(FSM_BrakeActuated_val, CANSIG_FSM_BRAKE_FSM_BRAKE_ACTUATED_SCALE, CANSIG_FSM_BRAKE_FSM_BRAKE_ACTUATED_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(FSM_BrakeActuated_raw, 2, 0x4);   // Packs bits _____#__ of byte 4
    
    // Pack 1-bit signal FSM_BrakePressureSensorOCSC into payload (at bit 35 to bit 36).
    const bool FSM_BrakePressureSensorOCSC_val = in_msg->FSM_BrakePressureSensorOCSC_value;
    const uint32_t FSM_BrakePressureSensorOCSC_raw = CAN_ENCODE(FSM_BrakePressureSensorOCSC_val, CANSIG_FSM_BRAKE_FSM_BRAKE_PRESSURE_SENSOR_OCSC_SCALE, CANSIG_FSM_BRAKE_FSM_BRAKE_PRESSURE_SENSOR_OCSC_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(FSM_BrakePressureSensorOCSC_raw, 3, 0x8);   // Packs bits ____#___ of byte 4
    
    // Pack 1-bit signal FSM_BrakePedalSensorOCSC into payload (at bit 36 to bit 37).
    const bool FSM_BrakePedalSensorOCSC_val = in_msg->FSM_BrakePedalSensorOCSC_value;
    const uint32_t FSM_BrakePedalSensorOCSC_raw = CAN_ENCODE(FSM_BrakePedalSensorOCSC_val, CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_SENSOR_OCSC_SCALE, CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_SENSOR_OCSC_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(FSM_BrakePedalSensorOCSC_raw, 4, 0x10);   // Packs bits ___#____ of byte 4
    
}

void App_CanUtils_FSM_Coolant_Pack(const FSM_Coolant_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 7-byte payload for message FSM_Coolant.
    // |xxxxxxxx|______EE|EEEEEEEE|DDDDDDDD|DDCCCCCC|CCCCBBBB|BBBBBBAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 10-bit signal FSM_CoolantFlowRate into payload (at bit 0 to bit 10).
    const float FSM_CoolantFlowRate_val = in_msg->FSM_CoolantFlowRate_value;
    const uint32_t FSM_CoolantFlowRate_raw = CAN_ENCODE(FSM_CoolantFlowRate_val, CANSIG_FSM_COOLANT_FSM_COOLANT_FLOW_RATE_SCALE, CANSIG_FSM_COOLANT_FSM_COOLANT_FLOW_RATE_OFFSET, float);
    out_data[0] |= packShiftRight(FSM_CoolantFlowRate_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(FSM_CoolantFlowRate_raw, 8, 0x3);   // Packs bits ______## of byte 1
    
    // Pack 10-bit signal FSM_CoolantTemperatureA into payload (at bit 10 to bit 20).
    const float FSM_CoolantTemperatureA_val = in_msg->FSM_CoolantTemperatureA_value;
    const uint32_t FSM_CoolantTemperatureA_raw = CAN_ENCODE(FSM_CoolantTemperatureA_val, CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_A_SCALE, CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_A_OFFSET, float);
    out_data[1] |= packShiftLeft(FSM_CoolantTemperatureA_raw, 2, 0xfc);   // Packs bits ######__ of byte 1
    out_data[2] |= packShiftRight(FSM_CoolantTemperatureA_raw, 6, 0xf);   // Packs bits ____#### of byte 2
    
    // Pack 10-bit signal FSM_CoolantTemperatureB into payload (at bit 20 to bit 30).
    const float FSM_CoolantTemperatureB_val = in_msg->FSM_CoolantTemperatureB_value;
    const uint32_t FSM_CoolantTemperatureB_raw = CAN_ENCODE(FSM_CoolantTemperatureB_val, CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_B_SCALE, CANSIG_FSM_COOLANT_FSM_COOLANT_TEMPERATURE_B_OFFSET, float);
    out_data[2] |= packShiftLeft(FSM_CoolantTemperatureB_raw, 4, 0xf0);   // Packs bits ####____ of byte 2
    out_data[3] |= packShiftRight(FSM_CoolantTemperatureB_raw, 4, 0x3f);   // Packs bits __###### of byte 3
    
    // Pack 10-bit signal FSM_CoolantPressureA into payload (at bit 30 to bit 40).
    const float FSM_CoolantPressureA_val = in_msg->FSM_CoolantPressureA_value;
    const uint32_t FSM_CoolantPressureA_raw = CAN_ENCODE(FSM_CoolantPressureA_val, CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_A_SCALE, CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_A_OFFSET, float);
    out_data[3] |= packShiftLeft(FSM_CoolantPressureA_raw, 6, 0xc0);   // Packs bits ##______ of byte 3
    out_data[4] |= packShiftRight(FSM_CoolantPressureA_raw, 2, 0xff);   // Packs bits ######## of byte 4
    
    // Pack 10-bit signal FSM_CoolantPressureB into payload (at bit 40 to bit 50).
    const float FSM_CoolantPressureB_val = in_msg->FSM_CoolantPressureB_value;
    const uint32_t FSM_CoolantPressureB_raw = CAN_ENCODE(FSM_CoolantPressureB_val, CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_B_SCALE, CANSIG_FSM_COOLANT_FSM_COOLANT_PRESSURE_B_OFFSET, float);
    out_data[5] |= packShiftRight(FSM_CoolantPressureB_raw, 0, 0xff);   // Packs bits ######## of byte 5
    out_data[6] |= packShiftRight(FSM_CoolantPressureB_raw, 8, 0x3);   // Packs bits ______## of byte 6
    
}

void App_CanUtils_FSM_Steering_Pack(const FSM_Steering_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 2-byte payload for message FSM_Steering.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____AAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 12-bit signal FSM_SteeringAngle into payload (at bit 0 to bit 12).
    const float FSM_SteeringAngle_val = in_msg->FSM_SteeringAngle_value;
    const uint32_t FSM_SteeringAngle_raw = CAN_ENCODE(FSM_SteeringAngle_val, CANSIG_FSM_STEERING_FSM_STEERING_ANGLE_SCALE, CANSIG_FSM_STEERING_FSM_STEERING_ANGLE_OFFSET, float);
    out_data[0] |= packShiftRight(FSM_SteeringAngle_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(FSM_SteeringAngle_raw, 8, 0xf);   // Packs bits ____#### of byte 1
    
}

void App_CanUtils_FSM_Wheels_Pack(const FSM_Wheels_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message FSM_Wheels.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__BBBBBB|BBBBBAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 11-bit signal FSM_LeftWheelSpeed into payload (at bit 0 to bit 11).
    const float FSM_LeftWheelSpeed_val = in_msg->FSM_LeftWheelSpeed_value;
    const uint32_t FSM_LeftWheelSpeed_raw = CAN_ENCODE(FSM_LeftWheelSpeed_val, CANSIG_FSM_WHEELS_FSM_LEFT_WHEEL_SPEED_SCALE, CANSIG_FSM_WHEELS_FSM_LEFT_WHEEL_SPEED_OFFSET, float);
    out_data[0] |= packShiftRight(FSM_LeftWheelSpeed_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(FSM_LeftWheelSpeed_raw, 8, 0x7);   // Packs bits _____### of byte 1
    
    // Pack 11-bit signal FSM_RightWheelSpeed into payload (at bit 11 to bit 22).
    const float FSM_RightWheelSpeed_val = in_msg->FSM_RightWheelSpeed_value;
    const uint32_t FSM_RightWheelSpeed_raw = CAN_ENCODE(FSM_RightWheelSpeed_val, CANSIG_FSM_WHEELS_FSM_RIGHT_WHEEL_SPEED_SCALE, CANSIG_FSM_WHEELS_FSM_RIGHT_WHEEL_SPEED_OFFSET, float);
    out_data[1] |= packShiftLeft(FSM_RightWheelSpeed_raw, 3, 0xf8);   // Packs bits #####___ of byte 1
    out_data[2] |= packShiftRight(FSM_RightWheelSpeed_raw, 5, 0x3f);   // Packs bits __###### of byte 2
    
}

void App_CanUtils_FSM_CommitInfo_Pack(const FSM_CommitInfo_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message FSM_CommitInfo.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_______B|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 32-bit signal FSM_Hash into payload (at bit 0 to bit 32).
    const uint32_t FSM_Hash_val = in_msg->FSM_Hash_value;
    const uint32_t FSM_Hash_raw = CAN_ENCODE(FSM_Hash_val, CANSIG_FSM_COMMIT_INFO_FSM_HASH_SCALE, CANSIG_FSM_COMMIT_INFO_FSM_HASH_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_Hash_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(FSM_Hash_raw, 8, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(FSM_Hash_raw, 16, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(FSM_Hash_raw, 24, 0xff);   // Packs bits ######## of byte 3
    
    // Pack 1-bit signal FSM_Clean into payload (at bit 32 to bit 33).
    const bool FSM_Clean_val = in_msg->FSM_Clean_value;
    const uint32_t FSM_Clean_raw = CAN_ENCODE(FSM_Clean_val, CANSIG_FSM_COMMIT_INFO_FSM_CLEAN_SCALE, CANSIG_FSM_COMMIT_INFO_FSM_CLEAN_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(FSM_Clean_raw, 0, 0x1);   // Packs bits _______# of byte 4
    
}

void App_CanUtils_FSM_Warnings_Pack(const FSM_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message FSM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______Q|PONMLKJI|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal FSM_Warning_StackWaterMarkHighTask1Hz into payload (at bit 0 to bit 1).
    const bool FSM_Warning_StackWaterMarkHighTask1Hz_val = in_msg->FSM_Warning_StackWaterMarkHighTask1Hz_value;
    const uint32_t FSM_Warning_StackWaterMarkHighTask1Hz_raw = CAN_ENCODE(FSM_Warning_StackWaterMarkHighTask1Hz_val, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_Warning_StackWaterMarkHighTask1Hz_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal FSM_Warning_StackWaterMarkHighTask100Hz into payload (at bit 1 to bit 2).
    const bool FSM_Warning_StackWaterMarkHighTask100Hz_val = in_msg->FSM_Warning_StackWaterMarkHighTask100Hz_value;
    const uint32_t FSM_Warning_StackWaterMarkHighTask100Hz_raw = CAN_ENCODE(FSM_Warning_StackWaterMarkHighTask100Hz_val, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Warning_StackWaterMarkHighTask100Hz_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal FSM_Warning_StackWaterMarkHighTask1kHz into payload (at bit 2 to bit 3).
    const bool FSM_Warning_StackWaterMarkHighTask1kHz_val = in_msg->FSM_Warning_StackWaterMarkHighTask1kHz_value;
    const uint32_t FSM_Warning_StackWaterMarkHighTask1kHz_raw = CAN_ENCODE(FSM_Warning_StackWaterMarkHighTask1kHz_val, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Warning_StackWaterMarkHighTask1kHz_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal FSM_Warning_StackWaterMarkHighTaskCanRx into payload (at bit 3 to bit 4).
    const bool FSM_Warning_StackWaterMarkHighTaskCanRx_val = in_msg->FSM_Warning_StackWaterMarkHighTaskCanRx_value;
    const uint32_t FSM_Warning_StackWaterMarkHighTaskCanRx_raw = CAN_ENCODE(FSM_Warning_StackWaterMarkHighTaskCanRx_val, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Warning_StackWaterMarkHighTaskCanRx_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal FSM_Warning_StackWaterMarkHighTaskCanTx into payload (at bit 4 to bit 5).
    const bool FSM_Warning_StackWaterMarkHighTaskCanTx_val = in_msg->FSM_Warning_StackWaterMarkHighTaskCanTx_value;
    const uint32_t FSM_Warning_StackWaterMarkHighTaskCanTx_raw = CAN_ENCODE(FSM_Warning_StackWaterMarkHighTaskCanTx_val, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Warning_StackWaterMarkHighTaskCanTx_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal FSM_Warning_WatchdogTimeout into payload (at bit 5 to bit 6).
    const bool FSM_Warning_WatchdogTimeout_val = in_msg->FSM_Warning_WatchdogTimeout_value;
    const uint32_t FSM_Warning_WatchdogTimeout_raw = CAN_ENCODE(FSM_Warning_WatchdogTimeout_val, CANSIG_FSM_WARNINGS_FSM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Warning_WatchdogTimeout_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 1-bit signal FSM_Warning_TxOverflow into payload (at bit 6 to bit 7).
    const bool FSM_Warning_TxOverflow_val = in_msg->FSM_Warning_TxOverflow_value;
    const uint32_t FSM_Warning_TxOverflow_raw = CAN_ENCODE(FSM_Warning_TxOverflow_val, CANSIG_FSM_WARNINGS_FSM_WARNING_TX_OVERFLOW_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Warning_TxOverflow_raw, 6, 0x40);   // Packs bits _#______ of byte 0
    
    // Pack 1-bit signal FSM_Warning_RxOverflow into payload (at bit 7 to bit 8).
    const bool FSM_Warning_RxOverflow_val = in_msg->FSM_Warning_RxOverflow_value;
    const uint32_t FSM_Warning_RxOverflow_raw = CAN_ENCODE(FSM_Warning_RxOverflow_val, CANSIG_FSM_WARNINGS_FSM_WARNING_RX_OVERFLOW_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Warning_RxOverflow_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    
    // Pack 1-bit signal FSM_Warning_LeftWheelSpeedOutOfRange into payload (at bit 8 to bit 9).
    const bool FSM_Warning_LeftWheelSpeedOutOfRange_val = in_msg->FSM_Warning_LeftWheelSpeedOutOfRange_value;
    const uint32_t FSM_Warning_LeftWheelSpeedOutOfRange_raw = CAN_ENCODE(FSM_Warning_LeftWheelSpeedOutOfRange_val, CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(FSM_Warning_LeftWheelSpeedOutOfRange_raw, 0, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 1-bit signal FSM_Warning_RightWheelSpeedOutOfRange into payload (at bit 9 to bit 10).
    const bool FSM_Warning_RightWheelSpeedOutOfRange_val = in_msg->FSM_Warning_RightWheelSpeedOutOfRange_value;
    const uint32_t FSM_Warning_RightWheelSpeedOutOfRange_raw = CAN_ENCODE(FSM_Warning_RightWheelSpeedOutOfRange_val, CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Warning_RightWheelSpeedOutOfRange_raw, 1, 0x2);   // Packs bits ______#_ of byte 1
    
    // Pack 1-bit signal FSM_Warning_SteeringAngleOCSC into payload (at bit 10 to bit 11).
    const bool FSM_Warning_SteeringAngleOCSC_val = in_msg->FSM_Warning_SteeringAngleOCSC_value;
    const uint32_t FSM_Warning_SteeringAngleOCSC_raw = CAN_ENCODE(FSM_Warning_SteeringAngleOCSC_val, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OCSC_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OCSC_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Warning_SteeringAngleOCSC_raw, 2, 0x4);   // Packs bits _____#__ of byte 1
    
    // Pack 1-bit signal FSM_Warning_SteeringAngleOutOfRange into payload (at bit 11 to bit 12).
    const bool FSM_Warning_SteeringAngleOutOfRange_val = in_msg->FSM_Warning_SteeringAngleOutOfRange_value;
    const uint32_t FSM_Warning_SteeringAngleOutOfRange_raw = CAN_ENCODE(FSM_Warning_SteeringAngleOutOfRange_val, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Warning_SteeringAngleOutOfRange_raw, 3, 0x8);   // Packs bits ____#___ of byte 1
    
    // Pack 1-bit signal FSM_Warning_FrontBrakePressureOutOfRange into payload (at bit 12 to bit 13).
    const bool FSM_Warning_FrontBrakePressureOutOfRange_val = in_msg->FSM_Warning_FrontBrakePressureOutOfRange_value;
    const uint32_t FSM_Warning_FrontBrakePressureOutOfRange_raw = CAN_ENCODE(FSM_Warning_FrontBrakePressureOutOfRange_val, CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Warning_FrontBrakePressureOutOfRange_raw, 4, 0x10);   // Packs bits ___#____ of byte 1
    
    // Pack 1-bit signal FSM_Warning_RearBrakePressureOutOfRangeWarning into payload (at bit 13 to bit 14).
    const bool FSM_Warning_RearBrakePressureOutOfRangeWarning_val = in_msg->FSM_Warning_RearBrakePressureOutOfRangeWarning_value;
    const uint32_t FSM_Warning_RearBrakePressureOutOfRangeWarning_raw = CAN_ENCODE(FSM_Warning_RearBrakePressureOutOfRangeWarning_val, CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Warning_RearBrakePressureOutOfRangeWarning_raw, 5, 0x20);   // Packs bits __#_____ of byte 1
    
    // Pack 1-bit signal FSM_Warning_FlowRateOutOfRange into payload (at bit 14 to bit 15).
    const bool FSM_Warning_FlowRateOutOfRange_val = in_msg->FSM_Warning_FlowRateOutOfRange_value;
    const uint32_t FSM_Warning_FlowRateOutOfRange_raw = CAN_ENCODE(FSM_Warning_FlowRateOutOfRange_val, CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Warning_FlowRateOutOfRange_raw, 6, 0x40);   // Packs bits _#______ of byte 1
    
    // Pack 1-bit signal FSM_Warning_BrakeAppsDisagreement into payload (at bit 15 to bit 16).
    const bool FSM_Warning_BrakeAppsDisagreement_val = in_msg->FSM_Warning_BrakeAppsDisagreement_value;
    const uint32_t FSM_Warning_BrakeAppsDisagreement_raw = CAN_ENCODE(FSM_Warning_BrakeAppsDisagreement_val, CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Warning_BrakeAppsDisagreement_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    
    // Pack 1-bit signal FSM_Warning_AppsDisagreement into payload (at bit 16 to bit 17).
    const bool FSM_Warning_AppsDisagreement_val = in_msg->FSM_Warning_AppsDisagreement_value;
    const uint32_t FSM_Warning_AppsDisagreement_raw = CAN_ENCODE(FSM_Warning_AppsDisagreement_val, CANSIG_FSM_WARNINGS_FSM_WARNING_APPS_DISAGREEMENT_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_APPS_DISAGREEMENT_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(FSM_Warning_AppsDisagreement_raw, 0, 0x1);   // Packs bits _______# of byte 2
    
}

void App_CanUtils_FSM_Faults_Pack(const FSM_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message FSM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__FEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal FSM_Fault_MissingHeartbeat into payload (at bit 0 to bit 1).
    const bool FSM_Fault_MissingHeartbeat_val = in_msg->FSM_Fault_MissingHeartbeat_value;
    const uint32_t FSM_Fault_MissingHeartbeat_raw = CAN_ENCODE(FSM_Fault_MissingHeartbeat_val, CANSIG_FSM_FAULTS_FSM_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_Fault_MissingHeartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal FSM_Fault_StateMachine into payload (at bit 1 to bit 2).
    const bool FSM_Fault_StateMachine_val = in_msg->FSM_Fault_StateMachine_value;
    const uint32_t FSM_Fault_StateMachine_raw = CAN_ENCODE(FSM_Fault_StateMachine_val, CANSIG_FSM_FAULTS_FSM_FAULT_STATE_MACHINE_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_STATE_MACHINE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Fault_StateMachine_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal FSM_Fault_PappsOCSC into payload (at bit 2 to bit 3).
    const bool FSM_Fault_PappsOCSC_val = in_msg->FSM_Fault_PappsOCSC_value;
    const uint32_t FSM_Fault_PappsOCSC_raw = CAN_ENCODE(FSM_Fault_PappsOCSC_val, CANSIG_FSM_FAULTS_FSM_FAULT_PAPPS_OCSC_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_PAPPS_OCSC_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Fault_PappsOCSC_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal FSM_Fault_SappsOCSCFault into payload (at bit 3 to bit 4).
    const bool FSM_Fault_SappsOCSCFault_val = in_msg->FSM_Fault_SappsOCSCFault_value;
    const uint32_t FSM_Fault_SappsOCSCFault_raw = CAN_ENCODE(FSM_Fault_SappsOCSCFault_val, CANSIG_FSM_FAULTS_FSM_FAULT_SAPPS_OCSC_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_SAPPS_OCSC_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Fault_SappsOCSCFault_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal FSM_Fault_FlowMeterUnderflow into payload (at bit 4 to bit 5).
    const bool FSM_Fault_FlowMeterUnderflow_val = in_msg->FSM_Fault_FlowMeterUnderflow_value;
    const uint32_t FSM_Fault_FlowMeterUnderflow_raw = CAN_ENCODE(FSM_Fault_FlowMeterUnderflow_val, CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_UNDERFLOW_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_UNDERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Fault_FlowMeterUnderflow_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal FSM_Fault_TorquePlausabilityFailed into payload (at bit 5 to bit 6).
    const bool FSM_Fault_TorquePlausabilityFailed_val = in_msg->FSM_Fault_TorquePlausabilityFailed_value;
    const uint32_t FSM_Fault_TorquePlausabilityFailed_raw = CAN_ENCODE(FSM_Fault_TorquePlausabilityFailed_val, CANSIG_FSM_FAULTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Fault_TorquePlausabilityFailed_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
}

void App_CanUtils_FSM_WarningsCounts_Pack(const FSM_WarningsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 7-byte payload for message FSM_WarningsCounts.
    // |xxxxxxxx|_____QQQ|PPPOOONN|NMMMLLLK|KKJJJIII|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal FSM_Warning_StackWaterMarkHighTask1HzCount into payload (at bit 0 to bit 3).
    const uint32_t FSM_Warning_StackWaterMarkHighTask1HzCount_val = in_msg->FSM_Warning_StackWaterMarkHighTask1HzCount_value;
    const uint32_t FSM_Warning_StackWaterMarkHighTask1HzCount_raw = CAN_ENCODE(FSM_Warning_StackWaterMarkHighTask1HzCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_Warning_StackWaterMarkHighTask1HzCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal FSM_Warning_StackWaterMarkHighTask100HzCount into payload (at bit 3 to bit 6).
    const uint32_t FSM_Warning_StackWaterMarkHighTask100HzCount_val = in_msg->FSM_Warning_StackWaterMarkHighTask100HzCount_value;
    const uint32_t FSM_Warning_StackWaterMarkHighTask100HzCount_raw = CAN_ENCODE(FSM_Warning_StackWaterMarkHighTask100HzCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Warning_StackWaterMarkHighTask100HzCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal FSM_Warning_StackWaterMarkHighTask1kHzCount into payload (at bit 6 to bit 9).
    const uint32_t FSM_Warning_StackWaterMarkHighTask1kHzCount_val = in_msg->FSM_Warning_StackWaterMarkHighTask1kHzCount_value;
    const uint32_t FSM_Warning_StackWaterMarkHighTask1kHzCount_raw = CAN_ENCODE(FSM_Warning_StackWaterMarkHighTask1kHzCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Warning_StackWaterMarkHighTask1kHzCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(FSM_Warning_StackWaterMarkHighTask1kHzCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 3-bit signal FSM_Warning_StackWaterMarkHighTaskCanRxCount into payload (at bit 9 to bit 12).
    const uint32_t FSM_Warning_StackWaterMarkHighTaskCanRxCount_val = in_msg->FSM_Warning_StackWaterMarkHighTaskCanRxCount_value;
    const uint32_t FSM_Warning_StackWaterMarkHighTaskCanRxCount_raw = CAN_ENCODE(FSM_Warning_StackWaterMarkHighTaskCanRxCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Warning_StackWaterMarkHighTaskCanRxCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 1
    
    // Pack 3-bit signal FSM_Warning_StackWaterMarkHighTaskCanTxCount into payload (at bit 12 to bit 15).
    const uint32_t FSM_Warning_StackWaterMarkHighTaskCanTxCount_val = in_msg->FSM_Warning_StackWaterMarkHighTaskCanTxCount_value;
    const uint32_t FSM_Warning_StackWaterMarkHighTaskCanTxCount_raw = CAN_ENCODE(FSM_Warning_StackWaterMarkHighTaskCanTxCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Warning_StackWaterMarkHighTaskCanTxCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1
    
    // Pack 3-bit signal FSM_Warning_WatchdogTimeoutCount into payload (at bit 15 to bit 18).
    const uint32_t FSM_Warning_WatchdogTimeoutCount_val = in_msg->FSM_Warning_WatchdogTimeoutCount_value;
    const uint32_t FSM_Warning_WatchdogTimeoutCount_raw = CAN_ENCODE(FSM_Warning_WatchdogTimeoutCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Warning_WatchdogTimeoutCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    out_data[2] |= packShiftRight(FSM_Warning_WatchdogTimeoutCount_raw, 1, 0x3);   // Packs bits ______## of byte 2
    
    // Pack 3-bit signal FSM_Warning_TxOverflowCount into payload (at bit 18 to bit 21).
    const uint32_t FSM_Warning_TxOverflowCount_val = in_msg->FSM_Warning_TxOverflowCount_value;
    const uint32_t FSM_Warning_TxOverflowCount_raw = CAN_ENCODE(FSM_Warning_TxOverflowCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_TX_OVERFLOW_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(FSM_Warning_TxOverflowCount_raw, 2, 0x1c);   // Packs bits ___###__ of byte 2
    
    // Pack 3-bit signal FSM_Warning_RxOverflowCount into payload (at bit 21 to bit 24).
    const uint32_t FSM_Warning_RxOverflowCount_val = in_msg->FSM_Warning_RxOverflowCount_value;
    const uint32_t FSM_Warning_RxOverflowCount_raw = CAN_ENCODE(FSM_Warning_RxOverflowCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_RX_OVERFLOW_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(FSM_Warning_RxOverflowCount_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    
    // Pack 3-bit signal FSM_Warning_LeftWheelSpeedOutOfRangeCount into payload (at bit 24 to bit 27).
    const uint32_t FSM_Warning_LeftWheelSpeedOutOfRangeCount_val = in_msg->FSM_Warning_LeftWheelSpeedOutOfRangeCount_value;
    const uint32_t FSM_Warning_LeftWheelSpeedOutOfRangeCount_raw = CAN_ENCODE(FSM_Warning_LeftWheelSpeedOutOfRangeCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftRight(FSM_Warning_LeftWheelSpeedOutOfRangeCount_raw, 0, 0x7);   // Packs bits _____### of byte 3
    
    // Pack 3-bit signal FSM_Warning_RightWheelSpeedOutOfRangeCount into payload (at bit 27 to bit 30).
    const uint32_t FSM_Warning_RightWheelSpeedOutOfRangeCount_val = in_msg->FSM_Warning_RightWheelSpeedOutOfRangeCount_value;
    const uint32_t FSM_Warning_RightWheelSpeedOutOfRangeCount_raw = CAN_ENCODE(FSM_Warning_RightWheelSpeedOutOfRangeCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(FSM_Warning_RightWheelSpeedOutOfRangeCount_raw, 3, 0x38);   // Packs bits __###___ of byte 3
    
    // Pack 3-bit signal FSM_Warning_SteeringAngleOCSCCount into payload (at bit 30 to bit 33).
    const uint32_t FSM_Warning_SteeringAngleOCSCCount_val = in_msg->FSM_Warning_SteeringAngleOCSCCount_value;
    const uint32_t FSM_Warning_SteeringAngleOCSCCount_raw = CAN_ENCODE(FSM_Warning_SteeringAngleOCSCCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(FSM_Warning_SteeringAngleOCSCCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 3
    out_data[4] |= packShiftRight(FSM_Warning_SteeringAngleOCSCCount_raw, 2, 0x1);   // Packs bits _______# of byte 4
    
    // Pack 3-bit signal FSM_Warning_SteeringAngleOutOfRangeCount into payload (at bit 33 to bit 36).
    const uint32_t FSM_Warning_SteeringAngleOutOfRangeCount_val = in_msg->FSM_Warning_SteeringAngleOutOfRangeCount_value;
    const uint32_t FSM_Warning_SteeringAngleOutOfRangeCount_raw = CAN_ENCODE(FSM_Warning_SteeringAngleOutOfRangeCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(FSM_Warning_SteeringAngleOutOfRangeCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 4
    
    // Pack 3-bit signal FSM_Warning_FrontBrakePressureOutOfRangeCount into payload (at bit 36 to bit 39).
    const uint32_t FSM_Warning_FrontBrakePressureOutOfRangeCount_val = in_msg->FSM_Warning_FrontBrakePressureOutOfRangeCount_value;
    const uint32_t FSM_Warning_FrontBrakePressureOutOfRangeCount_raw = CAN_ENCODE(FSM_Warning_FrontBrakePressureOutOfRangeCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(FSM_Warning_FrontBrakePressureOutOfRangeCount_raw, 4, 0x70);   // Packs bits _###____ of byte 4
    
    // Pack 3-bit signal FSM_Warning_RearBrakePressureOutOfRangeWarningCount into payload (at bit 39 to bit 42).
    const uint32_t FSM_Warning_RearBrakePressureOutOfRangeWarningCount_val = in_msg->FSM_Warning_RearBrakePressureOutOfRangeWarningCount_value;
    const uint32_t FSM_Warning_RearBrakePressureOutOfRangeWarningCount_raw = CAN_ENCODE(FSM_Warning_RearBrakePressureOutOfRangeWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(FSM_Warning_RearBrakePressureOutOfRangeWarningCount_raw, 7, 0x80);   // Packs bits #_______ of byte 4
    out_data[5] |= packShiftRight(FSM_Warning_RearBrakePressureOutOfRangeWarningCount_raw, 1, 0x3);   // Packs bits ______## of byte 5
    
    // Pack 3-bit signal FSM_Warning_FlowRateOutOfRangeCount into payload (at bit 42 to bit 45).
    const uint32_t FSM_Warning_FlowRateOutOfRangeCount_val = in_msg->FSM_Warning_FlowRateOutOfRangeCount_value;
    const uint32_t FSM_Warning_FlowRateOutOfRangeCount_raw = CAN_ENCODE(FSM_Warning_FlowRateOutOfRangeCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_COUNT_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(FSM_Warning_FlowRateOutOfRangeCount_raw, 2, 0x1c);   // Packs bits ___###__ of byte 5
    
    // Pack 3-bit signal FSM_Warning_BrakeAppsDisagreementCount into payload (at bit 45 to bit 48).
    const uint32_t FSM_Warning_BrakeAppsDisagreementCount_val = in_msg->FSM_Warning_BrakeAppsDisagreementCount_value;
    const uint32_t FSM_Warning_BrakeAppsDisagreementCount_raw = CAN_ENCODE(FSM_Warning_BrakeAppsDisagreementCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(FSM_Warning_BrakeAppsDisagreementCount_raw, 5, 0xe0);   // Packs bits ###_____ of byte 5
    
    // Pack 3-bit signal FSM_Warning_AppsDisagreementCount into payload (at bit 48 to bit 51).
    const uint32_t FSM_Warning_AppsDisagreementCount_val = in_msg->FSM_Warning_AppsDisagreementCount_value;
    const uint32_t FSM_Warning_AppsDisagreementCount_raw = CAN_ENCODE(FSM_Warning_AppsDisagreementCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_APPS_DISAGREEMENT_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WARNING_APPS_DISAGREEMENT_COUNT_OFFSET, uint32_t);
    out_data[6] |= packShiftRight(FSM_Warning_AppsDisagreementCount_raw, 0, 0x7);   // Packs bits _____### of byte 6
    
}

void App_CanUtils_FSM_FaultsCounts_Pack(const FSM_FaultsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message FSM_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______FF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal FSM_Fault_MissingHeartbeatCount into payload (at bit 0 to bit 3).
    const uint32_t FSM_Fault_MissingHeartbeatCount_val = in_msg->FSM_Fault_MissingHeartbeatCount_value;
    const uint32_t FSM_Fault_MissingHeartbeatCount_raw = CAN_ENCODE(FSM_Fault_MissingHeartbeatCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_MISSING_HEARTBEAT_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_MISSING_HEARTBEAT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_Fault_MissingHeartbeatCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal FSM_Fault_StateMachineCount into payload (at bit 3 to bit 6).
    const uint32_t FSM_Fault_StateMachineCount_val = in_msg->FSM_Fault_StateMachineCount_value;
    const uint32_t FSM_Fault_StateMachineCount_raw = CAN_ENCODE(FSM_Fault_StateMachineCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_STATE_MACHINE_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_STATE_MACHINE_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Fault_StateMachineCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal FSM_Fault_PappsOCSCCount into payload (at bit 6 to bit 9).
    const uint32_t FSM_Fault_PappsOCSCCount_val = in_msg->FSM_Fault_PappsOCSCCount_value;
    const uint32_t FSM_Fault_PappsOCSCCount_raw = CAN_ENCODE(FSM_Fault_PappsOCSCCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_PAPPS_OCSC_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_PAPPS_OCSC_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_Fault_PappsOCSCCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(FSM_Fault_PappsOCSCCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 3-bit signal FSM_Fault_SappsOCSCFaultCount into payload (at bit 9 to bit 12).
    const uint32_t FSM_Fault_SappsOCSCFaultCount_val = in_msg->FSM_Fault_SappsOCSCFaultCount_value;
    const uint32_t FSM_Fault_SappsOCSCFaultCount_raw = CAN_ENCODE(FSM_Fault_SappsOCSCFaultCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_SAPPS_OCSC_FAULT_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_SAPPS_OCSC_FAULT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Fault_SappsOCSCFaultCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 1
    
    // Pack 3-bit signal FSM_Fault_FlowMeterUnderflowCount into payload (at bit 12 to bit 15).
    const uint32_t FSM_Fault_FlowMeterUnderflowCount_val = in_msg->FSM_Fault_FlowMeterUnderflowCount_value;
    const uint32_t FSM_Fault_FlowMeterUnderflowCount_raw = CAN_ENCODE(FSM_Fault_FlowMeterUnderflowCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_FLOW_METER_UNDERFLOW_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_FLOW_METER_UNDERFLOW_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Fault_FlowMeterUnderflowCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1
    
    // Pack 3-bit signal FSM_Fault_TorquePlausabilityFailedCount into payload (at bit 15 to bit 18).
    const uint32_t FSM_Fault_TorquePlausabilityFailedCount_val = in_msg->FSM_Fault_TorquePlausabilityFailedCount_value;
    const uint32_t FSM_Fault_TorquePlausabilityFailedCount_raw = CAN_ENCODE(FSM_Fault_TorquePlausabilityFailedCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_FAULT_TORQUE_PLAUSABILITY_FAILED_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_Fault_TorquePlausabilityFailedCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    out_data[2] |= packShiftRight(FSM_Fault_TorquePlausabilityFailedCount_raw, 1, 0x3);   // Packs bits ______## of byte 2
    
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

void App_CanUtils_DCM_LeftInverterCommand_Unpack(const uint8_t* const in_data, DCM_LeftInverterCommand_Signals* const out_msg)
{
    // Unpack 8-byte payload for message DCM_LeftInverterCommand.
    // |GGGGGGGG|GGGGGGGG|_____FED|_______C|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 16-bit signal DCM_LeftInverterTorqueCommand from payload (at bit 0 to bit 16).
    uint32_t DCM_LeftInverterTorqueCommand_raw = 0;
    DCM_LeftInverterTorqueCommand_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    DCM_LeftInverterTorqueCommand_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    const int DCM_LeftInverterTorqueCommand_signed = SIGNED_DECODE(DCM_LeftInverterTorqueCommand_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float DCM_LeftInverterTorqueCommand_val = CAN_DECODE(DCM_LeftInverterTorqueCommand_signed, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_OFFSET, float);
    out_msg->DCM_LeftInverterTorqueCommand_value = DCM_LeftInverterTorqueCommand_val;
    
    // Unpack 16-bit signal DCM_LeftInverterSpeedCommand from payload (at bit 16 to bit 32).
    uint32_t DCM_LeftInverterSpeedCommand_raw = 0;
    DCM_LeftInverterSpeedCommand_raw |= unpackShiftRight(in_data[2], 0, 0xff);   // Unpacks bits ######## of msg byte 2
    DCM_LeftInverterSpeedCommand_raw |= unpackShiftLeft(in_data[3], 8, 0xff);   // Unpacks bits ######## of msg byte 3
    const int DCM_LeftInverterSpeedCommand_signed = SIGNED_DECODE(DCM_LeftInverterSpeedCommand_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    int DCM_LeftInverterSpeedCommand_val = CAN_DECODE(DCM_LeftInverterSpeedCommand_signed, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_OFFSET, int);
    out_msg->DCM_LeftInverterSpeedCommand_value = DCM_LeftInverterSpeedCommand_val;
    
    // Unpack 1-bit signal DCM_LeftInverterDirectionCommand from payload (at bit 32 to bit 33).
    uint32_t DCM_LeftInverterDirectionCommand_raw = 0;
    DCM_LeftInverterDirectionCommand_raw |= unpackShiftRight(in_data[4], 0, 0x1);   // Unpacks bits _______# of msg byte 4
    InverterDirection DCM_LeftInverterDirectionCommand_val = CAN_DECODE(DCM_LeftInverterDirectionCommand_raw, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_DIRECTION_COMMAND_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_msg->DCM_LeftInverterDirectionCommand_value = DCM_LeftInverterDirectionCommand_val;
    
    // Unpack 1-bit signal DCM_LeftInverterEnable from payload (at bit 40 to bit 41).
    uint32_t DCM_LeftInverterEnable_raw = 0;
    DCM_LeftInverterEnable_raw |= unpackShiftRight(in_data[5], 0, 0x1);   // Unpacks bits _______# of msg byte 5
    bool DCM_LeftInverterEnable_val = CAN_DECODE(DCM_LeftInverterEnable_raw, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_OFFSET, uint32_t);
    out_msg->DCM_LeftInverterEnable_value = DCM_LeftInverterEnable_val;
    
    // Unpack 1-bit signal DCM_LeftInverterEnableDischarge from payload (at bit 41 to bit 42).
    uint32_t DCM_LeftInverterEnableDischarge_raw = 0;
    DCM_LeftInverterEnableDischarge_raw |= unpackShiftRight(in_data[5], 1, 0x2);   // Unpacks bits ______#_ of msg byte 5
    bool DCM_LeftInverterEnableDischarge_val = CAN_DECODE(DCM_LeftInverterEnableDischarge_raw, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_DISCHARGE_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_DISCHARGE_OFFSET, uint32_t);
    out_msg->DCM_LeftInverterEnableDischarge_value = DCM_LeftInverterEnableDischarge_val;
    
    // Unpack 1-bit signal DCM_LeftInverterEnableSpeedMode from payload (at bit 42 to bit 43).
    uint32_t DCM_LeftInverterEnableSpeedMode_raw = 0;
    DCM_LeftInverterEnableSpeedMode_raw |= unpackShiftRight(in_data[5], 2, 0x4);   // Unpacks bits _____#__ of msg byte 5
    bool DCM_LeftInverterEnableSpeedMode_val = CAN_DECODE(DCM_LeftInverterEnableSpeedMode_raw, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_SPEED_MODE_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_SPEED_MODE_OFFSET, uint32_t);
    out_msg->DCM_LeftInverterEnableSpeedMode_value = DCM_LeftInverterEnableSpeedMode_val;
    
    // Unpack 16-bit signal DCM_LeftInverterTorqueLimit from payload (at bit 48 to bit 64).
    uint32_t DCM_LeftInverterTorqueLimit_raw = 0;
    DCM_LeftInverterTorqueLimit_raw |= unpackShiftRight(in_data[6], 0, 0xff);   // Unpacks bits ######## of msg byte 6
    DCM_LeftInverterTorqueLimit_raw |= unpackShiftLeft(in_data[7], 8, 0xff);   // Unpacks bits ######## of msg byte 7
    const int DCM_LeftInverterTorqueLimit_signed = SIGNED_DECODE(DCM_LeftInverterTorqueLimit_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float DCM_LeftInverterTorqueLimit_val = CAN_DECODE(DCM_LeftInverterTorqueLimit_signed, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_OFFSET, float);
    out_msg->DCM_LeftInverterTorqueLimit_value = DCM_LeftInverterTorqueLimit_val;
    
}

void App_CanUtils_DCM_RightInverterCommand_Unpack(const uint8_t* const in_data, DCM_RightInverterCommand_Signals* const out_msg)
{
    // Unpack 8-byte payload for message DCM_RightInverterCommand.
    // |GGGGGGGG|GGGGGGGG|_____FED|_______C|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 16-bit signal DCM_RightInverterTorqueCommand from payload (at bit 0 to bit 16).
    uint32_t DCM_RightInverterTorqueCommand_raw = 0;
    DCM_RightInverterTorqueCommand_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    DCM_RightInverterTorqueCommand_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    const int DCM_RightInverterTorqueCommand_signed = SIGNED_DECODE(DCM_RightInverterTorqueCommand_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float DCM_RightInverterTorqueCommand_val = CAN_DECODE(DCM_RightInverterTorqueCommand_signed, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_OFFSET, float);
    out_msg->DCM_RightInverterTorqueCommand_value = DCM_RightInverterTorqueCommand_val;
    
    // Unpack 16-bit signal DCM_RightInverterSpeedCommand from payload (at bit 16 to bit 32).
    uint32_t DCM_RightInverterSpeedCommand_raw = 0;
    DCM_RightInverterSpeedCommand_raw |= unpackShiftRight(in_data[2], 0, 0xff);   // Unpacks bits ######## of msg byte 2
    DCM_RightInverterSpeedCommand_raw |= unpackShiftLeft(in_data[3], 8, 0xff);   // Unpacks bits ######## of msg byte 3
    const int DCM_RightInverterSpeedCommand_signed = SIGNED_DECODE(DCM_RightInverterSpeedCommand_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    int DCM_RightInverterSpeedCommand_val = CAN_DECODE(DCM_RightInverterSpeedCommand_signed, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_OFFSET, int);
    out_msg->DCM_RightInverterSpeedCommand_value = DCM_RightInverterSpeedCommand_val;
    
    // Unpack 1-bit signal DCM_RightInverterDirectionCommand from payload (at bit 32 to bit 33).
    uint32_t DCM_RightInverterDirectionCommand_raw = 0;
    DCM_RightInverterDirectionCommand_raw |= unpackShiftRight(in_data[4], 0, 0x1);   // Unpacks bits _______# of msg byte 4
    InverterDirection DCM_RightInverterDirectionCommand_val = CAN_DECODE(DCM_RightInverterDirectionCommand_raw, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_DIRECTION_COMMAND_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_msg->DCM_RightInverterDirectionCommand_value = DCM_RightInverterDirectionCommand_val;
    
    // Unpack 1-bit signal DCM_RightInverterEnable from payload (at bit 40 to bit 41).
    uint32_t DCM_RightInverterEnable_raw = 0;
    DCM_RightInverterEnable_raw |= unpackShiftRight(in_data[5], 0, 0x1);   // Unpacks bits _______# of msg byte 5
    bool DCM_RightInverterEnable_val = CAN_DECODE(DCM_RightInverterEnable_raw, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_OFFSET, uint32_t);
    out_msg->DCM_RightInverterEnable_value = DCM_RightInverterEnable_val;
    
    // Unpack 1-bit signal DCM_RightInverterEnableDischarge from payload (at bit 41 to bit 42).
    uint32_t DCM_RightInverterEnableDischarge_raw = 0;
    DCM_RightInverterEnableDischarge_raw |= unpackShiftRight(in_data[5], 1, 0x2);   // Unpacks bits ______#_ of msg byte 5
    bool DCM_RightInverterEnableDischarge_val = CAN_DECODE(DCM_RightInverterEnableDischarge_raw, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_DISCHARGE_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_DISCHARGE_OFFSET, uint32_t);
    out_msg->DCM_RightInverterEnableDischarge_value = DCM_RightInverterEnableDischarge_val;
    
    // Unpack 1-bit signal DCM_RightInverterEnableSpeedMode from payload (at bit 42 to bit 43).
    uint32_t DCM_RightInverterEnableSpeedMode_raw = 0;
    DCM_RightInverterEnableSpeedMode_raw |= unpackShiftRight(in_data[5], 2, 0x4);   // Unpacks bits _____#__ of msg byte 5
    bool DCM_RightInverterEnableSpeedMode_val = CAN_DECODE(DCM_RightInverterEnableSpeedMode_raw, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_SPEED_MODE_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_SPEED_MODE_OFFSET, uint32_t);
    out_msg->DCM_RightInverterEnableSpeedMode_value = DCM_RightInverterEnableSpeedMode_val;
    
    // Unpack 16-bit signal DCM_RightInverterTorqueLimit from payload (at bit 48 to bit 64).
    uint32_t DCM_RightInverterTorqueLimit_raw = 0;
    DCM_RightInverterTorqueLimit_raw |= unpackShiftRight(in_data[6], 0, 0xff);   // Unpacks bits ######## of msg byte 6
    DCM_RightInverterTorqueLimit_raw |= unpackShiftLeft(in_data[7], 8, 0xff);   // Unpacks bits ######## of msg byte 7
    const int DCM_RightInverterTorqueLimit_signed = SIGNED_DECODE(DCM_RightInverterTorqueLimit_raw, 16, 0xffff); // Value is signed with length 16, decode via 2s complement
    float DCM_RightInverterTorqueLimit_val = CAN_DECODE(DCM_RightInverterTorqueLimit_signed, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_OFFSET, float);
    out_msg->DCM_RightInverterTorqueLimit_value = DCM_RightInverterTorqueLimit_val;
    
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

