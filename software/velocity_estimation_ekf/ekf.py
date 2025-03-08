import numpy as np

# Discrete Time Extended Kalman Filter:
# https://en.wikipedia.org/wiki/Extended_Kalman_filter

class EKF:
    def __init__(self, dim_x, dim_z, x, P, f, h, F_jacobian, H_jacobian):
        self.dim_x = dim_x
        self.dim_z = dim_z
        self.f = f  # process model prediction function
        self.h = h  # measurement model prediction function
        self.F_jacobian = F_jacobian  # function to compute process model Jacobian
        self.H_jacobian = H_jacobian  # function to compute measurement model Jacobian
        self.x = x
        self.P = P

    def predict(self, u, dt, Q):
        """
        Prediction step in the EKF, calculates the predicted state and
        predicted covariance based on previous state and covariance

        Args:
            u (np.array): control inputs
            dt (float): time step
            Q (np.array): process noise covariance matrix
        """
        # Compute the predicted state using the process model
        self.x = self.f(self.x, u, dt)
        # Compute the Jacobian of the process model at the current state
        F_k = self.F_jacobian(self.x, u, dt)
        # Update the covariance prediction
        self.P = F_k @ self.P @ F_k.T + Q

    def update(self, z_k, R):
        """
        Update step in the EKF, estimates the current state using
        predicted measurments and state, calculated measurements, 
        and a computed a kalman gain. Estimates the current covariance
        using the predicted covariance and measurement covariance

        Args:
            z_k (np.array): calculated measurements
            R (np.array): measurement noise covariance
        """
        
        # Compute the predicted measurement and its Jacobian
        z_pred = self.h(self.x)
        H_k = self.H_jacobian(self.x)
        # Compute measurement covariance
        S_k = H_k @ self.P @ H_k.T + R
        # Compute Kalman gain (note the transpose on H_k)
        K_k = self.P @ H_k.T @ np.linalg.inv(S_k)
        # Innovation (measurement residual)
        y_k = z_k - z_pred
        # Update state estimate
        self.x = self.x + K_k @ y_k
        # Update covariance estimate
        self.P = self.P - K_k @ H_k @ self.P


    def step(self, u, z, dt, Q, R):
        self.predict(u, dt, Q)
        self.update(z, R)
        return self.x, self.P
