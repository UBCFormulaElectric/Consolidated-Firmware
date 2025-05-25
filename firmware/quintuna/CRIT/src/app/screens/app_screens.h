#include "io_rotary.h"

typedef struct
{
    RotaryCallback ccw_callback; // Called on counter clockwise rotary rotation.
    RotaryCallback cw_callback;  // Called on clockwise rotary rotation.
    RotaryCallback update;       // Called every 100Hz cycle to update/broadcast data.
} Screen;

extern Screen main_drive_screen;
extern Screen init_screen;
extern Screen start_up_screen;
extern Screen vd_screen;
extern Screen warning_screen;

void app_screens_init(void);
void app_screens_update(void);
