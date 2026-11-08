#pragma once

// Debug signals, written by each module at the point it computes them (no recomputation) and read out once per
// cycle by the Simulink wrapper (or packed onto CAN on the car). Plain C so Simulink can import the types.
// All vehicle quantities are in the controller's ISO frame (x fwd, y left, z up).
typedef struct
{
    double kappas[4];  // chosen slip targets (fl, fr, rl, rr)
    double r_fx;       // weighted total Fx residual (sqrt(W_FX) * kN)
    double r_mz;       // weighted yaw moment residual (sqrt(W_MZ) * kNm)
    double fx_des_N;   // requested total Fx
    double mz_des_Nm;  // requested yaw moment
    double fx_pred_N;  // model-predicted total Fx at the chosen slips
    double mz_pred_Nm; // model-predicted yaw moment at the chosen slips
} optimizer_info;

typedef struct
{
    double r_ref_radps; // reference yaw rate
    double r_err_radps; // r_ref - r_meas
    double mz_des_Nm;   // yaw controller output: yaw moment request
} yrc_info;

typedef struct
{
    // Only quantities the controller computes itself; raw Simulink inputs are already loggable there.
    double beta_rad;     // body sideslip angle
    double alpha_rad[4]; // tire slip angles
    double fz_N[4];      // estimated normal loads
} veh_state_info;

typedef struct
{
    double accel_request;    // -1..1, fraction of MAX_AX_MPS2 (negative = regen)
    double torque_max_Nm[4]; // per-motor torque limits for the speed controller
    double torque_min_Nm[4];
} accel_request_info;

typedef struct
{
    double motor_speed_request_radps[4]; // motor speed setpoints
    double tire_ffb_Nm[4];               // tire force feedforward (motor torque)
    double desired_torque_Nm[4];         // speed controller output, before the brake and power limits
} speed_controller_info;

typedef struct
{
    //     // We apply gain scheduling to the covariance matrices of each step to adapt for changing conditions
    //     // that may affect the accuracy of each step. for example, when the gps is not in its optimal mode
    //     // increase its gain on the covariance which will naturally deprioritize its impact in the EKF.
    //     double predict_gain;
    //     double wheelspeed_gain;
    //     double gps_gain;
    double pred_v_x_mps;
    double pred_v_y_mps;
    double est_v_x_mps;
    double est_v_y_mps;
    double pred_wheel_omega_radps[4];
    double covariance[4];  // P of [v_x, v_y], (m/s)^2
    double zero_vx_active; // 1 when the zero-velocity update is applied to v_x this cycle
    double zero_vy_active; // 1 when the zero-velocity update is applied to v_y this cycle
} veh_ekf_info;

typedef struct
{
    double desired_power_W;   // mechanical power of the speed controller torques
    double limited_power_W;   // mechanical power after the power limit and the torque clamp
    double discharge_limit_W; // battery power limits from the BMS
    double charge_limit_W;
} power_limiting_info;

typedef struct
{
    optimizer_info        optimizer;
    yrc_info              yrc;
    veh_state_info        veh_state;
    veh_ekf_info          ekf_info;
    accel_request_info    accel_req;
    power_limiting_info   plim;
    speed_controller_info speed_ctrl;
} tv_debug;

#ifdef __cplusplus
// The one instance, defined in torque_vectoring.cpp. Only code called from update() writes to it.
extern tv_debug tv_debug_data;
#endif
