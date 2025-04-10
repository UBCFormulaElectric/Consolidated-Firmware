import can

CAN_BUS = can.Bus(interface="vector", app_name="CANalyzer", channel=0, bitrate=1000000)

for msg in CAN_BUS:
    print(msg.data)