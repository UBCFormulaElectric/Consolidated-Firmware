
struct LoadSwitch;

struct LoadSwitch *App_LoadSwitch_Create(
    float (*get_AUX1_current)(void),
    float (*get_AUX2_current)(void),
    float (*get_LEFT_INVERTER_current)(void),
    float (*get_RIGHT_INVERTER_current)(void),
    float (*get_ENERGY_METER_current)(void),
    float (*get_CAN_current)(void),
    float (*get_AIR_SHUTDOWN_current)(void));

void App_LoadSwitch_Destroy(struct LoadSwitch *load_switch);

float App_LoadSwitch_Get_AUX1_Current(struct LoadSwitch *load_switch);
float App_LoadSwitch_Get_AUX2_Current(struct LoadSwitch *load_switch);
float App_LoadSwitch_Get_LEFT_INVERTER_Current(struct LoadSwitch *load_switch);
float App_LoadSwitch_Get_RIGHT_INVERTER_Current(struct LoadSwitch *load_switch);
float App_LoadSwitch_Get_ENERGY_METER_Current(struct LoadSwitch *load_switch);
float App_LoadSwitch_Get_CAN_Current(struct LoadSwitch *load_switch);
float App_LoadSwitch_Get_AIR_SHUTDOWN_Current(struct LoadSwitch *load_switch);

int  App_LoadSwitch_InRangeCheck(float value, float min_value, float max_value);
bool App_LoadSwitch_AreCurrentsInRange(struct LoadSwitch *load_switch);
