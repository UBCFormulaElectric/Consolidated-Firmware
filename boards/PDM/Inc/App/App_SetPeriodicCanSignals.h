#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_PdmWorld.h"

// Even if the value is out-of-range, we still use it to set the CAN TX signal
void App_SetPeriodicCanSignals_InRangeCheck(
    struct PdmCanTxInterface * can_tx,
    const struct InRangeCheck *in_range_check,
    void (*can_signal_setter)(struct PdmCanTxInterface *, float),
    void (*const underflow_setter)(struct PdmCanTxInterface *, uint8_t),
    void (*const overflow_setter)(struct PdmCanTxInterface *, uint8_t));

void App_SetPeriodicCanSignals_CurrentChecks(struct PdmWorld* world);
void App_SetPeriodicCanSignals_VoltageChecks(struct PdmWorld* world);
