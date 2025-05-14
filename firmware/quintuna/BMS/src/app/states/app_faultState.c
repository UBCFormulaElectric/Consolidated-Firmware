static void app_faultStateRunOnEntry()
{

}

static void app_faultStateRunOnTick1Hz()
{

}

static void app_faultStateRunOnTick100Hz()
{

}

static void app_faultStateRunOnExit()
{

}

const State *app_faultState_get(void)
{
    static State fault_state = {
        .name               = "FAULT",
        .run_on_entry       = app_faultStateRunOnEntry,
        .run_on_tick_1Hz    = app_faultStateRunOnTick1Hz,
        .run_on_tick_100Hz  = app_faultStateRunOnTick100Hz,
        .run_on_exit        = app_faultStateRunOnExit,
    };

    return &fault_state;
}