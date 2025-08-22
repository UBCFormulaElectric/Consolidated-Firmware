#include <math.h>
#include <stdint.h>

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_imu.h"
#include "app_math.h"

// cuttoff frequencies to test
#define CUTTOFF_1 5.0f
#define CUTTOFF_2 10.0f
#define CUTTOFF_3 15.0f
#define CUTTOFF_4 20.0f

// assumed sample rate (Hz) for integration; adjust if you have a real value
#define SAMPLE_RATE_HZ 100U

// each element represents velocity with an increasing cuttoff frequency (5, 10, 15, 20)
static float velocity[] = { 0.0f, 0.0f, 0.0f, 0.0f };

// previous acceleration x, y, and z
static float prev_accel[3] = { 0.0f, 0.0f, 0.0f };

// per-filter previous filtered acceleration (x and y)
static float prev_ax_filt[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
static float prev_ay_filt[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

// per-filter velocity components (x and y)
static float vel_x[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
static float vel_y[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

static float lpf(float prevVal, float currentVal, uint16_t sampleRate_Hz, uint16_t cutoffFreq_Hz) 
{ 
    float temp  = 2.0f * (float)M_PI * (1.0f / (float)sampleRate_Hz) * (float)cutoffFreq_Hz; 
    float alpha = temp / (temp + 1.0f); 
    return (alpha * currentVal) + ((1.0f - alpha) * prevVal); 
}

// trapezoidal one-step integrator
static inline float integrator(float v_prev, float a_prev, float a_curr, float dt)
{
    return v_prev + 0.5f * (a_prev + a_curr) * dt;
}
void app_velocity_broadcast(void)
{
    bool car_not_moving =
        (app_canRx_INVFL_ActualVelocity_get() == 0) &&
        (app_canRx_INVFR_ActualVelocity_get() == 0) &&
        (app_canRx_INVRL_ActualVelocity_get() == 0) &&
        (app_canRx_INVRR_ActualVelocity_get() == 0);

    const ImuData *imu_data = app_get_imu_struct();
    const float ax = imu_data->long_accel; // X (longitudinal)
    const float ay = imu_data->lat_accel;  // Y (lateral)

    // reset drift when clearly stopped
    if (car_not_moving) {
        for (int i = 0; i < 4; i++) {
            vel_x[i] = 0.0f;
            vel_y[i] = 0.0f;
            velocity[i] = 0.0f;
            // sync filters to current accel to avoid a step on restart
            prev_ax_filt[i] = ax;
            prev_ay_filt[i] = ay;
        }
        prev_accel[0] = ax;
        prev_accel[1] = ay;
        prev_accel[2] = imu_data->z_accel;
        return;
    }

    const float dt = 1.0f / (float)SAMPLE_RATE_HZ;

    // cutoff table
    const float cutoffs[4] = { CUTTOFF_1, CUTTOFF_2, CUTTOFF_3, CUTTOFF_4 };

    for (int i = 0; i < 4; i++) {
        // filter x and y
        float ax_f = lpf(prev_ax_filt[i], ax, SAMPLE_RATE_HZ, (uint16_t)cutoffs[i]);
        float ay_f = lpf(prev_ay_filt[i], ay, SAMPLE_RATE_HZ, (uint16_t)cutoffs[i]);

        // integrate (trapezoidal) into velocity components
        vel_x[i] += integrator(vel_x[i], prev_ax_filt[i], ax_f, dt);
        vel_y[i] += integrator(vel_y[i], prev_ay_filt[i], ay_f, dt);

        // magnitude
        velocity[i] = sqrtf((vel_x[i] * vel_x[i]) + (vel_y[i] * vel_y[i]));

        // update prev filtered values
        prev_ax_filt[i] = ax_f;
        prev_ay_filt[i] = ay_f;
    }

    // keep for any debugging/other use
    prev_accel[0] = ax;
    prev_accel[1] = ay;
    prev_accel[2] = imu_data->z_accel;

    app_canTx_VC_ImuVelocityCutoff5hz_set(velocity[0]);
    app_canTx_VC_ImuVelocityCutoff10hz_set(velocity[1]);
    app_canTx_VC_ImuVelocityCutoff15hz_set(velocity[2]);
    app_canTx_VC_ImuVelocityCutoff20hz_set(velocity[3]);
}
