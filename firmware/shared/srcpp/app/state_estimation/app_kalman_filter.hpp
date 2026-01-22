#include "apPmath.hpp"

/**
* @brief The kalman filter class will serve as a simplification of the extended kalman filter. The idea is that for linear systems (or systems that can be easily linearized) 
* we use the kalman filter rather than the EKF.
* @note this implementation has been restricted to SISO systems
*/
template<typename T, size_t N>
class kf {
public:

    constexpr explicit kf(const matrix<T,N,N>& F, const matrix<T,N,1>& B, const matrix<T,N,N>& Q, 
                          const matrix<T,1,N>& H, T R, const matrix<T,N,1>& x0, const matrix<T,N,N>& P0): 
        F(F), B(B), Q(Q), H(H), R(R), x(x0), P(P0) {}

    void predict(T u) {
        x = F * x + (B * u);
        P = F * P * (~F) + Q;
    }

    void update(T z) { // keeping z a scalar for SISO 
        const T y = z - (H * x)(0,0);
        const T S = (H * P * (~H))(0,0) + R;
        const matrix<T,N,1> K = (P * (~H)) / S;

        x = x + K * y;
        P = (matrix<T,N,N>::identity() - K * H) * P;
    }

    const matrix<T,N,1>& x_vec() const { return x; }
    const matrix<T,N,N>& P_mat() const { return P; }

private:
    matrix<T,N,N> F, Q;
    matrix<T,N,1> B;
    matrix<T,1,N> H;
    T R;

    matrix<T,N,1> x{};
    matrix<T,N,N> P{};
};

template<typename T>
class  ekf{

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