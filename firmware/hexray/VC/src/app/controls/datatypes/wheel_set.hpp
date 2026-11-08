#pragma once
#include "controls/datatypes/decimal_dual.hpp"
#include "controls/datatypes/pair.hpp"
#include <cmath>
#include <cstdint>

namespace app::controls
{
enum class Wheel : uint8_t
{
    FL,
    FR,
    RL,
    RR
};

template <typename T> struct wheel_set
{
    T fl;
    T fr;
    T rl;
    T rr;

    [[nodiscard]] constexpr T operator[](const Wheel wheel) const
    {
        switch (wheel)
        {
            case Wheel::FL:
                return fl;
            case Wheel::FR:
                return fr;
            case Wheel::RL:
                return rl;
            case Wheel::RR:
                return rr;
            default:
                break;
        }
        return T(0);
    }

    /**
     * Writes the values in fl, fr, rl, rr order into a plain array (e.g. a double[4] field of the C debug struct,
     * which cannot be assigned from a braced list)
     * @param out destination array
     */
    template <typename U> constexpr void copyTo(U (&out)[4]) const
    {
        out[0] = static_cast<U>(fl);
        out[1] = static_cast<U>(fr);
        out[2] = static_cast<U>(rl);
        out[3] = static_cast<U>(rr);
    }
};

template <DecimalOrDual T> struct wheel_set<Pair<T>>
{
    Pair<T> fl;
    Pair<T> fr;
    Pair<T> rl;
    Pair<T> rr;

    /**
     * rotates each pair in the z-axis by the respective angle in z_rot
     * @param z_rot_rad rotation angles for each wheel in radians
     */
    void rotate(const wheel_set<float> &z_rot_rad)
    {
        fl = { fl.x * std::cos(z_rot_rad.fl) + fl.y * std::sin(z_rot_rad.fl),
               fl.y * std::cos(z_rot_rad.fl) - fl.x * std::sin(z_rot_rad.fl) };
        fr = { fr.x * std::cos(z_rot_rad.fr) + fr.y * std::sin(z_rot_rad.fr),
               fr.y * std::cos(z_rot_rad.fr) - fr.x * std::sin(z_rot_rad.fr) };
        rl = { rl.x * std::cos(z_rot_rad.rl) + rl.y * std::sin(z_rot_rad.rl),
               rl.y * std::cos(z_rot_rad.rl) - rl.x * std::sin(z_rot_rad.rl) };
        rr = { rr.x * std::cos(z_rot_rad.rr) + rr.y * std::sin(z_rot_rad.rr),
               rr.y * std::cos(z_rot_rad.rr) - rr.x * std::sin(z_rot_rad.rr) };
    }
};
} // namespace app::controls