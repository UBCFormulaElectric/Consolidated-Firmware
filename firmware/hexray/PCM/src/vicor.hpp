#pragma once

#include "util_errorCodes.hpp"

namespace vicor
{
result<void> operation(bool enable);
result<bool> read_operation();
result<void> clearFaults();

namespace read
{
    struct Capability
    {
        bool    packet_error_not_supported : 1;
        uint8_t max_bus_speed : 2;
        bool    smbus_alert_not_supported : 1;
        uint8_t reserved : 4;
        void    log() const
        {
            LOG_INFO(
                "Capability - PACKET_ERROR_NOT_SUPPORTED: %d, MAX_BUS_SPEED: %d, SMBUS_ALERT_NOT_SUPPORTED: %d",
                packet_error_not_supported, max_bus_speed, smbus_alert_not_supported);
        }
    };
    static_assert(sizeof(Capability) == sizeof(uint8_t), "VicorCapability should be 8 bits");
    result<Capability> capability();

    struct Power
    {
        float vin;
        float iin;
        float vout;
        float iout;
        float temp;
        float pout;
    };
    result<Power> power_stats();

    struct Metadata
    {
        uint8_t pmbus_version;
        char    id[2 + 1];
        char    part_number[18 + 1]{};
        char    revision[18 + 1]{};
        char    location[2 + 1]{};
        char    date[4 + 1]{};
        char    serial_num[16 + 1]{};

        void log() const
        {
            LOG_INFO(
                "Metadata - PMBus Version: %lX (expected 0x22), ID: %s (expect VI), Part Number: %s, Revision: %s, "
                "Location: %s, Date: "
                "%s (YY/MM), Serial Num: "
                "%s",
                pmbus_version, id, part_number, revision, location, date, serial_num);
        }
    };
    result<Metadata> metadata();

    struct Limits
    {
        uint16_t vin_min;
        uint16_t vin_max;
        uint16_t vout_min;
        uint16_t vout_max;
        uint16_t iout_max;
        uint16_t pout_max;
        void     log() const
        {
            LOG_INFO(
                "Limits - VIN_MIN: %d, VIN_MAX: %d, VOUT_MIN: %d, VOUT_MAX: %d, IOUT_MAX: %d, POUT_MAX: %d", vin_min,
                vin_max, vout_min, vout_max, iout_max, pout_max);
        }
    };
    result<Limits> limits();

    struct VicorLimitsHV
    {
        uint16_t read_k_factor; // hv only
        uint16_t read_bcm_rout; // hv only
    };
} // namespace read

namespace status
{
    struct CurrentOutput
    {
        bool pout_op_warning : 1;      // not supported
        bool pout_op_fault : 1;        // not supported
        bool in_pwr_limiting_mode : 1; // not supported
        bool current_share_fault : 1;  // not supported
        bool iout_uc_fault : 1;        // not supported
        bool iout_oc_warning : 1;
        bool iout_oc_lv_fault : 1; // not supported
        bool iout_oc_fault : 1;
    };
    result<CurrentOutput> iout();

    struct Input
    {
        bool pin_op_warning : 1; // not supported
        bool iin_oc_warning : 1; // not supported
        bool iin_oc_fault : 1;   // not supported
        bool unit_off_for_insufficient_voltage : 1;
        bool vin_uv_fault : 1;
        bool vin_uv_warning : 1; // not supported
        bool vin_ov_warning : 1;
        bool vin_ov_fault : 1;
    };
    result<Input> input();

    struct Temp
    {
        bool reserved : 4;
        bool ut_warning : 1;
        bool ut_fault : 1;
        bool ot_warning : 1;
        bool ot_fault : 1;
        void log() const
        {
            LOG_INFO(
                "Temp Status - UT Warning: %d, UT Fault: %d, OT Warning: %d, OT Fault: %d", ut_warning, ut_fault,
                ot_warning, ot_fault);
        }
    };
    static_assert(sizeof(Temp) == sizeof(uint8_t), "VicorTempStatus should be 8 bits");
    result<Temp> temp();

    struct General
    {
        // byte 1
        bool unknown : 1;    // not supported
        bool other_fake : 1; // not supported
        bool fan_alert : 1;  // not supported
        bool pwr_bad : 1;
        bool status_mfr_specific : 1;
        bool input_alert : 1;
        bool iout_or_pout_alert : 1;
        bool vout_alert : 1; // not supported

        // byte 2
        bool other : 1; // this means that STATUS_MFR_SPECIFIC (80h) or something in the first 8 bits is set
        bool pmbus_comm_event : 1;
        bool temp_alert : 1;
        bool vin_uv_fault : 1;
        bool iout_oc_fault : 1;
        bool vout_ov_fault : 1; // not supported
        bool unit_off : 1;
        bool unit_busy : 1;

        void log() const
        {
            LOG_INFO(
                "Status Word - PWR_BAD: %d, STATUS_MFR_SPECIFIC: %d, INPUT_ALERT: %d, IOUT_OR_POUT_ALERT: %d, OTHER: "
                "%d, "
                "PMBUS_COMM_EVENT: %d, TEMP_ALERT: %d, VIN_UV_FAULT: %d, IOUT_OC_FAULT: %d, UNIT_OFF: %d, UNIT_BUSY: "
                "%d",
                pwr_bad, status_mfr_specific, input_alert, iout_or_pout_alert, other, pmbus_comm_event, temp_alert,
                vin_uv_fault, iout_oc_fault, unit_off, unit_busy);
        }
    };
    static_assert(sizeof(General) == sizeof(uint16_t), "VicorStatus should be 16 bits");
    result<General> general();

    struct Comm
    {
        bool other_mem_or_logic_fault : 1; // unsupported
        bool other_comm_fault : 1;
        bool reserved : 1;
        bool processor_fault : 1; // unsupported
        bool mem_fault : 1;       // unsupported
        bool packet_error : 1;    // unsupported
        bool invalid_data : 1;
        bool invalid_command : 1;
        void log() const
        {
            LOG_INFO(
                "Comm Status - OTHER_MEM_OR_LOGIC_FAULT: %d, OTHER_COMM_FAULT: %d, PROCESSOR_FAULT: %d, MEM_FAULT: %d, "
                "PACKET_ERROR: %d, INVALID_DATA: %d, INVALID_COMMAND: %d",
                other_mem_or_logic_fault, other_comm_fault, processor_fault, mem_fault, packet_error, invalid_data,
                invalid_command);
        }
    };
    static_assert(sizeof(Comm) == sizeof(uint8_t), "VicorCommStatus should be 8 bits");
    result<Comm> comm();

    struct MFRSpecific
    {
        bool    reverse_operation : 1;
        bool    hw_protection_shutdown_fault : 1;
        bool    bcm_uart_cml : 1;
        uint8_t _reserved_2 : 1;
        bool    bcm_at_page_1_preset : 1;
        uint8_t _reserved : 3;
        void    log() const
        {
            LOG_INFO(
                "MFR Specific Status - REVERSE_OPERATION: %d, HW_PROTECTION_SHUTDOWN_FAULT: %d, BCM_UART_CML: %d, "
                "BCM_AT_PAGE_1_PRESET: %d",
                reverse_operation, hw_protection_shutdown_fault, bcm_uart_cml, bcm_at_page_1_preset);
        }
    };
    static_assert(sizeof(MFRSpecific) == sizeof(uint8_t), "VicorStatusMFRSpecific should be 8 bits");
    result<MFRSpecific> MfrSpecific();
} // namespace status
} // namespace vicor
