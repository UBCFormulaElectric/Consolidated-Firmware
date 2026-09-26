#pragma once

// Debug signals, written by each module at the point it computes them (no recomputation) and read out once per
// cycle by the Simulink wrapper (or packed onto CAN on the car). Plain C so Simulink can import the types.
// All vehicle quantities are in the controller's ISO frame (x fwd, y left, z up).
typedef struct
{
    double kappas[4];  // chosen slip targets (fl, fr, rl, rr)
    double r_ax;       // long acceleration residual
    double r_Mz;       // Yaw moment residual
    double fx_des_N;   // requested total Fx (after brake fade)
    double mz_des_Nm;  // requested yaw moment
    double fx_pred_N;  // model-predicted total Fx at the chosen slips
    double mz_pred_Nm; // model-predicted yaw moment at the chosen slips
} optimizer_info;

typedef struct
{
    double r_ref;     // reference yaw rate (rad/s)
    double r_err;     // r_ref - r_meas (rad/s)
    double r_dot_des; // yaw controller output (yaw moment request, Nm)
} yrc_info;

typedef struct
{
    // Only quantities the controller computes itself; raw Simulink inputs are already loggable there.
    double beta_rad;     // body sideslip angle
    double alpha_rad[4]; // tire slip angles
    double fz_N[4];      // estimated normal loads
} veh_state;

typedef struct
{
    optimizer_info optimizer;
    yrc_info       yrc;
    veh_state      veh_state;
} tv_debug;

#ifdef __cplusplus
// The one instance, defined in torque_vectoring.cpp. Only code called from update() writes to it.
extern tv_debug tv_debug_data;
#endif
