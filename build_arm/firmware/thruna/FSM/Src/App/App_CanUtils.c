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

void App_CanUtils_FSM_StackRemaining_Pack(const FSM_StackRemaining_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message FSM_StackRemaining.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_____EEE|EEEEDDDD|DDDCCCCC|CCBBBBBB|BAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 7-bit signal FSM_StackRemainingTask1Hz into payload (at bit 0 to bit 7).
    const uint32_t FSM_StackRemainingTask1Hz_val = in_msg->FSM_StackRemainingTask1Hz_value;
    const uint32_t FSM_StackRemainingTask1Hz_raw = CAN_ENCODE(FSM_StackRemainingTask1Hz_val, CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK1_HZ_SCALE, CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_StackRemainingTask1Hz_raw, 0, 0x7f);   // Packs bits _####### of byte 0
    
    // Pack 7-bit signal FSM_StackRemainingTask100Hz into payload (at bit 7 to bit 14).
    const uint32_t FSM_StackRemainingTask100Hz_val = in_msg->FSM_StackRemainingTask100Hz_value;
    const uint32_t FSM_StackRemainingTask100Hz_raw = CAN_ENCODE(FSM_StackRemainingTask100Hz_val, CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK100_HZ_SCALE, CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_StackRemainingTask100Hz_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    out_data[1] |= packShiftRight(FSM_StackRemainingTask100Hz_raw, 1, 0x3f);   // Packs bits __###### of byte 1
    
    // Pack 7-bit signal FSM_StackRemainingTask1kHz into payload (at bit 14 to bit 21).
    const uint32_t FSM_StackRemainingTask1kHz_val = in_msg->FSM_StackRemainingTask1kHz_value;
    const uint32_t FSM_StackRemainingTask1kHz_raw = CAN_ENCODE(FSM_StackRemainingTask1kHz_val, CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK1K_HZ_SCALE, CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK1K_HZ_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_StackRemainingTask1kHz_raw, 6, 0xc0);   // Packs bits ##______ of byte 1
    out_data[2] |= packShiftRight(FSM_StackRemainingTask1kHz_raw, 2, 0x1f);   // Packs bits ___##### of byte 2
    
    // Pack 7-bit signal FSM_StackRemainingTaskCanRx into payload (at bit 21 to bit 28).
    const uint32_t FSM_StackRemainingTaskCanRx_val = in_msg->FSM_StackRemainingTaskCanRx_value;
    const uint32_t FSM_StackRemainingTaskCanRx_raw = CAN_ENCODE(FSM_StackRemainingTaskCanRx_val, CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK_CAN_RX_SCALE, CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK_CAN_RX_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(FSM_StackRemainingTaskCanRx_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    out_data[3] |= packShiftRight(FSM_StackRemainingTaskCanRx_raw, 3, 0xf);   // Packs bits ____#### of byte 3
    
    // Pack 7-bit signal FSM_StackRemainingTaskCanTx into payload (at bit 28 to bit 35).
    const uint32_t FSM_StackRemainingTaskCanTx_val = in_msg->FSM_StackRemainingTaskCanTx_value;
    const uint32_t FSM_StackRemainingTaskCanTx_raw = CAN_ENCODE(FSM_StackRemainingTaskCanTx_val, CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK_CAN_TX_SCALE, CANSIG_FSM_STACK_REMAINING_FSM_STACK_REMAINING_TASK_CAN_TX_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(FSM_StackRemainingTaskCanTx_raw, 4, 0xf0);   // Packs bits ####____ of byte 3
    out_data[4] |= packShiftRight(FSM_StackRemainingTaskCanTx_raw, 4, 0x7);   // Packs bits _____### of byte 4
    
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
    
    // Pack 1-bit signal FSM_StackWaterMarkHighTask1HzWarning into payload (at bit 0 to bit 1).
    const bool FSM_StackWaterMarkHighTask1HzWarning_val = in_msg->FSM_StackWaterMarkHighTask1HzWarning_value;
    const uint32_t FSM_StackWaterMarkHighTask1HzWarning_raw = CAN_ENCODE(FSM_StackWaterMarkHighTask1HzWarning_val, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_StackWaterMarkHighTask1HzWarning_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal FSM_StackWaterMarkHighTask100HzWarning into payload (at bit 1 to bit 2).
    const bool FSM_StackWaterMarkHighTask100HzWarning_val = in_msg->FSM_StackWaterMarkHighTask100HzWarning_value;
    const uint32_t FSM_StackWaterMarkHighTask100HzWarning_raw = CAN_ENCODE(FSM_StackWaterMarkHighTask100HzWarning_val, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_StackWaterMarkHighTask100HzWarning_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal FSM_StackWaterMarkHighTask1kHzWarning into payload (at bit 2 to bit 3).
    const bool FSM_StackWaterMarkHighTask1kHzWarning_val = in_msg->FSM_StackWaterMarkHighTask1kHzWarning_value;
    const uint32_t FSM_StackWaterMarkHighTask1kHzWarning_raw = CAN_ENCODE(FSM_StackWaterMarkHighTask1kHzWarning_val, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_StackWaterMarkHighTask1kHzWarning_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal FSM_StackWaterMarkHighTaskCanRxWarning into payload (at bit 3 to bit 4).
    const bool FSM_StackWaterMarkHighTaskCanRxWarning_val = in_msg->FSM_StackWaterMarkHighTaskCanRxWarning_value;
    const uint32_t FSM_StackWaterMarkHighTaskCanRxWarning_raw = CAN_ENCODE(FSM_StackWaterMarkHighTaskCanRxWarning_val, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_StackWaterMarkHighTaskCanRxWarning_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal FSM_StackWaterMarkHighTaskCanTxWarning into payload (at bit 4 to bit 5).
    const bool FSM_StackWaterMarkHighTaskCanTxWarning_val = in_msg->FSM_StackWaterMarkHighTaskCanTxWarning_value;
    const uint32_t FSM_StackWaterMarkHighTaskCanTxWarning_raw = CAN_ENCODE(FSM_StackWaterMarkHighTaskCanTxWarning_val, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_StackWaterMarkHighTaskCanTxWarning_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal FSM_WatchdogTimeoutWarning into payload (at bit 5 to bit 6).
    const bool FSM_WatchdogTimeoutWarning_val = in_msg->FSM_WatchdogTimeoutWarning_value;
    const uint32_t FSM_WatchdogTimeoutWarning_raw = CAN_ENCODE(FSM_WatchdogTimeoutWarning_val, CANSIG_FSM_WARNINGS_FSM_WATCHDOG_TIMEOUT_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_WATCHDOG_TIMEOUT_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_WatchdogTimeoutWarning_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 1-bit signal FSM_TxOverflowWarning into payload (at bit 6 to bit 7).
    const bool FSM_TxOverflowWarning_val = in_msg->FSM_TxOverflowWarning_value;
    const uint32_t FSM_TxOverflowWarning_raw = CAN_ENCODE(FSM_TxOverflowWarning_val, CANSIG_FSM_WARNINGS_FSM_TX_OVERFLOW_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_TX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_TxOverflowWarning_raw, 6, 0x40);   // Packs bits _#______ of byte 0
    
    // Pack 1-bit signal FSM_RxOverflowWarning into payload (at bit 7 to bit 8).
    const bool FSM_RxOverflowWarning_val = in_msg->FSM_RxOverflowWarning_value;
    const uint32_t FSM_RxOverflowWarning_raw = CAN_ENCODE(FSM_RxOverflowWarning_val, CANSIG_FSM_WARNINGS_FSM_RX_OVERFLOW_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_RX_OVERFLOW_WARNING_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_RxOverflowWarning_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    
    // Pack 1-bit signal FSM_LeftWheelSpeedOutOfRangeWarning into payload (at bit 8 to bit 9).
    const bool FSM_LeftWheelSpeedOutOfRangeWarning_val = in_msg->FSM_LeftWheelSpeedOutOfRangeWarning_value;
    const uint32_t FSM_LeftWheelSpeedOutOfRangeWarning_raw = CAN_ENCODE(FSM_LeftWheelSpeedOutOfRangeWarning_val, CANSIG_FSM_WARNINGS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(FSM_LeftWheelSpeedOutOfRangeWarning_raw, 0, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 1-bit signal FSM_RightWheelSpeedOutOfRangeWarning into payload (at bit 9 to bit 10).
    const bool FSM_RightWheelSpeedOutOfRangeWarning_val = in_msg->FSM_RightWheelSpeedOutOfRangeWarning_value;
    const uint32_t FSM_RightWheelSpeedOutOfRangeWarning_raw = CAN_ENCODE(FSM_RightWheelSpeedOutOfRangeWarning_val, CANSIG_FSM_WARNINGS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_RightWheelSpeedOutOfRangeWarning_raw, 1, 0x2);   // Packs bits ______#_ of byte 1
    
    // Pack 1-bit signal FSM_SteeringAngleOCSCWarning into payload (at bit 10 to bit 11).
    const bool FSM_SteeringAngleOCSCWarning_val = in_msg->FSM_SteeringAngleOCSCWarning_value;
    const uint32_t FSM_SteeringAngleOCSCWarning_raw = CAN_ENCODE(FSM_SteeringAngleOCSCWarning_val, CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OCSC_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OCSC_WARNING_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_SteeringAngleOCSCWarning_raw, 2, 0x4);   // Packs bits _____#__ of byte 1
    
    // Pack 1-bit signal FSM_SteeringAngleOutOfRangeWarning into payload (at bit 11 to bit 12).
    const bool FSM_SteeringAngleOutOfRangeWarning_val = in_msg->FSM_SteeringAngleOutOfRangeWarning_value;
    const uint32_t FSM_SteeringAngleOutOfRangeWarning_raw = CAN_ENCODE(FSM_SteeringAngleOutOfRangeWarning_val, CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_SteeringAngleOutOfRangeWarning_raw, 3, 0x8);   // Packs bits ____#___ of byte 1
    
    // Pack 1-bit signal FSM_FrontBrakePressureOutOfRangeWarning into payload (at bit 12 to bit 13).
    const bool FSM_FrontBrakePressureOutOfRangeWarning_val = in_msg->FSM_FrontBrakePressureOutOfRangeWarning_value;
    const uint32_t FSM_FrontBrakePressureOutOfRangeWarning_raw = CAN_ENCODE(FSM_FrontBrakePressureOutOfRangeWarning_val, CANSIG_FSM_WARNINGS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_FrontBrakePressureOutOfRangeWarning_raw, 4, 0x10);   // Packs bits ___#____ of byte 1
    
    // Pack 1-bit signal FSM_RearBrakePressureOutOfRangeWarning into payload (at bit 13 to bit 14).
    const bool FSM_RearBrakePressureOutOfRangeWarning_val = in_msg->FSM_RearBrakePressureOutOfRangeWarning_value;
    const uint32_t FSM_RearBrakePressureOutOfRangeWarning_raw = CAN_ENCODE(FSM_RearBrakePressureOutOfRangeWarning_val, CANSIG_FSM_WARNINGS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_RearBrakePressureOutOfRangeWarning_raw, 5, 0x20);   // Packs bits __#_____ of byte 1
    
    // Pack 1-bit signal FSM_FlowRateOutOfRangeWarning into payload (at bit 14 to bit 15).
    const bool FSM_FlowRateOutOfRangeWarning_val = in_msg->FSM_FlowRateOutOfRangeWarning_value;
    const uint32_t FSM_FlowRateOutOfRangeWarning_raw = CAN_ENCODE(FSM_FlowRateOutOfRangeWarning_val, CANSIG_FSM_WARNINGS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_FlowRateOutOfRangeWarning_raw, 6, 0x40);   // Packs bits _#______ of byte 1
    
    // Pack 1-bit signal FSM_BrakeAppsDisagreementWarning into payload (at bit 15 to bit 16).
    const bool FSM_BrakeAppsDisagreementWarning_val = in_msg->FSM_BrakeAppsDisagreementWarning_value;
    const uint32_t FSM_BrakeAppsDisagreementWarning_raw = CAN_ENCODE(FSM_BrakeAppsDisagreementWarning_val, CANSIG_FSM_WARNINGS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_BrakeAppsDisagreementWarning_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    
    // Pack 1-bit signal FSM_AppsDisagreementWarning into payload (at bit 16 to bit 17).
    const bool FSM_AppsDisagreementWarning_val = in_msg->FSM_AppsDisagreementWarning_value;
    const uint32_t FSM_AppsDisagreementWarning_raw = CAN_ENCODE(FSM_AppsDisagreementWarning_val, CANSIG_FSM_WARNINGS_FSM_APPS_DISAGREEMENT_WARNING_SCALE, CANSIG_FSM_WARNINGS_FSM_APPS_DISAGREEMENT_WARNING_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(FSM_AppsDisagreementWarning_raw, 0, 0x1);   // Packs bits _______# of byte 2
    
}

void App_CanUtils_FSM_Faults_Pack(const FSM_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message FSM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__FEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal FSM_MissingHeartbeatFault into payload (at bit 0 to bit 1).
    const bool FSM_MissingHeartbeatFault_val = in_msg->FSM_MissingHeartbeatFault_value;
    const uint32_t FSM_MissingHeartbeatFault_raw = CAN_ENCODE(FSM_MissingHeartbeatFault_val, CANSIG_FSM_FAULTS_FSM_MISSING_HEARTBEAT_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_MISSING_HEARTBEAT_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_MissingHeartbeatFault_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal FSM_StateMachineFault into payload (at bit 1 to bit 2).
    const bool FSM_StateMachineFault_val = in_msg->FSM_StateMachineFault_value;
    const uint32_t FSM_StateMachineFault_raw = CAN_ENCODE(FSM_StateMachineFault_val, CANSIG_FSM_FAULTS_FSM_STATE_MACHINE_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_STATE_MACHINE_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_StateMachineFault_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal FSM_PappsOCSCFault into payload (at bit 2 to bit 3).
    const bool FSM_PappsOCSCFault_val = in_msg->FSM_PappsOCSCFault_value;
    const uint32_t FSM_PappsOCSCFault_raw = CAN_ENCODE(FSM_PappsOCSCFault_val, CANSIG_FSM_FAULTS_FSM_PAPPS_OCSC_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_PAPPS_OCSC_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_PappsOCSCFault_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal FSM_SappsOCSCFault into payload (at bit 3 to bit 4).
    const bool FSM_SappsOCSCFault_val = in_msg->FSM_SappsOCSCFault_value;
    const uint32_t FSM_SappsOCSCFault_raw = CAN_ENCODE(FSM_SappsOCSCFault_val, CANSIG_FSM_FAULTS_FSM_SAPPS_OCSC_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_SAPPS_OCSC_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_SappsOCSCFault_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal FSM_FlowMeterUnderflowFault into payload (at bit 4 to bit 5).
    const bool FSM_FlowMeterUnderflowFault_val = in_msg->FSM_FlowMeterUnderflowFault_value;
    const uint32_t FSM_FlowMeterUnderflowFault_raw = CAN_ENCODE(FSM_FlowMeterUnderflowFault_val, CANSIG_FSM_FAULTS_FSM_FLOW_METER_UNDERFLOW_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_FLOW_METER_UNDERFLOW_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_FlowMeterUnderflowFault_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal FSM_TorquePlausabilityFailedFault into payload (at bit 5 to bit 6).
    const bool FSM_TorquePlausabilityFailedFault_val = in_msg->FSM_TorquePlausabilityFailedFault_value;
    const uint32_t FSM_TorquePlausabilityFailedFault_raw = CAN_ENCODE(FSM_TorquePlausabilityFailedFault_val, CANSIG_FSM_FAULTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_SCALE, CANSIG_FSM_FAULTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_TorquePlausabilityFailedFault_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
}

void App_CanUtils_FSM_WarningsCounts_Pack(const FSM_WarningsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 7-byte payload for message FSM_WarningsCounts.
    // |xxxxxxxx|_____QQQ|PPPOOONN|NMMMLLLK|KKJJJIII|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal FSM_StackWaterMarkHighTask1HzWarningCount into payload (at bit 0 to bit 3).
    const uint32_t FSM_StackWaterMarkHighTask1HzWarningCount_val = in_msg->FSM_StackWaterMarkHighTask1HzWarningCount_value;
    const uint32_t FSM_StackWaterMarkHighTask1HzWarningCount_raw = CAN_ENCODE(FSM_StackWaterMarkHighTask1HzWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK1_HZ_WARNING_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_StackWaterMarkHighTask1HzWarningCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal FSM_StackWaterMarkHighTask100HzWarningCount into payload (at bit 3 to bit 6).
    const uint32_t FSM_StackWaterMarkHighTask100HzWarningCount_val = in_msg->FSM_StackWaterMarkHighTask100HzWarningCount_value;
    const uint32_t FSM_StackWaterMarkHighTask100HzWarningCount_raw = CAN_ENCODE(FSM_StackWaterMarkHighTask100HzWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK100_HZ_WARNING_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_StackWaterMarkHighTask100HzWarningCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal FSM_StackWaterMarkHighTask1kHzWarningCount into payload (at bit 6 to bit 9).
    const uint32_t FSM_StackWaterMarkHighTask1kHzWarningCount_val = in_msg->FSM_StackWaterMarkHighTask1kHzWarningCount_value;
    const uint32_t FSM_StackWaterMarkHighTask1kHzWarningCount_raw = CAN_ENCODE(FSM_StackWaterMarkHighTask1kHzWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK1K_HZ_WARNING_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_StackWaterMarkHighTask1kHzWarningCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(FSM_StackWaterMarkHighTask1kHzWarningCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 3-bit signal FSM_StackWaterMarkHighTaskCanRxWarningCount into payload (at bit 9 to bit 12).
    const uint32_t FSM_StackWaterMarkHighTaskCanRxWarningCount_val = in_msg->FSM_StackWaterMarkHighTaskCanRxWarningCount_value;
    const uint32_t FSM_StackWaterMarkHighTaskCanRxWarningCount_raw = CAN_ENCODE(FSM_StackWaterMarkHighTaskCanRxWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_RX_WARNING_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_StackWaterMarkHighTaskCanRxWarningCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 1
    
    // Pack 3-bit signal FSM_StackWaterMarkHighTaskCanTxWarningCount into payload (at bit 12 to bit 15).
    const uint32_t FSM_StackWaterMarkHighTaskCanTxWarningCount_val = in_msg->FSM_StackWaterMarkHighTaskCanTxWarningCount_value;
    const uint32_t FSM_StackWaterMarkHighTaskCanTxWarningCount_raw = CAN_ENCODE(FSM_StackWaterMarkHighTaskCanTxWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_STACK_WATER_MARK_HIGH_TASK_CAN_TX_WARNING_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_StackWaterMarkHighTaskCanTxWarningCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1
    
    // Pack 3-bit signal FSM_WatchdogTimeoutWarningCount into payload (at bit 15 to bit 18).
    const uint32_t FSM_WatchdogTimeoutWarningCount_val = in_msg->FSM_WatchdogTimeoutWarningCount_value;
    const uint32_t FSM_WatchdogTimeoutWarningCount_raw = CAN_ENCODE(FSM_WatchdogTimeoutWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_WATCHDOG_TIMEOUT_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_WATCHDOG_TIMEOUT_WARNING_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_WatchdogTimeoutWarningCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    out_data[2] |= packShiftRight(FSM_WatchdogTimeoutWarningCount_raw, 1, 0x3);   // Packs bits ______## of byte 2
    
    // Pack 3-bit signal FSM_TxOverflowWarningCount into payload (at bit 18 to bit 21).
    const uint32_t FSM_TxOverflowWarningCount_val = in_msg->FSM_TxOverflowWarningCount_value;
    const uint32_t FSM_TxOverflowWarningCount_raw = CAN_ENCODE(FSM_TxOverflowWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_TX_OVERFLOW_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_TX_OVERFLOW_WARNING_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(FSM_TxOverflowWarningCount_raw, 2, 0x1c);   // Packs bits ___###__ of byte 2
    
    // Pack 3-bit signal FSM_RxOverflowWarningCount into payload (at bit 21 to bit 24).
    const uint32_t FSM_RxOverflowWarningCount_val = in_msg->FSM_RxOverflowWarningCount_value;
    const uint32_t FSM_RxOverflowWarningCount_raw = CAN_ENCODE(FSM_RxOverflowWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_RX_OVERFLOW_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_RX_OVERFLOW_WARNING_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(FSM_RxOverflowWarningCount_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    
    // Pack 3-bit signal FSM_LeftWheelSpeedOutOfRangeWarningCount into payload (at bit 24 to bit 27).
    const uint32_t FSM_LeftWheelSpeedOutOfRangeWarningCount_val = in_msg->FSM_LeftWheelSpeedOutOfRangeWarningCount_value;
    const uint32_t FSM_LeftWheelSpeedOutOfRangeWarningCount_raw = CAN_ENCODE(FSM_LeftWheelSpeedOutOfRangeWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_LEFT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftRight(FSM_LeftWheelSpeedOutOfRangeWarningCount_raw, 0, 0x7);   // Packs bits _____### of byte 3
    
    // Pack 3-bit signal FSM_RightWheelSpeedOutOfRangeWarningCount into payload (at bit 27 to bit 30).
    const uint32_t FSM_RightWheelSpeedOutOfRangeWarningCount_val = in_msg->FSM_RightWheelSpeedOutOfRangeWarningCount_value;
    const uint32_t FSM_RightWheelSpeedOutOfRangeWarningCount_raw = CAN_ENCODE(FSM_RightWheelSpeedOutOfRangeWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_WARNING_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(FSM_RightWheelSpeedOutOfRangeWarningCount_raw, 3, 0x38);   // Packs bits __###___ of byte 3
    
    // Pack 3-bit signal FSM_SteeringAngleOCSCWarningCount into payload (at bit 30 to bit 33).
    const uint32_t FSM_SteeringAngleOCSCWarningCount_val = in_msg->FSM_SteeringAngleOCSCWarningCount_value;
    const uint32_t FSM_SteeringAngleOCSCWarningCount_raw = CAN_ENCODE(FSM_SteeringAngleOCSCWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_STEERING_ANGLE_OCSC_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_STEERING_ANGLE_OCSC_WARNING_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(FSM_SteeringAngleOCSCWarningCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 3
    out_data[4] |= packShiftRight(FSM_SteeringAngleOCSCWarningCount_raw, 2, 0x1);   // Packs bits _______# of byte 4
    
    // Pack 3-bit signal FSM_SteeringAngleOutOfRangeWarningCount into payload (at bit 33 to bit 36).
    const uint32_t FSM_SteeringAngleOutOfRangeWarningCount_val = in_msg->FSM_SteeringAngleOutOfRangeWarningCount_value;
    const uint32_t FSM_SteeringAngleOutOfRangeWarningCount_raw = CAN_ENCODE(FSM_SteeringAngleOutOfRangeWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_STEERING_ANGLE_OUT_OF_RANGE_WARNING_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(FSM_SteeringAngleOutOfRangeWarningCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 4
    
    // Pack 3-bit signal FSM_FrontBrakePressureOutOfRangeWarningCount into payload (at bit 36 to bit 39).
    const uint32_t FSM_FrontBrakePressureOutOfRangeWarningCount_val = in_msg->FSM_FrontBrakePressureOutOfRangeWarningCount_value;
    const uint32_t FSM_FrontBrakePressureOutOfRangeWarningCount_raw = CAN_ENCODE(FSM_FrontBrakePressureOutOfRangeWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(FSM_FrontBrakePressureOutOfRangeWarningCount_raw, 4, 0x70);   // Packs bits _###____ of byte 4
    
    // Pack 3-bit signal FSM_RearBrakePressureOutOfRangeWarningCount into payload (at bit 39 to bit 42).
    const uint32_t FSM_RearBrakePressureOutOfRangeWarningCount_val = in_msg->FSM_RearBrakePressureOutOfRangeWarningCount_value;
    const uint32_t FSM_RearBrakePressureOutOfRangeWarningCount_raw = CAN_ENCODE(FSM_RearBrakePressureOutOfRangeWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_WARNING_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(FSM_RearBrakePressureOutOfRangeWarningCount_raw, 7, 0x80);   // Packs bits #_______ of byte 4
    out_data[5] |= packShiftRight(FSM_RearBrakePressureOutOfRangeWarningCount_raw, 1, 0x3);   // Packs bits ______## of byte 5
    
    // Pack 3-bit signal FSM_FlowRateOutOfRangeWarningCount into payload (at bit 42 to bit 45).
    const uint32_t FSM_FlowRateOutOfRangeWarningCount_val = in_msg->FSM_FlowRateOutOfRangeWarningCount_value;
    const uint32_t FSM_FlowRateOutOfRangeWarningCount_raw = CAN_ENCODE(FSM_FlowRateOutOfRangeWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_FLOW_RATE_OUT_OF_RANGE_WARNING_COUNT_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(FSM_FlowRateOutOfRangeWarningCount_raw, 2, 0x1c);   // Packs bits ___###__ of byte 5
    
    // Pack 3-bit signal FSM_BrakeAppsDisagreementWarningCount into payload (at bit 45 to bit 48).
    const uint32_t FSM_BrakeAppsDisagreementWarningCount_val = in_msg->FSM_BrakeAppsDisagreementWarningCount_value;
    const uint32_t FSM_BrakeAppsDisagreementWarningCount_raw = CAN_ENCODE(FSM_BrakeAppsDisagreementWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_BRAKE_APPS_DISAGREEMENT_WARNING_COUNT_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(FSM_BrakeAppsDisagreementWarningCount_raw, 5, 0xe0);   // Packs bits ###_____ of byte 5
    
    // Pack 3-bit signal FSM_AppsDisagreementWarningCount into payload (at bit 48 to bit 51).
    const uint32_t FSM_AppsDisagreementWarningCount_val = in_msg->FSM_AppsDisagreementWarningCount_value;
    const uint32_t FSM_AppsDisagreementWarningCount_raw = CAN_ENCODE(FSM_AppsDisagreementWarningCount_val, CANSIG_FSM_WARNINGS_COUNTS_FSM_APPS_DISAGREEMENT_WARNING_COUNT_SCALE, CANSIG_FSM_WARNINGS_COUNTS_FSM_APPS_DISAGREEMENT_WARNING_COUNT_OFFSET, uint32_t);
    out_data[6] |= packShiftRight(FSM_AppsDisagreementWarningCount_raw, 0, 0x7);   // Packs bits _____### of byte 6
    
}

void App_CanUtils_FSM_FaultsCounts_Pack(const FSM_FaultsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message FSM_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______FF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal FSM_MissingHeartbeatFaultCount into payload (at bit 0 to bit 3).
    const uint32_t FSM_MissingHeartbeatFaultCount_val = in_msg->FSM_MissingHeartbeatFaultCount_value;
    const uint32_t FSM_MissingHeartbeatFaultCount_raw = CAN_ENCODE(FSM_MissingHeartbeatFaultCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_MISSING_HEARTBEAT_FAULT_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_MISSING_HEARTBEAT_FAULT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(FSM_MissingHeartbeatFaultCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal FSM_StateMachineFaultCount into payload (at bit 3 to bit 6).
    const uint32_t FSM_StateMachineFaultCount_val = in_msg->FSM_StateMachineFaultCount_value;
    const uint32_t FSM_StateMachineFaultCount_raw = CAN_ENCODE(FSM_StateMachineFaultCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_STATE_MACHINE_FAULT_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_STATE_MACHINE_FAULT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_StateMachineFaultCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal FSM_PappsOCSCFaultCount into payload (at bit 6 to bit 9).
    const uint32_t FSM_PappsOCSCFaultCount_val = in_msg->FSM_PappsOCSCFaultCount_value;
    const uint32_t FSM_PappsOCSCFaultCount_raw = CAN_ENCODE(FSM_PappsOCSCFaultCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_PAPPS_OCSC_FAULT_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_PAPPS_OCSC_FAULT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(FSM_PappsOCSCFaultCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(FSM_PappsOCSCFaultCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 3-bit signal FSM_SappsOCSCFaultCount into payload (at bit 9 to bit 12).
    const uint32_t FSM_SappsOCSCFaultCount_val = in_msg->FSM_SappsOCSCFaultCount_value;
    const uint32_t FSM_SappsOCSCFaultCount_raw = CAN_ENCODE(FSM_SappsOCSCFaultCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_SAPPS_OCSC_FAULT_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_SAPPS_OCSC_FAULT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_SappsOCSCFaultCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 1
    
    // Pack 3-bit signal FSM_FlowMeterUnderflowFaultCount into payload (at bit 12 to bit 15).
    const uint32_t FSM_FlowMeterUnderflowFaultCount_val = in_msg->FSM_FlowMeterUnderflowFaultCount_value;
    const uint32_t FSM_FlowMeterUnderflowFaultCount_raw = CAN_ENCODE(FSM_FlowMeterUnderflowFaultCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_FLOW_METER_UNDERFLOW_FAULT_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_FLOW_METER_UNDERFLOW_FAULT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_FlowMeterUnderflowFaultCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1
    
    // Pack 3-bit signal FSM_TorquePlausabilityFailedFaultCount into payload (at bit 15 to bit 18).
    const uint32_t FSM_TorquePlausabilityFailedFaultCount_val = in_msg->FSM_TorquePlausabilityFailedFaultCount_value;
    const uint32_t FSM_TorquePlausabilityFailedFaultCount_raw = CAN_ENCODE(FSM_TorquePlausabilityFailedFaultCount_val, CANSIG_FSM_FAULTS_COUNTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_COUNT_SCALE, CANSIG_FSM_FAULTS_COUNTS_FSM_TORQUE_PLAUSABILITY_FAILED_FAULT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(FSM_TorquePlausabilityFailedFaultCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    out_data[2] |= packShiftRight(FSM_TorquePlausabilityFailedFaultCount_raw, 1, 0x3);   // Packs bits ______## of byte 2
    
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

