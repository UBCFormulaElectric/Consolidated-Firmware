#include "controls/App_ClarkeParkTransform.h"
#include "configs/App_ControlSystemConfig.h"

dqs_values clarkeParkTransform(phase_values phase_currents, double theta){

	double iph_b = phase_currents.b;
	double iph_a = phase_currents.a;
	double iph_c = CALCULATE_PHC_CURRENT ? (-1 * (iph_b + iph_a) ) : phase_currents.c;
	dqs_values dqs_currents;

	// Clarke Transform
	double alpha = (iph_a - 0.5 * iph_b - 0.5 * iph_c) * 2 / 3;
	double beta  = ((sqrt(3) / 2) * iph_b - (sqrt(3) / 2) * iph_c) * 2 / 3;

	// Park Transform
	double id = alpha * cos(theta) + beta * sin(theta);
	double iq = -alpha * sin(theta) + beta * cos(theta);

	dqs_currents.d = id;
	dqs_currents.q = iq;
	dqs_currents.s = sqrt(id*id + iq*iq);

	return dqs_currents;
}

