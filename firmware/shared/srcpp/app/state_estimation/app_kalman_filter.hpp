#include "app_math.hpp"
#include <array>
#include <cstddef>
#include <cmath>
#include <functional>
#include <Eigen/Dense>
#include <autodiff/forward/dual.hpp>

namespace app::state_estimation{

/**
* @brief The kalman filter class will serve as a simplification of the extended kalman filter. The idea is that for linear systems (or systems that can be easily linearized) 
* we use the kalman filter rather than the EKF.
*/

/* Notation
* T : primitive data type (float or double)
* N : number of states
* M : number of measurement inputs
* U : inputs (incase we want to do multiple input)
*/

/** Matrix dimensions
* Everything is driven from three system sepcifications, t
* those being how many states our system estimates, number of measurements from sensors and how many inputs we have
* F (NxN) : represents how our states evolve with respect to each other 
* covariance matrixes are always square as they relate variables to each other
* B (NxU) : represents how our state evolves with respect to the input
* Q (NxN) : the noise in the model (how the noise in one state effect others)
* R (MxM) : the noise in the measurements (and how the noises of a measurement affect another measurement)
* H (MxN) : maps state prediction to measurement inputs 
* S (MxM) : residuial covariacne of how uncertain measurement error is (freaky)
* K (NxM) : basically decides how much to trust measurement vs prediction
*/


template<typename T, std::size_t N, std::size_t M, std::size_t U>
class kf {
public:
    using N_N = Eigen::Matrix<T, N, N>;
    using N_1 = Eigen::Matrix<T, N, 1>; 
    using M_1 = Eigen::Matrix<T, M, 1>;
    using U_1 = Eigen::Matrix<T, U, 1>;
    using M_M = Eigen::Matrix<T, M, M>; 
    using M_N = Eigen::Matrix<T, M, N>;
    using N_M = Eigen::Matrix<T, N, M>;
    using N_U = Eigen::Matrix<T, N, U>; 

    constexpr explicit kf(const N_N& F, const N_U& B, const N_N& Q, 
                          const M_N& H, const M_M& R, const N_1& x0, const N_N& P0): 
        F_(F), B_(B), Q_(Q), H_(H), R_(R), x_(x0), P_(P0) {}

    void predict(const U_1& u) {
        x_ = F_ * x_ + (B_ * u);
        P_ = F_ * P_ * F_.transpose() + Q_;
    }

    void update(const M_1& z) {
        const M_1 y = z - (H_ * x_);
        const M_M S = (H_ * P_ * H_.transpose()) + R_;
        const N_M K = (P_ * H_.transpose()) * S.inverse();

        x_ = x_ + K * y;
        P_ = (N_N::Identity() - K * H_) * P_;
    }

    const N_1& x_vec() const { return x_; }
    const N_N& P_mat() const { return P_; }

private:
    N_N F_, Q_;
    N_U B_;
    M_N H_;
    M_M R_;

    N_1 x_;
    N_N P_;
};

/**
 * @brief Extended Kalman Filter (EKF) with automatic Jacobian computation via autodifferentiation
 * 
 * Uses Eigen for fixed-size matrices and autodiff for computing F and H Jacobians.
 * No dynamic memory allocation - all matrix sizes are known at compile time.
 * 
 * Template parameters:
 * - T: scalar type (float or double, must support autodiff dual numbers)
 * - N: number of states
 * - M: number of measurements
 * 
 * State transition function f: R^N -> R^N (nonlinear)
 * Measurement function h: R^N -> R^M (nonlinear)
 * 
 * User provides function pointers for each row of f and h:
 * - f[i]: computes x_dot[i] = f[i](x) where x is the full state vector
 * - h[i]: computes z_predicted[i] = h[i](x) where x is the full state vector
 */
template <typename T, std::size_t N, std::size_t M>
class ekf {
public:
    using N_N = Eigen::Matrix<T, N, N>;
    using M_M = Eigen::Matrix<T, M, M>; 
    using N_1 = Eigen::Matrix<T, N, 1>;
    using M_1 = Eigen::Matrix<T, M, 1>;
    using M_N = Eigen::Matrix<T, M, N>;
    using N_M = Eigen::Matrix<T, N, M>;

    // Function signature: takes array of N dual numbers representing states, returns dual number
    // Each function computes one row of the state transition or measurement vector
    using StateFunction = std::function<autodiff::dual(const std::array<autodiff::dual, N>&)>;
    using MeasurementFunction = std::function<autodiff::dual(const std::array<autodiff::dual, N>&)>;

    /**
     * @brief Constructor for EKF
     * 
     * @param f Array of N functions, where f[i](x) computes the i-th state derivative
     * @param h Array of M functions, where h[i](x) computes the i-th measurement prediction
     * @param Q Process noise covariance (NxN) - how much we trust the model
     * @param R Measurement noise covariance (MxM) - how much we trust measurements
     * @param x0 Initial state estimate (Nx1)
     * @param P0 Initial state covariance estimate (NxN)
     */
    ekf(const std::array<StateFunction, N>& f,
        const std::array<MeasurementFunction, M>& h,
        const N_N& Q,
        const M_M& R,
        const N_1& x0,
        const N_N& P0)
    : f_(f), h_(h), Q_(Q), R_(R), x_(x0), P_(P0) {}

    /**
     * @brief Predict step: compute Jacobian F, then update state and covariance
     * 
     * This is called when there is no measurement, only state evolution.
     * Computes F = df/dx at current state, then:
     *   x = f(x)
     *   P = F*P*F^T + Q
     */
    void predict() {
        computeF(); // compute jacobian (linearized system) at current time instance
        x_ = evaluateStateFunction(x_); // predict current state by evaluationg non-linear functions using previous state and input
        P_ = F_ * P_ * F_.transpose() + Q_; // compute prediction covariacne using current linearized system for current time instance
    }

