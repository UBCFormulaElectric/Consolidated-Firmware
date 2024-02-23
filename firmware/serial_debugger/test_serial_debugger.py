"""
Example of how to use the serial debugger.
"""

import serial
import serial.tools.list_ports
import serial_debugger


# List available COM ports.
ports = list(serial.tools.list_ports.comports())
print("Found COM ports:")
for port in ports:
    print(port.device)

# Transmit data.
vc = serial_debugger.VC(ports[0].device)

# Note: All testable VC pin names are listed in Consolidated-Firmware/serial_debugger/board_protos/VC.proto

####### WRITE YOUR SCRIPT HERE 

# Read LED
print(vc.gpio_read("LED"))
# Write 1 to LED and read again
vc.gpio_write("LED", True)
print(vc.gpio_read("LED"))
# Write 0 to LED and read again
vc.gpio_write("LED", False)
print(vc.gpio_read("LED"))

# Read ADC value: VBAT_SENSE
print(vc.adc_read("VBAT_SENSE"))

#######