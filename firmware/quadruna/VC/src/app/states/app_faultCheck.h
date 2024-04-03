#pragma once

#include <stdbool.h>

/**
 * Check if any baord has faulted 
 */
bool app_boardFaultCheck();

/**
 * Check either right or left inverters have faulted
 * 
 */
bool app_inverterFaultCheck();


