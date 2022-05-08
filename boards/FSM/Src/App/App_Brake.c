#include <stdlib.h>
#include <assert.h>
#include "App_InRangeCheck.h"
#include "App_Brake.h"

struct Brake
{
    struct InRangeCheck *pressure_in_range_check;

    float min_pressure_psi;
    float max_pressure_psi;

    float (*get_pressure_psi)(void);
    bool (*is_pressure_sensor_open_or_short_circuit)(void);
    bool (*is_brake_actuated)(void);
};

struct Brake *App_Brake_Create(
    float (*get_pressure_psi)(void),
    bool (*is_pressure_sensor_open_or_short_circuit)(void),
    bool (*is_brake_actuated)(void),
    float min_pressure_psi,
    float max_pressure_psi)
{
    struct Brake *brake = malloc(sizeof(struct Brake));
    assert(brake != NULL);

    brake->pressure_in_range_check = App_InRangeCheck_Create(get_pressure_psi, min_pressure_psi, max_pressure_psi);
    brake->min_pressure_psi        = min_pressure_psi;
    brake->max_pressure_psi        = max_pressure_psi;
    brake->get_pressure_psi        = get_pressure_psi;
    brake->is_pressure_sensor_open_or_short_circuit = is_pressure_sensor_open_or_short_circuit;
    brake->is_brake_actuated                        = is_brake_actuated;

    return brake;
}

void App_Brake_Destroy(struct Brake *brake)
{
    App_InRangeCheck_Destroy(brake->pressure_in_range_check);
    free(brake);
}

struct InRangeCheck *App_Brake_GetPressureInRangeCheck(const struct Brake *const brake)
{
    return brake->pressure_in_range_check;
}

bool App_Brake_IsBrakeActuated(const struct Brake *brake)
{
    return brake->is_brake_actuated();
}

bool App_Brake_IsPressureSensorOpenOrShortCircuit(const struct Brake *brake)
{
    return brake->is_pressure_sensor_open_or_short_circuit();
}
