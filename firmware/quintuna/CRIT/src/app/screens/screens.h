#include "io/rotary.h"

typedef struct
{
    RotaryCallback ccw_callback; // Called on counter clockwise rotary rotation.
    RotaryCallback cw_callback;  // Called on clockwise rotary rotation.
    RotaryCallback update;       // Called every 100Hz cycle to update/broadcast data.
} Screen;

extern Screen main_drive_screen;
extern Screen drive_modes_screen;
extern Screen start_up_screen;
extern Screen init_screen;

void app_screens_init(void);
void app_screens_update(void);
