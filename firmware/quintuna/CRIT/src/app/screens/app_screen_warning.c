#include "app_screens.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_shift_register.h"

typedef struct
{
    uint8_t data_buffer[SEVEN_SEG_DATA_LENGTH];
    bool    warning_supressed;
} app_screen_warning_data_t;

/*********************** Static Function Declarations ***************************/
static void warning_cw_ccw(void);
static void warning_update(void);

/************************* Global Variables ***************************/
static app_screen_warning_data_t instance = { 0 };

/*********************** Function Definitions ***************************/
static void warning_cw_ccw(void)
{
    instance.warning_supressed = !instance.warning_supressed;
    warning_update();
}

static void warning_update(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        instance.data_buffer[i] = SEGMENT_DICT[DISP_DP];
    }

    if (true == instance.warning_supressed)
    {
        instance.data_buffer[0] = SEGMENT_DICT[DISP_Y];
        instance.data_buffer[1] = SEGMENT_DICT[DISP_E];
        instance.data_buffer[2] = SEGMENT_DICT[DISP_S];
    }
    else
    {
        instance.data_buffer[0] = SEGMENT_DICT[DISP_N];
        instance.data_buffer[1] = SEGMENT_DICT[DISP_O];
    }

    app_canTx_CRIT_SuppressWarnings_set(instance.warning_supressed);
}

Screen warning_screen = { .ccw_callback = warning_cw_ccw, .cw_callback = warning_cw_ccw, .update = warning_update };
