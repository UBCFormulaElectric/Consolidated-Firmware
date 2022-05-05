#pragma once

#define STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_IN_RANGE_CHECK(CAN_TX_INTERFACE)               \
    static enum InRangeCheck_Status App_SetPeriodicCanSignals_InRangeCheck(                       \
        struct CAN_TX_INTERFACE *can_tx, const struct InRangeCheck *in_range_check,               \
        void (*const can_signal_setter)(struct CAN_TX_INTERFACE *, float),                        \
        void (*const out_of_range_setter)(struct CAN_TX_INTERFACE *, uint8_t), uint8_t ok_choice, \
        uint8_t underflow_choice, uint8_t overflow_choice)                                        \
    {                                                                                             \
        float                    value;                                                           \
        enum InRangeCheck_Status status = App_InRangeCheck_GetValue(in_range_check, &value);      \
                                                                                                  \
        switch (status)                                                                           \
        {                                                                                         \
            case VALUE_IN_RANGE:                                                                  \
            {                                                                                     \
                out_of_range_setter(can_tx, ok_choice);                                           \
            }                                                                                     \
            break;                                                                                \
            case VALUE_UNDERFLOW:                                                                 \
            {                                                                                     \
                out_of_range_setter(can_tx, underflow_choice);                                    \
            }                                                                                     \
            break;                                                                                \
            case VALUE_OVERFLOW:                                                                  \
            {                                                                                     \
                out_of_range_setter(can_tx, overflow_choice);                                     \
            }                                                                                     \
            break;                                                                                \
        }                                                                                         \
                                                                                                  \
        can_signal_setter(can_tx, value);                                                         \
        return status;                                                                            \
    }
