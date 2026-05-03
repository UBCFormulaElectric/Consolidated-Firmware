#pragma once

#include "app_math.h"

/**
 * General Extended Kalman Filter API
 *
 * How to use:
 *
 * 1) Include in your file like this along with the dimensions
 * #define DIM x
 * #include "app_ekf.h"
 *
 * 2) Define the functions in the function pointers
 *
 * 3) Define your measurement functions in your estimator api
 * (They are not stored in the ekf struct which will allow multiple
 * measurement functions and thus multiple sensors if required)
 *
 * Note: If you have more than 2 sensors for the update/measurement step,
 * usually you will need the same amount of measurement functions.
 * Thus, the user must swap the measurement functions in the ekf struct
 * before calling the update step again with the new measurements
 * Explain why velocity estimator is an exception
 */

typedef struct
{
    // estimations
    Matrix state_estimate;
    Matrix covariance_estimate;
    // internal variables
    Matrix F_jacobian;
    Matrix H_jacobian;
    Matrix measurement_pred;
    // user implementable functions functions
    void (*state_transition)(Matrix *state_estimate, Matrix *prev_state, Matrix *control_input);
    void (*state_jacobian)(Matrix *F_jacobian, Matrix *prev_state, Matrix *control_input);
    void (*measurement_jacobian)(Matrix *H_jacobian, Matrix *prev_state);
    void (*measurement_func)(Matrix *measurement_pred, Matrix *prev_state);
} EKF;

static inline void predict(EKF *ekf, Matrix *prev_state, Matrix *control_input, Matrix *process_noise_cov)
{
    Matrix temp_cov    = { .mat = (float[DIM * DIM]){ 0.0f }, .rows = DIM, .cols = DIM };
    Matrix F_transpose = { .mat = (float[DIM * DIM]){ 0.0f }, .rows = DIM, .cols = DIM };

    // state prediction
    ekf->state_transition(&ekf->state_estimate, prev_state, control_input);
    ekf->state_jacobian(&ekf->F_jacobian, prev_state, control_input);

    // prediction covariance prediction
    mult(&temp_cov, &ekf->F_jacobian, &ekf->covariance_estimate);
    transpose(&F_transpose, &ekf->F_jacobian);
    mult(&ekf->covariance_estimate, &temp_cov, &F_transpose);
    add(&ekf->covariance_estimate, &ekf->covariance_estimate, process_noise_cov);
}

/**
 * Updates the prediction, gives the final, corrected state and covariance estimate
 * with measured values given previous state and noise covariance
 *
 * measurement pred is obtained from your measurement function
 */
static inline void update(EKF *ekf, Matrix *measurement, Matrix *prev_state, Matrix *measurement_noise_cov)
{
    Matrix PH_transpose = { .mat = (float[DIM * DIM]){ 0.0f }, .rows = DIM, .cols = DIM };
    Matrix Ky           = { .mat = (float[DIM * 1U]){ 0.0f }, .rows = DIM, .cols = 1 };
    Matrix H_transpose  = { .mat = (float[DIM * DIM]){ 0.0f }, .rows = DIM, .cols = DIM };
    Matrix S            = { .mat = (float[DIM * DIM]){ 0.0f }, .rows = DIM, .cols = DIM };
    Matrix S_inv        = { .mat = (float[DIM * DIM]){ 0.0f }, .rows = DIM, .cols = DIM };
    Matrix K            = { .mat = (float[DIM * DIM]){ 0.0f }, .rows = DIM, .cols = DIM };
    Matrix residual     = { .mat = (float[DIM * 1U]){ 0.0f }, .rows = DIM, .cols = 1 };
    Matrix KH           = { .mat = (float[DIM * DIM]){ 0.0f }, .rows = DIM, .cols = DIM };
    Matrix KHP          = { .mat = (float[DIM * DIM]){ 0.0f }, .rows = DIM, .cols = DIM };

    // measurement prediction
    ekf->measurement_func(&ekf->measurement_pred, prev_state);
    ekf->measurement_jacobian(&ekf->H_jacobian, prev_state);

    // measurement covariance prediction
    transpose(&H_transpose, &ekf->H_jacobian);
    mult(&PH_transpose, &ekf->covariance_estimate, &H_transpose);
    mult(&S, &ekf->H_jacobian, &PH_transpose);
    add(&S, &S, measurement_noise_cov);

    // kalman gain calculation
    inverse2x2(&S_inv, &S);
    mult(&K, &PH_transpose, &S_inv);

    // measurement residual calculation
    sub(&residual, measurement, &ekf->measurement_pred);

    // state estimation
    mult(&Ky, &K, &residual);
    add(&ekf->state_estimate, prev_state, &Ky);

    // covariance estimation
    mult(&KH, &K, &ekf->H_jacobian);
    mult(&KHP, &KH, &ekf->covariance_estimate);
    sub(&ekf->covariance_estimate, &ekf->covariance_estimate, &KHP);
}