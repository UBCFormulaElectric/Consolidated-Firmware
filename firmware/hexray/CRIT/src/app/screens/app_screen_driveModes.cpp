// #include <string.h>
#include <cstdint>
#include "app_screens.hpp"
#include "app_canUtils.hpp"
#include "io_sevenSeg.hpp"

#include <cstring>
// #include "app_screen_defines.hpp"
// #include "app_canTx.h"
// #include "app_canRx.h"
// #include "io_shift_register.h"
// #include "app_canUtils.h"
//
static auto              drive_mode       = app::can_utils::DriveMode::VANILLA;
static constexpr uint8_t DRIVE_MODE_COUNT = static_cast<size_t>(app::can_utils::DriveMode::COUNT);

static void drive_mode_ccw()
{
    drive_mode = static_cast<app::can_utils::DriveMode>(
        (static_cast<uint8_t>(drive_mode) - 1 + DRIVE_MODE_COUNT) % DRIVE_MODE_COUNT);
}

static void drive_mode_cw()
{
    drive_mode = static_cast<app::can_utils::DriveMode>((static_cast<uint8_t>(drive_mode) + 1) % DRIVE_MODE_COUNT);
}

static void drive_mode_update()
{
    // const uint8_t max_cell_temp                      = static_cast<uint8_t>(app_canRx_BMS_MaxCellTemp_get());
    // uint8_t data_buffer[DIG] = { 0 };
    std::array<io::seven_seg::digit, io::seven_seg::DIGITS> data_buffer{};
    // memset(&data_buffer[0], SEG_PATTERN_DP, (sizeof(uint8_t) * SEVEN_SEG_DATA_LENGTH));

    // data_buffer[3] = digit_to_segment(max_cell_temp / 100);
    // data_buffer[4] = digit_to_segment((max_cell_temp / 10) % 10);
    // data_buffer[5] = digit_to_segment(max_cell_temp % 10);

    switch (drive_mode)
    {
        case app::can_utils::DriveMode::VANILLA:
            data_buffer[0] = io::seven_seg::v;
            data_buffer[1] = io::seven_seg::a;
            data_buffer[2] = io::seven_seg::n;
            break;
        case app::can_utils::DriveMode::LAUNCH:
            data_buffer[0] = io::seven_seg::l;
            data_buffer[1] = io::seven_seg::a;
            data_buffer[2] = io::seven_seg::u;
            break;
        case app::can_utils::DriveMode::TV:
            data_buffer[0] = io::seven_seg::t;
            data_buffer[1] = io::seven_seg::v;
            break;
        case app::can_utils::DriveMode::COUNT:
        default:
            break;
    }

    LOG_IF_ERR(io::seven_seg::write(data_buffer));
}

constexpr app::screens::Screen app::screens::drive_modes_screen = { .ccw_callback = drive_mode_ccw,
                                                                    .cw_callback  = drive_mode_cw,
                                                                    .update       = drive_mode_update };
