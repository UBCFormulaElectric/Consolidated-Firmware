#include <stdbool.h>
#include "io_efuse.h"
#pragma once

typedef enum
{
    PROTOCOL_STATE_OFF,                // Efuse is not in a retry protocol
    PROTOCOL_STATE_CALC_AVG,           // Efuse is calculating the average current
    PROTOCOL_STATE_CALC_DONE,          // Efuse has finished calculating the average and needs further action
    PROTOCOL_STATE_DEPENDENCY_WAITING, // Efuse is waiting for another efuse to finish its retry protocol
    PROTOCOL_STATE_DEBOUNCE,           // Efuse is waiting for debounce time before turning on again
    PROTOCOL_STATE_FAULTED             // Efuse went through the protocol and is still faulting
} ProtocolState;

typedef struct
{
    bool  is_efuse_on;          // State of the efuse (on/off)
    int   retry_attempts_limit; // Limit for retry protocol attempts
    float min_needed_current;   // Minimum current required for the efuse
} RetryConfig;
typedef struct
{
    ProtocolState protocol_state; // Current state of the efuse protocol
    int           retry_attempts; // Number of retry attempts
    int           timer_attempts; // Timer attempts for current calculation
    float         current_sum;    // Sum of current readings for average calculation
} RetryData;

/**
 * @brief Starts the retry protocol for efuses.
 *
 * @param efuse Which efuse protocol to run.
 * @param retry_configs The configurations for each efuse (efuse_state).
 * @param retry_data The data for each efuse (ProtocolState).
 * @return If car should reset (VC go to init state).
 */
bool app_retryHandler_start(EfuseChannel efuse, const RetryConfig retry_configs[], RetryData retry_data[]);

/**
 * @brief Recovers from the protocol, sets the efuses that app_retry_handler_start turned off to on.
 *
 * @param efuse Which efuse protocol to run.
 * @param retry_configs The configurations for each efuse (efuse_state).
 * @param retry_data The data for each efuse (ProtocolState).
 */
void app_retryHandler_success(EfuseChannel efuse, const RetryConfig retry_configs[], RetryData retry_data[]);