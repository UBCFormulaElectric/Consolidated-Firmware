#include <stdbool.h>
#pragma once

typedef enum
{
    RETRY_PROTOCOL_LV,
    RETRY_PROTOCOL_DI_LHS,
    RETRY_PROTOCOL_DI_RHS,
    RETRY_PROTOCOL_AIR,
    RETRY_PROTOCOL_AUX,
    RETRY_PROTOCOL_FANS,
    RETRY_PROTOCOL_EMETER,
    RETRY_PROTOCOL_DRS,
    NUM_RETRY_PROTOCOL
} RetryProtocol;

typedef enum
{
    PROTOCOL_STATE_OFF,       // efuse is not in retry protocol
    PROTOCOL_STATE_CALC_AVG,  // efuse is in process of calculating the avg current for this efuse
    PROTOCOL_STATE_CALC_DONE, // efuse is done calculating the avg need to check on what to do next
    PROTOCOL_STATE_WAITING    // this efuse is waiting for another efuse to finish its retry protocol
} ProtocolState;

typedef struct
{
    bool          efuse_state;
    RetryProtocol retry_protocol;
    int           retry_attempts_limit;
    float         min_needed_current;
    int           timer_attempts_limit;
} RetryConfig;

typedef struct
{
    ProtocolState protocol_state;
    int           retry_attempts;
    int           timer_attempts;
    float         current_sum;
} RetryData;

/**
 * @note this initiates the retry protocol
 * @param protocol which efuse protocol to run
 * @param retry_config the configs for each efuse (efuse_state)
 * @param retry_data the data for each efuse (ProtocolState)
 * @return if should go to init state
 */
bool app_retry_handler_start(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[]);

/**
 * @note this recovers from the protocol sets the efuses that app_retry_handler_start turned off to on
 * @param protocol which efuse protocol to run
 * @param retry_config the configs for each efuse (efuse_state)
 * @param retry_data the data for each efuse (ProtocolState)
 */
void app_retry_handler_success(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[]);
