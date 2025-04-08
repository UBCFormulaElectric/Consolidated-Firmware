#include <stdint.h>
#include "io_rotary.h"

#define NUM_OF_SCREENS 5
#define DATA_LENGTH 9

typedef struct
{
    uint8_t       *display_data; // Pointer to the display data for a 7-seg. display.
    RotaryCallback ccw_callback; // Called on counter clockwise rotary rotation.
    RotaryCallback cw_callback;  // Called on clockwise rotary rotation.
    RotaryCallback update;       // Called every 100Hz cycle to update data.
} Screen;

extern Screen main_drive;
extern Screen accel;

void app_screens_init(void);
void app_screens_update(void);