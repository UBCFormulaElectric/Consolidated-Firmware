#pragma once

#include "util_errorCodes.hpp"

result<void>    vicor_operation(bool enable);
result<bool>    vicor_read_operation();
result<void>    vicor_clearFaults();
result<float>   vicor_readVin();
result<float>   vicor_readIin();
result<float>   vicor_readVout();
result<float>   vicor_readIout();
result<float>   vicor_readTemp();
result<float>   vicor_readPout();
result<void>    vicor_readSerial();
result<uint8_t> vicor_statusIout();
result<uint8_t> vicor_statusInput();
result<uint8_t> vicor_statusTemp();

struct VicorStatus
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
};
static_assert(sizeof(VicorStatus) == sizeof(uint16_t), "VicorStatus should be 16 bits");
result<VicorStatus> vicor_statusWord();

struct VicorCommStatus
{
    bool other_mem_or_logic_fault : 1; // unsupported
    bool other_comm_fault : 1;
    bool reserved : 1;
    bool processor_fault : 1; // unsupported
    bool mem_fault : 1;       // unsupported
    bool packet_error : 1;    // unsupported
    bool invalid_data : 1;
    bool invalid_command : 1;
};
result<VicorCommStatus> vicor_statusComm();

struct VicorStatusMFRSpecific
{
    bool    reverse_operation : 1;
    bool    hw_protection_shutdown_fault : 1;
    bool    bcm_uart_cml : 1;
    uint8_t _reserved_2 : 1;
    bool    bcm_at_page_1_preset : 1;
    uint8_t _reserved : 3;
};
result<VicorStatusMFRSpecific> vicor_statusMfrSpecific();

result<uint16_t> vicor_serialNumber();
