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
        const color _rsm,
        const color _bms,
        const color _vc,
        const color _fsm,
        const color _crit,
        const color _dam,
        const color _launch_control,
        const color _push_drive,
        const color _shdn,
        const bool  _regen,
        const bool  _tv,
        const bool  _imd,
        const bool  _ams,
        const bool  _bspd)
      : rsm(_rsm),
        bms(_bms),
        vc(_vc),
        fsm(_fsm),
        crit(_crit),
        dam(_dam),
        launch_control(_launch_control),
        push_drive(_push_drive),
        shdn(_shdn),
        regen(_regen),
        tv(_tv),
        imd(_imd),
        ams(_ams),
        bspd(_bspd)
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