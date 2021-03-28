/*
 * Created by Geoff Goodwin-Wilson
 * This code serves as a framework to set the gains of the control system
 * to values which depend on the magnitude of d and q axis currents.
 * These "adaptive" gains can be enabled or disabled in ControlSystemConfig.h
 */

#include "controls/App_AdaptionGain.h"
#include "configs/App_ControlSystemConfig.h"

controller_values adaptionGain(controller_values values, double is_ref)
{
	double Kp_a, Tn_a;
	controller_values new_values;

	// Lower region
	if (fabs(is_ref) < LOWER_ADAPTION_LIMIT * MAX_STATOR_CURRENT)
	{
		Kp_a = values.gain * ADAPTION_GAIN_CONST;
		Tn_a = values.time_const * ADAPTION_TIME_CONST;
	}

	// Adaption region
	else if (fabs(is_ref) > LOWER_ADAPTION_LIMIT * MAX_STATOR_CURRENT &&
	fabs(is_ref) <= UPPER_ADAPTION_LIMIT * MAX_STATOR_CURRENT)
	{
		Kp_a = ((values.gain - values.gain * ADAPTION_GAIN_CONST) /
				(UPPER_ADAPTION_LIMIT * MAX_STATOR_CURRENT - LOWER_ADAPTION_LIMIT * MAX_STATOR_CURRENT)) *
			   (fabs(is_ref) - MAX_STATOR_CURRENT * LOWER_ADAPTION_LIMIT) + values.gain * ADAPTION_GAIN_CONST;
		Tn_a = ((values.time_const - values.time_const * ADAPTION_TIME_CONST) /
				(UPPER_ADAPTION_LIMIT * MAX_STATOR_CURRENT - LOWER_ADAPTION_LIMIT * MAX_STATOR_CURRENT)) *
			   (fabs(is_ref) - MAX_STATOR_CURRENT * LOWER_ADAPTION_LIMIT) + values.time_const * ADAPTION_TIME_CONST;
	}

	// Upper region
	else if (fabs(is_ref) > UPPER_ADAPTION_LIMIT * MAX_STATOR_CURRENT)
	{
		Kp_a = values.gain;
		Tn_a = values.time_const;
	}
	else
	{
		Kp_a = values.gain;
		Tn_a = values.time_const;
	}

	new_values = values;
	new_values.gain = Kp_a;
	new_values.time_const = Tn_a;

	return new_values;
}

