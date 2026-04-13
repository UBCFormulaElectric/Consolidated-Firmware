#pragma once
#include "decimal_dual.hpp"
#include "pair.hpp"
#include <cmath>

namespace app::tv::shared_datatypes
{
template <typename T> struct wheel_set
{
    T fl;
    T fr;
    T rl;
    T rr;
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
    template <Decimal D> void rotate(const wheel_set<D> &z_rot_rad)
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
} // namespace app::tv::shared_datatypes