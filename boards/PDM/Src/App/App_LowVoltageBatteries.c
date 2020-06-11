#include <stdlib.h>
#include <assert.h>
#include "App_LowVoltageBatteries.h"

struct LowVoltageBatteries
{
    bool (*is_overvoltage)(void);
    bool (*has_charge_fault)(void);
    bool (*has_boost_fault)(void);
};

struct LowVoltageBatteries *App_LowVoltageBatteries_Create(
    bool (*is_overvoltage)(void),
    bool (*has_charge_fault)(void),
    bool (*has_boost_fault)(void))
{
    struct LowVoltageBatteries *low_voltage_batteries =
        malloc(sizeof(struct LowVoltageBatteries));
    assert(low_voltage_batteries != NULL);

    low_voltage_batteries->is_overvoltage   = is_overvoltage;
    low_voltage_batteries->has_charge_fault = has_charge_fault;
    low_voltage_batteries->has_boost_fault  = has_boost_fault;

    return low_voltage_batteries;
}

void App_LowVoltageBatteries_Destroy(
    struct LowVoltageBatteries *low_voltage_batteries)
{
    free(low_voltage_batteries);
}

bool App_LowVoltageBatteries_IsOvervoltage(
    const struct LowVoltageBatteries *low_voltage_batteries)
{
    return low_voltage_batteries->is_overvoltage();
}

bool App_LowVoltageBatteries_HasChargeFault(
    const struct LowVoltageBatteries *low_voltage_batteries)
{
    return low_voltage_batteries->has_charge_fault();
}

bool App_LowVoltageBatteries_HasBoostControllerFault(
    const struct LowVoltageBatteries *low_voltage_batteries)
{
    return low_voltage_batteries->has_boost_fault();
}
