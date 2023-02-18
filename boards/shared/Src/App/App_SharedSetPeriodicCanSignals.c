#include <stdint-gcc.h>
#include "App_InRangeCheck.h"

enum InRangeCheck_Status CheckStatus(enum InRangeCheck_Status status, void (*const out_of_range_setter)(uint8_t), uint8_t ok_choice,
                                     uint8_t underflow_choice,
                                     uint8_t overflow_choice){
    switch (status)
    {
        case VALUE_IN_RANGE:
        {
            out_of_range_setter(ok_choice);
        }
        break;
        case VALUE_UNDERFLOW:
        {
            out_of_range_setter(underflow_choice);
        }
        break;
        case VALUE_OVERFLOW:
        {
            out_of_range_setter(overflow_choice);
        }
        break;
    }
    return status;
}

enum InRangeCheck_Status App_SetPeriodicCanSignals_InRangeCheck_float(
    const struct InRangeCheck *in_range_check,
    void (*const can_signal_setter)(float),
    void (*const out_of_range_setter)(uint8_t),
    uint8_t ok_choice,
    uint8_t underflow_choice,
    uint8_t overflow_choice)
{
    float                    value;
    enum InRangeCheck_Status status = App_InRangeCheck_GetValue(in_range_check, &value);

    CheckStatus(status, out_of_range_setter, ok_choice, underflow_choice, overflow_choice);

    can_signal_setter(value);
    return status;
}
enum InRangeCheck_Status App_SetPeriodicCanSignals_InRangeCheck_long(
    const struct InRangeCheck *in_range_check,
    void (*const can_signal_setter)(uint32_t),
    void (*const out_of_range_setter)(uint8_t),
    uint8_t ok_choice,
    uint8_t underflow_choice,
    uint8_t overflow_choice)
{
    float value;
    enum InRangeCheck_Status status = App_InRangeCheck_GetValue(in_range_check, &value);

    CheckStatus(status, out_of_range_setter, ok_choice, underflow_choice, overflow_choice);

    can_signal_setter((uint32_t) value);
    return status;
}
