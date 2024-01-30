# Imports
import serial
ser = serial.Serial('/dev/ttyUSB0', baudrate=57600, stopbits=1, timeout=100) 

def send_msg(ser, command):
        ser.write(command.encode())

def main():     
    try:
        while True:
            msg = "hello lara\n"
            send_msg(ser,msg)
            print(msg)


    except KeyboardInterrupt:
        # Handle Ctrl+C to exit gracefully
        print("\nScript terminated by user.")

if __name__ == "__main__":
    main()
