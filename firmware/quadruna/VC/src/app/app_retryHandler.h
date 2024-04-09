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
    PROTOCOL_STATE_OFF,                // efuse is not in retry protocol
    PROTOCOL_STATE_CALC_AVG,           // efuse is in process of calculating the avg current for this efuse
    PROTOCOL_STATE_CALC_DONE,          // efuse is done calculating the avg need to check on what to do next
    PROTOCOL_STATE_DEPENDENCY_WAITING, // this efuse is waiting for another efuse to finish its retry protocol
    PROTOCOL_STATE_DEBOUNCE            // waiting for debounce time before turning on the efuse again with standby reset
} ProtocolState;

typedef struct
{
    bool          efuse_state;
    RetryProtocol retry_protocol;
    int           retry_attempts_limit;
    float         min_needed_current;
} RetryConfig;

typedef struct
{
    ProtocolState protocol_state;
    int           retry_attempts;
    int           current_timer_attempts;
    float         current_sum;
    int           debounce_timer_attempts;
} RetryData;

/**
 * @note this initiates the retry protocol
 * @param protocol which efuse protocol to run
 * @param retry_config the configs for each efuse (efuse_state)
 * @param retry_data the data for each efuse (ProtocolState)
 * @return if should go to init state
 */
bool app_retryHandler_start(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[]);

/**
 * @note this recovers from the protocol sets the efuses that app_retry_handler_start turned off to on
 * @param protocol which efuse protocol to run
 * @param retry_config the configs for each efuse (efuse_state)
 * @param retry_data the data for each efuse (ProtocolState)
 */
void app_retryHandler_success(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[]);