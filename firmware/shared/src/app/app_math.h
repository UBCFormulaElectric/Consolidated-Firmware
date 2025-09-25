#pragma once
#include "stdbool.h"

// Direction to derate x variable
#define REDUCE_X true
#define INCREASE_X false

#include <stdint.h>
#include <stdbool.h>

/**
 * Matrix
 * 
 * Used for all linear algebra related operations
 * 
 * mat array should be initialized as a single dimension array,
 * it is implemented this way for simplicity and cache optimization
 * 
 * rows and cols should be inputted as such
 */
typedef struct
{
    float   *mat;
    uint32_t rows;
    uint32_t cols;
} Matrix;


/**
 * ================================
 * Linear Algebra Functions
 * ================================
 */

/*
 * Matrix Addition: result = A + B
 */
static inline bool add(Matrix *result, const Matrix *A, const Matrix *B)
{
    // checks if matrix dimensions are violated
    if (A->rows != B->rows || A->cols != B->cols || result->rows != A->rows || result->cols != A->cols)
        return false;

    // addition algorithm
    for (uint32_t i = 0; i < A->rows * A->cols; i++)
    {
        result->mat[i] = A->mat[i] + B->mat[i];
    }

    return true;
}

/*
 * Matrix Subtraction: result = A - B
 */
static inline bool sub(Matrix *result, const Matrix *A, const Matrix *B)
{
    // checks if matrix dimensions are violated
    if (A->rows != B->rows || A->cols != B->cols || result->rows != A->rows || result->cols != A->cols)
        return false;

    // subtraction algorithm
    for (uint32_t i = 0; i < A->rows * A->cols; i++)
    {
        result->mat[i] = A->mat[i] - B->mat[i];
    }

    return true;
}

/*
 * Matrix Multiplication: result = A * B
 */
static inline bool mult(Matrix *result, const Matrix *A, const Matrix *B)
{
    // checks if matrix dimensions are violated
    if (A->cols != B->rows || result->rows != A->rows || result->cols != B->cols)
    {
        return false;
    }

    for (uint32_t i = 0; i < result->rows * result->cols; i++)
    {
        result->mat[i] = 0.0f;
    }

    // multiplication algorithm cache optimized
    for (uint32_t i = 0; i < A->rows; i++)
    {
        for (uint32_t j = 0; j < B->cols; j++)
        {
            for (uint32_t k = 0; k < A->cols; k++)
            {
                result->mat[i * result->cols + j] += A->mat[i * A->cols + k] * B->mat[k * B->cols + j];
            }
        }
    }

    return true;
}

/*
 * Matrix Transpose: result = input^T
 */
static inline bool transpose(Matrix *result, const Matrix *input)
{
    // checks if matrix dimensions are violated
    if (result->rows != input->cols || result->cols != input->rows)
    {
        return false;
    }

    // transpose algorithm
    for (uint32_t i = 0; i < input->rows; i++)
    {
        for (uint32_t j = 0; j < input->cols; j++)
        {
            result->mat[j * result->cols + i] = input->mat[i * input->cols + j];
        }
    }

    return true;
}

/*
 * 2x2 Matrix Inversion: result = X^-1
 */
static inline bool inverse2x2(Matrix *result, const Matrix *X)
{
    // checks if matrix dimensions are violated
    if (X->rows != 2 || X->cols != 2 || result->rows != 2 || result->cols != 2)
    {
        return false;
    }

    float a = X->mat[0];
    float b = X->mat[1];
    float c = X->mat[2];
    float d = X->mat[3];

    // compute determinant
    float det = a * d - b * c;
    if (det == 0.0f)
        return false;

    // inverse determinant
    float inv_det = 1.0f / det;

    // inverse matrix
    result->mat[0] = d * inv_det;
    result->mat[1] = -b * inv_det;
    result->mat[2] = -c * inv_det;
    result->mat[3] = a * inv_det;

    return true;
}

/**
 * Trapezoidal approximation used to approximate a definite integral
 * @param integral The current value of the integral
 * @param f_x0 The previous value of f(x) used to approximate the value of the current trapezoid
 * @param f_x1 The current value of f(x) used to approximate the value of the current trapezoid
 * @param d_x The uniform delta-x of each trapezoid
 */
static inline void app_math_trapezoidalRule(double *integral, float *f_prev, const float f_curr, const double d_x)
{
    // Calculate the trapezoid and add to the previous integral
    // Double is used here as the extra precision is needed when adding a small delta to a large integral amount
    *integral = *integral + (d_x * (double)(*f_prev + f_curr) * 0.5);
    *f_prev   = f_curr;
}

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
