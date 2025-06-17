#include "app_screens.h"
#include "app_screen_defines.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_shift_register.h"
#include <stdint.h>

typedef struct
{
    uint8_t   data_buffer[SEVEN_SEG_DATA_LENGTH];
    DriveMode current_drive_mode;
    uint8_t   power_draw;
    uint8_t   speed;
} app_screen_main_drive_data_t;

/*********************** Static Function Declarations ***************************/
static void    main_drive_ccw(void);
static void    main_drive_cw(void);
static void    main_drive_update(void);
static uint8_t digit_to_segment(uint8_t d);

/************************* Global Variables ***************************/
static app_screen_main_drive_data_t instance = { 0 };

/*********************** Function Definitions ***************************/
static void main_drive_ccw(void)
{
    if (instance.current_drive_mode == 0)
    {
        instance.current_drive_mode = (DriveMode)(DRIVE_MODE_COUNT - 1);
    }
    else
    {
        instance.current_drive_mode--;
    }

    main_drive_update();
}

static void main_drive_cw(void)
{
    instance.current_drive_mode = (DriveMode)((instance.current_drive_mode + 1) % DRIVE_MODE_COUNT);
    main_drive_update();
}

static uint8_t digit_to_segment(uint8_t digit)
{
    switch (digit)
    {
        case 0:
            return SEG_PATTERN_0;
        case 1:
            return SEG_PATTERN_1;
        case 2:
            return SEG_PATTERN_2;
        case 3:
            return SEG_PATTERN_3;
        case 4:
            return SEG_PATTERN_4;
        case 5:
            return SEG_PATTERN_5;
        case 6:
            return SEG_PATTERN_6;
        case 7:
            return SEG_PATTERN_7;
        case 8:
            return SEG_PATTERN_8;
        case 9:
            return SEG_PATTERN_9;
        default:
            return SEG_PATTERN_DP;
    }
}

static void main_drive_update(void)
{
    // Get inputs.
    instance.speed      = (uint8_t)app_canRx_VC_VehicleVelocity_get();
    instance.power_draw = (uint8_t)app_canRx_BMS_TractiveSystemPower_get();

    // Update SoC data buffer.
    instance.data_buffer[0] = digit_to_segment(instance.power_draw / 100);
    instance.data_buffer[1] = digit_to_segment(instance.power_draw / 10);
    instance.data_buffer[2] = digit_to_segment(instance.power_draw % 10);

    // Update speed data buffer.
    instance.data_buffer[3] = digit_to_segment(instance.speed / 100);
    instance.data_buffer[4] = digit_to_segment(instance.speed / 10);
    instance.data_buffer[5] = digit_to_segment(instance.speed % 10);

    // Update drive mode data buffer.
    switch (instance.current_drive_mode)
    {
        case DRIVE_MODE_POWER:
            instance.data_buffer[6] = SEG_PATTERN_P;
            instance.data_buffer[7] = SEG_PATTERN_W;
            instance.data_buffer[8] = SEG_PATTERN_R;
            break;
        case DRIVE_MODE_POWER_AND_ACTIVE:
            instance.data_buffer[6] = SEG_PATTERN_P;
            instance.data_buffer[7] = SEG_PATTERN_DP;
            instance.data_buffer[8] = SEG_PATTERN_A;
            break;
        case DRIVE_MODE_TV:
            instance.data_buffer[6] = SEG_PATTERN_T;
            instance.data_buffer[7] = SEG_PATTERN_V;
            instance.data_buffer[8] = SEG_PATTERN_DP;
            break;
        default:
            instance.data_buffer[6] = SEG_PATTERN_8;
            instance.data_buffer[7] = SEG_PATTERN_8;
            instance.data_buffer[8] = SEG_PATTERN_8;
            break;
    }

    io_shift_register_updateSevenSegRegisters((uint8_t *)instance.data_buffer);
    app_canTx_CRIT_DriveMode_set(instance.current_drive_mode);
}

Screen main_drive_screen = { .ccw_callback = main_drive_ccw,
                             .cw_callback  = main_drive_cw,
                             .update       = main_drive_update };
