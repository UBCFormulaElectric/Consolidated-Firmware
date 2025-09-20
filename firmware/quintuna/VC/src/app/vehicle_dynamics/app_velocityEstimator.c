#include <stdio.h>
#include <math.h>

#include "app_math.h"
#include "app_velocityEstimator.h"
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_sbgEllipse.h"

// for debugging purposes
#define TOGGLE_GPS 1
#define TOGGLE_WHEEL_SPD 1

// rpm derivative check constant, must be tuned
// Note: derivative check currently disabled
#define SPEED_CHECK_MPS 20000.0f

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

// state estimate vector containing estimated velocity (EKF: x)
static Matrix x = { .mat = (float[]){ 0.0f, 0.0f }, .rows = DIM, .cols = 1 };

// covariance estimate shows accuracy of velocity estimate (EKF: P)
static Matrix P = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

// state transition Jacobian (EKF: F_j)
static Matrix F_j = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

// measurement Jacobian (EKF: H_j)
static Matrix H_j = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

// predicted measurement (EKF: z_pred)
static Matrix z_pred = { 
    .mat = (float[]){ 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = 1 
};

// process noise covariance (EKF: Q)
static Matrix Q = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

// measurement noise covariance for wheel speed (EKF: R_ws)
static Matrix R_ws = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

// measurement noise covariance for GPS (EKF: R_gps)
static Matrix R_gps = { 
    .mat = (float[]){ 
        0.0f, 0.0f, 
        0.0f, 0.0f 
    }, 
    .rows = DIM, 
    .cols = DIM 
};

// Used for derivative calculation elements in order: {rr, rl, fr, fl}
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
    x.mat            = config->state_estimate_init;
    P.mat            = config->covariance_estimate_init;
    Q.mat            = config->process_noise_cov;
    R_ws.mat         = config->measurement_ws_noise_cov;
    R_gps.mat        = config->measurement_gps_noise_cov;
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

    predict(&x, &control_input);

#if (TOGGLE_WHEEL_SPD == 1)
    // use wheelspeed measurement if they are not slipping
    if (wsCheck == true)
        update(&measurement_ws, &x, &R_ws);
#endif

#if (TOGGLE_GPS == 1)
    // use gps measurement if sbg is in the correct mode (solution mode 4)
    if (app_sbgEllipse_getEkfSolutionMode() == POSITION)
        update(&measurement_gps, &x, &R_gps);
#endif
}

float *app_velocityEstimator_getVelocity()
{
    return x.mat;
}

float *app_velocityEstimator_getCovariance()
{
    return P.mat;
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
        // if (fabsf(v[i] - measurement_ws_prev.mat[i]) <= SPEED_CHECK_MPS) {
            sum_vx += vx[i];
            sum_vy += vy[i];
            count++;
        // }
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

    x.mat[0] += velx_pred;
    x.mat[1] += vely_pred;
}

void state_jacobian(Matrix *state, Matrix *control_input)
{
    UNUSED(state);

    F_j.mat[0 * DIM + 0] = 1;
    F_j.mat[0 * DIM + 1] = time_step * control_input->mat[2];
    F_j.mat[1 * DIM + 0] = -time_step * control_input->mat[2];
    F_j.mat[1 * DIM + 1] = 1;
}

void measurement_func(Matrix *state)
{
    for (int i = 0; i < DIM; i++)
    {
        z_pred.mat[i] = state->mat[i];
    }
}

void measurement_jacobian(Matrix *state)
{
    UNUSED(state);
    
    H_j.mat[0 * DIM + 0] = 1;
    H_j.mat[1 * DIM + 1] = 1;
}

void predict(Matrix *prev_state, Matrix *control_input)
{
    Matrix temp_cov    = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };
    Matrix F_transpose = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };

    // state prediction
    // x_k|k-1 = f(x_{k-1|k-1}, u_k)
    state_transition(prev_state, control_input);
    // F_k = ∂f/∂x |_{x_{k-1|k-1}, u_k}
    state_jacobian(prev_state, control_input);

    // prediction covariance prediction
    // P_k|k-1 = F_k P_{k-1|k-1} F_k^T + Q_k
    mult(&temp_cov, &F_j, &P);
    transpose(&F_transpose, &F_j);
    mult(&P, &temp_cov, &F_transpose);
    add(&P, &P, &Q);
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
    // z_pred_k = h(x_k|k-1)
    measurement_func(prev_state);
    // H_k = ∂h/∂x |_{x_k|k-1}
    measurement_jacobian(prev_state);

    // measurement covariance prediction
    // S_k = H_k P_k|k-1 H_k^T + R_k
    transpose(&H_transpose, &H_j);
    mult(&PH_transpose, &P, &H_transpose);
    mult(&S, &H_j, &PH_transpose);
    add(&S, &S, measurement_noise_cov);

    // kalman gain calculation
    // K_k = P_k|k-1 H_k^T S_k^{-1}
    inverse2x2(&S_inv, &S);
    mult(&K, &PH_transpose, &S_inv);

    // measurement residual calculation
    // r_k = z_k - z_pred_k
    sub(&residual, measurement, &z_pred);

    // state estimation
    // x_k|k = x_k|k-1 + K_k r_k
    mult(&Ky, &K, &residual);
    add(&x, prev_state, &Ky);

    Matrix KH  = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };
    Matrix KHP = { .mat = (float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, .rows = DIM, .cols = DIM };

    // covariance estimation
    // P_k|k = P_k|k-1 - K_k H_k P_k|k-1
    mult(&KH, &K, &H_j);
    mult(&KHP, &KH, &P);
    sub(&P, &P, &KHP);
}