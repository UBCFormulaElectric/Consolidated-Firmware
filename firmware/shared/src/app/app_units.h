#pragma once

// Time conversions.
#define S_TO_MS(time_s) ((uint32_t)((time_s)*1000U))
#define MIN_TO_HOUR (60.0f)
#define SECONDS_TO_HOUR(s) ((s) * (1 / 3600.0f))
#define HZ_TO_MS(hz) (1000 / (hz))

// Length conversions.
#define INCH_TO_KM (0.0000254f)
#define IN_TO_M (0.0254f)

// Radial units/conversions.
#define PI (3.1415926f)
#define RAD_TO_DEG(r) (r * 180.0f / PI)
#define DEG_TO_RAD(degrees) ((degrees) * (float)M_PI / 180.0f)
#define RPM_TO_RADS(rpm) ((rpm) * (float)M_PI / 30.0f)

#define WHEEL_DIAMETER_IN (16.0f)
#define GEAR_RATIO (4.3f)
#define MOTOR_RPM_TO_KMH(kmh) ((kmh) * (float)WHEEL_DIAMETER_IN * PI * INCH_TO_KM * MIN_TO_HOUR / GEAR_RATIO)
#define MOTOR_KMH_TO_RPM(rpm) ((int)((rpm) / ((float)WHEEL_DIAMETER_IN * PI * INCH_TO_KM * MIN_TO_HOUR / GEAR_RATIO)))
