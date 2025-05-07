#include "app_screens.h"
#include "io_shift_register.h"

typedef struct
{
    uint8_t data_buffer[SEVEN_SEG_DATA_LENGTH];
    uint8_t regen_strength;
    uint8_t regen_bias;
    uint8_t load_transfer;
} app_screen_vd_data_t;

/************************* Global Variables ***************************/
static app_screen_vd_data_t instance = { 0 };

/*********************** Function Definitions ***************************/
static void vd_update(void)
{
    // idk
}

Screen *get_vd_screen(void)
{
    static Screen vd = { .ccw_callback = NULL, .cw_callback = NULL, .update = vd_update };
    return &vd;
}
