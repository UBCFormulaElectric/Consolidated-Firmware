#pragma once

/**
 * @brief Maximum torque request to get the maximum motor current per Emrax
 * specification.
 *
 * @see Setting up the Controller to run with Emrax Motor Rev 1.2 PDF pg. 6
 */
#define MAX_TORQUE_REQUEST_NM 132.0f;

// For torque plausibility checking between FSM and DCM
#define TORQUE_REQUEST_UPPER_BOUND_FACTOR 1.1f
#define TORQUE_REQUEST_LOWER_BOUND_FACTOR 0.9f
