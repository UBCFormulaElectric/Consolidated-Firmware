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
    RETRY_STATE_OFF,
    RETRY_STATE_RUNNING,
    RETRY_STATE_EXPIRED,
    RETRY_STATE_WAITING
} RetryState;

typedef struct
{
    bool          efuse_state;
    RetryProtocol retry_protocol;
    int           retry_limit;
} RetryConfig;

typedef struct
{
    RetryState retry_state;
    int        retry_attempts;
    int        timer_attempts;
    int        timer_limit;
    float      current_sum;
} RetryData;

void init_retry_protocol(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[]);

void recover_retry_protocol(RetryProtocol protocol, const RetryConfig retry_configs[], RetryData retry_data[]);
