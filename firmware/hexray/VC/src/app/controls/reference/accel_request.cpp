#include "controls/controls_debug.h"
#include "controls/reference/accel_request.hpp"
#include "controls/params.hpp"

#include <algorithm>

using namespace app::controls::params;

namespace app::controls::reference::accel_request
{
AccelRequest compute(float apps, float brake, float v_x_mps, bool regen_enabled)
{
    // TODO: brake blending (regen in proportion to the brake pedal). May require some kind of brake model.
    // Net longitudinal request from both pedals (each 0..1). Braking requests fade out at low speed so the motors
    // stop helping before standstill (otherwise they lock the wheels / reverse the car); the brakes finish the stop.
    // const float pedal      = state.apps - state.brake;
    // const float brake_fade = std::clamp(state.v_x_mps / BRAKE_FADE_SPEED_MPS, 0.0f, 1.0f);
    // const float ax_mps2_setpoint = pedal >= 0 ? MAX_AX_MPS2 * pedal : MAX_AX_MPS2 * pedal * brake_fade;

    const float v_x_kmh                 = MPS_TO_KMH(v_x_mps);
    float       accel_request           = std::clamp((apps - PEDAL_DEADZONE) / (1.0f - PEDAL_DEADZONE), 0.0f, 1.0f);
    float       regen_derate_factor     = std::clamp((v_x_kmh - MIN_SPEED_REGEN_KMH) / MIN_SPEED_REGEN_KMH, 0.0f, 1.0f);
    float       brake_derate_factor     = std::clamp(5.0f * (1.0f - brake), 0.0f, 1.0f);
    float       low_pedal_derate_factor = std::clamp((apps / PEDAL_DEADZONE), 0.0f, 1.0f);

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
        float apps_remap = apps - REGEN_PEDAL_REGION;

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

            accel_request = (apps_remap / REGEN_PEDAL_REGION) * regen_derate_factor;
        }
        else if (apps_remap < PEDAL_DEADZONE)
        {
            accel_request = 0;
        }
        else
        {
            accel_request = (apps_remap - PEDAL_DEADZONE) / (1.0f - PEDAL_DEADZONE - REGEN_PEDAL_REGION);
        }
    }

    // While the driver brakes, never request drive: the motors must not work against the mechanical brakes (this also
    // covers throttle and brake pressed together). Regen requests pass through.
    if (brake > BRAKE_ACTIVE_THRESHOLD)
    {
        accel_request = std::min(accel_request, 0.0f);
    }

    tv_debug_data.accel_req.accel_request = accel_request;

    return { accel_request, regen_derate_factor, brake_derate_factor, low_pedal_derate_factor };
}
} // namespace app::controls::reference::accel_request