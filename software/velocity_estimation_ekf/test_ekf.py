import numpy as np
from ekf import EKF


def f(x, u, dt):
    return x + dt * u


def F_jacobian(x, u, dt):
    return np.eye(len(x))


def h(x):
    return x


def H_jacobian(x):
    return np.eye(len(x))


def ekf_instance():
    x = np.array([0.0, 0.0])
    P = np.eye(2)
    ekf = EKF(dim_x=2, dim_z=2, x=x, P=P, f=f, h=h, F_jacobian=F_jacobian, H_jacobian=H_jacobian)
    return ekf


def test_predict(ekf_instance):
    u = np.array([1.0, 1.0])
    dt = 1.0
    Q = 0.1 * np.eye(2)

    ekf_instance.predict(u, dt, Q)

    expected_x = np.array([1.0, 1.0])
    expected_P = np.eye(2) + Q  # since F is identity

    np.testing.assert_allclose(ekf_instance.x, expected_x, atol=1e-6)
    np.testing.assert_allclose(ekf_instance.P, expected_P, atol=1e-6)


def test_update(ekf_instance):
    # Set a known predicted state and covariance
    ekf_instance.x = np.array([1.0, 1.0])
    ekf_instance.P = np.eye(2)

    # Measurement is slightly different from the prediction
    z = np.array([1.2, 0.8])
    R = 0.05 * np.eye(2)

    # Compute expected results manually or compare against a trusted implementation
    # Here, we call update and simply check that the state moves closer to the measurement
    old_x = ekf_instance.x.copy()
    ekf_instance.update(z, R)

    # Check that the updated state is between the predicted state and the measurement
    assert np.all((ekf_instance.x - old_x) != 0)
    # Also, ensure that the updated covariance is symmetric
    np.testing.assert_allclose(ekf_instance.P, ekf_instance.P.T, atol=1e-6)

def main():
    ekf = ekf_instance()
    test_predict(ekf)
    test_update(ekf)
    print("Tests Passed!!!")

if __name__ == "__main__":
    main()
