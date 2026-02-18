#pragma once
#include <cmath>
#include <array>
#include <iostream>
#include <assert.h>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <utility>


#define APPROX_EQUAL_FLOAT(a, b, threshold) ((bool)(fabsf((a) - (b)) < threshold))
#define IS_IN_RANGE(min, max, val) (((val) > (min)) && ((val) < (max)))

namespace app { namespace math{

    template<typename T, size_t R, size_t C>
    class matrix{

        static_assert(R != 0 && C != 0, "matrix dimensions must be non-zero and non-negagtive");
        static_assert(R <= (std::numeric_limits<std::size_t>::max() / C), "R*C overflows size_t");

        private:
        static constexpr size_t mat_size = R * C;;
        std::array<T, mat_size> mat{};

        
        /**
        * @brief helper function for finding matrix index for row dominant flatened array  
        * @param row number and coloumn numer of index 
        * @return flatened matrix index 
        */
        static constexpr size_t idx(size_t row, size_t col)
        {
            assert(row < R && col < C); 
            return row * C + col;
        }

        public:
            /**
            * @brief zero initialized matrix;
            */
            constexpr matrix() = default; 

            /**
            * @brief initialization of flat matrix from an initializer list 
            * @param initializer list
            * @return initialized matrix
            */
            explicit constexpr matrix(std::initializer_list<T> list)
            {
                assert(list.size() == mat_size);
                std::copy(list.begin(), list.end(), mat.begin());
            }

            /**
            * @brief flat matrix initialization from array
            * @param array of matrix items
            * @return initialized flat matrix
            */
            explicit constexpr matrix(const std::array<T, mat_size> &in_mat)
            {
                mat = in_mat; 
            }; 

            /**
            * @brief overloading () coloumn major indexing for matrices
            * @param row and col position of desired index
            * @return matrix element
            */
            T& operator() (const size_t row, const size_t col)
            {
                assert(row < R && col < C );
                return mat[idx(row,col)];
            }

            // read only version for const matrix
            const T& operator() (const size_t row, const size_t col) const
            {
                assert(row < R && col < C );
                return mat[idx(row,col)]; 
            }

            /**
            * @brief overloading [] array like indexing for matrices
            * @param index of array in flatened config
            * @return matrix element
            */
            T& operator[] (const size_t idx)
            {
                assert(idx < mat_size);
                return mat[idx];
            }

            // read only version for const matrix
            const T& operator[] (const size_t idx) const
            {
                assert(idx < mat_size);
                return mat[idx]; 
            }

            /**
            * @brief overloading ~ for the transpose of a matrix 
            * @return transposed matrix
            */
            constexpr matrix<T, C, R> operator~() const {
                matrix<T, C, R> out{};
                for (size_t r = 0; r < R; ++r)
                    for (size_t c = 0; c < C; ++c)
                        out(c,r) = (*this)(r,c);
                return out;
            }

            /**
            * @brief overloading + to add two matrices 
            * @param matrix 1, matrix 2
            * @return matrix 
            */
            friend constexpr matrix<T,R,C> operator+ (const matrix &mat_1, const matrix &mat_2) 
            {
                matrix out; 
                for(size_t i = 0; i < mat_1.mat_size; i++) out[i] = mat_1[i] + mat_2[i];
                return out;
            }

            /**
            * @brief overloading - to subtract two matrices 
            * @param matrix 1, matrix 2
            * @return resultant matrix
            */
            friend constexpr matrix<T,R,C> operator-(const matrix& a, const matrix& b) 
            {
            matrix out{};
            for (size_t i = 0; i < mat_size; ++i) out[i] = a[i] - b[i];
            return out;
            }

            /**
            * @brief overloading * to multiply two matrices 
            * @param matrix 1, matrix 2
            * @return resultant matrix of multiplication  
            */
            template<size_t OTHER_COLS>
            friend matrix<T, R, OTHER_COLS> operator*(const matrix& mat_1, const matrix<T, C, OTHER_COLS>& mat_2)
            {
                matrix<T, R, OTHER_COLS> out;

                for (size_t i = 0; i < R; i++)
                    for (size_t j = 0; j < OTHER_COLS; j++)
                        for (size_t k = 0; k < C; k++)
                            out(i, j) += mat_1(i, k) * mat_2(k, j);

                return out;
            }

