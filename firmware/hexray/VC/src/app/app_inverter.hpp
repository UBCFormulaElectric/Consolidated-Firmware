#pragma once

#include "app_stateMachine.hpp"
#include <cstdint>
#include <cstdlib>


namespace app
{

enum InverterFaultHandler : uint8_t
{
    INV_FAULT_RETRY,
    INV_FAULT_LOCKOUT,
    INV_FAULT_RECOVERED,
};


struct InverterHandle
{
    void (*can_enable_inv)(bool);
    void (*can_invOn)(bool);
    void (*can_dcOn)(bool);
    uint16_t (*can_error_info)(void);
    void (*error_reset)(bool);
    bool (*can_error_bit)(void);
    void (*can_inv_warning)(bool);

    consteval explicit InverterHandle(
        void (*can_enable_inv_in)(bool),
        void (*can_invOn_in)(bool),
        void (*can_dcOn_in)(bool),
        uint16_t (*can_error_info_in)(void),
        void (*error_reset_in)(bool),
        bool (*can_error_bit_in)(void),
        void (*can_inv_warning_in)(bool)):
        can_enable_inv(can_enable_inv_in),
        can_invOn(can_invOn_in),
        can_dcOn(can_dcOn_in),
        can_error_info(can_error_info_in),
        error_reset(error_reset_in),
        can_error_bit(can_error_bit_in),
        can_inv_warning(can_inv_warning_in){}
};
    
namespace inverter
{
void FaultCheck(void);

bool lockout(void);

InverterFaultHandler FaultHandler(void);
} // namespace inverter

} // namespace app