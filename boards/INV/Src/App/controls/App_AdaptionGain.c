/*
 * Created by Geoff Goodwin-Wilson
 * This code serves as a framework to set the gains of the control system
 * to values which depend on the magnitude of d and q axis currents.
 * These "adaptive" gains can be enabled or disabled in ControlSystemConfig.h
 */

#include "controls/App_AdaptionGain.h"
#include "configs/App_ControlSystemConfig.h"

ControllerValues
    adaptionGain(const ControllerValues *const values, const double is_ref)
{
    double           kp_adaption, tn_adaption;
    ControllerValues new_values;

    // Lower region
    if (fabs(is_ref) < LOWER_ADAPTION_LIMIT * MAX_STATOR_CURRENT)
    {
        kp_adaption = values->gain * ADAPTION_GAIN_CONST;
        tn_adaption = values->time_const * ADAPTION_TIME_CONST;
    }

    // Adaption region
    else if (
        fabs(is_ref) > LOWER_ADAPTION_LIMIT * MAX_STATOR_CURRENT &&
        fabs(is_ref) <= UPPER_ADAPTION_LIMIT * MAX_STATOR_CURRENT)
    {
        kp_adaption =
            ((values->gain - values->gain * ADAPTION_GAIN_CONST) /
             (UPPER_ADAPTION_LIMIT * MAX_STATOR_CURRENT -
              LOWER_ADAPTION_LIMIT * MAX_STATOR_CURRENT)) *
                (fabs(is_ref) - MAX_STATOR_CURRENT * LOWER_ADAPTION_LIMIT) +
            values->gain * ADAPTION_GAIN_CONST;
        tn_adaption =
            ((values->time_const - values->time_const * ADAPTION_TIME_CONST) /
             (UPPER_ADAPTION_LIMIT * MAX_STATOR_CURRENT -
              LOWER_ADAPTION_LIMIT * MAX_STATOR_CURRENT)) *
                (fabs(is_ref) - MAX_STATOR_CURRENT * LOWER_ADAPTION_LIMIT) +
            values->time_const * ADAPTION_TIME_CONST;
    }

    // Upper region
    else if (fabs(is_ref) > UPPER_ADAPTION_LIMIT * MAX_STATOR_CURRENT)
    {
        kp_adaption = values->gain;
        tn_adaption = values->time_const;
    }
    else
    {
        kp_adaption = values->gain;
        tn_adaption = values->time_const;
    }

    new_values            = *values;
    new_values.gain       = kp_adaption;
    new_values.time_const = tn_adaption;

    return new_values;
}
