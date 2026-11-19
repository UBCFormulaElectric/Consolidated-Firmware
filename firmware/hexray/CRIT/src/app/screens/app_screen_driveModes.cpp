#include "app_screens.hpp"
#include "app_canUtils.hpp"
#include "io_sevenSeg.hpp"

#include <cstdint>
#include <cstring>

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
    char data_buffer[io::seven_seg::DIGITS];
    switch (drive_mode)
    {
        case app::can_utils::DriveMode::VANILLA:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "van");
            break;
        case app::can_utils::DriveMode::LAUNCH:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "lau");
            break;
        case app::can_utils::DriveMode::TV:
            snprintf(data_buffer, io::seven_seg::DIGITS + 1, "tv");
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
