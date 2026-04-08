

#pragma once
#include <cstdint>

// MVolateg Measurement
typedef enum
{
    VOLTAGE,
    CURRENT
} Measurement;

typedef enum
{
    CELL1 = 1,
    CELL2 = 2,
    CELL3 = 3,
    CELL4 = 4,
    CELL5 = 5
} CellNum;

// Security

enum class SecurityState : uint8_t
{
    FULL_ACCESS = 0x1,
    UNSEALED    = 0x2,
    SEALED      = 0x3,
};

constexpr uint16_t CELL1_MV = 0x14;
constexpr uint16_t CELL2_MV = 0x16;
constexpr uint16_t CELL3_MV = 0x18;
constexpr uint16_t CELL4_MV = 0x1A;
constexpr uint16_t CELL5_MV = 0x1C;
constexpr uint16_t PACK_MV  = 0x36;
constexpr uint16_t LOAD_MV  = 0x38;

// Internal regulator adresses
constexpr uint16_t REG0_CONFIG = 0x9237;
constexpr uint16_t REG1_CONFIG = 0x9236;

// Multifunction Pin Controls
constexpr uint16_t ALERT = 0x92FC;

// I2C
constexpr uint16_t COMM_TYPE = 0x9239;
constexpr uint16_t I2C_ADDY  = 0x923A;

// Vcell Mode
constexpr uint16_t VCELL_MODE = 0x9304;

// command/subcommand addresses
constexpr uint16_t REG_LOWER        = 0x3E;
constexpr uint16_t REG_UPPER        = 0x3F;
constexpr uint16_t REG_DATA         = 0x40;
constexpr uint16_t REG_CHECKSUM     = 0x60;
constexpr uint16_t REG_DATALENGTH   = 0x61;
constexpr uint16_t REG_ALARM_STATUS = 0x62;

constexpr uint8_t SUBCOMMAND_BYTES = 4; // account for the 0x3E/0x3F and 0x61 and 0x60

// DEEPSLEEP/SLEEP Checks
constexpr uint16_t CMD_CONTROL_STATUS    = 0x00;
constexpr uint16_t CMD_BATTERY_STATUS    = 0x12;
constexpr uint8_t  CTRL_STATUS_DEEPSLEEP = (1 << 2);
constexpr uint8_t  BAT_STATUS_SLEEP      = (1 << 15);

// Security stuff
constexpr uint16_t SECURITY_UNSEAL_FIRST  = 0x0414;
constexpr uint16_t SECURITY_UNSEAL_SECOND = 0x3672;
constexpr uint16_t FULL_ACCESS_EDIT       = 0x925D;
constexpr uint16_t SECURITY_FULLACESS     = 0xFFFF;

// CFG Check
constexpr uint8_t CFGUPDATE_STATUS = (1 << 0);

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

// Protections
constexpr uint16_t MFG_STATUS_INIT = 0x9343;

// Seafty commands
constexpr uint16_t SAFETY_A_A = 0x02;
constexpr uint16_t SAFETY_S_A = 0x03;
constexpr uint16_t SAFETY_A_B = 0x04;
constexpr uint16_t SAFETY_S_B = 0x05;
constexpr uint16_t SAFETY_A_C = 0x06;
constexpr uint16_t SAFETY_S_C = 0x07;

// Safety Alert/Status A
constexpr uint8_t SAFETY_A_SCD  = (1u << 7);
constexpr uint8_t SAFETY_A_OCD2 = (1u << 6);
constexpr uint8_t SAFETY_A_OCD1 = (1u << 5);
constexpr uint8_t SAFETY_A_OCC  = (1u << 4);
constexpr uint8_t SAFETY_A_COV  = (1u << 3);
constexpr uint8_t SAFETY_A_CUV  = (1u << 2);

// Safety Alert/Status B
constexpr uint8_t SAFETY_B_OTF   = (1u << 7);
constexpr uint8_t SAFETY_B_OTINT = (1u << 6);
constexpr uint8_t SAFETY_B_OTD   = (1u << 5);
constexpr uint8_t SAFETY_B_OTC   = (1u << 4);
constexpr uint8_t SAFETY_B_UTD   = (1u << 1);
constexpr uint8_t SAFETY_B_UTC   = (1u << 0);

// Safety Alert/Status C
constexpr uint8_t SAFETY_C_OCD3 = (1u << 7);
constexpr uint8_t SAFETY_C_SCDL = (1u << 6);
constexpr uint8_t SAFETY_C_OCDL = (1u << 5);
constexpr uint8_t SAFETY_C_COVL = (1u << 4);
constexpr uint8_t SAFETY_C_PTOS = (1u << 3);
// Status C only
constexpr uint8_t SAFETY_C_PTO  = (1u << 2);
constexpr uint8_t SAFETY_C_HWDF = (1u << 1);

// FET stuff
constexpr uint16_t FET_FET_OPTION = 0x9308;
constexpr uint16_t CMD_ALL_FETS_ON = 0x0096;
