#pragma once

enum InRangeCheck_Status App_SetPeriodicCanSignals_InRangeCheck_long(
    const struct InRangeCheck* in_range_check,
    void (*const can_signal_setter)(uint32_t),
    void (*const out_of_range_setter)(uint8_t));

enum InRangeCheck_Status App_SetPeriodicCanSignals_InRangeCheck_float(
    const struct InRangeCheck* in_range_check,
    void (*const can_signal_setter)(float),
    void (*const out_of_range_setter)(uint8_t));
