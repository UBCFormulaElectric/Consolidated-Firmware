#include "App_TractiveSystem.h"
#include <stdlib.h>
#include <assert.h>
#include "App_SharedMacros.h"

#define NUM_INVERTERS (2U)
#define INVERTER_CAPACITANCE_F (0.280e-3f)

struct TractiveSystem
{
    float (*get_system_voltage)(float);
    float (*get_adc_voltage)(void);
};

struct TractiveSystem *App_TractiveSystem_Create(
    float (*get_system_voltage)(float),
    float (*get_adc_voltage)(void))
{
    struct TractiveSystem *tractive_system =
        malloc(sizeof(struct TractiveSystem));
    assert(tractive_system != NULL);

    tractive_system->get_system_voltage = get_system_voltage;
    tractive_system->get_adc_voltage    = get_adc_voltage;

    return tractive_system;
}

void App_TractiveSystem_Destroy(struct TractiveSystem *tractive_system)
{
    free(tractive_system);
}

float App_TractiveSystem_GetSystemVoltage(const
    struct TractiveSystem *const tractive_system)
{
    return tractive_system->get_system_voltage(tractive_system->get_adc_voltage());
}

float App_TractiveSystem_GetSystemCapacitance(void)
{
    return INVERTER_CAPACITANCE_F * NUM_INVERTERS;
}
