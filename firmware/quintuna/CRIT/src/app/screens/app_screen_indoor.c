#include "app_screens.h"
#include "app_canTx.h"
#include "io_shift_register.h"

typedef struct
{
    uint8_t data_buffer[SEVEN_SEG_DATA_LENGTH];
    uint8_t lv_soc;
    uint8_t hv_soc;
    uint8_t speed;
} app_screen_indoor_data_t;

/*********************** Static Function Declarations ***************************/
static void indoor_ccw(void);
static void indoor_cw(void);
static void indoor_update(void);

/************************* Global Variables ***************************/
static app_screen_indoor_data_t instance = { 0 };

/*********************** Function Definitions ***************************/
static void indoor_ccw(void) {}

static void indoor_cw(void) {}

static void indoor_update(void) {}

Screen *get_indoor(void)
{
    static Screen indoor = { .display_data = (uint8_t *)&instance.data_buffer,
                             .ccw_callback = indoor_ccw,
                             .cw_callback  = indoor_cw,
                             .update       = indoor_update };
    return &indoor;
}
