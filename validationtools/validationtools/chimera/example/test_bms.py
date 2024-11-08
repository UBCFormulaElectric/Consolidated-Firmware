"""
Example of how to use the serial debugger.
"""

import serial
import serial.tools.list_ports
import time
import validationtools.chimera as chimera


# List available COM ports.
ports = list(serial.tools.list_ports.comports())
print("Found COM ports:")
for port in ports:
    print(port.device)

# Transmit data.
bms = chimera.BMS(ports[2].device)

# Note: All testable BMS pin names are listed in Consolidated-Firmware/chimera/proto/BMS.proto

####### WRITE YOUR SCRIPT HERE

for i in range(4):
    print("here")
    bms.gpio_write("LED", True)
    print("write true", fsm.gpio_read("LED"))
    time.sleep(0.1)
    bms.gpio_write("LED", False)
    print("write false", fsm.gpio_read("LED"))
    time.sleep(0.1)


# Read ADC value: VBAT_SENSE
# print("read adc", fsm.adc_read("APPS2_3V3"))

#######
