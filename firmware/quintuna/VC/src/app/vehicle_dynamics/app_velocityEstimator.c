#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "app_velocityEstimator.h"
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"

// for debugging purposes
#define TOGGLE_GPS 1
#define TOGGLE_WHEEL_SPD 1

/*
 * ================================
 * Velocity Estimator Variables
 * ================================
 */

// state estimate vector containing estimated velocity
static Matrix state_estimate = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM, .cols = 1 };

// covariance estimate shows accuracy of velocity estimate
static Matrix covariance_estimate = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };

static Matrix F_jacobian = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };

static Matrix H_jacobian = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };

static Matrix measurement_predicted = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM, .cols = 1 };

static Matrix process_noise_cov = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };

static Matrix measurement_ws_noise_cov = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };

static Matrix measurement_gps_noise_cov = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };

static float time_step;

/*
 * ================================
 * Linear Algebra Functions
 * ================================
 * 
 * Should these be inlined or will that increase mem size too much?
 */

// void print_arr(float *a, uint32_t len)
// {
//     for (uint32_t i = 0; i < len; i++)
//     {
//         printf("%d: %f\n", i, a[i]);
//     }
// }
/*
 * Matrix Addition: result = A + B
 */
static bool add(Matrix *result, const Matrix *A, const Matrix *B)
{
    if (A->rows != B->rows || A->cols != B->cols || result->rows != A->rows || result->cols != A->cols)
        return false;

    for (uint32_t i = 0; i < A->rows * A->cols; i++)
    {
        result->mat[i] = A->mat[i] + B->mat[i];
    }

    return true;
}

/*
 * Matrix Subtraction: result = A - B
 */
static bool sub(Matrix *result, const Matrix *A, const Matrix *B)
{
    if (A->rows != B->rows || A->cols != B->cols || result->rows != A->rows || result->cols != A->cols)
        return false;

    for (uint32_t i = 0; i < A->rows * A->cols; i++)
    {
        result->mat[i] = A->mat[i] - B->mat[i];
    }

    return true;
}

/*
 * Matrix Multiplication: result = A * B
 */
static bool mult(Matrix *result, const Matrix *A, const Matrix *B)
{
    if (A->cols != B->rows || result->rows != A->rows || result->cols != B->cols)
    {
        return false;
    }

    for (uint32_t i = 0; i < result->rows * result->cols; i++)
    {
        result->mat[i] = 0.0f;
    }

    for (uint32_t i = 0; i < A->rows; i++)
    {
        for (uint32_t j = 0; j < B->cols; j++)
        {
            for (uint32_t k = 0; k < A->cols; k++)
            {
                result->mat[i * result->cols + j] += A->mat[i * A->cols + k] * B->mat[k * B->cols + j];
            }
        }
    }

    return true;
}

/*
 * Matrix Transpose: result = input^T
 */
static bool transpose(Matrix *result, const Matrix *input)
{
    if (result->rows != input->cols || result->cols != input->rows)
    {
        return false; // Incompatible dimensions
    }

    for (uint32_t i = 0; i < input->rows; i++)
    {
        for (uint32_t j = 0; j < input->cols; j++)
        {
            result->mat[j * result->cols + i] = input->mat[i * input->cols + j];
        }
    }

    return true;
}

/*
 * 2x2 Matrix Inversion: result = X^-1
 */
static bool inverse2x2(Matrix *result, const Matrix *X)
{
    if (X->rows != 2 || X->cols != 2 || result->rows != 2 || result->cols != 2)
    {
        return false;
    }

    float a = X->mat[0];
    float b = X->mat[1];
    float c = X->mat[2];
    float d = X->mat[3];

    float det = a * d - b * c;
    if (det == 0.0f)
        return false;

    float inv_det = 1.0f / det;

    result->mat[0] = d * inv_det;
    result->mat[1] = -b * inv_det;
    result->mat[2] = -c * inv_det;
    result->mat[3] = a * inv_det;

    return true;
}

/*
 * ================================
 * Internal EKF Functions
 * (Should not be called, exposed for testing)
 * ================================
 */

void state_transition(Matrix *prev_state, Matrix *control_input)
{
    float velx_pred = time_step * (control_input->mat[0] + prev_state->mat[1] * control_input->mat[2]);
    float vely_pred = time_step * (control_input->mat[1] + prev_state->mat[0] * control_input->mat[2]);

    state_estimate.mat[0] += velx_pred;
    state_estimate.mat[1] += vely_pred;
}

void state_jacobian(Matrix *x, Matrix *control_input)
{
    F_jacobian.mat[0 * DIM + 0] = 1;
    F_jacobian.mat[0 * DIM + 1] = time_step * control_input->mat[2];
    F_jacobian.mat[1 * DIM + 0] = -time_step * control_input->mat[2];
    F_jacobian.mat[1 * DIM + 1] = 1;
}

void measurement_func(Matrix *x)
{
    for (int i = 0; i < DIM; i++)
    {
        measurement_predicted.mat[i] = x->mat[i];
    }
}

void measurement_jacobian(Matrix *x)
{
    H_jacobian.mat[0 * DIM + 0] = 1;
    H_jacobian.mat[1 * DIM + 1] = 1;
}

void predict(Matrix *prev_state, Matrix *control_input)
{
    Matrix temp_cov    = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };
    Matrix F_transpose = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };

    // state prediction
    state_transition(prev_state, control_input);
    state_jacobian(prev_state, control_input);

    // prediction covariance prediction
    mult(&temp_cov, &F_jacobian, &covariance_estimate);
    transpose(&F_transpose, &F_jacobian);
    mult(&covariance_estimate, &temp_cov, &F_transpose);
    add(&covariance_estimate, &covariance_estimate, &process_noise_cov);
}

