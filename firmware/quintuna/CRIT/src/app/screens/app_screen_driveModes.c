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

    const uint8_t max_cell_temp = (uint8_t)app_canRx_BMS_MaxCellTemp_get();

    data_buffer[1] = SEG_PATTERN_DP;

    data_buffer[3] = digit_to_segment(max_cell_temp / 100);
    data_buffer[4] = digit_to_segment(max_cell_temp / 10);
    data_buffer[5] = digit_to_segment(max_cell_temp % 10);

    switch (drive_mode)
    {
        case DRIVE_MODE_POWER:
            data_buffer[6] = SEG_PATTERN_P;
            data_buffer[7] = SEG_PATTERN_U;
            data_buffer[8] = SEG_PATTERN_R;
            break;
        case DRIVE_MODE_POWER_AND_ACTIVE:
            data_buffer[6] = SEG_PATTERN_P;
            data_buffer[7] = SEG_PATTERN_DP;
            data_buffer[8] = SEG_PATTERN_A;
            break;
        case DRIVE_MODE_TV:
            data_buffer[6] = SEG_PATTERN_T;
            data_buffer[7] = SEG_PATTERN_V;
            data_buffer[8] = SEG_PATTERN_DP;
            break;
        default:
            data_buffer[6] = SEG_PATTERN_8;
            data_buffer[7] = SEG_PATTERN_8;
            data_buffer[8] = SEG_PATTERN_8;
            break;
    }

    io_shift_register_updateSevenSegRegisters((uint8_t *)data_buffer);
}

Screen drive_modes_screen = { .ccw_callback = drive_mode_ccw,
                              .cw_callback  = drive_mode_cw,
                              .update       = drive_mode_update };
