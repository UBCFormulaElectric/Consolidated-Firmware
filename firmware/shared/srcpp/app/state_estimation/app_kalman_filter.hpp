#pragma once
#include <cassert>
#include <Eigen/Dense>
#include <dual.hpp>
#include <gradient.hpp>
namespace app::state_estimation{
namespace detail {
    /* Linear algebra terms used below
    * PSD : Positive Semi-Definite; covariance is valid with eigenvalues >= 0.
    * SPD : Symmetric Positive Definite; stricter than PSD and required by LLT solve.
    * LDLT : Matrix factorization A = L D L^T for symmetric matrices; useful for PSD checks.
    * LLT : Cholesky factorization A = L L^T for SPD matrices; used for fast/stable solves.
    * Cholesky decomposition : Rewriting an SPD matrix into triangular factors to solve Ax=b efficiently without explicit inverse.
    */


    /**
    * @brief Return the symmetric projection of a square matrix.
    *
    * This is used before PSD/SPD checks to damp out tiny floating-point
    * asymmetries (A != A^T by epsilon) that should not fail covariance checks.
    */
    template<typename Mat>
    auto symmetrize(const Mat& matrix) -> Mat
    {
        return (matrix + matrix.transpose()) * static_cast<typename Mat::Scalar>(0.5);
    }

    /**
    * @brief Check whether a matrix is symmetric within tolerance.
    *
    * Covariance matrices (P, Q, R) must be symmetric. This check is applied
    * in constructors before the filter is used.
    */
    template<typename Mat>
    auto is_symmetric(const Mat& matrix, typename Mat::Scalar tol = static_cast<typename Mat::Scalar>(1e-6)) -> bool
    {
        return matrix.isApprox(matrix.transpose(), tol);
    }

    /**
    * @brief Check whether a matrix is positive semidefinite (PSD).
    *
    * PSD is required for state covariance and process covariance matrices.
    * The LDLT factorization is used and the D diagonal is checked against
    * a small negative tolerance to handle numerical round-off.
    */
    template<typename Mat>
    auto is_positive_semidefinite(const Mat& matrix, typename Mat::Scalar tol = static_cast<typename Mat::Scalar>(1e-8)) -> bool
    {
        const Mat symmetric = symmetrize(matrix);
        Eigen::LDLT<Mat> ldlt(symmetric);

        if (ldlt.info() != Eigen::Success) return false;

        return (ldlt.vectorD().array() >= -tol).all();
    }

    /**
    * @brief Check whether a matrix is positive definite (PD).
    *
    * The EKF/KF update step solves with LLT, which assumes the innovation
    * covariance is SPD. Requiring R to be PD keeps this assumption valid.
    */
    template<typename Mat>
    auto is_positive_definite(const Mat& matrix) -> bool
    {
        const Mat symmetric = symmetrize(matrix);
        Eigen::LLT<Mat> llt(symmetric);
        return llt.info() == Eigen::Success;
    }

    /**
    * @brief Validate covariance constraints and fail fast with clear assertions.
    *
    * Enforces:
    * - P0 symmetric and PSD
    * - Q symmetric and PSD
    * - R symmetric and PD (required by LLT solve in update step)
    */
    template<typename StateCov, typename MeasurementCov>
    void assert_covariance_constraints(const StateCov& P0, const StateCov& Q, const MeasurementCov& R)
    {
        assert(is_symmetric(P0) && "Kalman filter constructor: P0 must be symmetric.");
        assert(is_positive_semidefinite(P0) && "Kalman filter constructor: P0 must be positive semidefinite (all eigenvalues must be >= 0).");
        assert(is_symmetric(Q) && "Kalman filter constructor: Q must be symmetric.");
        assert(is_positive_semidefinite(Q) && "Kalman filter constructor: Q must be positive semidefinite (all eigenvalues must be >= 0).");
        assert(is_symmetric(R) && "Kalman filter constructor: R must be symmetric.");
        assert(is_positive_definite(R) && "Kalman filter constructor: R must be positive definite because update() uses LLT for solving innovation covariance.");
    }
}

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
    * those being how many states our system estimates, number of measurements from MEASUREMENTS and how many inputs we have
    * F (NxN) : represents how our states evolve with respect to each other 
    * covariance matrixes are always square as they relate variables to each other
    * B (NxU) : represents how our state evolves with respect to the input
    * Q (NxN) : the noise in the model (how the noise in one state effect others)
    * R (MxM) : the noise in the measurements (and how the noises of a measurement affect another measurement)
    * H (MxN) : maps state prediction to measurement inputs 
    * S (MxM) : residuial covariacne of how uncertain measurement error is (freaky)
    * K (NxM) : basically decides how much to trust measurement vs prediction
    */
    template<typename T, std::size_t STATES, std::size_t MEASUREMENTS, std::size_t INPUTS>
    class kf {
    public:
        using N_N = Eigen::Matrix<T, STATES, STATES>;
        using N_1 = Eigen::Matrix<T, STATES, 1>; 
        using M_1 = Eigen::Matrix<T, MEASUREMENTS, 1>;
        using U_1 = Eigen::Matrix<T, INPUTS, 1>;
        using M_M = Eigen::Matrix<T, MEASUREMENTS, MEASUREMENTS>; 
        using M_N = Eigen::Matrix<T, MEASUREMENTS, STATES>;
        using N_M = Eigen::Matrix<T, STATES, MEASUREMENTS>;
        using N_U = Eigen::Matrix<T, STATES, INPUTS>; 

