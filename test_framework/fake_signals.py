import cantools
import can
import os
import json
import argparse
import asyncio

def _get_signal_id(mock_signal_lut, signal_name):
    """
    Given a signal name, return the corresponding mock signal ID
    """
    for mock_signal in mock_signal_lut:
        if mock_signal['signal_name'] == signal_name:
            return mock_signal['signal_id']

    raise Exception('Could not find signal in the list of availabe mock signals')

def _get_message_by_signal_name(dbc, signal_name):
    """
    Given a signal name, find the message in the DBC containing the signal
    """
    for message in dbc.messages:
        for signal in message.signals:
            if signal_name == signal.name:      
                return message

    raise Exception(
            'Could not find any message containing signal <{}> in the DBC'.format(signal_name))

def _get_expected_fault_signal(is_fault_above_threshold, is_fault_active_high):
    expected_fault_signal = 0 # Placeholder value
    if is_fault_above_threshold == True:
        if value > fault_threshold:
            # Expecting a fault
            expected_fault_signal = 1 if is_fault_active_high == True else 0
        else:
            # Not expecting a fault
            expected_fault_signal = 0 if is_fault_active_high == True else 1
    else:
        if value > fault_threshold:
            # Not expecting a fault
            expected_fault_signal = 0 if is_fault_active_high == True else 1
        else:
            # Expecting a fault
            expected_fault_signal = 1 if is_fault_active_high == True else 0

    return expected_fault_signal

def dbc_init(dbc):
    return cantools.database.load_file(dbc)

def can_init(can_bustype, can_bitrate, can_channel):
    """
    can_bustype: String
    can_bitrate: Integer
    can_channel: String
    """
    os.system("sudo ip link set {} type can bitrate {}".format(can_channel, can_bitrate))
    os.system("sudo ip link set up {}".format(can_channel))
    return can.interface.Bus(bustype=can_bustype, channel=can_channel, bitrate=can_bitrate)

async def common_fake_signal_cb(bus, dbc, fake_signal):
    while True:
        rx_msg = await fake_signal.reader.get_message()

        # Is the CAN ID correct?
        if rx_msg.arbitration_id == dbc.get_message_by_name('SIGNAL_INJECTION').frame_id:

            decoded_rx_msg = dbc.decode_message(rx_msg.arbitration_id,
                                                rx_msg.data)

            # Is payload well-formed?
            if decoded_rx_msg['Enable_Override'] == 1 and \
               decoded_rx_msg['Signal_ID'] == fake_signal.id:

                # Invoke the callback function unique to this signal
                await fake_signal.cb(bus, dbc, decoded_rx_msg['Value'])

            # Delete this
            # Invoke the callback function unique to this signal
            # fake_signal.cb(bus, dbc, decoded_rx_msg['Value'])


class MockSignal:
    """
    Lightweight class to bundle up name and reader
    """
    def __init__(self, name, cb, reader, ID):
        self.name = name
        self.cb = cb
        self.reader = reader
        self.id = ID

async def SetWheelSpeedCb(bus, dbc, wheel_speed):

    fault_outputs_msg = dbc.get_message_by_name('FAULT_OUTPUTS')

    # Default value for the first call:
    fault_outputs_payload = fault_outputs_msg.encode({'MotorShutdown': 0, 'BatteryShutdown': 0})

    if wheel_speed > 15:
        fault_outputs_payload = fault_outputs_msg.encode({'MotorShutdown': 1, 'BatteryShutdown': 0})
    else:
        fault_outputs_payload = fault_outputs_msg.encode({'MotorShutdown': 0, 'BatteryShutdown': 0})

    # For testing, let's send periodic message for about 1 sec at 5Hz or else hostpc.py will freeze
    tx_msg = can.Message(
        arbitration_id = fault_outputs_msg.frame_id,
        data = fault_outputs_payload,
        is_extended_id = False)
    task = bus.send_periodic(tx_msg, 0.2)

    task.start()
    await asyncio.sleep(1)
    task.stop()
    

async def SetTirePressureCb(bus, dbc, tire_pressure):
    print('tire')

async def SetBatteryVoltageCb(bus, dbc, voltage):

    fault_outputs_msg = dbc.get_message_by_name('FAULT_OUTPUTS')

    # Default value for the first call:
    fault_outputs_payload = fault_outputs_msg.encode({'BatteryShutdown': 0, 'MotorShutdown': 0})

    if voltage > 35:
        fault_outputs_payload = fault_outputs_msg.encode({'BatteryShutdown': 1, 'MotorShutdown': 0})
    else:
        fault_outputs_payload = fault_outputs_msg.encode({'BatteryShutdown': 0, 'MotorShutdown': 0})

    # For testing, let's send periodic message for about 1 sec at 5Hz or else hostpc.py will freeze
    tx_msg = can.Message(
        arbitration_id = fault_outputs_msg.frame_id,
        data = fault_outputs_payload,
        is_extended_id = False)
    task = bus.send_periodic(tx_msg, 0.2)

    task.start()
    await asyncio.sleep(1)
    task.stop()

async def fake_responses(bus, dbc, mock_signal_lut):
    """
    The fake signal callbacks are hard-coded
    """
    mock_signals = [('SetWheelSpeed', SetWheelSpeedCb),
                    ('SetTirePressure', SetTirePressureCb),
                    ('SetBatteryVoltage', SetBatteryVoltageCb)]

    # Append an unique async reader and unique ID to each signal
    mock_signals = [MockSignal(name,
                               cb,
                               can.AsyncBufferedReader(),
                              _get_signal_id(mock_signal_lut, name))
                    for name, cb in mock_signals]
    
    # Listeners to be registerd with can.Notifier
    listeners = [signal.reader for signal in mock_signals]

    # Create can.Notifier with an explicit event loop to use for scheduling callbacks 
    loop = asyncio.get_event_loop()
    notifier = can.Notifier(bus, listeners, loop=loop)

    await asyncio.gather(
            *(common_fake_signal_cb(bus, dbc, signal) for signal in mock_signals))

def run_fake(bus, dbc, mock_signal_lut):
    loop = asyncio.get_event_loop()
    loop.run_until_complete(fake_responses(bus, dbc, mock_signal_lut))
    loop.close()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--test_json')
    parser.add_argument('--signal_json')
    parser.add_argument('--dbc')
    args = parser.parse_args()

    with open(args.test_json) as f:
      tests = json.load(f)['Tests']

    with open(args.signal_json) as f:
        mock_signal_lut = json.load(f)['Mock_Signals']

    bus = can_init('socketcan', 500000, 'vcan0')
    dbc = dbc_init(args.dbc)

    run_fake(bus, dbc, mock_signal_lut)

if __name__ == '__main__':
    main()
