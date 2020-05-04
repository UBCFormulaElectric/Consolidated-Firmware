#pragma once

#define STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(            \
    CAN_INTERFACE_TYPE)                                                       \
    static void App_SetPeriodicCanSignals_InRangeCheck(                       \
        struct CAN_INTERFACE_TYPE *const can_tx,                              \
        const struct InRangeCheck *const in_range_check,                      \
        void (*const can_signal_setter)(struct CAN_INTERFACE_TYPE *, float),  \
        void (*const underflow_setter)(struct CAN_INTERFACE_TYPE *, uint8_t), \
        void (*const overflow_setter)(struct CAN_INTERFACE_TYPE *, uint8_t))  \
    {                                                                         \
        float                    value;                                       \
        enum InRangeCheck_Status status =                                     \
            App_InRangeCheck_GetValue(in_range_check, &value);                \
                                                                              \
        switch (status)                                                       \
        {                                                                     \
            case VALUE_IN_RANGE:                                              \
            {                                                                 \
                underflow_setter(can_tx, false);                              \
                overflow_setter(can_tx, false);                               \
            }                                                                 \
            break;                                                            \
            case VALUE_UNDERFLOW:                                             \
            {                                                                 \
                underflow_setter(can_tx, true);                               \
            }                                                                 \
            break;                                                            \
            case VALUE_OVERFLOW:                                              \
            {                                                                 \
                overflow_setter(can_tx, true);                                \
            }                                                                 \
            break;                                                            \
        }                                                                     \
                                                                              \
        can_signal_setter(can_tx, value);                                     \
    }
