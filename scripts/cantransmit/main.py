"""
main.py
-----------------
Script used to send torque commands.

"""
import argparse
import can
import time 
import pandas as pd 
import numpy as np

datafilename = 'data/Endurance_10kWMotor.csv'

def parse_args():
    parser = argparse.ArgumentParser(description='Send CAN messages from a CSV file and receive pedal percentage.')
    parser.add_argument('filepath', help='Path to the CSV file containing torque values')
    return parser.parse_args()

def load_torques(datafilename):
    """Load torques from csv file."""
    df = pd.read_csv(datafilename)
    torques = df['T_m'].tolist()
    return torques

def initialize_can_bus():
    """Initialize CAN bus."""
    return can.interface.Bus(bustype='socketcan', channel='can0', bitrate=500000)

def send_can_message(value, msg_id, bus):
    """Send torque command over CAN bus."""
    bus.send(can.Message(arbitration_id=msg_id, data=[value], is_extended_id=False))

if __name__ == "__main__":
    args = parse_args()
    filepath = args.filepath
    torques = load_torques(datafilename)
    bus = initialize_can_bus()
    time_error_s = 0.0
    msg_id = 35
    loop_period_s = 0.01
    start = time.time()
    try: 
        print("Sending CAN messages...")
        for i, torque in enumerate(torques):
            send_can_message(np.uint16(0.1*torque), msg_id, bus)
            time.sleep(loop_period_s + time_error_s) 
            end = time.time()
            delta_t = end - start
            print(f"Loop time delta: {delta_t} seconds")
            start = end
            time_error_s = loop_period_s - (delta_t)
    except KeyboardInterrupt:
        print("Keyboard Interrupt, shutting down...")
        bus.shutdown()
