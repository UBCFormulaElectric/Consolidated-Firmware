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

void App_CanUtils_JctVitals_Pack(const JctVitals_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message JctVitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_______B|BBBBBBBB|BBBBBBBB|BBBBBBBB|BBBBBBBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal Heartbeat into payload (at bit 0 to bit 1).
    const bool Heartbeat_val = in_msg->Heartbeat_value;
    const uint32_t Heartbeat_raw = CAN_ENCODE(Heartbeat_val, CANSIG_JCT_VITALS_HEARTBEAT_SCALE, CANSIG_JCT_VITALS_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 32-bit signal Timestamp into payload (at bit 1 to bit 33).
    const uint32_t Timestamp_val = in_msg->Timestamp_value;
    const uint32_t Timestamp_raw = CAN_ENCODE(Timestamp_val, CANSIG_JCT_VITALS_TIMESTAMP_SCALE, CANSIG_JCT_VITALS_TIMESTAMP_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(Timestamp_raw, 1, 0xfe);   // Packs bits #######_ of byte 0
    out_data[1] |= packShiftRight(Timestamp_raw, 7, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(Timestamp_raw, 15, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(Timestamp_raw, 23, 0xff);   // Packs bits ######## of byte 3
    out_data[4] |= packShiftRight(Timestamp_raw, 31, 0x1);   // Packs bits _______# of byte 4
    
}

void App_CanUtils_JctWarnings_Pack(const JctWarnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message JctWarnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal WatchdogTimeout into payload (at bit 0 to bit 1).
    const bool WatchdogTimeout_val = in_msg->WatchdogTimeout_value;
    const uint32_t WatchdogTimeout_raw = CAN_ENCODE(WatchdogTimeout_val, CANSIG_JCT_WARNINGS_WATCHDOG_TIMEOUT_SCALE, CANSIG_JCT_WARNINGS_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(WatchdogTimeout_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal BoardOvertemp into payload (at bit 1 to bit 2).
    const bool BoardOvertemp_val = in_msg->BoardOvertemp_value;
    const uint32_t BoardOvertemp_raw = CAN_ENCODE(BoardOvertemp_val, CANSIG_JCT_WARNINGS_BOARD_OVERTEMP_SCALE, CANSIG_JCT_WARNINGS_BOARD_OVERTEMP_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BoardOvertemp_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal BoardOvervoltage into payload (at bit 2 to bit 3).
    const bool BoardOvervoltage_val = in_msg->BoardOvervoltage_value;
    const uint32_t BoardOvervoltage_raw = CAN_ENCODE(BoardOvervoltage_val, CANSIG_JCT_WARNINGS_BOARD_OVERVOLTAGE_SCALE, CANSIG_JCT_WARNINGS_BOARD_OVERVOLTAGE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(BoardOvervoltage_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
}

void App_CanUtils_JctAirShutdownErrors_Pack(const JctAirShutdownErrors_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message JctAirShutdownErrors.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DummyAirShutdown into payload (at bit 0 to bit 1).
    const bool DummyAirShutdown_val = in_msg->DummyAirShutdown_value;
    const uint32_t DummyAirShutdown_raw = CAN_ENCODE(DummyAirShutdown_val, CANSIG_JCT_AIR_SHUTDOWN_ERRORS_DUMMY_AIR_SHUTDOWN_SCALE, CANSIG_JCT_AIR_SHUTDOWN_ERRORS_DUMMY_AIR_SHUTDOWN_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DummyAirShutdown_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
}

void App_CanUtils_JctMotorShutdownErrors_Pack(const JctMotorShutdownErrors_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message JctMotorShutdownErrors.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal DummyMotorShutdown into payload (at bit 0 to bit 1).
    const bool DummyMotorShutdown_val = in_msg->DummyMotorShutdown_value;
    const uint32_t DummyMotorShutdown_raw = CAN_ENCODE(DummyMotorShutdown_val, CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_DUMMY_MOTOR_SHUTDOWN_SCALE, CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_DUMMY_MOTOR_SHUTDOWN_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(DummyMotorShutdown_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
}

void App_CanUtils_JctStatus_Pack(const JctStatus_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message JctStatus.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|___DDDDD|DDDDDDDC|CCCCCCCC|CCCBBBBB|BBBBBBBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal ContactorsClosed into payload (at bit 0 to bit 1).
    const AirState ContactorsClosed_val = in_msg->ContactorsClosed_value;
    const uint32_t ContactorsClosed_raw = CAN_ENCODE(ContactorsClosed_val, CANSIG_JCT_STATUS_CONTACTORS_CLOSED_SCALE, CANSIG_JCT_STATUS_CONTACTORS_CLOSED_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(ContactorsClosed_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 12-bit signal Current into payload (at bit 1 to bit 13).
    const float Current_val = in_msg->Current_value;
    const uint32_t Current_raw = CAN_ENCODE(Current_val, CANSIG_JCT_STATUS_CURRENT_SCALE, CANSIG_JCT_STATUS_CURRENT_OFFSET, float);
    out_data[0] |= packShiftLeft(Current_raw, 1, 0xfe);   // Packs bits #######_ of byte 0
    out_data[1] |= packShiftRight(Current_raw, 7, 0x1f);   // Packs bits ___##### of byte 1
    
    // Pack 12-bit signal Voltage into payload (at bit 13 to bit 25).
    const float Voltage_val = in_msg->Voltage_value;
    const uint32_t Voltage_raw = CAN_ENCODE(Voltage_val, CANSIG_JCT_STATUS_VOLTAGE_SCALE, CANSIG_JCT_STATUS_VOLTAGE_OFFSET, float);
    out_data[1] |= packShiftLeft(Voltage_raw, 5, 0xe0);   // Packs bits ###_____ of byte 1
    out_data[2] |= packShiftRight(Voltage_raw, 3, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(Voltage_raw, 11, 0x1);   // Packs bits _______# of byte 3
    
    // Pack 12-bit signal UnsignedTester into payload (at bit 25 to bit 37).
    const int UnsignedTester_val = in_msg->UnsignedTester_value;
    const uint32_t UnsignedTester_raw = CAN_ENCODE(UnsignedTester_val, CANSIG_JCT_STATUS_UNSIGNED_TESTER_SCALE, CANSIG_JCT_STATUS_UNSIGNED_TESTER_OFFSET, int);
    out_data[3] |= packShiftLeft(UnsignedTester_raw, 1, 0xfe);   // Packs bits #######_ of byte 3
    out_data[4] |= packShiftRight(UnsignedTester_raw, 7, 0x1f);   // Packs bits ___##### of byte 4
    
}

void App_CanUtils_JCT_Warnings_Pack(const JCT_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message JCT_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal JCT_TEST into payload (at bit 0 to bit 1).
    const bool JCT_TEST_val = in_msg->JCT_TEST_value;
    const uint32_t JCT_TEST_raw = CAN_ENCODE(JCT_TEST_val, CANSIG_JCT_WARNINGS_JCT_TEST_SCALE, CANSIG_JCT_WARNINGS_JCT_TEST_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(JCT_TEST_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
}

void App_CanUtils_JCT_Faults_Pack(const JCT_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // No data to pack!
}

void App_CanUtils_FsmApps_Unpack(const uint8_t* const in_data, FsmApps_Signals* const out_msg)
{
    // Unpack 8-byte payload for message FsmApps.
    // |BBBBBBBB|BBBBBBBB|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 32-bit signal PappsMappedPedalPercentage from payload (at bit 0 to bit 32).
    uint32_t PappsMappedPedalPercentage_raw = 0;
    PappsMappedPedalPercentage_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    PappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    PappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[2], 16, 0xff);   // Unpacks bits ######## of msg byte 2
    PappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[3], 24, 0xff);   // Unpacks bits ######## of msg byte 3
    float PappsMappedPedalPercentage_val = CAN_DECODE(PappsMappedPedalPercentage_raw, CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_PAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET, float);
    out_msg->PappsMappedPedalPercentage_value = PappsMappedPedalPercentage_val;
    
    // Unpack 32-bit signal SappsMappedPedalPercentage from payload (at bit 32 to bit 64).
    uint32_t SappsMappedPedalPercentage_raw = 0;
    SappsMappedPedalPercentage_raw |= unpackShiftRight(in_data[4], 0, 0xff);   // Unpacks bits ######## of msg byte 4
    SappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[5], 8, 0xff);   // Unpacks bits ######## of msg byte 5
    SappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[6], 16, 0xff);   // Unpacks bits ######## of msg byte 6
    SappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[7], 24, 0xff);   // Unpacks bits ######## of msg byte 7
    float SappsMappedPedalPercentage_val = CAN_DECODE(SappsMappedPedalPercentage_raw, CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_SAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET, float);
    out_msg->SappsMappedPedalPercentage_value = SappsMappedPedalPercentage_val;
    
}

void App_CanUtils_FsmWarnings_Unpack(const uint8_t* const in_data, FsmWarnings_Signals* const out_msg)
{
    // Unpack 3-byte payload for message FsmWarnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|___PONMM|LLKKJJII|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal PappsOutOfRange from payload (at bit 0 to bit 1).
    uint32_t PappsOutOfRange_raw = 0;
    PappsOutOfRange_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool PappsOutOfRange_val = CAN_DECODE(PappsOutOfRange_raw, CANSIG_FSM_WARNINGS_PAPPS_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_PAPPS_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->PappsOutOfRange_value = PappsOutOfRange_val;
    
    // Unpack 1-bit signal SappsOutOfRange from payload (at bit 1 to bit 2).
    uint32_t SappsOutOfRange_raw = 0;
    SappsOutOfRange_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool SappsOutOfRange_val = CAN_DECODE(SappsOutOfRange_raw, CANSIG_FSM_WARNINGS_SAPPS_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_SAPPS_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->SappsOutOfRange_value = SappsOutOfRange_val;
    
    // Unpack 1-bit signal StackWatermarkAboveThresholdTask1Hz from payload (at bit 2 to bit 3).
    uint32_t StackWatermarkAboveThresholdTask1Hz_raw = 0;
    StackWatermarkAboveThresholdTask1Hz_raw |= unpackShiftRight(in_data[0], 2, 0x4);   // Unpacks bits _____#__ of msg byte 0
    bool StackWatermarkAboveThresholdTask1Hz_val = CAN_DECODE(StackWatermarkAboveThresholdTask1Hz_raw, CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_SCALE, CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1_HZ_OFFSET, uint32_t);
    out_msg->StackWatermarkAboveThresholdTask1Hz_value = StackWatermarkAboveThresholdTask1Hz_val;
    
    // Unpack 1-bit signal StackWatermarkAboveThresholdTask1kHz from payload (at bit 3 to bit 4).
    uint32_t StackWatermarkAboveThresholdTask1kHz_raw = 0;
    StackWatermarkAboveThresholdTask1kHz_raw |= unpackShiftRight(in_data[0], 3, 0x8);   // Unpacks bits ____#___ of msg byte 0
    bool StackWatermarkAboveThresholdTask1kHz_val = CAN_DECODE(StackWatermarkAboveThresholdTask1kHz_raw, CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_SCALE, CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1K_HZ_OFFSET, uint32_t);
    out_msg->StackWatermarkAboveThresholdTask1kHz_value = StackWatermarkAboveThresholdTask1kHz_val;
    
    // Unpack 1-bit signal StackWatermarkAboveThresholdTaskCanRx from payload (at bit 4 to bit 5).
    uint32_t StackWatermarkAboveThresholdTaskCanRx_raw = 0;
    StackWatermarkAboveThresholdTaskCanRx_raw |= unpackShiftRight(in_data[0], 4, 0x10);   // Unpacks bits ___#____ of msg byte 0
    bool StackWatermarkAboveThresholdTaskCanRx_val = CAN_DECODE(StackWatermarkAboveThresholdTaskCanRx_raw, CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_SCALE, CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_RX_OFFSET, uint32_t);
    out_msg->StackWatermarkAboveThresholdTaskCanRx_value = StackWatermarkAboveThresholdTaskCanRx_val;
    
    // Unpack 1-bit signal StackWatermarkAboveThresholdTaskCanTx from payload (at bit 5 to bit 6).
    uint32_t StackWatermarkAboveThresholdTaskCanTx_raw = 0;
    StackWatermarkAboveThresholdTaskCanTx_raw |= unpackShiftRight(in_data[0], 5, 0x20);   // Unpacks bits __#_____ of msg byte 0
    bool StackWatermarkAboveThresholdTaskCanTx_val = CAN_DECODE(StackWatermarkAboveThresholdTaskCanTx_raw, CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_SCALE, CANSIG_FSM_WARNINGS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK_CAN_TX_OFFSET, uint32_t);
    out_msg->StackWatermarkAboveThresholdTaskCanTx_value = StackWatermarkAboveThresholdTaskCanTx_val;
    
    // Unpack 1-bit signal WatchdogFault from payload (at bit 6 to bit 7).
    uint32_t WatchdogFault_raw = 0;
    WatchdogFault_raw |= unpackShiftRight(in_data[0], 6, 0x40);   // Unpacks bits _#______ of msg byte 0
    bool WatchdogFault_val = CAN_DECODE(WatchdogFault_raw, CANSIG_FSM_WARNINGS_WATCHDOG_FAULT_SCALE, CANSIG_FSM_WARNINGS_WATCHDOG_FAULT_OFFSET, uint32_t);
    out_msg->WatchdogFault_value = WatchdogFault_val;
    
    // Unpack 1-bit signal BspdFault from payload (at bit 7 to bit 8).
    uint32_t BspdFault_raw = 0;
    BspdFault_raw |= unpackShiftRight(in_data[0], 7, 0x80);   // Unpacks bits #_______ of msg byte 0
    bool BspdFault_val = CAN_DECODE(BspdFault_raw, CANSIG_FSM_WARNINGS_BSPD_FAULT_SCALE, CANSIG_FSM_WARNINGS_BSPD_FAULT_OFFSET, uint32_t);
    out_msg->BspdFault_value = BspdFault_val;
    
    // Unpack 2-bit signal LeftWheelSpeedOutOfRange from payload (at bit 8 to bit 10).
    uint32_t LeftWheelSpeedOutOfRange_raw = 0;
    LeftWheelSpeedOutOfRange_raw |= unpackShiftRight(in_data[1], 0, 0x3);   // Unpacks bits ______## of msg byte 1
    uint32_t LeftWheelSpeedOutOfRange_val = CAN_DECODE(LeftWheelSpeedOutOfRange_raw, CANSIG_FSM_WARNINGS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->LeftWheelSpeedOutOfRange_value = LeftWheelSpeedOutOfRange_val;
    
    // Unpack 2-bit signal RightWheelSpeedOutOfRange from payload (at bit 10 to bit 12).
    uint32_t RightWheelSpeedOutOfRange_raw = 0;
    RightWheelSpeedOutOfRange_raw |= unpackShiftRight(in_data[1], 2, 0xc);   // Unpacks bits ____##__ of msg byte 1
    uint32_t RightWheelSpeedOutOfRange_val = CAN_DECODE(RightWheelSpeedOutOfRange_raw, CANSIG_FSM_WARNINGS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->RightWheelSpeedOutOfRange_value = RightWheelSpeedOutOfRange_val;
    
    // Unpack 2-bit signal FlowRateOutOfRange from payload (at bit 12 to bit 14).
    uint32_t FlowRateOutOfRange_raw = 0;
    FlowRateOutOfRange_raw |= unpackShiftRight(in_data[1], 4, 0x30);   // Unpacks bits __##____ of msg byte 1
    uint32_t FlowRateOutOfRange_val = CAN_DECODE(FlowRateOutOfRange_raw, CANSIG_FSM_WARNINGS_FLOW_RATE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_FLOW_RATE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->FlowRateOutOfRange_value = FlowRateOutOfRange_val;
    
    // Unpack 2-bit signal SteeringAngleOutOfRange from payload (at bit 14 to bit 16).
    uint32_t SteeringAngleOutOfRange_raw = 0;
    SteeringAngleOutOfRange_raw |= unpackShiftRight(in_data[1], 6, 0xc0);   // Unpacks bits ##______ of msg byte 1
    uint32_t SteeringAngleOutOfRange_val = CAN_DECODE(SteeringAngleOutOfRange_raw, CANSIG_FSM_WARNINGS_STEERING_ANGLE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_STEERING_ANGLE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->SteeringAngleOutOfRange_value = SteeringAngleOutOfRange_val;
    
    // Unpack 2-bit signal BrakePressureOutOfRange from payload (at bit 16 to bit 18).
    uint32_t BrakePressureOutOfRange_raw = 0;
    BrakePressureOutOfRange_raw |= unpackShiftRight(in_data[2], 0, 0x3);   // Unpacks bits ______## of msg byte 2
    uint32_t BrakePressureOutOfRange_val = CAN_DECODE(BrakePressureOutOfRange_raw, CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OUT_OF_RANGE_OFFSET, uint32_t);
    out_msg->BrakePressureOutOfRange_value = BrakePressureOutOfRange_val;
    
    // Unpack 1-bit signal BrakePressureSc from payload (at bit 18 to bit 19).
    uint32_t BrakePressureSc_raw = 0;
    BrakePressureSc_raw |= unpackShiftRight(in_data[2], 2, 0x4);   // Unpacks bits _____#__ of msg byte 2
    bool BrakePressureSc_val = CAN_DECODE(BrakePressureSc_raw, CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_SC_SCALE, CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_SC_OFFSET, uint32_t);
    out_msg->BrakePressureSc_value = BrakePressureSc_val;
    
    // Unpack 1-bit signal BrakePressureOc from payload (at bit 19 to bit 20).
    uint32_t BrakePressureOc_raw = 0;
    BrakePressureOc_raw |= unpackShiftRight(in_data[2], 3, 0x8);   // Unpacks bits ____#___ of msg byte 2
    bool BrakePressureOc_val = CAN_DECODE(BrakePressureOc_raw, CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OC_SCALE, CANSIG_FSM_WARNINGS_BRAKE_PRESSURE_OC_OFFSET, uint32_t);
    out_msg->BrakePressureOc_value = BrakePressureOc_val;
    
    // Unpack 1-bit signal SteeringWheelBroke from payload (at bit 20 to bit 21).
    uint32_t SteeringWheelBroke_raw = 0;
    SteeringWheelBroke_raw |= unpackShiftRight(in_data[2], 4, 0x10);   // Unpacks bits ___#____ of msg byte 2
    bool SteeringWheelBroke_val = CAN_DECODE(SteeringWheelBroke_raw, CANSIG_FSM_WARNINGS_STEERING_WHEEL_BROKE_SCALE, CANSIG_FSM_WARNINGS_STEERING_WHEEL_BROKE_OFFSET, uint32_t);
    out_msg->SteeringWheelBroke_value = SteeringWheelBroke_val;
    
}

void App_CanUtils_FSM_Warnings_Unpack(const uint8_t* const in_data, FSM_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message FSM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_TEST1 from payload (at bit 0 to bit 1).
    uint32_t FSM_TEST1_raw = 0;
    FSM_TEST1_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_TEST1_val = CAN_DECODE(FSM_TEST1_raw, CANSIG_FSM_WARNINGS_FSM_TEST1_SCALE, CANSIG_FSM_WARNINGS_FSM_TEST1_OFFSET, uint32_t);
    out_msg->FSM_TEST1_value = FSM_TEST1_val;
    
    // Unpack 1-bit signal FSM_TEST2 from payload (at bit 1 to bit 2).
    uint32_t FSM_TEST2_raw = 0;
    FSM_TEST2_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool FSM_TEST2_val = CAN_DECODE(FSM_TEST2_raw, CANSIG_FSM_WARNINGS_FSM_TEST2_SCALE, CANSIG_FSM_WARNINGS_FSM_TEST2_OFFSET, uint32_t);
    out_msg->FSM_TEST2_value = FSM_TEST2_val;
    
}

void App_CanUtils_FSM_Faults_Unpack(const uint8_t* const in_data, FSM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message FSM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_FAULT_TEST3 from payload (at bit 0 to bit 1).
    uint32_t FSM_FAULT_TEST3_raw = 0;
    FSM_FAULT_TEST3_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_FAULT_TEST3_val = CAN_DECODE(FSM_FAULT_TEST3_raw, CANSIG_FSM_FAULTS_FSM_FAULT_TEST3_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_TEST3_OFFSET, uint32_t);
    out_msg->FSM_FAULT_TEST3_value = FSM_FAULT_TEST3_val;
    
}

