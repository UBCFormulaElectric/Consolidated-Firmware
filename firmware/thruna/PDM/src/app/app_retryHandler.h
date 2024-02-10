#include <stdbool.h>

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

void apply_retry_protocol(RetryProtocol protocol, bool isOn);
