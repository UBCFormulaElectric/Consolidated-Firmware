#pragma once

#include <stdint.h>

/*
 * Extended Kalman Filter (EKF)
 *
 * Bayesian filter used for state and covariance estimation of 
 * non-linear systems
 * 
 * API allows for combining multiple sensors to produce a reliable
 * data output. This should only be used for NON-LINEAR systems,
 * linear systems should use the normal Kalman Filter (KF)
 *
 * Prediction Step:
        calculates the predicted state and predicted covariance
        based on previous state and covariance

 * Update step:
        Estimates the current state using predicted measurments
        and state, calculated measurements, and a computed a kalman
        gain. Estimates the current covariance using the predicted
        covariance and measurement covariance
 *
*/

/*
 * Predict
 * Measure
 * Update
 */

#define DIM 2
#define DIM_U 2
#define DIM_MEAS 4

typedef struct 
{
    float *mat;
    uint32_t rows;
    uint32_t cols;
} Matrix;

typedef struct
{
    float measurement[DIM_MEAS];
    float control_input[DIM_U];
} VelocityEstimator_Inputs;

typedef struct
{
    float state_estimate_init[DIM];
    float covariance_estimate_init[DIM * DIM];
    float process_noise_cov[DIM * DIM];
    float measurement_noise_cov[DIM * DIM];
    float time_step; // sample time
} VelocityEstimator_Config;

/*
 * Initialize velocity estimator with:
 *
 * Initial state and covariance
 * Process and Measurement noise covariance
 * Time step
*/
void app_velocityEstimator_init(VelocityEstimator_Config *config);

/*
 * Runs the predict and update step of the velocity estimator
 *
 * Must provide measurement and control input in 
 * VelocityEstimator struct before eachc call
*/
void app_velocityEstimator_run(VelocityEstimator_Inputs inputs);

/*
 * Get the velocity estimate
*/
float *app_velocityEstimator_getVelocity();

/*
 * Get the covariance estimate
*/
float *app_velocityEstimator_getCovariance();

/*
 * Predicts the next state and covariance
 * given the previous state, control variables, and noise covariance
 */
void predict(Matrix prev_state, Matrix control_input, Matrix process_noise_cov);

/*
 * Updates the prediction, gives the final, corrected state and covariance estimate
 * with measured values given previous state and noise covariance
 */
void update(Matrix measurement, Matrix prev_state, Matrix measurement_noise_cov);

/*
 * Predicts next state given the previous state and control variables
*/
void state_transition(Matrix prev_state, Matrix control_input);

/*
 * Calculates the jacobian given the previous state and control variables
 */
void state_jacobian(Matrix x, Matrix control_input);

/*
 * Predicts the next measurement given the previous state
*/
void measurement_func(Matrix x);

/*
 * Calculates the jacobian given the previous state and control variables
 */
void measurement_jacobian(Matrix x);