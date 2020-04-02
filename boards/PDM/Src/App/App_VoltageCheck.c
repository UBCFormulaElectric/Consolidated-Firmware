#include <stdlib.h>

#include "App_VoltageCheck.h"
#include "App_SharedAssert.h"

struct VoltageCheck
{
    float (*get_voltage)(void);
    float (*get_min_voltage)(void);
    float (*get_max_voltage)(void);
    enum VoltageCheck_Status status;
};

struct VoltageCheck *App_VoltageCheck_Create(
    float (*get_voltage)(void),
    float (*get_min_voltage)(void),
    float (*get_max_voltage)(void))
{
    struct VoltageCheck *voltage_check = malloc(sizeof(struct VoltageCheck));

    shared_assert(voltage_check != NULL);

    voltage_check->get_voltage     = get_voltage;
    voltage_check->get_min_voltage = get_min_voltage;
    voltage_check->get_max_voltage = get_max_voltage;
    voltage_check->status          = VOLTAGECHECK_NORMAL;

    return voltage_check;
}

void App_VoltageCheck_Tick(struct VoltageCheck *voltage_check)
{
    const float voltage     = voltage_check->get_voltage();
    const float min_voltage = voltage_check->get_min_voltage();
    const float max_voltage = voltage_check->get_max_voltage();

    if (voltage < min_voltage)
    {
        voltage_check->status = VOLTAGECHECK_UNDERVOLTAGE;
    }
    else if (voltage > max_voltage)
    {
        voltage_check->status = VOLTAGECHECK_OVERVOLTAGE;
    }
    else
    {
        voltage_check->status = VOLTAGECHECK_NORMAL;
    }
}

enum VoltageCheck_Status
    App_VoltageCheck_GetStatus(struct VoltageCheck *voltage_check)
{
    return voltage_check->status;
}

void App_VoltageCheck_Destroy(struct VoltageCheck *voltage_check)
{
    free(voltage_check);
}
