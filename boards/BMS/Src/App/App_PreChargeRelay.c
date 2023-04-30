#include "App_PreChargeRelay.h"
#include "App_CanAlerts.h"

#define MAX_PRECHARGE_ATTEMPTS 3U

struct PrechargeRelay
{
    void (*close_relay)(void);
    void (*open_relay)(void);
    uint8_t precharge_fault_count;
};

struct PrechargeRelay *App_PrechargeRelay_Create(void (*close_relay)(void), void (*open_relay)(void))
{
    struct PrechargeRelay *pre_charge_relay = malloc(sizeof(struct PrechargeRelay));
    assert(pre_charge_relay != NULL);

    pre_charge_relay->close_relay           = close_relay;
    pre_charge_relay->open_relay            = open_relay;
    pre_charge_relay->precharge_fault_count = 0U;
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

void App_PrechargeRelay_IncFaultCounter(struct PrechargeRelay *precharge_relay)
{
    precharge_relay->precharge_fault_count++;
}

uint8_t App_PrechargeRelay_GetFaultCounterVal(const struct PrechargeRelay *const precharge_relay)
{
    return precharge_relay->precharge_fault_count;
}

void App_PrechargeRelay_ResetFaultCounterVal(struct PrechargeRelay *const precharge_relay)
{
    precharge_relay->precharge_fault_count = 0U;
}

bool App_PrechargeRelay_CheckFaults(
    struct PrechargeRelay *precharge_relay,
    bool                   is_charger_connected,
    bool                   is_ts_rising_slowly,
    bool                   is_ts_rising_quickly,
    bool *                 precharge_limit_exceeded)
{
    const bool has_precharge_fault =
        (is_charger_connected) ? is_ts_rising_slowly : (is_ts_rising_slowly | is_ts_rising_quickly);

    if (has_precharge_fault)
    {
        App_PrechargeRelay_IncFaultCounter(precharge_relay);
    }

    *precharge_limit_exceeded = App_PrechargeRelay_GetFaultCounterVal(precharge_relay) >= MAX_PRECHARGE_ATTEMPTS;
    App_CanAlerts_SetFault(BMS_FAULT_PRECHARGE_ERROR, *precharge_limit_exceeded);

    return has_precharge_fault;
}
