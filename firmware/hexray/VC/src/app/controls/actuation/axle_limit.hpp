#pragma once

#include "controls/datatypes/wheel_set.hpp"

namespace app::controls::actuation
{
/**
 * Lowers the torques so the sum on each axle is at most max_axle_Nm, so torque does not move between the axles (brake
 * balance). Both wheels on an axle drop by the same amount (keeps the yaw split); if one goes below its limit, the
 * other wheel takes the rest. Assumes max_axle_Nm >= the sum of the axle's two lower limits.
 * @param torque_Nm per-wheel torque requests in Nm
 * @param torque_min_Nm per-wheel lower torque limits in Nm
 * @param max_axle_Nm upper limit for the sum of the two torques on each axle in Nm
 * @return the lowered per-wheel torque requests in Nm
 */
template <Decimal T>
wheel_set<T> limitAxleTorque(wheel_set<T> torque_Nm, const wheel_set<T> &torque_min_Nm, const T max_axle_Nm)
{
    const auto limit_axle = [max_axle_Nm](T &left_Nm, T &right_Nm, const T min_left_Nm, const T min_right_Nm)
    {
        const T excess_Nm = left_Nm + right_Nm - max_axle_Nm;
        if (excess_Nm <= T(0))
        {
            return;
        }

        left_Nm -= excess_Nm / T(2.0);
        right_Nm -= excess_Nm / T(2.0);

        if (left_Nm < min_left_Nm)
        {
            right_Nm -= min_left_Nm - left_Nm;
            left_Nm = min_left_Nm;
        }
        else if (right_Nm < min_right_Nm)
        {
            left_Nm -= min_right_Nm - right_Nm;
            right_Nm = min_right_Nm;
        }
    };

    limit_axle(torque_Nm.fl, torque_Nm.fr, torque_min_Nm.fl, torque_min_Nm.fr);
    limit_axle(torque_Nm.rl, torque_Nm.rr, torque_min_Nm.rl, torque_min_Nm.rr);
    return torque_Nm;
}
} // namespace app::controls::actuation
