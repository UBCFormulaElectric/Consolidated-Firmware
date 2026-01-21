#include "app_math.hpp"

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
    void measurement(T sensor_val); 
    void compute_covariance();
    void find_jacobians()
    {
        /*
        Populate F and H matrix if not provided jacobians
        */
    };


}