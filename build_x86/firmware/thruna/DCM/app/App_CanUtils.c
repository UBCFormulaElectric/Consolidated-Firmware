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

void App_CanUtils_DCM_Vitals_Pack(const DCM_Vitals_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DCM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DCM_Heartbeat into payload (at bit 0 to bit 1).
    const bool DCM_Heartbeat_val = in_msg->DCM_Heartbeat_value;
    const uint32_t DCM_Heartbeat_raw = CAN_ENCODE(DCM_Heartbeat_val, CANSIG_DCM_VITALS_DCM_HEARTBEAT_SCALE, CANSIG_DCM_VITALS_DCM_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DCM_Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal DCM_State into payload (at bit 1 to bit 2).
    const DcmState DCM_State_val = in_msg->DCM_State_value;
    const uint32_t DCM_State_raw = CAN_ENCODE(DCM_State_val, CANSIG_DCM_VITALS_DCM_STATE_SCALE, CANSIG_DCM_VITALS_DCM_STATE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_State_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
}

void App_CanUtils_DCM_LeftInverterCommand_Pack(const DCM_LeftInverterCommand_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 8-byte payload for message DCM_LeftInverterCommand.
    // |GGGGGGGG|GGGGGGGG|_____FED|_______C|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal DCM_LeftInverterTorqueCommand into payload (at bit 0 to bit 16).
    const float DCM_LeftInverterTorqueCommand_val = in_msg->DCM_LeftInverterTorqueCommand_value;
    const uint32_t DCM_LeftInverterTorqueCommand_raw = CAN_ENCODE(DCM_LeftInverterTorqueCommand_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_COMMAND_OFFSET, float);
    out_data[0] |= packShiftRight(DCM_LeftInverterTorqueCommand_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(DCM_LeftInverterTorqueCommand_raw, 8, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 16-bit signal DCM_LeftInverterSpeedCommand into payload (at bit 16 to bit 32).
    const int DCM_LeftInverterSpeedCommand_val = in_msg->DCM_LeftInverterSpeedCommand_value;
    const uint32_t DCM_LeftInverterSpeedCommand_raw = CAN_ENCODE(DCM_LeftInverterSpeedCommand_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_SPEED_COMMAND_OFFSET, int);
    out_data[2] |= packShiftRight(DCM_LeftInverterSpeedCommand_raw, 0, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(DCM_LeftInverterSpeedCommand_raw, 8, 0xff);   // Packs bits ######## of byte 3
    
    // Pack 1-bit signal DCM_LeftInverterDirectionCommand into payload (at bit 32 to bit 33).
    const InverterDirection DCM_LeftInverterDirectionCommand_val = in_msg->DCM_LeftInverterDirectionCommand_value;
    const uint32_t DCM_LeftInverterDirectionCommand_raw = CAN_ENCODE(DCM_LeftInverterDirectionCommand_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_DIRECTION_COMMAND_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(DCM_LeftInverterDirectionCommand_raw, 0, 0x1);   // Packs bits _______# of byte 4
    
    // Pack 1-bit signal DCM_LeftInverterEnable into payload (at bit 40 to bit 41).
    const bool DCM_LeftInverterEnable_val = in_msg->DCM_LeftInverterEnable_value;
    const uint32_t DCM_LeftInverterEnable_raw = CAN_ENCODE(DCM_LeftInverterEnable_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_OFFSET, uint32_t);
    out_data[5] |= packShiftRight(DCM_LeftInverterEnable_raw, 0, 0x1);   // Packs bits _______# of byte 5
    
    // Pack 1-bit signal DCM_LeftInverterEnableDischarge into payload (at bit 41 to bit 42).
    const bool DCM_LeftInverterEnableDischarge_val = in_msg->DCM_LeftInverterEnableDischarge_value;
    const uint32_t DCM_LeftInverterEnableDischarge_raw = CAN_ENCODE(DCM_LeftInverterEnableDischarge_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_DISCHARGE_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_DISCHARGE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(DCM_LeftInverterEnableDischarge_raw, 1, 0x2);   // Packs bits ______#_ of byte 5
    
    // Pack 1-bit signal DCM_LeftInverterEnableSpeedMode into payload (at bit 42 to bit 43).
    const bool DCM_LeftInverterEnableSpeedMode_val = in_msg->DCM_LeftInverterEnableSpeedMode_value;
    const uint32_t DCM_LeftInverterEnableSpeedMode_raw = CAN_ENCODE(DCM_LeftInverterEnableSpeedMode_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_SPEED_MODE_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_ENABLE_SPEED_MODE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(DCM_LeftInverterEnableSpeedMode_raw, 2, 0x4);   // Packs bits _____#__ of byte 5
    
    // Pack 16-bit signal DCM_LeftInverterTorqueLimit into payload (at bit 48 to bit 64).
    const float DCM_LeftInverterTorqueLimit_val = in_msg->DCM_LeftInverterTorqueLimit_value;
    const uint32_t DCM_LeftInverterTorqueLimit_raw = CAN_ENCODE(DCM_LeftInverterTorqueLimit_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DCM_LEFT_INVERTER_TORQUE_LIMIT_OFFSET, float);
    out_data[6] |= packShiftRight(DCM_LeftInverterTorqueLimit_raw, 0, 0xff);   // Packs bits ######## of byte 6
    out_data[7] |= packShiftRight(DCM_LeftInverterTorqueLimit_raw, 8, 0xff);   // Packs bits ######## of byte 7
    
}

void App_CanUtils_DCM_RightInverterCommand_Pack(const DCM_RightInverterCommand_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 8-byte payload for message DCM_RightInverterCommand.
    // |GGGGGGGG|GGGGGGGG|_____FED|_______C|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal DCM_RightInverterTorqueCommand into payload (at bit 0 to bit 16).
    const float DCM_RightInverterTorqueCommand_val = in_msg->DCM_RightInverterTorqueCommand_value;
    const uint32_t DCM_RightInverterTorqueCommand_raw = CAN_ENCODE(DCM_RightInverterTorqueCommand_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_COMMAND_OFFSET, float);
    out_data[0] |= packShiftRight(DCM_RightInverterTorqueCommand_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(DCM_RightInverterTorqueCommand_raw, 8, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 16-bit signal DCM_RightInverterSpeedCommand into payload (at bit 16 to bit 32).
    const int DCM_RightInverterSpeedCommand_val = in_msg->DCM_RightInverterSpeedCommand_value;
    const uint32_t DCM_RightInverterSpeedCommand_raw = CAN_ENCODE(DCM_RightInverterSpeedCommand_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_SPEED_COMMAND_OFFSET, int);
    out_data[2] |= packShiftRight(DCM_RightInverterSpeedCommand_raw, 0, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(DCM_RightInverterSpeedCommand_raw, 8, 0xff);   // Packs bits ######## of byte 3
    
    // Pack 1-bit signal DCM_RightInverterDirectionCommand into payload (at bit 32 to bit 33).
    const InverterDirection DCM_RightInverterDirectionCommand_val = in_msg->DCM_RightInverterDirectionCommand_value;
    const uint32_t DCM_RightInverterDirectionCommand_raw = CAN_ENCODE(DCM_RightInverterDirectionCommand_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_DIRECTION_COMMAND_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(DCM_RightInverterDirectionCommand_raw, 0, 0x1);   // Packs bits _______# of byte 4
    
    // Pack 1-bit signal DCM_RightInverterEnable into payload (at bit 40 to bit 41).
    const bool DCM_RightInverterEnable_val = in_msg->DCM_RightInverterEnable_value;
    const uint32_t DCM_RightInverterEnable_raw = CAN_ENCODE(DCM_RightInverterEnable_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_OFFSET, uint32_t);
    out_data[5] |= packShiftRight(DCM_RightInverterEnable_raw, 0, 0x1);   // Packs bits _______# of byte 5
    
    // Pack 1-bit signal DCM_RightInverterEnableDischarge into payload (at bit 41 to bit 42).
    const bool DCM_RightInverterEnableDischarge_val = in_msg->DCM_RightInverterEnableDischarge_value;
    const uint32_t DCM_RightInverterEnableDischarge_raw = CAN_ENCODE(DCM_RightInverterEnableDischarge_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_DISCHARGE_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_DISCHARGE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(DCM_RightInverterEnableDischarge_raw, 1, 0x2);   // Packs bits ______#_ of byte 5
    
    // Pack 1-bit signal DCM_RightInverterEnableSpeedMode into payload (at bit 42 to bit 43).
    const bool DCM_RightInverterEnableSpeedMode_val = in_msg->DCM_RightInverterEnableSpeedMode_value;
    const uint32_t DCM_RightInverterEnableSpeedMode_raw = CAN_ENCODE(DCM_RightInverterEnableSpeedMode_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_SPEED_MODE_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_ENABLE_SPEED_MODE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(DCM_RightInverterEnableSpeedMode_raw, 2, 0x4);   // Packs bits _____#__ of byte 5
    
    // Pack 16-bit signal DCM_RightInverterTorqueLimit into payload (at bit 48 to bit 64).
    const float DCM_RightInverterTorqueLimit_val = in_msg->DCM_RightInverterTorqueLimit_value;
    const uint32_t DCM_RightInverterTorqueLimit_raw = CAN_ENCODE(DCM_RightInverterTorqueLimit_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DCM_RIGHT_INVERTER_TORQUE_LIMIT_OFFSET, float);
    out_data[6] |= packShiftRight(DCM_RightInverterTorqueLimit_raw, 0, 0xff);   // Packs bits ######## of byte 6
    out_data[7] |= packShiftRight(DCM_RightInverterTorqueLimit_raw, 8, 0xff);   // Packs bits ######## of byte 7
    
}

void App_CanUtils_DCM_AlertsContext_Pack(const DCM_AlertsContext_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message DCM_AlertsContext.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CCC|BBBBBBBB|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal DCM_TxOverflowCount into payload (at bit 0 to bit 8).
    const uint32_t DCM_TxOverflowCount_val = in_msg->DCM_TxOverflowCount_value;
    const uint32_t DCM_TxOverflowCount_raw = CAN_ENCODE(DCM_TxOverflowCount_val, CANSIG_DCM_ALERTS_CONTEXT_DCM_TX_OVERFLOW_COUNT_SCALE, CANSIG_DCM_ALERTS_CONTEXT_DCM_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DCM_TxOverflowCount_raw, 0, 0xff);   // Packs bits ######## of byte 0
    
    // Pack 8-bit signal DCM_RxOverflowCount into payload (at bit 8 to bit 16).
    const uint32_t DCM_RxOverflowCount_val = in_msg->DCM_RxOverflowCount_value;
    const uint32_t DCM_RxOverflowCount_raw = CAN_ENCODE(DCM_RxOverflowCount_val, CANSIG_DCM_ALERTS_CONTEXT_DCM_RX_OVERFLOW_COUNT_SCALE, CANSIG_DCM_ALERTS_CONTEXT_DCM_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(DCM_RxOverflowCount_raw, 0, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 3-bit signal DCM_WatchdogTimeoutTaskName into payload (at bit 16 to bit 19).
    const RtosTaskName DCM_WatchdogTimeoutTaskName_val = in_msg->DCM_WatchdogTimeoutTaskName_value;
    const uint32_t DCM_WatchdogTimeoutTaskName_raw = CAN_ENCODE(DCM_WatchdogTimeoutTaskName_val, CANSIG_DCM_ALERTS_CONTEXT_DCM_WATCHDOG_TIMEOUT_TASK_NAME_SCALE, CANSIG_DCM_ALERTS_CONTEXT_DCM_WATCHDOG_TIMEOUT_TASK_NAME_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(DCM_WatchdogTimeoutTaskName_raw, 0, 0x7);   // Packs bits _____### of byte 2
    
}

void App_CanUtils_DCM_EllipseStatus_Pack(const DCM_EllipseStatus_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 6-byte payload for message DCM_EllipseStatus.
    // |xxxxxxxx|xxxxxxxx|BBBBBBBB|BBBBBBBB|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal DCM_EllipseGeneralStatusBitmask into payload (at bit 0 to bit 16).
    const uint32_t DCM_EllipseGeneralStatusBitmask_val = in_msg->DCM_EllipseGeneralStatusBitmask_value;
    const uint32_t DCM_EllipseGeneralStatusBitmask_raw = CAN_ENCODE(DCM_EllipseGeneralStatusBitmask_val, CANSIG_DCM_ELLIPSE_STATUS_DCM_ELLIPSE_GENERAL_STATUS_BITMASK_SCALE, CANSIG_DCM_ELLIPSE_STATUS_DCM_ELLIPSE_GENERAL_STATUS_BITMASK_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DCM_EllipseGeneralStatusBitmask_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(DCM_EllipseGeneralStatusBitmask_raw, 8, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 32-bit signal DCM_EllipseComStatusBitmask into payload (at bit 16 to bit 48).
    const uint32_t DCM_EllipseComStatusBitmask_val = in_msg->DCM_EllipseComStatusBitmask_value;
    const uint32_t DCM_EllipseComStatusBitmask_raw = CAN_ENCODE(DCM_EllipseComStatusBitmask_val, CANSIG_DCM_ELLIPSE_STATUS_DCM_ELLIPSE_COM_STATUS_BITMASK_SCALE, CANSIG_DCM_ELLIPSE_STATUS_DCM_ELLIPSE_COM_STATUS_BITMASK_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(DCM_EllipseComStatusBitmask_raw, 0, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(DCM_EllipseComStatusBitmask_raw, 8, 0xff);   // Packs bits ######## of byte 3
    out_data[4] |= packShiftRight(DCM_EllipseComStatusBitmask_raw, 16, 0xff);   // Packs bits ######## of byte 4
    out_data[5] |= packShiftRight(DCM_EllipseComStatusBitmask_raw, 24, 0xff);   // Packs bits ######## of byte 5
    
}

void App_CanUtils_DCM_EllipseTime_Pack(const DCM_EllipseTime_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 4-byte payload for message DCM_EllipseTime.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 32-bit signal DCM_EllipseTimestamp into payload (at bit 0 to bit 32).
    const uint32_t DCM_EllipseTimestamp_val = in_msg->DCM_EllipseTimestamp_value;
    const uint32_t DCM_EllipseTimestamp_raw = CAN_ENCODE(DCM_EllipseTimestamp_val, CANSIG_DCM_ELLIPSE_TIME_DCM_ELLIPSE_TIMESTAMP_SCALE, CANSIG_DCM_ELLIPSE_TIME_DCM_ELLIPSE_TIMESTAMP_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DCM_EllipseTimestamp_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(DCM_EllipseTimestamp_raw, 8, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(DCM_EllipseTimestamp_raw, 16, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(DCM_EllipseTimestamp_raw, 24, 0xff);   // Packs bits ######## of byte 3
    
}

void App_CanUtils_DCM_EllipseAcceleration_Pack(const DCM_EllipseAcceleration_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message DCM_EllipseAcceleration.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_CCCCCCC|CCCCCCBB|BBBBBBBB|BBBAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 13-bit signal DCM_AccelerationForward into payload (at bit 0 to bit 13).
    const float DCM_AccelerationForward_val = in_msg->DCM_AccelerationForward_value;
    const uint32_t DCM_AccelerationForward_raw = CAN_ENCODE(DCM_AccelerationForward_val, CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_FORWARD_SCALE, CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_FORWARD_OFFSET, float);
    out_data[0] |= packShiftRight(DCM_AccelerationForward_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(DCM_AccelerationForward_raw, 8, 0x1f);   // Packs bits ___##### of byte 1
    
    // Pack 13-bit signal DCM_AccelerationLateral into payload (at bit 13 to bit 26).
    const float DCM_AccelerationLateral_val = in_msg->DCM_AccelerationLateral_value;
    const uint32_t DCM_AccelerationLateral_raw = CAN_ENCODE(DCM_AccelerationLateral_val, CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_LATERAL_SCALE, CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_LATERAL_OFFSET, float);
    out_data[1] |= packShiftLeft(DCM_AccelerationLateral_raw, 5, 0xe0);   // Packs bits ###_____ of byte 1
    out_data[2] |= packShiftRight(DCM_AccelerationLateral_raw, 3, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(DCM_AccelerationLateral_raw, 11, 0x3);   // Packs bits ______## of byte 3
    
    // Pack 13-bit signal DCM_AccelerationVertical into payload (at bit 26 to bit 39).
    const float DCM_AccelerationVertical_val = in_msg->DCM_AccelerationVertical_value;
    const uint32_t DCM_AccelerationVertical_raw = CAN_ENCODE(DCM_AccelerationVertical_val, CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_VERTICAL_SCALE, CANSIG_DCM_ELLIPSE_ACCELERATION_DCM_ACCELERATION_VERTICAL_OFFSET, float);
    out_data[3] |= packShiftLeft(DCM_AccelerationVertical_raw, 2, 0xfc);   // Packs bits ######__ of byte 3
    out_data[4] |= packShiftRight(DCM_AccelerationVertical_raw, 6, 0x7f);   // Packs bits _####### of byte 4
    
}

void App_CanUtils_DCM_EllipseAngularVelocity_Pack(const DCM_EllipseAngularVelocity_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 4-byte payload for message DCM_EllipseAngularVelocity.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__CCCCCC|CCCCBBBB|BBBBBBAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 10-bit signal DCM_AngularVelocityRoll into payload (at bit 0 to bit 10).
    const int DCM_AngularVelocityRoll_val = in_msg->DCM_AngularVelocityRoll_value;
    const uint32_t DCM_AngularVelocityRoll_raw = CAN_ENCODE(DCM_AngularVelocityRoll_val, CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_ROLL_SCALE, CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_ROLL_OFFSET, int);
    out_data[0] |= packShiftRight(DCM_AngularVelocityRoll_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(DCM_AngularVelocityRoll_raw, 8, 0x3);   // Packs bits ______## of byte 1
    
    // Pack 10-bit signal DCM_AngularVelocityPitch into payload (at bit 10 to bit 20).
    const int DCM_AngularVelocityPitch_val = in_msg->DCM_AngularVelocityPitch_value;
    const uint32_t DCM_AngularVelocityPitch_raw = CAN_ENCODE(DCM_AngularVelocityPitch_val, CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_PITCH_SCALE, CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_PITCH_OFFSET, int);
    out_data[1] |= packShiftLeft(DCM_AngularVelocityPitch_raw, 2, 0xfc);   // Packs bits ######__ of byte 1
    out_data[2] |= packShiftRight(DCM_AngularVelocityPitch_raw, 6, 0xf);   // Packs bits ____#### of byte 2
    
    // Pack 10-bit signal DCM_AngularVelocityYaw into payload (at bit 20 to bit 30).
    const int DCM_AngularVelocityYaw_val = in_msg->DCM_AngularVelocityYaw_value;
    const uint32_t DCM_AngularVelocityYaw_raw = CAN_ENCODE(DCM_AngularVelocityYaw_val, CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_YAW_SCALE, CANSIG_DCM_ELLIPSE_ANGULAR_VELOCITY_DCM_ANGULAR_VELOCITY_YAW_OFFSET, int);
    out_data[2] |= packShiftLeft(DCM_AngularVelocityYaw_raw, 4, 0xf0);   // Packs bits ####____ of byte 2
    out_data[3] |= packShiftRight(DCM_AngularVelocityYaw_raw, 4, 0x3f);   // Packs bits __###### of byte 3
    
}

void App_CanUtils_DCM_EllipseEulerAngles_Pack(const DCM_EllipseEulerAngles_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message DCM_EllipseEulerAngles.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|____CCCC|CCCCCCCC|BBBBBBBB|BBBBAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 12-bit signal DCM_EulerAnglesRoll into payload (at bit 0 to bit 12).
    const float DCM_EulerAnglesRoll_val = in_msg->DCM_EulerAnglesRoll_value;
    const uint32_t DCM_EulerAnglesRoll_raw = CAN_ENCODE(DCM_EulerAnglesRoll_val, CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_ROLL_SCALE, CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_ROLL_OFFSET, float);
    out_data[0] |= packShiftRight(DCM_EulerAnglesRoll_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(DCM_EulerAnglesRoll_raw, 8, 0xf);   // Packs bits ____#### of byte 1
    
    // Pack 12-bit signal DCM_EulerAnglesPitch into payload (at bit 12 to bit 24).
    const float DCM_EulerAnglesPitch_val = in_msg->DCM_EulerAnglesPitch_value;
    const uint32_t DCM_EulerAnglesPitch_raw = CAN_ENCODE(DCM_EulerAnglesPitch_val, CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_PITCH_SCALE, CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_PITCH_OFFSET, float);
    out_data[1] |= packShiftLeft(DCM_EulerAnglesPitch_raw, 4, 0xf0);   // Packs bits ####____ of byte 1
    out_data[2] |= packShiftRight(DCM_EulerAnglesPitch_raw, 4, 0xff);   // Packs bits ######## of byte 2
    
    // Pack 12-bit signal DCM_EulerAnglesYaw into payload (at bit 24 to bit 36).
    const float DCM_EulerAnglesYaw_val = in_msg->DCM_EulerAnglesYaw_value;
    const uint32_t DCM_EulerAnglesYaw_raw = CAN_ENCODE(DCM_EulerAnglesYaw_val, CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_YAW_SCALE, CANSIG_DCM_ELLIPSE_EULER_ANGLES_DCM_EULER_ANGLES_YAW_OFFSET, float);
    out_data[3] |= packShiftRight(DCM_EulerAnglesYaw_raw, 0, 0xff);   // Packs bits ######## of byte 3
    out_data[4] |= packShiftRight(DCM_EulerAnglesYaw_raw, 8, 0xf);   // Packs bits ____#### of byte 4
    
}

void App_CanUtils_DCM_CommitInfo_Pack(const DCM_CommitInfo_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message DCM_CommitInfo.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_______B|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 32-bit signal DCM_Hash into payload (at bit 0 to bit 32).
    const uint32_t DCM_Hash_val = in_msg->DCM_Hash_value;
    const uint32_t DCM_Hash_raw = CAN_ENCODE(DCM_Hash_val, CANSIG_DCM_COMMIT_INFO_DCM_HASH_SCALE, CANSIG_DCM_COMMIT_INFO_DCM_HASH_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DCM_Hash_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(DCM_Hash_raw, 8, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(DCM_Hash_raw, 16, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(DCM_Hash_raw, 24, 0xff);   // Packs bits ######## of byte 3
    
    // Pack 1-bit signal DCM_Clean into payload (at bit 32 to bit 33).
    const bool DCM_Clean_val = in_msg->DCM_Clean_value;
    const uint32_t DCM_Clean_raw = CAN_ENCODE(DCM_Clean_val, CANSIG_DCM_COMMIT_INFO_DCM_CLEAN_SCALE, CANSIG_DCM_COMMIT_INFO_DCM_CLEAN_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(DCM_Clean_raw, 0, 0x1);   // Packs bits _______# of byte 4
    
}

void App_CanUtils_DCM_Warnings_Pack(const DCM_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DCM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DCM_Warning_StackWaterMarkHighTask1Hz into payload (at bit 0 to bit 1).
    const bool DCM_Warning_StackWaterMarkHighTask1Hz_val = in_msg->DCM_Warning_StackWaterMarkHighTask1Hz_value;
    const uint32_t DCM_Warning_StackWaterMarkHighTask1Hz_raw = CAN_ENCODE(DCM_Warning_StackWaterMarkHighTask1Hz_val, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DCM_Warning_StackWaterMarkHighTask1Hz_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal DCM_Warning_StackWaterMarkHighTask100Hz into payload (at bit 1 to bit 2).
    const bool DCM_Warning_StackWaterMarkHighTask100Hz_val = in_msg->DCM_Warning_StackWaterMarkHighTask100Hz_value;
    const uint32_t DCM_Warning_StackWaterMarkHighTask100Hz_raw = CAN_ENCODE(DCM_Warning_StackWaterMarkHighTask100Hz_val, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Warning_StackWaterMarkHighTask100Hz_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal DCM_Warning_StackWaterMarkHighTask1kHz into payload (at bit 2 to bit 3).
    const bool DCM_Warning_StackWaterMarkHighTask1kHz_val = in_msg->DCM_Warning_StackWaterMarkHighTask1kHz_value;
    const uint32_t DCM_Warning_StackWaterMarkHighTask1kHz_raw = CAN_ENCODE(DCM_Warning_StackWaterMarkHighTask1kHz_val, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Warning_StackWaterMarkHighTask1kHz_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal DCM_Warning_StackWaterMarkHighTaskCanRx into payload (at bit 3 to bit 4).
    const bool DCM_Warning_StackWaterMarkHighTaskCanRx_val = in_msg->DCM_Warning_StackWaterMarkHighTaskCanRx_value;
    const uint32_t DCM_Warning_StackWaterMarkHighTaskCanRx_raw = CAN_ENCODE(DCM_Warning_StackWaterMarkHighTaskCanRx_val, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Warning_StackWaterMarkHighTaskCanRx_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal DCM_Warning_StackWaterMarkHighTaskCanTx into payload (at bit 4 to bit 5).
    const bool DCM_Warning_StackWaterMarkHighTaskCanTx_val = in_msg->DCM_Warning_StackWaterMarkHighTaskCanTx_value;
    const uint32_t DCM_Warning_StackWaterMarkHighTaskCanTx_raw = CAN_ENCODE(DCM_Warning_StackWaterMarkHighTaskCanTx_val, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Warning_StackWaterMarkHighTaskCanTx_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal DCM_Warning_WatchdogTimeout into payload (at bit 5 to bit 6).
    const bool DCM_Warning_WatchdogTimeout_val = in_msg->DCM_Warning_WatchdogTimeout_value;
    const uint32_t DCM_Warning_WatchdogTimeout_raw = CAN_ENCODE(DCM_Warning_WatchdogTimeout_val, CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Warning_WatchdogTimeout_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 1-bit signal DCM_Warning_TxOverflow into payload (at bit 6 to bit 7).
    const bool DCM_Warning_TxOverflow_val = in_msg->DCM_Warning_TxOverflow_value;
    const uint32_t DCM_Warning_TxOverflow_raw = CAN_ENCODE(DCM_Warning_TxOverflow_val, CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Warning_TxOverflow_raw, 6, 0x40);   // Packs bits _#______ of byte 0
    
    // Pack 1-bit signal DCM_Warning_RxOverflow into payload (at bit 7 to bit 8).
    const bool DCM_Warning_RxOverflow_val = in_msg->DCM_Warning_RxOverflow_value;
    const uint32_t DCM_Warning_RxOverflow_raw = CAN_ENCODE(DCM_Warning_RxOverflow_val, CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Warning_RxOverflow_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    
}

void App_CanUtils_DCM_Faults_Pack(const DCM_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DCM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DCM_Fault_MissingHeartbeat into payload (at bit 0 to bit 1).
    const bool DCM_Fault_MissingHeartbeat_val = in_msg->DCM_Fault_MissingHeartbeat_value;
    const uint32_t DCM_Fault_MissingHeartbeat_raw = CAN_ENCODE(DCM_Fault_MissingHeartbeat_val, CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DCM_Fault_MissingHeartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal DCM_Fault_LeftInverterFault into payload (at bit 1 to bit 2).
    const bool DCM_Fault_LeftInverterFault_val = in_msg->DCM_Fault_LeftInverterFault_value;
    const uint32_t DCM_Fault_LeftInverterFault_raw = CAN_ENCODE(DCM_Fault_LeftInverterFault_val, CANSIG_DCM_FAULTS_DCM_FAULT_LEFT_INVERTER_FAULT_SCALE, CANSIG_DCM_FAULTS_DCM_FAULT_LEFT_INVERTER_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Fault_LeftInverterFault_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal DCM_Fault_RightInverterFault into payload (at bit 2 to bit 3).
    const bool DCM_Fault_RightInverterFault_val = in_msg->DCM_Fault_RightInverterFault_value;
    const uint32_t DCM_Fault_RightInverterFault_raw = CAN_ENCODE(DCM_Fault_RightInverterFault_val, CANSIG_DCM_FAULTS_DCM_FAULT_RIGHT_INVERTER_FAULT_SCALE, CANSIG_DCM_FAULTS_DCM_FAULT_RIGHT_INVERTER_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Fault_RightInverterFault_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
}

void App_CanUtils_DCM_WarningsCounts_Pack(const DCM_WarningsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message DCM_WarningsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal DCM_Warning_StackWaterMarkHighTask1HzCount into payload (at bit 0 to bit 3).
    const uint32_t DCM_Warning_StackWaterMarkHighTask1HzCount_val = in_msg->DCM_Warning_StackWaterMarkHighTask1HzCount_value;
    const uint32_t DCM_Warning_StackWaterMarkHighTask1HzCount_raw = CAN_ENCODE(DCM_Warning_StackWaterMarkHighTask1HzCount_val, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_SCALE, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DCM_Warning_StackWaterMarkHighTask1HzCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal DCM_Warning_StackWaterMarkHighTask100HzCount into payload (at bit 3 to bit 6).
    const uint32_t DCM_Warning_StackWaterMarkHighTask100HzCount_val = in_msg->DCM_Warning_StackWaterMarkHighTask100HzCount_value;
    const uint32_t DCM_Warning_StackWaterMarkHighTask100HzCount_raw = CAN_ENCODE(DCM_Warning_StackWaterMarkHighTask100HzCount_val, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_SCALE, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Warning_StackWaterMarkHighTask100HzCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal DCM_Warning_StackWaterMarkHighTask1kHzCount into payload (at bit 6 to bit 9).
    const uint32_t DCM_Warning_StackWaterMarkHighTask1kHzCount_val = in_msg->DCM_Warning_StackWaterMarkHighTask1kHzCount_value;
    const uint32_t DCM_Warning_StackWaterMarkHighTask1kHzCount_raw = CAN_ENCODE(DCM_Warning_StackWaterMarkHighTask1kHzCount_val, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_SCALE, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Warning_StackWaterMarkHighTask1kHzCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(DCM_Warning_StackWaterMarkHighTask1kHzCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
    // Pack 3-bit signal DCM_Warning_StackWaterMarkHighTaskCanRxCount into payload (at bit 9 to bit 12).
    const uint32_t DCM_Warning_StackWaterMarkHighTaskCanRxCount_val = in_msg->DCM_Warning_StackWaterMarkHighTaskCanRxCount_value;
    const uint32_t DCM_Warning_StackWaterMarkHighTaskCanRxCount_raw = CAN_ENCODE(DCM_Warning_StackWaterMarkHighTaskCanRxCount_val, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_SCALE, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(DCM_Warning_StackWaterMarkHighTaskCanRxCount_raw, 1, 0xe);   // Packs bits ____###_ of byte 1
    
    // Pack 3-bit signal DCM_Warning_StackWaterMarkHighTaskCanTxCount into payload (at bit 12 to bit 15).
    const uint32_t DCM_Warning_StackWaterMarkHighTaskCanTxCount_val = in_msg->DCM_Warning_StackWaterMarkHighTaskCanTxCount_value;
    const uint32_t DCM_Warning_StackWaterMarkHighTaskCanTxCount_raw = CAN_ENCODE(DCM_Warning_StackWaterMarkHighTaskCanTxCount_val, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_SCALE, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(DCM_Warning_StackWaterMarkHighTaskCanTxCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1
    
    // Pack 3-bit signal DCM_Warning_WatchdogTimeoutCount into payload (at bit 15 to bit 18).
    const uint32_t DCM_Warning_WatchdogTimeoutCount_val = in_msg->DCM_Warning_WatchdogTimeoutCount_value;
    const uint32_t DCM_Warning_WatchdogTimeoutCount_raw = CAN_ENCODE(DCM_Warning_WatchdogTimeoutCount_val, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_WATCHDOG_TIMEOUT_COUNT_SCALE, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_WATCHDOG_TIMEOUT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(DCM_Warning_WatchdogTimeoutCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1
    out_data[2] |= packShiftRight(DCM_Warning_WatchdogTimeoutCount_raw, 1, 0x3);   // Packs bits ______## of byte 2
    
    // Pack 3-bit signal DCM_Warning_TxOverflowCount into payload (at bit 18 to bit 21).
    const uint32_t DCM_Warning_TxOverflowCount_val = in_msg->DCM_Warning_TxOverflowCount_value;
    const uint32_t DCM_Warning_TxOverflowCount_raw = CAN_ENCODE(DCM_Warning_TxOverflowCount_val, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_TX_OVERFLOW_COUNT_SCALE, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(DCM_Warning_TxOverflowCount_raw, 2, 0x1c);   // Packs bits ___###__ of byte 2
    
    // Pack 3-bit signal DCM_Warning_RxOverflowCount into payload (at bit 21 to bit 24).
    const uint32_t DCM_Warning_RxOverflowCount_val = in_msg->DCM_Warning_RxOverflowCount_value;
    const uint32_t DCM_Warning_RxOverflowCount_raw = CAN_ENCODE(DCM_Warning_RxOverflowCount_val, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_RX_OVERFLOW_COUNT_SCALE, CANSIG_DCM_WARNINGS_COUNTS_DCM_WARNING_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(DCM_Warning_RxOverflowCount_raw, 5, 0xe0);   // Packs bits ###_____ of byte 2
    
}

void App_CanUtils_DCM_FaultsCounts_Pack(const DCM_FaultsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 2-byte payload for message DCM_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______C|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal DCM_Fault_MissingHeartbeatCount into payload (at bit 0 to bit 3).
    const uint32_t DCM_Fault_MissingHeartbeatCount_val = in_msg->DCM_Fault_MissingHeartbeatCount_value;
    const uint32_t DCM_Fault_MissingHeartbeatCount_raw = CAN_ENCODE(DCM_Fault_MissingHeartbeatCount_val, CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_MISSING_HEARTBEAT_COUNT_SCALE, CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_MISSING_HEARTBEAT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DCM_Fault_MissingHeartbeatCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
    // Pack 3-bit signal DCM_Fault_LeftInverterFaultCount into payload (at bit 3 to bit 6).
    const uint32_t DCM_Fault_LeftInverterFaultCount_val = in_msg->DCM_Fault_LeftInverterFaultCount_value;
    const uint32_t DCM_Fault_LeftInverterFaultCount_raw = CAN_ENCODE(DCM_Fault_LeftInverterFaultCount_val, CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_LEFT_INVERTER_FAULT_COUNT_SCALE, CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_LEFT_INVERTER_FAULT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Fault_LeftInverterFaultCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0
    
    // Pack 3-bit signal DCM_Fault_RightInverterFaultCount into payload (at bit 6 to bit 9).
    const uint32_t DCM_Fault_RightInverterFaultCount_val = in_msg->DCM_Fault_RightInverterFaultCount_value;
    const uint32_t DCM_Fault_RightInverterFaultCount_raw = CAN_ENCODE(DCM_Fault_RightInverterFaultCount_val, CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_RIGHT_INVERTER_FAULT_COUNT_SCALE, CANSIG_DCM_FAULTS_COUNTS_DCM_FAULT_RIGHT_INVERTER_FAULT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_Fault_RightInverterFaultCount_raw, 6, 0xc0);   // Packs bits ##______ of byte 0
    out_data[1] |= packShiftRight(DCM_Fault_RightInverterFaultCount_raw, 2, 0x1);   // Packs bits _______# of byte 1
    
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

void App_CanUtils_DIM_Switches_Unpack(const uint8_t* const in_data, DIM_Switches_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DIM_Switches.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DIM_StartSwitch from payload (at bit 0 to bit 1).
    uint32_t DIM_StartSwitch_raw = 0;
    DIM_StartSwitch_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    SwitchState DIM_StartSwitch_val = CAN_DECODE(DIM_StartSwitch_raw, CANSIG_DIM_SWITCHES_DIM_START_SWITCH_SCALE, CANSIG_DIM_SWITCHES_DIM_START_SWITCH_OFFSET, uint32_t);
    out_msg->DIM_StartSwitch_value = DIM_StartSwitch_val;
    
    // Unpack 1-bit signal DIM_AuxSwitch from payload (at bit 1 to bit 2).
    uint32_t DIM_AuxSwitch_raw = 0;
    DIM_AuxSwitch_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    SwitchState DIM_AuxSwitch_val = CAN_DECODE(DIM_AuxSwitch_raw, CANSIG_DIM_SWITCHES_DIM_AUX_SWITCH_SCALE, CANSIG_DIM_SWITCHES_DIM_AUX_SWITCH_OFFSET, uint32_t);
    out_msg->DIM_AuxSwitch_value = DIM_AuxSwitch_val;
    
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

void App_CanUtils_INVR_InternalStates_Unpack(const uint8_t* const in_data, INVR_InternalStates_Signals* const out_msg)
{
    // Unpack 8-byte payload for message INVR_InternalStates.
    // |_WVUTSRQ|PO_____N|MMMM___L|KKK____J|__IHGFED|____CCCC|BBBBBBBB|____AAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 4-bit signal INVR_VsmState from payload (at bit 0 to bit 4).
    uint32_t INVR_VsmState_raw = 0;
    INVR_VsmState_raw |= unpackShiftRight(in_data[0], 0, 0xf);   // Unpacks bits ____#### of msg byte 0
    InverterVsmState INVR_VsmState_val = CAN_DECODE(INVR_VsmState_raw, CANSIG_INVR_INTERNAL_STATES_INVR_VSM_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_VSM_STATE_OFFSET, uint32_t);
    out_msg->INVR_VsmState_value = INVR_VsmState_val;
    
    // Unpack 8-bit signal INVR_PwmFrequency from payload (at bit 8 to bit 16).
    uint32_t INVR_PwmFrequency_raw = 0;
    INVR_PwmFrequency_raw |= unpackShiftRight(in_data[1], 0, 0xff);   // Unpacks bits ######## of msg byte 1
    uint32_t INVR_PwmFrequency_val = CAN_DECODE(INVR_PwmFrequency_raw, CANSIG_INVR_INTERNAL_STATES_INVR_PWM_FREQUENCY_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_PWM_FREQUENCY_OFFSET, uint32_t);
    out_msg->INVR_PwmFrequency_value = INVR_PwmFrequency_val;
    
    // Unpack 4-bit signal INVR_InverterState from payload (at bit 16 to bit 20).
    uint32_t INVR_InverterState_raw = 0;
    INVR_InverterState_raw |= unpackShiftRight(in_data[2], 0, 0xf);   // Unpacks bits ____#### of msg byte 2
    InverterState INVR_InverterState_val = CAN_DECODE(INVR_InverterState_raw, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_STATE_OFFSET, uint32_t);
    out_msg->INVR_InverterState_value = INVR_InverterState_val;
    
    // Unpack 1-bit signal INVR_Relay1State from payload (at bit 24 to bit 25).
    uint32_t INVR_Relay1State_raw = 0;
    INVR_Relay1State_raw |= unpackShiftRight(in_data[3], 0, 0x1);   // Unpacks bits _______# of msg byte 3
    bool INVR_Relay1State_val = CAN_DECODE(INVR_Relay1State_raw, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY1_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY1_STATE_OFFSET, uint32_t);
    out_msg->INVR_Relay1State_value = INVR_Relay1State_val;
    
    // Unpack 1-bit signal INVR_Relay2State from payload (at bit 25 to bit 26).
    uint32_t INVR_Relay2State_raw = 0;
    INVR_Relay2State_raw |= unpackShiftRight(in_data[3], 1, 0x2);   // Unpacks bits ______#_ of msg byte 3
    bool INVR_Relay2State_val = CAN_DECODE(INVR_Relay2State_raw, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY2_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY2_STATE_OFFSET, uint32_t);
    out_msg->INVR_Relay2State_value = INVR_Relay2State_val;
    
    // Unpack 1-bit signal INVR_Relay3State from payload (at bit 26 to bit 27).
    uint32_t INVR_Relay3State_raw = 0;
    INVR_Relay3State_raw |= unpackShiftRight(in_data[3], 2, 0x4);   // Unpacks bits _____#__ of msg byte 3
    bool INVR_Relay3State_val = CAN_DECODE(INVR_Relay3State_raw, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY3_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY3_STATE_OFFSET, uint32_t);
    out_msg->INVR_Relay3State_value = INVR_Relay3State_val;
    
    // Unpack 1-bit signal INVR_Relay4State from payload (at bit 27 to bit 28).
    uint32_t INVR_Relay4State_raw = 0;
    INVR_Relay4State_raw |= unpackShiftRight(in_data[3], 3, 0x8);   // Unpacks bits ____#___ of msg byte 3
    bool INVR_Relay4State_val = CAN_DECODE(INVR_Relay4State_raw, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY4_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY4_STATE_OFFSET, uint32_t);
    out_msg->INVR_Relay4State_value = INVR_Relay4State_val;
    
    // Unpack 1-bit signal INVR_Relay5State from payload (at bit 28 to bit 29).
    uint32_t INVR_Relay5State_raw = 0;
    INVR_Relay5State_raw |= unpackShiftRight(in_data[3], 4, 0x10);   // Unpacks bits ___#____ of msg byte 3
    bool INVR_Relay5State_val = CAN_DECODE(INVR_Relay5State_raw, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY5_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY5_STATE_OFFSET, uint32_t);
    out_msg->INVR_Relay5State_value = INVR_Relay5State_val;
    
    // Unpack 1-bit signal INVR_Relay6State from payload (at bit 29 to bit 30).
    uint32_t INVR_Relay6State_raw = 0;
    INVR_Relay6State_raw |= unpackShiftRight(in_data[3], 5, 0x20);   // Unpacks bits __#_____ of msg byte 3
    bool INVR_Relay6State_val = CAN_DECODE(INVR_Relay6State_raw, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY6_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_RELAY6_STATE_OFFSET, uint32_t);
    out_msg->INVR_Relay6State_value = INVR_Relay6State_val;
    
    // Unpack 1-bit signal INVR_InverterRunMode from payload (at bit 32 to bit 33).
    uint32_t INVR_InverterRunMode_raw = 0;
    INVR_InverterRunMode_raw |= unpackShiftRight(in_data[4], 0, 0x1);   // Unpacks bits _______# of msg byte 4
    bool INVR_InverterRunMode_val = CAN_DECODE(INVR_InverterRunMode_raw, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_RUN_MODE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_RUN_MODE_OFFSET, uint32_t);
    out_msg->INVR_InverterRunMode_value = INVR_InverterRunMode_val;
    
    // Unpack 3-bit signal INVR_InverterActiveDischargeState from payload (at bit 37 to bit 40).
    uint32_t INVR_InverterActiveDischargeState_raw = 0;
    INVR_InverterActiveDischargeState_raw |= unpackShiftRight(in_data[4], 5, 0xe0);   // Unpacks bits ###_____ of msg byte 4
    InverterActiveDischargeState INVR_InverterActiveDischargeState_val = CAN_DECODE(INVR_InverterActiveDischargeState_raw, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_ACTIVE_DISCHARGE_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_ACTIVE_DISCHARGE_STATE_OFFSET, uint32_t);
    out_msg->INVR_InverterActiveDischargeState_value = INVR_InverterActiveDischargeState_val;
    
    // Unpack 1-bit signal INVR_InverterCommandMode from payload (at bit 40 to bit 41).
    uint32_t INVR_InverterCommandMode_raw = 0;
    INVR_InverterCommandMode_raw |= unpackShiftRight(in_data[5], 0, 0x1);   // Unpacks bits _______# of msg byte 5
    InverterCommandMode INVR_InverterCommandMode_val = CAN_DECODE(INVR_InverterCommandMode_raw, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_COMMAND_MODE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_COMMAND_MODE_OFFSET, uint32_t);
    out_msg->INVR_InverterCommandMode_value = INVR_InverterCommandMode_val;
    
    // Unpack 4-bit signal INVR_RollingCounter from payload (at bit 44 to bit 48).
    uint32_t INVR_RollingCounter_raw = 0;
    INVR_RollingCounter_raw |= unpackShiftRight(in_data[5], 4, 0xf0);   // Unpacks bits ####____ of msg byte 5
    uint32_t INVR_RollingCounter_val = CAN_DECODE(INVR_RollingCounter_raw, CANSIG_INVR_INTERNAL_STATES_INVR_ROLLING_COUNTER_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_ROLLING_COUNTER_OFFSET, uint32_t);
    out_msg->INVR_RollingCounter_value = INVR_RollingCounter_val;
    
    // Unpack 1-bit signal INVR_InverterEnableState from payload (at bit 48 to bit 49).
    uint32_t INVR_InverterEnableState_raw = 0;
    INVR_InverterEnableState_raw |= unpackShiftRight(in_data[6], 0, 0x1);   // Unpacks bits _______# of msg byte 6
    bool INVR_InverterEnableState_val = CAN_DECODE(INVR_InverterEnableState_raw, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_ENABLE_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_ENABLE_STATE_OFFSET, uint32_t);
    out_msg->INVR_InverterEnableState_value = INVR_InverterEnableState_val;
    
    // Unpack 1-bit signal INVR_StartModeActive from payload (at bit 54 to bit 55).
    uint32_t INVR_StartModeActive_raw = 0;
    INVR_StartModeActive_raw |= unpackShiftRight(in_data[6], 6, 0x40);   // Unpacks bits _#______ of msg byte 6
    bool INVR_StartModeActive_val = CAN_DECODE(INVR_StartModeActive_raw, CANSIG_INVR_INTERNAL_STATES_INVR_START_MODE_ACTIVE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_START_MODE_ACTIVE_OFFSET, uint32_t);
    out_msg->INVR_StartModeActive_value = INVR_StartModeActive_val;
    
    // Unpack 1-bit signal INVR_InverterLockoutEnabled from payload (at bit 55 to bit 56).
    uint32_t INVR_InverterLockoutEnabled_raw = 0;
    INVR_InverterLockoutEnabled_raw |= unpackShiftRight(in_data[6], 7, 0x80);   // Unpacks bits #_______ of msg byte 6
    InverterLockout INVR_InverterLockoutEnabled_val = CAN_DECODE(INVR_InverterLockoutEnabled_raw, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_LOCKOUT_ENABLED_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_INVERTER_LOCKOUT_ENABLED_OFFSET, uint32_t);
    out_msg->INVR_InverterLockoutEnabled_value = INVR_InverterLockoutEnabled_val;
    
    // Unpack 1-bit signal INVR_DirectionCommand from payload (at bit 56 to bit 57).
    uint32_t INVR_DirectionCommand_raw = 0;
    INVR_DirectionCommand_raw |= unpackShiftRight(in_data[7], 0, 0x1);   // Unpacks bits _______# of msg byte 7
    InverterDirection INVR_DirectionCommand_val = CAN_DECODE(INVR_DirectionCommand_raw, CANSIG_INVR_INTERNAL_STATES_INVR_DIRECTION_COMMAND_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_msg->INVR_DirectionCommand_value = INVR_DirectionCommand_val;
    
    // Unpack 1-bit signal INVR_BmsActive from payload (at bit 57 to bit 58).
    uint32_t INVR_BmsActive_raw = 0;
    INVR_BmsActive_raw |= unpackShiftRight(in_data[7], 1, 0x2);   // Unpacks bits ______#_ of msg byte 7
    bool INVR_BmsActive_val = CAN_DECODE(INVR_BmsActive_raw, CANSIG_INVR_INTERNAL_STATES_INVR_BMS_ACTIVE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_BMS_ACTIVE_OFFSET, uint32_t);
    out_msg->INVR_BmsActive_value = INVR_BmsActive_val;
    
    // Unpack 1-bit signal INVR_BmsTorqueLimiting from payload (at bit 58 to bit 59).
    uint32_t INVR_BmsTorqueLimiting_raw = 0;
    INVR_BmsTorqueLimiting_raw |= unpackShiftRight(in_data[7], 2, 0x4);   // Unpacks bits _____#__ of msg byte 7
    bool INVR_BmsTorqueLimiting_val = CAN_DECODE(INVR_BmsTorqueLimiting_raw, CANSIG_INVR_INTERNAL_STATES_INVR_BMS_TORQUE_LIMITING_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_BMS_TORQUE_LIMITING_OFFSET, uint32_t);
    out_msg->INVR_BmsTorqueLimiting_value = INVR_BmsTorqueLimiting_val;
    
    // Unpack 1-bit signal INVR_MaxSpeedLimiting from payload (at bit 59 to bit 60).
    uint32_t INVR_MaxSpeedLimiting_raw = 0;
    INVR_MaxSpeedLimiting_raw |= unpackShiftRight(in_data[7], 3, 0x8);   // Unpacks bits ____#___ of msg byte 7
    bool INVR_MaxSpeedLimiting_val = CAN_DECODE(INVR_MaxSpeedLimiting_raw, CANSIG_INVR_INTERNAL_STATES_INVR_MAX_SPEED_LIMITING_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_MAX_SPEED_LIMITING_OFFSET, uint32_t);
    out_msg->INVR_MaxSpeedLimiting_value = INVR_MaxSpeedLimiting_val;
    
    // Unpack 1-bit signal INVR_HotSpotLimiting from payload (at bit 60 to bit 61).
    uint32_t INVR_HotSpotLimiting_raw = 0;
    INVR_HotSpotLimiting_raw |= unpackShiftRight(in_data[7], 4, 0x10);   // Unpacks bits ___#____ of msg byte 7
    bool INVR_HotSpotLimiting_val = CAN_DECODE(INVR_HotSpotLimiting_raw, CANSIG_INVR_INTERNAL_STATES_INVR_HOT_SPOT_LIMITING_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_HOT_SPOT_LIMITING_OFFSET, uint32_t);
    out_msg->INVR_HotSpotLimiting_value = INVR_HotSpotLimiting_val;
    
    // Unpack 1-bit signal INVR_LowSpeedLimiting from payload (at bit 61 to bit 62).
    uint32_t INVR_LowSpeedLimiting_raw = 0;
    INVR_LowSpeedLimiting_raw |= unpackShiftRight(in_data[7], 5, 0x20);   // Unpacks bits __#_____ of msg byte 7
    bool INVR_LowSpeedLimiting_val = CAN_DECODE(INVR_LowSpeedLimiting_raw, CANSIG_INVR_INTERNAL_STATES_INVR_LOW_SPEED_LIMITING_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_LOW_SPEED_LIMITING_OFFSET, uint32_t);
    out_msg->INVR_LowSpeedLimiting_value = INVR_LowSpeedLimiting_val;
    
    // Unpack 1-bit signal INVR_CoolantTemperatureLimiting from payload (at bit 62 to bit 63).
    uint32_t INVR_CoolantTemperatureLimiting_raw = 0;
    INVR_CoolantTemperatureLimiting_raw |= unpackShiftRight(in_data[7], 6, 0x40);   // Unpacks bits _#______ of msg byte 7
    bool INVR_CoolantTemperatureLimiting_val = CAN_DECODE(INVR_CoolantTemperatureLimiting_raw, CANSIG_INVR_INTERNAL_STATES_INVR_COOLANT_TEMPERATURE_LIMITING_SCALE, CANSIG_INVR_INTERNAL_STATES_INVR_COOLANT_TEMPERATURE_LIMITING_OFFSET, uint32_t);
    out_msg->INVR_CoolantTemperatureLimiting_value = INVR_CoolantTemperatureLimiting_val;
    
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

void App_CanUtils_BMS_AvailablePower_Unpack(const uint8_t* const in_data, BMS_AvailablePower_Signals* const out_msg)
{
    // Unpack 3-byte payload for message BMS_AvailablePower.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____AAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 20-bit signal BMS_AvailablePower from payload (at bit 0 to bit 20).
    uint32_t BMS_AvailablePower_raw = 0;
    BMS_AvailablePower_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    BMS_AvailablePower_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    BMS_AvailablePower_raw |= unpackShiftLeft(in_data[2], 16, 0xf);   // Unpacks bits ____#### of msg byte 2
    float BMS_AvailablePower_val = CAN_DECODE(BMS_AvailablePower_raw, CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_SCALE, CANSIG_BMS_AVAILABLE_POWER_BMS_AVAILABLE_POWER_OFFSET, float);
    out_msg->BMS_AvailablePower_value = BMS_AvailablePower_val;
    
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

void App_CanUtils_FSM_Apps_Unpack(const uint8_t* const in_data, FSM_Apps_Signals* const out_msg)
{
    // Unpack 7-byte payload for message FSM_Apps.
    // |xxxxxxxx|____FEEE|EEEEEEEE|DDDDDDDD|DDCCCCCC|CCCCBBBB|BBBBBBAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 10-bit signal FSM_PappsMappedPedalPercentage from payload (at bit 0 to bit 10).
    uint32_t FSM_PappsMappedPedalPercentage_raw = 0;
    FSM_PappsMappedPedalPercentage_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    FSM_PappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[1], 8, 0x3);   // Unpacks bits ______## of msg byte 1
    float FSM_PappsMappedPedalPercentage_val = CAN_DECODE(FSM_PappsMappedPedalPercentage_raw, CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET, float);
    out_msg->FSM_PappsMappedPedalPercentage_value = FSM_PappsMappedPedalPercentage_val;
    
    // Unpack 10-bit signal FSM_SappsMappedPedalPercentage from payload (at bit 10 to bit 20).
    uint32_t FSM_SappsMappedPedalPercentage_raw = 0;
    FSM_SappsMappedPedalPercentage_raw |= unpackShiftRight(in_data[1], 2, 0xfc);   // Unpacks bits ######__ of msg byte 1
    FSM_SappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[2], 6, 0xf);   // Unpacks bits ____#### of msg byte 2
    float FSM_SappsMappedPedalPercentage_val = CAN_DECODE(FSM_SappsMappedPedalPercentage_raw, CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET, float);
    out_msg->FSM_SappsMappedPedalPercentage_value = FSM_SappsMappedPedalPercentage_val;
    
    // Unpack 10-bit signal FSM_PappsRawPedalPercentage from payload (at bit 20 to bit 30).
    uint32_t FSM_PappsRawPedalPercentage_raw = 0;
    FSM_PappsRawPedalPercentage_raw |= unpackShiftRight(in_data[2], 4, 0xf0);   // Unpacks bits ####____ of msg byte 2
    FSM_PappsRawPedalPercentage_raw |= unpackShiftLeft(in_data[3], 4, 0x3f);   // Unpacks bits __###### of msg byte 3
    float FSM_PappsRawPedalPercentage_val = CAN_DECODE(FSM_PappsRawPedalPercentage_raw, CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_FSM_PAPPS_RAW_PEDAL_PERCENTAGE_OFFSET, float);
    out_msg->FSM_PappsRawPedalPercentage_value = FSM_PappsRawPedalPercentage_val;
    
    // Unpack 10-bit signal FSM_SappsRawPedalPercentage from payload (at bit 30 to bit 40).
    uint32_t FSM_SappsRawPedalPercentage_raw = 0;
    FSM_SappsRawPedalPercentage_raw |= unpackShiftRight(in_data[3], 6, 0xc0);   // Unpacks bits ##______ of msg byte 3
    FSM_SappsRawPedalPercentage_raw |= unpackShiftLeft(in_data[4], 2, 0xff);   // Unpacks bits ######## of msg byte 4
    float FSM_SappsRawPedalPercentage_val = CAN_DECODE(FSM_SappsRawPedalPercentage_raw, CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_FSM_SAPPS_RAW_PEDAL_PERCENTAGE_OFFSET, float);
    out_msg->FSM_SappsRawPedalPercentage_value = FSM_SappsRawPedalPercentage_val;
    
    // Unpack 11-bit signal FSM_TorqueLimit from payload (at bit 40 to bit 51).
    uint32_t FSM_TorqueLimit_raw = 0;
    FSM_TorqueLimit_raw |= unpackShiftRight(in_data[5], 0, 0xff);   // Unpacks bits ######## of msg byte 5
    FSM_TorqueLimit_raw |= unpackShiftLeft(in_data[6], 8, 0x7);   // Unpacks bits _____### of msg byte 6
    float FSM_TorqueLimit_val = CAN_DECODE(FSM_TorqueLimit_raw, CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_SCALE, CANSIG_FSM_APPS_FSM_TORQUE_LIMIT_OFFSET, float);
    out_msg->FSM_TorqueLimit_value = FSM_TorqueLimit_val;
    
    // Unpack 1-bit signal FSM_TorquePlausabilityFailed from payload (at bit 51 to bit 52).
    uint32_t FSM_TorquePlausabilityFailed_raw = 0;
    FSM_TorquePlausabilityFailed_raw |= unpackShiftRight(in_data[6], 3, 0x8);   // Unpacks bits ____#___ of msg byte 6
    bool FSM_TorquePlausabilityFailed_val = CAN_DECODE(FSM_TorquePlausabilityFailed_raw, CANSIG_FSM_APPS_FSM_TORQUE_PLAUSABILITY_FAILED_SCALE, CANSIG_FSM_APPS_FSM_TORQUE_PLAUSABILITY_FAILED_OFFSET, uint32_t);
    out_msg->FSM_TorquePlausabilityFailed_value = FSM_TorquePlausabilityFailed_val;
    
}

void App_CanUtils_FSM_Brake_Unpack(const uint8_t* const in_data, FSM_Brake_Signals* const out_msg)
{
    // Unpack 5-byte payload for message FSM_Brake.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|___FEDCC|CCCCCCCC|BBBBBBBB|BBBBAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 12-bit signal FSM_FrontBrakePressure from payload (at bit 0 to bit 12).
    uint32_t FSM_FrontBrakePressure_raw = 0;
    FSM_FrontBrakePressure_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    FSM_FrontBrakePressure_raw |= unpackShiftLeft(in_data[1], 8, 0xf);   // Unpacks bits ____#### of msg byte 1
    uint32_t FSM_FrontBrakePressure_val = CAN_DECODE(FSM_FrontBrakePressure_raw, CANSIG_FSM_BRAKE_FSM_FRONT_BRAKE_PRESSURE_SCALE, CANSIG_FSM_BRAKE_FSM_FRONT_BRAKE_PRESSURE_OFFSET, uint32_t);
    out_msg->FSM_FrontBrakePressure_value = FSM_FrontBrakePressure_val;
    
    // Unpack 12-bit signal FSM_RearBrakePressure from payload (at bit 12 to bit 24).
    uint32_t FSM_RearBrakePressure_raw = 0;
    FSM_RearBrakePressure_raw |= unpackShiftRight(in_data[1], 4, 0xf0);   // Unpacks bits ####____ of msg byte 1
    FSM_RearBrakePressure_raw |= unpackShiftLeft(in_data[2], 4, 0xff);   // Unpacks bits ######## of msg byte 2
    uint32_t FSM_RearBrakePressure_val = CAN_DECODE(FSM_RearBrakePressure_raw, CANSIG_FSM_BRAKE_FSM_REAR_BRAKE_PRESSURE_SCALE, CANSIG_FSM_BRAKE_FSM_REAR_BRAKE_PRESSURE_OFFSET, uint32_t);
    out_msg->FSM_RearBrakePressure_value = FSM_RearBrakePressure_val;
    
    // Unpack 10-bit signal FSM_BrakePedalPercentage from payload (at bit 24 to bit 34).
    uint32_t FSM_BrakePedalPercentage_raw = 0;
    FSM_BrakePedalPercentage_raw |= unpackShiftRight(in_data[3], 0, 0xff);   // Unpacks bits ######## of msg byte 3
    FSM_BrakePedalPercentage_raw |= unpackShiftLeft(in_data[4], 8, 0x3);   // Unpacks bits ______## of msg byte 4
    float FSM_BrakePedalPercentage_val = CAN_DECODE(FSM_BrakePedalPercentage_raw, CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_PERCENTAGE_OFFSET, float);
    out_msg->FSM_BrakePedalPercentage_value = FSM_BrakePedalPercentage_val;
    
    // Unpack 1-bit signal FSM_BrakeActuated from payload (at bit 34 to bit 35).
    uint32_t FSM_BrakeActuated_raw = 0;
    FSM_BrakeActuated_raw |= unpackShiftRight(in_data[4], 2, 0x4);   // Unpacks bits _____#__ of msg byte 4
    bool FSM_BrakeActuated_val = CAN_DECODE(FSM_BrakeActuated_raw, CANSIG_FSM_BRAKE_FSM_BRAKE_ACTUATED_SCALE, CANSIG_FSM_BRAKE_FSM_BRAKE_ACTUATED_OFFSET, uint32_t);
    out_msg->FSM_BrakeActuated_value = FSM_BrakeActuated_val;
    
    // Unpack 1-bit signal FSM_BrakePressureSensorOCSC from payload (at bit 35 to bit 36).
    uint32_t FSM_BrakePressureSensorOCSC_raw = 0;
    FSM_BrakePressureSensorOCSC_raw |= unpackShiftRight(in_data[4], 3, 0x8);   // Unpacks bits ____#___ of msg byte 4
    bool FSM_BrakePressureSensorOCSC_val = CAN_DECODE(FSM_BrakePressureSensorOCSC_raw, CANSIG_FSM_BRAKE_FSM_BRAKE_PRESSURE_SENSOR_OCSC_SCALE, CANSIG_FSM_BRAKE_FSM_BRAKE_PRESSURE_SENSOR_OCSC_OFFSET, uint32_t);
    out_msg->FSM_BrakePressureSensorOCSC_value = FSM_BrakePressureSensorOCSC_val;
    
    // Unpack 1-bit signal FSM_BrakePedalSensorOCSC from payload (at bit 36 to bit 37).
    uint32_t FSM_BrakePedalSensorOCSC_raw = 0;
    FSM_BrakePedalSensorOCSC_raw |= unpackShiftRight(in_data[4], 4, 0x10);   // Unpacks bits ___#____ of msg byte 4
    bool FSM_BrakePedalSensorOCSC_val = CAN_DECODE(FSM_BrakePedalSensorOCSC_raw, CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_SENSOR_OCSC_SCALE, CANSIG_FSM_BRAKE_FSM_BRAKE_PEDAL_SENSOR_OCSC_OFFSET, uint32_t);
    out_msg->FSM_BrakePedalSensorOCSC_value = FSM_BrakePedalSensorOCSC_val;
    
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

void App_CanUtils_INVL_InternalStates_Unpack(const uint8_t* const in_data, INVL_InternalStates_Signals* const out_msg)
{
    // Unpack 8-byte payload for message INVL_InternalStates.
    // |_WVUTSRQ|PO_____N|MMMM___L|KKK____J|__IHGFED|____CCCC|BBBBBBBB|____AAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 4-bit signal INVL_VsmState from payload (at bit 0 to bit 4).
    uint32_t INVL_VsmState_raw = 0;
    INVL_VsmState_raw |= unpackShiftRight(in_data[0], 0, 0xf);   // Unpacks bits ____#### of msg byte 0
    InverterVsmState INVL_VsmState_val = CAN_DECODE(INVL_VsmState_raw, CANSIG_INVL_INTERNAL_STATES_INVL_VSM_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_VSM_STATE_OFFSET, uint32_t);
    out_msg->INVL_VsmState_value = INVL_VsmState_val;
    
    // Unpack 8-bit signal INVL_PwmFrequency from payload (at bit 8 to bit 16).
    uint32_t INVL_PwmFrequency_raw = 0;
    INVL_PwmFrequency_raw |= unpackShiftRight(in_data[1], 0, 0xff);   // Unpacks bits ######## of msg byte 1
    uint32_t INVL_PwmFrequency_val = CAN_DECODE(INVL_PwmFrequency_raw, CANSIG_INVL_INTERNAL_STATES_INVL_PWM_FREQUENCY_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_PWM_FREQUENCY_OFFSET, uint32_t);
    out_msg->INVL_PwmFrequency_value = INVL_PwmFrequency_val;
    
    // Unpack 4-bit signal INVL_InverterState from payload (at bit 16 to bit 20).
    uint32_t INVL_InverterState_raw = 0;
    INVL_InverterState_raw |= unpackShiftRight(in_data[2], 0, 0xf);   // Unpacks bits ____#### of msg byte 2
    InverterState INVL_InverterState_val = CAN_DECODE(INVL_InverterState_raw, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_STATE_OFFSET, uint32_t);
    out_msg->INVL_InverterState_value = INVL_InverterState_val;
    
    // Unpack 1-bit signal INVL_Relay1State from payload (at bit 24 to bit 25).
    uint32_t INVL_Relay1State_raw = 0;
    INVL_Relay1State_raw |= unpackShiftRight(in_data[3], 0, 0x1);   // Unpacks bits _______# of msg byte 3
    bool INVL_Relay1State_val = CAN_DECODE(INVL_Relay1State_raw, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY1_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY1_STATE_OFFSET, uint32_t);
    out_msg->INVL_Relay1State_value = INVL_Relay1State_val;
    
    // Unpack 1-bit signal INVL_Relay2State from payload (at bit 25 to bit 26).
    uint32_t INVL_Relay2State_raw = 0;
    INVL_Relay2State_raw |= unpackShiftRight(in_data[3], 1, 0x2);   // Unpacks bits ______#_ of msg byte 3
    bool INVL_Relay2State_val = CAN_DECODE(INVL_Relay2State_raw, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY2_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY2_STATE_OFFSET, uint32_t);
    out_msg->INVL_Relay2State_value = INVL_Relay2State_val;
    
    // Unpack 1-bit signal INVL_Relay3State from payload (at bit 26 to bit 27).
    uint32_t INVL_Relay3State_raw = 0;
    INVL_Relay3State_raw |= unpackShiftRight(in_data[3], 2, 0x4);   // Unpacks bits _____#__ of msg byte 3
    bool INVL_Relay3State_val = CAN_DECODE(INVL_Relay3State_raw, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY3_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY3_STATE_OFFSET, uint32_t);
    out_msg->INVL_Relay3State_value = INVL_Relay3State_val;
    
    // Unpack 1-bit signal INVL_Relay4State from payload (at bit 27 to bit 28).
    uint32_t INVL_Relay4State_raw = 0;
    INVL_Relay4State_raw |= unpackShiftRight(in_data[3], 3, 0x8);   // Unpacks bits ____#___ of msg byte 3
    bool INVL_Relay4State_val = CAN_DECODE(INVL_Relay4State_raw, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY4_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY4_STATE_OFFSET, uint32_t);
    out_msg->INVL_Relay4State_value = INVL_Relay4State_val;
    
    // Unpack 1-bit signal INVL_Relay5State from payload (at bit 28 to bit 29).
    uint32_t INVL_Relay5State_raw = 0;
    INVL_Relay5State_raw |= unpackShiftRight(in_data[3], 4, 0x10);   // Unpacks bits ___#____ of msg byte 3
    bool INVL_Relay5State_val = CAN_DECODE(INVL_Relay5State_raw, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY5_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY5_STATE_OFFSET, uint32_t);
    out_msg->INVL_Relay5State_value = INVL_Relay5State_val;
    
    // Unpack 1-bit signal INVL_Relay6State from payload (at bit 29 to bit 30).
    uint32_t INVL_Relay6State_raw = 0;
    INVL_Relay6State_raw |= unpackShiftRight(in_data[3], 5, 0x20);   // Unpacks bits __#_____ of msg byte 3
    bool INVL_Relay6State_val = CAN_DECODE(INVL_Relay6State_raw, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY6_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_RELAY6_STATE_OFFSET, uint32_t);
    out_msg->INVL_Relay6State_value = INVL_Relay6State_val;
    
    // Unpack 1-bit signal INVL_InverterRunMode from payload (at bit 32 to bit 33).
    uint32_t INVL_InverterRunMode_raw = 0;
    INVL_InverterRunMode_raw |= unpackShiftRight(in_data[4], 0, 0x1);   // Unpacks bits _______# of msg byte 4
    bool INVL_InverterRunMode_val = CAN_DECODE(INVL_InverterRunMode_raw, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_RUN_MODE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_RUN_MODE_OFFSET, uint32_t);
    out_msg->INVL_InverterRunMode_value = INVL_InverterRunMode_val;
    
    // Unpack 3-bit signal INVL_InverterActiveDischargeState from payload (at bit 37 to bit 40).
    uint32_t INVL_InverterActiveDischargeState_raw = 0;
    INVL_InverterActiveDischargeState_raw |= unpackShiftRight(in_data[4], 5, 0xe0);   // Unpacks bits ###_____ of msg byte 4
    InverterActiveDischargeState INVL_InverterActiveDischargeState_val = CAN_DECODE(INVL_InverterActiveDischargeState_raw, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_ACTIVE_DISCHARGE_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_ACTIVE_DISCHARGE_STATE_OFFSET, uint32_t);
    out_msg->INVL_InverterActiveDischargeState_value = INVL_InverterActiveDischargeState_val;
    
    // Unpack 1-bit signal INVL_InverterCommandMode from payload (at bit 40 to bit 41).
    uint32_t INVL_InverterCommandMode_raw = 0;
    INVL_InverterCommandMode_raw |= unpackShiftRight(in_data[5], 0, 0x1);   // Unpacks bits _______# of msg byte 5
    InverterCommandMode INVL_InverterCommandMode_val = CAN_DECODE(INVL_InverterCommandMode_raw, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_COMMAND_MODE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_COMMAND_MODE_OFFSET, uint32_t);
    out_msg->INVL_InverterCommandMode_value = INVL_InverterCommandMode_val;
    
    // Unpack 4-bit signal INVL_RollingCounter from payload (at bit 44 to bit 48).
    uint32_t INVL_RollingCounter_raw = 0;
    INVL_RollingCounter_raw |= unpackShiftRight(in_data[5], 4, 0xf0);   // Unpacks bits ####____ of msg byte 5
    uint32_t INVL_RollingCounter_val = CAN_DECODE(INVL_RollingCounter_raw, CANSIG_INVL_INTERNAL_STATES_INVL_ROLLING_COUNTER_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_ROLLING_COUNTER_OFFSET, uint32_t);
    out_msg->INVL_RollingCounter_value = INVL_RollingCounter_val;
    
    // Unpack 1-bit signal INVL_InverterEnableState from payload (at bit 48 to bit 49).
    uint32_t INVL_InverterEnableState_raw = 0;
    INVL_InverterEnableState_raw |= unpackShiftRight(in_data[6], 0, 0x1);   // Unpacks bits _______# of msg byte 6
    bool INVL_InverterEnableState_val = CAN_DECODE(INVL_InverterEnableState_raw, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_ENABLE_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_ENABLE_STATE_OFFSET, uint32_t);
    out_msg->INVL_InverterEnableState_value = INVL_InverterEnableState_val;
    
    // Unpack 1-bit signal INVL_StartModeActive from payload (at bit 54 to bit 55).
    uint32_t INVL_StartModeActive_raw = 0;
    INVL_StartModeActive_raw |= unpackShiftRight(in_data[6], 6, 0x40);   // Unpacks bits _#______ of msg byte 6
    bool INVL_StartModeActive_val = CAN_DECODE(INVL_StartModeActive_raw, CANSIG_INVL_INTERNAL_STATES_INVL_START_MODE_ACTIVE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_START_MODE_ACTIVE_OFFSET, uint32_t);
    out_msg->INVL_StartModeActive_value = INVL_StartModeActive_val;
    
    // Unpack 1-bit signal INVL_InverterLockoutEnabled from payload (at bit 55 to bit 56).
    uint32_t INVL_InverterLockoutEnabled_raw = 0;
    INVL_InverterLockoutEnabled_raw |= unpackShiftRight(in_data[6], 7, 0x80);   // Unpacks bits #_______ of msg byte 6
    InverterLockout INVL_InverterLockoutEnabled_val = CAN_DECODE(INVL_InverterLockoutEnabled_raw, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_LOCKOUT_ENABLED_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_INVERTER_LOCKOUT_ENABLED_OFFSET, uint32_t);
    out_msg->INVL_InverterLockoutEnabled_value = INVL_InverterLockoutEnabled_val;
    
    // Unpack 1-bit signal INVL_DirectionCommand from payload (at bit 56 to bit 57).
    uint32_t INVL_DirectionCommand_raw = 0;
    INVL_DirectionCommand_raw |= unpackShiftRight(in_data[7], 0, 0x1);   // Unpacks bits _______# of msg byte 7
    InverterDirection INVL_DirectionCommand_val = CAN_DECODE(INVL_DirectionCommand_raw, CANSIG_INVL_INTERNAL_STATES_INVL_DIRECTION_COMMAND_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_msg->INVL_DirectionCommand_value = INVL_DirectionCommand_val;
    
    // Unpack 1-bit signal INVL_BmsActive from payload (at bit 57 to bit 58).
    uint32_t INVL_BmsActive_raw = 0;
    INVL_BmsActive_raw |= unpackShiftRight(in_data[7], 1, 0x2);   // Unpacks bits ______#_ of msg byte 7
    bool INVL_BmsActive_val = CAN_DECODE(INVL_BmsActive_raw, CANSIG_INVL_INTERNAL_STATES_INVL_BMS_ACTIVE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_BMS_ACTIVE_OFFSET, uint32_t);
    out_msg->INVL_BmsActive_value = INVL_BmsActive_val;
    
    // Unpack 1-bit signal INVL_BmsTorqueLimiting from payload (at bit 58 to bit 59).
    uint32_t INVL_BmsTorqueLimiting_raw = 0;
    INVL_BmsTorqueLimiting_raw |= unpackShiftRight(in_data[7], 2, 0x4);   // Unpacks bits _____#__ of msg byte 7
    bool INVL_BmsTorqueLimiting_val = CAN_DECODE(INVL_BmsTorqueLimiting_raw, CANSIG_INVL_INTERNAL_STATES_INVL_BMS_TORQUE_LIMITING_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_BMS_TORQUE_LIMITING_OFFSET, uint32_t);
    out_msg->INVL_BmsTorqueLimiting_value = INVL_BmsTorqueLimiting_val;
    
    // Unpack 1-bit signal INVL_MaxSpeedLimiting from payload (at bit 59 to bit 60).
    uint32_t INVL_MaxSpeedLimiting_raw = 0;
    INVL_MaxSpeedLimiting_raw |= unpackShiftRight(in_data[7], 3, 0x8);   // Unpacks bits ____#___ of msg byte 7
    bool INVL_MaxSpeedLimiting_val = CAN_DECODE(INVL_MaxSpeedLimiting_raw, CANSIG_INVL_INTERNAL_STATES_INVL_MAX_SPEED_LIMITING_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_MAX_SPEED_LIMITING_OFFSET, uint32_t);
    out_msg->INVL_MaxSpeedLimiting_value = INVL_MaxSpeedLimiting_val;
    
    // Unpack 1-bit signal INVL_HotSpotLimiting from payload (at bit 60 to bit 61).
    uint32_t INVL_HotSpotLimiting_raw = 0;
    INVL_HotSpotLimiting_raw |= unpackShiftRight(in_data[7], 4, 0x10);   // Unpacks bits ___#____ of msg byte 7
    bool INVL_HotSpotLimiting_val = CAN_DECODE(INVL_HotSpotLimiting_raw, CANSIG_INVL_INTERNAL_STATES_INVL_HOT_SPOT_LIMITING_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_HOT_SPOT_LIMITING_OFFSET, uint32_t);
    out_msg->INVL_HotSpotLimiting_value = INVL_HotSpotLimiting_val;
    
    // Unpack 1-bit signal INVL_LowSpeedLimiting from payload (at bit 61 to bit 62).
    uint32_t INVL_LowSpeedLimiting_raw = 0;
    INVL_LowSpeedLimiting_raw |= unpackShiftRight(in_data[7], 5, 0x20);   // Unpacks bits __#_____ of msg byte 7
    bool INVL_LowSpeedLimiting_val = CAN_DECODE(INVL_LowSpeedLimiting_raw, CANSIG_INVL_INTERNAL_STATES_INVL_LOW_SPEED_LIMITING_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_LOW_SPEED_LIMITING_OFFSET, uint32_t);
    out_msg->INVL_LowSpeedLimiting_value = INVL_LowSpeedLimiting_val;
    
    // Unpack 1-bit signal INVL_CoolantTemperatureLimiting from payload (at bit 62 to bit 63).
    uint32_t INVL_CoolantTemperatureLimiting_raw = 0;
    INVL_CoolantTemperatureLimiting_raw |= unpackShiftRight(in_data[7], 6, 0x40);   // Unpacks bits _#______ of msg byte 7
    bool INVL_CoolantTemperatureLimiting_val = CAN_DECODE(INVL_CoolantTemperatureLimiting_raw, CANSIG_INVL_INTERNAL_STATES_INVL_COOLANT_TEMPERATURE_LIMITING_SCALE, CANSIG_INVL_INTERNAL_STATES_INVL_COOLANT_TEMPERATURE_LIMITING_OFFSET, uint32_t);
    out_msg->INVL_CoolantTemperatureLimiting_value = INVL_CoolantTemperatureLimiting_val;
    
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

