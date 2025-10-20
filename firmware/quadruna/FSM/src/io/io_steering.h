#pragma once
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_adc.h"

typedef struct
{
    const AdcChannel *steering;
} SteeringConfig;

#else
#include "app_utils.h"
EMPTY_STRUCT(SteeringConfig)
#endif

/**
 * Initialize pins for steering sensor
 * @param steering_config wrapper around steering adc pin
 */
void io_steering_init(const SteeringConfig *steering_config);

/**
 * Get the steering angle in degrees from the steering angle sensor
 * @return The steering angle of the steering angle sensor in degrees
 */
float io_steering_getAngleDegrees(void);

/**
 * @return whether or not the steering sensor is open or short circuit
 */
bool io_steering_sensorOCSC(void);
