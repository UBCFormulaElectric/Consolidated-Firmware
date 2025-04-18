#include "app_screens.h"
#include "io_shift_register.h"

typedef struct
{
    uint8_t data_buffer[SEVEN_SEG_DATA_LENGTH];
    uint8_t count;
} app_screen_accel_data_t;

/*********************** Static Function Declarations ***************************/
static void accel_ccw(void);
static void accel_cw(void);
static void accel_update(void);

/************************* Global Variables ***************************/
static app_screen_accel_data_t instance = { 0 };

/*********************** Function Definitions ***************************/
static void accel_ccw(void)
{
    instance.count--;
}

static void accel_cw(void)
{
    instance.count++;
}

static void accel_update(void)
{
    instance.data_buffer[0] = SEGMENT_DICT[DISP_J];
    instance.data_buffer[1] = SEGMENT_DICT[DISP_U];
    instance.data_buffer[2] = SEGMENT_DICT[DISP_G];

    instance.data_buffer[3] = SEGMENT_DICT[(instance.count / 100)];
    instance.data_buffer[4] = SEGMENT_DICT[(instance.count / 10)];
    instance.data_buffer[5] = SEGMENT_DICT[(instance.count % 10)];

    instance.data_buffer[0] = SEGMENT_DICT[DISP_N];
    instance.data_buffer[1] = SEGMENT_DICT[DISP_O];
    instance.data_buffer[2] = SEGMENT_DICT[DISP_P];
}

Screen *get_accel(void)
{
    static Screen accel = { .display_data = (uint8_t *)&instance.data_buffer,
                            .ccw_callback = accel_ccw,
                            .cw_callback  = accel_cw,
                            .update       = accel_update };
    return &accel;
}
