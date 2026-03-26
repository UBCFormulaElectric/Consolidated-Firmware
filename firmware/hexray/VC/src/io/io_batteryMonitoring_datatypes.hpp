

#pragma once
#include <cstdint>

//add all of the defines in here
constexpr uint16_t CMD_CONTROL_STATUS = 0x00; 
constexpr uint16_t CMD_BATTERY_STATUS = 0x12;

// 4 cells 
constexpr uint16_t CMD_CELL1 = 0x14;
//constexpr uint16_t CMD_CELL2 = 0x16;
//constexpr uint16_t CMD_CELL3 = 0x18;
//constexpr uint16_t CMD_CELL4 = 0x1A;

// register addresses 

// command/subcommand addresses 
constexpr uint16_t REG_LOWER     = 0x3E;
constexpr uint16_t REG_UPPER     = 0x3F;
constexpr uint16_t REG_DATA      = 0x40;
constexpr uint16_t REG_CHECKSUM  = 0x60; 
constexpr uint16_t REG_DATALENGTH = 0x61;

constexpr uint16_t SUBCOMMAND_BYTES = 4; // account for the 0x3E/0x3F and 0x61 and 0x60

// regsiter bitmasks 
constexpr uint16_t BAT_STAT_SLEEP      = (1 << 15);
constexpr uint16_t CTRL_STAT_DEEPSLEEP = (1 << 14);

// wake commands
constexpr uint16_t CMD_WAKE_SLEEP        = 0x009A;
constexpr uint16_t CMD_WAKE_DEEPSLEEP    = 0x000E;