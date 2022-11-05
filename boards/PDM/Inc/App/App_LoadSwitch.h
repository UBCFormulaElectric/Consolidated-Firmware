#include <stdbool.h>
struct LoadSwitch;

struct LoadSwitch *App_LoadSwitch_Create(
        float (*get_CS_AIR_current)(void),
        float (*get_CS_LVPWR_current)(void),
        float (*get_CS_AUX1_current)(void),
        float (*get_CS_AUX2_current)(void),
        float (*get_CS_FRONTLHS_current)(void),
        float (*get_CS_FRONTRHS_current)(void),
        float (*get_CS_REARLHS_current)(void),
        float (*get_CS_REARRHS_current)(void));

void App_LoadSwitch_Destroy(struct LoadSwitch *load_switch);

float App_LoadSwitch_Get_CS_AIR_Current(struct LoadSwitch *load_switch);
float App_LoadSwitch_Get_CS_LVPWR_Current(struct LoadSwitch *load_switch);

float App_LoadSwitch_Get_CS_AUX1_Current(struct LoadSwitch *load_switch);
float App_LoadSwitch_Get_CS_AUX2_Current(struct LoadSwitch *load_switch);

float App_LoadSwitch_Get_CS_FRONTLHS_Current(struct LoadSwitch *load_switch);
float App_LoadSwitch_Get_CS_FRONTRHS_Current(struct LoadSwitch *load_switch);

float App_LoadSwitch_Get_CS_REARLHS_Current(struct LoadSwitch *load_switch);
float App_LoadSwitch_Get_CS_REARRHS_Current(struct LoadSwitch *load_switch);



