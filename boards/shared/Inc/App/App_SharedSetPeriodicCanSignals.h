#pragma once

#include <stdint.h>

#define STATIC_DEFINE_APP_SET_PERIODIC_CAN_SIGNALS_BINARY_STATUS(            \
    CAN_TX_INTERFACE)                                                        \
    static void App_SetPeriodicCanSignals_SharedBinaryStatus(                \
        struct CAN_TX_INTERFACE *  can_tx,                                   \
        const struct BinaryStatus *binary_status,                            \
        void (*const can_signal_setter)(struct CAN_TX_INTERFACE *, uint8_t), \
        uint8_t on_choice, uint8_t off_choice)                               \
    {                                                                        \
        if (App_SharedBinaryStatus_IsActive(binary_status))                  \
        {                                                                    \
            can_signal_setter(can_tx, on_choice);                            \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            can_signal_setter(can_tx, off_choice);                           \
        }                                                                    \
    }
