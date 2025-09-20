#include <stdio.h>
#include <math.h>

#include "app_velocityEstimator.h"
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_sbgEllipse.h"

// for debugging purposes
#define TOGGLE_GPS 1
#define TOGGLE_WHEEL_SPD 1

// rpm derivative check constant, must be tuned
#define SPEED_CHECK_MPS 2.0f

/**
 * For making a generalized ekf api:
 * estimation functions: app_velocityEstimator_init -> (app_ekf_init), app_velocityEstimator_run -> (app_ekf_run) should be implemented by the user 
 * internal ekf functions: state_transition, state_jacobian, measurement_func, measurement_jacobian all should be implemented by the user
 *
 * things to figure out:
 * predict and update internally have matrices of fixed dimensions, should be dynamic
 * generalized velocity estimator -> (ekf) variables have fixed size, should be dynamic 
 * all app_math.h linear algebra functions can handle different sizes except for inverse, use cholesky decomp or something for this
 *
 * approaches/solutions:
 * there is a way to change the allocation of .mat to make it dynamic without malloc
 * 
 * create app_ekf api with ekf struct with the following members:
 * state estimate
 * covariance estimate, 
 * State jacobian,
 * Measurement jacobian 
 * function pointers of internal ekf functions for user to implement, 
 * time step
 * 
 * ekf api will have predict and update functions which will call the internal ekf function via the function pointers
 * 
 * Some variables will be statically defined in their respective estimator files 
 * current internal ekf api functions signature may to change to include matrices to store result or they can be added as members to the ekf struct
 * it is up to the user to not violate matrix dimensions
 * 
 * all the user has to do is define the internal ekf functions and call predict and update to run their estimator
 */

/*
 * ================================
 * Velocity Estimator Variables
 * ================================
 */

// state estimate vector containing estimated velocity
static Matrix state_estimate = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM, .cols = 1 };

// covariance estimate shows accuracy of velocity estimate
static Matrix covariance_estimate = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

static Matrix F_jacobian = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

static Matrix H_jacobian = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

static Matrix measurement_predicted = { 
    .mat = (float[]){ 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = 1 
};

static Matrix process_noise_cov = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

static Matrix measurement_ws_noise_cov = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

static Matrix measurement_gps_noise_cov = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

// elements in order: {rr, rl, fr, fl}
static Matrix measurement_ws_prev = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = 1 
};

static float time_step;

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

void app_velocityEstimator_run(VelocityEstimator_Inputs *inputs)
{
    Matrix control_input   = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM_U, .cols = 1 };
    Matrix measurement_ws  = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM, .cols = 1 };
    Matrix measurement_gps = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM, .cols = 1 };

    // set up control inputs for prediction step
    setUpControlInputs(&control_input, inputs);

    // set up measurements for update step
    convertGpsToMeasurement(&measurement_gps, inputs);
    bool wsCheck = convertWheelSpeedToMeasurement(&measurement_ws, inputs);

    predict(&state_estimate, &control_input);

#if (TOGGLE_WHEEL_SPD == 1)
    // use wheelspeed measurement if they are not slipping
    if (wsCheck == true)
        update(&measurement_ws, &state_estimate, &measurement_ws_noise_cov);
#endif

#if (TOGGLE_GPS == 1)
    // use gps measurement if sbg is in the correct mode (solution mode 4)
    if (app_sbgEllipse_getEkfSolutionMode() == POSITION)
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

/**
 * add functionality to remove a specific wheel from the velocity calculation
 * if it violates the derivative check
 */
bool convertWheelSpeedToMeasurement(Matrix *measurement, VelocityEstimator_Inputs *inputs)
{
    // element ordering: {rr, rl, fr, fl}
    float v[4] = {
        MOTOR_RPM_TO_MPS(inputs->rpm_rr),
        MOTOR_RPM_TO_MPS(inputs->rpm_rl),
        MOTOR_RPM_TO_MPS(inputs->rpm_fr),
        MOTOR_RPM_TO_MPS(inputs->rpm_fl)
    };

    float yaw_rate    = inputs->yaw_rate_rad;
    float wheel_angle = inputs->wheel_angle_rad;

    // x velocity components
    float vx[4] = {
        v[0] + yaw_rate * TRACK_WIDTH_m,
        v[1] - yaw_rate * TRACK_WIDTH_m,
        v[2] * cosf(wheel_angle) + yaw_rate * TRACK_WIDTH_m,
        v[3] * cosf(wheel_angle) - yaw_rate * TRACK_WIDTH_m
    };

    // y velocity components
    float vy[4] = {
        -yaw_rate * DIST_REAR_AXLE_CG - v[0],
        -yaw_rate * DIST_REAR_AXLE_CG - v[1],
        yaw_rate * DIST_FRONT_AXLE_CG - v[2] * sinf(wheel_angle),
        yaw_rate * DIST_FRONT_AXLE_CG - v[3] * sinf(wheel_angle)
    };

    float sum_vx = 0.0f, sum_vy = 0.0f;
    int count = 0;

    for (int i = 0; i < 4; i++) {
        if (fabsf(v[i] - measurement_ws_prev.mat[i]) <= SPEED_CHECK_MPS) {
            sum_vx += vx[i];
            sum_vy += vy[i];
            count++;
        }
    }

    // store current speeds for next check
    for (int i = 0; i < 4; i++) {
        measurement_ws_prev.mat[i] = v[i];
    }

    measurement->mat[0] = sum_vx / (float)count;
    measurement->mat[1] = sum_vy / (float)count;

    if (count == 0) {
        return false; // all wheels failed derivative check
    }

    return true;
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