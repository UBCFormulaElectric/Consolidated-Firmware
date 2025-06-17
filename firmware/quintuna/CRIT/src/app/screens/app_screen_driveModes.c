#include <string.h>
#include <stdint.h>
#include "app_screens.h"
#include "app_screen_defines.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "io_shift_register.h"
#include "app_canUtils.h"

static DriveMode drive_mode = DRIVE_MODE_POWER;

static void drive_mode_ccw(void);
static void drive_mode_cw(void);
static void drive_mode_update(void);

static void drive_mode_ccw(void)
{
    if (drive_mode == 0)
    {
        drive_mode = (DriveMode)(DRIVE_MODE_COUNT - 1);
    }
    else
    {
        drive_mode--;
    }

    drive_mode_update();
}

static void drive_mode_cw(void)
{
    drive_mode = (DriveMode)((drive_mode + 1) % DRIVE_MODE_COUNT);
    drive_mode_update();
}

static void drive_mode_update(void)
{
    uint8_t data_buffer[SEVEN_SEG_DATA_LENGTH] = { 0 };
    (void)memset(&data_buffer[0], SEG_PATTERN_DP, (sizeof(uint8_t) * SEVEN_SEG_DATA_LENGTH));

    switch (drive_mode)
    {
        case DRIVE_MODE_POWER:
            data_buffer[3] = SEG_PATTERN_P;
            data_buffer[4] = SEG_PATTERN_U;
            data_buffer[5] = SEG_PATTERN_R;
            break;
        case DRIVE_MODE_POWER_AND_ACTIVE:
            data_buffer[3] = SEG_PATTERN_P;
            data_buffer[4] = SEG_PATTERN_DP;
            data_buffer[5] = SEG_PATTERN_A;
            break;
        case DRIVE_MODE_TV:
            data_buffer[3] = SEG_PATTERN_T;
            data_buffer[4] = SEG_PATTERN_V;
            data_buffer[5] = SEG_PATTERN_DP;
            break;
        default:
            data_buffer[3] = SEG_PATTERN_8;
            data_buffer[4] = SEG_PATTERN_8;
            data_buffer[5] = SEG_PATTERN_8;
            break;
    }

    io_shift_register_updateSevenSegRegisters((uint8_t *)data_buffer);
}

Screen drive_modes_screen = { .ccw_callback = drive_mode_ccw,
                              .cw_callback  = drive_mode_cw,
                              .update       = drive_mode_update };