    /**
     * @brief Update step: compute Jacobian H, then fuse measurement
     * 
     * Called when a measurement is available.
     * Computes H = dh/dx at current state, then:
     *   y = z - h(x)  (measurement innovation)
     *   S = H*P*H^T + R  (innovation covariance)
     *   K = P*H^T*S^{-1}  (Kalman gain)
     *   x = x + K*y  (state update)
     *   P = (I - K*H)*P  (covariance update)
     * 
     * @param z Sensor Measurement vector (Mx1)
     */
    void update(const M_1& z) {
        computeH();
        const M_1 y = z - evaluateMeasurementFunction(x_); // find the noise between the measurement and predicted state
        const M_M S = H_ * P_ * H_.transpose() + R_; // find the evolution covariance using linearized measurement evolution functions and measurement noise
        const N_M K = P_ * H_.transpose() * S.inverse(); // find the kalman gain... lets use Cholesky decomp instead of an inverse here
        
        x_ = x_ + K * y;
        P_ = (N_N::Identity() - K * H_) * P_;
    }

    // getter functions
    const N_1& x_vec() const { return x_; }
    const N_N& P_mat() const { return P_; }
    const N_N& F_mat() const { return F_; }
    const M_N& H_mat() const { return H_; }

private:
    std::array<StateFunction, N> f_;
    std::array<MeasurementFunction, M> h_;
    N_N Q_, P_, F_;
    M_M R_;
    M_N H_;
    N_1 x_;

    /**
     * @brief Compute Jacobian F = df/dx using automatic differentiation
     * 
     * For each state function f_i, computes the i-th row of the Jacobian matrix.
     * F(i,j) = df_i/dx_j using autodiff's built-in jacobian function.
     */
    void computeF() {
        // Convert current state to dual numbers for differentiation
        Eigen::Matrix<autodiff::dual, N, 1> x_dual = x_.template cast<autodiff::dual>();
        
        for (std::size_t i = 0; i < N; ++i) {
            // Create a lambda that captures the i-th function and evaluates it
            auto f_i = [this, i](const Eigen::Matrix<autodiff::dual, N, 1>& x) {
                // Convert Eigen vector to std::array for the state function
                std::array<autodiff::dual, N> x_array;
                for (std::size_t k = 0; k < N; ++k) {
                    x_array[k] = x(k);
                }
                return f_[i](x_array);
            };
            
            // Compute the jacobian (1×N matrix) for f_i using autodiff
            Eigen::Matrix<T, 1, N> jac_row;
            autodiff::jacobian(f_i, wrt(x_dual), at(x_dual), jac_row);
            
            // Extract the row into F
            F_.row(i) = jac_row;
        }
    }

    /**
     * @brief Compute Jacobian H = dh/dx using automatic differentiation
     * 
     * For each measurement function h_i, computes the i-th row of the Jacobian matrix.
     * H(i,j) = dh_i/dx_j using autodiff's built-in jacobian function.
     */
    void computeH() {
        // Convert current state to dual numbers for differentiation
        Eigen::Matrix<autodiff::dual, N, 1> x_dual = x_.template cast<autodiff::dual>();
        
        for (std::size_t i = 0; i < M; ++i) {
            // Create a lambda that captures the i-th function and evaluates it
            auto h_i = [this, i](const Eigen::Matrix<autodiff::dual, N, 1>& x) {
                // Convert Eigen vector to std::array for the measurement function
                std::array<autodiff::dual, N> x_array;
                for (std::size_t k = 0; k < N; ++k) {
                    x_array[k] = x(k);
                }
                return h_[i](x_array);
            };
            
            // Compute the jacobian (1×N matrix) for h_i using autodiff
            Eigen::Matrix<T, 1, N> jac_row;
            autodiff::jacobian(h_i, wrt(x_dual), at(x_dual), jac_row);
            
            // Extract the row into H
            H_.row(i) = jac_row;
        }
    }

    /**
     * @brief Evaluate state transition function f at current state (non-differentiating evaluation)
     * 
     * Converts state vector to array of dual numbers (with zero derivatives), evaluates all
     * state functions, and extracts the values.
     * 
     * @param x State vector
     * @return f(x) - the state derivative vector
     */
    N_1 evaluateStateFunction(const N_1& x) {
        N_1 result;
        std::array<autodiff::dual, N> x_dual;
        
        // Convert state to dual (with zero derivative for value-only evaluation)
        for (std::size_t i = 0; i < N; ++i) {
            x_dual[i] = autodiff::dual(x(i), 0.0);
        }
        
        // Evaluate each state function
        for (std::size_t i = 0; i < N; ++i) {
            result(i) = f_[i](x_dual).value;
        }
        
        return result;
    }

    /**
     * @brief Evaluate measurement function h at current state (non-differentiating evaluation)
     * 
     * Similar to evaluateStateFunction but for measurement functions.
     * 
     * @param x State vector
     * @return h(x) - the predicted measurement vector
     */
    M_1 evaluateMeasurementFunction(const N_1& x) {
        M_1 result;
        std::array<autodiff::dual, N> x_dual;
        
        // Convert state to dual (with zero derivative for value-only evaluation)
        for (std::size_t i = 0; i < N; ++i) {
            x_dual[i] = autodiff::dual(x(i), 0.0);
        }
        
        // Evaluate each measurement function
        for (std::size_t i = 0; i < M; ++i) {
            result(i) = h_[i](x_dual).value;
        }
        
        return result;
    }
};
}
}
