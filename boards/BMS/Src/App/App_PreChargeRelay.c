#include <assert.h>
#include <stdlib.h>

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
