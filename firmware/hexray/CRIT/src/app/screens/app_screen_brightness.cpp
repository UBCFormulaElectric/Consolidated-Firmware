#include "app_screens.hpp"
#include "app_brightness.hpp"
#include "io_sevenSeg.hpp"

#include <algorithm>

void static update()
{ // render brightness
    std::array<io::seven_seg::digit, io::seven_seg::DIGITS> screen_buf{
        { io::seven_seg::dot, io::seven_seg::dot, io::seven_seg::dot, io::seven_seg::b, io::seven_seg::r,
          io::seven_seg::i, io::seven_seg::g, io::seven_seg::h, io::seven_seg::t }
    };
    screen_buf[0] = io::seven_seg::digit_to_segment(app::brightness / 100 % 10);
    screen_buf[1] = io::seven_seg::digit_to_segment(app::brightness / 10 % 10);
    screen_buf[2] = io::seven_seg::digit_to_segment(app::brightness / 1 % 10);
    LOG_IF_ERR(io::seven_seg::write(screen_buf));
}

constexpr app::screens::Screen app::screens::brightness_screen = {
    [] { app::brightness = static_cast<uint8_t>(std::clamp(app::brightness - 1, 0, 100)); },
    [] { app::brightness                                       = static_cast<uint8_t>(std::clamp(app::brightness + 1, 0, 100)); }, update
};
