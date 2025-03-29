import serial
import sample_pb2

DEBUG_SIZE_MSG_BUF_SIZE = 1
if __name__ == "__main__":

    # Try to open the serial port with the default baud rate.
    ser = serial.Serial("/dev/ttyUSB0", baudrate=57600, timeout=1)
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    s = set()
    s.add(255)
    s.add(165)
    s.add(0)
    s.add(253)
    # try:
    while True:

        packet_size = int.from_bytes(
            ser.read(DEBUG_SIZE_MSG_BUF_SIZE), byteorder="little"
        )
        if packet_size in s:
            continue
        reply = sample_pb2.TelemMessage()
        bytes = ser.read(packet_size)
        reply.ParseFromString(bytes)

        print(reply.can_id, reply.data, reply.time_stamp)

        # print(ser.read(1))

    # except KeyboardInterrupt:
    #        ser.close()
