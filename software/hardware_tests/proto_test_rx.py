import serial
import argparse
import simple_pb2


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--com', default="/dev/ttyUSB0", help='The desired comport to open')
    args = parser.parse_args()

    # Try to open the serial port with the default baud rate.
    with serial.Serial(args.com, 57600, timeout=1) as ser:

        running = True

        while running:

                # Await a reply.
                # First the length of the message.
                # length_bytes = ser.read(1)
                # length = int.from_bytes(length_bytes, byteorder="little")
                # if 0 < length:
                #     # Next the actual data
                    packet_size = int.from_bytes(ser.read(1), byteorder="little")
                    # # Check if we have received all bytes.
                    print(packet_size)
                    
                    # if length == len(bytes):
                    reply = simple_pb2.SimpleMessage()
                    reply.ParseFromString(ser.read(packet_size))

                    print(reply.lucky_number)