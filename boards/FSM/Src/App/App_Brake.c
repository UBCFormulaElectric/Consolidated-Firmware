#include <stdlib.h>
#include <assert.h>

#include "configs/App_BrakePressureThresholds.h"
#include "App_InRangeCheck.h"
#include "App_Brake.h"

#define BRAKE_PRESSURE_SENSOR_MAX_V (5.0f)
#define BRAKE_PRESSURE_SC_THRESHOLD_V (4.6f)
#define BRAKE_PRESSURE_OC_THRESHOLD_V (0.4f)
#define BRAKE_PRESSURE_SC_THRESHOLD \
    (MAX_BRAKE_PRESSURE_PSI * BRAKE_PRESSURE_SC_THRESHOLD_V / BRAKE_PRESSURE_SENSOR_MAX_V)
#define BRAKE_PRESSURE_OC_THRESHOLD \
    (MAX_BRAKE_PRESSURE_PSI * BRAKE_PRESSURE_OC_THRESHOLD_V / BRAKE_PRESSURE_SENSOR_MAX_V)

struct Brake
{
    struct InRangeCheck *pressure_in_range_check;

    float min_pressure_psi;
    float max_pressure_psi;

    float (*get_primary_pressure_psi)(void);
    float (*get_secondary_pressure_psi)(void);
    bool (*is_primary_pressure_sensor_open_or_short_circuit)(void);
    bool (*is_secondary_pressure_sensor_open_or_short_circuit)(void);
    float (*pedal_angle)(void);
    bool (*is_pedal_angle_sensor_open_or_short_circuit)(void);
    bool (*is_brake_actuated)(void);
};

struct Brake *App_Brake_Create(
    float (*get_primary_pressure_psi)(void),
    float (*get_secondary_pressure_psi)(void),
    bool (*is_primary_pressure_sensor_open_or_short_circuit)(void),
    bool (*is_secondary_sensor_open_or_short_circuit)(void),
    float (*pedal_angle)(void),
    bool (*is_pedal_angle_sensor_open_or_short_circuit)(void),
    bool (*is_brake_actuated)(void),
    float min_pressure_psi,
    float max_pressure_psi)
{
    struct Brake *brake = malloc(sizeof(struct Brake));
    assert(brake != NULL);

    brake->pressure_in_range_check = App_InRangeCheck_Create(get_primary_pressure_psi, min_pressure_psi, max_pressure_psi);
    brake->min_pressure_psi        = min_pressure_psi;
    brake->max_pressure_psi        = max_pressure_psi;
    brake->get_primary_pressure_psi = get_primary_pressure_psi;
    brake->get_secondary_pressure_psi = get_secondary_pressure_psi;
    brake->is_primary_pressure_sensor_open_or_short_circuit = is_primary_pressure_sensor_open_or_short_circuit;
    brake->is_secondary_pressure_sensor_open_or_short_circuit = is_secondary_sensor_open_or_short_circuit;
    brake->pedal_angle             = pedal_angle;
    brake->is_pedal_angle_sensor_open_or_short_circuit = is_pedal_angle_sensor_open_or_short_circuit;
    brake->is_brake_actuated       = is_brake_actuated;

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
    //TODO perhaps rethink how this is implemented
    return brake->is_primary_pressure_sensor_open_or_short_circuit() || brake->is_secondary_pressure_sensor_open_or_short_circuit();
}
bool App_Brake_IsPressureSensorOpenCircuit(const struct Brake *brake)
{
    return brake->get_primary_pressure_psi() < BRAKE_PRESSURE_OC_THRESHOLD;
}
bool App_Brake_IsPressureSensorShortCircuited(const struct Brake *brake)
{
    return brake->get_primary_pressure_psi() > BRAKE_PRESSURE_SC_THRESHOLD;
}
//TODO Implement as signal??
bool App_Brake_IsPedalSensorOpenOrShortCircuit(const struct Brake *brake){
    return brake->is_pedal_angle_sensor_open_or_short_circuit();
}
