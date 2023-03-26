#pragma once

#include <stdbool.h>

typedef struct
{
    float x;
    float y;
    float z;
} Vector3;

typedef struct
{
    float roll;
    float pitch;
    float yaw;
} Attitude;

/*
 * Initialize the interface to the SBG Ellipse N GPS sensor.
 */
bool Io_SbgEllipseGps_Init(void);

void Io_SbgEllipseGps_Process(void);

void Io_SbgEllipseGps_GetAttitude(Attitude *attitude);
