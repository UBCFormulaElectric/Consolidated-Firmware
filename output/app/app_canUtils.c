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




 
// pack function for VC_Vitals
void app_canUtils_VC_Vitals_pack(const VC_Vitals_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______EE|EEDDDDDD|DDDDCBBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal VC_Heartbeat into payload at [ bit 0 to bit 1 ).
    const bool VC_Heartbeat_val = in_msg->VC_Heartbeat_value;
    const uint32_t VC_Heartbeat_raw = CAN_ENCODE(VC_Heartbeat_val, CANSIG_VC_HEARTBEAT_SCALE, CANSIG_VC_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_Heartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 2-bit signal VC_State into payload at [ bit 1 to bit 3 ).
    const VcState VC_State_val = in_msg->VC_State_value;
    const uint32_t VC_State_raw = CAN_ENCODE(VC_State_val, CANSIG_VC_STATE_SCALE, CANSIG_VC_STATE_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_State_raw, 1, 0x6);   // Packs bits _____##_ of byte 0 
    
    // Pack 1-bit signal VC_BuzzerOn into payload at [ bit 3 to bit 4 ).
    const bool VC_BuzzerOn_val = in_msg->VC_BuzzerOn_value;
    const uint32_t VC_BuzzerOn_raw = CAN_ENCODE(VC_BuzzerOn_val, CANSIG_VC_BUZZER_ON_SCALE, CANSIG_VC_BUZZER_ON_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_BuzzerOn_raw, 3, 0x8);   // Packs bits ____#___ of byte 0 
    
    // Pack 10-bit signal VC_NumberOfCanDataLogs into payload at [ bit 4 to bit 14 ).
    const uint32_t VC_NumberOfCanDataLogs_val = in_msg->VC_NumberOfCanDataLogs_value;
    const uint32_t VC_NumberOfCanDataLogs_raw = CAN_ENCODE(VC_NumberOfCanDataLogs_val, CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_SCALE, CANSIG_VC_NUMBER_OF_CAN_DATA_LOGS_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_NumberOfCanDataLogs_raw, 4, 0xF0);   // Packs bits ####____ of byte 0 
    out_data[1] |= packShiftRight(VC_NumberOfCanDataLogs_raw, 4, 0x3F);   // Packs bits __###### of byte 1 
    
    // Pack 4-bit signal VC_CanLoggingRemainingErrors into payload at [ bit 14 to bit 18 ).
    const uint32_t VC_CanLoggingRemainingErrors_val = in_msg->VC_CanLoggingRemainingErrors_value;
    const uint32_t VC_CanLoggingRemainingErrors_raw = CAN_ENCODE(VC_CanLoggingRemainingErrors_val, CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_SCALE, CANSIG_VC_CAN_LOGGING_REMAINING_ERRORS_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_CanLoggingRemainingErrors_raw, 6, 0xC0);   // Packs bits ##______ of byte 1 
    out_data[2] |= packShiftRight(VC_CanLoggingRemainingErrors_raw, 2, 0x3);   // Packs bits ______## of byte 2 
    
    
}
 
// pack function for VC_LeftInverterCommand
void app_canUtils_VC_LeftInverterCommand_pack(const VC_LeftInverterCommand_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_LeftInverterCommand.
    // |GGGGGGGG|GGGGGGGG|_____FED|_______C|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal VC_LeftInverterTorqueCommand into payload at [ bit 0 to bit 16 ).
    const float VC_LeftInverterTorqueCommand_val = in_msg->VC_LeftInverterTorqueCommand_value;
    const int32_t VC_LeftInverterTorqueCommand_raw = CAN_SIGNED_ENCODE(VC_LeftInverterTorqueCommand_val, CANSIG_VC_LEFT_INVERTER_TORQUE_COMMAND_SCALE, CANSIG_VC_LEFT_INVERTER_TORQUE_COMMAND_OFFSET, int32_t);
    out_data[0] |= packShiftRight(VC_LeftInverterTorqueCommand_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_LeftInverterTorqueCommand_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    
    // Pack 16-bit signal VC_LeftInverterSpeedCommand into payload at [ bit 16 to bit 32 ).
    const int VC_LeftInverterSpeedCommand_val = in_msg->VC_LeftInverterSpeedCommand_value;
    const int32_t VC_LeftInverterSpeedCommand_raw = CAN_SIGNED_ENCODE(VC_LeftInverterSpeedCommand_val, CANSIG_VC_LEFT_INVERTER_SPEED_COMMAND_SCALE, CANSIG_VC_LEFT_INVERTER_SPEED_COMMAND_OFFSET, int32_t);
    out_data[2] |= packShiftRight(VC_LeftInverterSpeedCommand_raw, 0, 0xFF);   // Packs bits ######## of byte 2 
    out_data[3] |= packShiftRight(VC_LeftInverterSpeedCommand_raw, 8, 0xFF);   // Packs bits ######## of byte 3 
    
    // Pack 1-bit signal VC_LeftInverterDirectionCommand into payload at [ bit 32 to bit 33 ).
    const InverterDirection VC_LeftInverterDirectionCommand_val = in_msg->VC_LeftInverterDirectionCommand_value;
    const uint32_t VC_LeftInverterDirectionCommand_raw = CAN_ENCODE(VC_LeftInverterDirectionCommand_val, CANSIG_VC_LEFT_INVERTER_DIRECTION_COMMAND_SCALE, CANSIG_VC_LEFT_INVERTER_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(VC_LeftInverterDirectionCommand_raw, 0, 0x1);   // Packs bits _______# of byte 4 
    
    // Pack 1-bit signal VC_LeftInverterEnable into payload at [ bit 40 to bit 41 ).
    const bool VC_LeftInverterEnable_val = in_msg->VC_LeftInverterEnable_value;
    const uint32_t VC_LeftInverterEnable_raw = CAN_ENCODE(VC_LeftInverterEnable_val, CANSIG_VC_LEFT_INVERTER_ENABLE_SCALE, CANSIG_VC_LEFT_INVERTER_ENABLE_OFFSET, uint32_t);
    out_data[5] |= packShiftRight(VC_LeftInverterEnable_raw, 0, 0x1);   // Packs bits _______# of byte 5 
    
    // Pack 1-bit signal VC_LeftInverterEnableDischarge into payload at [ bit 41 to bit 42 ).
    const bool VC_LeftInverterEnableDischarge_val = in_msg->VC_LeftInverterEnableDischarge_value;
    const uint32_t VC_LeftInverterEnableDischarge_raw = CAN_ENCODE(VC_LeftInverterEnableDischarge_val, CANSIG_VC_LEFT_INVERTER_ENABLE_DISCHARGE_SCALE, CANSIG_VC_LEFT_INVERTER_ENABLE_DISCHARGE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(VC_LeftInverterEnableDischarge_raw, 1, 0x2);   // Packs bits ______#_ of byte 5 
    
    // Pack 1-bit signal VC_LeftInverterEnableSpeedMode into payload at [ bit 42 to bit 43 ).
    const bool VC_LeftInverterEnableSpeedMode_val = in_msg->VC_LeftInverterEnableSpeedMode_value;
    const uint32_t VC_LeftInverterEnableSpeedMode_raw = CAN_ENCODE(VC_LeftInverterEnableSpeedMode_val, CANSIG_VC_LEFT_INVERTER_ENABLE_SPEED_MODE_SCALE, CANSIG_VC_LEFT_INVERTER_ENABLE_SPEED_MODE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(VC_LeftInverterEnableSpeedMode_raw, 2, 0x4);   // Packs bits _____#__ of byte 5 
    
    // Pack 16-bit signal VC_LeftInverterTorqueLimit into payload at [ bit 48 to bit 64 ).
    const float VC_LeftInverterTorqueLimit_val = in_msg->VC_LeftInverterTorqueLimit_value;
    const int32_t VC_LeftInverterTorqueLimit_raw = CAN_SIGNED_ENCODE(VC_LeftInverterTorqueLimit_val, CANSIG_VC_LEFT_INVERTER_TORQUE_LIMIT_SCALE, CANSIG_VC_LEFT_INVERTER_TORQUE_LIMIT_OFFSET, int32_t);
    out_data[6] |= packShiftRight(VC_LeftInverterTorqueLimit_raw, 0, 0xFF);   // Packs bits ######## of byte 6 
    out_data[7] |= packShiftRight(VC_LeftInverterTorqueLimit_raw, 8, 0xFF);   // Packs bits ######## of byte 7 
    
    
}
 
// pack function for VC_INVL_ReadWriteParamCommand
void app_canUtils_VC_INVL_ReadWriteParamCommand_pack(const VC_INVL_ReadWriteParamCommand_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_INVL_ReadWriteParamCommand.
    // |xxxxxxxx|xxxxxxxx|CCCCCCCC|CCCCCCCC|________|_______B|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal VC_INVL_CommandParameterAddress into payload at [ bit 0 to bit 16 ).
    const uint32_t VC_INVL_CommandParameterAddress_val = in_msg->VC_INVL_CommandParameterAddress_value;
    const uint32_t VC_INVL_CommandParameterAddress_raw = CAN_ENCODE(VC_INVL_CommandParameterAddress_val, CANSIG_VC_INVL_COMMAND_PARAMETER_ADDRESS_SCALE, CANSIG_VC_INVL_COMMAND_PARAMETER_ADDRESS_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_INVL_CommandParameterAddress_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_INVL_CommandParameterAddress_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    
    // Pack 1-bit signal VC_INVL_CommandReadWrite into payload at [ bit 16 to bit 17 ).
    const InverterReadWriteCommand VC_INVL_CommandReadWrite_val = in_msg->VC_INVL_CommandReadWrite_value;
    const uint32_t VC_INVL_CommandReadWrite_raw = CAN_ENCODE(VC_INVL_CommandReadWrite_val, CANSIG_VC_INVL_COMMAND_READ_WRITE_SCALE, CANSIG_VC_INVL_COMMAND_READ_WRITE_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(VC_INVL_CommandReadWrite_raw, 0, 0x1);   // Packs bits _______# of byte 2 
    
    // Pack 16-bit signal VC_INVL_CommandData into payload at [ bit 32 to bit 48 ).
    const uint32_t VC_INVL_CommandData_val = in_msg->VC_INVL_CommandData_value;
    const uint32_t VC_INVL_CommandData_raw = CAN_ENCODE(VC_INVL_CommandData_val, CANSIG_VC_INVL_COMMAND_DATA_SCALE, CANSIG_VC_INVL_COMMAND_DATA_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(VC_INVL_CommandData_raw, 0, 0xFF);   // Packs bits ######## of byte 4 
    out_data[5] |= packShiftRight(VC_INVL_CommandData_raw, 8, 0xFF);   // Packs bits ######## of byte 5 
    
    
}
 
// pack function for VC_RightInverterCommand
void app_canUtils_VC_RightInverterCommand_pack(const VC_RightInverterCommand_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_RightInverterCommand.
    // |GGGGGGGG|GGGGGGGG|_____FED|_______C|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal VC_RightInverterTorqueCommand into payload at [ bit 0 to bit 16 ).
    const float VC_RightInverterTorqueCommand_val = in_msg->VC_RightInverterTorqueCommand_value;
    const int32_t VC_RightInverterTorqueCommand_raw = CAN_SIGNED_ENCODE(VC_RightInverterTorqueCommand_val, CANSIG_VC_RIGHT_INVERTER_TORQUE_COMMAND_SCALE, CANSIG_VC_RIGHT_INVERTER_TORQUE_COMMAND_OFFSET, int32_t);
    out_data[0] |= packShiftRight(VC_RightInverterTorqueCommand_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_RightInverterTorqueCommand_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    
    // Pack 16-bit signal VC_RightInverterSpeedCommand into payload at [ bit 16 to bit 32 ).
    const int VC_RightInverterSpeedCommand_val = in_msg->VC_RightInverterSpeedCommand_value;
    const int32_t VC_RightInverterSpeedCommand_raw = CAN_SIGNED_ENCODE(VC_RightInverterSpeedCommand_val, CANSIG_VC_RIGHT_INVERTER_SPEED_COMMAND_SCALE, CANSIG_VC_RIGHT_INVERTER_SPEED_COMMAND_OFFSET, int32_t);
    out_data[2] |= packShiftRight(VC_RightInverterSpeedCommand_raw, 0, 0xFF);   // Packs bits ######## of byte 2 
    out_data[3] |= packShiftRight(VC_RightInverterSpeedCommand_raw, 8, 0xFF);   // Packs bits ######## of byte 3 
    
    // Pack 1-bit signal VC_RightInverterDirectionCommand into payload at [ bit 32 to bit 33 ).
    const InverterDirection VC_RightInverterDirectionCommand_val = in_msg->VC_RightInverterDirectionCommand_value;
    const uint32_t VC_RightInverterDirectionCommand_raw = CAN_ENCODE(VC_RightInverterDirectionCommand_val, CANSIG_VC_RIGHT_INVERTER_DIRECTION_COMMAND_SCALE, CANSIG_VC_RIGHT_INVERTER_DIRECTION_COMMAND_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(VC_RightInverterDirectionCommand_raw, 0, 0x1);   // Packs bits _______# of byte 4 
    
    // Pack 1-bit signal VC_RightInverterEnable into payload at [ bit 40 to bit 41 ).
    const bool VC_RightInverterEnable_val = in_msg->VC_RightInverterEnable_value;
    const uint32_t VC_RightInverterEnable_raw = CAN_ENCODE(VC_RightInverterEnable_val, CANSIG_VC_RIGHT_INVERTER_ENABLE_SCALE, CANSIG_VC_RIGHT_INVERTER_ENABLE_OFFSET, uint32_t);
    out_data[5] |= packShiftRight(VC_RightInverterEnable_raw, 0, 0x1);   // Packs bits _______# of byte 5 
    
    // Pack 1-bit signal VC_RightInverterEnableDischarge into payload at [ bit 41 to bit 42 ).
    const bool VC_RightInverterEnableDischarge_val = in_msg->VC_RightInverterEnableDischarge_value;
    const uint32_t VC_RightInverterEnableDischarge_raw = CAN_ENCODE(VC_RightInverterEnableDischarge_val, CANSIG_VC_RIGHT_INVERTER_ENABLE_DISCHARGE_SCALE, CANSIG_VC_RIGHT_INVERTER_ENABLE_DISCHARGE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(VC_RightInverterEnableDischarge_raw, 1, 0x2);   // Packs bits ______#_ of byte 5 
    
    // Pack 1-bit signal VC_RightInverterEnableSpeedMode into payload at [ bit 42 to bit 43 ).
    const bool VC_RightInverterEnableSpeedMode_val = in_msg->VC_RightInverterEnableSpeedMode_value;
    const uint32_t VC_RightInverterEnableSpeedMode_raw = CAN_ENCODE(VC_RightInverterEnableSpeedMode_val, CANSIG_VC_RIGHT_INVERTER_ENABLE_SPEED_MODE_SCALE, CANSIG_VC_RIGHT_INVERTER_ENABLE_SPEED_MODE_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(VC_RightInverterEnableSpeedMode_raw, 2, 0x4);   // Packs bits _____#__ of byte 5 
    
    // Pack 16-bit signal VC_RightInverterTorqueLimit into payload at [ bit 48 to bit 64 ).
    const float VC_RightInverterTorqueLimit_val = in_msg->VC_RightInverterTorqueLimit_value;
    const int32_t VC_RightInverterTorqueLimit_raw = CAN_SIGNED_ENCODE(VC_RightInverterTorqueLimit_val, CANSIG_VC_RIGHT_INVERTER_TORQUE_LIMIT_SCALE, CANSIG_VC_RIGHT_INVERTER_TORQUE_LIMIT_OFFSET, int32_t);
    out_data[6] |= packShiftRight(VC_RightInverterTorqueLimit_raw, 0, 0xFF);   // Packs bits ######## of byte 6 
    out_data[7] |= packShiftRight(VC_RightInverterTorqueLimit_raw, 8, 0xFF);   // Packs bits ######## of byte 7 
    
    
}
 
// pack function for VC_INVR_ReadWriteParamCommand
void app_canUtils_VC_INVR_ReadWriteParamCommand_pack(const VC_INVR_ReadWriteParamCommand_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_INVR_ReadWriteParamCommand.
    // |xxxxxxxx|xxxxxxxx|CCCCCCCC|CCCCCCCC|________|_______B|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal VC_INVR_CommandParameterAddress into payload at [ bit 0 to bit 16 ).
    const uint32_t VC_INVR_CommandParameterAddress_val = in_msg->VC_INVR_CommandParameterAddress_value;
    const uint32_t VC_INVR_CommandParameterAddress_raw = CAN_ENCODE(VC_INVR_CommandParameterAddress_val, CANSIG_VC_INVR_COMMAND_PARAMETER_ADDRESS_SCALE, CANSIG_VC_INVR_COMMAND_PARAMETER_ADDRESS_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_INVR_CommandParameterAddress_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_INVR_CommandParameterAddress_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    
    // Pack 1-bit signal VC_INVR_CommandReadWrite into payload at [ bit 16 to bit 17 ).
    const InverterReadWriteCommand VC_INVR_CommandReadWrite_val = in_msg->VC_INVR_CommandReadWrite_value;
    const uint32_t VC_INVR_CommandReadWrite_raw = CAN_ENCODE(VC_INVR_CommandReadWrite_val, CANSIG_VC_INVR_COMMAND_READ_WRITE_SCALE, CANSIG_VC_INVR_COMMAND_READ_WRITE_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(VC_INVR_CommandReadWrite_raw, 0, 0x1);   // Packs bits _______# of byte 2 
    
    // Pack 16-bit signal VC_INVR_CommandData into payload at [ bit 32 to bit 48 ).
    const uint32_t VC_INVR_CommandData_val = in_msg->VC_INVR_CommandData_value;
    const uint32_t VC_INVR_CommandData_raw = CAN_ENCODE(VC_INVR_CommandData_val, CANSIG_VC_INVR_COMMAND_DATA_SCALE, CANSIG_VC_INVR_COMMAND_DATA_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(VC_INVR_CommandData_raw, 0, 0xFF);   // Packs bits ######## of byte 4 
    out_data[5] |= packShiftRight(VC_INVR_CommandData_raw, 8, 0xFF);   // Packs bits ######## of byte 5 
    
    
}
 
// pack function for VC_AlertsContext
void app_canUtils_VC_AlertsContext_pack(const VC_AlertsContext_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_AlertsContext.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|____DDDD|DDDDDDDD|DDDDCCCC|CCCCCCCC|CCCCBBBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal VC_WatchdogTimeout into payload at [ bit 0 to bit 1 ).
    const bool VC_WatchdogTimeout_val = in_msg->VC_WatchdogTimeout_value;
    const uint32_t VC_WatchdogTimeout_raw = CAN_ENCODE(VC_WatchdogTimeout_val, CANSIG_VC_WATCHDOG_TIMEOUT_SCALE, CANSIG_VC_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_WatchdogTimeout_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 3-bit signal VC_WatchdogTimeoutTaskName into payload at [ bit 1 to bit 4 ).
    const RtosTaskName VC_WatchdogTimeoutTaskName_val = in_msg->VC_WatchdogTimeoutTaskName_value;
    const uint32_t VC_WatchdogTimeoutTaskName_raw = CAN_ENCODE(VC_WatchdogTimeoutTaskName_val, CANSIG_VC_WATCHDOG_TIMEOUT_TASK_NAME_SCALE, CANSIG_VC_WATCHDOG_TIMEOUT_TASK_NAME_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_WatchdogTimeoutTaskName_raw, 1, 0xE);   // Packs bits ____###_ of byte 0 
    
    // Pack 16-bit signal VC_TxOverflowCount into payload at [ bit 4 to bit 20 ).
    const uint32_t VC_TxOverflowCount_val = in_msg->VC_TxOverflowCount_value;
    const uint32_t VC_TxOverflowCount_raw = CAN_ENCODE(VC_TxOverflowCount_val, CANSIG_VC_TX_OVERFLOW_COUNT_SCALE, CANSIG_VC_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_TxOverflowCount_raw, 4, 0xF0);   // Packs bits ####____ of byte 0 
    out_data[1] |= packShiftRight(VC_TxOverflowCount_raw, 4, 0xFF);   // Packs bits ######## of byte 1 
    out_data[2] |= packShiftRight(VC_TxOverflowCount_raw, 12, 0xF);   // Packs bits ____#### of byte 2 
    
    // Pack 16-bit signal VC_RxOverflowCount into payload at [ bit 20 to bit 36 ).
    const uint32_t VC_RxOverflowCount_val = in_msg->VC_RxOverflowCount_value;
    const uint32_t VC_RxOverflowCount_raw = CAN_ENCODE(VC_RxOverflowCount_val, CANSIG_VC_RX_OVERFLOW_COUNT_SCALE, CANSIG_VC_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(VC_RxOverflowCount_raw, 4, 0xF0);   // Packs bits ####____ of byte 2 
    out_data[3] |= packShiftRight(VC_RxOverflowCount_raw, 4, 0xFF);   // Packs bits ######## of byte 3 
    out_data[4] |= packShiftRight(VC_RxOverflowCount_raw, 12, 0xF);   // Packs bits ____#### of byte 4 
    
    
}
 
// pack function for VC_EllipseStatus
void app_canUtils_VC_EllipseStatus_pack(const VC_EllipseStatus_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_EllipseStatus.
    // |xxxxxxxx|_____CCC|BBBBBBBB|BBBBBBBB|BBBBBBBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 16-bit signal VC_EllipseGeneralStatusBitmask into payload at [ bit 0 to bit 16 ).
    const uint32_t VC_EllipseGeneralStatusBitmask_val = in_msg->VC_EllipseGeneralStatusBitmask_value;
    const uint32_t VC_EllipseGeneralStatusBitmask_raw = CAN_ENCODE(VC_EllipseGeneralStatusBitmask_val, CANSIG_VC_ELLIPSE_GENERAL_STATUS_BITMASK_SCALE, CANSIG_VC_ELLIPSE_GENERAL_STATUS_BITMASK_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_EllipseGeneralStatusBitmask_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_EllipseGeneralStatusBitmask_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    
    // Pack 32-bit signal VC_EllipseComStatusBitmask into payload at [ bit 16 to bit 48 ).
    const uint32_t VC_EllipseComStatusBitmask_val = in_msg->VC_EllipseComStatusBitmask_value;
    const uint32_t VC_EllipseComStatusBitmask_raw = CAN_ENCODE(VC_EllipseComStatusBitmask_val, CANSIG_VC_ELLIPSE_COM_STATUS_BITMASK_SCALE, CANSIG_VC_ELLIPSE_COM_STATUS_BITMASK_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(VC_EllipseComStatusBitmask_raw, 0, 0xFF);   // Packs bits ######## of byte 2 
    out_data[3] |= packShiftRight(VC_EllipseComStatusBitmask_raw, 8, 0xFF);   // Packs bits ######## of byte 3 
    out_data[4] |= packShiftRight(VC_EllipseComStatusBitmask_raw, 16, 0xFF);   // Packs bits ######## of byte 4 
    out_data[5] |= packShiftRight(VC_EllipseComStatusBitmask_raw, 24, 0xFF);   // Packs bits ######## of byte 5 
    
    // Pack 3-bit signal VC_EkfSolutionMode into payload at [ bit 48 to bit 51 ).
    const VcEkfStatus VC_EkfSolutionMode_val = in_msg->VC_EkfSolutionMode_value;
    const uint32_t VC_EkfSolutionMode_raw = CAN_ENCODE(VC_EkfSolutionMode_val, CANSIG_VC_EKF_SOLUTION_MODE_SCALE, CANSIG_VC_EKF_SOLUTION_MODE_OFFSET, uint32_t);
    out_data[6] |= packShiftRight(VC_EkfSolutionMode_raw, 0, 0x7);   // Packs bits _____### of byte 6 
    
    
}
 
// pack function for VC_EllipseTime
void app_canUtils_VC_EllipseTime_pack(const VC_EllipseTime_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_EllipseTime.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 32-bit signal VC_EllipseTimestamp into payload at [ bit 0 to bit 32 ).
    const uint32_t VC_EllipseTimestamp_val = in_msg->VC_EllipseTimestamp_value;
    const uint32_t VC_EllipseTimestamp_raw = CAN_ENCODE(VC_EllipseTimestamp_val, CANSIG_VC_ELLIPSE_TIMESTAMP_SCALE, CANSIG_VC_ELLIPSE_TIMESTAMP_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_EllipseTimestamp_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_EllipseTimestamp_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    out_data[2] |= packShiftRight(VC_EllipseTimestamp_raw, 16, 0xFF);   // Packs bits ######## of byte 2 
    out_data[3] |= packShiftRight(VC_EllipseTimestamp_raw, 24, 0xFF);   // Packs bits ######## of byte 3 
    
    
}
 
// pack function for VC_EllipseEulerAngles
void app_canUtils_VC_EllipseEulerAngles_pack(const VC_EllipseEulerAngles_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_EllipseEulerAngles.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|____CCCC|CCCCCCCC|BBBBBBBB|BBBBAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 12-bit signal VC_EulerAnglesRoll into payload at [ bit 0 to bit 12 ).
    const float VC_EulerAnglesRoll_val = in_msg->VC_EulerAnglesRoll_value;
    const uint32_t VC_EulerAnglesRoll_raw = CAN_ENCODE(VC_EulerAnglesRoll_val, CANSIG_VC_EULER_ANGLES_ROLL_SCALE, CANSIG_VC_EULER_ANGLES_ROLL_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_EulerAnglesRoll_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_EulerAnglesRoll_raw, 8, 0xF);   // Packs bits ____#### of byte 1 
    
    // Pack 12-bit signal VC_EulerAnglesPitch into payload at [ bit 12 to bit 24 ).
    const float VC_EulerAnglesPitch_val = in_msg->VC_EulerAnglesPitch_value;
    const uint32_t VC_EulerAnglesPitch_raw = CAN_ENCODE(VC_EulerAnglesPitch_val, CANSIG_VC_EULER_ANGLES_PITCH_SCALE, CANSIG_VC_EULER_ANGLES_PITCH_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_EulerAnglesPitch_raw, 4, 0xF0);   // Packs bits ####____ of byte 1 
    out_data[2] |= packShiftRight(VC_EulerAnglesPitch_raw, 4, 0xFF);   // Packs bits ######## of byte 2 
    
    // Pack 12-bit signal VC_EulerAnglesYaw into payload at [ bit 24 to bit 36 ).
    const float VC_EulerAnglesYaw_val = in_msg->VC_EulerAnglesYaw_value;
    const uint32_t VC_EulerAnglesYaw_raw = CAN_ENCODE(VC_EulerAnglesYaw_val, CANSIG_VC_EULER_ANGLES_YAW_SCALE, CANSIG_VC_EULER_ANGLES_YAW_OFFSET, uint32_t);
    out_data[3] |= packShiftRight(VC_EulerAnglesYaw_raw, 0, 0xFF);   // Packs bits ######## of byte 3 
    out_data[4] |= packShiftRight(VC_EulerAnglesYaw_raw, 8, 0xF);   // Packs bits ____#### of byte 4 
    
    
}
 
// pack function for VC_CommitInfo
void app_canUtils_VC_CommitInfo_pack(const VC_CommitInfo_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_CommitInfo.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_______B|AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 32-bit signal VC_Hash into payload at [ bit 0 to bit 32 ).
    const uint32_t VC_Hash_val = in_msg->VC_Hash_value;
    const uint32_t VC_Hash_raw = CAN_ENCODE(VC_Hash_val, CANSIG_VC_HASH_SCALE, CANSIG_VC_HASH_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_Hash_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_Hash_raw, 8, 0xFF);   // Packs bits ######## of byte 1 
    out_data[2] |= packShiftRight(VC_Hash_raw, 16, 0xFF);   // Packs bits ######## of byte 2 
    out_data[3] |= packShiftRight(VC_Hash_raw, 24, 0xFF);   // Packs bits ######## of byte 3 
    
    // Pack 1-bit signal VC_Clean into payload at [ bit 32 to bit 33 ).
    const bool VC_Clean_val = in_msg->VC_Clean_value;
    const uint32_t VC_Clean_raw = CAN_ENCODE(VC_Clean_val, CANSIG_VC_CLEAN_SCALE, CANSIG_VC_CLEAN_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(VC_Clean_raw, 0, 0x1);   // Packs bits _______# of byte 4 
    
    
}
 
// pack function for VC_PowerEstimate
void app_canUtils_VC_PowerEstimate_pack(const VC_PowerEstimate_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_PowerEstimate.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__BBBBBB|BBBBBAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 11-bit signal VC_PowerEstimateValue into payload at [ bit 0 to bit 11 ).
    const float VC_PowerEstimateValue_val = in_msg->VC_PowerEstimateValue_value;
    const uint32_t VC_PowerEstimateValue_raw = CAN_ENCODE(VC_PowerEstimateValue_val, CANSIG_VC_POWER_ESTIMATE_VALUE_SCALE, CANSIG_VC_POWER_ESTIMATE_VALUE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_PowerEstimateValue_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_PowerEstimateValue_raw, 8, 0x7);   // Packs bits _____### of byte 1 
    
    // Pack 11-bit signal VC_PowerEstimateMeasuredValue into payload at [ bit 11 to bit 22 ).
    const float VC_PowerEstimateMeasuredValue_val = in_msg->VC_PowerEstimateMeasuredValue_value;
    const uint32_t VC_PowerEstimateMeasuredValue_raw = CAN_ENCODE(VC_PowerEstimateMeasuredValue_val, CANSIG_VC_POWER_ESTIMATE_MEASURED_VALUE_SCALE, CANSIG_VC_POWER_ESTIMATE_MEASURED_VALUE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_PowerEstimateMeasuredValue_raw, 3, 0xF8);   // Packs bits #####___ of byte 1 
    out_data[2] |= packShiftRight(VC_PowerEstimateMeasuredValue_raw, 5, 0x3F);   // Packs bits __###### of byte 2 
    
    
}
 
// pack function for VC_PowerEstimatePID
void app_canUtils_VC_PowerEstimatePID_pack(const VC_PowerEstimatePID_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_PowerEstimatePID.
    // |DDDDDDDD|DDDDDDDD|DDDDDDDD|CCCCCCCC|CCCCCCCC|BBBBBBBB|BBBBBBBB|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal VC_PIDPowerEstimateOutput into payload at [ bit 0 to bit 8 ).
    const float VC_PIDPowerEstimateOutput_val = in_msg->VC_PIDPowerEstimateOutput_value;
    const int32_t VC_PIDPowerEstimateOutput_raw = CAN_SIGNED_ENCODE(VC_PIDPowerEstimateOutput_val, CANSIG_VC_PID_POWER_ESTIMATE_OUTPUT_SCALE, CANSIG_VC_PID_POWER_ESTIMATE_OUTPUT_OFFSET, int32_t);
    out_data[0] |= packShiftRight(VC_PIDPowerEstimateOutput_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    
    // Pack 16-bit signal VC_PIDPowerEstimateError into payload at [ bit 8 to bit 24 ).
    const float VC_PIDPowerEstimateError_val = in_msg->VC_PIDPowerEstimateError_value;
    const int32_t VC_PIDPowerEstimateError_raw = CAN_SIGNED_ENCODE(VC_PIDPowerEstimateError_val, CANSIG_VC_PID_POWER_ESTIMATE_ERROR_SCALE, CANSIG_VC_PID_POWER_ESTIMATE_ERROR_OFFSET, int32_t);
    out_data[1] |= packShiftRight(VC_PIDPowerEstimateError_raw, 0, 0xFF);   // Packs bits ######## of byte 1 
    out_data[2] |= packShiftRight(VC_PIDPowerEstimateError_raw, 8, 0xFF);   // Packs bits ######## of byte 2 
    
    // Pack 16-bit signal VC_PIDPowerEstimateDerivative into payload at [ bit 24 to bit 40 ).
    const float VC_PIDPowerEstimateDerivative_val = in_msg->VC_PIDPowerEstimateDerivative_value;
    const int32_t VC_PIDPowerEstimateDerivative_raw = CAN_SIGNED_ENCODE(VC_PIDPowerEstimateDerivative_val, CANSIG_VC_PID_POWER_ESTIMATE_DERIVATIVE_SCALE, CANSIG_VC_PID_POWER_ESTIMATE_DERIVATIVE_OFFSET, int32_t);
    out_data[3] |= packShiftRight(VC_PIDPowerEstimateDerivative_raw, 0, 0xFF);   // Packs bits ######## of byte 3 
    out_data[4] |= packShiftRight(VC_PIDPowerEstimateDerivative_raw, 8, 0xFF);   // Packs bits ######## of byte 4 
    
    // Pack 24-bit signal VC_PIDPowerEstimateIntegral into payload at [ bit 40 to bit 64 ).
    const float VC_PIDPowerEstimateIntegral_val = in_msg->VC_PIDPowerEstimateIntegral_value;
    const int32_t VC_PIDPowerEstimateIntegral_raw = CAN_SIGNED_ENCODE(VC_PIDPowerEstimateIntegral_val, CANSIG_VC_PID_POWER_ESTIMATE_INTEGRAL_SCALE, CANSIG_VC_PID_POWER_ESTIMATE_INTEGRAL_OFFSET, int32_t);
    out_data[5] |= packShiftRight(VC_PIDPowerEstimateIntegral_raw, 0, 0xFF);   // Packs bits ######## of byte 5 
    out_data[6] |= packShiftRight(VC_PIDPowerEstimateIntegral_raw, 8, 0xFF);   // Packs bits ######## of byte 6 
    out_data[7] |= packShiftRight(VC_PIDPowerEstimateIntegral_raw, 16, 0xFF);   // Packs bits ######## of byte 7 
    
    
}
 
// pack function for VC_PIDSlipRatio
void app_canUtils_VC_PIDSlipRatio_pack(const VC_PIDSlipRatio_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_PIDSlipRatio.
    // |DDDDDDDD|DDDDDDDD|DDDDDDDD|CCCCCCCC|CCCCCCCC|BBBBBBBB|BBBBBBBB|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal VC_PIDSlipRatioOutput into payload at [ bit 0 to bit 8 ).
    const float VC_PIDSlipRatioOutput_val = in_msg->VC_PIDSlipRatioOutput_value;
    const int32_t VC_PIDSlipRatioOutput_raw = CAN_SIGNED_ENCODE(VC_PIDSlipRatioOutput_val, CANSIG_VC_PID_SLIP_RATIO_OUTPUT_SCALE, CANSIG_VC_PID_SLIP_RATIO_OUTPUT_OFFSET, int32_t);
    out_data[0] |= packShiftRight(VC_PIDSlipRatioOutput_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    
    // Pack 16-bit signal VC_PIDSlipRatioError into payload at [ bit 8 to bit 24 ).
    const float VC_PIDSlipRatioError_val = in_msg->VC_PIDSlipRatioError_value;
    const int32_t VC_PIDSlipRatioError_raw = CAN_SIGNED_ENCODE(VC_PIDSlipRatioError_val, CANSIG_VC_PID_SLIP_RATIO_ERROR_SCALE, CANSIG_VC_PID_SLIP_RATIO_ERROR_OFFSET, int32_t);
    out_data[1] |= packShiftRight(VC_PIDSlipRatioError_raw, 0, 0xFF);   // Packs bits ######## of byte 1 
    out_data[2] |= packShiftRight(VC_PIDSlipRatioError_raw, 8, 0xFF);   // Packs bits ######## of byte 2 
    
    // Pack 16-bit signal VC_PIDSlipRatioDerivative into payload at [ bit 24 to bit 40 ).
    const float VC_PIDSlipRatioDerivative_val = in_msg->VC_PIDSlipRatioDerivative_value;
    const int32_t VC_PIDSlipRatioDerivative_raw = CAN_SIGNED_ENCODE(VC_PIDSlipRatioDerivative_val, CANSIG_VC_PID_SLIP_RATIO_DERIVATIVE_SCALE, CANSIG_VC_PID_SLIP_RATIO_DERIVATIVE_OFFSET, int32_t);
    out_data[3] |= packShiftRight(VC_PIDSlipRatioDerivative_raw, 0, 0xFF);   // Packs bits ######## of byte 3 
    out_data[4] |= packShiftRight(VC_PIDSlipRatioDerivative_raw, 8, 0xFF);   // Packs bits ######## of byte 4 
    
    // Pack 24-bit signal VC_PIDSlipRatioIntegral into payload at [ bit 40 to bit 64 ).
    const float VC_PIDSlipRatioIntegral_val = in_msg->VC_PIDSlipRatioIntegral_value;
    const int32_t VC_PIDSlipRatioIntegral_raw = CAN_SIGNED_ENCODE(VC_PIDSlipRatioIntegral_val, CANSIG_VC_PID_SLIP_RATIO_INTEGRAL_SCALE, CANSIG_VC_PID_SLIP_RATIO_INTEGRAL_OFFSET, int32_t);
    out_data[5] |= packShiftRight(VC_PIDSlipRatioIntegral_raw, 0, 0xFF);   // Packs bits ######## of byte 5 
    out_data[6] |= packShiftRight(VC_PIDSlipRatioIntegral_raw, 8, 0xFF);   // Packs bits ######## of byte 6 
    out_data[7] |= packShiftRight(VC_PIDSlipRatioIntegral_raw, 16, 0xFF);   // Packs bits ######## of byte 7 
    
    
}
 
// pack function for VC_SlipRatios
void app_canUtils_VC_SlipRatios_pack(const VC_SlipRatios_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_SlipRatios.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|BBBBBBBB|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal VC_SlipRatioLeft into payload at [ bit 0 to bit 8 ).
    const float VC_SlipRatioLeft_val = in_msg->VC_SlipRatioLeft_value;
    const int32_t VC_SlipRatioLeft_raw = CAN_SIGNED_ENCODE(VC_SlipRatioLeft_val, CANSIG_VC_SLIP_RATIO_LEFT_SCALE, CANSIG_VC_SLIP_RATIO_LEFT_OFFSET, int32_t);
    out_data[0] |= packShiftRight(VC_SlipRatioLeft_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    
    // Pack 8-bit signal VC_SlipRatioRight into payload at [ bit 8 to bit 16 ).
    const float VC_SlipRatioRight_val = in_msg->VC_SlipRatioRight_value;
    const int32_t VC_SlipRatioRight_raw = CAN_SIGNED_ENCODE(VC_SlipRatioRight_val, CANSIG_VC_SLIP_RATIO_RIGHT_SCALE, CANSIG_VC_SLIP_RATIO_RIGHT_OFFSET, int32_t);
    out_data[1] |= packShiftRight(VC_SlipRatioRight_raw, 0, 0xFF);   // Packs bits ######## of byte 1 
    
    
}
 
// pack function for VC_ActiveDiffOutputs
void app_canUtils_VC_ActiveDiffOutputs_pack(const VC_ActiveDiffOutputs_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_ActiveDiffOutputs.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____BBBB|BBBBBBAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 10-bit signal VC_ActiveDiffTorqueLeft into payload at [ bit 0 to bit 10 ).
    const float VC_ActiveDiffTorqueLeft_val = in_msg->VC_ActiveDiffTorqueLeft_value;
    const uint32_t VC_ActiveDiffTorqueLeft_raw = CAN_ENCODE(VC_ActiveDiffTorqueLeft_val, CANSIG_VC_ACTIVE_DIFF_TORQUE_LEFT_SCALE, CANSIG_VC_ACTIVE_DIFF_TORQUE_LEFT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_ActiveDiffTorqueLeft_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_ActiveDiffTorqueLeft_raw, 8, 0x3);   // Packs bits ______## of byte 1 
    
    // Pack 10-bit signal VC_ActiveDiffTorqueRight into payload at [ bit 10 to bit 20 ).
    const float VC_ActiveDiffTorqueRight_val = in_msg->VC_ActiveDiffTorqueRight_value;
    const uint32_t VC_ActiveDiffTorqueRight_raw = CAN_ENCODE(VC_ActiveDiffTorqueRight_val, CANSIG_VC_ACTIVE_DIFF_TORQUE_RIGHT_SCALE, CANSIG_VC_ACTIVE_DIFF_TORQUE_RIGHT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_ActiveDiffTorqueRight_raw, 2, 0xFC);   // Packs bits ######__ of byte 1 
    out_data[2] |= packShiftRight(VC_ActiveDiffTorqueRight_raw, 6, 0xF);   // Packs bits ____#### of byte 2 
    
    
}
 
// pack function for VC_PowerLimit
void app_canUtils_VC_PowerLimit_pack(const VC_PowerLimit_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_PowerLimit.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__BBBBBB|BBBBBAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 11-bit signal VC_PowerLimitValue into payload at [ bit 0 to bit 11 ).
    const float VC_PowerLimitValue_val = in_msg->VC_PowerLimitValue_value;
    const uint32_t VC_PowerLimitValue_raw = CAN_ENCODE(VC_PowerLimitValue_val, CANSIG_VC_POWER_LIMIT_VALUE_SCALE, CANSIG_VC_POWER_LIMIT_VALUE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_PowerLimitValue_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_PowerLimitValue_raw, 8, 0x7);   // Packs bits _____### of byte 1 
    
    // Pack 11-bit signal VC_PowerLimitValueAtFullThrottle into payload at [ bit 11 to bit 22 ).
    const float VC_PowerLimitValueAtFullThrottle_val = in_msg->VC_PowerLimitValueAtFullThrottle_value;
    const uint32_t VC_PowerLimitValueAtFullThrottle_raw = CAN_ENCODE(VC_PowerLimitValueAtFullThrottle_val, CANSIG_VC_POWER_LIMIT_VALUE_AT_FULL_THROTTLE_SCALE, CANSIG_VC_POWER_LIMIT_VALUE_AT_FULL_THROTTLE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_PowerLimitValueAtFullThrottle_raw, 3, 0xF8);   // Packs bits #####___ of byte 1 
    out_data[2] |= packShiftRight(VC_PowerLimitValueAtFullThrottle_raw, 5, 0x3F);   // Packs bits __###### of byte 2 
    
    
}
 
// pack function for VC_VehicleDynamics
void app_canUtils_VC_VehicleDynamics_pack(const VC_VehicleDynamics_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_VehicleDynamics.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_______C|CCCCCCCC|CCCCCCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal VC_RegenEnabled into payload at [ bit 0 to bit 1 ).
    const bool VC_RegenEnabled_val = in_msg->VC_RegenEnabled_value;
    const uint32_t VC_RegenEnabled_raw = CAN_ENCODE(VC_RegenEnabled_val, CANSIG_VC_REGEN_ENABLED_SCALE, CANSIG_VC_REGEN_ENABLED_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_RegenEnabled_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 1-bit signal VC_TorqueVectoringEnabled into payload at [ bit 1 to bit 2 ).
    const bool VC_TorqueVectoringEnabled_val = in_msg->VC_TorqueVectoringEnabled_value;
    const uint32_t VC_TorqueVectoringEnabled_raw = CAN_ENCODE(VC_TorqueVectoringEnabled_val, CANSIG_VC_TORQUE_VECTORING_ENABLED_SCALE, CANSIG_VC_TORQUE_VECTORING_ENABLED_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_TorqueVectoringEnabled_raw, 1, 0x2);   // Packs bits ______#_ of byte 0 
    
    // Pack 15-bit signal VC_VehicleVelocity into payload at [ bit 2 to bit 17 ).
    const float VC_VehicleVelocity_val = in_msg->VC_VehicleVelocity_value;
    const uint32_t VC_VehicleVelocity_raw = CAN_ENCODE(VC_VehicleVelocity_val, CANSIG_VC_VEHICLE_VELOCITY_SCALE, CANSIG_VC_VEHICLE_VELOCITY_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_VehicleVelocity_raw, 2, 0xFC);   // Packs bits ######__ of byte 0 
    out_data[1] |= packShiftRight(VC_VehicleVelocity_raw, 6, 0xFF);   // Packs bits ######## of byte 1 
    out_data[2] |= packShiftRight(VC_VehicleVelocity_raw, 14, 0x1);   // Packs bits _______# of byte 2 
    
    
}
 
// pack function for VC_EllipseEkfNavVelocity
void app_canUtils_VC_EllipseEkfNavVelocity_pack(const VC_EllipseEkfNavVelocity_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_EllipseEkfNavVelocity.
    // |____FFFF|FFFFFEEE|EEEEEEEE|DDDDDDDD|DCCCCCCC|CCCCBBBB|BBBBBAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 11-bit signal VC_VelocityNorth into payload at [ bit 0 to bit 11 ).
    const float VC_VelocityNorth_val = in_msg->VC_VelocityNorth_value;
    const uint32_t VC_VelocityNorth_raw = CAN_ENCODE(VC_VelocityNorth_val, CANSIG_VC_VELOCITY_NORTH_SCALE, CANSIG_VC_VELOCITY_NORTH_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_VelocityNorth_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_VelocityNorth_raw, 8, 0x7);   // Packs bits _____### of byte 1 
    
    // Pack 9-bit signal VC_VelocityNorthAccuracy into payload at [ bit 11 to bit 20 ).
    const float VC_VelocityNorthAccuracy_val = in_msg->VC_VelocityNorthAccuracy_value;
    const uint32_t VC_VelocityNorthAccuracy_raw = CAN_ENCODE(VC_VelocityNorthAccuracy_val, CANSIG_VC_VELOCITY_NORTH_ACCURACY_SCALE, CANSIG_VC_VELOCITY_NORTH_ACCURACY_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_VelocityNorthAccuracy_raw, 3, 0xF8);   // Packs bits #####___ of byte 1 
    out_data[2] |= packShiftRight(VC_VelocityNorthAccuracy_raw, 5, 0xF);   // Packs bits ____#### of byte 2 
    
    // Pack 11-bit signal VC_VelocityEast into payload at [ bit 20 to bit 31 ).
    const float VC_VelocityEast_val = in_msg->VC_VelocityEast_value;
    const uint32_t VC_VelocityEast_raw = CAN_ENCODE(VC_VelocityEast_val, CANSIG_VC_VELOCITY_EAST_SCALE, CANSIG_VC_VELOCITY_EAST_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(VC_VelocityEast_raw, 4, 0xF0);   // Packs bits ####____ of byte 2 
    out_data[3] |= packShiftRight(VC_VelocityEast_raw, 4, 0x7F);   // Packs bits _####### of byte 3 
    
    // Pack 9-bit signal VC_VelocityEastAccuracy into payload at [ bit 31 to bit 40 ).
    const float VC_VelocityEastAccuracy_val = in_msg->VC_VelocityEastAccuracy_value;
    const uint32_t VC_VelocityEastAccuracy_raw = CAN_ENCODE(VC_VelocityEastAccuracy_val, CANSIG_VC_VELOCITY_EAST_ACCURACY_SCALE, CANSIG_VC_VELOCITY_EAST_ACCURACY_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(VC_VelocityEastAccuracy_raw, 7, 0x80);   // Packs bits #_______ of byte 3 
    out_data[4] |= packShiftRight(VC_VelocityEastAccuracy_raw, 1, 0xFF);   // Packs bits ######## of byte 4 
    
    // Pack 11-bit signal VC_VelocityDown into payload at [ bit 40 to bit 51 ).
    const float VC_VelocityDown_val = in_msg->VC_VelocityDown_value;
    const uint32_t VC_VelocityDown_raw = CAN_ENCODE(VC_VelocityDown_val, CANSIG_VC_VELOCITY_DOWN_SCALE, CANSIG_VC_VELOCITY_DOWN_OFFSET, uint32_t);
    out_data[5] |= packShiftRight(VC_VelocityDown_raw, 0, 0xFF);   // Packs bits ######## of byte 5 
    out_data[6] |= packShiftRight(VC_VelocityDown_raw, 8, 0x7);   // Packs bits _____### of byte 6 
    
    // Pack 9-bit signal VC_VelocityDownAccuracy into payload at [ bit 51 to bit 60 ).
    const float VC_VelocityDownAccuracy_val = in_msg->VC_VelocityDownAccuracy_value;
    const uint32_t VC_VelocityDownAccuracy_raw = CAN_ENCODE(VC_VelocityDownAccuracy_val, CANSIG_VC_VELOCITY_DOWN_ACCURACY_SCALE, CANSIG_VC_VELOCITY_DOWN_ACCURACY_OFFSET, uint32_t);
    out_data[6] |= packShiftLeft(VC_VelocityDownAccuracy_raw, 3, 0xF8);   // Packs bits #####___ of byte 6 
    out_data[7] |= packShiftRight(VC_VelocityDownAccuracy_raw, 5, 0xF);   // Packs bits ____#### of byte 7 
    
    
}
 
// pack function for VC_Voltages
void app_canUtils_VC_Voltages_pack(const VC_Voltages_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_Voltages.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|______CC|CCCCCCCC|CCBBBBBB|BBBBBBAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 10-bit signal VC_BatVoltage into payload at [ bit 0 to bit 10 ).
    const float VC_BatVoltage_val = in_msg->VC_BatVoltage_value;
    const uint32_t VC_BatVoltage_raw = CAN_ENCODE(VC_BatVoltage_val, CANSIG_VC_BAT_VOLTAGE_SCALE, CANSIG_VC_BAT_VOLTAGE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_BatVoltage_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_BatVoltage_raw, 8, 0x3);   // Packs bits ______## of byte 1 
    
    // Pack 12-bit signal VC_BoostVoltage into payload at [ bit 10 to bit 22 ).
    const float VC_BoostVoltage_val = in_msg->VC_BoostVoltage_value;
    const uint32_t VC_BoostVoltage_raw = CAN_ENCODE(VC_BoostVoltage_val, CANSIG_VC_BOOST_VOLTAGE_SCALE, CANSIG_VC_BOOST_VOLTAGE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_BoostVoltage_raw, 2, 0xFC);   // Packs bits ######__ of byte 1 
    out_data[2] |= packShiftRight(VC_BoostVoltage_raw, 6, 0x3F);   // Packs bits __###### of byte 2 
    
    // Pack 12-bit signal VC_AccVoltage into payload at [ bit 22 to bit 34 ).
    const float VC_AccVoltage_val = in_msg->VC_AccVoltage_value;
    const uint32_t VC_AccVoltage_raw = CAN_ENCODE(VC_AccVoltage_val, CANSIG_VC_ACC_VOLTAGE_SCALE, CANSIG_VC_ACC_VOLTAGE_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(VC_AccVoltage_raw, 6, 0xC0);   // Packs bits ##______ of byte 2 
    out_data[3] |= packShiftRight(VC_AccVoltage_raw, 2, 0xFF);   // Packs bits ######## of byte 3 
    out_data[4] |= packShiftRight(VC_AccVoltage_raw, 10, 0x3);   // Packs bits ______## of byte 4 
    
    
}
 
// pack function for VC_CurrentSensing
void app_canUtils_VC_CurrentSensing_pack(const VC_CurrentSensing_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_CurrentSensing.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|BBBBBBBB|BBBBBAAA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 11-bit signal VC_AccCurrent into payload at [ bit 0 to bit 11 ).
    const float VC_AccCurrent_val = in_msg->VC_AccCurrent_value;
    const uint32_t VC_AccCurrent_raw = CAN_ENCODE(VC_AccCurrent_val, CANSIG_VC_ACC_CURRENT_SCALE, CANSIG_VC_ACC_CURRENT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_AccCurrent_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_AccCurrent_raw, 8, 0x7);   // Packs bits _____### of byte 1 
    
    // Pack 13-bit signal VC_BatCurrent into payload at [ bit 11 to bit 24 ).
    const float VC_BatCurrent_val = in_msg->VC_BatCurrent_value;
    const uint32_t VC_BatCurrent_raw = CAN_ENCODE(VC_BatCurrent_val, CANSIG_VC_BAT_CURRENT_SCALE, CANSIG_VC_BAT_CURRENT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_BatCurrent_raw, 3, 0xF8);   // Packs bits #####___ of byte 1 
    out_data[2] |= packShiftRight(VC_BatCurrent_raw, 5, 0xFF);   // Packs bits ######## of byte 2 
    
    
}
 
// pack function for VC_Efuse
void app_canUtils_VC_Efuse_pack(const VC_Efuse_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_Efuse.
    // |__LKKKKK|KKKKJIII|IIIIIIHG|GGGGGGGG|FEEEEEEE|EEDCCCCC|CCCCCCBA|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 9-bit signal VC_ShdnCurrent into payload at [ bit 0 to bit 9 ).
    const float VC_ShdnCurrent_val = in_msg->VC_ShdnCurrent_value;
    const uint32_t VC_ShdnCurrent_raw = CAN_ENCODE(VC_ShdnCurrent_val, CANSIG_VC_SHDN_CURRENT_SCALE, CANSIG_VC_SHDN_CURRENT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_ShdnCurrent_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    out_data[1] |= packShiftRight(VC_ShdnCurrent_raw, 8, 0x1);   // Packs bits _______# of byte 1 
    
    // Pack 1-bit signal VC_ShdnStatus into payload at [ bit 9 to bit 10 ).
    const bool VC_ShdnStatus_val = in_msg->VC_ShdnStatus_value;
    const uint32_t VC_ShdnStatus_raw = CAN_ENCODE(VC_ShdnStatus_val, CANSIG_VC_SHDN_STATUS_SCALE, CANSIG_VC_SHDN_STATUS_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_ShdnStatus_raw, 1, 0x2);   // Packs bits ______#_ of byte 1 
    
    // Pack 11-bit signal VC_LvCurrent into payload at [ bit 10 to bit 21 ).
    const float VC_LvCurrent_val = in_msg->VC_LvCurrent_value;
    const uint32_t VC_LvCurrent_raw = CAN_ENCODE(VC_LvCurrent_val, CANSIG_VC_LV_CURRENT_SCALE, CANSIG_VC_LV_CURRENT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_LvCurrent_raw, 2, 0xFC);   // Packs bits ######__ of byte 1 
    out_data[2] |= packShiftRight(VC_LvCurrent_raw, 6, 0x1F);   // Packs bits ___##### of byte 2 
    
    // Pack 1-bit signal VC_LvStatus into payload at [ bit 21 to bit 22 ).
    const bool VC_LvStatus_val = in_msg->VC_LvStatus_value;
    const uint32_t VC_LvStatus_raw = CAN_ENCODE(VC_LvStatus_val, CANSIG_VC_LV_STATUS_SCALE, CANSIG_VC_LV_STATUS_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(VC_LvStatus_raw, 5, 0x20);   // Packs bits __#_____ of byte 2 
    
    // Pack 9-bit signal VC_PumpCurrent into payload at [ bit 22 to bit 31 ).
    const float VC_PumpCurrent_val = in_msg->VC_PumpCurrent_value;
    const uint32_t VC_PumpCurrent_raw = CAN_ENCODE(VC_PumpCurrent_val, CANSIG_VC_PUMP_CURRENT_SCALE, CANSIG_VC_PUMP_CURRENT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(VC_PumpCurrent_raw, 6, 0xC0);   // Packs bits ##______ of byte 2 
    out_data[3] |= packShiftRight(VC_PumpCurrent_raw, 2, 0x7F);   // Packs bits _####### of byte 3 
    
    // Pack 1-bit signal VC_PumpStatus into payload at [ bit 31 to bit 32 ).
    const bool VC_PumpStatus_val = in_msg->VC_PumpStatus_value;
    const uint32_t VC_PumpStatus_raw = CAN_ENCODE(VC_PumpStatus_val, CANSIG_VC_PUMP_STATUS_SCALE, CANSIG_VC_PUMP_STATUS_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(VC_PumpStatus_raw, 7, 0x80);   // Packs bits #_______ of byte 3 
    
    // Pack 9-bit signal VC_AuxCurrent into payload at [ bit 32 to bit 41 ).
    const float VC_AuxCurrent_val = in_msg->VC_AuxCurrent_value;
    const uint32_t VC_AuxCurrent_raw = CAN_ENCODE(VC_AuxCurrent_val, CANSIG_VC_AUX_CURRENT_SCALE, CANSIG_VC_AUX_CURRENT_OFFSET, uint32_t);
    out_data[4] |= packShiftRight(VC_AuxCurrent_raw, 0, 0xFF);   // Packs bits ######## of byte 4 
    out_data[5] |= packShiftRight(VC_AuxCurrent_raw, 8, 0x1);   // Packs bits _______# of byte 5 
    
    // Pack 1-bit signal VC_AuxStatus into payload at [ bit 41 to bit 42 ).
    const bool VC_AuxStatus_val = in_msg->VC_AuxStatus_value;
    const uint32_t VC_AuxStatus_raw = CAN_ENCODE(VC_AuxStatus_val, CANSIG_VC_AUX_STATUS_SCALE, CANSIG_VC_AUX_STATUS_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(VC_AuxStatus_raw, 1, 0x2);   // Packs bits ______#_ of byte 5 
    
    // Pack 9-bit signal VC_InvRCurrent into payload at [ bit 42 to bit 51 ).
    const float VC_InvRCurrent_val = in_msg->VC_InvRCurrent_value;
    const uint32_t VC_InvRCurrent_raw = CAN_ENCODE(VC_InvRCurrent_val, CANSIG_VC_INV_R_CURRENT_SCALE, CANSIG_VC_INV_R_CURRENT_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(VC_InvRCurrent_raw, 2, 0xFC);   // Packs bits ######__ of byte 5 
    out_data[6] |= packShiftRight(VC_InvRCurrent_raw, 6, 0x7);   // Packs bits _____### of byte 6 
    
    // Pack 1-bit signal VC_InvRStatus into payload at [ bit 51 to bit 52 ).
    const bool VC_InvRStatus_val = in_msg->VC_InvRStatus_value;
    const uint32_t VC_InvRStatus_raw = CAN_ENCODE(VC_InvRStatus_val, CANSIG_VC_INV_R_STATUS_SCALE, CANSIG_VC_INV_R_STATUS_OFFSET, uint32_t);
    out_data[6] |= packShiftLeft(VC_InvRStatus_raw, 3, 0x8);   // Packs bits ____#___ of byte 6 
    
    // Pack 9-bit signal VC_InvLCurrent into payload at [ bit 52 to bit 61 ).
    const float VC_InvLCurrent_val = in_msg->VC_InvLCurrent_value;
    const uint32_t VC_InvLCurrent_raw = CAN_ENCODE(VC_InvLCurrent_val, CANSIG_VC_INV_L_CURRENT_SCALE, CANSIG_VC_INV_L_CURRENT_OFFSET, uint32_t);
    out_data[6] |= packShiftLeft(VC_InvLCurrent_raw, 4, 0xF0);   // Packs bits ####____ of byte 6 
    out_data[7] |= packShiftRight(VC_InvLCurrent_raw, 4, 0x1F);   // Packs bits ___##### of byte 7 
    
    // Pack 1-bit signal VC_InvLStatus into payload at [ bit 61 to bit 62 ).
    const bool VC_InvLStatus_val = in_msg->VC_InvLStatus_value;
    const uint32_t VC_InvLStatus_raw = CAN_ENCODE(VC_InvLStatus_val, CANSIG_VC_INV_L_STATUS_SCALE, CANSIG_VC_INV_L_STATUS_OFFSET, uint32_t);
    out_data[7] |= packShiftLeft(VC_InvLStatus_raw, 5, 0x20);   // Packs bits __#_____ of byte 7 
    
    
}
 
// pack function for VC_ImuLinearAcceleration
void app_canUtils_VC_ImuLinearAcceleration_pack(const VC_ImuLinearAcceleration_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_ImuLinearAcceleration.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|CCCCCCCC|BBBBBBBB|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal VC_ImuAccelerationX into payload at [ bit 0 to bit 8 ).
    const float VC_ImuAccelerationX_val = in_msg->VC_ImuAccelerationX_value;
    const uint32_t VC_ImuAccelerationX_raw = CAN_ENCODE(VC_ImuAccelerationX_val, CANSIG_VC_IMU_ACCELERATION_X_SCALE, CANSIG_VC_IMU_ACCELERATION_X_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_ImuAccelerationX_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    
    // Pack 8-bit signal VC_ImuAccelerationY into payload at [ bit 8 to bit 16 ).
    const float VC_ImuAccelerationY_val = in_msg->VC_ImuAccelerationY_value;
    const uint32_t VC_ImuAccelerationY_raw = CAN_ENCODE(VC_ImuAccelerationY_val, CANSIG_VC_IMU_ACCELERATION_Y_SCALE, CANSIG_VC_IMU_ACCELERATION_Y_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(VC_ImuAccelerationY_raw, 0, 0xFF);   // Packs bits ######## of byte 1 
    
    // Pack 8-bit signal VC_ImuAccelerationZ into payload at [ bit 16 to bit 24 ).
    const float VC_ImuAccelerationZ_val = in_msg->VC_ImuAccelerationZ_value;
    const uint32_t VC_ImuAccelerationZ_raw = CAN_ENCODE(VC_ImuAccelerationZ_val, CANSIG_VC_IMU_ACCELERATION_Z_SCALE, CANSIG_VC_IMU_ACCELERATION_Z_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(VC_ImuAccelerationZ_raw, 0, 0xFF);   // Packs bits ######## of byte 2 
    
    
}
 
// pack function for VC_CoolantPumpFlowRate
void app_canUtils_VC_CoolantPumpFlowRate_pack(const VC_CoolantPumpFlowRate_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_CoolantPumpFlowRate.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal VC_FlowRate into payload at [ bit 0 to bit 8 ).
    const float VC_FlowRate_val = in_msg->VC_FlowRate_value;
    const uint32_t VC_FlowRate_raw = CAN_ENCODE(VC_FlowRate_val, CANSIG_VC_FLOW_RATE_SCALE, CANSIG_VC_FLOW_RATE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_FlowRate_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    
    
}
 
// pack function for VC_VCShdnNodeStatus
void app_canUtils_VC_VCShdnNodeStatus_pack(const VC_VCShdnNodeStatus_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_VCShdnNodeStatus.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____DCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal VC_LEStopOKStatus into payload at [ bit 0 to bit 1 ).
    const bool VC_LEStopOKStatus_val = in_msg->VC_LEStopOKStatus_value;
    const uint32_t VC_LEStopOKStatus_raw = CAN_ENCODE(VC_LEStopOKStatus_val, CANSIG_VC_LE_STOP_OK_STATUS_SCALE, CANSIG_VC_LE_STOP_OK_STATUS_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_LEStopOKStatus_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 1-bit signal VC_REStopOKStatus into payload at [ bit 1 to bit 2 ).
    const bool VC_REStopOKStatus_val = in_msg->VC_REStopOKStatus_value;
    const uint32_t VC_REStopOKStatus_raw = CAN_ENCODE(VC_REStopOKStatus_val, CANSIG_VC_RE_STOP_OK_STATUS_SCALE, CANSIG_VC_RE_STOP_OK_STATUS_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_REStopOKStatus_raw, 1, 0x2);   // Packs bits ______#_ of byte 0 
    
    // Pack 1-bit signal VC_TSMSOKStatus into payload at [ bit 2 to bit 3 ).
    const bool VC_TSMSOKStatus_val = in_msg->VC_TSMSOKStatus_value;
    const uint32_t VC_TSMSOKStatus_raw = CAN_ENCODE(VC_TSMSOKStatus_val, CANSIG_VC_TSMSOK_STATUS_SCALE, CANSIG_VC_TSMSOK_STATUS_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_TSMSOKStatus_raw, 2, 0x4);   // Packs bits _____#__ of byte 0 
    
    // Pack 1-bit signal VC_SplitterBoxInterlockOKStatus into payload at [ bit 3 to bit 4 ).
    const bool VC_SplitterBoxInterlockOKStatus_val = in_msg->VC_SplitterBoxInterlockOKStatus_value;
    const uint32_t VC_SplitterBoxInterlockOKStatus_raw = CAN_ENCODE(VC_SplitterBoxInterlockOKStatus_val, CANSIG_VC_SPLITTER_BOX_INTERLOCK_OK_STATUS_SCALE, CANSIG_VC_SPLITTER_BOX_INTERLOCK_OK_STATUS_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_SplitterBoxInterlockOKStatus_raw, 3, 0x8);   // Packs bits ____#___ of byte 0 
    
    
}
 
// pack function for VC_GlobalShdnNodeStatus
void app_canUtils_VC_GlobalShdnNodeStatus_pack(const VC_GlobalShdnNodeStatus_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_GlobalShdnNodeStatus.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____AAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 4-bit signal VC_FirstFaultNode into payload at [ bit 0 to bit 4 ).
    const ShutdownNode VC_FirstFaultNode_val = in_msg->VC_FirstFaultNode_value;
    const uint32_t VC_FirstFaultNode_raw = CAN_ENCODE(VC_FirstFaultNode_val, CANSIG_VC_FIRST_FAULT_NODE_SCALE, CANSIG_VC_FIRST_FAULT_NODE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_FirstFaultNode_raw, 0, 0xF);   // Packs bits ____#### of byte 0 
    
    
}
 
// pack function for VC_PedalPercentage
void app_canUtils_VC_PedalPercentage_pack(const VC_PedalPercentage_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_PedalPercentage.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|AAAAAAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 8-bit signal VC_MappedPedalPercentage into payload at [ bit 0 to bit 8 ).
    const float VC_MappedPedalPercentage_val = in_msg->VC_MappedPedalPercentage_value;
    const uint32_t VC_MappedPedalPercentage_raw = CAN_ENCODE(VC_MappedPedalPercentage_val, CANSIG_VC_MAPPED_PEDAL_PERCENTAGE_SCALE, CANSIG_VC_MAPPED_PEDAL_PERCENTAGE_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_MappedPedalPercentage_raw, 0, 0xFF);   // Packs bits ######## of byte 0 
    
    
}
 
// pack function for VC_Warnings
void app_canUtils_VC_Warnings_pack(const VC_Warnings_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____TSRQ|PONMLKJI|HGFEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal VC_Warning_StackWaterMarkHighTask1Hz into payload at [ bit 0 to bit 1 ).
    const bool VC_Warning_StackWaterMarkHighTask1Hz_val = in_msg->VC_Warning_StackWaterMarkHighTask1Hz_value;
    const uint32_t VC_Warning_StackWaterMarkHighTask1Hz_raw = CAN_ENCODE(VC_Warning_StackWaterMarkHighTask1Hz_val, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_Warning_StackWaterMarkHighTask1Hz_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 1-bit signal VC_Warning_StackWaterMarkHighTask100Hz into payload at [ bit 1 to bit 2 ).
    const bool VC_Warning_StackWaterMarkHighTask100Hz_val = in_msg->VC_Warning_StackWaterMarkHighTask100Hz_value;
    const uint32_t VC_Warning_StackWaterMarkHighTask100Hz_raw = CAN_ENCODE(VC_Warning_StackWaterMarkHighTask100Hz_val, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Warning_StackWaterMarkHighTask100Hz_raw, 1, 0x2);   // Packs bits ______#_ of byte 0 
    
    // Pack 1-bit signal VC_Warning_StackWaterMarkHighTask1kHz into payload at [ bit 2 to bit 3 ).
    const bool VC_Warning_StackWaterMarkHighTask1kHz_val = in_msg->VC_Warning_StackWaterMarkHighTask1kHz_value;
    const uint32_t VC_Warning_StackWaterMarkHighTask1kHz_raw = CAN_ENCODE(VC_Warning_StackWaterMarkHighTask1kHz_val, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Warning_StackWaterMarkHighTask1kHz_raw, 2, 0x4);   // Packs bits _____#__ of byte 0 
    
    // Pack 1-bit signal VC_Warning_StackWaterMarkHighTaskCanRx into payload at [ bit 3 to bit 4 ).
    const bool VC_Warning_StackWaterMarkHighTaskCanRx_val = in_msg->VC_Warning_StackWaterMarkHighTaskCanRx_value;
    const uint32_t VC_Warning_StackWaterMarkHighTaskCanRx_raw = CAN_ENCODE(VC_Warning_StackWaterMarkHighTaskCanRx_val, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Warning_StackWaterMarkHighTaskCanRx_raw, 3, 0x8);   // Packs bits ____#___ of byte 0 
    
    // Pack 1-bit signal VC_Warning_StackWaterMarkHighTaskCanTx into payload at [ bit 4 to bit 5 ).
    const bool VC_Warning_StackWaterMarkHighTaskCanTx_val = in_msg->VC_Warning_StackWaterMarkHighTaskCanTx_value;
    const uint32_t VC_Warning_StackWaterMarkHighTaskCanTx_raw = CAN_ENCODE(VC_Warning_StackWaterMarkHighTaskCanTx_val, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Warning_StackWaterMarkHighTaskCanTx_raw, 4, 0x10);   // Packs bits ___#____ of byte 0 
    
    // Pack 1-bit signal VC_Warning_WatchdogTimeout into payload at [ bit 5 to bit 6 ).
    const bool VC_Warning_WatchdogTimeout_val = in_msg->VC_Warning_WatchdogTimeout_value;
    const uint32_t VC_Warning_WatchdogTimeout_raw = CAN_ENCODE(VC_Warning_WatchdogTimeout_val, CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Warning_WatchdogTimeout_raw, 5, 0x20);   // Packs bits __#_____ of byte 0 
    
    // Pack 1-bit signal VC_Warning_TxOverflow into payload at [ bit 6 to bit 7 ).
    const bool VC_Warning_TxOverflow_val = in_msg->VC_Warning_TxOverflow_value;
    const uint32_t VC_Warning_TxOverflow_raw = CAN_ENCODE(VC_Warning_TxOverflow_val, CANSIG_VC_WARNING_TX_OVERFLOW_SCALE, CANSIG_VC_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Warning_TxOverflow_raw, 6, 0x40);   // Packs bits _#______ of byte 0 
    
    // Pack 1-bit signal VC_Warning_RxOverflow into payload at [ bit 7 to bit 8 ).
    const bool VC_Warning_RxOverflow_val = in_msg->VC_Warning_RxOverflow_value;
    const uint32_t VC_Warning_RxOverflow_raw = CAN_ENCODE(VC_Warning_RxOverflow_val, CANSIG_VC_WARNING_RX_OVERFLOW_SCALE, CANSIG_VC_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Warning_RxOverflow_raw, 7, 0x80);   // Packs bits #_______ of byte 0 
    
    // Pack 1-bit signal VC_Warning_RegenNotAvailable into payload at [ bit 8 to bit 9 ).
    const bool VC_Warning_RegenNotAvailable_val = in_msg->VC_Warning_RegenNotAvailable_value;
    const uint32_t VC_Warning_RegenNotAvailable_raw = CAN_ENCODE(VC_Warning_RegenNotAvailable_val, CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_SCALE, CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_OFFSET, uint32_t);
    out_data[1] |= packShiftRight(VC_Warning_RegenNotAvailable_raw, 0, 0x1);   // Packs bits _______# of byte 1 
    
    // Pack 1-bit signal VC_Warning_ImuInitFailed into payload at [ bit 9 to bit 10 ).
    const bool VC_Warning_ImuInitFailed_val = in_msg->VC_Warning_ImuInitFailed_value;
    const uint32_t VC_Warning_ImuInitFailed_raw = CAN_ENCODE(VC_Warning_ImuInitFailed_val, CANSIG_VC_WARNING_IMU_INIT_FAILED_SCALE, CANSIG_VC_WARNING_IMU_INIT_FAILED_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Warning_ImuInitFailed_raw, 1, 0x2);   // Packs bits ______#_ of byte 1 
    
    // Pack 1-bit signal VC_Warning_FlowRateInputOutOfRange into payload at [ bit 10 to bit 11 ).
    const bool VC_Warning_FlowRateInputOutOfRange_val = in_msg->VC_Warning_FlowRateInputOutOfRange_value;
    const uint32_t VC_Warning_FlowRateInputOutOfRange_raw = CAN_ENCODE(VC_Warning_FlowRateInputOutOfRange_val, CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_SCALE, CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Warning_FlowRateInputOutOfRange_raw, 2, 0x4);   // Packs bits _____#__ of byte 1 
    
    // Pack 1-bit signal VC_Warning_BatteryRailOvercurrentFault into payload at [ bit 11 to bit 12 ).
    const bool VC_Warning_BatteryRailOvercurrentFault_val = in_msg->VC_Warning_BatteryRailOvercurrentFault_value;
    const uint32_t VC_Warning_BatteryRailOvercurrentFault_raw = CAN_ENCODE(VC_Warning_BatteryRailOvercurrentFault_val, CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_SCALE, CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Warning_BatteryRailOvercurrentFault_raw, 3, 0x8);   // Packs bits ____#___ of byte 1 
    
    // Pack 1-bit signal VC_Warning_AccumulatorRailOvercurrentFault into payload at [ bit 12 to bit 13 ).
    const bool VC_Warning_AccumulatorRailOvercurrentFault_val = in_msg->VC_Warning_AccumulatorRailOvercurrentFault_value;
    const uint32_t VC_Warning_AccumulatorRailOvercurrentFault_raw = CAN_ENCODE(VC_Warning_AccumulatorRailOvercurrentFault_val, CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_SCALE, CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Warning_AccumulatorRailOvercurrentFault_raw, 4, 0x10);   // Packs bits ___#____ of byte 1 
    
    // Pack 1-bit signal VC_Warning_LvChargeFault into payload at [ bit 13 to bit 14 ).
    const bool VC_Warning_LvChargeFault_val = in_msg->VC_Warning_LvChargeFault_value;
    const uint32_t VC_Warning_LvChargeFault_raw = CAN_ENCODE(VC_Warning_LvChargeFault_val, CANSIG_VC_WARNING_LV_CHARGE_FAULT_SCALE, CANSIG_VC_WARNING_LV_CHARGE_FAULT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Warning_LvChargeFault_raw, 5, 0x20);   // Packs bits __#_____ of byte 1 
    
    // Pack 1-bit signal VC_Warning_BoostControllerFault into payload at [ bit 14 to bit 15 ).
    const bool VC_Warning_BoostControllerFault_val = in_msg->VC_Warning_BoostControllerFault_value;
    const uint32_t VC_Warning_BoostControllerFault_raw = CAN_ENCODE(VC_Warning_BoostControllerFault_val, CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_SCALE, CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Warning_BoostControllerFault_raw, 6, 0x40);   // Packs bits _#______ of byte 1 
    
    // Pack 1-bit signal VC_Warning_HighNumberOfCanDataLogs into payload at [ bit 15 to bit 16 ).
    const bool VC_Warning_HighNumberOfCanDataLogs_val = in_msg->VC_Warning_HighNumberOfCanDataLogs_value;
    const uint32_t VC_Warning_HighNumberOfCanDataLogs_raw = CAN_ENCODE(VC_Warning_HighNumberOfCanDataLogs_val, CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_SCALE, CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Warning_HighNumberOfCanDataLogs_raw, 7, 0x80);   // Packs bits #_______ of byte 1 
    
    // Pack 1-bit signal VC_Warning_CanLoggingSdCardNotPresent into payload at [ bit 16 to bit 17 ).
    const bool VC_Warning_CanLoggingSdCardNotPresent_val = in_msg->VC_Warning_CanLoggingSdCardNotPresent_value;
    const uint32_t VC_Warning_CanLoggingSdCardNotPresent_raw = CAN_ENCODE(VC_Warning_CanLoggingSdCardNotPresent_val, CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_SCALE, CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_OFFSET, uint32_t);
    out_data[2] |= packShiftRight(VC_Warning_CanLoggingSdCardNotPresent_raw, 0, 0x1);   // Packs bits _______# of byte 2 
    
    // Pack 1-bit signal VC_Warning_CanLoggingErrored into payload at [ bit 17 to bit 18 ).
    const bool VC_Warning_CanLoggingErrored_val = in_msg->VC_Warning_CanLoggingErrored_value;
    const uint32_t VC_Warning_CanLoggingErrored_raw = CAN_ENCODE(VC_Warning_CanLoggingErrored_val, CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_SCALE, CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(VC_Warning_CanLoggingErrored_raw, 1, 0x2);   // Packs bits ______#_ of byte 2 
    
    // Pack 1-bit signal VC_Warning_BrakeAppsDisagreement into payload at [ bit 18 to bit 19 ).
    const bool VC_Warning_BrakeAppsDisagreement_val = in_msg->VC_Warning_BrakeAppsDisagreement_value;
    const uint32_t VC_Warning_BrakeAppsDisagreement_raw = CAN_ENCODE(VC_Warning_BrakeAppsDisagreement_val, CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_SCALE, CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(VC_Warning_BrakeAppsDisagreement_raw, 2, 0x4);   // Packs bits _____#__ of byte 2 
    
    // Pack 1-bit signal VC_Warning_SbgInitFailed into payload at [ bit 19 to bit 20 ).
    const bool VC_Warning_SbgInitFailed_val = in_msg->VC_Warning_SbgInitFailed_value;
    const uint32_t VC_Warning_SbgInitFailed_raw = CAN_ENCODE(VC_Warning_SbgInitFailed_val, CANSIG_VC_WARNING_SBG_INIT_FAILED_SCALE, CANSIG_VC_WARNING_SBG_INIT_FAILED_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(VC_Warning_SbgInitFailed_raw, 3, 0x8);   // Packs bits ____#___ of byte 2 
    
    
}
 
// pack function for VC_Faults
void app_canUtils_VC_Faults_pack(const VC_Faults_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__FEDCBA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 1-bit signal VC_Fault_MissingBMSHeartbeat into payload at [ bit 0 to bit 1 ).
    const bool VC_Fault_MissingBMSHeartbeat_val = in_msg->VC_Fault_MissingBMSHeartbeat_value;
    const uint32_t VC_Fault_MissingBMSHeartbeat_raw = CAN_ENCODE(VC_Fault_MissingBMSHeartbeat_val, CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_SCALE, CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_Fault_MissingBMSHeartbeat_raw, 0, 0x1);   // Packs bits _______# of byte 0 
    
    // Pack 1-bit signal VC_Fault_MissingFSMHeartbeat into payload at [ bit 1 to bit 2 ).
    const bool VC_Fault_MissingFSMHeartbeat_val = in_msg->VC_Fault_MissingFSMHeartbeat_value;
    const uint32_t VC_Fault_MissingFSMHeartbeat_raw = CAN_ENCODE(VC_Fault_MissingFSMHeartbeat_val, CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_SCALE, CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Fault_MissingFSMHeartbeat_raw, 1, 0x2);   // Packs bits ______#_ of byte 0 
    
    // Pack 1-bit signal VC_Fault_MissingRSMHeartbeat into payload at [ bit 2 to bit 3 ).
    const bool VC_Fault_MissingRSMHeartbeat_val = in_msg->VC_Fault_MissingRSMHeartbeat_value;
    const uint32_t VC_Fault_MissingRSMHeartbeat_raw = CAN_ENCODE(VC_Fault_MissingRSMHeartbeat_val, CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_SCALE, CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Fault_MissingRSMHeartbeat_raw, 2, 0x4);   // Packs bits _____#__ of byte 0 
    
    // Pack 1-bit signal VC_Fault_MissingCRITHeartbeat into payload at [ bit 3 to bit 4 ).
    const bool VC_Fault_MissingCRITHeartbeat_val = in_msg->VC_Fault_MissingCRITHeartbeat_value;
    const uint32_t VC_Fault_MissingCRITHeartbeat_raw = CAN_ENCODE(VC_Fault_MissingCRITHeartbeat_val, CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_SCALE, CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Fault_MissingCRITHeartbeat_raw, 3, 0x8);   // Packs bits ____#___ of byte 0 
    
    // Pack 1-bit signal VC_Fault_LeftInverterFault into payload at [ bit 4 to bit 5 ).
    const bool VC_Fault_LeftInverterFault_val = in_msg->VC_Fault_LeftInverterFault_value;
    const uint32_t VC_Fault_LeftInverterFault_raw = CAN_ENCODE(VC_Fault_LeftInverterFault_val, CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_SCALE, CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Fault_LeftInverterFault_raw, 4, 0x10);   // Packs bits ___#____ of byte 0 
    
    // Pack 1-bit signal VC_Fault_RightInverterFault into payload at [ bit 5 to bit 6 ).
    const bool VC_Fault_RightInverterFault_val = in_msg->VC_Fault_RightInverterFault_value;
    const uint32_t VC_Fault_RightInverterFault_raw = CAN_ENCODE(VC_Fault_RightInverterFault_val, CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_SCALE, CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Fault_RightInverterFault_raw, 5, 0x20);   // Packs bits __#_____ of byte 0 
    
    
}
 
// pack function for VC_WarningsCounts
void app_canUtils_VC_WarningsCounts_pack(const VC_WarningsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_WarningsCounts.
    // |____TTTS|SSRRRQQQ|PPPOOONN|NMMMLLLK|KKJJJIII|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal VC_Warning_StackWaterMarkHighTask1HzCount into payload at [ bit 0 to bit 3 ).
    const uint32_t VC_Warning_StackWaterMarkHighTask1HzCount_val = in_msg->VC_Warning_StackWaterMarkHighTask1HzCount_value;
    const uint32_t VC_Warning_StackWaterMarkHighTask1HzCount_raw = CAN_ENCODE(VC_Warning_StackWaterMarkHighTask1HzCount_val, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_Warning_StackWaterMarkHighTask1HzCount_raw, 0, 0x7);   // Packs bits _____### of byte 0 
    
    // Pack 3-bit signal VC_Warning_StackWaterMarkHighTask100HzCount into payload at [ bit 3 to bit 6 ).
    const uint32_t VC_Warning_StackWaterMarkHighTask100HzCount_val = in_msg->VC_Warning_StackWaterMarkHighTask100HzCount_value;
    const uint32_t VC_Warning_StackWaterMarkHighTask100HzCount_raw = CAN_ENCODE(VC_Warning_StackWaterMarkHighTask100HzCount_val, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Warning_StackWaterMarkHighTask100HzCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0 
    
    // Pack 3-bit signal VC_Warning_StackWaterMarkHighTask1kHzCount into payload at [ bit 6 to bit 9 ).
    const uint32_t VC_Warning_StackWaterMarkHighTask1kHzCount_val = in_msg->VC_Warning_StackWaterMarkHighTask1kHzCount_value;
    const uint32_t VC_Warning_StackWaterMarkHighTask1kHzCount_raw = CAN_ENCODE(VC_Warning_StackWaterMarkHighTask1kHzCount_val, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Warning_StackWaterMarkHighTask1kHzCount_raw, 6, 0xC0);   // Packs bits ##______ of byte 0 
    out_data[1] |= packShiftRight(VC_Warning_StackWaterMarkHighTask1kHzCount_raw, 2, 0x1);   // Packs bits _______# of byte 1 
    
    // Pack 3-bit signal VC_Warning_StackWaterMarkHighTaskCanRxCount into payload at [ bit 9 to bit 12 ).
    const uint32_t VC_Warning_StackWaterMarkHighTaskCanRxCount_val = in_msg->VC_Warning_StackWaterMarkHighTaskCanRxCount_value;
    const uint32_t VC_Warning_StackWaterMarkHighTaskCanRxCount_raw = CAN_ENCODE(VC_Warning_StackWaterMarkHighTaskCanRxCount_val, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Warning_StackWaterMarkHighTaskCanRxCount_raw, 1, 0xE);   // Packs bits ____###_ of byte 1 
    
    // Pack 3-bit signal VC_Warning_StackWaterMarkHighTaskCanTxCount into payload at [ bit 12 to bit 15 ).
    const uint32_t VC_Warning_StackWaterMarkHighTaskCanTxCount_val = in_msg->VC_Warning_StackWaterMarkHighTaskCanTxCount_value;
    const uint32_t VC_Warning_StackWaterMarkHighTaskCanTxCount_raw = CAN_ENCODE(VC_Warning_StackWaterMarkHighTaskCanTxCount_val, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_SCALE, CANSIG_VC_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Warning_StackWaterMarkHighTaskCanTxCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1 
    
    // Pack 3-bit signal VC_Warning_WatchdogTimeoutCount into payload at [ bit 15 to bit 18 ).
    const uint32_t VC_Warning_WatchdogTimeoutCount_val = in_msg->VC_Warning_WatchdogTimeoutCount_value;
    const uint32_t VC_Warning_WatchdogTimeoutCount_raw = CAN_ENCODE(VC_Warning_WatchdogTimeoutCount_val, CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_COUNT_SCALE, CANSIG_VC_WARNING_WATCHDOG_TIMEOUT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Warning_WatchdogTimeoutCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1 
    out_data[2] |= packShiftRight(VC_Warning_WatchdogTimeoutCount_raw, 1, 0x3);   // Packs bits ______## of byte 2 
    
    // Pack 3-bit signal VC_Warning_TxOverflowCount into payload at [ bit 18 to bit 21 ).
    const uint32_t VC_Warning_TxOverflowCount_val = in_msg->VC_Warning_TxOverflowCount_value;
    const uint32_t VC_Warning_TxOverflowCount_raw = CAN_ENCODE(VC_Warning_TxOverflowCount_val, CANSIG_VC_WARNING_TX_OVERFLOW_COUNT_SCALE, CANSIG_VC_WARNING_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(VC_Warning_TxOverflowCount_raw, 2, 0x1C);   // Packs bits ___###__ of byte 2 
    
    // Pack 3-bit signal VC_Warning_RxOverflowCount into payload at [ bit 21 to bit 24 ).
    const uint32_t VC_Warning_RxOverflowCount_val = in_msg->VC_Warning_RxOverflowCount_value;
    const uint32_t VC_Warning_RxOverflowCount_raw = CAN_ENCODE(VC_Warning_RxOverflowCount_val, CANSIG_VC_WARNING_RX_OVERFLOW_COUNT_SCALE, CANSIG_VC_WARNING_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    out_data[2] |= packShiftLeft(VC_Warning_RxOverflowCount_raw, 5, 0xE0);   // Packs bits ###_____ of byte 2 
    
    // Pack 3-bit signal VC_Warning_RegenNotAvailableCount into payload at [ bit 24 to bit 27 ).
    const uint32_t VC_Warning_RegenNotAvailableCount_val = in_msg->VC_Warning_RegenNotAvailableCount_value;
    const uint32_t VC_Warning_RegenNotAvailableCount_raw = CAN_ENCODE(VC_Warning_RegenNotAvailableCount_val, CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_COUNT_SCALE, CANSIG_VC_WARNING_REGEN_NOT_AVAILABLE_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftRight(VC_Warning_RegenNotAvailableCount_raw, 0, 0x7);   // Packs bits _____### of byte 3 
    
    // Pack 3-bit signal VC_Warning_ImuInitFailedCount into payload at [ bit 27 to bit 30 ).
    const uint32_t VC_Warning_ImuInitFailedCount_val = in_msg->VC_Warning_ImuInitFailedCount_value;
    const uint32_t VC_Warning_ImuInitFailedCount_raw = CAN_ENCODE(VC_Warning_ImuInitFailedCount_val, CANSIG_VC_WARNING_IMU_INIT_FAILED_COUNT_SCALE, CANSIG_VC_WARNING_IMU_INIT_FAILED_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(VC_Warning_ImuInitFailedCount_raw, 3, 0x38);   // Packs bits __###___ of byte 3 
    
    // Pack 3-bit signal VC_Warning_FlowRateInputOutOfRangeCount into payload at [ bit 30 to bit 33 ).
    const uint32_t VC_Warning_FlowRateInputOutOfRangeCount_val = in_msg->VC_Warning_FlowRateInputOutOfRangeCount_value;
    const uint32_t VC_Warning_FlowRateInputOutOfRangeCount_raw = CAN_ENCODE(VC_Warning_FlowRateInputOutOfRangeCount_val, CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_COUNT_SCALE, CANSIG_VC_WARNING_FLOW_RATE_INPUT_OUT_OF_RANGE_COUNT_OFFSET, uint32_t);
    out_data[3] |= packShiftLeft(VC_Warning_FlowRateInputOutOfRangeCount_raw, 6, 0xC0);   // Packs bits ##______ of byte 3 
    out_data[4] |= packShiftRight(VC_Warning_FlowRateInputOutOfRangeCount_raw, 2, 0x1);   // Packs bits _______# of byte 4 
    
    // Pack 3-bit signal VC_Warning_BatteryRailOvercurrentFaultCount into payload at [ bit 33 to bit 36 ).
    const uint32_t VC_Warning_BatteryRailOvercurrentFaultCount_val = in_msg->VC_Warning_BatteryRailOvercurrentFaultCount_value;
    const uint32_t VC_Warning_BatteryRailOvercurrentFaultCount_raw = CAN_ENCODE(VC_Warning_BatteryRailOvercurrentFaultCount_val, CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_COUNT_SCALE, CANSIG_VC_WARNING_BATTERY_RAIL_OVERCURRENT_FAULT_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(VC_Warning_BatteryRailOvercurrentFaultCount_raw, 1, 0xE);   // Packs bits ____###_ of byte 4 
    
    // Pack 3-bit signal VC_Warning_AccumulatorRailOvercurrentFaultCount into payload at [ bit 36 to bit 39 ).
    const uint32_t VC_Warning_AccumulatorRailOvercurrentFaultCount_val = in_msg->VC_Warning_AccumulatorRailOvercurrentFaultCount_value;
    const uint32_t VC_Warning_AccumulatorRailOvercurrentFaultCount_raw = CAN_ENCODE(VC_Warning_AccumulatorRailOvercurrentFaultCount_val, CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_COUNT_SCALE, CANSIG_VC_WARNING_ACCUMULATOR_RAIL_OVERCURRENT_FAULT_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(VC_Warning_AccumulatorRailOvercurrentFaultCount_raw, 4, 0x70);   // Packs bits _###____ of byte 4 
    
    // Pack 3-bit signal VC_Warning_LvChargeFaultCount into payload at [ bit 39 to bit 42 ).
    const uint32_t VC_Warning_LvChargeFaultCount_val = in_msg->VC_Warning_LvChargeFaultCount_value;
    const uint32_t VC_Warning_LvChargeFaultCount_raw = CAN_ENCODE(VC_Warning_LvChargeFaultCount_val, CANSIG_VC_WARNING_LV_CHARGE_FAULT_COUNT_SCALE, CANSIG_VC_WARNING_LV_CHARGE_FAULT_COUNT_OFFSET, uint32_t);
    out_data[4] |= packShiftLeft(VC_Warning_LvChargeFaultCount_raw, 7, 0x80);   // Packs bits #_______ of byte 4 
    out_data[5] |= packShiftRight(VC_Warning_LvChargeFaultCount_raw, 1, 0x3);   // Packs bits ______## of byte 5 
    
    // Pack 3-bit signal VC_Warning_BoostControllerFaultCount into payload at [ bit 42 to bit 45 ).
    const uint32_t VC_Warning_BoostControllerFaultCount_val = in_msg->VC_Warning_BoostControllerFaultCount_value;
    const uint32_t VC_Warning_BoostControllerFaultCount_raw = CAN_ENCODE(VC_Warning_BoostControllerFaultCount_val, CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_COUNT_SCALE, CANSIG_VC_WARNING_BOOST_CONTROLLER_FAULT_COUNT_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(VC_Warning_BoostControllerFaultCount_raw, 2, 0x1C);   // Packs bits ___###__ of byte 5 
    
    // Pack 3-bit signal VC_Warning_HighNumberOfCanDataLogsCount into payload at [ bit 45 to bit 48 ).
    const uint32_t VC_Warning_HighNumberOfCanDataLogsCount_val = in_msg->VC_Warning_HighNumberOfCanDataLogsCount_value;
    const uint32_t VC_Warning_HighNumberOfCanDataLogsCount_raw = CAN_ENCODE(VC_Warning_HighNumberOfCanDataLogsCount_val, CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_COUNT_SCALE, CANSIG_VC_WARNING_HIGH_NUMBER_OF_CAN_DATA_LOGS_COUNT_OFFSET, uint32_t);
    out_data[5] |= packShiftLeft(VC_Warning_HighNumberOfCanDataLogsCount_raw, 5, 0xE0);   // Packs bits ###_____ of byte 5 
    
    // Pack 3-bit signal VC_Warning_CanLoggingSdCardNotPresentCount into payload at [ bit 48 to bit 51 ).
    const uint32_t VC_Warning_CanLoggingSdCardNotPresentCount_val = in_msg->VC_Warning_CanLoggingSdCardNotPresentCount_value;
    const uint32_t VC_Warning_CanLoggingSdCardNotPresentCount_raw = CAN_ENCODE(VC_Warning_CanLoggingSdCardNotPresentCount_val, CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_COUNT_SCALE, CANSIG_VC_WARNING_CAN_LOGGING_SD_CARD_NOT_PRESENT_COUNT_OFFSET, uint32_t);
    out_data[6] |= packShiftRight(VC_Warning_CanLoggingSdCardNotPresentCount_raw, 0, 0x7);   // Packs bits _____### of byte 6 
    
    // Pack 3-bit signal VC_Warning_CanLoggingErroredCount into payload at [ bit 51 to bit 54 ).
    const uint32_t VC_Warning_CanLoggingErroredCount_val = in_msg->VC_Warning_CanLoggingErroredCount_value;
    const uint32_t VC_Warning_CanLoggingErroredCount_raw = CAN_ENCODE(VC_Warning_CanLoggingErroredCount_val, CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_COUNT_SCALE, CANSIG_VC_WARNING_CAN_LOGGING_ERRORED_COUNT_OFFSET, uint32_t);
    out_data[6] |= packShiftLeft(VC_Warning_CanLoggingErroredCount_raw, 3, 0x38);   // Packs bits __###___ of byte 6 
    
    // Pack 3-bit signal VC_Warning_BrakeAppsDisagreementCount into payload at [ bit 54 to bit 57 ).
    const uint32_t VC_Warning_BrakeAppsDisagreementCount_val = in_msg->VC_Warning_BrakeAppsDisagreementCount_value;
    const uint32_t VC_Warning_BrakeAppsDisagreementCount_raw = CAN_ENCODE(VC_Warning_BrakeAppsDisagreementCount_val, CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_SCALE, CANSIG_VC_WARNING_BRAKE_APPS_DISAGREEMENT_COUNT_OFFSET, uint32_t);
    out_data[6] |= packShiftLeft(VC_Warning_BrakeAppsDisagreementCount_raw, 6, 0xC0);   // Packs bits ##______ of byte 6 
    out_data[7] |= packShiftRight(VC_Warning_BrakeAppsDisagreementCount_raw, 2, 0x1);   // Packs bits _______# of byte 7 
    
    // Pack 3-bit signal VC_Warning_SbgInitFailedCount into payload at [ bit 57 to bit 60 ).
    const uint32_t VC_Warning_SbgInitFailedCount_val = in_msg->VC_Warning_SbgInitFailedCount_value;
    const uint32_t VC_Warning_SbgInitFailedCount_raw = CAN_ENCODE(VC_Warning_SbgInitFailedCount_val, CANSIG_VC_WARNING_SBG_INIT_FAILED_COUNT_SCALE, CANSIG_VC_WARNING_SBG_INIT_FAILED_COUNT_OFFSET, uint32_t);
    out_data[7] |= packShiftLeft(VC_Warning_SbgInitFailedCount_raw, 1, 0xE);   // Packs bits ____###_ of byte 7 
    
    
}
 
// pack function for VC_FaultsCounts
void app_canUtils_VC_FaultsCounts_pack(const VC_FaultsCounts_Signals* const in_msg, uint8_t* const out_data)
{
    // Pack message VC_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______FF|FEEEDDDC|CCBBBAAA|
    
    if (in_msg == NULL || out_data == NULL)
    {
        return;
    }
    
    // Pack 3-bit signal VC_Fault_MissingBMSHeartbeatCount into payload at [ bit 0 to bit 3 ).
    const uint32_t VC_Fault_MissingBMSHeartbeatCount_val = in_msg->VC_Fault_MissingBMSHeartbeatCount_value;
    const uint32_t VC_Fault_MissingBMSHeartbeatCount_raw = CAN_ENCODE(VC_Fault_MissingBMSHeartbeatCount_val, CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_COUNT_SCALE, CANSIG_VC_FAULT_MISSING_BMS_HEARTBEAT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftRight(VC_Fault_MissingBMSHeartbeatCount_raw, 0, 0x7);   // Packs bits _____### of byte 0 
    
    // Pack 3-bit signal VC_Fault_MissingFSMHeartbeatCount into payload at [ bit 3 to bit 6 ).
    const uint32_t VC_Fault_MissingFSMHeartbeatCount_val = in_msg->VC_Fault_MissingFSMHeartbeatCount_value;
    const uint32_t VC_Fault_MissingFSMHeartbeatCount_raw = CAN_ENCODE(VC_Fault_MissingFSMHeartbeatCount_val, CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_COUNT_SCALE, CANSIG_VC_FAULT_MISSING_FSM_HEARTBEAT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Fault_MissingFSMHeartbeatCount_raw, 3, 0x38);   // Packs bits __###___ of byte 0 
    
    // Pack 3-bit signal VC_Fault_MissingRSMHeartbeatCount into payload at [ bit 6 to bit 9 ).
    const uint32_t VC_Fault_MissingRSMHeartbeatCount_val = in_msg->VC_Fault_MissingRSMHeartbeatCount_value;
    const uint32_t VC_Fault_MissingRSMHeartbeatCount_raw = CAN_ENCODE(VC_Fault_MissingRSMHeartbeatCount_val, CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_COUNT_SCALE, CANSIG_VC_FAULT_MISSING_RSM_HEARTBEAT_COUNT_OFFSET, uint32_t);
    out_data[0] |= packShiftLeft(VC_Fault_MissingRSMHeartbeatCount_raw, 6, 0xC0);   // Packs bits ##______ of byte 0 
    out_data[1] |= packShiftRight(VC_Fault_MissingRSMHeartbeatCount_raw, 2, 0x1);   // Packs bits _______# of byte 1 
    
    // Pack 3-bit signal VC_Fault_MissingCRITHeartbeatCount into payload at [ bit 9 to bit 12 ).
    const uint32_t VC_Fault_MissingCRITHeartbeatCount_val = in_msg->VC_Fault_MissingCRITHeartbeatCount_value;
    const uint32_t VC_Fault_MissingCRITHeartbeatCount_raw = CAN_ENCODE(VC_Fault_MissingCRITHeartbeatCount_val, CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_COUNT_SCALE, CANSIG_VC_FAULT_MISSING_CRIT_HEARTBEAT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Fault_MissingCRITHeartbeatCount_raw, 1, 0xE);   // Packs bits ____###_ of byte 1 
    
    // Pack 3-bit signal VC_Fault_LeftInverterFaultCount into payload at [ bit 12 to bit 15 ).
    const uint32_t VC_Fault_LeftInverterFaultCount_val = in_msg->VC_Fault_LeftInverterFaultCount_value;
    const uint32_t VC_Fault_LeftInverterFaultCount_raw = CAN_ENCODE(VC_Fault_LeftInverterFaultCount_val, CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_COUNT_SCALE, CANSIG_VC_FAULT_LEFT_INVERTER_FAULT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Fault_LeftInverterFaultCount_raw, 4, 0x70);   // Packs bits _###____ of byte 1 
    
    // Pack 3-bit signal VC_Fault_RightInverterFaultCount into payload at [ bit 15 to bit 18 ).
    const uint32_t VC_Fault_RightInverterFaultCount_val = in_msg->VC_Fault_RightInverterFaultCount_value;
    const uint32_t VC_Fault_RightInverterFaultCount_raw = CAN_ENCODE(VC_Fault_RightInverterFaultCount_val, CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_COUNT_SCALE, CANSIG_VC_FAULT_RIGHT_INVERTER_FAULT_COUNT_OFFSET, uint32_t);
    out_data[1] |= packShiftLeft(VC_Fault_RightInverterFaultCount_raw, 7, 0x80);   // Packs bits #_______ of byte 1 
    out_data[2] |= packShiftRight(VC_Fault_RightInverterFaultCount_raw, 1, 0x3);   // Packs bits ______## of byte 2 
    
    
}




// unpack function for BMS_Warnings
void app_canUtils_BMS_Warnings_unpack(const uint8_t* const in_data, BMS_Warnings_Signals* const out_msg)
{
    // Unpack message BMS_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_ONMLKJI|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t BMS_Warning_StackWaterMarkHighTask1Hz_raw = 0; // Temporary raw variable for BMS_Warning_StackWaterMarkHighTask1Hz
    BMS_Warning_StackWaterMarkHighTask1Hz_raw |= unpackShiftRight(in_data[0], 0, 0x1); // Unpacks bits _______# from byte 0
    out_msg->BMS_Warning_StackWaterMarkHighTask1Hz_value = CAN_DECODE(BMS_Warning_StackWaterMarkHighTask1Hz_raw, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_StackWaterMarkHighTask100Hz_raw = 0; // Temporary raw variable for BMS_Warning_StackWaterMarkHighTask100Hz
    BMS_Warning_StackWaterMarkHighTask100Hz_raw |= unpackShiftLeft(in_data[0], 1, 0x2); // Unpacks bits ______#_ from byte 0
    out_msg->BMS_Warning_StackWaterMarkHighTask100Hz_value = CAN_DECODE(BMS_Warning_StackWaterMarkHighTask100Hz_raw, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_StackWaterMarkHighTask1kHz_raw = 0; // Temporary raw variable for BMS_Warning_StackWaterMarkHighTask1kHz
    BMS_Warning_StackWaterMarkHighTask1kHz_raw |= unpackShiftLeft(in_data[0], 2, 0x4); // Unpacks bits _____#__ from byte 0
    out_msg->BMS_Warning_StackWaterMarkHighTask1kHz_value = CAN_DECODE(BMS_Warning_StackWaterMarkHighTask1kHz_raw, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_StackWaterMarkHighTaskCanRx_raw = 0; // Temporary raw variable for BMS_Warning_StackWaterMarkHighTaskCanRx
    BMS_Warning_StackWaterMarkHighTaskCanRx_raw |= unpackShiftLeft(in_data[0], 3, 0x8); // Unpacks bits ____#___ from byte 0
    out_msg->BMS_Warning_StackWaterMarkHighTaskCanRx_value = CAN_DECODE(BMS_Warning_StackWaterMarkHighTaskCanRx_raw, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_StackWaterMarkHighTaskCanTx_raw = 0; // Temporary raw variable for BMS_Warning_StackWaterMarkHighTaskCanTx
    BMS_Warning_StackWaterMarkHighTaskCanTx_raw |= unpackShiftLeft(in_data[0], 4, 0x10); // Unpacks bits ___#____ from byte 0
    out_msg->BMS_Warning_StackWaterMarkHighTaskCanTx_value = CAN_DECODE(BMS_Warning_StackWaterMarkHighTaskCanTx_raw, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_WatchdogTimeout_raw = 0; // Temporary raw variable for BMS_Warning_WatchdogTimeout
    BMS_Warning_WatchdogTimeout_raw |= unpackShiftLeft(in_data[0], 5, 0x20); // Unpacks bits __#_____ from byte 0
    out_msg->BMS_Warning_WatchdogTimeout_value = CAN_DECODE(BMS_Warning_WatchdogTimeout_raw, CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_TxOverflow_raw = 0; // Temporary raw variable for BMS_Warning_TxOverflow
    BMS_Warning_TxOverflow_raw |= unpackShiftLeft(in_data[0], 6, 0x40); // Unpacks bits _#______ from byte 0
    out_msg->BMS_Warning_TxOverflow_value = CAN_DECODE(BMS_Warning_TxOverflow_raw, CANSIG_BMS_WARNING_TX_OVERFLOW_SCALE, CANSIG_BMS_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_RxOverflow_raw = 0; // Temporary raw variable for BMS_Warning_RxOverflow
    BMS_Warning_RxOverflow_raw |= unpackShiftLeft(in_data[0], 7, 0x80); // Unpacks bits #_______ from byte 0
    out_msg->BMS_Warning_RxOverflow_value = CAN_DECODE(BMS_Warning_RxOverflow_raw, CANSIG_BMS_WARNING_RX_OVERFLOW_SCALE, CANSIG_BMS_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheckFault_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheckFault
    BMS_Warning_OpenWireCheckFault_raw |= unpackShiftRight(in_data[1], 0, 0x1); // Unpacks bits _______# from byte 1
    out_msg->BMS_Warning_OpenWireCheckFault_value = CAN_DECODE(BMS_Warning_OpenWireCheckFault_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheck_Segment0_GND_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheck_Segment0_GND
    BMS_Warning_OpenWireCheck_Segment0_GND_raw |= unpackShiftLeft(in_data[1], 1, 0x2); // Unpacks bits ______#_ from byte 1
    out_msg->BMS_Warning_OpenWireCheck_Segment0_GND_value = CAN_DECODE(BMS_Warning_OpenWireCheck_Segment0_GND_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheck_Segment1_GND_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheck_Segment1_GND
    BMS_Warning_OpenWireCheck_Segment1_GND_raw |= unpackShiftLeft(in_data[1], 2, 0x4); // Unpacks bits _____#__ from byte 1
    out_msg->BMS_Warning_OpenWireCheck_Segment1_GND_value = CAN_DECODE(BMS_Warning_OpenWireCheck_Segment1_GND_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheck_Segment2_GND_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheck_Segment2_GND
    BMS_Warning_OpenWireCheck_Segment2_GND_raw |= unpackShiftLeft(in_data[1], 3, 0x8); // Unpacks bits ____#___ from byte 1
    out_msg->BMS_Warning_OpenWireCheck_Segment2_GND_value = CAN_DECODE(BMS_Warning_OpenWireCheck_Segment2_GND_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheck_Segment3_GND_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheck_Segment3_GND
    BMS_Warning_OpenWireCheck_Segment3_GND_raw |= unpackShiftLeft(in_data[1], 4, 0x10); // Unpacks bits ___#____ from byte 1
    out_msg->BMS_Warning_OpenWireCheck_Segment3_GND_value = CAN_DECODE(BMS_Warning_OpenWireCheck_Segment3_GND_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheck_Segment4_GND_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheck_Segment4_GND
    BMS_Warning_OpenWireCheck_Segment4_GND_raw |= unpackShiftLeft(in_data[1], 5, 0x20); // Unpacks bits __#_____ from byte 1
    out_msg->BMS_Warning_OpenWireCheck_Segment4_GND_value = CAN_DECODE(BMS_Warning_OpenWireCheck_Segment4_GND_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_MissingVCHeartbeat_raw = 0; // Temporary raw variable for BMS_Warning_MissingVCHeartbeat
    BMS_Warning_MissingVCHeartbeat_raw |= unpackShiftLeft(in_data[1], 6, 0x40); // Unpacks bits _#______ from byte 1
    out_msg->BMS_Warning_MissingVCHeartbeat_value = CAN_DECODE(BMS_Warning_MissingVCHeartbeat_raw, CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_SCALE, CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_OFFSET, uint32_t);
    
    
}


// unpack function for BMS_Faults
void app_canUtils_BMS_Faults_unpack(const uint8_t* const in_data, BMS_Faults_Signals* const out_msg)
{
    // Unpack message BMS_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|_____KJI|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t BMS_Fault_StateMachine_raw = 0; // Temporary raw variable for BMS_Fault_StateMachine
    BMS_Fault_StateMachine_raw |= unpackShiftRight(in_data[0], 0, 0x1); // Unpacks bits _______# from byte 0
    out_msg->BMS_Fault_StateMachine_value = CAN_DECODE(BMS_Fault_StateMachine_raw, CANSIG_BMS_FAULT_STATE_MACHINE_SCALE, CANSIG_BMS_FAULT_STATE_MACHINE_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_CellUndervoltage_raw = 0; // Temporary raw variable for BMS_Fault_CellUndervoltage
    BMS_Fault_CellUndervoltage_raw |= unpackShiftLeft(in_data[0], 1, 0x2); // Unpacks bits ______#_ from byte 0
    out_msg->BMS_Fault_CellUndervoltage_value = CAN_DECODE(BMS_Fault_CellUndervoltage_raw, CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_SCALE, CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_CellOvervoltage_raw = 0; // Temporary raw variable for BMS_Fault_CellOvervoltage
    BMS_Fault_CellOvervoltage_raw |= unpackShiftLeft(in_data[0], 2, 0x4); // Unpacks bits _____#__ from byte 0
    out_msg->BMS_Fault_CellOvervoltage_value = CAN_DECODE(BMS_Fault_CellOvervoltage_raw, CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_SCALE, CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_ModuleCommunicationError_raw = 0; // Temporary raw variable for BMS_Fault_ModuleCommunicationError
    BMS_Fault_ModuleCommunicationError_raw |= unpackShiftLeft(in_data[0], 3, 0x8); // Unpacks bits ____#___ from byte 0
    out_msg->BMS_Fault_ModuleCommunicationError_value = CAN_DECODE(BMS_Fault_ModuleCommunicationError_raw, CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_SCALE, CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_CellUndertemp_raw = 0; // Temporary raw variable for BMS_Fault_CellUndertemp
    BMS_Fault_CellUndertemp_raw |= unpackShiftLeft(in_data[0], 4, 0x10); // Unpacks bits ___#____ from byte 0
    out_msg->BMS_Fault_CellUndertemp_value = CAN_DECODE(BMS_Fault_CellUndertemp_raw, CANSIG_BMS_FAULT_CELL_UNDERTEMP_SCALE, CANSIG_BMS_FAULT_CELL_UNDERTEMP_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_CellOvertemp_raw = 0; // Temporary raw variable for BMS_Fault_CellOvertemp
    BMS_Fault_CellOvertemp_raw |= unpackShiftLeft(in_data[0], 5, 0x20); // Unpacks bits __#_____ from byte 0
    out_msg->BMS_Fault_CellOvertemp_value = CAN_DECODE(BMS_Fault_CellOvertemp_raw, CANSIG_BMS_FAULT_CELL_OVERTEMP_SCALE, CANSIG_BMS_FAULT_CELL_OVERTEMP_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_ChargerReportedError_raw = 0; // Temporary raw variable for BMS_Fault_ChargerReportedError
    BMS_Fault_ChargerReportedError_raw |= unpackShiftLeft(in_data[0], 6, 0x40); // Unpacks bits _#______ from byte 0
    out_msg->BMS_Fault_ChargerReportedError_value = CAN_DECODE(BMS_Fault_ChargerReportedError_raw, CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_SCALE, CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_ChargerDisconnectedDuringCharge_raw = 0; // Temporary raw variable for BMS_Fault_ChargerDisconnectedDuringCharge
    BMS_Fault_ChargerDisconnectedDuringCharge_raw |= unpackShiftLeft(in_data[0], 7, 0x80); // Unpacks bits #_______ from byte 0
    out_msg->BMS_Fault_ChargerDisconnectedDuringCharge_value = CAN_DECODE(BMS_Fault_ChargerDisconnectedDuringCharge_raw, CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_SCALE, CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_ChargerShutdownLoopOpen_raw = 0; // Temporary raw variable for BMS_Fault_ChargerShutdownLoopOpen
    BMS_Fault_ChargerShutdownLoopOpen_raw |= unpackShiftRight(in_data[1], 0, 0x1); // Unpacks bits _______# from byte 1
    out_msg->BMS_Fault_ChargerShutdownLoopOpen_value = CAN_DECODE(BMS_Fault_ChargerShutdownLoopOpen_raw, CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_SCALE, CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_TractiveSystemOvercurrent_raw = 0; // Temporary raw variable for BMS_Fault_TractiveSystemOvercurrent
    BMS_Fault_TractiveSystemOvercurrent_raw |= unpackShiftLeft(in_data[1], 1, 0x2); // Unpacks bits ______#_ from byte 1
    out_msg->BMS_Fault_TractiveSystemOvercurrent_value = CAN_DECODE(BMS_Fault_TractiveSystemOvercurrent_raw, CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_SCALE, CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_PrechargeFailure_raw = 0; // Temporary raw variable for BMS_Fault_PrechargeFailure
    BMS_Fault_PrechargeFailure_raw |= unpackShiftLeft(in_data[1], 2, 0x4); // Unpacks bits _____#__ from byte 1
    out_msg->BMS_Fault_PrechargeFailure_value = CAN_DECODE(BMS_Fault_PrechargeFailure_raw, CANSIG_BMS_FAULT_PRECHARGE_FAILURE_SCALE, CANSIG_BMS_FAULT_PRECHARGE_FAILURE_OFFSET, uint32_t);
    
    
}


// unpack function for BMS_WarningsCounts
void app_canUtils_BMS_WarningsCounts_unpack(const uint8_t* const in_data, BMS_WarningsCounts_Signals* const out_msg)
{
    // Unpack message BMS_WarningsCounts.
    // |xxxxxxxx|xxxxxxxx|___OOONN|NMMMLLLK|KKJJJIII|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t BMS_Warning_StackWaterMarkHighTask1HzCount_raw = 0; // Temporary raw variable for BMS_Warning_StackWaterMarkHighTask1HzCount
    BMS_Warning_StackWaterMarkHighTask1HzCount_raw |= unpackShiftRight(in_data[0], 0, 0x7); // Unpacks bits _____### from byte 0
    out_msg->BMS_Warning_StackWaterMarkHighTask1HzCount_value = CAN_DECODE(BMS_Warning_StackWaterMarkHighTask1HzCount_raw, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_StackWaterMarkHighTask100HzCount_raw = 0; // Temporary raw variable for BMS_Warning_StackWaterMarkHighTask100HzCount
    BMS_Warning_StackWaterMarkHighTask100HzCount_raw |= unpackShiftLeft(in_data[0], 3, 0x38); // Unpacks bits __###___ from byte 0
    out_msg->BMS_Warning_StackWaterMarkHighTask100HzCount_value = CAN_DECODE(BMS_Warning_StackWaterMarkHighTask100HzCount_raw, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_StackWaterMarkHighTask1kHzCount_raw = 0; // Temporary raw variable for BMS_Warning_StackWaterMarkHighTask1kHzCount
    BMS_Warning_StackWaterMarkHighTask1kHzCount_raw |= unpackShiftLeft(in_data[0], 6, 0xC0); // Unpacks bits ##______ from byte 0
    BMS_Warning_StackWaterMarkHighTask1kHzCount_raw |= unpackShiftRight(in_data[1], 2, 0x1); // Unpacks bits _______# from byte 1
    out_msg->BMS_Warning_StackWaterMarkHighTask1kHzCount_value = CAN_DECODE(BMS_Warning_StackWaterMarkHighTask1kHzCount_raw, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_StackWaterMarkHighTaskCanRxCount_raw = 0; // Temporary raw variable for BMS_Warning_StackWaterMarkHighTaskCanRxCount
    BMS_Warning_StackWaterMarkHighTaskCanRxCount_raw |= unpackShiftLeft(in_data[1], 1, 0xE); // Unpacks bits ____###_ from byte 1
    out_msg->BMS_Warning_StackWaterMarkHighTaskCanRxCount_value = CAN_DECODE(BMS_Warning_StackWaterMarkHighTaskCanRxCount_raw, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_StackWaterMarkHighTaskCanTxCount_raw = 0; // Temporary raw variable for BMS_Warning_StackWaterMarkHighTaskCanTxCount
    BMS_Warning_StackWaterMarkHighTaskCanTxCount_raw |= unpackShiftLeft(in_data[1], 4, 0x70); // Unpacks bits _###____ from byte 1
    out_msg->BMS_Warning_StackWaterMarkHighTaskCanTxCount_value = CAN_DECODE(BMS_Warning_StackWaterMarkHighTaskCanTxCount_raw, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_SCALE, CANSIG_BMS_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_WatchdogTimeoutCount_raw = 0; // Temporary raw variable for BMS_Warning_WatchdogTimeoutCount
    BMS_Warning_WatchdogTimeoutCount_raw |= unpackShiftLeft(in_data[1], 7, 0x80); // Unpacks bits #_______ from byte 1
    BMS_Warning_WatchdogTimeoutCount_raw |= unpackShiftRight(in_data[2], 1, 0x3); // Unpacks bits ______## from byte 2
    out_msg->BMS_Warning_WatchdogTimeoutCount_value = CAN_DECODE(BMS_Warning_WatchdogTimeoutCount_raw, CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_SCALE, CANSIG_BMS_WARNING_WATCHDOG_TIMEOUT_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_TxOverflowCount_raw = 0; // Temporary raw variable for BMS_Warning_TxOverflowCount
    BMS_Warning_TxOverflowCount_raw |= unpackShiftLeft(in_data[2], 2, 0x1C); // Unpacks bits ___###__ from byte 2
    out_msg->BMS_Warning_TxOverflowCount_value = CAN_DECODE(BMS_Warning_TxOverflowCount_raw, CANSIG_BMS_WARNING_TX_OVERFLOW_COUNT_SCALE, CANSIG_BMS_WARNING_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_RxOverflowCount_raw = 0; // Temporary raw variable for BMS_Warning_RxOverflowCount
    BMS_Warning_RxOverflowCount_raw |= unpackShiftLeft(in_data[2], 5, 0xE0); // Unpacks bits ###_____ from byte 2
    out_msg->BMS_Warning_RxOverflowCount_value = CAN_DECODE(BMS_Warning_RxOverflowCount_raw, CANSIG_BMS_WARNING_RX_OVERFLOW_COUNT_SCALE, CANSIG_BMS_WARNING_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheckFaultCount_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheckFaultCount
    BMS_Warning_OpenWireCheckFaultCount_raw |= unpackShiftRight(in_data[3], 0, 0x7); // Unpacks bits _____### from byte 3
    out_msg->BMS_Warning_OpenWireCheckFaultCount_value = CAN_DECODE(BMS_Warning_OpenWireCheckFaultCount_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_FAULT_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheck_Segment0_GNDCount_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheck_Segment0_GNDCount
    BMS_Warning_OpenWireCheck_Segment0_GNDCount_raw |= unpackShiftLeft(in_data[3], 3, 0x38); // Unpacks bits __###___ from byte 3
    out_msg->BMS_Warning_OpenWireCheck_Segment0_GNDCount_value = CAN_DECODE(BMS_Warning_OpenWireCheck_Segment0_GNDCount_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT0_GND_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheck_Segment1_GNDCount_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheck_Segment1_GNDCount
    BMS_Warning_OpenWireCheck_Segment1_GNDCount_raw |= unpackShiftLeft(in_data[3], 6, 0xC0); // Unpacks bits ##______ from byte 3
    BMS_Warning_OpenWireCheck_Segment1_GNDCount_raw |= unpackShiftRight(in_data[4], 2, 0x1); // Unpacks bits _______# from byte 4
    out_msg->BMS_Warning_OpenWireCheck_Segment1_GNDCount_value = CAN_DECODE(BMS_Warning_OpenWireCheck_Segment1_GNDCount_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT1_GND_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheck_Segment2_GNDCount_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheck_Segment2_GNDCount
    BMS_Warning_OpenWireCheck_Segment2_GNDCount_raw |= unpackShiftLeft(in_data[4], 1, 0xE); // Unpacks bits ____###_ from byte 4
    out_msg->BMS_Warning_OpenWireCheck_Segment2_GNDCount_value = CAN_DECODE(BMS_Warning_OpenWireCheck_Segment2_GNDCount_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT2_GND_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheck_Segment3_GNDCount_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheck_Segment3_GNDCount
    BMS_Warning_OpenWireCheck_Segment3_GNDCount_raw |= unpackShiftLeft(in_data[4], 4, 0x70); // Unpacks bits _###____ from byte 4
    out_msg->BMS_Warning_OpenWireCheck_Segment3_GNDCount_value = CAN_DECODE(BMS_Warning_OpenWireCheck_Segment3_GNDCount_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT3_GND_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_OpenWireCheck_Segment4_GNDCount_raw = 0; // Temporary raw variable for BMS_Warning_OpenWireCheck_Segment4_GNDCount
    BMS_Warning_OpenWireCheck_Segment4_GNDCount_raw |= unpackShiftLeft(in_data[4], 7, 0x80); // Unpacks bits #_______ from byte 4
    BMS_Warning_OpenWireCheck_Segment4_GNDCount_raw |= unpackShiftRight(in_data[5], 1, 0x3); // Unpacks bits ______## from byte 5
    out_msg->BMS_Warning_OpenWireCheck_Segment4_GNDCount_value = CAN_DECODE(BMS_Warning_OpenWireCheck_Segment4_GNDCount_raw, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_SCALE, CANSIG_BMS_WARNING_OPEN_WIRE_CHECK_SEGMENT4_GND_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Warning_MissingVCHeartbeatCount_raw = 0; // Temporary raw variable for BMS_Warning_MissingVCHeartbeatCount
    BMS_Warning_MissingVCHeartbeatCount_raw |= unpackShiftLeft(in_data[5], 2, 0x1C); // Unpacks bits ___###__ from byte 5
    out_msg->BMS_Warning_MissingVCHeartbeatCount_value = CAN_DECODE(BMS_Warning_MissingVCHeartbeatCount_raw, CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_SCALE, CANSIG_BMS_WARNING_MISSING_VC_HEARTBEAT_COUNT_OFFSET, uint32_t);
    
    
}


// unpack function for BMS_FaultsCounts
void app_canUtils_BMS_FaultsCounts_unpack(const uint8_t* const in_data, BMS_FaultsCounts_Signals* const out_msg)
{
    // Unpack message BMS_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|_______K|KKJJJIII|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t BMS_Fault_StateMachineCount_raw = 0; // Temporary raw variable for BMS_Fault_StateMachineCount
    BMS_Fault_StateMachineCount_raw |= unpackShiftRight(in_data[0], 0, 0x7); // Unpacks bits _____### from byte 0
    out_msg->BMS_Fault_StateMachineCount_value = CAN_DECODE(BMS_Fault_StateMachineCount_raw, CANSIG_BMS_FAULT_STATE_MACHINE_COUNT_SCALE, CANSIG_BMS_FAULT_STATE_MACHINE_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_CellUndervoltageCount_raw = 0; // Temporary raw variable for BMS_Fault_CellUndervoltageCount
    BMS_Fault_CellUndervoltageCount_raw |= unpackShiftLeft(in_data[0], 3, 0x38); // Unpacks bits __###___ from byte 0
    out_msg->BMS_Fault_CellUndervoltageCount_value = CAN_DECODE(BMS_Fault_CellUndervoltageCount_raw, CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_SCALE, CANSIG_BMS_FAULT_CELL_UNDERVOLTAGE_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_CellOvervoltageCount_raw = 0; // Temporary raw variable for BMS_Fault_CellOvervoltageCount
    BMS_Fault_CellOvervoltageCount_raw |= unpackShiftLeft(in_data[0], 6, 0xC0); // Unpacks bits ##______ from byte 0
    BMS_Fault_CellOvervoltageCount_raw |= unpackShiftRight(in_data[1], 2, 0x1); // Unpacks bits _______# from byte 1
    out_msg->BMS_Fault_CellOvervoltageCount_value = CAN_DECODE(BMS_Fault_CellOvervoltageCount_raw, CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_SCALE, CANSIG_BMS_FAULT_CELL_OVERVOLTAGE_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_ModuleCommunicationErrorCount_raw = 0; // Temporary raw variable for BMS_Fault_ModuleCommunicationErrorCount
    BMS_Fault_ModuleCommunicationErrorCount_raw |= unpackShiftLeft(in_data[1], 1, 0xE); // Unpacks bits ____###_ from byte 1
    out_msg->BMS_Fault_ModuleCommunicationErrorCount_value = CAN_DECODE(BMS_Fault_ModuleCommunicationErrorCount_raw, CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_SCALE, CANSIG_BMS_FAULT_MODULE_COMMUNICATION_ERROR_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_CellUndertempCount_raw = 0; // Temporary raw variable for BMS_Fault_CellUndertempCount
    BMS_Fault_CellUndertempCount_raw |= unpackShiftLeft(in_data[1], 4, 0x70); // Unpacks bits _###____ from byte 1
    out_msg->BMS_Fault_CellUndertempCount_value = CAN_DECODE(BMS_Fault_CellUndertempCount_raw, CANSIG_BMS_FAULT_CELL_UNDERTEMP_COUNT_SCALE, CANSIG_BMS_FAULT_CELL_UNDERTEMP_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_CellOvertempCount_raw = 0; // Temporary raw variable for BMS_Fault_CellOvertempCount
    BMS_Fault_CellOvertempCount_raw |= unpackShiftLeft(in_data[1], 7, 0x80); // Unpacks bits #_______ from byte 1
    BMS_Fault_CellOvertempCount_raw |= unpackShiftRight(in_data[2], 1, 0x3); // Unpacks bits ______## from byte 2
    out_msg->BMS_Fault_CellOvertempCount_value = CAN_DECODE(BMS_Fault_CellOvertempCount_raw, CANSIG_BMS_FAULT_CELL_OVERTEMP_COUNT_SCALE, CANSIG_BMS_FAULT_CELL_OVERTEMP_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_ChargerReportedErrorCount_raw = 0; // Temporary raw variable for BMS_Fault_ChargerReportedErrorCount
    BMS_Fault_ChargerReportedErrorCount_raw |= unpackShiftLeft(in_data[2], 2, 0x1C); // Unpacks bits ___###__ from byte 2
    out_msg->BMS_Fault_ChargerReportedErrorCount_value = CAN_DECODE(BMS_Fault_ChargerReportedErrorCount_raw, CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_SCALE, CANSIG_BMS_FAULT_CHARGER_REPORTED_ERROR_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_ChargerDisconnectedDuringChargeCount_raw = 0; // Temporary raw variable for BMS_Fault_ChargerDisconnectedDuringChargeCount
    BMS_Fault_ChargerDisconnectedDuringChargeCount_raw |= unpackShiftLeft(in_data[2], 5, 0xE0); // Unpacks bits ###_____ from byte 2
    out_msg->BMS_Fault_ChargerDisconnectedDuringChargeCount_value = CAN_DECODE(BMS_Fault_ChargerDisconnectedDuringChargeCount_raw, CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_SCALE, CANSIG_BMS_FAULT_CHARGER_DISCONNECTED_DURING_CHARGE_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_ChargerShutdownLoopOpenCount_raw = 0; // Temporary raw variable for BMS_Fault_ChargerShutdownLoopOpenCount
    BMS_Fault_ChargerShutdownLoopOpenCount_raw |= unpackShiftRight(in_data[3], 0, 0x7); // Unpacks bits _____### from byte 3
    out_msg->BMS_Fault_ChargerShutdownLoopOpenCount_value = CAN_DECODE(BMS_Fault_ChargerShutdownLoopOpenCount_raw, CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_SCALE, CANSIG_BMS_FAULT_CHARGER_SHUTDOWN_LOOP_OPEN_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_TractiveSystemOvercurrentCount_raw = 0; // Temporary raw variable for BMS_Fault_TractiveSystemOvercurrentCount
    BMS_Fault_TractiveSystemOvercurrentCount_raw |= unpackShiftLeft(in_data[3], 3, 0x38); // Unpacks bits __###___ from byte 3
    out_msg->BMS_Fault_TractiveSystemOvercurrentCount_value = CAN_DECODE(BMS_Fault_TractiveSystemOvercurrentCount_raw, CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_SCALE, CANSIG_BMS_FAULT_TRACTIVE_SYSTEM_OVERCURRENT_COUNT_OFFSET, uint32_t);
    
    uint32_t BMS_Fault_PrechargeFailureCount_raw = 0; // Temporary raw variable for BMS_Fault_PrechargeFailureCount
    BMS_Fault_PrechargeFailureCount_raw |= unpackShiftLeft(in_data[3], 6, 0xC0); // Unpacks bits ##______ from byte 3
    BMS_Fault_PrechargeFailureCount_raw |= unpackShiftRight(in_data[4], 2, 0x1); // Unpacks bits _______# from byte 4
    out_msg->BMS_Fault_PrechargeFailureCount_value = CAN_DECODE(BMS_Fault_PrechargeFailureCount_raw, CANSIG_BMS_FAULT_PRECHARGE_FAILURE_COUNT_SCALE, CANSIG_BMS_FAULT_PRECHARGE_FAILURE_COUNT_OFFSET, uint32_t);
    
    
}


// unpack function for FSM_Warnings
void app_canUtils_FSM_Warnings_unpack(const uint8_t* const in_data, FSM_Warnings_Signals* const out_msg)
{
    // Unpack message FSM_Warnings.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______RQ|PONMLKJI|HGFEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t FSM_Warning_WatchdogTimeout_raw = 0; // Temporary raw variable for FSM_Warning_WatchdogTimeout
    FSM_Warning_WatchdogTimeout_raw |= unpackShiftRight(in_data[0], 0, 0x1); // Unpacks bits _______# from byte 0
    out_msg->FSM_Warning_WatchdogTimeout_value = CAN_DECODE(FSM_Warning_WatchdogTimeout_raw, CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_SCALE, CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_TxOverflow_raw = 0; // Temporary raw variable for FSM_Warning_TxOverflow
    FSM_Warning_TxOverflow_raw |= unpackShiftLeft(in_data[0], 1, 0x2); // Unpacks bits ______#_ from byte 0
    out_msg->FSM_Warning_TxOverflow_value = CAN_DECODE(FSM_Warning_TxOverflow_raw, CANSIG_FSM_WARNING_TX_OVERFLOW_SCALE, CANSIG_FSM_WARNING_TX_OVERFLOW_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_RxOverflow_raw = 0; // Temporary raw variable for FSM_Warning_RxOverflow
    FSM_Warning_RxOverflow_raw |= unpackShiftLeft(in_data[0], 2, 0x4); // Unpacks bits _____#__ from byte 0
    out_msg->FSM_Warning_RxOverflow_value = CAN_DECODE(FSM_Warning_RxOverflow_raw, CANSIG_FSM_WARNING_RX_OVERFLOW_SCALE, CANSIG_FSM_WARNING_RX_OVERFLOW_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_StackWaterMarkHighTask1Hz_raw = 0; // Temporary raw variable for FSM_Warning_StackWaterMarkHighTask1Hz
    FSM_Warning_StackWaterMarkHighTask1Hz_raw |= unpackShiftLeft(in_data[0], 3, 0x8); // Unpacks bits ____#___ from byte 0
    out_msg->FSM_Warning_StackWaterMarkHighTask1Hz_value = CAN_DECODE(FSM_Warning_StackWaterMarkHighTask1Hz_raw, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_SCALE, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_StackWaterMarkHighTask100Hz_raw = 0; // Temporary raw variable for FSM_Warning_StackWaterMarkHighTask100Hz
    FSM_Warning_StackWaterMarkHighTask100Hz_raw |= unpackShiftLeft(in_data[0], 4, 0x10); // Unpacks bits ___#____ from byte 0
    out_msg->FSM_Warning_StackWaterMarkHighTask100Hz_value = CAN_DECODE(FSM_Warning_StackWaterMarkHighTask100Hz_raw, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_SCALE, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_StackWaterMarkHighTask1kHz_raw = 0; // Temporary raw variable for FSM_Warning_StackWaterMarkHighTask1kHz
    FSM_Warning_StackWaterMarkHighTask1kHz_raw |= unpackShiftLeft(in_data[0], 5, 0x20); // Unpacks bits __#_____ from byte 0
    out_msg->FSM_Warning_StackWaterMarkHighTask1kHz_value = CAN_DECODE(FSM_Warning_StackWaterMarkHighTask1kHz_raw, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_SCALE, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_StackWaterMarkHighTaskCanRx_raw = 0; // Temporary raw variable for FSM_Warning_StackWaterMarkHighTaskCanRx
    FSM_Warning_StackWaterMarkHighTaskCanRx_raw |= unpackShiftLeft(in_data[0], 6, 0x40); // Unpacks bits _#______ from byte 0
    out_msg->FSM_Warning_StackWaterMarkHighTaskCanRx_value = CAN_DECODE(FSM_Warning_StackWaterMarkHighTaskCanRx_raw, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_SCALE, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_StackWaterMarkHighTaskCanTx_raw = 0; // Temporary raw variable for FSM_Warning_StackWaterMarkHighTaskCanTx
    FSM_Warning_StackWaterMarkHighTaskCanTx_raw |= unpackShiftLeft(in_data[0], 7, 0x80); // Unpacks bits #_______ from byte 0
    out_msg->FSM_Warning_StackWaterMarkHighTaskCanTx_value = CAN_DECODE(FSM_Warning_StackWaterMarkHighTaskCanTx_raw, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_SCALE, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_SteeringAngleOCSC_raw = 0; // Temporary raw variable for FSM_Warning_SteeringAngleOCSC
    FSM_Warning_SteeringAngleOCSC_raw |= unpackShiftRight(in_data[1], 0, 0x1); // Unpacks bits _______# from byte 1
    out_msg->FSM_Warning_SteeringAngleOCSC_value = CAN_DECODE(FSM_Warning_SteeringAngleOCSC_raw, CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_SCALE, CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_SteeringAngleOutOfRange_raw = 0; // Temporary raw variable for FSM_Warning_SteeringAngleOutOfRange
    FSM_Warning_SteeringAngleOutOfRange_raw |= unpackShiftLeft(in_data[1], 1, 0x2); // Unpacks bits ______#_ from byte 1
    out_msg->FSM_Warning_SteeringAngleOutOfRange_value = CAN_DECODE(FSM_Warning_SteeringAngleOutOfRange_raw, CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_LeftWheelSpeedOutOfRange_raw = 0; // Temporary raw variable for FSM_Warning_LeftWheelSpeedOutOfRange
    FSM_Warning_LeftWheelSpeedOutOfRange_raw |= unpackShiftLeft(in_data[1], 2, 0x4); // Unpacks bits _____#__ from byte 1
    out_msg->FSM_Warning_LeftWheelSpeedOutOfRange_value = CAN_DECODE(FSM_Warning_LeftWheelSpeedOutOfRange_raw, CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_RightWheelSpeedOutOfRange_raw = 0; // Temporary raw variable for FSM_Warning_RightWheelSpeedOutOfRange
    FSM_Warning_RightWheelSpeedOutOfRange_raw |= unpackShiftLeft(in_data[1], 3, 0x8); // Unpacks bits ____#___ from byte 1
    out_msg->FSM_Warning_RightWheelSpeedOutOfRange_value = CAN_DECODE(FSM_Warning_RightWheelSpeedOutOfRange_raw, CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_AppsDisagreement_raw = 0; // Temporary raw variable for FSM_Warning_AppsDisagreement
    FSM_Warning_AppsDisagreement_raw |= unpackShiftLeft(in_data[1], 4, 0x10); // Unpacks bits ___#____ from byte 1
    out_msg->FSM_Warning_AppsDisagreement_value = CAN_DECODE(FSM_Warning_AppsDisagreement_raw, CANSIG_FSM_WARNING_APPS_DISAGREEMENT_SCALE, CANSIG_FSM_WARNING_APPS_DISAGREEMENT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_FrontBrakePressureOutOfRange_raw = 0; // Temporary raw variable for FSM_Warning_FrontBrakePressureOutOfRange
    FSM_Warning_FrontBrakePressureOutOfRange_raw |= unpackShiftLeft(in_data[1], 5, 0x20); // Unpacks bits __#_____ from byte 1
    out_msg->FSM_Warning_FrontBrakePressureOutOfRange_value = CAN_DECODE(FSM_Warning_FrontBrakePressureOutOfRange_raw, CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_RearBrakePressureOutOfRange_raw = 0; // Temporary raw variable for FSM_Warning_RearBrakePressureOutOfRange
    FSM_Warning_RearBrakePressureOutOfRange_raw |= unpackShiftLeft(in_data[1], 6, 0x40); // Unpacks bits _#______ from byte 1
    out_msg->FSM_Warning_RearBrakePressureOutOfRange_value = CAN_DECODE(FSM_Warning_RearBrakePressureOutOfRange_raw, CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_SCALE, CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_FrontBrakePressureOcSc_raw = 0; // Temporary raw variable for FSM_Warning_FrontBrakePressureOcSc
    FSM_Warning_FrontBrakePressureOcSc_raw |= unpackShiftLeft(in_data[1], 7, 0x80); // Unpacks bits #_______ from byte 1
    out_msg->FSM_Warning_FrontBrakePressureOcSc_value = CAN_DECODE(FSM_Warning_FrontBrakePressureOcSc_raw, CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_SCALE, CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_RearBrakePressureOcSc_raw = 0; // Temporary raw variable for FSM_Warning_RearBrakePressureOcSc
    FSM_Warning_RearBrakePressureOcSc_raw |= unpackShiftRight(in_data[2], 0, 0x1); // Unpacks bits _______# from byte 2
    out_msg->FSM_Warning_RearBrakePressureOcSc_value = CAN_DECODE(FSM_Warning_RearBrakePressureOcSc_raw, CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_SCALE, CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_BrakeOcScNotOk_raw = 0; // Temporary raw variable for FSM_Warning_BrakeOcScNotOk
    FSM_Warning_BrakeOcScNotOk_raw |= unpackShiftLeft(in_data[2], 1, 0x2); // Unpacks bits ______#_ from byte 2
    out_msg->FSM_Warning_BrakeOcScNotOk_value = CAN_DECODE(FSM_Warning_BrakeOcScNotOk_raw, CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_SCALE, CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_OFFSET, uint32_t);
    
    
}


// unpack function for FSM_Faults
void app_canUtils_FSM_Faults_unpack(const uint8_t* const in_data, FSM_Faults_Signals* const out_msg)
{
    // Unpack message FSM_Faults.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t FSM_Fault_PappsOCSC_raw = 0; // Temporary raw variable for FSM_Fault_PappsOCSC
    FSM_Fault_PappsOCSC_raw |= unpackShiftRight(in_data[0], 0, 0x1); // Unpacks bits _______# from byte 0
    out_msg->FSM_Fault_PappsOCSC_value = CAN_DECODE(FSM_Fault_PappsOCSC_raw, CANSIG_FSM_FAULT_PAPPS_OCSC_SCALE, CANSIG_FSM_FAULT_PAPPS_OCSC_OFFSET, uint32_t);
    
    uint32_t FSM_Fault_SappsOCSC_raw = 0; // Temporary raw variable for FSM_Fault_SappsOCSC
    FSM_Fault_SappsOCSC_raw |= unpackShiftLeft(in_data[0], 1, 0x2); // Unpacks bits ______#_ from byte 0
    out_msg->FSM_Fault_SappsOCSC_value = CAN_DECODE(FSM_Fault_SappsOCSC_raw, CANSIG_FSM_FAULT_SAPPS_OCSC_SCALE, CANSIG_FSM_FAULT_SAPPS_OCSC_OFFSET, uint32_t);
    
    
}


// unpack function for FSM_WarningsCounts
void app_canUtils_FSM_WarningsCounts_unpack(const uint8_t* const in_data, FSM_WarningsCounts_Signals* const out_msg)
{
    // Unpack message FSM_WarningsCounts.
    // |xxxxxxxx|__RRRQQQ|PPPOOONN|NMMMLLLK|KKJJJIII|HHHGGGFF|FEEEDDDC|CCBBBAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t FSM_Warning_WatchdogTimeoutCount_raw = 0; // Temporary raw variable for FSM_Warning_WatchdogTimeoutCount
    FSM_Warning_WatchdogTimeoutCount_raw |= unpackShiftRight(in_data[0], 0, 0x7); // Unpacks bits _____### from byte 0
    out_msg->FSM_Warning_WatchdogTimeoutCount_value = CAN_DECODE(FSM_Warning_WatchdogTimeoutCount_raw, CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_SCALE, CANSIG_FSM_WARNING_WATCHDOG_TIMEOUT_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_TxOverflowCount_raw = 0; // Temporary raw variable for FSM_Warning_TxOverflowCount
    FSM_Warning_TxOverflowCount_raw |= unpackShiftLeft(in_data[0], 3, 0x38); // Unpacks bits __###___ from byte 0
    out_msg->FSM_Warning_TxOverflowCount_value = CAN_DECODE(FSM_Warning_TxOverflowCount_raw, CANSIG_FSM_WARNING_TX_OVERFLOW_COUNT_SCALE, CANSIG_FSM_WARNING_TX_OVERFLOW_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_RxOverflowCount_raw = 0; // Temporary raw variable for FSM_Warning_RxOverflowCount
    FSM_Warning_RxOverflowCount_raw |= unpackShiftLeft(in_data[0], 6, 0xC0); // Unpacks bits ##______ from byte 0
    FSM_Warning_RxOverflowCount_raw |= unpackShiftRight(in_data[1], 2, 0x1); // Unpacks bits _______# from byte 1
    out_msg->FSM_Warning_RxOverflowCount_value = CAN_DECODE(FSM_Warning_RxOverflowCount_raw, CANSIG_FSM_WARNING_RX_OVERFLOW_COUNT_SCALE, CANSIG_FSM_WARNING_RX_OVERFLOW_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_StackWaterMarkHighTask1HzCount_raw = 0; // Temporary raw variable for FSM_Warning_StackWaterMarkHighTask1HzCount
    FSM_Warning_StackWaterMarkHighTask1HzCount_raw |= unpackShiftLeft(in_data[1], 1, 0xE); // Unpacks bits ____###_ from byte 1
    out_msg->FSM_Warning_StackWaterMarkHighTask1HzCount_value = CAN_DECODE(FSM_Warning_StackWaterMarkHighTask1HzCount_raw, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_SCALE, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1_HZ_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_StackWaterMarkHighTask100HzCount_raw = 0; // Temporary raw variable for FSM_Warning_StackWaterMarkHighTask100HzCount
    FSM_Warning_StackWaterMarkHighTask100HzCount_raw |= unpackShiftLeft(in_data[1], 4, 0x70); // Unpacks bits _###____ from byte 1
    out_msg->FSM_Warning_StackWaterMarkHighTask100HzCount_value = CAN_DECODE(FSM_Warning_StackWaterMarkHighTask100HzCount_raw, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_SCALE, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK100_HZ_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_StackWaterMarkHighTask1kHzCount_raw = 0; // Temporary raw variable for FSM_Warning_StackWaterMarkHighTask1kHzCount
    FSM_Warning_StackWaterMarkHighTask1kHzCount_raw |= unpackShiftLeft(in_data[1], 7, 0x80); // Unpacks bits #_______ from byte 1
    FSM_Warning_StackWaterMarkHighTask1kHzCount_raw |= unpackShiftRight(in_data[2], 1, 0x3); // Unpacks bits ______## from byte 2
    out_msg->FSM_Warning_StackWaterMarkHighTask1kHzCount_value = CAN_DECODE(FSM_Warning_StackWaterMarkHighTask1kHzCount_raw, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_SCALE, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK1K_HZ_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_StackWaterMarkHighTaskCanRxCount_raw = 0; // Temporary raw variable for FSM_Warning_StackWaterMarkHighTaskCanRxCount
    FSM_Warning_StackWaterMarkHighTaskCanRxCount_raw |= unpackShiftLeft(in_data[2], 2, 0x1C); // Unpacks bits ___###__ from byte 2
    out_msg->FSM_Warning_StackWaterMarkHighTaskCanRxCount_value = CAN_DECODE(FSM_Warning_StackWaterMarkHighTaskCanRxCount_raw, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_SCALE, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_RX_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_StackWaterMarkHighTaskCanTxCount_raw = 0; // Temporary raw variable for FSM_Warning_StackWaterMarkHighTaskCanTxCount
    FSM_Warning_StackWaterMarkHighTaskCanTxCount_raw |= unpackShiftLeft(in_data[2], 5, 0xE0); // Unpacks bits ###_____ from byte 2
    out_msg->FSM_Warning_StackWaterMarkHighTaskCanTxCount_value = CAN_DECODE(FSM_Warning_StackWaterMarkHighTaskCanTxCount_raw, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_SCALE, CANSIG_FSM_WARNING_STACK_WATER_MARK_HIGH_TASK_CAN_TX_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_SteeringAngleOCSCCount_raw = 0; // Temporary raw variable for FSM_Warning_SteeringAngleOCSCCount
    FSM_Warning_SteeringAngleOCSCCount_raw |= unpackShiftRight(in_data[3], 0, 0x7); // Unpacks bits _____### from byte 3
    out_msg->FSM_Warning_SteeringAngleOCSCCount_value = CAN_DECODE(FSM_Warning_SteeringAngleOCSCCount_raw, CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_SCALE, CANSIG_FSM_WARNING_STEERING_ANGLE_OCSC_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_SteeringAngleOutOfRangeCount_raw = 0; // Temporary raw variable for FSM_Warning_SteeringAngleOutOfRangeCount
    FSM_Warning_SteeringAngleOutOfRangeCount_raw |= unpackShiftLeft(in_data[3], 3, 0x38); // Unpacks bits __###___ from byte 3
    out_msg->FSM_Warning_SteeringAngleOutOfRangeCount_value = CAN_DECODE(FSM_Warning_SteeringAngleOutOfRangeCount_raw, CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_SCALE, CANSIG_FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_LeftWheelSpeedOutOfRangeCount_raw = 0; // Temporary raw variable for FSM_Warning_LeftWheelSpeedOutOfRangeCount
    FSM_Warning_LeftWheelSpeedOutOfRangeCount_raw |= unpackShiftLeft(in_data[3], 6, 0xC0); // Unpacks bits ##______ from byte 3
    FSM_Warning_LeftWheelSpeedOutOfRangeCount_raw |= unpackShiftRight(in_data[4], 2, 0x1); // Unpacks bits _______# from byte 4
    out_msg->FSM_Warning_LeftWheelSpeedOutOfRangeCount_value = CAN_DECODE(FSM_Warning_LeftWheelSpeedOutOfRangeCount_raw, CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_SCALE, CANSIG_FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_RightWheelSpeedOutOfRangeCount_raw = 0; // Temporary raw variable for FSM_Warning_RightWheelSpeedOutOfRangeCount
    FSM_Warning_RightWheelSpeedOutOfRangeCount_raw |= unpackShiftLeft(in_data[4], 1, 0xE); // Unpacks bits ____###_ from byte 4
    out_msg->FSM_Warning_RightWheelSpeedOutOfRangeCount_value = CAN_DECODE(FSM_Warning_RightWheelSpeedOutOfRangeCount_raw, CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_SCALE, CANSIG_FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_AppsDisagreementCount_raw = 0; // Temporary raw variable for FSM_Warning_AppsDisagreementCount
    FSM_Warning_AppsDisagreementCount_raw |= unpackShiftLeft(in_data[4], 4, 0x70); // Unpacks bits _###____ from byte 4
    out_msg->FSM_Warning_AppsDisagreementCount_value = CAN_DECODE(FSM_Warning_AppsDisagreementCount_raw, CANSIG_FSM_WARNING_APPS_DISAGREEMENT_COUNT_SCALE, CANSIG_FSM_WARNING_APPS_DISAGREEMENT_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_FrontBrakePressureOutOfRangeCount_raw = 0; // Temporary raw variable for FSM_Warning_FrontBrakePressureOutOfRangeCount
    FSM_Warning_FrontBrakePressureOutOfRangeCount_raw |= unpackShiftLeft(in_data[4], 7, 0x80); // Unpacks bits #_______ from byte 4
    FSM_Warning_FrontBrakePressureOutOfRangeCount_raw |= unpackShiftRight(in_data[5], 1, 0x3); // Unpacks bits ______## from byte 5
    out_msg->FSM_Warning_FrontBrakePressureOutOfRangeCount_value = CAN_DECODE(FSM_Warning_FrontBrakePressureOutOfRangeCount_raw, CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_SCALE, CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_RearBrakePressureOutOfRangeCount_raw = 0; // Temporary raw variable for FSM_Warning_RearBrakePressureOutOfRangeCount
    FSM_Warning_RearBrakePressureOutOfRangeCount_raw |= unpackShiftLeft(in_data[5], 2, 0x1C); // Unpacks bits ___###__ from byte 5
    out_msg->FSM_Warning_RearBrakePressureOutOfRangeCount_value = CAN_DECODE(FSM_Warning_RearBrakePressureOutOfRangeCount_raw, CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_SCALE, CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OUT_OF_RANGE_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_FrontBrakePressureOcScCount_raw = 0; // Temporary raw variable for FSM_Warning_FrontBrakePressureOcScCount
    FSM_Warning_FrontBrakePressureOcScCount_raw |= unpackShiftLeft(in_data[5], 5, 0xE0); // Unpacks bits ###_____ from byte 5
    out_msg->FSM_Warning_FrontBrakePressureOcScCount_value = CAN_DECODE(FSM_Warning_FrontBrakePressureOcScCount_raw, CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_COUNT_SCALE, CANSIG_FSM_WARNING_FRONT_BRAKE_PRESSURE_OC_SC_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_RearBrakePressureOcScCount_raw = 0; // Temporary raw variable for FSM_Warning_RearBrakePressureOcScCount
    FSM_Warning_RearBrakePressureOcScCount_raw |= unpackShiftRight(in_data[6], 0, 0x7); // Unpacks bits _____### from byte 6
    out_msg->FSM_Warning_RearBrakePressureOcScCount_value = CAN_DECODE(FSM_Warning_RearBrakePressureOcScCount_raw, CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_COUNT_SCALE, CANSIG_FSM_WARNING_REAR_BRAKE_PRESSURE_OC_SC_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Warning_BrakeOcScNotOkCount_raw = 0; // Temporary raw variable for FSM_Warning_BrakeOcScNotOkCount
    FSM_Warning_BrakeOcScNotOkCount_raw |= unpackShiftLeft(in_data[6], 3, 0x38); // Unpacks bits __###___ from byte 6
    out_msg->FSM_Warning_BrakeOcScNotOkCount_value = CAN_DECODE(FSM_Warning_BrakeOcScNotOkCount_raw, CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_COUNT_SCALE, CANSIG_FSM_WARNING_BRAKE_OC_SC_NOT_OK_COUNT_OFFSET, uint32_t);
    
    
}


// unpack function for FSM_FaultsCounts
void app_canUtils_FSM_FaultsCounts_unpack(const uint8_t* const in_data, FSM_FaultsCounts_Signals* const out_msg)
{
    // Unpack message FSM_FaultsCounts.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__BBBAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t FSM_Fault_PappsOCSCCount_raw = 0; // Temporary raw variable for FSM_Fault_PappsOCSCCount
    FSM_Fault_PappsOCSCCount_raw |= unpackShiftRight(in_data[0], 0, 0x7); // Unpacks bits _____### from byte 0
    out_msg->FSM_Fault_PappsOCSCCount_value = CAN_DECODE(FSM_Fault_PappsOCSCCount_raw, CANSIG_FSM_FAULT_PAPPS_OCSC_COUNT_SCALE, CANSIG_FSM_FAULT_PAPPS_OCSC_COUNT_OFFSET, uint32_t);
    
    uint32_t FSM_Fault_SappsOCSCCount_raw = 0; // Temporary raw variable for FSM_Fault_SappsOCSCCount
    FSM_Fault_SappsOCSCCount_raw |= unpackShiftLeft(in_data[0], 3, 0x38); // Unpacks bits __###___ from byte 0
    out_msg->FSM_Fault_SappsOCSCCount_value = CAN_DECODE(FSM_Fault_SappsOCSCCount_raw, CANSIG_FSM_FAULT_SAPPS_OCSC_COUNT_SCALE, CANSIG_FSM_FAULT_SAPPS_OCSC_COUNT_OFFSET, uint32_t);
    
    
}


// unpack function for BMS_TractiveSystem
void app_canUtils_BMS_TractiveSystem_unpack(const uint8_t* const in_data, BMS_TractiveSystem_Signals* const out_msg)
{
    // Unpack message BMS_TractiveSystem.
    // |xxxxxxxx|_EDDDDDD|DDDDDDDD|DDDCCCCC|CCCCCCBB|BBBBBBBB|BBBAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t BMS_TractiveSystemVoltage_raw = 0; // Temporary raw variable for BMS_TractiveSystemVoltage
    BMS_TractiveSystemVoltage_raw |= unpackShiftRight(in_data[0], 0, 0xFF); // Unpacks bits ######## from byte 0
    BMS_TractiveSystemVoltage_raw |= unpackShiftRight(in_data[1], 8, 0x1F); // Unpacks bits ___##### from byte 1
    out_msg->BMS_TractiveSystemVoltage_value = CAN_DECODE(BMS_TractiveSystemVoltage_raw, CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_OFFSET, float);
    
    uint32_t BMS_TractiveSystemCurrent_raw = 0; // Temporary raw variable for BMS_TractiveSystemCurrent
    BMS_TractiveSystemCurrent_raw |= unpackShiftLeft(in_data[1], 5, 0xE0); // Unpacks bits ###_____ from byte 1
    BMS_TractiveSystemCurrent_raw |= unpackShiftRight(in_data[2], 3, 0xFF); // Unpacks bits ######## from byte 2
    BMS_TractiveSystemCurrent_raw |= unpackShiftRight(in_data[3], 11, 0x3); // Unpacks bits ______## from byte 3
    out_msg->BMS_TractiveSystemCurrent_value = CAN_DECODE(BMS_TractiveSystemCurrent_raw, CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_OFFSET, float);
    
    uint32_t BMS_TractiveSystemPower_raw = 0; // Temporary raw variable for BMS_TractiveSystemPower
    BMS_TractiveSystemPower_raw |= unpackShiftLeft(in_data[3], 2, 0xFC); // Unpacks bits ######__ from byte 3
    BMS_TractiveSystemPower_raw |= unpackShiftRight(in_data[4], 6, 0x1F); // Unpacks bits ___##### from byte 4
    out_msg->BMS_TractiveSystemPower_value = CAN_DECODE(BMS_TractiveSystemPower_raw, CANSIG_BMS_TRACTIVE_SYSTEM_POWER_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_POWER_OFFSET, float);
    
    uint32_t BMS_AvailablePower_raw = 0; // Temporary raw variable for BMS_AvailablePower
    BMS_AvailablePower_raw |= unpackShiftLeft(in_data[4], 5, 0xE0); // Unpacks bits ###_____ from byte 4
    BMS_AvailablePower_raw |= unpackShiftRight(in_data[5], 3, 0xFF); // Unpacks bits ######## from byte 5
    BMS_AvailablePower_raw |= unpackShiftRight(in_data[6], 11, 0x3F); // Unpacks bits __###### from byte 6
    out_msg->BMS_AvailablePower_value = CAN_DECODE(BMS_AvailablePower_raw, CANSIG_BMS_AVAILABLE_POWER_SCALE, CANSIG_BMS_AVAILABLE_POWER_OFFSET, uint32_t);
    
    uint32_t BMS_BSPDCurrentThresholdExceeded_raw = 0; // Temporary raw variable for BMS_BSPDCurrentThresholdExceeded
    BMS_BSPDCurrentThresholdExceeded_raw |= unpackShiftLeft(in_data[6], 6, 0x40); // Unpacks bits _#______ from byte 6
    out_msg->BMS_BSPDCurrentThresholdExceeded_value = CAN_DECODE(BMS_BSPDCurrentThresholdExceeded_raw, CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_SCALE, CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_OFFSET, uint32_t);
    
    
}


// unpack function for BMS_BMSShdnNodeStatus
void app_canUtils_BMS_BMSShdnNodeStatus_unpack(const uint8_t* const in_data, BMS_BMSShdnNodeStatus_Signals* const out_msg)
{
    // Unpack message BMS_BMSShdnNodeStatus.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|______BA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t BMS_HVDShdnOKStatus_raw = 0; // Temporary raw variable for BMS_HVDShdnOKStatus
    BMS_HVDShdnOKStatus_raw |= unpackShiftRight(in_data[0], 0, 0x1); // Unpacks bits _______# from byte 0
    out_msg->BMS_HVDShdnOKStatus_value = CAN_DECODE(BMS_HVDShdnOKStatus_raw, CANSIG_BMS_HVD_SHDN_OK_STATUS_SCALE, CANSIG_BMS_HVD_SHDN_OK_STATUS_OFFSET, uint32_t);
    
    uint32_t BMS_TSIlckOKStatus_raw = 0; // Temporary raw variable for BMS_TSIlckOKStatus
    BMS_TSIlckOKStatus_raw |= unpackShiftLeft(in_data[0], 1, 0x2); // Unpacks bits ______#_ from byte 0
    out_msg->BMS_TSIlckOKStatus_value = CAN_DECODE(BMS_TSIlckOKStatus_raw, CANSIG_BMS_TS_ILCK_OK_STATUS_SCALE, CANSIG_BMS_TS_ILCK_OK_STATUS_OFFSET, uint32_t);
    
    
}


// unpack function for BMS_CellTemperatures
void app_canUtils_BMS_CellTemperatures_unpack(const uint8_t* const in_data, BMS_CellTemperatures_Signals* const out_msg)
{
    // Unpack message BMS_CellTemperatures.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____BBBB|BBBBBBAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t BMS_MinCellTemperature_raw = 0; // Temporary raw variable for BMS_MinCellTemperature
    BMS_MinCellTemperature_raw |= unpackShiftRight(in_data[0], 0, 0xFF); // Unpacks bits ######## from byte 0
    BMS_MinCellTemperature_raw |= unpackShiftRight(in_data[1], 8, 0x3); // Unpacks bits ______## from byte 1
    out_msg->BMS_MinCellTemperature_value = CAN_DECODE(BMS_MinCellTemperature_raw, CANSIG_BMS_MIN_CELL_TEMPERATURE_SCALE, CANSIG_BMS_MIN_CELL_TEMPERATURE_OFFSET, float);
    
    uint32_t BMS_MaxCellTemperature_raw = 0; // Temporary raw variable for BMS_MaxCellTemperature
    BMS_MaxCellTemperature_raw |= unpackShiftLeft(in_data[1], 2, 0xFC); // Unpacks bits ######__ from byte 1
    BMS_MaxCellTemperature_raw |= unpackShiftRight(in_data[2], 6, 0xF); // Unpacks bits ____#### from byte 2
    out_msg->BMS_MaxCellTemperature_value = CAN_DECODE(BMS_MaxCellTemperature_raw, CANSIG_BMS_MAX_CELL_TEMPERATURE_SCALE, CANSIG_BMS_MAX_CELL_TEMPERATURE_OFFSET, float);
    
    
}


// unpack function for BMS_VoltageAndChargeStats
void app_canUtils_BMS_VoltageAndChargeStats_unpack(const uint8_t* const in_data, BMS_VoltageAndChargeStats_Signals* const out_msg)
{
    // Unpack message BMS_VoltageAndChargeStats.
    // |xxxxxxxx|___EEEEE|EEEEEEEE|DDDDDDDD|DDDDDCBB|BBBBBBBB|AAAAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t BMS_PackVoltage_raw = 0; // Temporary raw variable for BMS_PackVoltage
    BMS_PackVoltage_raw |= unpackShiftRight(in_data[0], 0, 0xFF); // Unpacks bits ######## from byte 0
    BMS_PackVoltage_raw |= unpackShiftRight(in_data[1], 8, 0xFF); // Unpacks bits ######## from byte 1
    out_msg->BMS_PackVoltage_value = CAN_DECODE(BMS_PackVoltage_raw, CANSIG_BMS_PACK_VOLTAGE_SCALE, CANSIG_BMS_PACK_VOLTAGE_OFFSET, float);
    
    uint32_t BMS_Soc_raw = 0; // Temporary raw variable for BMS_Soc
    BMS_Soc_raw |= unpackShiftRight(in_data[2], 0, 0xFF); // Unpacks bits ######## from byte 2
    BMS_Soc_raw |= unpackShiftRight(in_data[3], 8, 0x3); // Unpacks bits ______## from byte 3
    out_msg->BMS_Soc_value = CAN_DECODE(BMS_Soc_raw, CANSIG_BMS_SOC_SCALE, CANSIG_BMS_SOC_OFFSET, float);
    
    uint32_t BMS_SocCorrupt_raw = 0; // Temporary raw variable for BMS_SocCorrupt
    BMS_SocCorrupt_raw |= unpackShiftLeft(in_data[3], 2, 0x4); // Unpacks bits _____#__ from byte 3
    out_msg->BMS_SocCorrupt_value = CAN_DECODE(BMS_SocCorrupt_raw, CANSIG_BMS_SOC_CORRUPT_SCALE, CANSIG_BMS_SOC_CORRUPT_OFFSET, uint32_t);
    
    uint32_t BMS_MinCellVoltage_raw = 0; // Temporary raw variable for BMS_MinCellVoltage
    BMS_MinCellVoltage_raw |= unpackShiftLeft(in_data[3], 3, 0xF8); // Unpacks bits #####___ from byte 3
    BMS_MinCellVoltage_raw |= unpackShiftRight(in_data[4], 5, 0xFF); // Unpacks bits ######## from byte 4
    out_msg->BMS_MinCellVoltage_value = CAN_DECODE(BMS_MinCellVoltage_raw, CANSIG_BMS_MIN_CELL_VOLTAGE_SCALE, CANSIG_BMS_MIN_CELL_VOLTAGE_OFFSET, float);
    
    uint32_t BMS_MaxCellVoltage_raw = 0; // Temporary raw variable for BMS_MaxCellVoltage
    BMS_MaxCellVoltage_raw |= unpackShiftRight(in_data[5], 0, 0xFF); // Unpacks bits ######## from byte 5
    BMS_MaxCellVoltage_raw |= unpackShiftRight(in_data[6], 8, 0x1F); // Unpacks bits ___##### from byte 6
    out_msg->BMS_MaxCellVoltage_value = CAN_DECODE(BMS_MaxCellVoltage_raw, CANSIG_BMS_MAX_CELL_VOLTAGE_SCALE, CANSIG_BMS_MAX_CELL_VOLTAGE_OFFSET, float);
    
    
}


// unpack function for BMS_OkStatuses
void app_canUtils_BMS_OkStatuses_unpack(const uint8_t* const in_data, BMS_OkStatuses_Signals* const out_msg)
{
    // Unpack message BMS_OkStatuses.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|__FEDCBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t BMS_BmsOk_raw = 0; // Temporary raw variable for BMS_BmsOk
    BMS_BmsOk_raw |= unpackShiftRight(in_data[0], 0, 0x1); // Unpacks bits _______# from byte 0
    out_msg->BMS_BmsOk_value = CAN_DECODE(BMS_BmsOk_raw, CANSIG_BMS_BMS_OK_SCALE, CANSIG_BMS_BMS_OK_OFFSET, uint32_t);
    
    uint32_t BMS_ImdOk_raw = 0; // Temporary raw variable for BMS_ImdOk
    BMS_ImdOk_raw |= unpackShiftLeft(in_data[0], 1, 0x2); // Unpacks bits ______#_ from byte 0
    out_msg->BMS_ImdOk_value = CAN_DECODE(BMS_ImdOk_raw, CANSIG_BMS_IMD_OK_SCALE, CANSIG_BMS_IMD_OK_OFFSET, uint32_t);
    
    uint32_t BMS_BspdOk_raw = 0; // Temporary raw variable for BMS_BspdOk
    BMS_BspdOk_raw |= unpackShiftLeft(in_data[0], 2, 0x4); // Unpacks bits _____#__ from byte 0
    out_msg->BMS_BspdOk_value = CAN_DECODE(BMS_BspdOk_raw, CANSIG_BMS_BSPD_OK_SCALE, CANSIG_BMS_BSPD_OK_OFFSET, uint32_t);
    
    uint32_t BMS_BmsLatchedFault_raw = 0; // Temporary raw variable for BMS_BmsLatchedFault
    BMS_BmsLatchedFault_raw |= unpackShiftLeft(in_data[0], 3, 0x8); // Unpacks bits ____#___ from byte 0
    out_msg->BMS_BmsLatchedFault_value = CAN_DECODE(BMS_BmsLatchedFault_raw, CANSIG_BMS_BMS_LATCHED_FAULT_SCALE, CANSIG_BMS_BMS_LATCHED_FAULT_OFFSET, uint32_t);
    
    uint32_t BMS_ImdLatchedFault_raw = 0; // Temporary raw variable for BMS_ImdLatchedFault
    BMS_ImdLatchedFault_raw |= unpackShiftLeft(in_data[0], 4, 0x10); // Unpacks bits ___#____ from byte 0
    out_msg->BMS_ImdLatchedFault_value = CAN_DECODE(BMS_ImdLatchedFault_raw, CANSIG_BMS_IMD_LATCHED_FAULT_SCALE, CANSIG_BMS_IMD_LATCHED_FAULT_OFFSET, uint32_t);
    
    uint32_t BMS_BspdLatchedFault_raw = 0; // Temporary raw variable for BMS_BspdLatchedFault
    BMS_BspdLatchedFault_raw |= unpackShiftLeft(in_data[0], 5, 0x20); // Unpacks bits __#_____ from byte 0
    out_msg->BMS_BspdLatchedFault_value = CAN_DECODE(BMS_BspdLatchedFault_raw, CANSIG_BMS_BSPD_LATCHED_FAULT_SCALE, CANSIG_BMS_BSPD_LATCHED_FAULT_OFFSET, uint32_t);
    
    
}


// unpack function for BMS_TractiveSystem
void app_canUtils_BMS_TractiveSystem_unpack(const uint8_t* const in_data, BMS_TractiveSystem_Signals* const out_msg)
{
    // Unpack message BMS_TractiveSystem.
    // |xxxxxxxx|_EDDDDDD|DDDDDDDD|DDDCCCCC|CCCCCCBB|BBBBBBBB|BBBAAAAA|AAAAAAAA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t BMS_TractiveSystemVoltage_raw = 0; // Temporary raw variable for BMS_TractiveSystemVoltage
    BMS_TractiveSystemVoltage_raw |= unpackShiftRight(in_data[0], 0, 0xFF); // Unpacks bits ######## from byte 0
    BMS_TractiveSystemVoltage_raw |= unpackShiftRight(in_data[1], 8, 0x1F); // Unpacks bits ___##### from byte 1
    out_msg->BMS_TractiveSystemVoltage_value = CAN_DECODE(BMS_TractiveSystemVoltage_raw, CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_VOLTAGE_OFFSET, float);
    
    uint32_t BMS_TractiveSystemCurrent_raw = 0; // Temporary raw variable for BMS_TractiveSystemCurrent
    BMS_TractiveSystemCurrent_raw |= unpackShiftLeft(in_data[1], 5, 0xE0); // Unpacks bits ###_____ from byte 1
    BMS_TractiveSystemCurrent_raw |= unpackShiftRight(in_data[2], 3, 0xFF); // Unpacks bits ######## from byte 2
    BMS_TractiveSystemCurrent_raw |= unpackShiftRight(in_data[3], 11, 0x3); // Unpacks bits ______## from byte 3
    out_msg->BMS_TractiveSystemCurrent_value = CAN_DECODE(BMS_TractiveSystemCurrent_raw, CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_CURRENT_OFFSET, float);
    
    uint32_t BMS_TractiveSystemPower_raw = 0; // Temporary raw variable for BMS_TractiveSystemPower
    BMS_TractiveSystemPower_raw |= unpackShiftLeft(in_data[3], 2, 0xFC); // Unpacks bits ######__ from byte 3
    BMS_TractiveSystemPower_raw |= unpackShiftRight(in_data[4], 6, 0x1F); // Unpacks bits ___##### from byte 4
    out_msg->BMS_TractiveSystemPower_value = CAN_DECODE(BMS_TractiveSystemPower_raw, CANSIG_BMS_TRACTIVE_SYSTEM_POWER_SCALE, CANSIG_BMS_TRACTIVE_SYSTEM_POWER_OFFSET, float);
    
    uint32_t BMS_AvailablePower_raw = 0; // Temporary raw variable for BMS_AvailablePower
    BMS_AvailablePower_raw |= unpackShiftLeft(in_data[4], 5, 0xE0); // Unpacks bits ###_____ from byte 4
    BMS_AvailablePower_raw |= unpackShiftRight(in_data[5], 3, 0xFF); // Unpacks bits ######## from byte 5
    BMS_AvailablePower_raw |= unpackShiftRight(in_data[6], 11, 0x3F); // Unpacks bits __###### from byte 6
    out_msg->BMS_AvailablePower_value = CAN_DECODE(BMS_AvailablePower_raw, CANSIG_BMS_AVAILABLE_POWER_SCALE, CANSIG_BMS_AVAILABLE_POWER_OFFSET, uint32_t);
    
    uint32_t BMS_BSPDCurrentThresholdExceeded_raw = 0; // Temporary raw variable for BMS_BSPDCurrentThresholdExceeded
    BMS_BSPDCurrentThresholdExceeded_raw |= unpackShiftLeft(in_data[6], 6, 0x40); // Unpacks bits _#______ from byte 6
    out_msg->BMS_BSPDCurrentThresholdExceeded_value = CAN_DECODE(BMS_BSPDCurrentThresholdExceeded_raw, CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_SCALE, CANSIG_BMS_BSPD_CURRENT_THRESHOLD_EXCEEDED_OFFSET, uint32_t);
    
    
}


// unpack function for BMS_Vitals
void app_canUtils_BMS_Vitals_unpack(const uint8_t* const in_data, BMS_Vitals_Signals* const out_msg)
{
    // Unpack message BMS_Vitals.
    // |xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|xxxxxxxx|____BBBA|
    
    if (in_data == NULL || out_msg == NULL)
    {
        return;
    }
    
    uint32_t BMS_Heartbeat_raw = 0; // Temporary raw variable for BMS_Heartbeat
    BMS_Heartbeat_raw |= unpackShiftRight(in_data[0], 0, 0x1); // Unpacks bits _______# from byte 0
    out_msg->BMS_Heartbeat_value = CAN_DECODE(BMS_Heartbeat_raw, CANSIG_BMS_HEARTBEAT_SCALE, CANSIG_BMS_HEARTBEAT_OFFSET, uint32_t);
    
    uint32_t BMS_State_raw = 0; // Temporary raw variable for BMS_State
    BMS_State_raw |= unpackShiftLeft(in_data[0], 1, 0xE); // Unpacks bits ____###_ from byte 0
    out_msg->BMS_State_value = CAN_DECODE(BMS_State_raw, CANSIG_BMS_STATE_SCALE, CANSIG_BMS_STATE_OFFSET, uint32_t);
    
    
}
