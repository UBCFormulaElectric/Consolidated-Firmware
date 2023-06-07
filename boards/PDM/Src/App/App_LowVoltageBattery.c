#include <stdlib.h>
#include <assert.h>
#include "App_LowVoltageBattery.h"

struct LowVoltageBattery
{
    bool (*is_overvoltage)(void);
    bool (*has_charge_fault)(void);
    bool (*has_boost_fault)(void);
    float (*get_bat_voltage)(void);
    float (*get_boost_voltage)(void);
    float (*get_aux_voltage)(void);
};

struct LowVoltageBattery *App_LowVoltageBattery_Create(
    bool (*has_charge_fault)(void),
    bool (*has_boost_fault)(void),
    float (*get_bat_voltage)(void),
    float (*get_boost_voltage)(void),
    float (*get_aux_voltage)(void))
{
    struct LowVoltageBattery *low_voltage_battery = malloc(sizeof(struct LowVoltageBattery));
    assert(low_voltage_battery != NULL);

    low_voltage_battery->has_charge_fault  = has_charge_fault;
    low_voltage_battery->has_boost_fault   = has_boost_fault;
    low_voltage_battery->get_bat_voltage   = get_bat_voltage;
    low_voltage_battery->get_aux_voltage   = get_aux_voltage;
    low_voltage_battery->get_boost_voltage = get_boost_voltage;

    return low_voltage_battery;
}

void App_LowVoltageBattery_Destroy(struct LowVoltageBattery *low_voltage_battery)
{
    free(low_voltage_battery);
}

bool App_LowVoltageBattery_IsOvervoltage(const struct LowVoltageBattery *low_voltage_battery)
{
    return low_voltage_battery->is_overvoltage();
}

bool App_LowVoltageBattery_HasChargeFault(const struct LowVoltageBattery *low_voltage_battery)
{
    return low_voltage_battery->has_charge_fault();
}

bool App_LowVoltageBattery_HasBoostControllerFault(const struct LowVoltageBattery *low_voltage_battery)
{
    return low_voltage_battery->has_boost_fault();
}
float App_LowVoltageBattery_GetBatVoltage(struct LowVoltageBattery *low_voltage_battery)
{
    return low_voltage_battery->get_bat_voltage();
}

float App_LowVoltageBattery_GetBoostVoltage(struct LowVoltageBattery *low_voltage_battery)
{
    return low_voltage_battery->get_boost_voltage();
}

float App_LowVoltageBattery_GetAuxVoltage(struct LowVoltageBattery *low_voltage_battery)
{
    return low_voltage_battery->get_aux_voltage();
}
