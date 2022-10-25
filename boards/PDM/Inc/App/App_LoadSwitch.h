#include <stdbool.h>
struct LoadSwitch;

struct LoadSwitch *App_LoadSwitch_Create(float (*get_CS1_voltage)(void), float (*get_CS2_voltage)(void));

void App_LoadSwitch_Destroy(struct LoadSwitch *load_switch);

float App_LowVoltageBattery_GetCS1Voltage(struct LoadSwitch *load_switch);

float App_LowVoltageBattery_GetCS2Voltage(struct LoadSwitch *load_switch);



