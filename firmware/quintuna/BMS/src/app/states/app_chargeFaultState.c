static void app_chargeFaultStateRunOnEntry()
{

}

static void app_chargeFaultStateRunOnTick1Hz()
{

}

static void app_chargeFaultStateRunOnTick100Hz()
{

}

static void app_chargeFaultStateRunOnExit()
{

}

const State chargeFaultState_get(void)
{
    static State charge_fault_state = {
        .name = "CHARGE FAULT",
        .run_on_entry = app_chargeFaultStateRunOnEntry,
        .run_on_tick_1Hz = app_chargeFaultStateRunOnTick1Hz,
        .run_on_tick_100Hz = app_chargeFaultStateRunOnTick100Hz,
        .run_on_exit = app_chargeFaultStateRunOnExit,
    };

    return &charge_fault_state;
}