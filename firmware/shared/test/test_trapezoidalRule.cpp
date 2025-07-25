#include <gtest/gtest.h>
#include <math.h>

extern "C"
{
#include "app_math.h"
}

TEST(VoltageSenseTest, tractive_system_voltage_calculation)
{
    double integral = 0.0f;
    float  prev_fx  = 0.0f;

    // A vector between 0 and 2pi, in pi/4 increments
    const std::vector<float> x_vec{ 0.0,         M_PI * 0.25, M_PI * 0.5,  M_PI * 0.75, M_PI,
                                    M_PI * 1.25, M_PI * 1.5,  M_PI * 1.75, M_PI * 2.0 };

    // Expected value calculated using Wolfram's Trapezium Rule MyAlevelMathsTutor
    const std::vector<float> expected_val{ 0.0, 0.27768, 0.948059, 1.61844, 1.896119, 1.61844, 0.948059, 0.27768, 0.0 };

    for (std::size_t i = 0, e = x_vec.size(); i != e; i++)
    {
        app_math_trapezoidalRule(&integral, &prev_fx, sin(x_vec[i]), M_PI_4);
        ASSERT_NEAR(integral, expected_val[i], 1e-4);
    }
}
