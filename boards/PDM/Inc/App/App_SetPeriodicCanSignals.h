#pragma once

struct PdmCanTxInterface;
struct InRangeCheck;

void App_SetPeriodicCanSignals_InRangeCheck(
    struct PdmCanTxInterface * can_tx,
    const struct InRangeCheck *in_range_check,
    void (*can_signal_setter)(struct PdmCanTxInterface *, float));
