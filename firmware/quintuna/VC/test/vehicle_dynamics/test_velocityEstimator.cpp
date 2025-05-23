#include <gtest/gtest.h>

extern "C"
{
#include "app_utils.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_velocityEstimator.h"
#include "math.h"
#include "app_units.h"
}

VelocityEstimator_Config VELOCITY_ESTIMATOR_TEST_CONFIG = { .state_estimate_init      = { 1.0f, 2.0f },
                                                            .covariance_estimate_init = { 1.0f, 2.0f, 2.0f, 1.0f },
                                                            .process_noise_cov        = { 0.0f, 0.0f, 0.0f, 0.0f },
                                                            .measurement_noise_cov    = { 0.0f, 0.0f, 0.0f, 0.0f },
                                                            .time_step                = 0.01f };

class VelocityEstimatorTest : public testing::Test
{
};

void print_two_arrs(float *a, float *b, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
    {
        std::cout << a[i] << " --- " << b[i] << std::endl;
    }
}

void print_arr(float *a, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
    {
        printf("%d: %f\n", i, a[i]);
    }
}

TEST_F(VelocityEstimatorTest, init)
{
    float *expected_result_state      = VELOCITY_ESTIMATOR_TEST_CONFIG.state_estimate_init;
    float *expected_result_covariance = VELOCITY_ESTIMATOR_TEST_CONFIG.covariance_estimate_init;

    app_velocityEstimator_init(&VELOCITY_ESTIMATOR_TEST_CONFIG);

    float *actual_result_state      = app_velocityEstimator_getVelocity();
    float *actual_result_covariance = app_velocityEstimator_getCovariance();

    for (int i = 0; i < DIM; i++)
    {
        ASSERT_FLOAT_EQ(expected_result_state[i], actual_result_state[i]);
    }

    for (int i = 0; i < DIM * DIM; i++)
    {
        ASSERT_FLOAT_EQ(expected_result_covariance[i], actual_result_covariance[i]);
    }
}

TEST_F(VelocityEstimatorTest, predict)
{
    app_velocityEstimator_init(&VELOCITY_ESTIMATOR_TEST_CONFIG);

    float ctrl_inputs[] = { 1.0f, 2.0f, 3.0f };
    float state[]       = { 1.0f, 2.0f };

    Matrix prev_state    = { state, DIM, 1 };
    Matrix control_input = { ctrl_inputs, DIM_U, 1 };

    predict(&prev_state, &control_input);

    float expected_result_state[]      = { 1.07f, 2.05f };
    float expected_result_covariance[] = { 1.1209f, 1.9982f, 1.9982f, 0.8809f };

    float *actual_result_state      = app_velocityEstimator_getVelocity();
    float *actual_result_covariance = app_velocityEstimator_getCovariance();

    for (int i = 0; i < DIM; i++)
    {
        ASSERT_FLOAT_EQ(expected_result_state[i], actual_result_state[i]);
    }

    for (int i = 0; i < DIM * DIM; i++)
    {
        ASSERT_FLOAT_EQ(expected_result_covariance[i], actual_result_covariance[i]);
    }
}

TEST_F(VelocityEstimatorTest, update)
{
    float state[] = { 1.07f, 2.05f };
    float meas[]  = { 1.5f, 2.5f };

    Matrix prev_state  = { state, DIM, 1 };
    Matrix measurement = { meas, DIM, 1 };

    update(&measurement, &prev_state);

    float expected_result_state[]      = { 1.5f, 2.5f };
    float expected_result_covariance[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    float *actual_result_state      = app_velocityEstimator_getVelocity();
    float *actual_result_covariance = app_velocityEstimator_getCovariance();

    for (int i = 0; i < DIM; i++)
    {
        ASSERT_FLOAT_EQ(expected_result_state[i], actual_result_state[i]);
    }

    print_two_arrs(actual_result_covariance, expected_result_covariance, 4);
    for (int i = 0; i < DIM * DIM; i++)
    {
        ASSERT_NEAR(expected_result_covariance[i], actual_result_covariance[i], 1e-6f);
    }
}

TEST_F(VelocityEstimatorTest, measurment) {}