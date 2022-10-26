#include <assert.h>
#include <stdlib.h>
#include "App_PreChargeRelay.h"

struct PrechargeRelay
{
    void (*close_relay)(void);
    void (*open_relay)(void);
};

struct PrechargeRelay *App_PrechargeRelay_Create(void (*close_relay)(void), void (*open_relay)(void))
{
    struct PrechargeRelay *pre_charge_relay = malloc(sizeof(struct PrechargeRelay));
    assert(pre_charge_relay != NULL);

    pre_charge_relay->close_relay = close_relay;
    pre_charge_relay->open_relay  = open_relay;

    return pre_charge_relay;
}

void App_PrechargeRelay_Destroy(struct PrechargeRelay *precharge_relay)
{
    free(precharge_relay);
}

void App_PrechargeRelay_Close(const struct PrechargeRelay *const precharge_relay)
{
    precharge_relay->close_relay();
}

void App_PrechargeRelay_Open(const struct PrechargeRelay *const precharge_relay)
{
    precharge_relay->open_relay();
}

bool App_PrechargeRelay_CheckFaults(
    struct BmsCanTxInterface *can_tx,
    bool                      is_charger_connected,
    bool                      is_ts_rising_slowly,
    bool                      is_ts_rising_quickly)
{
    static uint8_t precharge_fault_count    = 0;
    bool           precharge_shutdown_fault = false;
    bool           has_precharge_fault =
        (is_charger_connected) ? is_ts_rising_slowly : (is_ts_rising_slowly | is_ts_rising_quickly);

    if (has_precharge_fault)
    {
        precharge_fault_count++;
    }
    else
    {
        precharge_fault_count = 0;
    }

    if (precharge_fault_count >= 3)
    {
        precharge_shutdown_fault = true;
    }

    App_CanTx_SetPeriodicSignal_PRECHARGE_FAULT(can_tx, precharge_shutdown_fault);
    return precharge_shutdown_fault;
}
