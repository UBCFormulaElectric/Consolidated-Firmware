#pragma once
#include <cmath>

#define APPROX_EQUAL_FLOAT(a, b, threshold) ((bool)(fabsf((a) - (b)) < threshold))
#define IS_IN_RANGE(min, max, val) (((val) > (min)) && ((val) < (max)))

#pragma once
#include <array>
#include <iostream>
#include <assert.h>

namespace app::math
{
/**
 * Linear derating algorithm
 * @param x Value on x axis
 * @param max_y Value corresponding to 100% of the y value
 * @param roll_off_x The value to begin derating the power
 * @param limit_x The x value where you want y to be 0%
 * @param derating_direction True if x needs to be decreased, False if x needs to be increased
 * @return Linearly derated value
 */
float app_math_linearDerating(float x, float max_y, float roll_off_x, float limit_x, bool derating_direction);

template <typename T, size_t ROWS, size_t COLS> class matrix
{
    static constexpr std::size_t SIZE = ROWS * COLS;
    std::array<T, SIZE>          mat;

  public:
    /**
     * @brief matrix instantiation using array
     * @param array in_mat
     * @return initialized matrix
     */
    explicit matrix(const std::array<T, SIZE> &in_mat) : mat(in_mat) {}

    /**
     * @brief Zero-initialize matrix
     * @return Zero-initialized matrix
     */
    matrix() : mat{} {}

    /**
     * @brief C-style array style initialization of matrix
     * @param C-style array
     * @return initialized matrix
     */
    matrix(std::initializer_list<T> list)
    {
        assert(list.size() == SIZE);
        std::copy(list.begin(), list.end(), mat.begin());
    }

    T &operator()(const size_t row, const size_t col)
    {
        assert(row < ROWS && col < COLS);
        return mat[row * COLS + col];
    }

    const T &operator()(const size_t row, const size_t col) const
    {
        assert(row < ROWS && col < COLS);
        return mat[row * COLS + col];
    }

    T &operator[](const size_t idx) { return mat[idx]; }

    const T &operator[](const size_t idx) const { return mat[idx]; }

    inline matrix<T, ROWS, COLS> operator+(const matrix<T, ROWS, COLS> &X) const
    {
        matrix<T, ROWS, COLS> Z{};

        for (size_t i = 0; i < SIZE; i++)
        {
            Z[i] = (*this)[i] + X[i];
        }

        return Z;
    }

    friend inline matrix<T, ROWS, COLS> operator+(const T a, const matrix<T, ROWS, COLS> &X)
    {
        matrix<T, ROWS, COLS> Z{};

        for (size_t i = 0; i < SIZE; i++)
        {
            Z[i] = a + X[i];
        }

        return Z;
    }

    inline matrix<T, ROWS, COLS> operator-(const matrix<T, ROWS, COLS> &X) const
    {
        matrix<T, ROWS, COLS> Z{};

        for (size_t i = 0; i < SIZE; i++)
        {
            Z[i] = (*this)[i] - X[i];
        }

        return Z;
    }

    template <size_t OTHER_COLS>
    inline matrix<T, ROWS, OTHER_COLS> operator*(const matrix<T, COLS, OTHER_COLS> &X) const
    {
        matrix<T, ROWS, OTHER_COLS> Z{};

        for (size_t i = 0; i < ROWS; i++)
        {
            for (size_t j = 0; j < OTHER_COLS; j++)
            {
                for (size_t k = 0; k < COLS; k++)
                {
                    Z(i, j) += (*this)(i, k) * X(k, j);
                }
            }
        }

        return Z;
    }

    friend inline matrix<T, ROWS, COLS> operator*(const T a, const matrix<T, ROWS, COLS>& X)
    {
        matrix<T, ROWS, COLS> Z{};

        for (size_t i = 0; i < SIZE; i++)
        {
            Z[i] = a * X[i];
        }

        return Z;
    }

    inline matrix<T, COLS, ROWS> operator~() const
    {
        matrix<T, COLS, ROWS> result{};

        for (size_t i = 0; i < ROWS; i++)
        {
            for (size_t j = 0; j < COLS; j++)
            {
                result(j, i) = (*this)(i, j);
            }
        }

        return result;
    }

    inline matrix<T, COLS, ROWS> inverse() const;

    T sum() const
    {
        T total = T{};
        for (size_t i = 0; i < SIZE; i++)
        {
            total += mat[i];
        }
        return total;
    }

    // std::ostream& operator<<(std::ostream& out, const matrix<T, ROWS, COLS>& X)
};
} // namespace app::math
