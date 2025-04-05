#include <stdint.h>
#include "io_rotary.h"

#define NUM_OF_SCREENS 5
#define DATA_LENGTH 9

typedef struct
{
    uint8_t       *display_data; // Pointer to the display data for a 7-seg display.
    RotaryCallback ccw_callback; // Called on counter-clockwise rotary rotation.
    RotaryCallback cw_callback;  // Called on clockwise rotary rotation.
} Screen;

extern Screen  main_drive;
extern Screen  idk;
extern Screen *screens[NUM_OF_SCREENS];

void app_screens_update(void);
void app_screens_handle_rotary_clockwise(void);
void app_screens_handle_rotary_ccw(void);
void app_screens_set_current(uint8_t screen_index);
void app_screens_next(void);
