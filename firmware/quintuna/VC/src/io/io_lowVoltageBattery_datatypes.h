#pragma once

#include <stdint.h>

#define REG_SUBCOMMAND (0x3E)
#define REG_DATA_BUFFER (0x40)
#define REG_CHECKSUM (0x60)
#define REG_RESPONSE_LENGTH (0x61)
#define ALARM_PIN_CONFIG (0x56)
#define ALARM_ENABLE_REG (0x66)
#define ALARM_STATUS_REG (0x62)

#define SAFTEY_ALERT_A (0x02)
#define SAFTEY_STATUS_A (0x03)
#define SAFTEY_ALERT_B (0x04)
#define SAFTEY_STATUS_B (0x05)
#define SAFTEY_ALERT_C (0x06)
#define SAFTEY_STATUS_C (0x07)
#define ALARM_RAW_STATUS (0x64)
#define FET_STATUS (0x7F)

#define BATTERY_STATUS (0x12)
#define CONTROL_STATUS (0x00)

#define CELL0_VOLTAGE_COMMAND (0x14)
#define CELL1_VOLTAGE_COMMAND (0x16)
#define CELL2_VOLTAGE_COMMAND (0x1A)
#define CELL4_VOLTAGE_COMMAND (0x1C)
#define STACK_VOLTAGE_COMMAND (0x34)

#define CONFIG_UPDATE_COMMAND (0x0090)
#define CONFIG_UPDATE_EXIT_COMMAND (0x0092)
#define OTP_COMMAND (0x00F4)
#define OTP_WR_CHECK (0x00A0)
#define OTP_WR (0x00A1)

#define ACCUMULATED_CHARGE_COMMAND (0x0076)

#define ENABLED_PROTECTIONS_A (0x9256)
#define ENABLED_PROTECTIONS_B (0x9262)
#define OV_THRESHOLD (0x9278)
#define UV_THRESHOLD (0x9275)
#define OV_DELAY (0x9279)
#define UV_DELAY (0x9276)
#define OV_RECOVERY_HYSTERESIS (0x927C)
#define UV_RECOVERY_HYSTERESIS (0x927B)
#define SCD_THRESHOLD (0x9286)
#define OCD1_DELAY (0x9283)
#define OCD1_THRESHOLD (0x9282)
#define OCD_RECOVERY_THRESHOLD (0x928D)

#define ALARM_CLEAR_CMD (0x01)
#define ALARM_PIN_INTERRUPT_CONFIG (0x02)
#define ALARM_ENABLE_VALUE (0x82)

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

typedef struct
{
    uint8_t response_buffer[32];
    uint8_t length;
} Subcommand_Response;

typedef enum
{
    I2C_TIMEOUT_MS       = 100, // Timeout for I2C operations
    CMD_SUBCOMMAND_SIZE  = 2,   // Two bytes for subcommand
    RESPONSE_LENGTH_SIZE = 4,   // One byte for response length and checksum
    SOC_RESPONSE_LENGTH  = 6,   // Expected response size for SOC command
} comm_settings_t;
