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
rsm = chimera.RSM(ports[0].device)

# Note: All testable RSM pin names are listed in Consolidated-Firmware/chimera/board_protos/RSM.proto

####### WRITE YOUR SCRIPT HERE

for i in range(4):
    rsm.gpio_write("LED", True)
    print("write true", rsm.gpio_read("LED"))
    time.sleep(0.1)
    rsm.gpio_write("LED", False)
    print("write false", rsm.gpio_read("LED"))
    time.sleep(0.1)


# Read ADC value: VBAT_SENSE
print("read adc", rsm.adc_read("LC3_OUT"))

#######
