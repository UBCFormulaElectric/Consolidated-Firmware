#pragma once

#define MIN_BRAKE_PRESSURE_PSI (0.0f)
#define MAX_BRAKE_PRESSURE_PSI (1000.0f)

namespace app::brake{
    /**
     * CAN BUS Broadcast Function
     */
    void broadcast(void);
}
