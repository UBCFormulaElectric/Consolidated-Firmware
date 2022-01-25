#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "App_InRangeCheck.h"

struct InRangeCheck
{
    float (*get_value)(void);
    float min_value;
    float max_value;
};

struct InRangeCheck *App_InRangeCheck_Create(
    float (*const get_value)(void),
    float min_value,
    float max_value)
{
    assert(get_value != NULL);
    assert(min_value <= max_value);

    struct InRangeCheck *in_range_check = malloc(sizeof(struct InRangeCheck));
    assert(in_range_check != NULL);

    in_range_check->get_value = get_value;
    in_range_check->min_value = min_value;
    in_range_check->max_value = max_value;

    return in_range_check;
}

void App_InRangeCheck_Destroy(struct InRangeCheck *const in_range_check)
{
    free(in_range_check);
}

enum InRangeCheck_Status App_InRangeCheck_GetValue(
    const struct InRangeCheck *const in_range_check,
    float *                          returned_value)
{
    const float              value  = in_range_check->get_value();
    enum InRangeCheck_Status status = VALUE_IN_RANGE;

    if (value < in_range_check->min_value)
    {
        status = VALUE_UNDERFLOW;
    }
    else if (value > in_range_check->max_value)
    {
        status = VALUE_OVERFLOW;
    }

    *returned_value = value;

    return status;
}

enum InRangeCheck_Status App_InRangeCheck_SetPeriodicCanSignal(
    struct CAN_TX_INTERFACE *can_tx,
    float                    value,
    float                    min,
    float                    max,
    void (*out_of_range_setter)(struct CAN_TX_INTERFACE *, uint8_t),
    void (*can_signal_setter)(struct CAN_TX_INTERFACE *, float))
{
    uint8_t status = VALUE_IN_RANGE;

    if (value < min)
    {
        status = VALUE_UNDERFLOW;
    }
    else if (value > max)
    {
        status = VALUE_OVERFLOW;
    }

    out_of_range_setter(can_tx, (uint8_t)status);
    can_signal_setter(can_tx, value);

    return status;
}

enum InRangeCheck_Status App_SetPeriodicCanSignals_InRangeCheck(
    struct CAN_TX_INTERFACE *  can_tx,
    const struct InRangeCheck *in_range_check,
    void (*const can_signal_setter)(struct CAN_TX_INTERFACE *, float),
    void (*const out_of_range_setter)(struct CAN_TX_INTERFACE *, uint8_t),
    uint8_t ok_choice,
    uint8_t underflow_choice,
    uint8_t overflow_choice)
{
    UNUSED(ok_choice);
    UNUSED(underflow_choice);
    UNUSED(overflow_choice);

    float                    value;
    enum InRangeCheck_Status status =
        App_InRangeCheck_GetValue(in_range_check, &value);

    App_InRangeCheck_SetPeriodicCanSignal(
        can_tx, value, in_range_check->min_value, in_range_check->max_value,
        out_of_range_setter, can_signal_setter);

    return status;
}
