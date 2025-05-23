import numpy as np
import matplotlib.pyplot as plt
import math
from ekf import EKF

# Velocity estimator using EKF:
# http://zotero.org/groups/5809911/vehicle_controls_2024/items/ZKJWDKCJ/reader

def predict_velocity(x, u, dt):
    """
    Predicts the current velocity given the previous 
    velocity, acceleration, and yaw rate 
    (pg. 39)

    Args:
        x (np.array): previous velocity (vx, vy)
        u (np.array): control inputs (ax, ay, r)
        dt (float): time step

    Returns:
        np.array: predicted current velocity
    """
    x_pred = np.array(x.shape)

    x_pred[0] = x[0] + dt * (u[0] + x[1] * u[2])
    x_pred[1] = x[1] + dt * (u[1] + x[0] * u[2])

    return x_pred


def F_jacobian(x, u, dt):
    """
    Computes the jacobian of the prediction function
    does not use previous velocity, uses yaw rate
    from control inputs: u[2] 
    (pg. 40)

    Args:
        x (np.array): previous velocity (vx, vy)
        u (np.array): control inputs (ax, ay, r), only uses r
        dt (float): time step

    Returns:
        np.array: jacobian matrix of the prediction function
    """
    F_k = np.ones((len(x), len(x)))

    F_k[0, 1] = dt * u[2]
    F_k[1, 0] = -dt * u[2]

    return F_k


def predict_measurements(x):
    """
    Calculate the predicted measurements, this function just
    returns the predicted state as there is no transformation
    needed to convert it into the desired measurements 
    (pg. 39)

    Args:
        x (np.array): previous state (vx, vy)

    Returns:
        np.array: predicted measurements
    """
    return x


def H_jacobian(x, u, dt):
    """
    Computes the jacobian of the predicted measurement function,
    because it is very simple it just returns an identity matrix
    (pg. 40)

    Args:
        x (np.array): previous state (vx, vy)
        u (np.array): control inputs (ax, ay, r)
        dt (float): time step

    Returns:
        np.array: jacobian matrix of the predicted measurement function
    """
    return np.eye(len(x))

def rpm_to_mps(rpm):
    print("hi")

def calculate_measurements(
    rpm_rr,
    rpm_rl,
    rpm_fr,
    rpm_fl,
    r,
    a,
    tf,
    l2,
    l1
):
    """
    Calculates velocity based on various measurements including
    wheel speeds, wheel angle, and yaw rate
    (pg. 34-35)

    Args:
        rpm_rr (float): rpm of rear right wheel
        rpm_rl (float): rpm of rear left wheel
        rpm_fr (float): rpm of front right wheel
        rpm_fl (float): rpm of front left wheel
        r (float): yaw rate
        a (float): front wheel angle
        tf (float): track width
        l2 (float): distance from rear axle to cog
        l1 (float): distance from front axle to cog

    Returns:
        np.array: calculated velocity measurements
    """
    v_rr = rpm_to_mps(rpm_rr)
    v_rl = rpm_to_mps(rpm_rl)
    v_fr = rpm_to_mps(rpm_fr)
    v_fl = rpm_to_mps(rpm_fl)

    vx_rr = v_rr + r * tf
    vx_rl = v_rl - r * tf
    vx_fr = v_fr * math.cos(a) + r * tf
    vx_fl = v_fr * math.cos(a) - r * tf

    vy_rr = -r * l2 - v_rr
    vy_rl = -r * l2 - v_rl
    vy_fr = -r * l1 - v_fr * math.sin(a)
    vy_fl = -r * l1 - v_fl * math.sin(a)

    z_k = np.array((2))

    z_k[0] = (vx_rr + vx_rl + vx_fr + vx_fl) / 4.0
    z_k[1] = (vy_rr + vy_rl + vy_fr + vy_fl) / 4.0

    return z_k


def main():
    dim_x = 2
    dim_z = 2
    dt = 0.01

    x = np.zeros((dim_x))
    u = np.zeros((3))
    z_k = np.zeros((dim_x))
    P = np.eye((dim_x, dim_x))

    Q = np.eye((dim_x))
    R = np.eye((dim_x))

    ekf = EKF(
        dim_x=dim_x,
        dim_z=dim_z,
        x=x,
        P=P,
        f=predict_velocity,
        h=predict_measurements,
        F_jacobian=F_jacobian,
        H_jacobian=H_jacobian,
    )

    while(True):
        # update u with new sensor data
        # u = getSensorData()
        # z_k = calculate_measurements()

        x, P = ekf.step(u, z_k, dt, Q, R)


if __name__ == "__main__":
    main()
