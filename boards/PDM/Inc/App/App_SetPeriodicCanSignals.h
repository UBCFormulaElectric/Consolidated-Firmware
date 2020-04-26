#pragma once

struct PdmCanTxInterface;
struct InRangeCheck;

// Even if the value is out-of-range, we still use it to set the CAN TX signal
void App_SetPeriodicCanSignals_InRangeCheck(
    struct PdmCanTxInterface * can_tx,
    const struct InRangeCheck *in_range_check,
    void (*can_signal_setter)(struct PdmCanTxInterface *, float));
