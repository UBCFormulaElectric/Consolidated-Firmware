

#pragma once
#include <cstdint>

// MVolateg Measurement
typedef enum 
{
    VOLTAGE,
    CURRENT
} Measurement;

constexpr uint16_t CELL1_MV = 0x14;
constexpr uint16_t CELL2_MV = 0x16;
constexpr uint16_t CELL3_MV = 0x1A;
constexpr uint16_t CELL4_MV = 0x1C;
constexpr uint16_t PACK_MV  = 0x36;
constexpr uint16_t LOAD_MV  = 0x38;

// Internal regulator adresses
constexpr uint16_t REG0_CONFIG = 0x9237;
constexpr uint16_t REG1_CONFIG = 0x9236;

// Multifunction Pin Controls
constexpr uint16_t ALERT = 0x92FC;

// I2C
constexpr uint16_t COMM_TYPE = 0x9239;
constexpr uint16_t I2C_ADDY = 0x923A;

// Vcell Mode 
constexpr uint16_t VCELL_MODE = 0x9304;

// command/subcommand addresses 
constexpr uint16_t REG_LOWER     = 0x3E;
constexpr uint16_t REG_UPPER     = 0x3F;
constexpr uint16_t REG_DATA      = 0x40;
constexpr uint16_t REG_CHECKSUM  = 0x60; 
constexpr uint16_t REG_DATALENGTH = 0x61;

constexpr uint16_t SUBCOMMAND_BYTES = 4; // account for the 0x3E/0x3F and 0x61 and 0x60

// DEEPSLEEP/SLEEP Checks
constexpr uint16_t CMD_CONTROL_STATUS = 0x00; 
constexpr uint16_t CMD_BATTERY_STATUS = 0x12;
constexpr uint16_t CTRL_STATUS_DEEPSLEEP = (1 << 2);
constexpr uint16_t BAT_STATUS_SLEEP      = (1 << 15);

// Security stuff
constexpr uint16_t SECURITY_UNSEAL_FIRST  = 0x0414;
constexpr uint16_t SECURITY_UNSEAL_SECOND = 0x3672;
constexpr uint16_t FULL_ACCESS_EDIT     = 0x925D;
constexpr uint16_t SECURITY_FULLACESS   = 0xFFFF; 

// CFG Check 
constexpr uint16_t CFGUPDATE_STATUS = (1 << 0);

// wake commands
constexpr uint16_t CMD_WAKE_SLEEP     = 0x009A;
constexpr uint16_t CMD_WAKE_DEEPSLEEP = 0x000E;

// Firmware Operation
constexpr uint16_t SET_CFGUPDATE  = 0x0090;
constexpr uint16_t EXIT_CFGUPDATE = 0x0092;

// OTP
constexpr uint16_t OTP_WR_CHECK = 0x00A0;
constexpr uint16_t OTP_WRITE    = 0x00A1;

// Synchronous voltage and current 
constexpr uint16_t CMD_V_C_COUNT1 = 0x0071; // cell 1, 2, 3, 4
constexpr uint16_t CMD_V_C_COUNT2 = 0x0072; // cell 5

