import serial
import msgpack

def receive_data():
    ser = serial.Serial('/dev/ttyUSB0', baudrate=57600, stopbits=1, timeout=100) 

    tick_count = 0

    try:
        while True:
            packed_line = ser.readline(16).decode().strip() 
            unpacked_line = msgpack.unpackb(packed_line, raw=False)
            print(unpacked_line)
            tick_count+=1

    except KeyboardInterrupt:
        # Close the serial port on Ctrl+C
        ser.close()
        print(tick_count)

if __name__ == "__main__":

    receive_data()