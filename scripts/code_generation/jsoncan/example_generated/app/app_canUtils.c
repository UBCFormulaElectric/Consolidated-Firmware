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

void app_canUtils_JCT_Vitals_pack(const JCT_Vitals_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message JCT_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_______B|BBBBBBBB|BBBBBBBB|BBBBBBBB|BBBBBBBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal JCT_Heartbeat into payload (at bit 0 to bit 1).
    const bool JCT_Heartbeat_val = in_msg->JCT_Heartbeat_value;
    const uint32_t JCT_Heartbeat_raw = CAN_ENCODE(JCT_Heartbeat_val, CANSIG_JCT_VITALS_JCT_HEARTBEAT_SCALE, CANSIG_JCT_VITALS_JCT_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(JCT_Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 32-bit signal JCT_Timestamp into payload (at bit 1 to bit 33).
    const uint32_t JCT_Timestamp_val = in_msg->JCT_Timestamp_value;
    const uint32_t JCT_Timestamp_raw = CAN_ENCODE(JCT_Timestamp_val, CANSIG_JCT_VITALS_JCT_TIMESTAMP_SCALE, CANSIG_JCT_VITALS_JCT_TIMESTAMP_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(JCT_Timestamp_raw, 1, 0xfe);   // Packs bits #######_ of byte 0
    out_data[1] |= packShiftRight(JCT_Timestamp_raw, 7, 0xff);   // Packs bits ######## of byte 1
    out_data[2] |= packShiftRight(JCT_Timestamp_raw, 15, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(JCT_Timestamp_raw, 23, 0xff);   // Packs bits ######## of byte 3
    out_data[4] |= packShiftRight(JCT_Timestamp_raw, 31, 0x1);   // Packs bits _______# of byte 4
    
}

void app_canUtils_JCT_WarningsTest_pack(const JCT_WarningsTest_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message JCT_WarningsTest.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____CBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal JCT_WatchdogTimeout into payload (at bit 0 to bit 1).
    const bool JCT_WatchdogTimeout_val = in_msg->JCT_WatchdogTimeout_value;
    const uint32_t JCT_WatchdogTimeout_raw = CAN_ENCODE(JCT_WatchdogTimeout_val, CANSIG_JCT_WARNINGS_TEST_JCT_WATCHDOG_TIMEOUT_SCALE, CANSIG_JCT_WARNINGS_TEST_JCT_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(JCT_WatchdogTimeout_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 1-bit signal JCT_BoardOvertemp into payload (at bit 1 to bit 2).
    const bool JCT_BoardOvertemp_val = in_msg->JCT_BoardOvertemp_value;
    const uint32_t JCT_BoardOvertemp_raw = CAN_ENCODE(JCT_BoardOvertemp_val, CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERTEMP_SCALE, CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERTEMP_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(JCT_BoardOvertemp_raw, 1, 0x2);   // Packs bits ______#_ of byte 0
    
    // Pack 1-bit signal JCT_BoardOvervoltage into payload (at bit 2 to bit 3).
    const bool JCT_BoardOvervoltage_val = in_msg->JCT_BoardOvervoltage_value;
    const uint32_t JCT_BoardOvervoltage_raw = CAN_ENCODE(JCT_BoardOvervoltage_val, CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERVOLTAGE_SCALE, CANSIG_JCT_WARNINGS_TEST_JCT_BOARD_OVERVOLTAGE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(JCT_BoardOvervoltage_raw, 2, 0x4);   // Packs bits _____#__ of byte 0
    
}

void app_canUtils_JCT_AirShutdownErrors_pack(const JCT_AirShutdownErrors_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message JCT_AirShutdownErrors.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal JCT_DummyAirShutdown into payload (at bit 0 to bit 1).
    const bool JCT_DummyAirShutdown_val = in_msg->JCT_DummyAirShutdown_value;
    const uint32_t JCT_DummyAirShutdown_raw = CAN_ENCODE(JCT_DummyAirShutdown_val, CANSIG_JCT_AIR_SHUTDOWN_ERRORS_JCT_DUMMY_AIR_SHUTDOWN_SCALE, CANSIG_JCT_AIR_SHUTDOWN_ERRORS_JCT_DUMMY_AIR_SHUTDOWN_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(JCT_DummyAirShutdown_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
}

void app_canUtils_JCT_MotorShutdownErrors_pack(const JCT_MotorShutdownErrors_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message JCT_MotorShutdownErrors.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal JCT_DummyMotorShutdown into payload (at bit 0 to bit 1).
    const bool JCT_DummyMotorShutdown_val = in_msg->JCT_DummyMotorShutdown_value;
    const uint32_t JCT_DummyMotorShutdown_raw = CAN_ENCODE(JCT_DummyMotorShutdown_val, CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_JCT_DUMMY_MOTOR_SHUTDOWN_SCALE, CANSIG_JCT_MOTOR_SHUTDOWN_ERRORS_JCT_DUMMY_MOTOR_SHUTDOWN_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(JCT_DummyMotorShutdown_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
}

void app_canUtils_JCT_Status_pack(const JCT_Status_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 5-byte payload for message JCT_Status.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|___DDDDD|DDDDDDDC|CCCCCCCC|CCCBBBBB|BBBBBBBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal JCT_ContactorsClosed into payload (at bit 0 to bit 1).
    const AirState JCT_ContactorsClosed_val = in_msg->JCT_ContactorsClosed_value;
    const uint32_t JCT_ContactorsClosed_raw = CAN_ENCODE(JCT_ContactorsClosed_val, CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_SCALE, CANSIG_JCT_STATUS_JCT_CONTACTORS_CLOSED_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(JCT_ContactorsClosed_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
    // Pack 12-bit signal JCT_Current into payload (at bit 1 to bit 13).
    const float JCT_Current_val = in_msg->JCT_Current_value;
    const uint32_t JCT_Current_raw = CAN_ENCODE(JCT_Current_val, CANSIG_JCT_STATUS_JCT_CURRENT_SCALE, CANSIG_JCT_STATUS_JCT_CURRENT_OFFSET, float);
    out_data[0] |= packShiftLeft(JCT_Current_raw, 1, 0xfe);   // Packs bits #######_ of byte 0
    out_data[1] |= packShiftRight(JCT_Current_raw, 7, 0x1f);   // Packs bits ___##### of byte 1
    
    // Pack 12-bit signal JCT_Voltage into payload (at bit 13 to bit 25).
    const float JCT_Voltage_val = in_msg->JCT_Voltage_value;
    const uint32_t JCT_Voltage_raw = CAN_ENCODE(JCT_Voltage_val, CANSIG_JCT_STATUS_JCT_VOLTAGE_SCALE, CANSIG_JCT_STATUS_JCT_VOLTAGE_OFFSET, float);
    out_data[1] |= packShiftLeft(JCT_Voltage_raw, 5, 0xe0);   // Packs bits ###_____ of byte 1
    out_data[2] |= packShiftRight(JCT_Voltage_raw, 3, 0xff);   // Packs bits ######## of byte 2
    out_data[3] |= packShiftRight(JCT_Voltage_raw, 11, 0x1);   // Packs bits _______# of byte 3
    
    // Pack 12-bit signal JCT_UnsignedTester into payload (at bit 25 to bit 37).
    const int JCT_UnsignedTester_val = in_msg->JCT_UnsignedTester_value;
    const int32_t JCT_UnsignedTester_raw = CAN_SIGNED_ENCODE(JCT_UnsignedTester_val, CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_SCALE, CANSIG_JCT_STATUS_JCT_UNSIGNED_TESTER_OFFSET, int);
    out_data[3] |= packShiftLeft(JCT_UnsignedTester_raw, 1, 0xfe);   // Packs bits #######_ of byte 3
    out_data[4] |= packShiftRight(JCT_UnsignedTester_raw, 7, 0x1f);   // Packs bits ___##### of byte 4
    
}

void app_canUtils_JCT_Warnings_pack(const JCT_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message JCT_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal JCT_Warning_Warning_Test into payload (at bit 0 to bit 1).
    const bool JCT_Warning_Warning_Test_val = in_msg->JCT_Warning_Warning_Test_value;
    const uint32_t JCT_Warning_Warning_Test_raw = CAN_ENCODE(JCT_Warning_Warning_Test_val, CANSIG_JCT_WARNINGS_JCT_WARNING_WARNING_TEST_SCALE, CANSIG_JCT_WARNINGS_JCT_WARNING_WARNING_TEST_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(JCT_Warning_Warning_Test_raw, 0, 0x1);   // Packs bits _______# of byte 0
    
}

void app_canUtils_JCT_Faults_pack(const JCT_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // No data to pack!
}

void app_canUtils_JCT_WarningsCounts_pack(const JCT_WarningsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack 1-byte payload for message JCT_WarningsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____AAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal JCT_Warning_Warning_TestCount into payload (at bit 0 to bit 3).
    const uint32_t JCT_Warning_Warning_TestCount_val = in_msg->JCT_Warning_Warning_TestCount_value;
    const uint32_t JCT_Warning_Warning_TestCount_raw = CAN_ENCODE(JCT_Warning_Warning_TestCount_val, CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_SCALE, CANSIG_JCT_WARNINGS_COUNTS_JCT_WARNING_WARNING_TEST_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(JCT_Warning_Warning_TestCount_raw, 0, 0x7);   // Packs bits _____### of byte 0
    
}

void app_canUtils_JCT_FaultsCounts_pack(const JCT_FaultsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // No data to pack!
}

void app_canUtils_FSM_Apps_unpack(const uint8_t* const in_data, FSM_Apps_Signals* const out_msg)
{
    // Unpack 8-byte payload for message FSM_Apps.
    // |BBBBBBBB|BBBBBBBB|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 32-bit signal FSM_PappsMappedPedalPercentage from payload (at bit 0 to bit 32).
    uint32_t FSM_PappsMappedPedalPercentage_raw = 0;
    FSM_PappsMappedPedalPercentage_raw |= unpackShiftRight(in_data[0], 0, 0xff);   // Unpacks bits ######## of msg byte 0
    FSM_PappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[1], 8, 0xff);   // Unpacks bits ######## of msg byte 1
    FSM_PappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[2], 16, 0xff);   // Unpacks bits ######## of msg byte 2
    FSM_PappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[3], 24, 0xff);   // Unpacks bits ######## of msg byte 3
    float FSM_PappsMappedPedalPercentage_val = CAN_DECODE(FSM_PappsMappedPedalPercentage_raw, CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_FSM_PAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET, float);
    out_msg->FSM_PappsMappedPedalPercentage_value = FSM_PappsMappedPedalPercentage_val;
    
    // Unpack 32-bit signal FSM_SappsMappedPedalPercentage from payload (at bit 32 to bit 64).
    uint32_t FSM_SappsMappedPedalPercentage_raw = 0;
    FSM_SappsMappedPedalPercentage_raw |= unpackShiftRight(in_data[4], 0, 0xff);   // Unpacks bits ######## of msg byte 4
    FSM_SappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[5], 8, 0xff);   // Unpacks bits ######## of msg byte 5
    FSM_SappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[6], 16, 0xff);   // Unpacks bits ######## of msg byte 6
    FSM_SappsMappedPedalPercentage_raw |= unpackShiftLeft(in_data[7], 24, 0xff);   // Unpacks bits ######## of msg byte 7
    float FSM_SappsMappedPedalPercentage_val = CAN_DECODE(FSM_SappsMappedPedalPercentage_raw, CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_SCALE, CANSIG_FSM_APPS_FSM_SAPPS_MAPPED_PEDAL_PERCENTAGE_OFFSET, float);
    out_msg->FSM_SappsMappedPedalPercentage_value = FSM_SappsMappedPedalPercentage_val;
    
}

void app_canUtils_FSM_Warnings_unpack(const uint8_t* const in_data, FSM_Warnings_Signals* const out_msg)
{
    // Unpack 1-byte payload for message FSM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_Warning_Warning_Test1 from payload (at bit 0 to bit 1).
    uint32_t FSM_Warning_Warning_Test1_raw = 0;
    FSM_Warning_Warning_Test1_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_Warning_Warning_Test1_val = CAN_DECODE(FSM_Warning_Warning_Test1_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST1_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST1_OFFSET, uint32_t);
    out_msg->FSM_Warning_Warning_Test1_value = FSM_Warning_Warning_Test1_val;
    
    // Unpack 1-bit signal FSM_Warning_Warning_Test2 from payload (at bit 1 to bit 2).
    uint32_t FSM_Warning_Warning_Test2_raw = 0;
    FSM_Warning_Warning_Test2_raw |= unpackShiftRight(in_data[0], 1, 0x2);   // Unpacks bits ______#_ of msg byte 0
    bool FSM_Warning_Warning_Test2_val = CAN_DECODE(FSM_Warning_Warning_Test2_raw, CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST2_SCALE, CANSIG_FSM_WARNINGS_FSM_WARNING_WARNING_TEST2_OFFSET, uint32_t);
    out_msg->FSM_Warning_Warning_Test2_value = FSM_Warning_Warning_Test2_val;
    
}

void app_canUtils_FSM_Faults_unpack(const uint8_t* const in_data, FSM_Faults_Signals* const out_msg)
{
    // Unpack 1-byte payload for message FSM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______A|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    // Unpack 1-bit signal FSM_Fault_Fault_Test3 from payload (at bit 0 to bit 1).
    uint32_t FSM_Fault_Fault_Test3_raw = 0;
    FSM_Fault_Fault_Test3_raw |= unpackShiftRight(in_data[0], 0, 0x1);   // Unpacks bits _______# of msg byte 0
    bool FSM_Fault_Fault_Test3_val = CAN_DECODE(FSM_Fault_Fault_Test3_raw, CANSIG_FSM_FAULTS_FSM_FAULT_FAULT_TEST3_SCALE, CANSIG_FSM_FAULTS_FSM_FAULT_FAULT_TEST3_OFFSET, uint32_t);
    out_msg->FSM_Fault_Fault_Test3_value = FSM_Fault_Fault_Test3_val;
    
}

