#pragma once 


// function to get all board faults
#include <stdbool.h>
bool app_faultHandeling_checkBoardStatus(void);

// handle inverter faulting
bool app_faultHandeling_inverterStatus(void);

void app_faultHandling_globalFaultCheck(void);
