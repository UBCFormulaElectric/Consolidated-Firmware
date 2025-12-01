#pragma once
#include "stdbool.h"
#include <stdint.h>
#include "app_stateMachine.h"

extern const State *state_to_recover_after_fault;
extern bool         state_before_fault_locked;
typedef enum
{
    CAN_ISSUES = 3587,
    DC_BUS_ISSUES,
} Inverter_Fault_Info;
typedef enum
{
    INVERTER_FL,
    INVERTER_FR,
    INVERTER_RL,
    INVERTER_RR,
    NUM_INVERTERS
} InverterConfig;

typedef enum
{
    INV_FAULT_RETRY,
    INV_FAULT_LOCKOUT,
    INV_FAULT_RECOVERED,
} VCInverterFaults;
typedef struct
{
    void (*can_enable_inv)(bool);
    void (*can_invOn)(bool);
    void (*can_dcOn)(bool);
    uint32_t (*can_error_info)(void);
    void (*error_reset)(bool);
    bool (*can_error_bit)(void);
    void (*can_inv_warning)(bool);
} InverterHandle;

extern const InverterHandle inverter_handle_FL;
extern const InverterHandle inverter_handle_FR;
extern const InverterHandle inverter_handle_RL;
extern const InverterHandle inverter_handle_RR;

// inverters
bool app_inverter_inverterStatus(void);
void app_stateMachine_inverterFaultHandling(void);