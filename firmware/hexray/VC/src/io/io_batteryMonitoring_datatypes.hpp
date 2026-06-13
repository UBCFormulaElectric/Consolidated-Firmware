
#pragma once
#include <array>

// Commands for voltages
/**
 * @brief The commands for reading the voltages of the 4 cells
 */
enum class CellReading : uint8_t
{
    CELL1 = 0x14,
    CELL2 = 0x16,
    CELL3 = 0x1A,
    CELL4 = 0x1C
};
/**
 * @brief
 * PACK voltage is cell stack state
 * LOAD voltage is after the FETS (don't rlly have this year), reading this confirms voltage is delivered past the IC
 */
enum class SystemReading : uint8_t
{
    PACK_V = 0x36,
    LOAD_V = 0x38
};

// Balancing cell bitmask
enum class CellBalance_BitMask : uint16_t
{
    CELL1 = 0x0001, // 0001
    CELL2 = 0x0002, // 0010
    CELL3 = 0x0008, // 1000
    CELL4 = 0x0010, // 00010000
};

// TODO: Protection stuff
struct ProtectionStatus
{
    bool overvoltage  = false;
    bool undervoltage = false;
};

// Internal regulator adresses
inline constexpr uint16_t REG0_CONFIG = 0x9237;
inline constexpr uint16_t REG1_CONFIG = 0x9236;

// Multifunction Pin Controls
inline constexpr uint16_t ALERT = 0x92FC;

// I2C
inline constexpr uint16_t COMM_TYPE = 0x9239;

// Vcell Mode
inline constexpr uint16_t VCELL_MODE = 0x9304;

// command/subcommand addresses
inline constexpr uint16_t REG_LOWER      = 0x3E;
inline constexpr uint16_t REG_UPPER      = 0x3F;
inline constexpr uint16_t REG_DATA       = 0x40;
inline constexpr uint16_t REG_CHECKSUM   = 0x60;
inline constexpr uint16_t REG_DATALENGTH = 0x61;

inline constexpr uint8_t  SUBCOMMAND_BYTES = 4;
inline constexpr uint32_t RETRIES          = 20;

// DEEPSLEEP/SLEEP Checks
inline constexpr uint16_t CMD_CONTROL_STATUS    = 0x00;
inline constexpr uint16_t SUBCMD_WAKE_DEEPSLEEP = 0x000E;

inline constexpr uint16_t CMD_BATTERY_STATUS = 0x12;
inline constexpr uint16_t SUBCMD_WAKE_SLEEP  = 0x009A;

// CONFIG_UPDATE
inline constexpr uint16_t SUBCMD_SET_CFGUPDATE = 0x0090;
inline constexpr uint16_t EXIT_CFGUPDATE       = 0x0092;

// CC and Digital Filters
inline constexpr uint16_t SUBCMD_RESETCHARGEACCUM = 0x0082;

// Current
inline constexpr uint16_t CMD_GETCURRENT = 0x3A;

// Charge
inline constexpr uint16_t SUBCMD_DASTATUS5            = 0x0075;
inline constexpr uint16_t SUBCMD_GET_INEGRATED_CHARGE = 0x0076;

// Cell Balancing
inline constexpr uint16_t CMD_TEMPERATURE_IC          = 0x68;
inline constexpr uint16_t BALANCE_CFG                 = 0x9335;
inline constexpr uint16_t MIN_CELL_TEMP               = 0x9336;
inline constexpr uint16_t MAX_CELL_TEMP               = 0x9337;
inline constexpr uint16_t MAX_IC_TEMP                 = 0x9338;
inline constexpr uint16_t CELL_BALANCE_INTERVAL       = 0x9339; // 20 seconds default
inline constexpr uint16_t MAX_CELLS_BALANCING         = 0x933A;
inline constexpr uint16_t CELL_BALANCE_MIN_V_CHRG     = 0x933B;
inline constexpr uint16_t CELL_BALANCE_MIN_V_DEL_CHRG = 0x933D;
inline constexpr uint16_t CELL_BALANCE_MIN_V          = 0x933F;
inline constexpr uint16_t CELL_BALANCE_MIN_DELTA      = 0x9341;
inline constexpr uint16_t CELL_BALANCE_STOP_V         = 0x9342;
inline constexpr uint16_t CB_ACTIVE_CELLS             = 0x0083;
inline constexpr uint16_t CBSTATUS1                   = 0x0085;
inline constexpr uint16_t CBSTATUS2                   = 0x0086;
inline constexpr uint16_t CB_SET_LVL                  = 0x0084;

// Protections
inline constexpr uint16_t REG_PROTECTIONS_A   = 0x9261;
inline constexpr uint16_t REG_PROTECTIONS_B   = 0x9262;
inline constexpr uint16_t REG_PROTECTIONS_CUV = 0x9275;
inline constexpr uint16_t REG_PROTECTIONS_COV = 0x9278;
inline constexpr uint16_t REG_SF_ALERT_MASK_A = 0x926F;
inline constexpr uint16_t REG_SF_ALERT_MASK_B = 0x9270;

inline constexpr uint16_t CUV_SNAPSHOT = 0x0080;
inline constexpr uint16_t COV_SNAPSHOT = 0x9270;

// OTP
inline constexpr uint16_t OTP_WR_CHECK = 0x00A0;
inline constexpr uint16_t OTP_WRITE    = 0x00A1;

// Synchronous voltage and current
inline constexpr uint16_t CMD_V_C_COUNT1 = 0x0071; // cell 1, 2, 3, 4
inline constexpr uint16_t CMD_V_C_COUNT2 = 0x0072; // cell 5

