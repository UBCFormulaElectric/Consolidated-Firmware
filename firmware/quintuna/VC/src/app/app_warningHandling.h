#pragma once
#include "stdbool.h"
#include <stdint.h>

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
} InverterWarningHandling;

extern InverterWarningHandling inverter_handle_FL;
extern InverterWarningHandling inverter_handle_FR;
extern InverterWarningHandling inverter_handle_RL;
extern InverterWarningHandling inverter_handle_RR;

// board warnings
bool app_warningHandling_boardWarningCheck(void);

// inverters
bool app_warningHandling_inverterStatus(void);
// void app_warningHandling_inverterReset(void);

// bspd
void app_softwareBspd_init(void);
bool app_warningHandling_checkSoftwareBspd(float papps_pedal_percentage);
