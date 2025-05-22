#include <gtest/gtest.h>

extern "C"
{
#include "velocity_estimator.h"
}

class VelocityEstimatorTest : public testing::Test
{
};

TEST_F(VelocityEstimatorTest, test_mult2)
{
    float X[DIM][DIM] = { { 1, 2 }, { 3, 4 } };

    float Y[DIM][DIM] = { { 1, 2 }, { 3, 4 } };

    float result[DIM][DIM];

    mult2(result, X, Y);

    std::cout << result[0][0] << result[0][1] << std::endl;
    std::cout << result[1][0] << result[1][1] << std::endl;
}