#include "app_math.hpp"
#include <cstddef>

namespace app { namespace state_estimation{

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


template<typename T, size_t N,  size_t M, size_t U>
class kf {
public:
    using N_N = matrix<T,N,N>;
    using N_1 = matrix<T,N,1>; 
    using M_1 = matrix<T,M,1>;
    using U_1 = matrix<T,U,1>;
    using M_M = matrix<T,M,M>; 
    using M_N = matrix<T,M,N>;
    using N_M = matrix<T,N,M>;
    using N_U = matrix<T,N,U>; 

    constexpr explicit kf(const N_N& F, const N_U& B, const N_N& Q, 
                          const M_N& H, M_M R, const N_1& x0, const N_N& P0): 
        F(F), B(B), Q(Q), H(H), R(R), x(x0), P(P0) {}

    void predict(U_1 &u) {
        x = F * x + (B * u);
        P = F * P * (~F) + Q;
    }

    void update(M_1 &z) {
        const M_1 y = z - (H * x);
        const M_M S = (H * P * (~H)) + R;
        const N_M K = (P * (~H)) * inverse(S);

        x = x + K * y;
        P = (N_N::identity() - K * H) * P;
    }

    const N_1& x_vec() const { return x; }
    const N_N& P_mat() const { return P; }

private:
    N_N F, Q;
    N_U B;
    M_N H;
    M_M R;

    N_1 x{};
    N_N P{};
};

template<typename T>
class  ekf{
    /**
    Waiting on autodiff library
    */

    constexpr ekf(matrix &H, matrix &H, matrix &Q, matrix &R) // decide what we want for inputs here
    {
        // change as needed
        this.F = P; 
        this.H = H;
        this.Q = Q; 
        this.R = R; 
        find_jacobians()
    };
    T update();

    private:
    // update attributes based on inputs.....
    // if we pass in A and B matrices we need to implement a function to find linearized jacobians
    // if we pass in function ptr or reference then we need to find a way to also find linearized jacobians
    matrix F; 
    matrix H;
    matrix Q; 
    matrix R;
      
    void prediction(); 
    void measurement(T sensoRval); 
    void compute_covariance();
    void find_jacobians()
    {
        /*
        Populate F and H matrix if not provided jacobians
        */
    };


}
} }