#include "app_powerGauge.hpp"
#include "app_rgbhsv.hpp"
#include "app_brightness.hpp"
#include "app_canRx.hpp"

#include "io_powerGauge.hpp"
#include "io_time.hpp"

#include <cmath>

void app::power_gauge::update()
{
    // const double h = std::fmod(static_cast<double>(io::time::getCurrentMs()) / 10, 360);
    // auto [r, g, b] = rgb::from({ h, 1.0, 0.5 });
    // const auto r_8 = static_cast<uint8_t>(r * static_cast<double>(std::numeric_limits<uint8_t>::max()));
    // const auto g_8 = static_cast<uint8_t>(g * static_cast<double>(std::numeric_limits<uint8_t>::max()));
    // const auto b_8 = static_cast<uint8_t>(b * static_cast<double>(std::numeric_limits<uint8_t>::max()));
    // std::array<io::power_gauge::led_frame, io::power_gauge::LEDS> data{ {
    //     { static_cast<double>(brightness) / 100, r_8, g_8, b_8 },
    //     { static_cast<double>(brightness) / 100, r_8, g_8, b_8 },
    //     { static_cast<double>(brightness) / 100, r_8, g_8, b_8 },
    //     { static_cast<double>(brightness) / 100, r_8, g_8, b_8 },
    //     { static_cast<double>(brightness) / 100, r_8, g_8, b_8 },
    //     { static_cast<double>(brightness) / 100, r_8, g_8, b_8 },
    //     { static_cast<double>(brightness) / 100, r_8, g_8, b_8 },
    //     { static_cast<double>(brightness) / 100, r_8, g_8, b_8 },
    // } };
    std::array<io::power_gauge::led_frame, io::power_gauge::LEDS> data{};

    const float current = -app::can_rx::BMS_TractiveSystemCurrent_get();

    static constexpr uint32_t r = 0xF54927FF;
    static constexpr uint32_t y = 0xF5B027FF;
    static constexpr uint32_t g = 0x27F549FF;

    io::power_gauge::led_frame c;
    if (current < 80)
        c = io::power_gauge::led_frame{ g };
    else if (current < 100)
        c = io::power_gauge::led_frame{ y };
    else
        c = io::power_gauge::led_frame{ r };

    static constexpr uint32_t MAX_CURRENT = 120;
    for (size_t i = 0; i < data.size(); i++)
    {
        const bool on = current > static_cast<float>(MAX_CURRENT * i) / data.size();
        data[i]       = on ? c : io::power_gauge::led_frame{ 0x0 };
    }
    LOG_IF_ERR(io::power_gauge::update(data));
}