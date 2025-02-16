import time
import can
import chimera_v2
import matplotlib.pyplot as plt

# Config.
DATA_POINTS_TO_RENDER = 100
FRAME_INTERVAL_SECS = 0.05
MSG_NAME = "VC_ImuAngularData"
SIG_NAME = "VC_ImuAngularVelocityYaw"
DBC_PATH = "../../../can_bus/dbcs/quadruna.dbc"
CAN_BUS = can.Bus(interface="vector", app_name="CANalyzer", channel=0, bitrate=1000000)

# Init.
can_device = chimera_v2.CanDevice(DBC_PATH, CAN_BUS)
timestamps, data = [], []
start = time.time()

while True:
    # Add data.
    data.append(can_device.get(msg_name=MSG_NAME, signal_name=SIG_NAME))
    timestamps.append(time.time() - start)

    # Cut off early data points.
    data, timestamps = (
        data[-DATA_POINTS_TO_RENDER:],
        timestamps[-DATA_POINTS_TO_RENDER:],
    )

    # Plot.
    plt.clf()
    plt.plot(timestamps, data)
    plt.pause(FRAME_INTERVAL_SECS)
