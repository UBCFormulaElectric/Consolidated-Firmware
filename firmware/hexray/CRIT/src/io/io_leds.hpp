#pragma once
#include <cstdint>
#include "util_errorCodes.hpp"

namespace io::leds
{
enum class color : uint8_t
{
    OFF     = 0b000,
    RED     = 0b001,
    GREEN   = 0b010,
    YELLOW  = 0b011,
    BLUE    = 0b100,
    MAGENTA = 0b101,
    CYAN    = 0b110,
    WHITE   = 0b111
};
struct config
{
    color rsm;
    color bms;
    color vc;
    color fsm;
    color crit;
    color dam;
    color launch_control;
    color push_drive;
    color shdn;
    bool  regen; // green
    bool  tv;    // green
    bool  imd;   // red
    bool  ams;   // red
    bool  bspd;  // red
    config(
        const color rsm,
        const color bms,
        const color vc,
        const color fsm,
        const color crit,
        const color dam,
        const color launch_control,
        const color push_drive,
        const color shdn,
        const bool  regen,
        const bool  tv,
        const bool  imd,
        const bool  ams,
        const bool  bspd)
      : rsm(rsm),
        bms(bms),
        vc(vc),
        fsm(fsm),
        crit(crit),
        dam(dam),
        launch_control(launch_control),
        push_drive(push_drive),
        shdn(shdn),
        regen(regen),
        tv(tv),
        imd(imd),
        ams(ams),
        bspd(bspd)
    {
    }
};

/**
 * @brief Update the LED states based on the provided config.
 * @param c config
 * @return success of operation
 */
std::expected<void, ErrorCode> update(const config &c);

/**
 * @brief Set the brightness of the LEDs.
 * @param brightness Value from 0.0 (off) to 1.0 (max brightness).
 */
std::expected<void, ErrorCode> setBrightness(float brightness);
} // namespace io::leds