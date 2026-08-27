#include "app_screens.hpp"
#include "app_brightness.hpp"
#include "io_sevenSeg.hpp"

#include <algorithm>
#include <array>

void static update()
{ // render brightness
    // std::array<char, io::seven_seg::DIGITS> screen_buf{ { ' ', ' ', ' ', 'b', 'r', 'i', 'g', 'h', 't' } };
    // screen_buf[0] = static_cast<char>('0' + ((app::brightness / 100) % 10));
    // screen_buf[1] = static_cast<char>('0' + ((app::brightness / 10) % 10));
    // screen_buf[2] = static_cast<char>('0' + (app::brightness % 10));
    char screen_buf[io::seven_seg::DIGITS];
    snprintf(screen_buf, sizeof(screen_buf) + 1, "%3ubright", app::brightness);

    LOG_IF_ERR(io::seven_seg::write(screen_buf));
}

constexpr app::screens::Screen app::screens::brightness_screen = {
    [] { app::brightness = static_cast<uint8_t>(std::clamp(app::brightness - 5, 0, 100)); },
    [] { app::brightness                                       = static_cast<uint8_t>(std::clamp(app::brightness + 5, 0, 100)); }, update
};
