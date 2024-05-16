import serial


def receive_data():
    ser = serial.Serial("/dev/ttyUSB0", baudrate=57600, stopbits=1, timeout=100)

    tick_count = 0

    try:
        while True:
            line = ser.readline(12).decode().strip()
            print(line)
            tick_count += 1

    except KeyboardInterrupt:
        # Close the serial port on Ctrl+C
        ser.close()
        print(tick_count)


if __name__ == "__main__":

    receive_data()