        /**
        * @brief Constructor for KF
        * 
        * @param F Square matrix (NxN) representing state evolution with respect to previous state
        * @param B NxU matrix representing the evolution of the state with respect to the previous inputs
        * @param H MxN matrix measurement matrix transforming state to measurements 
        * @param Q Process noise covariance (NxN) - how much we trust the model
        * @param R Measurement noise covariance (MxM) - how much we trust measurements
        * @param x0 Initial state estimate (Nx1)
        * @param P0 Initial state covariance estimate (NxN)
        */
        explicit kf(const N_N& F, const N_U& B, const N_N& Q, 
                            const M_N& H, const M_M& R, const N_1& x0, const N_N& P0): 
            F_(F), Q_(Q), B_(B), H_(H), R_(R), x_(x0), P_(P0)
        {
            detail::assert_covariance_constraints(P0, Q, R);
        }

        const N_1& state() const { return x_; }
        const N_N& covariance() const { return P_; }


        const N_1 estimated_states(const U_1 &inputs, const M_1 &measurements){ 
            predict(inputs);
            update(measurements);
            return x_; 
        }

    private:
        N_N F_, Q_;
        N_U B_;
        M_N H_;
        M_M R_;

        N_1 x_;
        N_N P_;

        void predict(const U_1& u) {
            x_ = F_ * x_ + (B_ * u);
            P_ = F_ * P_ * F_.transpose() + Q_;
        }

        void update(const M_1& z)
        {
            const M_M S = H_ * P_ * H_.transpose() + R_; // find the evolution covariance using linearized measurement evolution functions and measurement noise
            const N_M K = P_ * H_.transpose() * S.llt().solve(M_M::Identity()); // cholesky decomp to avoid inversing computation overhead
            
            x_ = x_ + K * (z - (H_ * x_));
            P_ = (N_N::Identity() - K * H_) * P_;
        }

    };

    /**
    * @brief Extended Kalman Filter (EKF) with automatic Jacobian computation via autodifferentiation
    * 
    * Uses Eigen for fixed-size matrices and autodiff for computing F and H Jacobians.
    * No dynamic memory allocation - all matrix sizes are known at compile time.
    * 
    * Template parameters:
    * - T: scalar type (float or double, must support autodiff dual numbers)
    * - STATES: number of states
    * - MEASUREMENTS: number of measurements
    * 
    * State transition function f: R^STATES -> R^STATES (nonlinear)
    * Measurement function h: R^STATES -> R^MEASUREMENTS (nonlinear)
    * 
    * User provides function pointers for each row of f and h:
    * - f[i]: computes x_dot[i] = f[i](x) where x is the full state vector
    * - h[i]: computes z_predicted[i] = h[i](x) where x is the full state vector
    */
    template <typename T, std::size_t STATES, std::size_t MEASUREMENTS, std::size_t INPUTS>
    class ekf {
    public:
        using N_N = Eigen::Matrix<T, STATES, STATES>;
        using N_1 = Eigen::Matrix<T, STATES, 1>; 
        using M_1 = Eigen::Matrix<T, MEASUREMENTS, 1>;
        using U_1 = Eigen::Matrix<T, INPUTS, 1>;
        using M_M = Eigen::Matrix<T, MEASUREMENTS, MEASUREMENTS>; 
        using M_N = Eigen::Matrix<T, MEASUREMENTS, STATES>;
        using N_M = Eigen::Matrix<T, STATES, MEASUREMENTS>;
        using N_U = Eigen::Matrix<T, STATES, INPUTS>; 

        /*
        For EKF
        the autodiff library expects a vector of states and control inputs that the function uses
        this complicates the creation of our B and F jacobian as they need to be split from the computed jacobian
        from autodiff cpp. For this we assume that the vector of the user provided function is of the size N + U...
        Knowing this we can split the output jacobian based on the index
        */
        using state_mtx = Eigen::Matrix<autodiff::dual, STATES, 1>;
        using state_inp_mtx = Eigen::Matrix<autodiff::dual, STATES + INPUTS, 1>;
        using measurement_arr = Eigen::Matrix<autodiff::dual, MEASUREMENTS, 1>;
        using StateFunction = autodiff::dual(*)(const state_inp_mtx&);
        using MeasurementFunction = autodiff::dual(*)(const state_mtx&);

