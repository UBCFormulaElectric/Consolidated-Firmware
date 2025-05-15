static void app_prechargeStateRunOnEntry()
{

}

static void app_prechargeStateRunOnTick1Hz()
{

}

static void app_prechargeStateRunOnTick100Hz()
{

}

static void app_prechargeStateRunOnExit()
{

}

const State *app_prechargeState_get(void)
{
    static State precharge_state = {
        .name               = "PRECHARGE",
        .run_on_entry       = app_prechargeStateRunOnEntry,
        .run_on_tick_1Hz    = app_prechargeStateRunOnTick1Hz,
        .run_on_tick_100Hz  = app_prechargeStateRunOnTick100Hz,
        .run_on_exit        = app_prechargeStateRunOnExit,
    };

    return &precharge_state;
}