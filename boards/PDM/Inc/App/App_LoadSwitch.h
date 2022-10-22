#include <stdbool.h>
struct LoadSwitch;

struct LoadSwitch *App_LoadSwitch_Create(bool (*has_current_fault)(void));

void App_LoadSwitch_Destroy(struct LoadSwitch *load_switch);

bool App_LowVoltageBattery_HasCurrentFault(const struct LoadSwitch *load_switch);



