#include "app_driveMode.h"
#include "app_canTx.h"
#include "io_rotary.h"
#include <stdint.h>

#define NUM_MODES 7

void app_driveMode_broadcast(void)
{
    const uint32_t encoder_position = io_rotary_toggleEventHandler();
    const uint8_t  mode             = (uint8_t)((encoder_position % NUM_MODES) + NUM_MODES) % NUM_MODES;
    app_canTx_CRIT_DriveMode_set((DriveMode_e)mode);
}
