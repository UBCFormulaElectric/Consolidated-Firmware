static void app_chargeStateRunOnEntry()
{

}

static void app_chargeStateRunOnTick1Hz()
{

}

static void app_chargeStateRunOnTick100Hz()
{

}

static void app_chargeStateRunOnExit()
{

}

const State *app_chargeState_get(void)
{
    static State charge_state = {
        .name               = "CHARGE",
        .run_on_entry       = app_chargeStateRunOnEntry,
        .run_on_tick_1Hz    = app_chargeStateRunOnTick1Hz,
        .run_on_tick_100Hz  = app_chargeStateRunOnTick100Hz,
        .run_on_exit        = app_chargeStateRunOnExit,
    };

    return &charge_state;
}