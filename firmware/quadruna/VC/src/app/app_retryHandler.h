#include <stdbool.h>
#pragma once

typedef enum
{
    RETRY_PROTOCOL_SHDN,
    RETRY_PROTOCOL_LV,
    RETRY_PROTOCOL_PUMP,
    RETRY_PROTOCOL_INV_R,
    RETRY_PROTOCOL_INV_L,
    RETRY_PROTOCOL_NONE,
    NUM_RETRY_PROTOCOL
} RetryProtocol;

typedef enum
{
    PROTOCOL_STATE_OFF,                // Efuse is not in a retry protocol
    PROTOCOL_STATE_CALC_AVG,           // Efuse is calculating the average current
    PROTOCOL_STATE_CALC_DONE,          // Efuse has finished calculating the average and needs further action
    PROTOCOL_STATE_DEPENDENCY_WAITING, // Efuse is waiting for another efuse to finish its retry protocol
    PROTOCOL_STATE_DEBOUNCE            // Efuse is waiting for debounce time before turning on again
} ProtocolState;

typedef struct
{
    bool          efuse_state;          // State of the efuse (on/off)
    RetryProtocol retry_protocol;       // Protocol for efuse retry handling
    int           retry_attempts_limit; // Limit for retry protocol attempts
    float         min_needed_current;   // Minimum current required for the efuse
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
 * @param protocol Which efuse protocol to run.
 * @param retry_configs The configurations for each efuse (efuse_state).
 * @param retry_data The data for each efuse (ProtocolState).
 * @return If VC should go to the init state.
 */
bool app_retryHandler_start(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[]);

/**
 * @brief Recovers from the protocol, sets the efuses that app_retry_handler_start turned off to on.
 *
 * @param protocol Which efuse protocol to run.
 * @param retry_configs The configurations for each efuse (efuse_state).
 * @param retry_data The data for each efuse (ProtocolState).
 */
void app_retryHandler_success(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[]);