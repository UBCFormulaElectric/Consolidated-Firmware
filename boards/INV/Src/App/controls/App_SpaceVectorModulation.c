#include "controls/App_SpaceVectorModulation.h"
#include "controls/App_ControlLoop.h"
#include "configs/App_ControlSystemConfig.h"

phase_values calculatePwmEdges(const phase_values * const phase_voltages, const double bus_voltage)
{
	phase_values phase_duration;
	double t0, t1, t2;
	double vs_max = MAX_MOD_INDEX * bus_voltage / sqrt(3);

	// The sector in which the voltage vector is generated for the motor
	int sectorNumber = 0;

	int sign_phase_a = (phase_voltages->a > 0) ? 1 : 0;
	int sign_phase_b = (phase_voltages->b > 0) ? 1 : 0;
	int sign_phase_c = (phase_voltages->c > 0) ? 1 : 0;

	int sectorEncode = sign_phase_a + 2 * sign_phase_b + 4 * sign_phase_c;

	switch (sectorEncode)
	{
		case 3:
			sectorNumber = 1;
			break;
		case 2:
			sectorNumber = 2;
			break;
		case 6:
			sectorNumber = 3;
			break;
		case 4:
			sectorNumber = 4;
			break;
		case 5:
			sectorNumber = 5;
			break;
		case 1:
			sectorNumber = 6;
			break;
		default:
			sectorNumber = 0;
	}

	switch (sectorNumber)
	{
		case 1:
			t1               = phase_voltages->a / vs_max;
			t2               = phase_voltages->b / vs_max;
			t0               = 1 - t1 - t2;
			phase_duration.a = t1 + t2 + 0.5 * t0;
			phase_duration.b = t2 + 0.5 * t0;
			phase_duration.c = 0.5 * t0;
			break;
		case 2:
			t1               = -phase_voltages->c / vs_max;
			t2               = -phase_voltages->a / vs_max;
			t0               = 1 - t1 - t2;
			phase_duration.a = t1 + 0.5 * t0;
			phase_duration.b = t1 + t2 + 0.5 * t0;
			phase_duration.c = 0.5 * t0;
			break;
		case 3:
			t1               = phase_voltages->b / vs_max;
			t2               = phase_voltages->c / vs_max;
			t0               = 1 - t1 - t2;
			phase_duration.a = 0.5 * t0;
			phase_duration.b = t1 + t2 + 0.5 * t0;
			phase_duration.c = t2 + 0.5 * t0;
			break;
		case 4:
			t1               = -phase_voltages->a / vs_max;
			t2               = -phase_voltages->b / vs_max;
			t0               = 1 - t1 - t2;
			phase_duration.a = 0.5 * t0;
			phase_duration.b = t1 + 0.5 * t0;
			phase_duration.c = t1 + t2 + 0.5 * t0;
			break;
		case 5:
			t1               = phase_voltages->c / vs_max;
			t2               = phase_voltages->a / vs_max;
			t0               = 1 - t1 - t2;
			phase_duration.a = t2 + 0.5 * t0;
			phase_duration.b = 0.5 * t0;
			phase_duration.c = t1 + t2 + 0.5 * t0;
			break;
		case 6:
			t1               = -phase_voltages->b / vs_max;
			t2               = -phase_voltages->c / vs_max;
			t0               = 1 - t1 - t2;
			phase_duration.a = t1 + t2 + 0.5 * t0;
			phase_duration.b = 0.5 * t0;
			phase_duration.c = t1 + 0.5 * t0;
			break;
		default: // Null vector
			t1               = 0;
			t2               = 0;
			t0               = 1 - t1 - t2;
			phase_duration.a = t1 + t2 + 0.5 * t0;
			phase_duration.b = t2 + 0.5 * t0;
			phase_duration.c = 0.5 * t0;
	}
	return phase_duration;
}

//void setPwmEdges()//(const phase_values * const phase_duration)
//{
//	;
//}
//	pwm_a_top = (counter * 2 > phase_duration.a * clockFrequency / SWITCHING_FREQUENCY) ? 0	: 1;
//	pwm_b_top = (counter * 2 > phase_duration.b * clockFrequency / SWITCHING_FREQUENCY) ? 0 : 1;
//	pwm_c_top = (counter * 2 > phase_duration.c * clockFrequency / SWITCHING_FREQUENCY) ? 0 : 1;
//
//	pwm_a_bot = (pwm_a_top) ? 0 : 1;
//	pwm_b_bot = (pwm_b_top) ? 0 : 1;
//	pwm_c_bot = (pwm_c_top) ? 0 : 1;
//
//	if (currentPeriodCycle < clockFrequency / SWITCHING_FREQUENCY)
//	{
//		if (currentPeriodCycle < clockFrequency / (SWITCHING_FREQUENCY * 2))
//		{
//			counter++;
//		}
//		else
//		{
//			counter--;
//		}
//		currentPeriodCycle++;
//	}
//	else
//	{
//		counter            = 0;
//		currentPeriodCycle = 0;
//	}
//
//}