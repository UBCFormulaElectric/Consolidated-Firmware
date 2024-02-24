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
crit = chimera.CRIT(ports[0].device)

# Note: All testable CRIT pin names are listed in Consolidated-Firmware/chimera/board_protos/CRIT.proto

####### WRITE YOUR SCRIPT HERE

for i in range(4):
    crit.gpio_write("LED", True)
    print("write true", crit.gpio_read("LED"))
    time.sleep(0.1)
    crit.gpio_write("LED", False)
    print("write false", crit.gpio_read("LED"))
    time.sleep(0.1)


# Read ADC value: VBAT_SENSE
print("read adc", crit.adc_read("REGEN_3V3"))

#######
