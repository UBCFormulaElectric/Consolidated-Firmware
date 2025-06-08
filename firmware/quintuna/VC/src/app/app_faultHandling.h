#pragma once 


// function to get all board faults
#include <stdbool.h>

typedef enum
{
    NO_FAULT,
    BOARD_FAULT, 
    INVERTER_FAULT
} faultType; 

bool app_faultHandeling_checkBoardStatus(void);

// handle inverter faulting
bool app_faultHandeling_inverterStatus(void);

faultType app_faultHandling_globalFaultCheck(void);

