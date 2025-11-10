#pragma once
#include <stdbool.h>

namespace io::suspension {
    /**
     * Get the travel of the front left suspension
     * @return The travel of the suspension in (TODO: add units)
     */
    float getLeftTravel(void);

    /**
     * Get the travel of the front right suspension
     * @return The travel of the suspension in (TODO: add units)
     */
    float getRightTravel(void);

    /**
     * @return whether the left suspension sensor is open or short circuit
     */
    bool leftSensorOCSC(void);

    /**
     * @return whether the right suspension sensor is open or short circuit
     */
    bool rightSensorOCSC(void);
}