            /**
            * @brief overloading * to multiply a matrix by a scalr
            * @param matrix, scalar 
            * @return resultant matrix of multiplication  
            */
            friend matrix<T, R, C> operator*(const matrix& mat, const T scalar)
            {
                assert(std::abs(scalar) > T(0));
                matrix<T, R, C> out;

                for(size_t i = 0; i < mat_size; ++i)
                {
                    out[i] = mat[i] * scalar;
                }

                return out;
            }

            /**
            * NO MATRIX BY MATRIX DIVISION OPERATOR YET, CAN IMPLEMENT IF NEEDED
            */

            /**
            * @brief overloading / to divide a matrix by a scalar
            * @param matrix, scalar 
            * @return resultant matrix of division  
            */
            friend matrix<T,R,C> operator/(const matrix& m, T s) 
            {
                assert(std::abs(s) > T(0));
                matrix out{};
                for (size_t i = 0; i < mat_size; ++i) out[i] = m[i] / s;
                return out;
            }

            /**
            * @brief proivdes identity square matrix of RxC 
            * @return resultant matrix of multiplication  
            */
            static constexpr matrix<T,R,C> identity()
            {
                static_assert(R == C, "can only provide square matrices for identity");
                matrix<T,R,C> I;
                for(size_t i = 0; i < R; ++i)
                {
                    I(i,i) = T(1); 
                }
                return I; 
            }

            /**
            * @brief inverse function for square matrix
            * @note inverting a matrix is expensive. OK for small/medium size matrix lets say no larger than 6.
            */
            matrix<T, R, C> inverse(T eps = static_cast<T>(1e-6)) const {
                static_assert(R == C, "inverse() only valid for square matrices");

                matrix<T, R, C> out; // will become inverse

                // 2x2 special case (fast path)
                if constexpr (R == 2 && C == 2) {
                    const T a = (*this)(0,0);
                    const T b = (*this)(0,1);
                    const T c = (*this)(1,0);
                    const T d = (*this)(1,1);

                    const T det = a*d - b*c;
                    assert(std::abs(det) > eps && "Matrix is singular / not invertible");

                    const T invdet = static_cast<T>(1) / det;
                    out(0,0) =  d * invdet;
                    out(0,1) = -b * invdet;
                    out(1,0) = -c * invdet;
                    out(1,1) =  a * invdet;
                    return out;
                }
                else {
                    matrix<T, R, C> A = *this; // working copy

                    // out = Identity
                    for (std::size_t i = 0; i < R; ++i) {
                        for (std::size_t j = 0; j < C; ++j) {
                            out(i,j) = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
                        }
                    }

                    for (std::size_t col = 0; col < R; ++col) {
                        // Find pivot row (partial pivoting)
                        std::size_t pivot = col;
                        T maxAbs = std::abs(A(col, col));
                        for (std::size_t r = col + 1; r < R; ++r) {
                            T v = std::abs(A(r, col));
                            if (v > maxAbs) { maxAbs = v; pivot = r; }
                        }

                        // Singular if pivot too small
                        assert(maxAbs > eps && "Matrix is singular / not invertible");

                        // Swap rows in A and out if needed
                        if (pivot != col) {
                            for (std::size_t j = 0; j < R; ++j) {
                                std::swap(A(col, j), A(pivot, j));
                                std::swap(out(col, j), out(pivot, j));
                            }
                        }

                        // Normalize pivot row
                        const T piv = A(col, col);
                        for (std::size_t j = 0; j < R; ++j) {
                            A(col, j)   /= piv;
                            out(col, j) /= piv;
                        }

                        // Eliminate column in all other rows
                        for (std::size_t r = 0; r < R; ++r) {
                            if (r == col) continue;
                            const T factor = A(r, col);
                            if (std::abs(factor) <= eps) continue;

                            for (std::size_t j = 0; j < R; ++j) {
                                A(r, j)   -= factor * A(col, j);
                                out(r, j) -= factor * out(col, j);
                            }
                        }
                    }

                    return out;
                }

            } 
        
    };
    
}
}