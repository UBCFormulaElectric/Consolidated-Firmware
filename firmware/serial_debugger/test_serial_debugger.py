"""
Example of how to use the serial debugger.
"""

import serial
import serial.tools.list_ports
import serial_debugger
import time


# List available COM ports.
ports = list(serial.tools.list_ports.comports())
print("Found COM ports:")
for port in ports:
    print(port.device)

# Transmit data.
vc = serial_debugger.VC(ports[1].device)

# Note: All testable VC pin names are listed in Consolidated-Firmware/serial_debugger/board_protos/VC.proto

####### WRITE YOUR SCRIPT HERE 

# Read LED
# print(vc.gpio_read("LED"))
# Write 1 to LED and read again
# print(vc.gpio_read("LED"))
# Write 0 to LED and read again
# vc.gpio_write("LED", False)
# print(vc.gpio_read("LED"))

for i in range(4):
    vc.gpio_write("LED", True)
    time.sleep(0.1)
    vc.gpio_write("LED", False)
    time.sleep(0.1)


# Read ADC value: VBAT_SENSE
print(vc.adc_read("VBAT_SENSE"))

#######