        /**
        * @brief Constructor for EKF
        * 
        * @param f Array of STATES functions -- for every state we have one function 
        * @param h Array of MEASUREMENTS functions -- for every measurement we have one function 
        * @param Q Process noise covariance (NxN) - how much we trust the model
        * @param R Measurement noise covariance (MxM) - how much we trust measurements
        * @param x0 Initial state estimate (Nx1)
        * @param P0 Initial state covariance estimate (NxN)
        */
        ekf(const std::array<StateFunction, STATES>& f,
            const std::array<MeasurementFunction, MEASUREMENTS>& h,
            const N_N& Q,
            const M_M& R,
            const N_1& x0,
            const N_N& P0)
        : f_(f),
          h_(h),
          Q_(Q),
          P_(P0),
          F_(N_N::Identity()),
          R_(R),
          H_(M_N::Zero()),
          x_(x0),
          y_(M_1::Zero())
        {
            detail::assert_covariance_constraints(P0, Q, R);
        }

        const N_1& state() const { return x_; }
        const N_N& covariance() const { return P_; }

        // estimate functions
        const N_1 estimated_states (U_1 &inputs, M_1 &measurements) 
        {
            predict(inputs);
            update(measurements);
            return x_;
        }

    private:
        std::array<StateFunction, STATES> f_;
        std::array<MeasurementFunction, MEASUREMENTS> h_;
        N_N Q_, P_, F_;
        M_M R_;
        M_N H_;
        N_1 x_;
        M_1 y_;

        /**
        * @brief Compute Jacobian H = dh/dx using automatic differentiation
        * 
        * For each measurement function h_i, computes the i-th row of the Jacobian matrix.
        * H(i,j) = dh_i/dx_j using autodiff's built-in jacobian function.
        */

        void compute_F_eval_states(const U_1& u)
        {
            state_inp_mtx base;
            for (std::size_t j = 0; j < STATES; ++j)
                base(static_cast<Eigen::Index>(j)) = autodiff::dual(x_(static_cast<Eigen::Index>(j)));

            for (std::size_t k = 0; k < INPUTS; ++k)
                base(static_cast<Eigen::Index>(STATES + k)) = autodiff::dual(u(static_cast<Eigen::Index>(k)));

            /*
            lambda function developed to wrap a single function call around the array of functions in f_. This creates a vector valued function of the 
            state functions where the output is a an array of vectors evaluated at the scalar input. In our case it is an array of scalar inputs .. p 
            */
            auto f_vec = [&](const state_inp_mtx& p) {
                state_mtx out;
                for (std::size_t i = 0; i < STATES; ++i)
                    out(static_cast<Eigen::Index>(i)) = f_[i](p); // places each 
                return out;
            };

            state_inp_mtx p = base;
            state_mtx x_eval_dual;
            Eigen::Matrix<T, STATES, STATES + INPUTS> jac;
            autodiff::jacobian(f_vec, autodiff::wrt(p), autodiff::at(p), x_eval_dual, jac);

            // throwing away partials with respect to inputs as we do not need them 
            F_ = jac.template leftCols<STATES>();
            for (std::size_t i = 0; i < STATES; ++i)
                x_(static_cast<Eigen::Index>(i)) = static_cast<T>(autodiff::val(x_eval_dual(static_cast<Eigen::Index>(i))));
        }

        void compute_H_eval_y(const M_1& z)
        {
            state_mtx base;
            for (std::size_t j = 0; j < STATES; ++j)
                base(static_cast<Eigen::Index>(j)) = autodiff::dual(x_(static_cast<Eigen::Index>(j)));
            
            /*
            lambda function developed to wrap a single function call around the array of functions in h_. This creates a vector valued function of the 
            state functions where the output is a an array of vectors evaluated at the scalar input. In our case it is an array of scalar inputs .. p 
            */
            auto h_vec = [&](const state_mtx& p) {
                measurement_arr out;
                for (std::size_t i = 0; i < MEASUREMENTS; ++i)
                    out(static_cast<Eigen::Index>(i)) = h_[i](p);
                return out;
            };

            state_mtx p = base;
            measurement_arr z_eval_dual;
            autodiff::jacobian(h_vec, autodiff::wrt(p), autodiff::at(p), z_eval_dual, H_);

            for (std::size_t i = 0; i < MEASUREMENTS; ++i)
                y_(static_cast<Eigen::Index>(i)) = z(static_cast<Eigen::Index>(i)) - static_cast<T>(autodiff::val(z_eval_dual(static_cast<Eigen::Index>(i))));
        }

        /**
        * @brief Predict step: compute Jacobian F, then update state and covariance
        * 
        * This is called when there is no measurement, only state evolution.
        * Computes F = df/dx at current state, then:
        *   x = f(x)
        *   P = F*P*F^T + Q
        */
        void predict(U_1 &u) {
            compute_F_eval_states(u); // already evaluates the state vector at the prev instance... that is part of autodiff
            P_ = F_ * P_ * F_.transpose() + Q_;
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
            // the jacobian function works by providing you the jacobian (partial derrivatives of a function with respect to each variable) and at the same time evaluating 
            compute_H_eval_y(z);
            const M_M S = H_ * P_ * H_.transpose() + R_; // find the evolution covariance using linearized measurement evolution functions and measurement noise
            const N_M K = P_ * H_.transpose() * S.llt().solve(M_M::Identity()); // cholesky decomp to avoid inversing computation overhead
            
            x_ = x_ + K * y_;
            P_ = (N_N::Identity() - K * H_) * P_;
        }
    };
}
