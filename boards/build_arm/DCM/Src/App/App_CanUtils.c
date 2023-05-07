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
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____BBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal Heartbeat into payload (at bit 0 to bit 1).
    const bool Heartbeat_val = in_msg->Heartbeat_value;
    const uint32_t Heartbeat_raw = CAN_ENCODE(Heartbeat_val, CANSIG_DCM_VITALS_HEARTBEAT_SCALE, CANSIG_DCM_VITALS_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 2-bit signal CurrentState into payload (at bit 1 to bit 3).
    const DcmState CurrentState_val = in_msg->CurrentState_value;
    const uint32_t CurrentState_raw = CAN_ENCODE(CurrentState_val, CANSIG_DCM_VITALS_CURRENT_STATE_SCALE, CANSIG_DCM_VITALS_CURRENT_STATE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(CurrentState_raw, 1, 0x6);   // Packs bits _____##_ of byte 0
    
}

void App_CanUtils_DCM_LeftInverterCommand_Pack(const DCM_LeftInverterCommand_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 8-byte payload for message DCM_LeftInverterCommand.
    // |GGGGGGGG|GGGGGGGG|_____FED|_______C|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal TorqueCommand into payload (at bit 0 to bit 16).
    const float TorqueCommand_val = in_msg->TorqueCommand_value;
    const uint32_t TorqueCommand_raw = CAN_ENCODE(TorqueCommand_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_COMMAND_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_COMMAND_OFFSET, float);
    out_data[0] |= packShiftRight(TorqueCommand_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(TorqueCommand_raw, 8, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 16-bit signal SpeedCommand into payload (at bit 16 to bit 32).
    const int SpeedCommand_val = in_msg->SpeedCommand_value;
    const uint32_t SpeedCommand_raw = CAN_ENCODE(SpeedCommand_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_SPEED_COMMAND_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_SPEED_COMMAND_OFFSET, int);
    out_data[2] |= packShiftRight(SpeedCommand_raw, 0, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(SpeedCommand_raw, 8, 0xff);   // Packs bits ######## of byte 3
    
    // Pack 1-bit signal DirectionCommand into payload (at bit 32 to bit 33).
    const InverterDirection DirectionCommand_val = in_msg->DirectionCommand_value;
    const uint32_t DirectionCommand_raw = CAN_ENCODE(DirectionCommand_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_DIRECTION_COMMAND_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(DirectionCommand_raw, 0, 0x1);   // Packs bits _______# of byte 4
    
    // Pack 1-bit signal EnableInverter into payload (at bit 40 to bit 41).
    const bool EnableInverter_val = in_msg->EnableInverter_value;
    const uint32_t EnableInverter_raw = CAN_ENCODE(EnableInverter_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_ENABLE_INVERTER_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_ENABLE_INVERTER_OFFSET, uint32_t);
    out_data[5] |= packShiftRight(EnableInverter_raw, 0, 0x1);   // Packs bits _______# of byte 5
    
    // Pack 1-bit signal EnableDischarge into payload (at bit 41 to bit 42).
    const bool EnableDischarge_val = in_msg->EnableDischarge_value;
    const uint32_t EnableDischarge_raw = CAN_ENCODE(EnableDischarge_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_ENABLE_DISCHARGE_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_ENABLE_DISCHARGE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(EnableDischarge_raw, 1, 0x2);   // Packs bits ______#_ of byte 5
    
    // Pack 1-bit signal EnableSpeedMode into payload (at bit 42 to bit 43).
    const bool EnableSpeedMode_val = in_msg->EnableSpeedMode_value;
    const uint32_t EnableSpeedMode_raw = CAN_ENCODE(EnableSpeedMode_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_ENABLE_SPEED_MODE_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_ENABLE_SPEED_MODE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(EnableSpeedMode_raw, 2, 0x4);   // Packs bits _____#__ of byte 5
    
    // Pack 16-bit signal TorqueLimit into payload (at bit 48 to bit 64).
    const float TorqueLimit_val = in_msg->TorqueLimit_value;
    const uint32_t TorqueLimit_raw = CAN_ENCODE(TorqueLimit_val, CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_LIMIT_SCALE, CANSIG_DCM_LEFT_INVERTER_COMMAND_TORQUE_LIMIT_OFFSET, float);
    out_data[6] |= packShiftRight(TorqueLimit_raw, 0, 0xff);   // Packs bits ######## of byte 6
    out_data[7] |= packShiftRight(TorqueLimit_raw, 8, 0xff);   // Packs bits ######## of byte 7
    
}

void App_CanUtils_DCM_RightInverterCommand_Pack(const DCM_RightInverterCommand_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 8-byte payload for message DCM_RightInverterCommand.
    // |GGGGGGGG|GGGGGGGG|_____FED|_______C|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal TorqueCommand into payload (at bit 0 to bit 16).
    const float TorqueCommand_val = in_msg->TorqueCommand_value;
    const uint32_t TorqueCommand_raw = CAN_ENCODE(TorqueCommand_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_COMMAND_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_COMMAND_OFFSET, float);
    out_data[0] |= packShiftRight(TorqueCommand_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(TorqueCommand_raw, 8, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 16-bit signal SpeedCommand into payload (at bit 16 to bit 32).
    const int SpeedCommand_val = in_msg->SpeedCommand_value;
    const uint32_t SpeedCommand_raw = CAN_ENCODE(SpeedCommand_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_SPEED_COMMAND_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_SPEED_COMMAND_OFFSET, int);
    out_data[2] |= packShiftRight(SpeedCommand_raw, 0, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(SpeedCommand_raw, 8, 0xff);   // Packs bits ######## of byte 3
    
    // Pack 1-bit signal DirectionCommand into payload (at bit 32 to bit 33).
    const InverterDirection DirectionCommand_val = in_msg->DirectionCommand_value;
    const uint32_t DirectionCommand_raw = CAN_ENCODE(DirectionCommand_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DIRECTION_COMMAND_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(DirectionCommand_raw, 0, 0x1);   // Packs bits _______# of byte 4
    
    // Pack 1-bit signal EnableInverter into payload (at bit 40 to bit 41).
    const bool EnableInverter_val = in_msg->EnableInverter_value;
    const uint32_t EnableInverter_raw = CAN_ENCODE(EnableInverter_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_ENABLE_INVERTER_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_ENABLE_INVERTER_OFFSET, uint32_t);
    out_data[5] |= packShiftRight(EnableInverter_raw, 0, 0x1);   // Packs bits _______# of byte 5
    
    // Pack 1-bit signal EnableDischarge into payload (at bit 41 to bit 42).
    const bool EnableDischarge_val = in_msg->EnableDischarge_value;
    const uint32_t EnableDischarge_raw = CAN_ENCODE(EnableDischarge_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_ENABLE_DISCHARGE_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_ENABLE_DISCHARGE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(EnableDischarge_raw, 1, 0x2);   // Packs bits ______#_ of byte 5
    
    // Pack 1-bit signal EnableSpeedMode into payload (at bit 42 to bit 43).
    const bool EnableSpeedMode_val = in_msg->EnableSpeedMode_value;
    const uint32_t EnableSpeedMode_raw = CAN_ENCODE(EnableSpeedMode_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_ENABLE_SPEED_MODE_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_ENABLE_SPEED_MODE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(EnableSpeedMode_raw, 2, 0x4);   // Packs bits _____#__ of byte 5
    
    // Pack 16-bit signal TorqueLimit into payload (at bit 48 to bit 64).
    const float TorqueLimit_val = in_msg->TorqueLimit_value;
    const uint32_t TorqueLimit_raw = CAN_ENCODE(TorqueLimit_val, CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_LIMIT_SCALE, CANSIG_DCM_RIGHT_INVERTER_COMMAND_TORQUE_LIMIT_OFFSET, float);
    out_data[6] |= packShiftRight(TorqueLimit_raw, 0, 0xff);   // Packs bits ######## of byte 6
    out_data[7] |= packShiftRight(TorqueLimit_raw, 8, 0xff);   // Packs bits ######## of byte 7
    
}

void App_CanUtils_DCM_AlertsContext_Pack(const DCM_AlertsContext_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 3-byte payload for message DCM_AlertsContext.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CCC|BBBBBBBB|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal TxOverflowCount into payload (at bit 0 to bit 8).
    const uint32_t TxOverflowCount_val = in_msg->TxOverflowCount_value;
    const uint32_t TxOverflowCount_raw = CAN_ENCODE(TxOverflowCount_val, CANSIG_DCM_ALERTS_CONTEXT_TX_OVERFLOW_COUNT_SCALE, CANSIG_DCM_ALERTS_CONTEXT_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(TxOverflowCount_raw, 0, 0xff);   // Packs bits ######## of byte 0
    
    // Pack 8-bit signal RxOverflowCount into payload (at bit 8 to bit 16).
    const uint32_t RxOverflowCount_val = in_msg->RxOverflowCount_value;
    const uint32_t RxOverflowCount_raw = CAN_ENCODE(RxOverflowCount_val, CANSIG_DCM_ALERTS_CONTEXT_RX_OVERFLOW_COUNT_SCALE, CANSIG_DCM_ALERTS_CONTEXT_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(RxOverflowCount_raw, 0, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 3-bit signal WatchdogTimeoutTaskName into payload (at bit 16 to bit 19).
    const RtosTaskName WatchdogTimeoutTaskName_val = in_msg->WatchdogTimeoutTaskName_value;
    const uint32_t WatchdogTimeoutTaskName_raw = CAN_ENCODE(WatchdogTimeoutTaskName_val, CANSIG_DCM_ALERTS_CONTEXT_WATCHDOG_TIMEOUT_TASK_NAME_SCALE, CANSIG_DCM_ALERTS_CONTEXT_WATCHDOG_TIMEOUT_TASK_NAME_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(WatchdogTimeoutTaskName_raw, 0, 0x7);   // Packs bits _____### of byte 2
    
}

void App_CanUtils_DCM_EllipseImuStatus_Pack(const DCM_EllipseImuStatus_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 6-byte payload for message DCM_EllipseImuStatus.
    // |xxxxxxxx|xxxxxxxx|BBBBBBBB|BBBBBBBB|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal GeneralStatusBitmask into payload (at bit 0 to bit 16).
    const uint32_t GeneralStatusBitmask_val = in_msg->GeneralStatusBitmask_value;
    const uint32_t GeneralStatusBitmask_raw = CAN_ENCODE(GeneralStatusBitmask_val, CANSIG_DCM_ELLIPSE_IMU_STATUS_GENERAL_STATUS_BITMASK_SCALE, CANSIG_DCM_ELLIPSE_IMU_STATUS_GENERAL_STATUS_BITMASK_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(GeneralStatusBitmask_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(GeneralStatusBitmask_raw, 8, 0xff);   // Packs bits ######## of byte 1
    
    // Pack 32-bit signal ComStatusBitmask into payload (at bit 16 to bit 48).
    const uint32_t ComStatusBitmask_val = in_msg->ComStatusBitmask_value;
    const uint32_t ComStatusBitmask_raw = CAN_ENCODE(ComStatusBitmask_val, CANSIG_DCM_ELLIPSE_IMU_STATUS_COM_STATUS_BITMASK_SCALE, CANSIG_DCM_ELLIPSE_IMU_STATUS_COM_STATUS_BITMASK_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(ComStatusBitmask_raw, 0, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(ComStatusBitmask_raw, 8, 0xff);   // Packs bits ######## of byte 3
    out_data[4] |= packShiftRight(ComStatusBitmask_raw, 16, 0xff);   // Packs bits ######## of byte 4
    out_data[5] |= packShiftRight(ComStatusBitmask_raw, 24, 0xff);   // Packs bits ######## of byte 5
    
}

void App_CanUtils_DCM_EllipseImuTime_Pack(const DCM_EllipseImuTime_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 4-byte payload for message DCM_EllipseImuTime.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 32-bit signal Timestamp into payload (at bit 0 to bit 32).
    const uint32_t Timestamp_val = in_msg->Timestamp_value;
    const uint32_t Timestamp_raw = CAN_ENCODE(Timestamp_val, CANSIG_DCM_ELLIPSE_IMU_TIME_TIMESTAMP_SCALE, CANSIG_DCM_ELLIPSE_IMU_TIME_TIMESTAMP_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(Timestamp_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(Timestamp_raw, 8, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(Timestamp_raw, 16, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(Timestamp_raw, 24, 0xff);   // Packs bits ######## of byte 3
    
}

void App_CanUtils_DCM_EllipseImuAcceleration_Pack(const DCM_EllipseImuAcceleration_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message DCM_EllipseImuAcceleration.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_CCCCCCC|CCCCCCBB|BBBBBBBB|BBBAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 13-bit signal Forward into payload (at bit 0 to bit 13).
    const float Forward_val = in_msg->Forward_value;
    const uint32_t Forward_raw = CAN_ENCODE(Forward_val, CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_FORWARD_SCALE, CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_FORWARD_OFFSET, float);
    out_data[0] |= packShiftRight(Forward_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(Forward_raw, 8, 0x1f);   // Packs bits ___##### of byte 1
    
    // Pack 13-bit signal Lateral into payload (at bit 13 to bit 26).
    const float Lateral_val = in_msg->Lateral_value;
    const uint32_t Lateral_raw = CAN_ENCODE(Lateral_val, CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_LATERAL_SCALE, CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_LATERAL_OFFSET, float);
    out_data[1] |= packShiftLeft(Lateral_raw, 5, 0xe0);   // Packs bits ###_____ of byte 1
    out_data[2] |= packShiftRight(Lateral_raw, 3, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(Lateral_raw, 11, 0x3);   // Packs bits ______## of byte 3
    
    // Pack 13-bit signal Vertical into payload (at bit 26 to bit 39).
    const float Vertical_val = in_msg->Vertical_value;
    const uint32_t Vertical_raw = CAN_ENCODE(Vertical_val, CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_VERTICAL_SCALE, CANSIG_DCM_ELLIPSE_IMU_ACCELERATION_VERTICAL_OFFSET, float);
    out_data[3] |= packShiftLeft(Vertical_raw, 2, 0xfc);   // Packs bits ######__ of byte 3
    out_data[4] |= packShiftRight(Vertical_raw, 6, 0x7f);   // Packs bits _####### of byte 4
    
}

void App_CanUtils_DCM_EllipseImuAngularVelocity_Pack(const DCM_EllipseImuAngularVelocity_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 4-byte payload for message DCM_EllipseImuAngularVelocity.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__CCCCCC|CCCCBBBB|BBBBBBAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 10-bit signal Roll into payload (at bit 0 to bit 10).
    const int Roll_val = in_msg->Roll_value;
    const uint32_t Roll_raw = CAN_ENCODE(Roll_val, CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_ROLL_SCALE, CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_ROLL_OFFSET, int);
    out_data[0] |= packShiftRight(Roll_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(Roll_raw, 8, 0x3);   // Packs bits ______## of byte 1
    
    // Pack 10-bit signal Pitch into payload (at bit 10 to bit 20).
    const int Pitch_val = in_msg->Pitch_value;
    const uint32_t Pitch_raw = CAN_ENCODE(Pitch_val, CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_PITCH_SCALE, CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_PITCH_OFFSET, int);
    out_data[1] |= packShiftLeft(Pitch_raw, 2, 0xfc);   // Packs bits ######__ of byte 1
    out_data[2] |= packShiftRight(Pitch_raw, 6, 0xf);   // Packs bits ____#### of byte 2
    
    // Pack 10-bit signal Yaw into payload (at bit 20 to bit 30).
    const int Yaw_val = in_msg->Yaw_value;
    const uint32_t Yaw_raw = CAN_ENCODE(Yaw_val, CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_YAW_SCALE, CANSIG_DCM_ELLIPSE_IMU_ANGULAR_VELOCITY_YAW_OFFSET, int);
    out_data[2] |= packShiftLeft(Yaw_raw, 4, 0xf0);   // Packs bits ####____ of byte 2
    out_data[3] |= packShiftRight(Yaw_raw, 4, 0x3f);   // Packs bits __###### of byte 3
    
}

void App_CanUtils_DCM_EllipseImuEulerAngles_Pack(const DCM_EllipseImuEulerAngles_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message DCM_EllipseImuEulerAngles.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|____CCCC|CCCCCCCC|BBBBBBBB|BBBBAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 12-bit signal Roll into payload (at bit 0 to bit 12).
    const float Roll_val = in_msg->Roll_value;
    const uint32_t Roll_raw = CAN_ENCODE(Roll_val, CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_ROLL_SCALE, CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_ROLL_OFFSET, float);
    out_data[0] |= packShiftRight(Roll_raw, 0, 0xff);   // Packs bits ######## of byte 0
    out_data[1] |= packShiftRight(Roll_raw, 8, 0xf);   // Packs bits ____#### of byte 1
    
    // Pack 12-bit signal Pitch into payload (at bit 12 to bit 24).
    const float Pitch_val = in_msg->Pitch_value;
    const uint32_t Pitch_raw = CAN_ENCODE(Pitch_val, CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_PITCH_SCALE, CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_PITCH_OFFSET, float);
    out_data[1] |= packShiftLeft(Pitch_raw, 4, 0xf0);   // Packs bits ####____ of byte 1
    out_data[2] |= packShiftRight(Pitch_raw, 4, 0xff);   // Packs bits ######## of byte 2
    
    // Pack 12-bit signal Yaw into payload (at bit 24 to bit 36).
    const float Yaw_val = in_msg->Yaw_value;
    const uint32_t Yaw_raw = CAN_ENCODE(Yaw_val, CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_YAW_SCALE, CANSIG_DCM_ELLIPSE_IMU_EULER_ANGLES_YAW_OFFSET, float);
    out_data[3] |= packShiftRight(Yaw_raw, 0, 0xff);   // Packs bits ######## of byte 3
    out_data[4] |= packShiftRight(Yaw_raw, 8, 0xf);   // Packs bits ____#### of byte 4
    
}

void App_CanUtils_DCM_Warnings_Pack(const DCM_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DCM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ into payload (at bit 0 to bit 1).
    const bool DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val = in_msg->DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value;
    const uint32_t DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw = CAN_ENCODE(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ into payload (at bit 1 to bit 2).
    const bool DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val = in_msg->DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value;
    const uint32_t DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw = CAN_ENCODE(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ into payload (at bit 2 to bit 3).
    const bool DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val = in_msg->DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value;
    const uint32_t DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw = CAN_ENCODE(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
    // Pack 1-bit signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX into payload (at bit 3 to bit 4).
    const bool DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val = in_msg->DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value;
    const uint32_t DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw = CAN_ENCODE(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw, 3, 0x8);   // Packs bits ____#___ of byte 0
    
    // Pack 1-bit signal DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX into payload (at bit 4 to bit 5).
    const bool DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val = in_msg->DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value;
    const uint32_t DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw = CAN_ENCODE(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw, 4, 0x10);   // Packs bits ___#____ of byte 0
    
    // Pack 1-bit signal DCM_WARNING_WATCHDOG_TIMEOUT into payload (at bit 5 to bit 6).
    const bool DCM_WARNING_WATCHDOG_TIMEOUT_val = in_msg->DCM_WARNING_WATCHDOG_TIMEOUT_value;
    const uint32_t DCM_WARNING_WATCHDOG_TIMEOUT_raw = CAN_ENCODE(DCM_WARNING_WATCHDOG_TIMEOUT_val, CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_WARNING_WATCHDOG_TIMEOUT_raw, 5, 0x20);   // Packs bits __#_____ of byte 0
    
    // Pack 1-bit signal DCM_WARNING_TX_OVERFLOW into payload (at bit 6 to bit 7).
    const bool DCM_WARNING_TX_OVERFLOW_val = in_msg->DCM_WARNING_TX_OVERFLOW_value;
    const uint32_t DCM_WARNING_TX_OVERFLOW_raw = CAN_ENCODE(DCM_WARNING_TX_OVERFLOW_val, CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_WARNING_TX_OVERFLOW_raw, 6, 0x40);   // Packs bits _#______ of byte 0
    
    // Pack 1-bit signal DCM_WARNING_RX_OVERFLOW into payload (at bit 7 to bit 8).
    const bool DCM_WARNING_RX_OVERFLOW_val = in_msg->DCM_WARNING_RX_OVERFLOW_value;
    const uint32_t DCM_WARNING_RX_OVERFLOW_raw = CAN_ENCODE(DCM_WARNING_RX_OVERFLOW_val, CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_SCALE, CANSIG_DCM_WARNINGS_DCM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_WARNING_RX_OVERFLOW_raw, 7, 0x80);   // Packs bits #_______ of byte 0
    
}

void App_CanUtils_DCM_Faults_Pack(const DCM_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message DCM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DCM_FAULT_STATE_FAULT into payload (at bit 0 to bit 1).
    const bool DCM_FAULT_STATE_FAULT_val = in_msg->DCM_FAULT_STATE_FAULT_value;
    const uint32_t DCM_FAULT_STATE_FAULT_raw = CAN_ENCODE(DCM_FAULT_STATE_FAULT_val, CANSIG_DCM_FAULTS_DCM_FAULT_STATE_FAULT_SCALE, CANSIG_DCM_FAULTS_DCM_FAULT_STATE_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DCM_FAULT_STATE_FAULT_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal DCM_FAULT_MISSING_HEARTBEAT into payload (at bit 1 to bit 2).
    const bool DCM_FAULT_MISSING_HEARTBEAT_val = in_msg->DCM_FAULT_MISSING_HEARTBEAT_value;
    const uint32_t DCM_FAULT_MISSING_HEARTBEAT_raw = CAN_ENCODE(DCM_FAULT_MISSING_HEARTBEAT_val, CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_DCM_FAULTS_DCM_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(DCM_FAULT_MISSING_HEARTBEAT_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
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

void App_CanUtils_BMS_AvailablePower_Unpack(const uint8_t* const in_data, BMS_AvailablePower_Signals* const out_msg)
{
    // Unpack 3-byte payload for message BMS_AvailablePower.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____AAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 20-bit signal AvailablePower from payload (at bit 0 to bit 20).
    uint32_t AvailablePower_raw = 0;
    AvailablePower_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    AvailablePower_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    AvailablePower_raw |= unpackShiftLeft(in_data[2], 16, 0xf);   // Unpacks bits ____#### of msg byte 2
    float AvailablePower_val = CAN_DECODE(AvailablePower_raw, CANSIG_BMS_AVAILABLE_POWER_AVAILABLE_POWER_SCALE, CANSIG_BMS_AVAILABLE_POWER_AVAILABLE_POWER_OFFSET, float);
    out_msg->AvailablePower_value = AvailablePower_val;
    
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
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|___MLKJI|HGFEDCBA|
    
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

void App_CanUtils_DIM_Vitals_Unpack(const uint8_t* const in_data, DIM_Vitals_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DIM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal Heartbeat from payload (at bit 0 to bit 1).
    uint32_t Heartbeat_raw = 0;
    Heartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool Heartbeat_val = CAN_DECODE(Heartbeat_raw, CANSIG_DIM_VITALS_HEARTBEAT_SCALE, CANSIG_DIM_VITALS_HEARTBEAT_OFFSET, uint32_t);
    out_msg->Heartbeat_value = Heartbeat_val;
    
    // Unpack 1-bit signal State from payload (at bit 1 to bit 2).
    uint32_t State_raw = 0;
    State_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    DimState State_val = CAN_DECODE(State_raw, CANSIG_DIM_VITALS_STATE_SCALE, CANSIG_DIM_VITALS_STATE_OFFSET, uint32_t);
    out_msg->State_value = State_val;
    
}

void App_CanUtils_DIM_Switches_Unpack(const uint8_t* const in_data, DIM_Switches_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DIM_Switches.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal StartSwitch from payload (at bit 0 to bit 1).
    uint32_t StartSwitch_raw = 0;
    StartSwitch_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    SwitchState StartSwitch_val = CAN_DECODE(StartSwitch_raw, CANSIG_DIM_SWITCHES_START_SWITCH_SCALE, CANSIG_DIM_SWITCHES_START_SWITCH_OFFSET, uint32_t);
    out_msg->StartSwitch_value = StartSwitch_val;
    
    // Unpack 1-bit signal AuxSwitch from payload (at bit 1 to bit 2).
    uint32_t AuxSwitch_raw = 0;
    AuxSwitch_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    SwitchState AuxSwitch_val = CAN_DECODE(AuxSwitch_raw, CANSIG_DIM_SWITCHES_AUX_SWITCH_SCALE, CANSIG_DIM_SWITCHES_AUX_SWITCH_OFFSET, uint32_t);
    out_msg->AuxSwitch_value = AuxSwitch_val;
    
}

void App_CanUtils_DIM_Warnings_Unpack(const uint8_t* const in_data, DIM_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DIM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ from payload (at bit 0 to bit 1).
    uint32_t DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw = 0;
    DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val = CAN_DECODE(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_HZ_OFFSET, uint32_t);
    out_msg->DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_value = DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1HZ_val;
    
    // Unpack 1-bit signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ from payload (at bit 1 to bit 2).
    uint32_t DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw = 0;
    DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val = CAN_DECODE(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100_HZ_OFFSET, uint32_t);
    out_msg->DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_value = DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_100HZ_val;
    
    // Unpack 1-bit signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ from payload (at bit 2 to bit 3).
    uint32_t DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw = 0;
    DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val = CAN_DECODE(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1_KHZ_OFFSET, uint32_t);
    out_msg->DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_value = DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_1KHZ_val;
    
    // Unpack 1-bit signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX from payload (at bit 3 to bit 4).
    uint32_t DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw = 0;
    DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val = CAN_DECODE(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_OFFSET, uint32_t);
    out_msg->DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_value = DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANRX_val;
    
    // Unpack 1-bit signal DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX from payload (at bit 4 to bit 5).
    uint32_t DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw = 0;
    DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val = CAN_DECODE(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_OFFSET, uint32_t);
    out_msg->DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_value = DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CANTX_val;
    
    // Unpack 1-bit signal DIM_WARNING_WATCHDOG_TIMEOUT from payload (at bit 5 to bit 6).
    uint32_t DIM_WARNING_WATCHDOG_TIMEOUT_raw = 0;
    DIM_WARNING_WATCHDOG_TIMEOUT_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool DIM_WARNING_WATCHDOG_TIMEOUT_val = CAN_DECODE(DIM_WARNING_WATCHDOG_TIMEOUT_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_msg->DIM_WARNING_WATCHDOG_TIMEOUT_value = DIM_WARNING_WATCHDOG_TIMEOUT_val;
    
    // Unpack 1-bit signal DIM_WARNING_TX_OVERFLOW from payload (at bit 6 to bit 7).
    uint32_t DIM_WARNING_TX_OVERFLOW_raw = 0;
    DIM_WARNING_TX_OVERFLOW_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool DIM_WARNING_TX_OVERFLOW_val = CAN_DECODE(DIM_WARNING_TX_OVERFLOW_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_msg->DIM_WARNING_TX_OVERFLOW_value = DIM_WARNING_TX_OVERFLOW_val;
    
    // Unpack 1-bit signal DIM_WARNING_RX_OVERFLOW from payload (at bit 7 to bit 8).
    uint32_t DIM_WARNING_RX_OVERFLOW_raw = 0;
    DIM_WARNING_RX_OVERFLOW_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool DIM_WARNING_RX_OVERFLOW_val = CAN_DECODE(DIM_WARNING_RX_OVERFLOW_raw, CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_SCALE, CANSIG_DIM_WARNINGS_DIM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_msg->DIM_WARNING_RX_OVERFLOW_value = DIM_WARNING_RX_OVERFLOW_val;
    
}

void App_CanUtils_DIM_Faults_Unpack(const uint8_t* const in_data, DIM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message DIM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal DIM_FAULT_MISSING_HEARTBEAT from payload (at bit 0 to bit 1).
    uint32_t DIM_FAULT_MISSING_HEARTBEAT_raw = 0;
    DIM_FAULT_MISSING_HEARTBEAT_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool DIM_FAULT_MISSING_HEARTBEAT_val = CAN_DECODE(DIM_FAULT_MISSING_HEARTBEAT_raw, CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_SCALE, CANSIG_DIM_FAULTS_DIM_FAULT_MISSING_HEARTBEAT_OFFSET, uint32_t);
    out_msg->DIM_FAULT_MISSING_HEARTBEAT_value = DIM_FAULT_MISSING_HEARTBEAT_val;
    
}

void App_CanUtils_FSM_Vitals_Unpack(const uint8_t* const in_data, FSM_Vitals_Signals* const out_msg)
{
    // Unpack 1-byte payload for message FSM_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal Heartbeat from payload (at bit 0 to bit 1).
    uint32_t Heartbeat_raw = 0;
    Heartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool Heartbeat_val = CAN_DECODE(Heartbeat_raw, CANSIG_FSM_VITALS_HEARTBEAT_SCALE, CANSIG_FSM_VITALS_HEARTBEAT_OFFSET, uint32_t);
    out_msg->Heartbeat_value = Heartbeat_val;
    
    // Unpack 1-bit signal State from payload (at bit 1 to bit 2).
    uint32_t State_raw = 0;
    State_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    State_Machine State_val = CAN_DECODE(State_raw, CANSIG_FSM_VITALS_STATE_SCALE, CANSIG_FSM_VITALS_STATE_OFFSET, uint32_t);
    out_msg->State_value = State_val;
    
}

void App_CanUtils_FSM_Brake_Unpack(const uint8_t* const in_data, FSM_Brake_Signals* const out_msg)
{
    // Unpack 5-byte payload for message FSM_Brake.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|___FEDCC|CCCCCCCC|BBBBBBBB|BBBBAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 12-bit signal FrontBrakePressure from payload (at bit 0 to bit 12).
    uint32_t FrontBrakePressure_raw = 0;
    FrontBrakePressure_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    FrontBrakePressure_raw |= unpackShiftLeft(in_data[1], 8, 0xf);   // Unpacks bits ____#### of msg byte 1
    uint32_t FrontBrakePressure_val = CAN_DECODE(FrontBrakePressure_raw, CANSIG_FSM_BRAKE_FRONT_BRAKE_PRESSURE_SCALE, CANSIG_FSM_BRAKE_FRONT_BRAKE_PRESSURE_OFFSET, uint32_t);
    out_msg->FrontBrakePressure_value = FrontBrakePressure_val;
    
    // Unpack 12-bit signal RearBrakePressure from payload (at bit 12 to bit 24).
    uint32_t RearBrakePressure_raw = 0;
    RearBrakePressure_raw |= unpackShiftRight(in_data[1], 4, 0xf0);   // Unpacks bits ####____ of msg byte 1
    RearBrakePressure_raw |= unpackShiftLeft(in_data[2], 4, 0xff);   // Unpacks bits ######## of msg byte 2
    uint32_t RearBrakePressure_val = CAN_DECODE(RearBrakePressure_raw, CANSIG_FSM_BRAKE_REAR_BRAKE_PRESSURE_SCALE, CANSIG_FSM_BRAKE_REAR_BRAKE_PRESSURE_OFFSET, uint32_t);
    out_msg->RearBrakePressure_value = RearBrakePressure_val;
    
    // Unpack 10-bit signal BrakePedalPercentage from payload (at bit 24 to bit 34).
    uint32_t BrakePedalPercentage_raw = 0;
    BrakePedalPercentage_raw |= unpackShiftRight(in_data[3], 0, 0xff);   // Unpacks bits ######## of msg byte 3
    BrakePedalPercentage_raw |= unpackShiftLeft(in_data[4], 8, 0x3);   // Unpacks bits ______## of msg byte 4
    float BrakePedalPercentage_val = CAN_DECODE(BrakePedalPercentage_raw, CANSIG_FSM_BRAKE_BRAKE_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_BRAKE_BRAKE_PEDAL_PERCENTAGE_OFFSET, float);
    out_msg->BrakePedalPercentage_value = BrakePedalPercentage_val;
    
    // Unpack 1-bit signal IsActuated from payload (at bit 34 to bit 35).
    uint32_t IsActuated_raw = 0;
    IsActuated_raw |= unpackShiftRight(in_data[4], 2, 0x4);   // Unpacks bits _____#__ of msg byte 4
    bool IsActuated_val = CAN_DECODE(IsActuated_raw, CANSIG_FSM_BRAKE_IS_ACTUATED_SCALE, CANSIG_FSM_BRAKE_IS_ACTUATED_OFFSET, uint32_t);
    out_msg->IsActuated_value = IsActuated_val;
    
    // Unpack 1-bit signal PressureSensorOpenShortCircuit from payload (at bit 35 to bit 36).
    uint32_t PressureSensorOpenShortCircuit_raw = 0;
    PressureSensorOpenShortCircuit_raw |= unpackShiftRight(in_data[4], 3, 0x8);   // Unpacks bits ____#___ of msg byte 4
    bool PressureSensorOpenShortCircuit_val = CAN_DECODE(PressureSensorOpenShortCircuit_raw, CANSIG_FSM_BRAKE_PRESSURE_SENSOR_OPEN_SHORT_CIRCUIT_SCALE, CANSIG_FSM_BRAKE_PRESSURE_SENSOR_OPEN_SHORT_CIRCUIT_OFFSET, uint32_t);
    out_msg->PressureSensorOpenShortCircuit_value = PressureSensorOpenShortCircuit_val;
    
    // Unpack 1-bit signal PedalOpenShortCircuit from payload (at bit 36 to bit 37).
    uint32_t PedalOpenShortCircuit_raw = 0;
    PedalOpenShortCircuit_raw |= unpackShiftRight(in_data[4], 4, 0x10);   // Unpacks bits ___#____ of msg byte 4
    bool PedalOpenShortCircuit_val = CAN_DECODE(PedalOpenShortCircuit_raw, CANSIG_FSM_BRAKE_PEDAL_OPEN_SHORT_CIRCUIT_SCALE, CANSIG_FSM_BRAKE_PEDAL_OPEN_SHORT_CIRCUIT_OFFSET, uint32_t);
    out_msg->PedalOpenShortCircuit_value = PedalOpenShortCircuit_val;
    
}

void App_CanUtils_FSM_Warnings_Unpack(const uint8_t* const in_data, FSM_Warnings_Signals* const out_msg)
{
    // Unpack 2-byte payload for message FSM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|PONMLKJI|HGFEDCBA|
    
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
    
    // Unpack 1-bit signal FSM_WARNING_BRAKE_ACC_DISAGREEMENT from payload (at bit 8 to bit 9).
    uint32_t FSM_WARNING_BRAKE_ACC_DISAGREEMENT_raw = 0;
    FSM_WARNING_BRAKE_ACC_DISAGREEMENT_raw |= unpackShiftRight(in_data[1], 0, 0x1);   // Unpacks bits _______# of msg byte 1
    bool FSM_WARNING_BRAKE_ACC_DISAGREEMENT_val = CAN_DECODE(FSM_WARNING_BRAKE_ACC_DISAGREEMENT_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_BRAKE_ACC_DISAGREEMENT_OFFSET, uint32_t);
    out_msg->FSM_WARNING_BRAKE_ACC_DISAGREEMENT_value = FSM_WARNING_BRAKE_ACC_DISAGREEMENT_val;
    
    // Unpack 1-bit signal FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE from payload (at bit 9 to bit 10).
    uint32_t FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 1, 0x2);   // Unpacks bits ______#_ of msg byte 1
    bool FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_value = FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_val;
    
    // Unpack 1-bit signal FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE from payload (at bit 10 to bit 11).
    uint32_t FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 2, 0x4);   // Unpacks bits _____#__ of msg byte 1
    bool FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_value = FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_val;
    
    // Unpack 1-bit signal FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC from payload (at bit 11 to bit 12).
    uint32_t FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_raw = 0;
    FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_raw |= unpackShiftRight(in_data[1], 3, 0x8);   // Unpacks bits ____#___ of msg byte 1
    bool FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_val = CAN_DECODE(FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_OFFSET, uint32_t);
    out_msg->FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_value = FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC_val;
    
    // Unpack 1-bit signal FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE from payload (at bit 12 to bit 13).
    uint32_t FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 4, 0x10);   // Unpacks bits ___#____ of msg byte 1
    bool FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_value = FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_val;
    
    // Unpack 1-bit signal FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE from payload (at bit 13 to bit 14).
    uint32_t FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 5, 0x20);   // Unpacks bits __#_____ of msg byte 1
    bool FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_value = FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_val;
    
    // Unpack 1-bit signal FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE from payload (at bit 14 to bit 15).
    uint32_t FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 6, 0x40);   // Unpacks bits _#______ of msg byte 1
    bool FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_value = FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_val;
    
    // Unpack 1-bit signal FSM_WARNING_FLOW_RATE_OUT_OF_RANGE from payload (at bit 15 to bit 16).
    uint32_t FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_raw = 0;
    FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_raw |= unpackShiftRight(in_data[1], 7, 0x80);   // Unpacks bits #_______ of msg byte 1
    bool FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_val = CAN_DECODE(FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_value = FSM_WARNING_FLOW_RATE_OUT_OF_RANGE_val;
    
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
    
    // Unpack 1-bit signal FSM_FAULT_APPS_HAS_DISAGREEMENT from payload (at bit 4 to bit 5).
    uint32_t FSM_FAULT_APPS_HAS_DISAGREEMENT_raw = 0;
    FSM_FAULT_APPS_HAS_DISAGREEMENT_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool FSM_FAULT_APPS_HAS_DISAGREEMENT_val = CAN_DECODE(FSM_FAULT_APPS_HAS_DISAGREEMENT_raw, CANSIG_FSM_FAULTS_FSM_FAULT_APPS_HAS_DISAGREEMENT_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_APPS_HAS_DISAGREEMENT_OFFSET, uint32_t);
    out_msg->FSM_FAULT_APPS_HAS_DISAGREEMENT_value = FSM_FAULT_APPS_HAS_DISAGREEMENT_val;
    
    // Unpack 1-bit signal FSM_FAULT_FLOW_METER_HAS_UNDERFLOW from payload (at bit 5 to bit 6).
    uint32_t FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_raw = 0;
    FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_val = CAN_DECODE(FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_raw, CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_OFFSET, uint32_t);
    out_msg->FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_value = FSM_FAULT_FLOW_METER_HAS_UNDERFLOW_val;
    
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

void App_CanUtils_INVL_InternalStates_Unpack(const uint8_t* const in_data, INVL_InternalStates_Signals* const out_msg)
{
    // Unpack 8-byte payload for message INVL_InternalStates.
    // |_WVUTSRQ|PO_____N|MMMM___L|KKK____J|__IHGFED|____CCCC|BBBBBBBB|____AAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 4-bit signal VsmState from payload (at bit 0 to bit 4).
    uint32_t VsmState_raw = 0;
    VsmState_raw |= unpackShiftRight(in_data[0], 0, 0xf);   // Unpacks bits ____#### of msg byte 0
    InverterVsmState VsmState_val = CAN_DECODE(VsmState_raw, CANSIG_INVL_INTERNAL_STATES_VSM_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_VSM_STATE_OFFSET, uint32_t);
    out_msg->VsmState_value = VsmState_val;
    
    // Unpack 8-bit signal PwmFrequency from payload (at bit 8 to bit 16).
    uint32_t PwmFrequency_raw = 0;
    PwmFrequency_raw |= unpackShiftRight(in_data[1], 0, 0xff);   // Unpacks bits ######## of msg byte 1
    uint32_t PwmFrequency_val = CAN_DECODE(PwmFrequency_raw, CANSIG_INVL_INTERNAL_STATES_PWM_FREQUENCY_SCALE, CANSIG_INVL_INTERNAL_STATES_PWM_FREQUENCY_OFFSET, uint32_t);
    out_msg->PwmFrequency_value = PwmFrequency_val;
    
    // Unpack 4-bit signal InverterState from payload (at bit 16 to bit 20).
    uint32_t InverterState_raw = 0;
    InverterState_raw |= unpackShiftRight(in_data[2], 0, 0xf);   // Unpacks bits ____#### of msg byte 2
    InverterState InverterState_val = CAN_DECODE(InverterState_raw, CANSIG_INVL_INTERNAL_STATES_INVERTER_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVERTER_STATE_OFFSET, uint32_t);
    out_msg->InverterState_value = InverterState_val;
    
    // Unpack 1-bit signal Relay1State from payload (at bit 24 to bit 25).
    uint32_t Relay1State_raw = 0;
    Relay1State_raw |= unpackShiftRight(in_data[3], 0, 0x1);   // Unpacks bits _______# of msg byte 3
    bool Relay1State_val = CAN_DECODE(Relay1State_raw, CANSIG_INVL_INTERNAL_STATES_RELAY1_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_RELAY1_STATE_OFFSET, uint32_t);
    out_msg->Relay1State_value = Relay1State_val;
    
    // Unpack 1-bit signal Relay2State from payload (at bit 25 to bit 26).
    uint32_t Relay2State_raw = 0;
    Relay2State_raw |= unpackShiftRight(in_data[3], 1, 0x2);   // Unpacks bits ______#_ of msg byte 3
    bool Relay2State_val = CAN_DECODE(Relay2State_raw, CANSIG_INVL_INTERNAL_STATES_RELAY2_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_RELAY2_STATE_OFFSET, uint32_t);
    out_msg->Relay2State_value = Relay2State_val;
    
    // Unpack 1-bit signal Relay3State from payload (at bit 26 to bit 27).
    uint32_t Relay3State_raw = 0;
    Relay3State_raw |= unpackShiftRight(in_data[3], 2, 0x4);   // Unpacks bits _____#__ of msg byte 3
    bool Relay3State_val = CAN_DECODE(Relay3State_raw, CANSIG_INVL_INTERNAL_STATES_RELAY3_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_RELAY3_STATE_OFFSET, uint32_t);
    out_msg->Relay3State_value = Relay3State_val;
    
    // Unpack 1-bit signal Relay4State from payload (at bit 27 to bit 28).
    uint32_t Relay4State_raw = 0;
    Relay4State_raw |= unpackShiftRight(in_data[3], 3, 0x8);   // Unpacks bits ____#___ of msg byte 3
    bool Relay4State_val = CAN_DECODE(Relay4State_raw, CANSIG_INVL_INTERNAL_STATES_RELAY4_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_RELAY4_STATE_OFFSET, uint32_t);
    out_msg->Relay4State_value = Relay4State_val;
    
    // Unpack 1-bit signal Relay5State from payload (at bit 28 to bit 29).
    uint32_t Relay5State_raw = 0;
    Relay5State_raw |= unpackShiftRight(in_data[3], 4, 0x10);   // Unpacks bits ___#____ of msg byte 3
    bool Relay5State_val = CAN_DECODE(Relay5State_raw, CANSIG_INVL_INTERNAL_STATES_RELAY5_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_RELAY5_STATE_OFFSET, uint32_t);
    out_msg->Relay5State_value = Relay5State_val;
    
    // Unpack 1-bit signal Relay6State from payload (at bit 29 to bit 30).
    uint32_t Relay6State_raw = 0;
    Relay6State_raw |= unpackShiftRight(in_data[3], 5, 0x20);   // Unpacks bits __#_____ of msg byte 3
    bool Relay6State_val = CAN_DECODE(Relay6State_raw, CANSIG_INVL_INTERNAL_STATES_RELAY6_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_RELAY6_STATE_OFFSET, uint32_t);
    out_msg->Relay6State_value = Relay6State_val;
    
    // Unpack 1-bit signal InverterRunMode from payload (at bit 32 to bit 33).
    uint32_t InverterRunMode_raw = 0;
    InverterRunMode_raw |= unpackShiftRight(in_data[4], 0, 0x1);   // Unpacks bits _______# of msg byte 4
    bool InverterRunMode_val = CAN_DECODE(InverterRunMode_raw, CANSIG_INVL_INTERNAL_STATES_INVERTER_RUN_MODE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVERTER_RUN_MODE_OFFSET, uint32_t);
    out_msg->InverterRunMode_value = InverterRunMode_val;
    
    // Unpack 3-bit signal InverterActiveDischargeState from payload (at bit 37 to bit 40).
    uint32_t InverterActiveDischargeState_raw = 0;
    InverterActiveDischargeState_raw |= unpackShiftRight(in_data[4], 5, 0xe0);   // Unpacks bits ###_____ of msg byte 4
    InverterActiveDischargeState InverterActiveDischargeState_val = CAN_DECODE(InverterActiveDischargeState_raw, CANSIG_INVL_INTERNAL_STATES_INVERTER_ACTIVE_DISCHARGE_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVERTER_ACTIVE_DISCHARGE_STATE_OFFSET, uint32_t);
    out_msg->InverterActiveDischargeState_value = InverterActiveDischargeState_val;
    
    // Unpack 1-bit signal InverterCommandMode from payload (at bit 40 to bit 41).
    uint32_t InverterCommandMode_raw = 0;
    InverterCommandMode_raw |= unpackShiftRight(in_data[5], 0, 0x1);   // Unpacks bits _______# of msg byte 5
    InverterCommandMode InverterCommandMode_val = CAN_DECODE(InverterCommandMode_raw, CANSIG_INVL_INTERNAL_STATES_INVERTER_COMMAND_MODE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVERTER_COMMAND_MODE_OFFSET, uint32_t);
    out_msg->InverterCommandMode_value = InverterCommandMode_val;
    
    // Unpack 4-bit signal RollingCounter from payload (at bit 44 to bit 48).
    uint32_t RollingCounter_raw = 0;
    RollingCounter_raw |= unpackShiftRight(in_data[5], 4, 0xf0);   // Unpacks bits ####____ of msg byte 5
    uint32_t RollingCounter_val = CAN_DECODE(RollingCounter_raw, CANSIG_INVL_INTERNAL_STATES_ROLLING_COUNTER_SCALE, CANSIG_INVL_INTERNAL_STATES_ROLLING_COUNTER_OFFSET, uint32_t);
    out_msg->RollingCounter_value = RollingCounter_val;
    
    // Unpack 1-bit signal InverterEnableState from payload (at bit 48 to bit 49).
    uint32_t InverterEnableState_raw = 0;
    InverterEnableState_raw |= unpackShiftRight(in_data[6], 0, 0x1);   // Unpacks bits _______# of msg byte 6
    bool InverterEnableState_val = CAN_DECODE(InverterEnableState_raw, CANSIG_INVL_INTERNAL_STATES_INVERTER_ENABLE_STATE_SCALE, CANSIG_INVL_INTERNAL_STATES_INVERTER_ENABLE_STATE_OFFSET, uint32_t);
    out_msg->InverterEnableState_value = InverterEnableState_val;
    
    // Unpack 1-bit signal StartModeActive from payload (at bit 54 to bit 55).
    uint32_t StartModeActive_raw = 0;
    StartModeActive_raw |= unpackShiftRight(in_data[6], 6, 0x40);   // Unpacks bits _#______ of msg byte 6
    bool StartModeActive_val = CAN_DECODE(StartModeActive_raw, CANSIG_INVL_INTERNAL_STATES_START_MODE_ACTIVE_SCALE, CANSIG_INVL_INTERNAL_STATES_START_MODE_ACTIVE_OFFSET, uint32_t);
    out_msg->StartModeActive_value = StartModeActive_val;
    
    // Unpack 1-bit signal InverterLockoutEnabled from payload (at bit 55 to bit 56).
    uint32_t InverterLockoutEnabled_raw = 0;
    InverterLockoutEnabled_raw |= unpackShiftRight(in_data[6], 7, 0x80);   // Unpacks bits #_______ of msg byte 6
    InverterLockout InverterLockoutEnabled_val = CAN_DECODE(InverterLockoutEnabled_raw, CANSIG_INVL_INTERNAL_STATES_INVERTER_LOCKOUT_ENABLED_SCALE, CANSIG_INVL_INTERNAL_STATES_INVERTER_LOCKOUT_ENABLED_OFFSET, uint32_t);
    out_msg->InverterLockoutEnabled_value = InverterLockoutEnabled_val;
    
    // Unpack 1-bit signal DirectionCommand from payload (at bit 56 to bit 57).
    uint32_t DirectionCommand_raw = 0;
    DirectionCommand_raw |= unpackShiftRight(in_data[7], 0, 0x1);   // Unpacks bits _______# of msg byte 7
    InverterDirection DirectionCommand_val = CAN_DECODE(DirectionCommand_raw, CANSIG_INVL_INTERNAL_STATES_DIRECTION_COMMAND_SCALE, CANSIG_INVL_INTERNAL_STATES_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_msg->DirectionCommand_value = DirectionCommand_val;
    
    // Unpack 1-bit signal BmsActive from payload (at bit 57 to bit 58).
    uint32_t BmsActive_raw = 0;
    BmsActive_raw |= unpackShiftRight(in_data[7], 1, 0x2);   // Unpacks bits ______#_ of msg byte 7
    bool BmsActive_val = CAN_DECODE(BmsActive_raw, CANSIG_INVL_INTERNAL_STATES_BMS_ACTIVE_SCALE, CANSIG_INVL_INTERNAL_STATES_BMS_ACTIVE_OFFSET, uint32_t);
    out_msg->BmsActive_value = BmsActive_val;
    
    // Unpack 1-bit signal BmsTorqueLimiting from payload (at bit 58 to bit 59).
    uint32_t BmsTorqueLimiting_raw = 0;
    BmsTorqueLimiting_raw |= unpackShiftRight(in_data[7], 2, 0x4);   // Unpacks bits _____#__ of msg byte 7
    bool BmsTorqueLimiting_val = CAN_DECODE(BmsTorqueLimiting_raw, CANSIG_INVL_INTERNAL_STATES_BMS_TORQUE_LIMITING_SCALE, CANSIG_INVL_INTERNAL_STATES_BMS_TORQUE_LIMITING_OFFSET, uint32_t);
    out_msg->BmsTorqueLimiting_value = BmsTorqueLimiting_val;
    
    // Unpack 1-bit signal MaxSpeedLimiting from payload (at bit 59 to bit 60).
    uint32_t MaxSpeedLimiting_raw = 0;
    MaxSpeedLimiting_raw |= unpackShiftRight(in_data[7], 3, 0x8);   // Unpacks bits ____#___ of msg byte 7
    bool MaxSpeedLimiting_val = CAN_DECODE(MaxSpeedLimiting_raw, CANSIG_INVL_INTERNAL_STATES_MAX_SPEED_LIMITING_SCALE, CANSIG_INVL_INTERNAL_STATES_MAX_SPEED_LIMITING_OFFSET, uint32_t);
    out_msg->MaxSpeedLimiting_value = MaxSpeedLimiting_val;
    
    // Unpack 1-bit signal HotSpotLimiting from payload (at bit 60 to bit 61).
    uint32_t HotSpotLimiting_raw = 0;
    HotSpotLimiting_raw |= unpackShiftRight(in_data[7], 4, 0x10);   // Unpacks bits ___#____ of msg byte 7
    bool HotSpotLimiting_val = CAN_DECODE(HotSpotLimiting_raw, CANSIG_INVL_INTERNAL_STATES_HOT_SPOT_LIMITING_SCALE, CANSIG_INVL_INTERNAL_STATES_HOT_SPOT_LIMITING_OFFSET, uint32_t);
    out_msg->HotSpotLimiting_value = HotSpotLimiting_val;
    
    // Unpack 1-bit signal LowSpeedLimiting from payload (at bit 61 to bit 62).
    uint32_t LowSpeedLimiting_raw = 0;
    LowSpeedLimiting_raw |= unpackShiftRight(in_data[7], 5, 0x20);   // Unpacks bits __#_____ of msg byte 7
    bool LowSpeedLimiting_val = CAN_DECODE(LowSpeedLimiting_raw, CANSIG_INVL_INTERNAL_STATES_LOW_SPEED_LIMITING_SCALE, CANSIG_INVL_INTERNAL_STATES_LOW_SPEED_LIMITING_OFFSET, uint32_t);
    out_msg->LowSpeedLimiting_value = LowSpeedLimiting_val;
    
    // Unpack 1-bit signal CoolantTemperatureLimiting from payload (at bit 62 to bit 63).
    uint32_t CoolantTemperatureLimiting_raw = 0;
    CoolantTemperatureLimiting_raw |= unpackShiftRight(in_data[7], 6, 0x40);   // Unpacks bits _#______ of msg byte 7
    bool CoolantTemperatureLimiting_val = CAN_DECODE(CoolantTemperatureLimiting_raw, CANSIG_INVL_INTERNAL_STATES_COOLANT_TEMPERATURE_LIMITING_SCALE, CANSIG_INVL_INTERNAL_STATES_COOLANT_TEMPERATURE_LIMITING_OFFSET, uint32_t);
    out_msg->CoolantTemperatureLimiting_value = CoolantTemperatureLimiting_val;
    
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

void App_CanUtils_INVR_InternalStates_Unpack(const uint8_t* const in_data, INVR_InternalStates_Signals* const out_msg)
{
    // Unpack 8-byte payload for message INVR_InternalStates.
    // |_WVUTSRQ|PO_____N|MMMM___L|KKK____J|__IHGFED|____CCCC|BBBBBBBB|____AAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 4-bit signal VsmState from payload (at bit 0 to bit 4).
    uint32_t VsmState_raw = 0;
    VsmState_raw |= unpackShiftRight(in_data[0], 0, 0xf);   // Unpacks bits ____#### of msg byte 0
    InverterVsmState VsmState_val = CAN_DECODE(VsmState_raw, CANSIG_INVR_INTERNAL_STATES_VSM_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_VSM_STATE_OFFSET, uint32_t);
    out_msg->VsmState_value = VsmState_val;
    
    // Unpack 8-bit signal PwmFrequency from payload (at bit 8 to bit 16).
    uint32_t PwmFrequency_raw = 0;
    PwmFrequency_raw |= unpackShiftRight(in_data[1], 0, 0xff);   // Unpacks bits ######## of msg byte 1
    uint32_t PwmFrequency_val = CAN_DECODE(PwmFrequency_raw, CANSIG_INVR_INTERNAL_STATES_PWM_FREQUENCY_SCALE, CANSIG_INVR_INTERNAL_STATES_PWM_FREQUENCY_OFFSET, uint32_t);
    out_msg->PwmFrequency_value = PwmFrequency_val;
    
    // Unpack 4-bit signal InverterState from payload (at bit 16 to bit 20).
    uint32_t InverterState_raw = 0;
    InverterState_raw |= unpackShiftRight(in_data[2], 0, 0xf);   // Unpacks bits ____#### of msg byte 2
    InverterState InverterState_val = CAN_DECODE(InverterState_raw, CANSIG_INVR_INTERNAL_STATES_INVERTER_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVERTER_STATE_OFFSET, uint32_t);
    out_msg->InverterState_value = InverterState_val;
    
    // Unpack 1-bit signal Relay1State from payload (at bit 24 to bit 25).
    uint32_t Relay1State_raw = 0;
    Relay1State_raw |= unpackShiftRight(in_data[3], 0, 0x1);   // Unpacks bits _______# of msg byte 3
    bool Relay1State_val = CAN_DECODE(Relay1State_raw, CANSIG_INVR_INTERNAL_STATES_RELAY1_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_RELAY1_STATE_OFFSET, uint32_t);
    out_msg->Relay1State_value = Relay1State_val;
    
    // Unpack 1-bit signal Relay2State from payload (at bit 25 to bit 26).
    uint32_t Relay2State_raw = 0;
    Relay2State_raw |= unpackShiftRight(in_data[3], 1, 0x2);   // Unpacks bits ______#_ of msg byte 3
    bool Relay2State_val = CAN_DECODE(Relay2State_raw, CANSIG_INVR_INTERNAL_STATES_RELAY2_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_RELAY2_STATE_OFFSET, uint32_t);
    out_msg->Relay2State_value = Relay2State_val;
    
    // Unpack 1-bit signal Relay3State from payload (at bit 26 to bit 27).
    uint32_t Relay3State_raw = 0;
    Relay3State_raw |= unpackShiftRight(in_data[3], 2, 0x4);   // Unpacks bits _____#__ of msg byte 3
    bool Relay3State_val = CAN_DECODE(Relay3State_raw, CANSIG_INVR_INTERNAL_STATES_RELAY3_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_RELAY3_STATE_OFFSET, uint32_t);
    out_msg->Relay3State_value = Relay3State_val;
    
    // Unpack 1-bit signal Relay4State from payload (at bit 27 to bit 28).
    uint32_t Relay4State_raw = 0;
    Relay4State_raw |= unpackShiftRight(in_data[3], 3, 0x8);   // Unpacks bits ____#___ of msg byte 3
    bool Relay4State_val = CAN_DECODE(Relay4State_raw, CANSIG_INVR_INTERNAL_STATES_RELAY4_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_RELAY4_STATE_OFFSET, uint32_t);
    out_msg->Relay4State_value = Relay4State_val;
    
    // Unpack 1-bit signal Relay5State from payload (at bit 28 to bit 29).
    uint32_t Relay5State_raw = 0;
    Relay5State_raw |= unpackShiftRight(in_data[3], 4, 0x10);   // Unpacks bits ___#____ of msg byte 3
    bool Relay5State_val = CAN_DECODE(Relay5State_raw, CANSIG_INVR_INTERNAL_STATES_RELAY5_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_RELAY5_STATE_OFFSET, uint32_t);
    out_msg->Relay5State_value = Relay5State_val;
    
    // Unpack 1-bit signal Relay6State from payload (at bit 29 to bit 30).
    uint32_t Relay6State_raw = 0;
    Relay6State_raw |= unpackShiftRight(in_data[3], 5, 0x20);   // Unpacks bits __#_____ of msg byte 3
    bool Relay6State_val = CAN_DECODE(Relay6State_raw, CANSIG_INVR_INTERNAL_STATES_RELAY6_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_RELAY6_STATE_OFFSET, uint32_t);
    out_msg->Relay6State_value = Relay6State_val;
    
    // Unpack 1-bit signal InverterRunMode from payload (at bit 32 to bit 33).
    uint32_t InverterRunMode_raw = 0;
    InverterRunMode_raw |= unpackShiftRight(in_data[4], 0, 0x1);   // Unpacks bits _______# of msg byte 4
    bool InverterRunMode_val = CAN_DECODE(InverterRunMode_raw, CANSIG_INVR_INTERNAL_STATES_INVERTER_RUN_MODE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVERTER_RUN_MODE_OFFSET, uint32_t);
    out_msg->InverterRunMode_value = InverterRunMode_val;
    
    // Unpack 3-bit signal InverterActiveDischargeState from payload (at bit 37 to bit 40).
    uint32_t InverterActiveDischargeState_raw = 0;
    InverterActiveDischargeState_raw |= unpackShiftRight(in_data[4], 5, 0xe0);   // Unpacks bits ###_____ of msg byte 4
    InverterActiveDischargeState InverterActiveDischargeState_val = CAN_DECODE(InverterActiveDischargeState_raw, CANSIG_INVR_INTERNAL_STATES_INVERTER_ACTIVE_DISCHARGE_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVERTER_ACTIVE_DISCHARGE_STATE_OFFSET, uint32_t);
    out_msg->InverterActiveDischargeState_value = InverterActiveDischargeState_val;
    
    // Unpack 1-bit signal InverterCommandMode from payload (at bit 40 to bit 41).
    uint32_t InverterCommandMode_raw = 0;
    InverterCommandMode_raw |= unpackShiftRight(in_data[5], 0, 0x1);   // Unpacks bits _______# of msg byte 5
    InverterCommandMode InverterCommandMode_val = CAN_DECODE(InverterCommandMode_raw, CANSIG_INVR_INTERNAL_STATES_INVERTER_COMMAND_MODE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVERTER_COMMAND_MODE_OFFSET, uint32_t);
    out_msg->InverterCommandMode_value = InverterCommandMode_val;
    
    // Unpack 4-bit signal RollingCounter from payload (at bit 44 to bit 48).
    uint32_t RollingCounter_raw = 0;
    RollingCounter_raw |= unpackShiftRight(in_data[5], 4, 0xf0);   // Unpacks bits ####____ of msg byte 5
    uint32_t RollingCounter_val = CAN_DECODE(RollingCounter_raw, CANSIG_INVR_INTERNAL_STATES_ROLLING_COUNTER_SCALE, CANSIG_INVR_INTERNAL_STATES_ROLLING_COUNTER_OFFSET, uint32_t);
    out_msg->RollingCounter_value = RollingCounter_val;
    
    // Unpack 1-bit signal InverterEnableState from payload (at bit 48 to bit 49).
    uint32_t InverterEnableState_raw = 0;
    InverterEnableState_raw |= unpackShiftRight(in_data[6], 0, 0x1);   // Unpacks bits _______# of msg byte 6
    bool InverterEnableState_val = CAN_DECODE(InverterEnableState_raw, CANSIG_INVR_INTERNAL_STATES_INVERTER_ENABLE_STATE_SCALE, CANSIG_INVR_INTERNAL_STATES_INVERTER_ENABLE_STATE_OFFSET, uint32_t);
    out_msg->InverterEnableState_value = InverterEnableState_val;
    
    // Unpack 1-bit signal StartModeActive from payload (at bit 54 to bit 55).
    uint32_t StartModeActive_raw = 0;
    StartModeActive_raw |= unpackShiftRight(in_data[6], 6, 0x40);   // Unpacks bits _#______ of msg byte 6
    bool StartModeActive_val = CAN_DECODE(StartModeActive_raw, CANSIG_INVR_INTERNAL_STATES_START_MODE_ACTIVE_SCALE, CANSIG_INVR_INTERNAL_STATES_START_MODE_ACTIVE_OFFSET, uint32_t);
    out_msg->StartModeActive_value = StartModeActive_val;
    
    // Unpack 1-bit signal InverterLockoutEnabled from payload (at bit 55 to bit 56).
    uint32_t InverterLockoutEnabled_raw = 0;
    InverterLockoutEnabled_raw |= unpackShiftRight(in_data[6], 7, 0x80);   // Unpacks bits #_______ of msg byte 6
    InverterLockout InverterLockoutEnabled_val = CAN_DECODE(InverterLockoutEnabled_raw, CANSIG_INVR_INTERNAL_STATES_INVERTER_LOCKOUT_ENABLED_SCALE, CANSIG_INVR_INTERNAL_STATES_INVERTER_LOCKOUT_ENABLED_OFFSET, uint32_t);
    out_msg->InverterLockoutEnabled_value = InverterLockoutEnabled_val;
    
    // Unpack 1-bit signal DirectionCommand from payload (at bit 56 to bit 57).
    uint32_t DirectionCommand_raw = 0;
    DirectionCommand_raw |= unpackShiftRight(in_data[7], 0, 0x1);   // Unpacks bits _______# of msg byte 7
    InverterDirection DirectionCommand_val = CAN_DECODE(DirectionCommand_raw, CANSIG_INVR_INTERNAL_STATES_DIRECTION_COMMAND_SCALE, CANSIG_INVR_INTERNAL_STATES_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_msg->DirectionCommand_value = DirectionCommand_val;
    
    // Unpack 1-bit signal BmsActive from payload (at bit 57 to bit 58).
    uint32_t BmsActive_raw = 0;
    BmsActive_raw |= unpackShiftRight(in_data[7], 1, 0x2);   // Unpacks bits ______#_ of msg byte 7
    bool BmsActive_val = CAN_DECODE(BmsActive_raw, CANSIG_INVR_INTERNAL_STATES_BMS_ACTIVE_SCALE, CANSIG_INVR_INTERNAL_STATES_BMS_ACTIVE_OFFSET, uint32_t);
    out_msg->BmsActive_value = BmsActive_val;
    
    // Unpack 1-bit signal BmsTorqueLimiting from payload (at bit 58 to bit 59).
    uint32_t BmsTorqueLimiting_raw = 0;
    BmsTorqueLimiting_raw |= unpackShiftRight(in_data[7], 2, 0x4);   // Unpacks bits _____#__ of msg byte 7
    bool BmsTorqueLimiting_val = CAN_DECODE(BmsTorqueLimiting_raw, CANSIG_INVR_INTERNAL_STATES_BMS_TORQUE_LIMITING_SCALE, CANSIG_INVR_INTERNAL_STATES_BMS_TORQUE_LIMITING_OFFSET, uint32_t);
    out_msg->BmsTorqueLimiting_value = BmsTorqueLimiting_val;
    
    // Unpack 1-bit signal MaxSpeedLimiting from payload (at bit 59 to bit 60).
    uint32_t MaxSpeedLimiting_raw = 0;
    MaxSpeedLimiting_raw |= unpackShiftRight(in_data[7], 3, 0x8);   // Unpacks bits ____#___ of msg byte 7
    bool MaxSpeedLimiting_val = CAN_DECODE(MaxSpeedLimiting_raw, CANSIG_INVR_INTERNAL_STATES_MAX_SPEED_LIMITING_SCALE, CANSIG_INVR_INTERNAL_STATES_MAX_SPEED_LIMITING_OFFSET, uint32_t);
    out_msg->MaxSpeedLimiting_value = MaxSpeedLimiting_val;
    
    // Unpack 1-bit signal HotSpotLimiting from payload (at bit 60 to bit 61).
    uint32_t HotSpotLimiting_raw = 0;
    HotSpotLimiting_raw |= unpackShiftRight(in_data[7], 4, 0x10);   // Unpacks bits ___#____ of msg byte 7
    bool HotSpotLimiting_val = CAN_DECODE(HotSpotLimiting_raw, CANSIG_INVR_INTERNAL_STATES_HOT_SPOT_LIMITING_SCALE, CANSIG_INVR_INTERNAL_STATES_HOT_SPOT_LIMITING_OFFSET, uint32_t);
    out_msg->HotSpotLimiting_value = HotSpotLimiting_val;
    
    // Unpack 1-bit signal LowSpeedLimiting from payload (at bit 61 to bit 62).
    uint32_t LowSpeedLimiting_raw = 0;
    LowSpeedLimiting_raw |= unpackShiftRight(in_data[7], 5, 0x20);   // Unpacks bits __#_____ of msg byte 7
    bool LowSpeedLimiting_val = CAN_DECODE(LowSpeedLimiting_raw, CANSIG_INVR_INTERNAL_STATES_LOW_SPEED_LIMITING_SCALE, CANSIG_INVR_INTERNAL_STATES_LOW_SPEED_LIMITING_OFFSET, uint32_t);
    out_msg->LowSpeedLimiting_value = LowSpeedLimiting_val;
    
    // Unpack 1-bit signal CoolantTemperatureLimiting from payload (at bit 62 to bit 63).
    uint32_t CoolantTemperatureLimiting_raw = 0;
    CoolantTemperatureLimiting_raw |= unpackShiftRight(in_data[7], 6, 0x40);   // Unpacks bits _#______ of msg byte 7
    bool CoolantTemperatureLimiting_val = CAN_DECODE(CoolantTemperatureLimiting_raw, CANSIG_INVR_INTERNAL_STATES_COOLANT_TEMPERATURE_LIMITING_SCALE, CANSIG_INVR_INTERNAL_STATES_COOLANT_TEMPERATURE_LIMITING_OFFSET, uint32_t);
    out_msg->CoolantTemperatureLimiting_value = CoolantTemperatureLimiting_val;
    
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

