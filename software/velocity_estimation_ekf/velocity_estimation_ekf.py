import numpy as np
import matplotlib.pyplot as plt

class EKF:
    def __init__(self, dim_x, dim_z):
        self.dim_x = dim_x
        self.dim_z = dim_z
        self.x = np.zeros((dim_x))
        self.P = np.zeros((dim_x, dim_x))
        self.H_k = np.zeros((dim_x, dim_x))

    # prediction step
    #
    # function f must follow signature f(x, u, dt)
    # x: previous state, u: control variables, dt: timestep
    #
    # function jacobian_f must follow signature jacobian_f(x, u, dt)
    # x: previous state, u: control variables, dt: timestep
    def predict(self, f, jacobian_f, dt, Q, u):
        x_kpred = f(self.x, u, dt)
        F_k = jacobian_f(self.x, u, dt)
        P_kpred = F_k * self.P * F_k.T * Q

        return x_kpred, P_kpred

    # measurement step
    #
    # function h must follow signature h(x)
    # x: previous state
    #
    # function jacobian_h must follow signature jacobian(h)
    # h: measurement function
    def measure(self, P_kpred, h, jacobian_h, calc_meas, meas, R):
        z_kpred = h(meas)
        z_k = calc_meas(meas)
        H_k = jacobian_h(h)
        S_k = H_k * P_kpred * H_k.T + R

        self.H_k = H_k
        self.S_k = S_k
        self.z_k = z_k
        self.z_kpred = z_kpred

        return z_kpred, z_k, S_k, H_k

    # update step
    # 
    # estimates the current state given the measurement and prediction
    def update(self, x_kpred, P_kpred, z_kpred, z_k, S_k, H_k):
        K_k = P_kpred * H_k * np.linalg.inv(S_k)
        y_k = z_k - z_kpred
        x = x_kpred + K_k * y_k
        P = P_kpred - K_k * H_k * P_kpred
        self.x = x
        self.P = P

        return x, P

def predict_vel(x, u, dt):
    x[0] = x[0] + dt * (u[0] + x[1] * u[2])
    x[1] = x[1] + dt * (u[1] + x[0] * u[2])
    
    return x

def predict_vel_jacobian(x, u, dt):
    F_k = np.ones(x.shape)
    F_k[0, 1] = dt * u[2]
    F_k[1, 0] = -dt * u[2]   
    
    return F_k

def calc_meas(v_rr, v_rl, v_fr, v_fl, r, t_f, l):
    V_xrl = v_rr - r * t_f
    V_yrl = v_rr
    V_xrr = v_rl
    V_yrr = v_rl
    V_xfl = v_fr
    V_yfr = v_fr
    V_xfr = v_fl
    V_yfr = v_fl
def main():
    print("hi")


if __name__ == "__main__":
    main()
