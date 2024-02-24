"""
Example of how to use the serial debugger.
"""

import serial
import serial.tools.list_ports
import time
import chimera


# List available COM ports.
ports = list(serial.tools.list_ports.comports())
print("Found COM ports:")
for port in ports:
    print(port.device)

# Transmit data.
vc = chimera.VC(ports[0].device)

# Note: All testable VC pin names are listed in Consolidated-Firmware/chimera/board_protos/VC.proto

####### WRITE YOUR SCRIPT HERE

for i in range(4):
    vc.gpio_write("LED", True)
    print("write true", vc.gpio_read("LED"))
    time.sleep(0.1)
    vc.gpio_write("LED", False)
    print("write false", vc.gpio_read("LED"))
    time.sleep(0.1)


# Read ADC value: VBAT_SENSE
print("read adc", vc.adc_read("VBAT_SENSE"))

#######
