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