void update(Matrix *measurement, Matrix *prev_state, Matrix *measurement_noise_cov)
{
    Matrix PH_transpose = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };
    Matrix Ky           = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM, .cols = 1 };
    Matrix H_transpose  = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };
    Matrix S            = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };
    Matrix S_inv        = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };
    Matrix K            = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };
    Matrix residual     = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM, .cols = 1 };

    // measurement prediction
    measurement_func(prev_state);
    measurement_jacobian(prev_state);

    // measurement covariance prediction
    transpose(&H_transpose, &H_jacobian);
    mult(&PH_transpose, &covariance_estimate, &H_transpose);
    mult(&S, &H_jacobian, &PH_transpose);
    add(&S, &S, measurement_noise_cov);

    // kalman gain calculation
    inverse2x2(&S_inv, &S);
    mult(&K, &PH_transpose, &S_inv);

    // measurement residual calculation
    sub(&residual, measurement, &measurement_predicted);

    // state estimation
    mult(&Ky, &K, &residual);
    add(&state_estimate, prev_state, &Ky);

    Matrix KH  = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };
    Matrix KHP = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };

    // covariance estimation
    mult(&KH, &K, &H_jacobian);
    mult(&KHP, &KH, &covariance_estimate);
    sub(&covariance_estimate, &covariance_estimate, &KHP);
}

/*
 * ================================
 * Velocity Estimator Functions
 * ================================
 */

void app_velocityEstimator_init(VelocityEstimator_Config *config)
{
    state_estimate.mat            = config->state_estimate_init;
    covariance_estimate.mat       = config->covariance_estimate_init;
    process_noise_cov.mat         = config->process_noise_cov;
    measurement_ws_noise_cov.mat  = config->measurement_ws_noise_cov;
    measurement_gps_noise_cov.mat = config->measurement_gps_noise_cov;
    time_step                     = config->time_step;
}

void convertWheelSpeedToMeasurement(Matrix *measurement, VelocityEstimator_Inputs *inputs)
{
    float v_rr = MOTOR_RPM_TO_MPS(inputs->rpm_rr);
    float v_rl = MOTOR_RPM_TO_MPS(inputs->rpm_rl);
    float v_fr = MOTOR_RPM_TO_MPS(inputs->rpm_fr);
    float v_fl = MOTOR_RPM_TO_MPS(inputs->rpm_fl);

    float yaw_rate    = inputs->yaw_rate_rad;
    float wheel_angle = inputs->wheel_angle_rad;

    float vx_rr = v_rr + yaw_rate * TRACK_WIDTH_m;
    float vx_rl = v_rl - yaw_rate * TRACK_WIDTH_m;
    float vx_fr = v_fr * cosf(wheel_angle) + yaw_rate * TRACK_WIDTH_m;
    float vx_fl = v_fr * cosf(wheel_angle) - yaw_rate * TRACK_WIDTH_m;

    float vy_rr = -yaw_rate * DIST_REAR_AXLE_CG - v_rr;
    float vy_rl = -yaw_rate * DIST_REAR_AXLE_CG - v_rl;
    float vy_fr = yaw_rate * DIST_FRONT_AXLE_CG - v_fr * sinf(wheel_angle);
    float vy_fl = yaw_rate * DIST_FRONT_AXLE_CG - v_fl * sinf(wheel_angle);

    float vx = (vx_fl + vx_fr + vx_rl + vx_rr) / 4.0f;
    float vy = (vy_fl + vy_fr + vy_rl + vy_rr) / 4.0f;

    measurement->mat[0] = vx;
    measurement->mat[1] = vy;
}

void setUpControlInputs(Matrix *control_inputs, VelocityEstimator_Inputs *inputs)
{
    control_inputs->mat[0] = inputs->accel_x;
    control_inputs->mat[1] = inputs->accel_y;
    control_inputs->mat[2] = inputs->yaw_rate_rad;
}

void convertGpsToMeasurement(Matrix *measurement, VelocityEstimator_Inputs *inputs)
{
    measurement->mat[0] = inputs->gps_vx;
    measurement->mat[1] = inputs->gps_vy;
}

void app_velocityEstimator_run(VelocityEstimator_Inputs *inputs)
{
    Matrix control_input   = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM_U, .cols = 1 };
    Matrix measurement_ws  = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM, .cols = 1 };
    Matrix measurement_gps = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM, .cols = 1 };

    setUpControlInputs(&control_input, inputs);
    convertWheelSpeedToMeasurement(&measurement_ws, inputs);
    convertGpsToMeasurement(&measurement_gps, inputs);

    predict(&state_estimate, &control_input);

    #if(TOGGLE_WHEEL_SPD == 1)
    // use wheelspeed measurement if they are not slipping
    if (inputs->rpm_derivative_ok)
        update(&measurement_ws, &state_estimate, &measurement_ws_noise_cov);
    #endif

    #if(TOGGLE_GPS == 1)
    // use gps measurement if sbg is in the correct mode (solution mode 4)
    if (inputs->ekf_solution_4_valid)
        update(&measurement_gps, &state_estimate, &measurement_gps_noise_cov);
    #endif
}

float *app_velocityEstimator_getVelocity()
{
    return state_estimate.mat;
}

float *app_velocityEstimator_getCovariance()
{
    return covariance_estimate.mat;
}
