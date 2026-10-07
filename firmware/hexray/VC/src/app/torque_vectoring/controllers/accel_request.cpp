#include "accel_request.hpp"
#include "../shared_datatypes/constants.hpp"

#include <algorithm>

using namespace app::tv::shared_datatypes::vd_constants;

namespace app::tv::controllers::accel_request
{
template <Decimal T> T compute_accel_request(T apps, T brakes, T v_x_mps, bool regen_enabled)
{
    (void)brakes; // TODO: Implement braking consideration. may require some kind of brake model
    // Net longitudinal request from both pedals (each 0..1). Braking requests fade out at low speed so the motors
    // stop helping before standstill (otherwise they lock the wheels / reverse the car); the brakes finish the stop.
    // const T pedal      = state.apps - state.brake;
    // const T brake_fade = std::clamp(state.v_x_mps / static_cast<T>(BRAKE_FADE_SPEED_MPS), T(0), T(1));
    // const T ax_mps2_setpoint = pedal >= 0 ? MAX_AX_MPS2 * pedal : MAX_AX_MPS2 * pedal * brake_fade;

    T accel_request_percent = apps;

    /**
     * With Regen enabled, we remap the acceleration pedal to enable one pedal driving:
     *
     * If regen is not enabled:
     * Apps: [0 ... 1] -> positive pedal percentage means positive acceleration, and no regen
     *
     * If regen is enabled:
     * Apps: [-0.2 ... 0) [0 ... 0.1) [0.1 ... 1]
     *
     * [-0.2 ... 0) -> This is your actual regen region, the pedal will output a negative value,
     * and this interval gets remapped to a value from 0 to -1, with -1 meaning maximum regenerative
     * braking force. When driving the car at speed, fully lifting off apps would put the pedal into
     * the negative/regen region and cause a regenerative braking request on the motors.
     *
     * [0 ... 0.1) -> This is a deadzone area where apps pedal percentages correspond to 0 acceleration.
     *
     * [0.1 ... 1] -> This is the acceleration region, where all apps pedal requests would result in
     * positive acceleration and positive torque requests (mostly).
     *
     * Within the regen and acceleration intervals specified above, we make those map from
     * [-1 ... 0] and [0 .. 1] so that they can properly request minimum or maximum acceleration
     *
     * Intuitively it can be thought of like this as you are pressing the pedal:
     *
     * Without regen the pedal acts like this [0 ... 1]
     *
     * With regen, [0 ... 0.2) [0.2 ... 0.3) [0.3 ... 1]
     *
     * AKA the first 20% will regen, the next 10% is a deadzone, the last 70% is positive acceleration
     */

    if (regen_enabled)
    {
        T apps_remap = apps - REGEN_PEDAL_REGION;

        if (apps_remap < 0)
        {
            /**
             * Regen derating based on speed is rules required. We must not produce significant and
             * intentional regen torque below 5 kmh. Now in the cases like traction control, the
             * motor may still produce a negative torque to reduce wheelspin below 5 kmh but this in
             * theory is fine
             *
             * TODO: is it actually fine also reference the actual rule for this
             *
             * Thus we perform a linear derate from 10 kmh to 5 kmh, so at 5 kmh and below we do not
             * produce any intentional regen torque
             *
             * When driving the car, this would feel like a deadzone for the first 30% of pedal travel,
             * and then the last 70% would produce a positive torque
             */

            T speed_derate_factor = std::clamp((v_x_mps - MIN_SPEED_REGEN_KMH) / MIN_SPEED_REGEN_KMH, T(0), T(1));
            accel_request_percent = (apps_remap / REGEN_PEDAL_REGION) * speed_derate_factor;
        }
        else if (apps_remap < PEDAL_DEADZONE)
        {
            accel_request_percent = 0;
        }
        else
        {
            accel_request_percent = (apps_remap - PEDAL_DEADZONE) / (T(1) - PEDAL_DEADZONE - REGEN_PEDAL_REGION);
        }
    }

    return accel_request_percent * MAX_AX_MPS2;
}
template tv_real compute_accel_request(tv_real apps, tv_real brakes, tv_real v_x_mps, bool regen_enabled);
} // namespace app::tv::controllers::accel_request