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
    PROTOCOL_STATE_DONE_CALC, // efuse is done calculating the avg need to check on what to do next
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
 * 
*/
bool app_retry_handler_start(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[]);

/**
 * 
*/
void app_retry_handler_success(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[]);
