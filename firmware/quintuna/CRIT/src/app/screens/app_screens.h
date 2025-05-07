#include <stdint.h>
#include "io_rotary.h"

// clang-format off
#define SEG_D   (1<<0)   // DRAIN0
#define SEG_E   (1<<1)   // DRAIN1
#define SEG_G   (1<<2)   // DRAIN2
#define SEG_F   (1<<3)   // DRAIN3
#define SEG_A   (1<<4)   // DRAIN4
#define SEG_B   (1<<5)   // DRAIN5
#define SEG_DP  (1<<6)   // DRAIN6
#define SEG_C   (1<<7)   // DRAIN7


typedef enum {
    DISP_0 = 0u, DISP_1, DISP_2, DISP_3, DISP_4,
    DISP_5, DISP_6, DISP_7, DISP_8, DISP_9,
    DISP_A, DISP_B, DISP_C, DISP_D, DISP_E, DISP_F,
    DISP_G, DISP_H, DISP_I, DISP_J, DISP_K, DISP_L,
    DISP_M, DISP_N, DISP_O, DISP_P, DISP_Q, DISP_R,
    DISP_S, DISP_T, DISP_U, DISP_V, DISP_W, DISP_X,
    DISP_Y, DISP_Z, DISP_DP, DISP_COUNT,
} SevenSegSymbol;

static const uint8_t SEGMENT_DICT[DISP_COUNT] = {
    [DISP_0] = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F),
    [DISP_1] = (SEG_B | SEG_C),
    [DISP_2] = (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G),
    [DISP_3] = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_G),
    [DISP_4] = (SEG_B | SEG_C | SEG_F | SEG_G),
    [DISP_5] = (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G),
    [DISP_6] = (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),
    [DISP_7] = (SEG_A | SEG_B | SEG_C),
    [DISP_8] = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),
    [DISP_9] = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G),
    [DISP_A] = (SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G),
    [DISP_B] = (SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),
    [DISP_C] = (SEG_A | SEG_D | SEG_E | SEG_F),
    [DISP_D] = (SEG_B | SEG_C | SEG_D | SEG_E | SEG_G),
    [DISP_E] = (SEG_A | SEG_D | SEG_E | SEG_F | SEG_G),
    [DISP_F] = (SEG_A | SEG_E | SEG_F | SEG_G),
    [DISP_G] = (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F),
    [DISP_H] = (SEG_B | SEG_C | SEG_E | SEG_F | SEG_G),
    [DISP_I] = (SEG_B | SEG_C),
    [DISP_J] = (SEG_B | SEG_C | SEG_D | SEG_E),
    [DISP_K] = (SEG_A | SEG_B | SEG_E | SEG_F | SEG_G),
    [DISP_L] = (SEG_D | SEG_E | SEG_F),
    [DISP_M] = (SEG_A | SEG_B | SEG_C | SEG_E | SEG_F),
    [DISP_N] = (SEG_A | SEG_B | SEG_C | SEG_E | SEG_F),
    [DISP_O] = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F),
    [DISP_P] = (SEG_A | SEG_B | SEG_E | SEG_F | SEG_G),
    [DISP_Q] = (SEG_A | SEG_B | SEG_C | SEG_F | SEG_G),
    [DISP_R] = (SEG_E | SEG_G), 
    [DISP_S] = (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G), 
    [DISP_T] = (SEG_D | SEG_E | SEG_F | SEG_G),
    [DISP_U] = (SEG_B | SEG_C | SEG_D | SEG_E | SEG_F),
    [DISP_V] = (SEG_C | SEG_D | SEG_E),
    [DISP_W] = (SEG_B | SEG_D | SEG_F),
    [DISP_X] = (SEG_B | SEG_C | SEG_E | SEG_F | SEG_G), 
    [DISP_Y] = (SEG_B | SEG_C | SEG_D | SEG_F | SEG_G),
    [DISP_Z] = (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G), 
    [DISP_DP] = (SEG_DP)
};
// clang-format on

typedef struct
{
    RotaryCallback ccw_callback; // Called on counter clockwise rotary rotation.
    RotaryCallback cw_callback;  // Called on clockwise rotary rotation.
    RotaryCallback update;       // Called every 100Hz cycle to update/broadcast data.
} Screen;

Screen *get_main_drive_screen(void);
Screen *get_init_screen(void);
Screen *get_start_up_screen(void);
Screen *get_vd_screen(void);

void app_screens_init(void);
void app_screens_update(void);
