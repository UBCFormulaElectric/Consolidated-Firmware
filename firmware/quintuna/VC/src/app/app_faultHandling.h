#pragma once 


// function to get all board faults
#include <stdbool.h>

typedef enum
{
    NO_FAULT,
    BMS_LATCH_FAULT,
    FSM_BSM_HEARTBEAT_FAULT, 
    INVERTER_FAULT
} faultType;

bool app_faultHandling_bmsLatchedFaults(void); 

bool app_faultHandling_fsm_bms_HeartbeartChecks(void);

// handle inverter faulting
bool app_faultHandeling_inverterStatus(void);

void app_fault_Handling_clear_inverterFaults(void);

faultType app_faultHandling_globalFaultCheck(void);

