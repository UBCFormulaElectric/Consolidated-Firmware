#pragma once
#include <iostream>

// Fault handling is dedicated to faults that take the VC state machine out of drive state
namespace app::fault_handling
{
// Check if the air minus contactor is closed
bool air_minus_closed(void);
// Function ensuring the BMS hearbeat is valid for VC
bool fsm_bms_HeartbeartChecks(void);
} // namespace app::fault_handling
