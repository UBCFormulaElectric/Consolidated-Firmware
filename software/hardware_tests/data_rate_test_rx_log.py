import serial
import csv
from datetime import datetime
from pathlib import Path


def timestamp():
    return datetime.now().strftime("%Y%m%d%H%M%S")


# csv_file_path = f'rx_data_{timestamp()}.csv'

csv_file_path = Path("rx_data_" + timestamp() + ".csv")


def line_to_csv(data, csv_file_path):
    with open(csv_file_path, "a", newline="") as csv_file:
        csv_writer = csv.writer(csv_file)
        csv_writer.writerow([data])


def receive_data():
    ser = serial.Serial("/dev/ttyUSB0", baudrate=57600, stopbits=1, timeout=100)

    try:
        while True:
            line = ser.readline().decode().strip()
            line_to_csv(line, csv_file_path)
            print(line)

    except KeyboardInterrupt:
        # Close the serial port on Ctrl+C
        ser.close()


if __name__ == "__main__":

    receive_data()
