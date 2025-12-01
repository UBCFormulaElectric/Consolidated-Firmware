#pragma once

#include <stdint.h>

typedef struct __attribute__((packed))
{
    uint8_t CFGUPDATE : 1;
    uint8_t PCHG_MODE : 1;
    uint8_t SLEEP_EN : 1;
    uint8_t POR : 1;
    uint8_t WD : 1;
    uint8_t COW_CHECK : 1;
    uint8_t OTPW : 1;
    uint8_t OTPB : 1;

    uint8_t SEC0 : 1;
    uint8_t SEC1 : 1;
    uint8_t SS : 1;
    uint8_t FUSE : 1;
    uint8_t PF : 1;
    uint8_t SDM : 1;
    uint8_t RSVD : 1;
    uint8_t SLEEP : 1;
} Battery_Status;

typedef struct __attribute__((packed))
{
    uint8_t RSVD2 : 8;

    uint8_t LD_ON : 1;
    uint8_t LD_TIMEOUT : 1;
    uint8_t DEEPSLEEP : 1;
    uint8_t RSVD1 : 5;
} Control_Status;

typedef struct __attribute__((packed))
{
    uint8_t RSVD1 : 1; // Reserved
    uint8_t RSVD2 : 1; // Reserved
    uint8_t CUV : 1;   // Cell undervoltage
    uint8_t COV : 1;   // Cell overvoltage
    uint8_t OCC : 1;   // Overcurrent in charge
    uint8_t OCD1 : 1;  // Overcurrent in discharge 1
    uint8_t OCD2 : 1;  // Overcurrent in discharge 2
    uint8_t SCD : 1;   // Short Circuit in Discharge
} SafteyStatusA;       // alerts associated with the battery chip

typedef struct __attribute__((packed))
{
    uint8_t UTC : 1;
    uint8_t UTD : 1;
    uint8_t UTINT : 1;
    uint8_t RSVD : 1;
    uint8_t OTC : 1;
    uint8_t OTD : 1;
    uint8_t OTINT : 1;
    uint8_t OTF : 1;
} SafteyStatusB;

typedef struct __attribute__((packed))
{
    uint8_t RSVD1 : 1;
    uint8_t RSVD2 : 1;
    uint8_t RSVD3 : 1;
    uint8_t PTOS : 1;
    uint8_t COVL : 1;
    uint8_t OCDL : 1;
    uint8_t SCDL : 1;
    uint8_t OCD3 : 1;
} SafteyStatusC;

typedef struct __attribute__((packed))
{
    uint8_t WAKE : 1;
    uint8_t ADSCAN : 1;
    uint8_t CB : 1;
    uint8_t FUSE : 1;
    uint8_t SHUTV : 1;
    uint8_t XDSG : 1;
    uint8_t XCHG : 1;
    uint8_t FULLSCAN : 1;

    uint8_t RSVD : 1;
    uint8_t INITCOMP : 1;
    uint8_t INITSTART : 1;
    uint8_t MSK_PFALERT : 1;
    uint8_t MSK_SFALERT : 1;
    uint8_t PF : 1;
    uint8_t SSA : 1;
    uint8_t SSBC : 1;
} AlertStatus;

typedef enum
{
    BQ76922_I2C_ADDR    = 0x10,
    REG_SUBCOMMAND      = 0x3E,
    REG_DATA_BUFFER     = 0x40,
    REG_CHECKSUM        = 0x60,
    REG_RESPONSE_LENGTH = 0x61,
    ALERT_PIN_CONFIG    = 0x56,
    ALARM_ENABLE_REG    = 0x66,
    ALARM_STATUS_REG    = 0x62
} register_address_t;

typedef enum
{
    SAFTEY_ALERT_A   = 0x02,
    SAFTEY_STATUS_A  = 0x03,
    SAFTEY_ALERT_B   = 0x04,
    SAFTEY_STATUS_B  = 0x05,
    SAFTEY_ALERT_C   = 0x06,
    SAFTEY_STATUS_C  = 0x07,
    ALARM_RAW_STATUS = 0x64
} saftey_register;

typedef enum
{
    ENABLED_PROTECTIONS_A  = 0x9256,
    ENABLED_PROTECTIONS_B  = 0x9262,
    OV_THRESHOLD           = 0x9278,
    UV_THRESHOLD           = 0x9275,
    OV_DELAY               = 0x9279,
    UV_DELAY               = 0x9276,
    OV_RECOVERY_HYSTERESIS = 0x927C,
    UV_RECOVERY_HYSTERESIS = 0x927B,
    SCD_THRESHOLD          = 0x9286,
    OTC_THRESHOLD          = 0x929A,
    OCD1_DELAY             = 0x9283,
    OCD1_THRESHOLD         = 0x9282,
    OCD_RECOVERY_THRESHOLD = 0x928D
} protection_t;

typedef enum
{
    ACCUMULATED_CHARGE_COMMAND = 0x0076
} charge_cmd_t;

typedef enum
{
    BATTERY_STATUS = 0x12,
    CONTROL_STATUS = 0x00
} commands;
typedef enum
{
    CELL0_VOLTAGE_COMMAND = 0x14,
    CELL1_VOLTAGE_COMMAND = 0x16,
    CELL2_VOLTAGE_COMMAND = 0x1A,
    CELL4_VOLTAGE_COMMAND = 0x1C,
    STACK_VOLTAGE_COMMAND = 0x34
} voltage_cmd_t;

typedef struct
{
    uint8_t response_buffer[32];
    uint8_t length;
} Subcommand_Response;

typedef enum
{
    CONFIG_UPDATE_COMMAND      = 0x0090,
    CONFIG_UPDATE_EXIT_COMMAND = 0x0092,
    OTP_COMMAND                = 0x00F4,
    OTP_WR_CHECK               = 0x00A0,
    OTP_WR                     = 0x00A1
} config_cmd_t;

typedef enum
{
    ALARM_CLEAR_CMD            = 0x01,
    ALERT_ACTIVE_LOW_BIT       = 5,    // Bit position for active-low configuration
    ALERT_PIN_INTERRUPT_CONFIG = 0x02, // ALERT pin configured as an interrupt output
    ALARM_ENABLE_VALUE         = 0x82  // Enable ADC scan alerts
} alarm_config_t;

typedef enum
{
    I2C_TIMEOUT_MS       = 100, // Timeout for I2C operations
    CMD_SUBCOMMAND_SIZE  = 2,   // Two bytes for subcommand
    RESPONSE_LENGTH_SIZE = 4,   // One byte for response length and checksum
    SOC_RESPONSE_LENGTH  = 6,   // Expected response size for SOC command
} comm_settings_t;