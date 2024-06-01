"""
Signal util and uart 'handling' class 

Reads in the bytes coming in over UART and emits 
them to the flask app appropriately
TODO: Implement proper error handling for things like no data available.
"""

import os
import sys

import flask_socketio
from . import telem_pb2

import serial

sys.path.insert(
    0,
    os.path.abspath(
        os.path.join(
            os.path.dirname(__file__), "../../../../../scripts/code_generation/"
        )
    ),
)
bus_path = os.path.abspath(
    os.path.join(os.path.dirname(__file__), "../../../../../can_bus/quadruna")
)
from jsoncan.src.json_parsing.json_can_parsing import JsonCanParser
from jsoncan.src.can_database import CanDatabase

ser = serial.Serial(
    "/dev/ttyUSB0", baudrate=57600, timeout=1
)  # TODO: generalize the serial port (only linux as this file structure)
can_db = JsonCanParser(bus_path).make_database()
ser.reset_input_buffer()
ser.reset_output_buffer()

available_signals = {}
client_signals = {}

s = set()
s.add(255)
s.add(165)
s.add(253)

def read_messages():
    """
    Read messages coming in through the serial port, decode them, unpack them and then emit them to the socket
    """
    last_bit = 0

    try:
        while True:
            packet_size = int.from_bytes(ser.read(1), byteorder="little")
            print(packet_size)
            if packet_size in s:
                continue

            if (
                last_bit == 0 and packet_size != 0
            ):  # the size will be different due to 0 not often being include

                # Read in UART message and parse the protobuf
                bytes_read = ser.read(packet_size)
                message_received = telem_pb2.TelemMessage()
                message_received.ParseFromString(bytes_read)

                # Make data array out of ints
                data_array = make_bytes(message_received)

                # Unpack the data and add the id and meta data
                signal_list = can_db.unpack(message_received.can_id, data_array)

                for single_signal in signal_list:

                    # Add the time stamp
                    single_signal["timestamp"] = message_received.time_stamp
                    signal_name = single_signal["name"]

                    # Update the list of availble signals and add it to client signals
                    if signal_name not in available_signals:
                        available_signals[signal_name] = True
                        client_signals[signal_name] = []

                    # Emit the message
                    flask_socketio.emit("signal_response", single_signal)

            else:
                last_bit = packet_size

    except KeyboardInterrupt:
        ser.close()
    except Exception as e:
        print("Error receiving/sending proto msg:", e)
    finally:
        ser.close()


def get_available_signals(self):
    """
    Emit a list of the availble signals
    """
    flask_socketio.emit(available_signals.copy())


def connect_client_to_signal(self, client_id, signal_name):
    """
    Append the client ID to a specific signal.
    :param client_id: The ID of the client to connect.
    :param signal_name: The name of the signal to connect to.
    """
    if signal_name in list(client_signals.keys()):
        if client_id not in client_id[signal_name]:
            client_signals[signal_name].append(client_id)


def connect_client_to_available_signals(client_id):
    """
    Connect the client to all available signals.
    :param client_id: The ID of the client to connect.
    """
    for signal_name in list(client_signals.keys()):
        if client_id not in client_signals[signal_name]:
            client_signals[signal_name].append(client_id)


def disconnect_client_from_signal(client_id, signal_name):
    """
    Remove a specific signal from a specific client.
    :param client_id: The ID of the client to disconnect.
    :param signal_name: The name of the signal to disconnect from.
    """
    if signal_name in list(client_signals.keys()):
        if client_id in client_signals[signal_name]:
            client_signals[signal_name].remove(client_id)


def disconnect_client(client_id):
    """
    Remove a specific client from all signals.
    :param client_id: The ID of the client to disconnect.
    """
    for signal_name in list(client_signals.keys()):
        if client_id in client_signals[signal_name]:
            client_signals[signal_name].remove(client_id)


def make_bytes(message):
    """
    Make the byte array out of the message array.
    :param message: The message to convert to bytes.
    :return: Bytearray of the message.
    """
    return bytearray(
        [
            message.message_0,
            message.message_1,
            message.message_2,
            message.message_3,
            message.message_4,
            message.message_5,
            message.message_6,
            message.message_7,
        ]
    )
