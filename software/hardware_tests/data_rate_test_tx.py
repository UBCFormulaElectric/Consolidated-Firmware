import serial
import time


def generate_data_rate(target_data_rate_kbps):
    ser = serial.Serial("/dev/ttyUSB0", baudrate=57600, stopbits=1, timeout=100)

    target_data_rate_bps = target_data_rate_kbps * 1000
    delay = 80 / target_data_rate_bps
    tick_count = 0

    try:
        while True:
            msg = str(int(tick_count)) + "\n"
            ser.write(msg)  # 10 characters at 8 bits each
            # 80 bits

            time.sleep(delay)
            tick_count += 1

    except KeyboardInterrupt:
        # Close the serial port on Ctrl+C
        ser.close()
        print(tick_count)


if __name__ == "__main__":

    target_data_rate_kbps = 10

    generate_data_rate(target_data_rate_kbps)