// Protections
inline constexpr uint16_t MFG_STATUS_INIT = 0x9343;

// Seafty commands
inline constexpr uint16_t CMD_SAFETY_STATUS_A = 0x02;
inline constexpr uint16_t CMD_SAFETY_STATUS_B = 0x04;
inline constexpr uint16_t CMD_ALARM_STATUS    = 0x62;
inline constexpr uint16_t DEFAULT_ALARM       = 0x926D;

// FET stuff
inline constexpr uint16_t FET_FET_OPTION     = 0x9308;
inline constexpr uint16_t SUBCMD_ALL_FETS_ON = 0x0096;

// OTP Security
enum class SecurityState : uint8_t
{
    FULL_ACCESS = 0x1,
    UNSEALED    = 0x2,
    SEALED      = 0x3,
};

inline constexpr uint16_t SECURITY_UNSEAL_FIRST  = 0x0414;
inline constexpr uint16_t SECURITY_UNSEAL_SECOND = 0x3672;
inline constexpr uint16_t FULL_ACCESS_EDIT       = 0x925D;
inline constexpr uint16_t SECURITY_FULLACESS     = 0xFFFF;

union AlertStatus
{
    struct __attribute__((packed))
    {
        uint16_t WAKE : 1;
        uint16_t ADSCAN : 1;
        uint16_t CB : 1;
        uint16_t FUSE : 1;
        uint16_t SHUTV : 1;
        uint16_t XDSG : 1;
        uint16_t XCHG : 1;
        uint16_t FULLSCAN : 1;

        uint16_t RSVD : 1;
        uint16_t INITCOMP : 1;
        uint16_t INITSTART : 1;
        uint16_t MSK_PFALERT : 1;
        uint16_t MSK_SFALERT : 1; // This bit is set when a safety alert is triggered
        uint16_t PF : 1;
        uint16_t SSA : 1;  // This bit is set when a bit is set in Safety Status A().
        uint16_t SSBC : 1; // This bit is set when a bit is set in Safety Status B() or Safety Status C().
    } bits;
    uint16_t raw_value;
    static_assert(sizeof(bits) == sizeof(raw_value));
};

union BatteryStatus
{
    struct __attribute__((packed))
    {
        uint16_t CFGUPDATE : 1;
        uint16_t PCHG_MODE : 1;
        uint16_t SLEEP_EN : 1;
        uint16_t POR : 1;
        uint16_t WD : 1;
        uint16_t COW_CHECK : 1;
        uint16_t OTPW : 1;
        uint16_t OTPB : 1;
        uint16_t SEC0 : 1;
        uint16_t SEC1 : 1;
        uint16_t SS : 1;
        uint16_t FUSE : 1;
        uint16_t PF : 1;
        uint16_t SDM : 1;
        uint16_t RSVD : 1;
        uint16_t SLEEP : 1;
    } bits;
    uint16_t raw_value;
    static_assert(sizeof(bits) == sizeof(raw_value));
};

union ControlStatus
{
    struct __attribute__((packed))
    {
        uint16_t RSVD2 : 8;
        uint16_t LD_ON : 1;
        uint16_t LD_TIMEOUT : 1;
        uint16_t DEEPSLEEP : 1;
        uint16_t RSVD1 : 5;
    } bits;
    uint16_t raw_value;
    static_assert(sizeof(bits) == sizeof(raw_value));
};
union SafetyStatusA
{
    struct __attribute__((packed))
    {
        uint8_t RSVD : 2; // Reserved
        uint8_t CUV : 1;  // Cell undervoltage
        uint8_t COV : 1;  // Cell overvoltage
        uint8_t OCC : 1;  // Overcurrent in charge
        uint8_t OCD1 : 1; // Overcurrent in discharge 1
        uint8_t OCD2 : 1; // Overcurrent in discharge 2
        uint8_t SCD : 1;  // Short Circuit in Discharge
    } bits;               // alerts associated with the battery chip
    uint8_t raw_status;
    static_assert(sizeof(bits) == sizeof(raw_status));
};
union SafetyStatusB
{
    struct __attribute__((packed))
    {
        uint8_t UTC : 1;
        uint8_t UTD : 1;
        uint8_t UTINT : 1;
        uint8_t RSVD : 1;
        uint8_t OTC : 1;
        uint8_t OTD : 1;
        uint8_t OTINT : 1; // Internal Die Overtemperature
        uint8_t OTF : 1;
    } bits;
    uint8_t raw_status;
    static_assert(sizeof(bits) == sizeof(raw_status));
};
union CUV
{
    typedef struct __attribute__((packed))
    {
        uint16_t cell1_voltage; // Bytes 0-1
        uint16_t cell2_voltage; // Bytes 2-3
        uint16_t cell3_voltage; // Bytes 4-5
        uint16_t cell4_voltage; // Bytes 6-7
        uint16_t cell5_voltage; // Bytes 8-9
        uint16_t reserved[11];  // Bytes 10-31
    } bytes;
    std::array<uint8_t, 32> snapshot_undervoltages;
};
union COV
{
    typedef struct __attribute__((packed))
    {
        uint16_t cell1_voltage; // Bytes 0-1
        uint16_t cell2_voltage; // Bytes 2-3
        uint16_t cell3_voltage; // Bytes 4-5
        uint16_t cell4_voltage; // Bytes 6-7
        uint16_t cell5_voltage; // Bytes 8-9
        uint16_t reserved[11];  // Bytes 10-31
    } bytes;
    std::array<uint8_t, 32> snapshot_overvoltages;
};
