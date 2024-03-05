#include <stdbool.h>
#pragma once

typedef enum
{
    RETRY_PROTOCOL_LV,
    RETRY_PROTOCOL_INVERTERS,
    RETRY_PROTOCOL_AIR,
    RETRY_PROTOCOL_AUX,
    RETRY_PROTOCOL_FANS,
    RETRY_PROTOCOL_EMETER,
    RETRY_PROTOCOL_DRS,
    NUM_RETRY_PROTOCOL
} RetryProtocol;

typedef struct {
    bool          state;
    RetryProtocol protocol;
    int           limit;
} RetryConfig;

void apply_retry_protocol(RetryProtocol protocol, const RetryConfig retry_config[], bool success);
