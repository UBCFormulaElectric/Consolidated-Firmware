#pragma once

// TODO: angles need to be validated
#define MIN_STEERING_ANGLE_DEG (-95.0f)
#define MAX_STEERING_ANGLE_DEG (95.0f)

namespace app::steering{
    /**
     * CAN BUS Broadcast Function
     */
    void broadcast(void);
}
