#pragma once

#include "app_stateMachine.hpp"
#include <cstdint>
#include <cstdlib>

namespace app::inverter
{

enum FaultHandlerState : uint8_t
{
    INV_FAULT_RETRY,
    INV_FAULT_LOCKOUT,
    INV_FAULT_RECOVERED,
};

struct Handle
{
    void (*can_enable_inv)(bool);
    void (*can_invOn)(bool);
    void (*can_dcOn)(bool);
    void (*can_torque_setpoint)(int16_t);
    void (*can_torque_limit_negative)(int16_t);
    void (*can_torque_limit_positive)(int16_t);
    uint16_t (*can_error_info)(void);
    void (*error_reset)(bool);
    bool (*can_error_bit)(void);
    void (*can_inv_warning)(bool);

    consteval explicit Handle(
        void (*can_enable_inv_in)(bool),
        void (*can_invOn_in)(bool),
        void (*can_dcOn_in)(bool),
        void (*can_torque_setpoint_in)(int16_t),
        void (*can_torque_limit_negative_in)(int16_t),
        void (*can_torque_limit_positive_in)(int16_t),
        uint16_t (*can_error_info_in)(void),
        void (*error_reset_in)(bool),
        bool (*can_error_bit_in)(void),
        void (*can_inv_warning_in)(bool))
      : can_enable_inv(can_enable_inv_in),
        can_invOn(can_invOn_in),
        can_dcOn(can_dcOn_in),
        can_torque_setpoint(can_torque_setpoint_in),
        can_torque_limit_negative(can_torque_limit_negative_in),
        can_torque_limit_positive(can_torque_limit_positive_in),
        can_error_info(can_error_info_in),
        error_reset(error_reset_in),
        can_error_bit(can_error_bit_in),
        can_inv_warning(can_inv_warning_in)
    {
    }
};

void set_torque_limit_negative(float fl_Nm, float fr_Nm, float rl_Nm, float rr_Nm);
void set_torque_limit_positive(float fl_Nm, float fr_Nm, float rl_Nm, float rr_Nm);
void send_torque(float fl_Nm, float fr_Nm, float rl_Nm, float rr_Nm);

void FaultCheck(void);

FaultHandlerState FaultHandler(void);

State *recovery_state(void);
} // namespace app::inverter