#pragma once

typedef struct
{
    bool imuOk;
    bool steeringOk;
    bool gpsOk;
    bool useTV;
} SensorChecks;

void app_torqueVectoring_init(void);
void app_torqueVectoring_run(float accelerator_pedal_percentage);
void app_torqueVectoring_handleAcceleration(void);
