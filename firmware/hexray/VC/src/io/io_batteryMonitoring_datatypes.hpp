

#pragma once
#include <cstdint>

// 4 cells, no need for CELLx (0x18)
constexpr uint16_t CMD_CELL1 = 0x14;
constexpr uint16_t CMD_CELL2 = 0x16;
constexpr uint16_t CMD_CELL3 = 0x1A;
constexpr uint16_t CMD_CELL4 = 0x1C;

// register addresses 

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

// wake commands
constexpr uint16_t CMD_WAKE_SLEEP     = 0x009A;
constexpr uint16_t CMD_WAKE_DEEPSLEEP = 0x000E;

// Firmware Operation
constexpr uint16_t SET_CFGUPDATE  = 0x0090;
constexpr uint16_t EXIT_CFGUPDATE = 0x0092;

// OTP
constexpr uint16_t OTP_WR_CHECK = 0x00A0;
constexpr uint16_t OTP_WRITE    = 0x00A1;
