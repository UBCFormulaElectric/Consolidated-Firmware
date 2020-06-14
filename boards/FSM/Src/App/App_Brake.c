#include <stdlib.h>
#include <assert.h>
#include "App_SharedBinaryStatus.h"
#include "App_InRangeCheck.h"

struct Brake
{
    struct InRangeCheck *brake_pressure_in_range_check;
    struct BinaryStatus *is_brake_open_or_short_circuit_binary_status,
        *is_brake_actuated_binary_status;

    float min_brake_pressure_psi;
    float max_brake_pressure_psi;

    float (*get_pressure_psi)(void);
    bool (*pressure_sensor_has_open_or_short_circuit)(void);
    bool (*is_brake_actuated)(void);
};

struct Brake *App_Brake_Create(
    float (*get_pressure_psi)(void),
    bool (*pressure_sensor_has_open_or_short_circuit)(void),
    bool (*is_brake_actuated)(void),
    float min_brake_pressure_psi,
    float max_brake_pressure_psi)
{
    struct Brake *brake = malloc(sizeof(struct Brake));
    assert(brake != NULL);

    brake->brake_pressure_in_range_check = App_InRangeCheck_Create(
        get_pressure_psi, min_brake_pressure_psi, max_brake_pressure_psi);
    brake->is_brake_open_or_short_circuit_binary_status =
        App_SharedBinaryStatus_Create(
            pressure_sensor_has_open_or_short_circuit);
    brake->is_brake_actuated_binary_status =
        App_SharedBinaryStatus_Create(is_brake_actuated);

    brake->min_brake_pressure_psi = min_brake_pressure_psi;
    brake->max_brake_pressure_psi = max_brake_pressure_psi;

    brake->get_pressure_psi = get_pressure_psi;
    brake->pressure_sensor_has_open_or_short_circuit =
        pressure_sensor_has_open_or_short_circuit;
    brake->is_brake_actuated = is_brake_actuated;

    return brake;
}

struct InRangeCheck *
    App_Brake_GetBrakePressureInRangeCheck(const struct Brake *const brake)
{
    return brake->brake_pressure_in_range_check;
}

struct BinaryStatus *
    App_Brake_GetIsBrakeActuatedBinaryStatus(const struct Brake *const brake)
{
    return brake->is_brake_actuated_binary_status;
}

struct BinaryStatus *
    App_Brake_GetIsPressureSensorOpenOrShortCircuitBinaryStatus(
        const struct Brake *const brake)
{
    return brake->is_brake_open_or_short_circuit_binary_status;
}

void App_Brake_Destroy(struct Brake *brake)
{
    App_InRangeCheck_Destroy(brake->brake_pressure_in_range_check);
    App_SharedBinaryStatus_Destroy(brake->is_brake_actuated_binary_status);
    App_SharedBinaryStatus_Destroy(
        brake->is_brake_open_or_short_circuit_binary_status);

    free(brake);
}
