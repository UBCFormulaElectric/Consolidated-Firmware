#include <gtest/gtest.h>

#include <array>

#include "app_math.hpp"

using app::math::matrix;

// helper function
template <typename T, std::size_t R, std::size_t C>
void ExpectMatrixNear(const matrix<T, R, C>& actual, const matrix<T, R, C>& expected, T tol)
{
    for (std::size_t r = 0; r < R; ++r)
    {
        for (std::size_t c = 0; c < C; ++c)
        {
            EXPECT_NEAR(actual(r, c), expected(r, c), tol);
        }
    }
}

TEST(MatrixTest, test_zero_init_constructor)
{
    matrix<float, 2, 3> m;
    for (std::size_t r = 0; r < 2; ++r)
    {
        for (std::size_t c = 0; c < 3; ++c)
        {
            EXPECT_FLOAT_EQ(m(r, c), 0.0f);
        }
    }
}

TEST(MatrixTest, initializer_list_and_indexing)
{
    matrix<float, 2, 3> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };

    // test matrix like indexing -- (row,col)
    EXPECT_FLOAT_EQ(m(0, 0), 1.0f);
    EXPECT_FLOAT_EQ(m(0, 1), 2.0f);
    EXPECT_FLOAT_EQ(m(0, 2), 3.0f);
    EXPECT_FLOAT_EQ(m(1, 0), 4.0f);
    EXPECT_FLOAT_EQ(m(1, 1), 5.0f);
    EXPECT_FLOAT_EQ(m(1, 2), 6.0f);

    // test array list indexing
    EXPECT_FLOAT_EQ(m[0], 1.0f);
    EXPECT_FLOAT_EQ(m[1], 2.0f);
    EXPECT_FLOAT_EQ(m[2], 3.0f);
    EXPECT_FLOAT_EQ(m[3], 4.0f);
    EXPECT_FLOAT_EQ(m[4], 5.0f);
    EXPECT_FLOAT_EQ(m[5], 6.0f);

    m(1, 1) = 42.0f;
    EXPECT_FLOAT_EQ(m[4], 42.0f);
}

TEST(MatrixTest, array_constructor)
{
    // using double to ensure both floats and doubles work
    std::array<double, 4> data{ { 1.0f, 2.0f, 3.0f, 4.0f } };
    matrix<double, 2, 2>  m(data);

    EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(m(1, 0), 3.0);
    EXPECT_DOUBLE_EQ(m(1, 1), 4.0);

    // check array like indexing again 
    EXPECT_DOUBLE_EQ(m[0], 1.0);
    EXPECT_DOUBLE_EQ(m[1], 2.0);
    EXPECT_DOUBLE_EQ(m[2], 3.0);
    EXPECT_DOUBLE_EQ(m[3], 4.0);
}

TEST(MatrixTest, transpose_round_trip)
{
    constexpr float kTol = 1e-6f;

    matrix<float, 2, 3> m{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
    matrix<float, 3, 2> expected{ 1.0f, 4.0f, 2.0f, 5.0f, 3.0f, 6.0f };

    matrix<float, 3, 2> t_m = ~m; // transpose m
    ExpectMatrixNear(t_m, expected, kTol); // should match expected transpose matrix

    matrix<float, 2, 3> tt_m = ~t_m; // transpose it back 
    ExpectMatrixNear(tt_m, m, kTol); // should original matrix
}

TEST(MatrixTest, addition_and_subtraction)
{
    constexpr float kTol = 1e-6f;

    matrix<float, 2, 2> a{ 1.0f, 2.0f, 3.0f, 4.0f };
    matrix<float, 2, 2> b{ 5.0f, 6.0f, 7.0f, 8.0f };

    matrix<float, 2, 2> expected_sum{ 6.0f, 8.0f, 10.0f, 12.0f };
    matrix<float, 2, 2> expected_diff{ 4.0f, 4.0f, 4.0f, 4.0f };

    ExpectMatrixNear(a + b, expected_sum, kTol);
    ExpectMatrixNear(b - a, expected_diff, kTol);
}

TEST(MatrixTest, matrix_multiplication)
{
    constexpr float kTol = 1e-6f;

    matrix<float, 2, 3> a{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
    matrix<float, 3, 2> b{ 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f };
    matrix<float, 2, 2> expected{ 58.0f, 64.0f, 139.0f, 154.0f };

    ExpectMatrixNear(a * b, expected, kTol);
}

TEST(MatrixTest, scalar_multiplication_and_division)
{
    constexpr float kTol = 1e-6f;

    matrix<float, 2, 2> m{ 1.0f, 2.0f, 3.0f, 4.0f };
    matrix<float, 2, 2> expected_scaled{ 2.0f, 4.0f, 6.0f, 8.0f };
    matrix<float, 2, 2> expected_divided{ 0.5f, 1.0f, 1.5f, 2.0f };

    ExpectMatrixNear(m * 2.0f, expected_scaled, kTol);
    ExpectMatrixNear(m / 2.0f, expected_divided, kTol);
}

TEST(MatrixTest, identity_matrix)
{
    constexpr float kTol = 1e-6f;

    matrix<float, 3, 3> identity = matrix<float, 3, 3>::identity();
    matrix<float, 3, 3> expected{ 1.0f, 0.0f, 0.0f,
                                  0.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 1.0f };

    ExpectMatrixNear(identity, expected, kTol);

    matrix<float, 3, 3> m{ 2.0f, 1.0f, 3.0f,
                           4.0f, 1.0f, 6.0f,
                           7.0f, 8.0f, 9.0f };
    ExpectMatrixNear(m * identity, m, kTol);
}

TEST(MatrixTest, inverse_2x2)
{
    constexpr float kTol = 1e-6f;

    matrix<float, 2, 2> m{ 4.0f, 7.0f, 2.0f, 6.0f };
    matrix<float, 2, 2> expected{ 0.6f, -0.7f, -0.2f, 0.4f };

    ExpectMatrixNear(m.inverse(), expected, kTol);
}

TEST(MatrixTest, inverse_3x3_round_trip)
{
    constexpr float kTol = 1e-5f;

    matrix<float, 3, 3> m{ 1.0f, 2.0f, 3.0f,
                           0.0f, 1.0f, 4.0f,
                           5.0f, 6.0f, 0.0f };
    matrix<float, 3, 3> identity = matrix<float, 3, 3>::identity();

    matrix<float, 3, 3> inv = m.inverse();
    ExpectMatrixNear(m * inv, identity, kTol);
    ExpectMatrixNear(inv * m, identity, kTol);
}
