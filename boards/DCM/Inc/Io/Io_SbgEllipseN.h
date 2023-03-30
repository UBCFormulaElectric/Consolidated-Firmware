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
bool Io_SbgEllipseN_Init(void);

void Io_SbgEllipseN_HandleLogs(void);

void Io_SbgEllipseN_GetAttitude(Attitude *attitude);
