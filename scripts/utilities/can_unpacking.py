
from ..code_generation.jsoncan.src.can_database import *

vc_can_db = CanDatabase()

# class CanMessage:
#     """
#     Class that represents a can message with a given ID
#     """
#     name = str


class PythonCanUtils:

    """
    Class that handles the unpacking of CAN signals for telemetry.
    """
    def __init__(self):
        self.can_database = vc_can_db # assuming this is already populated with the vc rx and tx and all that jazz
        # self.decoded_values = {}

    def unpack_message(self, message_id, raw_data):
        """
        Take the id and the raw data array of the CAN signal and return a dictionary with unpacked values of that ID
        """
        message = next((msg for msg in self.can_database.msgs if msg.id == message_id), None)
        if message is None:
            print(f"No message found with ID {message_id}")
            return

        # Initialize dictionary to store decoded signal values
        decoded_values = {}

        # Iterate over signals in the message
        for signal in message.signals:
            # Need to get the signals meta data lets call it 
            start_byte, start_bit = divmod(signal.start_bit, 8)
            end_byte, end_bit = divmod(signal.start_bit + signal.bits - 1, 8)
            signal_bytes = raw_data[start_byte:end_byte + 1]
            extracted_bits = 0
            for i, byte in enumerate(signal_bytes):
                extracted_bits |= byte << (8 * i)
            extracted_bits >>= start_bit # TODO: get this so that it can use pack shift or pack left options 

            # Apply scaling and offset to get signal value
            raw_value = extracted_bits * signal.scale + signal.offset

            # Store decoded value
            decoded_values[signal.name] = raw_value

        return decoded_values
    
    def unpack_left(self, input, mask,shift):
        """
        Unpack left
        """
        return (input & mask) << shift
    
    def unpack_right(self,input, mask , shift):
        """
        Unpack right
        """
        return (input & mask ) >> shift
    