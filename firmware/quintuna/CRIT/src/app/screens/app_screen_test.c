#include "app_screens.h"
#include "io_shift_register.h"

typedef struct
{
    uint8_t data_buffer[SEVEN_SEG_DATA_LENGTH];
    uint8_t count;
} app_screen_test_data_t;

/*********************** Static Function Declarations ***************************/
static void test_ccw(void);
static void test_cw(void);
static void test_update(void);

/************************* Global Variables ***************************/
static app_screen_test_data_t instance = { .count = 100, .data_buffer = { 0 } };

/*********************** Function Definitions ***************************/
static void test_ccw(void)
{
    instance.count--;
    test_update();
}

static void test_cw(void)
{
    instance.count++;
    test_update();
}

static void test_update(void)
{
    instance.data_buffer[0] = SEGMENT_DICT[DISP_1];
    instance.data_buffer[1] = SEGMENT_DICT[DISP_H];
    instance.data_buffer[2] = SEGMENT_DICT[DISP_I];

    instance.data_buffer[3] = SEGMENT_DICT[(instance.count / 100)];      // hundreds
    instance.data_buffer[4] = SEGMENT_DICT[(instance.count % 100) / 10]; // tens
    instance.data_buffer[5] = SEGMENT_DICT[(instance.count % 10)];       // ones

    instance.data_buffer[6] = SEGMENT_DICT[DISP_J];
    instance.data_buffer[7] = SEGMENT_DICT[DISP_U];
    instance.data_buffer[8] = SEGMENT_DICT[DISP_G];

    io_shift_register_updateSevenSegRegisters((uint8_t *)instance.data_buffer);
}

Screen *get_test_screen(void)
{
    static Screen test = { .ccw_callback = test_ccw, .cw_callback = test_cw, .update = test_update };
    return &test;
}
