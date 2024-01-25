#include "App_InRangeCheck.h"
#include "App_CanTx.h"

enum InRangeCheck_Status CheckStatus(enum InRangeCheck_Status status, void (*const out_of_range_setter)(uint8_t))
{
    switch (status)
    {
        case VALUE_IN_RANGE:
        {
            out_of_range_setter(RANGE_CHECK_OK);
        }
        break;
        case VALUE_UNDERFLOW:
        {
            out_of_range_setter(RANGE_CHECK_UNDERFLOW);
        }
        break;
        case VALUE_OVERFLOW:
        {
            out_of_range_setter(RANGE_CHECK_OVERFLOW);
        }
        break;
    }

    return status;
}

enum InRangeCheck_Status App_SetPeriodicCanSignals_InRangeCheck_float(
    const struct InRangeCheck * in_range_check,
    void (*const can_signal_setter)(float),
    void (*const out_of_range_setter)(uint8_t))
{
    float                    value;
    enum InRangeCheck_Status status = App_InRangeCheck_GetValue(in_range_check, &value);

    CheckStatus(status, out_of_range_setter);

    can_signal_setter(value);
    return status;
}
enum InRangeCheck_Status App_SetPeriodicCanSignals_InRangeCheck_long(
    const struct InRangeCheck * in_range_check,
    void (*const can_signal_setter)(uint32_t),
    void (*const out_of_range_setter)(uint8_t))
{
    float                    value;
    enum InRangeCheck_Status status = App_InRangeCheck_GetValue(in_range_check, &value);

    CheckStatus(status, out_of_range_setter);

    can_signal_setter((uint32_t)value);
    return status;
}
