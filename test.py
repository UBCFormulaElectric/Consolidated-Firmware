import serial
import time

# Configure the serial port
# Replace 'COM3' with your specific port if different
# Adjust baudrate to match your device's settings
ser = serial.Serial(
    port='COM3',
    baudrate=57600,  # Common baud rates: 9600, 19200, 38400, 115200
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1     # Read timeout in seconds
)

print(f"Opening serial port {ser.name}...")

try:
    if ser.is_open:
        print("Serial port opened successfully.")
        while True:
            # Read a line from the serial port
            # ser.readline() reads until a newline character (\n) is received
            # or until the timeout is reached.
            # .decode('utf-8') converts bytes to a string.
            line = ser.readline().decode('utf-8').strip()
            if line:
                print(f"Received: {line}")
            time.sleep(0.1) # Small delay to prevent busy-waiting
    else:
        print("Failed to open serial port.")

except serial.SerialException as e:
    print(f"Serial port error: {e}")
except KeyboardInterrupt:
    print("Program terminated by user.")
finally:
    if ser.is_open:
        ser.close()
        print("Serial port closed.")
